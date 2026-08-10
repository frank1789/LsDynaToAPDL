/**
 * @file fields.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Field splitting and numeric conversion for LS-DYNA card lines.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_LSDYNA_FIELDS_HH
#define LSDYNA_TO_APDL_LSDYNA_FIELDS_HH

#include <charconv>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

/**
 * @def LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS
 * @brief Whether std::from_chars accepts a double on this toolchain.
 *
 * @details The build system settles this with a compile probe (see
 * cmake/CxxFeatures.cmake) and passes the answer in. The fallback below only
 * exists for consumers that include this header without that definition — for
 * example through pkg-config — so it has to reach the same verdict on its own.
 *
 * libstdc++ has had floating-point from_chars since GCC 11 and MSVC since
 * 19.24. libc++ gained it late, and on Apple platforms it carries an
 * availability attribute: the declaration exists, `__cpp_lib_to_chars` may be
 * defined, and calling it is still a hard error below the required deployment
 * target ("'from_chars' is unavailable: introduced in macOS 26.0"). Hence the
 * extra deployment-target test rather than a feature-test macro alone.
 */
#ifndef LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS
#  if defined(_LIBCPP_VERSION)
#    if defined(__cpp_lib_to_chars) && __cpp_lib_to_chars >= 201611L && \
        (!defined(__APPLE__) ||                                         \
         (defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) &&     \
          __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 260000))
#      define LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS 1
#    else
#      define LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS 0
#    endif
#  elif defined(__cpp_lib_to_chars) && __cpp_lib_to_chars >= 201611L
#    define LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS 1
#  elif defined(__GNUC__) && __GNUC__ >= 11
#    define LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS 1
#  elif defined(_MSC_VER) && _MSC_VER >= 1924
#    define LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS 1
#  else
#    define LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS 0
#  endif
#endif

#if !LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS
#  include <locale.h>

#  include <array>
#  include <cerrno>
#  include <cstdlib>
#  ifdef __APPLE__
#    include <xlocale.h>
#  endif
#endif

namespace lsdynatoapdl::lsdyna {

/**
 * @brief Fields of one card line.
 */
using Fields = std::vector<std::string_view>;

namespace detail {

/**
 * @brief Whether @p field is a plain decimal number this converter accepts.
 *
 * @details Shared by both implementations of @ref to_double so they agree by
 * construction instead of by carefully duplicated checks.
 *
 * The two disagree on their own: std::from_chars in the general format accepts
 * `inf` and `nan`, strtod accepts those plus a leading '+', leading whitespace
 * and hexadecimal floats. None of them belongs in a coordinate or a thickness,
 * and an infinite coordinate reaching the APDL deck is worse than a rejected
 * card, so all of them are refused here, up front, once.
 *
 * @param field the text to inspect.
 * @return true when the text may be handed to the numeric parser.
 */
[[nodiscard]] inline auto is_plain_decimal(std::string_view field) noexcept
    -> bool {
  if (field.empty()) {
    return false;
  }

  const char first = field.front();
  if (first == '+' || first == ' ' || first == '\t' || first == '\n' ||
      first == '\r' || first == '\f' || first == '\v') {
    return false;
  }

  const auto digits = (first == '-') ? field.substr(1) : field;
  if (digits.empty()) {
    return false;
  }

  const char lead = digits.front();
  if (lead == 'i' || lead == 'I' || lead == 'n' || lead == 'N') {
    return false;  // inf, infinity, nan
  }
  if (digits.size() > 1 && digits[0] == '0' &&
      (digits[1] == 'x' || digits[1] == 'X')) {
    return false;  // hexadecimal float
  }
  return true;
}

}  // namespace detail

#if !LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS
namespace detail {

/**
 * @brief A "C" locale for numeric conversion, created once.
 *
 * @details strtod follows the global locale, and under a European locale it
 * reads "1,5" as one and a half while stopping at the dot in "1.5" — silently
 * truncating every coordinate in the deck. strtod_l takes the locale as an
 * argument instead, which keeps the parse independent of whatever the host is
 * configured for. The handle is read-only once built, so sharing it across
 * threads is safe.
 */
class CNumericLocale {
 public:
#  ifdef _WIN32
  using Handle = ::_locale_t;
#  else
  using Handle = ::locale_t;
#  endif

  CNumericLocale() noexcept :
#  ifdef _WIN32
      m_locale(::_create_locale(LC_NUMERIC, "C"))
#  else
      m_locale(
          ::newlocale(LC_NUMERIC_MASK, "C", static_cast< ::locale_t>(nullptr)))
#  endif
  {
  }

  ~CNumericLocale() {
    if (m_locale != nullptr) {
#  ifdef _WIN32
      ::_free_locale(m_locale);
#  else
      ::freelocale(m_locale);
#  endif
    }
  }

  CNumericLocale(const CNumericLocale&) = delete;
  CNumericLocale(CNumericLocale&&) = delete;
  auto operator=(const CNumericLocale&) -> CNumericLocale& = delete;
  auto operator=(CNumericLocale&&) -> CNumericLocale& = delete;

  [[nodiscard]] auto get() const noexcept -> Handle { return m_locale; }

