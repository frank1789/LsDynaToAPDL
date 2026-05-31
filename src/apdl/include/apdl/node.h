#ifndef LSDYNA_TO_APDL_APDL_NODE_H
#define LSDYNA_TO_APDL_APDL_NODE_H

#include <fmt/ostream.h>

#include <cstdint>
#include <format>
#include <iomanip>
#include <optional>
#include <ostream>

namespace lsdynatoapdl {
namespace apdl {

class Node : public AbstractGeometricEntity, public GeometricEntity<Node> {
 public:
  friend std::ostream& operator<<(std::ostream& os, const Node& d);

  Node(std::uint64_t id, double x, double y, double z) :
      m_id(id), m_x(x), m_y(y), m_z(z) {}

  Node(std::uint64_t id, double x, double y, double z,
       std::optional<double> thxy = std::nullopt,
       std::optional<double> thyz = std::nullopt,
       std::optional<double> thzx = std::nullopt) :
      m_id(id),
      m_x(x),
      m_y(y),
      m_z(z),
      m_THXY(thxy),
      m_THYZ(thyz),
      m_THZX(thzx) {}

  [[nodiscard]] auto id() const noexcept -> std::uint64_t { return m_id; }

  [[nodiscard]] auto x() const noexcept -> std::int64_t { return m_x; }

  [[nodiscard]] auto y() const noexcept -> std::int64_t { return m_y; }

  [[nodiscard]] auto z() const noexcept -> std::int64_t { return m_z; }

  [[nodiscard]] auto THXY() const noexcept -> std::optional<double> {
    return m_THXY;
  }

  [[nodiscard]] auto THYZ() const noexcept -> std::optional<double> {
    return m_THYZ;
  }

  [[nodiscard]] auto THZX() const noexcept -> std::optional<double> {
    return m_THZX;
  }

 private:
  std::uint64_t m_id;
  double m_x;
  double m_y;
  double m_z;
  std::optional<double> m_THXY;
  std::optional<double> m_THYZ;
  std::optional<double> m_THZX;
};

std::ostream& operator<<(std::ostream& os, const Node& d) {
  return os << "N," << d.id << ',' << d.x << ',' << d.y << ',' << d.z << ','
            << d.THXY << ',' << d.THYZ ',' << d.THZX;
}

template <>
struct fmt::formatter<Node> : ostream_formatter {};

template <>
struct std::formatter<Node> {
  constexpr auto parse(std::format_parse_context& ctx)
      -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  auto format(const Node& node, std::format_context& ctx) const
      -> decltype(ctx.out()) {
    if (node.m_THXY.has_value() && node.m_THYZ.has_value() &&
        node.m_THZX.has_value()) {
      return std::format_to(ctx.out(), "N,{},{},{},{},{},{},{}", node.id,
                            node.x, node.y, node.z, node.m_THXY.value(),
                            node.m_THYZ.value(), node.m_THZX.value());
    } else if (node.m_THXY.has_value() && node.m_THYZ.has_value()) {
      return std::format_to(ctx.out(), "N,{},{},{},{},{},{}", node.id, node.x,
                            node.y, node.z, node.m_THXY.value(),
                            node.m_THYZ.value());
    } else if (node.m_THXY.has_value()) {
      return std::format_to(ctx.out(), "N,{},{},{},{},{}", node.id, node.x,
                            node.y, node.z, node.m_THXY.value());
    }

    return std::format_to("N,{},{},{},{}", node.id, node.x, node.y, node.z);
  }
};

}  // namespace apdl
}  // namespace lsdynatoapdl

#endif  // LSDYNA_TO_APDL_APDL_NODE_H
