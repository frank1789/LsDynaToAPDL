#include "keywords.h"

std::ostream &operator<<(std::ostream &os, const sintax::lsdyna::KeywordDyna &key) {
  switch (key) {
    case sintax::lsdyna::KeywordDyna::$:
      os << "$";
      break;
    case sintax::lsdyna::KeywordDyna::KEYWORD:
      os << "KEYWORD";
      break;
    case sintax::lsdyna::KeywordDyna::NODE:
      os << "NODE";
      break;
    case sintax::lsdyna::KeywordDyna::ELEMENTSHELL:
      os << "ELEMENTSHELL";
      break;
    case sintax::lsdyna::KeywordDyna::ELEMENTSOLID:
      os << "ELEMENTSOLID";
      break;
    case sintax::lsdyna::KeywordDyna::INITIALSTRAINSOLID:
      os << "INITIALSTRAINSOLID";
      break;
    case sintax::lsdyna::KeywordDyna::INITIALSTRESSSHELL:
      os << "INITIALSTRESSSHELL";
      break;
    case sintax::lsdyna::KeywordDyna::END:
      os << "END";
      break;
  }
  return os;
}

QDebug operator<<(QDebug os, const sintax::lsdyna::KeywordDyna &key) {
  switch (key) {
    case sintax::lsdyna::KeywordDyna::$:
      os << "$";
      break;
    case sintax::lsdyna::KeywordDyna::KEYWORD:
      os << "KEYWORD";
      break;
    case sintax::lsdyna::KeywordDyna::NODE:
      os << "NODE";
      break;
    case sintax::lsdyna::KeywordDyna::ELEMENTSHELL:
      os << "ELEMENTSHELL";
      break;
    case sintax::lsdyna::KeywordDyna::ELEMENTSOLID:
      os << "ELEMENTSOLID";
      break;
    case sintax::lsdyna::KeywordDyna::INITIALSTRAINSOLID:
      os << "INITIALSTRAINSOLID";
      break;
    case sintax::lsdyna::KeywordDyna::INITIALSTRESSSHELL:
      os << "INITIALSTRESSSHELL";
      break;
    case sintax::lsdyna::KeywordDyna::END:
      os << "END";
      break;
  }
  return os;
}