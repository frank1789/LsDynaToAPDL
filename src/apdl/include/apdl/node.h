#ifndef LSDYNA_TO_APDL_APDL_NODE_H
#define LSDYNA_TO_APDL_APDL_NODE_H

#include <fmt/ostream.h>

#include <iomanip>
#include <ostream>

class Node : public AbstractGeometricEntity, public GeometricEntity<Node>{
  int year, month, day;

public:
  friend std::ostream& operator<<(std::ostream& os, const date& d) {
    return os << "N," << id << "," << x << "," << y << "," << z << "," << THXY
              << "," << THYZ "," << THZX;
  }
};

template <>
struct fmt::formatter<Node> : ostream_formatter {};

#endif  // LSDYNA_TO_APDL_APDL_NODE_H
