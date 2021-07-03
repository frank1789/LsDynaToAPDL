#ifndef VERSION_H
#define VERSION_H

#include <sstream>

constexpr char kVERSMAJOR[] = "0";
constexpr char kVERSMINOR[] = "3";
constexpr char kVERSPATCH[] = "0";
constexpr char kREVISION[] = "d515d37";

std::string compact_version() {
  std::stringstream v;
  v << kVERSMAJOR << "." << kVERSMINOR << "." << kVERSPATCH;
  return v.str();
}

#endif // VERSION_H