 private:
  Handle m_locale;
};

/**
 * @brief Longest numeric field this parser accepts, including the terminator.
 *
 * @details A double never needs more than about 30 characters to be written
 * exactly; anything longer is not a number this deck should contain.
 */
inline constexpr std::size_t kMaxNumericFieldSize{64};

/**
 * @brief Parse a double the way std::from_chars would, without it.
 *
 * @details The caller has already run @ref is_plain_decimal, so only the
 * numeric conversion itself is left. Range errors are reported as failures,
 * matching what from_chars does.
 *
 * @param field the text to convert; need not be null terminated.
 * @return std::optional<double> the value, or nothing when @p field is not a
 * complete number.
 */
[[nodiscard]] inline auto strtod_c_locale(std::string_view field)
    -> std::optional<double> {
  if (field.size() >= kMaxNumericFieldSize) {
    return std::nullopt;
  }

  // strtod_l needs a null-terminated string and a field is a view into a
  // longer line, so copy it; the size check above bounds the copy.
  std::array<char, kMaxNumericFieldSize> buffer{};
  field.copy(buffer.data(), field.size());
  buffer.at(field.size()) = '\0';

  static const CNumericLocale kLocale;
  if (kLocale.get() == nullptr) {
    return std::nullopt;
  }

  char* end = nullptr;
  errno = 0;
#  ifdef _WIN32
  const double value = ::_strtod_l(buffer.data(), &end, kLocale.get());
#  else
  const double value = ::strtod_l(buffer.data(), &end, kLocale.get());
#  endif

  // Out of range is a failure for from_chars, so it is one here too.
  if (errno == ERANGE) {
    return std::nullopt;
  }
  // The whole field has to be consumed, which is what rejects "1.0x".
  if (end != buffer.data() + field.size()) {
    return std::nullopt;
  }
  return value;
}

}  // namespace detail
#endif  // !LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS

/**
 * @brief Split a card line into non-empty fields.
 *
 * @details HyperMesh writes LS-DYNA cards in fixed columns padded with
 * spaces, but comma-separated decks are equally legal, so both separators are
 * accepted. Views into @p line are returned rather than copies: a deck of
 * several hundred thousand lines is parsed one line at a time, and allocating
 * a string per field dominates the runtime otherwise.
 *
 * @param line the card line; must outlive the returned views.
 * @return Fields the non-empty fields, in order.
 */
[[nodiscard]] inline auto split_fields(std::string_view line) -> Fields {
  constexpr std::string_view kSeparators{" \t\r,"};
  Fields fields;
  for (std::size_t begin = line.find_first_not_of(kSeparators);
       begin != std::string_view::npos;
       begin = line.find_first_not_of(kSeparators, begin)) {
    const auto end = line.find_first_of(kSeparators, begin);
    fields.push_back(line.substr(begin, end == std::string_view::npos
                                            ? std::string_view::npos
                                            : end - begin));
    if (end == std::string_view::npos) {
      break;
    }
    begin = end;
  }
  return fields;
}

/**
 * @brief Convert a field to an unsigned integer.
 *
 * @param field the text to convert.
 * @return std::optional<std::uint64_t> the value, or nothing when @p field is
 * not a complete integer.
 */
[[nodiscard]] inline auto to_uint(std::string_view field)
    -> std::optional<std::uint64_t> {
  std::uint64_t value{};
  // Both ends of the view are handed to from_chars, so it never depends on
  // null termination the way the check assumes.
  // NOLINTBEGIN(bugprone-suspicious-stringview-data-usage)
  const auto* const last = field.data() + field.size();
  const auto result = std::from_chars(field.data(), last, value);
  // NOLINTEND(bugprone-suspicious-stringview-data-usage)
  if (result.ec != std::errc{} || result.ptr != last) {
    return std::nullopt;
  }
  return value;
}

/**
 * @brief Convert a field to a floating point value.
 *
 * @details Uses std::from_chars, which is locale independent — a comma
 * decimal separator would silently corrupt coordinates with strtod under a
 * European locale — and reports failure without throwing.
 *
 * @param field the text to convert.
 * @return std::optional<double> the value, or nothing when @p field is not a
 * complete number.
 */
[[nodiscard]] inline auto to_double(std::string_view field)
    -> std::optional<double> {
  if (not detail::is_plain_decimal(field)) {
    return std::nullopt;
  }

#if LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS
  double value{};
  // Both ends of the view are handed to from_chars, so it never depends on
  // null termination the way the check assumes.
  // NOLINTBEGIN(bugprone-suspicious-stringview-data-usage)
  const auto* const last = field.data() + field.size();
  const auto result = std::from_chars(field.data(), last, value);
  // NOLINTEND(bugprone-suspicious-stringview-data-usage)
  if (result.ec != std::errc{} || result.ptr != last) {
    return std::nullopt;
  }
  return value;
#else
  return detail::strtod_c_locale(field);
#endif
}

/**
 * @brief Whether the line opens a keyword section (`*NODE`, `*END`, ...).
 */
[[nodiscard]] inline auto is_keyword(std::string_view line) noexcept -> bool {
  return line.starts_with('*');
}

/**
 * @brief Whether the line is a LS-DYNA comment.
 */
[[nodiscard]] inline auto is_comment(std::string_view line) noexcept -> bool {
  return line.starts_with('$');
}

/**
 * @brief Whether the line carries no data at all.
 */
[[nodiscard]] inline auto is_skippable(std::string_view line) noexcept -> bool {
  return line.empty() || is_keyword(line) || is_comment(line);
}

}  // namespace lsdynatoapdl::lsdyna

#endif  // LSDYNA_TO_APDL_LSDYNA_FIELDS_HH
