/**
 * @file keywords.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Useful keywords used in LsDyna
 * @version 0.4.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LSDYNA_SYNTAX_KEYWORDS_H
#define LSDYNA_SYNTAX_KEYWORDS_H

#include <fmt/core.h>
#include <fmt/format.h>

namespace syntax {
namespace lsdyna {
/**
 * @brief The KeywordDyna enum define keywords in language LsDyna
 * All keywords are preceded by symbol so it is possible move between
 * different mode to interpret the data.
 *
 */
enum class KeywordDyna {
  Header,             /**<  header's document. */
  KeyWord,            /**<  other keyword not specified. */
  Node,               /**< list node declaration. */
  ElementShell,       /**< list element shell. */
  ElementSolid,       /**< list element solid. */
  InitialStrainSolid, /**< initial strain on solid element. */
  InitialStressShell, /**< stress on shell element.*/
  End                 /**< end document. */
};

}  // namespace lsdyna
}  // namespace syntax

template <>
struct fmt::formatter<syntax::lsdyna::KeywordDyna> : formatter<string_view> {
  // parse is inherited from formatter<string_view>.

  auto format(syntax::lsdyna::KeywordDyna key,
              format_context& ctx) const {
    string_view name{};
    using dyna = syntax::lsdyna::KeywordDyna;
    switch (key) {
      case dyna::Header:
        name = "$";
        break;
      case dyna::KeyWord:
        name = "KEYWORD";
        break;
      case dyna::Node:
        name = "NODE";
        break;
      case dyna::ElementShell:
        name = "ELEMENTSHELL";
        break;
      case dyna::ElementSolid:
        name = "ELEMENTSOLID";
        break;
      case dyna::InitialStrainSolid:
        name = "INITIALSTRAINSOLID";
        break;
      case dyna::InitialStressShell:
        name = "INITIALSTRESSSHELL";
        break;
      case dyna::End:
        name = "END";
        break;
    }
    return formatter<string_view>::format(name, ctx);
  }
};

#endif  // LSDYNA_SYNTAX_KEYWORDS_H
