/**
 * @file material.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Material parameters and their assignment to a material reference.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_APDL_MATERIAL_HH
#define LSDYNA_TO_APDL_APDL_MATERIAL_HH

#include <fmt/format.h>

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

#include "apdl/apdl_entity.hh"

namespace lsdynatoapdl::apdl {

/**
 * @brief Suffixes appended to a material name to build its parameter names.
 *
 * @details Chosen to match the reference deck, which writes `DuralEyounG`,
 * `DuralNi` and `DuralDensity` — capitalisation included.
 */
inline constexpr std::string_view kYoungsModulusSuffix{"EyounG"};
inline constexpr std::string_view kPoissonRatioSuffix{"Ni"};
inline constexpr std::string_view kDensitySuffix{"Density"};

/**
 * @brief A material, written as APDL scalar parameters.
 *
 * @details Rendered as the `*SET` block that belongs before `/PREP7`:
 *
 * @code
 * !*** Duraluminium
 * *SET, DuraluminiumEyounG, 72e9    ![Pa] Young's modulus
 * *SET, DuraluminiumNi, 0.33        !Poisson's ratio
 * *SET, DuraluminiumDensity, 2810   ![Kg/m^3]
 * @endcode
 *
 * The values are kept as the text the user supplied rather than re-rendered
 * from a double: `72e9` is how an engineer writes a Young's modulus, and
 * printing it back as `72000000000` would be a gratuitous change to their
 * input. They are validated as numbers when parsed, not when stored.
 */
class Material : public ApdlEntity<Material> {
 public:
  Material() = default;

  /**
   * @brief Construct a material.
   *
   * @param name identifier used to build the parameter names.
   * @param youngs_modulus Young's modulus, as written by the user, in Pa.
   * @param poisson_ratio Poisson's ratio, as written by the user.
   * @param density density, as written by the user, in kg/m^3.
   */
  Material(std::string name, std::string youngs_modulus,
           std::string poisson_ratio, std::string density) :
      m_name(std::move(name)),
      m_youngs_modulus(std::move(youngs_modulus)),
      m_poisson_ratio(std::move(poisson_ratio)),
      m_density(std::move(density)) {}

  [[nodiscard]] auto name() const noexcept -> const std::string& {
    return m_name;
  }

  [[nodiscard]] auto youngs_modulus() const noexcept -> const std::string& {
    return m_youngs_modulus;
  }

  [[nodiscard]] auto poisson_ratio() const noexcept -> const std::string& {
    return m_poisson_ratio;
  }

  [[nodiscard]] auto density() const noexcept -> const std::string& {
    return m_density;
  }

  /**
   * @brief The APDL parameter name carrying @p suffix for this material.
   */
  [[nodiscard]] auto parameter(std::string_view suffix) const -> std::string {
    return fmt::format("{}{}", m_name, suffix);
  }

  /**
   * @brief Render the `*SET` block declaring this material's parameters.
   */
  [[nodiscard]] auto apdl_command() const -> std::string {
    return fmt::format(
        "!*** {}\n"
        "*SET, {}, {}\t ![Pa] Young's modulus\n"
        "*SET, {}, {}\t\t !Poisson's ratio\n"
        "*SET, {}, {}\t ![Kg/m^3]",
        m_name, parameter(kYoungsModulusSuffix), m_youngs_modulus,
        parameter(kPoissonRatioSuffix), m_poisson_ratio,
        parameter(kDensitySuffix), m_density);
  }

 private:
  std::string m_name;
  std::string m_youngs_modulus;
  std::string m_poisson_ratio;
  std::string m_density;
};

/**
 * @brief Binds a @ref Material to a material reference number inside /PREP7.
 *
 * @details Rendered as the `MP` commands, which refer to the parameters
 * declared earlier rather than repeating the numbers:
 *
 * @code
 * MP, EX, 1, DuraluminiumEyounG
 * MP, NUXY, 1, DuraluminiumNi
 * MP, DENS, 1, DuraluminiumDensity
 * @endcode
 */
class MaterialProperty : public ApdlEntity<MaterialProperty> {
 public:
  MaterialProperty() = default;

  /**
   * @brief Bind @p material to reference number @p material_id.
   */
  MaterialProperty(std::uint64_t material_id, std::string name) :
      m_material_id(material_id), m_name(std::move(name)) {}

  [[nodiscard]] auto material_id() const noexcept -> std::uint64_t {
    return m_material_id;
  }

  /**
   * @brief Render the MP commands for this material.
   */
  [[nodiscard]] auto apdl_command() const -> std::string {
    return fmt::format(
        "MP, EX, {}, {}{}\n"
        "MP, NUXY, {}, {}{}\n"
        "MP, DENS, {}, {}{}",
        m_material_id, m_name, kYoungsModulusSuffix, m_material_id, m_name,
        kPoissonRatioSuffix, m_material_id, m_name, kDensitySuffix);
  }

 private:
  std::uint64_t m_material_id{1};
  std::string m_name;
};

}  // namespace lsdynatoapdl::apdl

#endif  // LSDYNA_TO_APDL_APDL_MATERIAL_HH
