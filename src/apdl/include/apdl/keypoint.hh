/**
 * @file keypoint.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief APDL keypoint of the solid model, printed with the K command.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_APDL_KEYPOINT_HH
#define LSDYNA_TO_APDL_APDL_KEYPOINT_HH

#include <fmt/format.h>

#include <cstdint>
#include <string>

#include "apdl/apdl_entity.hh"

namespace lsdynatoapdl::apdl {

/**
 * @brief A keypoint of the APDL solid model.
 *
 * @details Rendered with the APDL @c K command:
 *
 * @code
 * K, NPT, X, Y, Z
 * @endcode
 *
 * Keypoints belong to the solid model, not to the mesh: they are the vertices
 * from which lines, areas and volumes are built. A converted LS-DYNA deck is
 * already meshed, so keypoints are emitted only where the geometry has to be
 * reconstructed — typically to anchor a later solid-modelling step.
 */
class Keypoint : public ApdlEntity<Keypoint> {
 public:
  constexpr Keypoint() noexcept = default;

  /**
   * @brief Construct a keypoint.
   *
   * @param id keypoint number.
   * @param x coordinate along X in the active coordinate system.
   * @param y coordinate along Y in the active coordinate system.
   * @param z coordinate along Z in the active coordinate system.
   */
  constexpr Keypoint(std::uint64_t id, double x, double y, double z) noexcept :
      m_id(id), m_x(x), m_y(y), m_z(z) {}

  [[nodiscard]] constexpr auto id() const noexcept -> std::uint64_t {
    return m_id;
  }

  [[nodiscard]] constexpr auto x() const noexcept -> double { return m_x; }

  [[nodiscard]] constexpr auto y() const noexcept -> double { return m_y; }

  [[nodiscard]] constexpr auto z() const noexcept -> double { return m_z; }

  /**
   * @brief Render the K command for this keypoint.
   *
   * @return std::string a single APDL line.
   */
  [[nodiscard]] auto apdl_command() const -> std::string {
    return fmt::format("K, {}, {}, {}, {}", m_id, format_scalar(m_x),
                       format_scalar(m_y), format_scalar(m_z));
  }

  [[nodiscard]] friend constexpr auto operator==(const Keypoint&,
                                                 const Keypoint&) noexcept
      -> bool = default;

 private:
  std::uint64_t m_id{};
  double m_x{};
  double m_y{};
  double m_z{};
};

}  // namespace lsdynatoapdl::apdl

#endif  // LSDYNA_TO_APDL_APDL_KEYPOINT_HH
