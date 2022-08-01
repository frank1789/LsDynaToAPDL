/**
 * @file keywords.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Useful keywords used in LS-DYNA
 * @version 0.1
 * @date 2022-08-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <QDebug>
#include <QString>

#ifndef LSDYNA_SINTAX_KEYWORD_H
#define LSDYNA_SINTAX_KEYWORD_H

namespace sintax {
namespace lsdyna {
/**
 * @brief The KeywordDyna enum define keywords in languange LsDyna
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

inline QDebug operator<<(QDebug os, const KeywordDyna &key) {
  switch (key) {
    case sintax::lsdyna::KeywordDyna::Header:
      os << QStringLiteral("$");
      break;
    case sintax::lsdyna::KeywordDyna::KeyWord:
      os << QStringLiteral("KEYWORD");
      break;
    case sintax::lsdyna::KeywordDyna::Node:
      os << QStringLiteral("NODE");
      break;
    case sintax::lsdyna::KeywordDyna::ElementShell:
      os << QStringLiteral("ELEMENTSHELL");
      break;
    case sintax::lsdyna::KeywordDyna::ElementSolid:
      os << QStringLiteral("ELEMENTSOLID");
      break;
    case sintax::lsdyna::KeywordDyna::InitialStrainSolid:
      os << QStringLiteral("INITIALSTRAINSOLID");
      break;
    case sintax::lsdyna::KeywordDyna::InitialStressShell:
      os << QStringLiteral("INITIALSTRESSSHELL");
      break;
    case sintax::lsdyna::KeywordDyna::End:
      os << QStringLiteral("END");
      break;
  }
  return os;
}

}  // namespace lsdyna
}  // namespace sintax

#endif  // LSDYNA_SINTAX_KEYWORD_H
