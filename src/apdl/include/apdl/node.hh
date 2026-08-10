/**
 * @file node.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief APDL nodal point, printed with the N command.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_APDL_NODE_HH
#define LSDYNA_TO_APDL_APDL_NODE_HH

#include <fmt/format.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

#include "apdl/apdl_entity.hh"

namespace lsdynatoapdl::apdl {

/**
 * @brief A nodal point of the finite element mesh.
 *
 * @details Rendered with the APDL @c N command:
 *
 * @code
 * N, NODE, X, Y, Z, THXY, THYZ, THZX
 * @endcode
 *
 * The three rotation angles are optional. APDL is positional, so a rotation
 * that is set while an earlier one is not still has to occupy its slot; the
 * gap is emitted as an empty field (`N,1,0,0,0,,45`). Trailing unset angles
 * are dropped entirely rather than written as zeros, because an explicit 0
 * would reset a previously defined rotation.
 */
class Node : public ApdlEntity<Node> {
 public:
  constexpr Node() noexcept = default;

  /**
   * @brief Construct a node without nodal rotations.
   *
   * @param id node identification number.
   * @param x coordinate along X in the active coordinate system.
   * @param y coordinate along Y in the active coordinate system.
   * @param z coordinate along Z in the active coordinate system.
   */
  constexpr Node(std::uint64_t id, double x, double y, double z) noexcept :
      m_id(id), m_x(x), m_y(y), m_z(z) {}

  /**
   * @brief Construct a node with optional nodal rotation angles, in degrees.
   *
   * @param id node identification number.
   * @param x coordinate along X.
   * @param y coordinate along Y.
   * @param z coordinate along Z.
   * @param thxy first rotation about nodal Z (positive X toward Y).
   * @param thyz second rotation about nodal X (positive Y toward Z).
   * @param thzx third rotation about nodal Y (positive Z toward X).
   */
  constexpr Node(std::uint64_t id, double x, double y, double z,
                 std::optional<double> thxy, std::optional<double> thyz,
                 std::optional<double> thzx) noexcept :
      m_id(id),
      m_x(x),
      m_y(y),
      m_z(z),
      m_thxy(thxy),
      m_thyz(thyz),
      m_thzx(thzx) {}

  [[nodiscard]] constexpr auto id() const noexcept -> std::uint64_t {
    return m_id;
  }

  [[nodiscard]] constexpr auto x() const noexcept -> double { return m_x; }

  [[nodiscard]] constexpr auto y() const noexcept -> double { return m_y; }

  [[nodiscard]] constexpr auto z() const noexcept -> double { return m_z; }

  [[nodiscard]] constexpr auto thxy() const noexcept -> std::optional<double> {
    return m_thxy;
  }

  [[nodiscard]] constexpr auto thyz() const noexcept -> std::optional<double> {
    return m_thyz;
  }

  [[nodiscard]] constexpr auto thzx() const noexcept -> std::optional<double> {
    return m_thzx;
  }

  /**
   * @brief Render the N command for this node.
   *
   * @return std::string a single APDL line.
   */
  [[nodiscard]] auto apdl_command() const -> std::string {
    auto command = fmt::format("N, {}, {}, {}, {}", m_id, format_scalar(m_x),
                               format_scalar(m_y), format_scalar(m_z));
    append_rotations(command);
    return command;
  }

  [[nodiscard]] friend constexpr auto operator==(const Node&,
                                                 const Node&) noexcept
      -> bool = default;

 private:
  void append_rotations(std::string& command) const {
    const std::array<std::optional<double>, 3> angles{m_thxy, m_thyz, m_thzx};

    // Emit up to the last angle that carries a value; everything after it is
    // dropped, and any gap before it is written as an empty field.
    std::size_t last_set = 0;
    for (std::size_t index = 0; index < angles.size(); ++index) {
      if (angles.at(index).has_value()) {
        last_set = index + 1;
      }
    }

    for (std::size_t index = 0; index < last_set; ++index) {
      command.append(", ");
      if (const auto& angle = angles.at(index); angle.has_value()) {
        command.append(format_scalar(angle.value()));
      }
    }
  }

  std::uint64_t m_id{};
  double m_x{};
  double m_y{};
  double m_z{};
  std::optional<double> m_thxy;
  std::optional<double> m_thyz;
  std::optional<double> m_thzx;
};

}  // namespace lsdynatoapdl::apdl

#endif  // LSDYNA_TO_APDL_APDL_NODE_HH
