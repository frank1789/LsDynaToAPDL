#ifndef LSDYNA_TO_APDL_APDL_NODE_H
#  define LSDYNA_TO_APDL_APDL_NODE_H

#  include <fmt/ostream.h>

#  include <iomanip>
#  include <ostream>

class Node : public AbstractGeometricEntity, public GeometricEntity<Node> {
 public:
  friend std::ostream& operator<<(std::ostream& os, const Node& d);
};

std::ostream& operator<<(std::ostream& os, const Node& d) {
  return os << "N," << d.id << ',' << d.x << ',' << d.y << ',' << d.z << ','
            << d.THXY << ',' << d.THYZ ',' << d.THZX;
}

template <>
struct fmt::formatter<Node> : ostream_formatter {};

// LSDYNA_TO_APDL_APDL_NODE_H
