/**
 * @file parser.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Drives the conversion from a LS-DYNA deck to an APDL deck.
 * @version 0.4.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LSDYNA_TO_APDL_CORE_PARSER_HH
#define LSDYNA_TO_APDL_CORE_PARSER_HH

#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "apdl/deck_writer.hh"
#include "apdl/material.hh"
#include "common/file_manager.hh"
#include "lsdyna/converter.hh"

namespace lsdynatoapdl {

/**
 * @brief Runs one conversion, from an input deck to an output deck.
 *
 * @details Owns the three collaborators of the pipeline: the file manager
 * decides the names, the converter reads the LS-DYNA deck into an
 * @ref apdl::Model, and the writer serialises that model as APDL.
 */
class Parser {
 public:
  Parser();

  /**
   * @brief Convert @p input_file.
   *
   * @details The output name is derived from the input, next to it, with the
   * `_converted.txt` suffix.
   *
   * @param input_file path of the LS-DYNA deck to read.
   * @return true when the deck was read and the APDL file written.
   */
  [[nodiscard]] auto elaborate(const std::filesystem::path& input_file) -> bool;

  /**
   * @brief Decimal places kept when grouping shell thicknesses.
   *
   * @param decimals number of decimal places; lower merges more elements into
   * one APDL section.
   */
  void set_thickness_decimals(int decimals) noexcept {
    m_thickness_decimals = decimals;
  }

  /**
   * @brief Materials to declare in the deck, as given on the command line.
   *
   * @param materials the material list, in the order they should be numbered.
   */
  void set_materials(std::vector<apdl::Material> materials) {
    m_materials = std::move(materials);
  }

  /**
   * @brief Analysis title for /TITLE; empty means derive it from the input.
   */
  void set_title(std::string title) { m_title = std::move(title); }

  /**
   * @brief Job name for /FILNAM; empty means derive it from the input.
   */
  void set_jobname(std::string jobname) { m_jobname = std::move(jobname); }

  /**
   * @brief Unit system for /UNIT.
   */
  void set_units(std::string units) { m_units = std::move(units); }

 private:
  std::unique_ptr<FileManager> m_file_handler;
  std::unique_ptr<lsdyna::ConverterSyntax> m_converter;
  std::unique_ptr<apdl::DeckWriter> m_writer;
  int m_thickness_decimals{apdl::Model::kDefaultThicknessDecimals};
  std::vector<apdl::Material> m_materials;
  std::string m_title;
  std::string m_jobname;
  std::string m_units{"SI"};
};

}  // namespace lsdynatoapdl

#endif  // LSDYNA_TO_APDL_CORE_PARSER_HH
