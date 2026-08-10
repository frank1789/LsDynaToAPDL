/**
 * @file keywords.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Keywords recognised in a LS-DYNA deck.
 * @version 0.4.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LSDYNA_TO_APDL_LSDYNA_KEYWORDS_HH
#define LSDYNA_TO_APDL_LSDYNA_KEYWORDS_HH

#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace lsdynatoapdl::lsdyna {

/**
 * @brief Sections of a LS-DYNA deck.
 *
 * @details Every section is opened by a line starting with `*`, except the
 * header comments which start with `$`. Tracking the active section is what
 * lets the converter interpret otherwise identical-looking numeric lines.
 */
enum class KeywordDyna : std::uint8_t {
  Header,             /**< header's document. */
  KeyWord,            /**< other keyword not specified. */
  Node,               /**< list node declaration. */
  ElementShell,       /**< list element shell. */
  ElementSolid,       /**< list element solid. */
  InitialStrainSolid, /**< initial strain on solid element. */
  InitialStressShell, /**< stress on shell element.*/
  End                 /**< end document. */
};

/**
 * @brief The deck token that opens @p keyword.
 */
[[nodiscard]] constexpr auto to_string_view(KeywordDyna keyword) noexcept
    -> std::string_view {
  switch (keyword) {
    case KeywordDyna::Header:
      return "$";
    case KeywordDyna::KeyWord:
      return "KEYWORD";
    case KeywordDyna::Node:
      return "NODE";
    case KeywordDyna::ElementShell:
      return "ELEMENTSHELL";
    case KeywordDyna::ElementSolid:
      return "ELEMENTSOLID";
    case KeywordDyna::InitialStrainSolid:
      return "INITIALSTRAINSOLID";
    case KeywordDyna::InitialStressShell:
      return "INITIALSTRESSSHELL";
    case KeywordDyna::End:
      return "END";
  }
  return {};
}

}  // namespace lsdynatoapdl::lsdyna

template <>
struct fmt::formatter<lsdynatoapdl::lsdyna::KeywordDyna>
    : formatter<string_view> {
  auto format(lsdynatoapdl::lsdyna::KeywordDyna key,
              format_context& ctx) const {
    return formatter<string_view>::format(
        lsdynatoapdl::lsdyna::to_string_view(key), ctx);
  }
};

#endif  // LSDYNA_TO_APDL_LSDYNA_KEYWORDS_HH
