/**
 * @file section.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Element type and shell section declarations (ET, SECTYPE, SECDATA).
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_APDL_SECTION_HH
#define LSDYNA_TO_APDL_APDL_SECTION_HH

#include <fmt/format.h>

#include <cstdint>
#include <string>
#include <string_view>

#include "apdl/apdl_entity.hh"

namespace lsdynatoapdl::apdl {

/**
 * @brief Declaration of an element type.
 *
 * @details Rendered with the APDL @c ET command:
 *
 * @code
 * ET, ITYPE, Ename
 * @endcode
 */
class ElementType : public ApdlEntity<ElementType> {
 public:
  constexpr ElementType() noexcept = default;

  /**
   * @brief Declare an element type.
   *
   * @param type_id reference number assigned to this element type.
   * @param name APDL element name, e.g. @c SHELL181.
   */
  constexpr ElementType(std::uint64_t type_id, std::string_view name) noexcept :
      m_type_id(type_id), m_name(name) {}

  [[nodiscard]] constexpr auto type_id() const noexcept -> std::uint64_t {
    return m_type_id;
  }

  [[nodiscard]] constexpr auto name() const noexcept -> std::string_view {
    return m_name;
  }

  /**
   * @brief Render the ET command.
   *
   * @return std::string a single APDL line.
   */
  [[nodiscard]] auto apdl_command() const -> std::string {
    return fmt::format("ET, {}, {}", m_type_id, m_name);
  }

 private:
  std::uint64_t m_type_id{};
  std::string_view m_name;
};

/**
 * @brief A shell section carrying the thickness of a group of elements.
 *
 * @details Rendered as the APDL pair:
 *
 * @code
 * SECTYPE, SECID, SHELL, , NAME
 * SECDATA, TK, MATID, THETA, NUMPT
 * @endcode
 *
 * SHELL181 reads its thickness from the section, so one section is emitted per
 * distinct thickness found in the source deck. @c THETA is the layer
 * orientation and @c NUMPT the number of integration points through the
 * thickness; the defaults below match a single homogeneous layer.
 */
class ShellSection : public ApdlEntity<ShellSection> {
 public:
  /**
   * @brief Integration points through the thickness of a single layer.
   */
  static constexpr std::uint32_t kDefaultIntegrationPoints{3};

  constexpr ShellSection() noexcept = default;

  /**
   * @brief Declare a shell section.
   *
   * @param section_id section reference number.
   * @param thickness layer thickness.
   * @param material_id material reference number for the layer.
   * @param integration_points integration points through the thickness.
   */
  constexpr ShellSection(
      std::uint64_t section_id, double thickness, std::uint64_t material_id = 1,
      std::uint32_t integration_points = kDefaultIntegrationPoints) noexcept :
      m_section_id(section_id),
      m_thickness(thickness),
      m_material_id(material_id),
      m_integration_points(integration_points) {}

  [[nodiscard]] constexpr auto section_id() const noexcept -> std::uint64_t {
    return m_section_id;
  }

  [[nodiscard]] constexpr auto thickness() const noexcept -> double {
    return m_thickness;
  }

  [[nodiscard]] constexpr auto material_id() const noexcept -> std::uint64_t {
    return m_material_id;
  }

  /**
   * @brief Render the SECTYPE and SECDATA pair.
   *
   * @return std::string two APDL lines separated by a newline.
   */
  [[nodiscard]] auto apdl_command() const -> std::string {
    return fmt::format("SECTYPE, {}, SHELL, , shell_{}\nSECDATA, {}, {}, 0, {}",
                       m_section_id, m_section_id, format_scalar(m_thickness),
                       m_material_id, m_integration_points);
  }

 private:
  std::uint64_t m_section_id{};
  double m_thickness{};
  std::uint64_t m_material_id{1};
  std::uint32_t m_integration_points{kDefaultIntegrationPoints};
};

}  // namespace lsdynatoapdl::apdl

#endif  // LSDYNA_TO_APDL_APDL_SECTION_HH
