#ifndef LSDYNA_SINTAX_KEYWORD_H
#define LSDYNA_SINTAX_KEYWORD_H

#include <QDebug>
#include <ostream>

namespace sintax {
namespace lsdyna {
/**
 * @brief The KeywordDyna enum define keywords in languange LsDyna
 * All keywords are preceded by symbol so it is possible move between
 * different mode to interpret the data.
 *
 */
enum class KeywordDyna {
  // clang-format off
  $,       /**< Identifies the part of the document it contains header. */
  KEYWORD, /**< Identifies the part of the document it contains other keyword not specified. */
  NODE,    /**< Identifies the part of the document it contains list node declaration. */
  ELEMENTSHELL,       /**< Identifies the part of the document it contains list element shell. */
  ELEMENTSOLID,       /**< Identifies the part of the document it contains list element solid. */
  INITIALSTRAINSOLID, /**< Identifies the part of the document it contains initial strain on solid element. */
  INITIALSTRESSSHELL, /**< Identifies the part of the document it contains stress on shell element.*/
  END                 /**< Identifies the end of the document. */
};
// clang-on


std::ostream& operator<<(std::ostream &os, const KeywordDyna &key);

QDebug& operator<<(QDebug &os, const KeywordDyna &key);

} // namespace lsdyna
} // namespace sintax

#endif // LSDYNA_SINTAX_KEYWORD_H