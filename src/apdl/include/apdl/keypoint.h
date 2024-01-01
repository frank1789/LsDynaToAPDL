#ifndef LSDYNA_TO_APDL_APDL_KEYPOINT_H
#define LSDYNA_TO_APDL_APDL_KEYPOINT_H

#include <fmt/ostream.h>

#include <iomanip>
#include <ostream>

class Keypoint : public AbstractGeometricEntity, public GeometricEntity<Node> {
  int year, month, day;

 public:
  friend std::ostream& operator<<(std::ostream& os, const date& d) {
    return os << "K," << id << "," << x << "," << y << "," << z;
  }
};

template <>
struct fmt::formatter<Keypoint> : ostream_formatter {};

#endif  // LSDYNA_TO_APDL_APDL_KEYPOINT_H
