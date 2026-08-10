/**
 * @file model.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief In-memory APDL model collected while a LS-DYNA deck is parsed.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_APDL_MODEL_HH
#define LSDYNA_TO_APDL_APDL_MODEL_HH

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "apdl/keypoint.hh"
#include "apdl/material.hh"
#include "apdl/node.hh"
#include "apdl/section.hh"
#include "apdl/shell181.hh"

namespace lsdynatoapdl::apdl {

/**
 * @brief Everything the writer needs to emit a complete APDL deck.
 *
 * @details The model owns three flat vectors plus the shell sections derived
 * from the element thicknesses. Entities are value types with no vtable, so
 * the vectors stay contiguous even for decks with millions of nodes.
 *
 * SHELL181 takes its thickness from a section, and a converted deck can carry
 * a slightly different thickness on every element. Emitting one section per
 * element would be correct but unusable, so thicknesses are rounded to
 * @ref thickness_decimals before being grouped; elements sharing a rounded
 * thickness share a section.
 */
class Model {
 public:
  /**
   * @brief Decimal places kept when grouping thicknesses into sections.
   */
  static constexpr int kDefaultThicknessDecimals{6};

  Model() = default;

  /**
   * @brief Append a material, as supplied on the command line.
   *
   * @return std::uint64_t the material reference number, one-based.
   */
  auto add_material(Material material) -> std::uint64_t {
    m_materials.push_back(std::move(material));
    return static_cast<std::uint64_t>(m_materials.size());
  }

  [[nodiscard]] auto materials() const noexcept
      -> const std::vector<Material>& {
    return m_materials;
  }

  /**
   * @brief Job name written to /FILNAM.
   */
  [[nodiscard]] auto jobname() const noexcept -> const std::string& {
    return m_jobname;
  }
  void set_jobname(std::string jobname) { m_jobname = std::move(jobname); }

  /**
   * @brief Analysis title written to /TITLE and to the /COM banner.
   */
  [[nodiscard]] auto title() const noexcept -> const std::string& {
    return m_title;
  }
  void set_title(std::string title) { m_title = std::move(title); }

  /**
   * @brief Unit system written to /UNIT.
   */
  [[nodiscard]] auto units() const noexcept -> const std::string& {
    return m_units;
  }
  void set_units(std::string units) { m_units = std::move(units); }

  /**
   * @brief Name of the deck this model was converted from.
   */
  [[nodiscard]] auto source() const noexcept -> const std::string& {
    return m_source;
  }
  void set_source(std::string source) { m_source = std::move(source); }

  /**
   * @brief Append a node.
   */
  void add_node(const Node& node) { m_nodes.push_back(node); }

  /**
   * @brief Append a keypoint.
   */
  void add_keypoint(const Keypoint& keypoint) {
    m_keypoints.push_back(keypoint);
  }

  /**
   * @brief Append a shell element, assigning it to a thickness section.
   *
   * @param id element identification number.
   * @param nodes the four corner node numbers.
   * @param thickness shell thickness.
   */
  void add_shell(std::uint64_t id, const Shell181::NodeIds& nodes,
                 double thickness) {
    m_shells.emplace_back(id, nodes, thickness);
    section_for(thickness);
  }

  /**
   * @brief Section number that carries @p thickness, creating it if needed.
   *
   * @details Deliberately not [[nodiscard]]: callers legitimately invoke it
   * only to register the section.
   *
   * @param thickness the thickness to look up.
   * @return std::uint64_t the section reference number, one-based.
   */
  auto section_for(double thickness) -> std::uint64_t {
    const auto key = quantise(thickness);
    if (const auto found = m_sections.find(key); found != m_sections.end()) {
      return found->second;
    }
    const auto section_id = static_cast<std::uint64_t>(m_sections.size() + 1);
    m_sections.emplace(key, section_id);
    return section_id;
  }

  /**
   * @brief Section number of an element already added to the model.
   */
  [[nodiscard]] auto section_of(const Shell181& shell) const -> std::uint64_t {
    const auto found = m_sections.find(quantise(shell.thickness()));
    return found == m_sections.end() ? 1U : found->second;
  }

  /**
   * @brief The shell sections, ordered by section number.
   */
  [[nodiscard]] auto sections() const -> std::vector<ShellSection> {
    std::vector<ShellSection> ordered;
    ordered.reserve(m_sections.size());
    for (const auto& [thickness, section_id] : m_sections) {
      ordered.emplace_back(section_id, thickness);
    }
    std::ranges::sort(ordered, {}, &ShellSection::section_id);
    return ordered;
  }

  [[nodiscard]] auto nodes() const noexcept -> const std::vector<Node>& {
    return m_nodes;
  }

  [[nodiscard]] auto keypoints() const noexcept
      -> const std::vector<Keypoint>& {
    return m_keypoints;
  }

  [[nodiscard]] auto shells() const noexcept -> const std::vector<Shell181>& {
    return m_shells;
  }

  [[nodiscard]] auto empty() const noexcept -> bool {
    return m_nodes.empty() && m_keypoints.empty() && m_shells.empty();
  }

  /**
   * @brief Reserve storage, avoiding reallocations on large decks.
   */
  void reserve(std::size_t node_count, std::size_t shell_count) {
    m_nodes.reserve(node_count);
    m_shells.reserve(shell_count);
  }

  /**
   * @brief Decimal places used when grouping thicknesses.
   */
  [[nodiscard]] constexpr auto thickness_decimals() const noexcept -> int {
    return m_thickness_decimals;
  }

  /**
   * @brief Set the decimal places used when grouping thicknesses.
   *
   * @details Must be called before the first element is added, otherwise
   * previously assigned sections would no longer match.
   */
  void set_thickness_decimals(int decimals) noexcept {
    m_thickness_decimals = decimals;
  }

 private:
  [[nodiscard]] auto quantise(double thickness) const -> double {
    const auto scale = std::pow(10.0, m_thickness_decimals);
    return std::round(thickness * scale) / scale;
  }

  std::vector<Node> m_nodes;
  std::vector<Keypoint> m_keypoints;
  std::vector<Shell181> m_shells;
  std::map<double, std::uint64_t> m_sections;
  std::vector<Material> m_materials;
  std::string m_jobname{"lsdynatoapdl"};
  std::string m_title{"Model converted from LS-DYNA"};
  std::string m_units{"SI"};
  std::string m_source;
  int m_thickness_decimals{kDefaultThicknessDecimals};
};

}  // namespace lsdynatoapdl::apdl

#endif  // LSDYNA_TO_APDL_APDL_MODEL_HH
