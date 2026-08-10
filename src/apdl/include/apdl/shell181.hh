/**
 * @file shell181.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief SHELL181 four-node structural shell, printed with the EN command.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_APDL_SHELL181_HH
#define LSDYNA_TO_APDL_APDL_SHELL181_HH

#include <fmt/format.h>

#include <array>
#include <cstdint>
#include <string>

#include "apdl/apdl_entity.hh"

namespace lsdynatoapdl::apdl {

/**
 * @brief Number of nodes of a SHELL181 element.
 */
inline constexpr std::size_t kShell181NodeCount{4};

/**
 * @brief The APDL element name of the supported shell.
 */
inline constexpr std::string_view kShell181Name{"SHELL181"};

/**
 * @brief A four-node structural shell element (SHELL181).
 *
 * @details Rendered with the APDL @c EN command, which states the element
 * number explicitly so the numbering of the source LS-DYNA deck survives the
 * conversion:
 *
 * @code
 * EN, IEL, I, J, K, L
 * @endcode
 *
 * SHELL181 takes its thickness from a section rather than from real constants,
 * so the thickness travels with the element only to let the writer group
 * elements into `SECTYPE`/`SECDATA` blocks. `TYPE`, `MAT` and `SECNUM` are
 * modal commands emitted by the writer before the element block, not part of
 * the element line itself.
 *
 * A degenerate quadrilateral — LS-DYNA writes triangles by repeating the last
 * node — is passed through unchanged, which is exactly how SHELL181 expects a
 * triangular element to be given.
 */
class Shell181 : public ApdlEntity<Shell181> {
 public:
  using NodeIds = std::array<std::uint64_t, kShell181NodeCount>;

  constexpr Shell181() noexcept = default;

  /**
   * @brief Construct a shell element.
   *
   * @param id element identification number.
   * @param nodes the four corner node numbers, in connectivity order.
   * @param thickness shell thickness, in the deck's length unit.
   */
  constexpr Shell181(std::uint64_t id, const NodeIds& nodes,
                     double thickness) noexcept :
      m_id(id), m_nodes(nodes), m_thickness(thickness) {}

  [[nodiscard]] constexpr auto id() const noexcept -> std::uint64_t {
    return m_id;
  }

  [[nodiscard]] constexpr auto nodes() const noexcept -> const NodeIds& {
    return m_nodes;
  }

  [[nodiscard]] constexpr auto thickness() const noexcept -> double {
    return m_thickness;
  }

  /**
   * @brief Whether the element degenerates to a triangle.
   *
   * @details LS-DYNA encodes a three-node shell as a quadrilateral whose last
   * two nodes coincide.
   *
   * @return true when the third and fourth nodes are the same.
   */
  [[nodiscard]] constexpr auto is_degenerate() const noexcept -> bool {
    return m_nodes[2] == m_nodes[3];
  }

  /**
   * @brief Render the EN command for this element.
   *
   * @return std::string a single APDL line.
   */
  [[nodiscard]] auto apdl_command() const -> std::string {
    return fmt::format("EN, {}, {}, {}, {}, {}", m_id, m_nodes[0], m_nodes[1],
                       m_nodes[2], m_nodes[3]);
  }

  [[nodiscard]] friend constexpr auto operator==(const Shell181&,
                                                 const Shell181&) noexcept
      -> bool = default;

 private:
  std::uint64_t m_id{};
  NodeIds m_nodes{};
  double m_thickness{};
};

}  // namespace lsdynatoapdl::apdl

#endif  // LSDYNA_TO_APDL_APDL_SHELL181_HH
