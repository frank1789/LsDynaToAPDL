/**
 * @file apdl_entity.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Static-polymorphic (CRTP) base for every entity printable as APDL.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_APDL_APDL_ENTITY_HH
#define LSDYNA_TO_APDL_APDL_APDL_ENTITY_HH

#include <fmt/format.h>

#include <concepts>
#include <ostream>
#include <string>
#include <string_view>
#include <version>

// std::format is used when the standard library provides it, so callers can
// pick whichever of the three printing routes suits them. fmt stays the
// implementation used internally: it is a hard dependency already, and it
// formats doubles identically on every platform, which std::format
// implementations only converged on recently.
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
#  define LSDYNATOAPDL_HAS_STD_FORMAT 1
#  include <format>
#else
#  define LSDYNATOAPDL_HAS_STD_FORMAT 0
#endif

namespace lsdynatoapdl::apdl {

/**
 * @brief CRTP base giving every APDL entity a uniform printing interface.
 *
 * @details Each concrete entity derives as `class Node : public
 * ApdlEntity<Node>` and supplies a single public member:
 *
 * @code
 * [[nodiscard]] auto apdl_command() const -> std::string;
 * @endcode
 *
 * The base then provides @ref to_apdl, @ref write_to, `operator<<` and an
 * fmt formatter for free. Dispatch is resolved at compile time, so entities
 * stay trivially copyable, carry no vtable pointer, and can be stored
 * contiguously in a `std::vector` — which matters when a deck holds millions
 * of nodes.
 *
 * The destructor is protected and non-virtual on purpose: an ApdlEntity is
 * never owned or deleted through a base pointer.
 *
 * @tparam Derived the concrete entity type.
 */
template <typename Derived>
class ApdlEntity {
 public:
  /**
   * @brief Render the entity as a single APDL command block.
   *
   * @return std::string APDL text without a trailing newline.
   */
  [[nodiscard]] auto to_apdl() const -> std::string {
    return self().apdl_command();
  }

  /**
   * @brief Stream the entity into @p os.
   *
   * @param os destination stream.
   * @return std::ostream& @p os, to allow chaining.
   */
  auto write_to(std::ostream& os) const -> std::ostream& {
    return os << self().apdl_command();
  }

  /**
   * @brief Compare two bases, which are always equal.
   *
   * @details The base holds no state, but a derived class that writes
   * `operator==(...) = default` still needs one to exist here: a defaulted
   * comparison compares base classes first, and without this it is *implicitly
   * deleted*. GCC accepts the declaration and only fails where the operator is
   * used, so the entities compared equal nowhere and nothing noticed until
   * Clang reported -Wdefaulted-function-deleted.
   */
  [[nodiscard]] friend constexpr auto operator==(
      const ApdlEntity& /*lhs*/, const ApdlEntity& /*rhs*/) noexcept -> bool {
    return true;
  }

 private:
  // Only Derived may construct the base. Combined with the ApdlPrintable
  // concept this makes the CRTP self-reference impossible to get wrong: a
  // class cannot derive from ApdlEntity<SomeOtherType> at all.
  friend Derived;

  constexpr ApdlEntity() noexcept = default;
  ~ApdlEntity() noexcept = default;
  ApdlEntity(const ApdlEntity&) noexcept = default;
  ApdlEntity(ApdlEntity&&) noexcept = default;
  auto operator=(const ApdlEntity&) noexcept -> ApdlEntity& = default;
  auto operator=(ApdlEntity&&) noexcept -> ApdlEntity& = default;

  [[nodiscard]] constexpr auto self() const noexcept -> const Derived& {
    return static_cast<const Derived&>(*this);
  }
};

/**
 * @brief Satisfied by any type wired into the CRTP printing framework.
 *
 * @details Checks both halves of the contract: the type must derive from
 * ApdlEntity instantiated with itself (which rules out the classic CRTP
 * mistake of copy-pasting `: ApdlEntity<Node>` into `Keypoint`), and it must
 * actually supply `apdl_command()`.
 */
template <typename T>
concept ApdlPrintable =
    std::derived_from<T, ApdlEntity<T>> && requires(const T& entity) {
      { entity.apdl_command() } -> std::convertible_to<std::string>;
    };

/**
 * @brief Stream insertion shared by every APDL entity.
 */
template <ApdlPrintable T>
auto operator<<(std::ostream& os, const T& entity) -> std::ostream& {
  return entity.write_to(os);
}

/**
 * @brief The APDL comment marker.
 */
inline constexpr std::string_view kCommentPrefix{"! "};

/**
 * @brief Format a coordinate or a scalar the way APDL expects it.
 *
 * @details Uses the shortest representation that round-trips back to the same
 * double, so no precision is lost relative to the source deck and the output
 * stays readable.
 *
 * @param value the number to render.
 * @return std::string the rendered number.
 */
[[nodiscard]] inline auto format_scalar(double value) -> std::string {
  return fmt::format("{}", value);
}

}  // namespace lsdynatoapdl::apdl

/**
 * @brief One fmt formatter covering every ApdlPrintable entity.
 *
 * @details Constrained on the concept rather than written per type, so a new
 * entity is formattable the moment it derives from ApdlEntity.
 *
 * @code
 * fmt::print("{}\n", node);
 * @endcode
 */
template <lsdynatoapdl::apdl::ApdlPrintable T>
struct fmt::formatter<T> : fmt::formatter<std::string> {
  template <typename FormatContext>
  auto format(const T& entity, FormatContext& ctx) const {
    return fmt::formatter<std::string>::format(entity.apdl_command(), ctx);
  }
};

#if LSDYNATOAPDL_HAS_STD_FORMAT
/**
 * @brief The same, for std::format.
 *
 * @code
 * std::string line = std::format("{}", node);
 * @endcode
 */
// Specialising std::formatter for a program-defined type is exactly what the
// standard asks for; the check cannot distinguish it from real tampering.
// NOLINTBEGIN(cert-dcl58-cpp,bugprone-std-namespace-modification)
template <lsdynatoapdl::apdl::ApdlPrintable T>
struct std::formatter<T> : std::formatter<std::string> {
  template <typename FormatContext>
  auto format(const T& entity, FormatContext& ctx) const {
    return std::formatter<std::string>::format(entity.apdl_command(), ctx);
  }
};
// NOLINTEND(cert-dcl58-cpp,bugprone-std-namespace-modification)
#endif

#endif  // LSDYNA_TO_APDL_APDL_APDL_ENTITY_HH
