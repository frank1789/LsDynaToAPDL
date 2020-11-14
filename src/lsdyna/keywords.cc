#include "keywords.h"

namespace sintax {
namespace lsdyna {

inline std::ostream &operator<<(std::ostream &os, const KeywordDyna &key) {
  switch (key) {
    case KeywordDyna::$:
      os << "$";
      break;
    case KeywordDyna::KEYWORD:
      os << "KEYWORD";
      break;
    case KeywordDyna::NODE:
      os << "NODE";
      break;
    case KeywordDyna::ELEMENTSHELL:
      os << "ELEMENTSHELL";
      break;
    case KeywordDyna::ELEMENTSOLID:
      os << "ELEMENTSOLID";
      break;
    case KeywordDyna::INITIALSTRAINSOLID:
      os << "INITIALSTRAINSOLID";
      break;
    case KeywordDyna::INITIALSTRESSSHELL:
      os << "INITIALSTRESSSHELL";
      break;
    case KeywordDyna::END:
      os << "END";
      break;
  }
  return os;
}

inline QDebug &operator<<(QDebug &os, const KeywordDyna &key) {
  switch (key) {
    case KeywordDyna::$:
      os << "$";
      break;
    case KeywordDyna::KEYWORD:
      os << "KEYWORD";
      break;
    case KeywordDyna::NODE:
      os << "NODE";
      break;
    case KeywordDyna::ELEMENTSHELL:
      os << "ELEMENTSHELL";
      break;
    case KeywordDyna::ELEMENTSOLID:
      os << "ELEMENTSOLID";
      break;
    case KeywordDyna::INITIALSTRAINSOLID:
      os << "INITIALSTRAINSOLID";
      break;
    case KeywordDyna::INITIALSTRESSSHELL:
      os << "INITIALSTRESSSHELL";
      break;
    case KeywordDyna::END:
      os << "END";
      break;
  }
  return os;
}

}  // namespace lsdyna
}  // namespace sintax