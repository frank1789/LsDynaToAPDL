#ifndef LSDYNA_TO_APDL_LSDYNA_NODE_H
#define LSDYNA_TO_APDL_LSDYNA_NODE_H

#include <fmt/ostream.h>

#include <string_view>

#include "finite_element_types.h"

namespace syntax {
namespace lsdyna {

struct Node : public AbstractGeometricEntity, public GeometricEntity<Node> {
 public:
  explicit Node() noexcept = default;
  explicit Node(std::string_view input);
  Node(uint64_t id, double x, double y, double z) noexcept;

  void parse(std::string_view input);

  friend std::ostream& operator<<(std::ostream& os, const Node& node);

  friend std::istream& operator>>(std::istream& is, Node& node);

  uint64_t id{};
  double x{};
  double y{};
  double z{};
};



std::ostream& operator<<(std::ostream& os, const Node& node);

std::istream& operator>>(std::istream& is, Node& node);

}  // namespace lsdyna
}  // namespace syntax 

template <>
struct fmt::formatter<syntax::lsdyna::Node> : ostream_formatter {};

#endif  // LSDYNA_TO_APDL_LSDYNA_NODE_H
