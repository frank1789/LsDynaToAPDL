/**
 * @file parser.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The parser implementation.
 * @version 0.4.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "core/parser.hh"

#include <fmt/format.h>
#include <spdlog/spdlog.h>

namespace lsdynatoapdl {

Parser::Parser() :
    m_file_handler(std::make_unique<FileManager>()),
    m_converter(std::make_unique<lsdyna::ConverterSyntax>()),
    m_writer(std::make_unique<apdl::DeckWriter>()) {
  spdlog::debug("conversion pipeline initialised");
}

namespace {

/**
 * @brief Warn when thickness grouping produced almost one section per element.
 *
 * @details A converted deck often carries a marginally different thickness on
 * every element. Kept at full precision that yields one APDL section per
 * element, which Ansys accepts but no engineer wants to work with, so say so
 * rather than emitting a technically valid but unusable deck in silence.
 */
void warn_on_section_explosion(const apdl::Model& model) {
  const auto shells = model.shells().size();
  const auto sections = model.sections().size();
  if (shells < 2 || sections * 2 < shells) {
    return;
  }

  spdlog::warn(
      "{} shell sections for {} elements: the deck's thicknesses differ below "
      "the current precision of {} decimals. Re-run with a smaller "
      "--thickness_precision to merge them.",
      sections, shells, model.thickness_decimals());
}

}  // namespace

auto Parser::elaborate(const std::filesystem::path& input_file) -> bool {
  if (input_file.empty()) {
    spdlog::error("no input file given");
    return false;
  }

  if (not FileManager::is_valid_file(input_file)) {
    spdlog::error("\"{}\" is not a readable file", input_file.string());
    return false;
  }

  m_file_handler->set_input_filename(input_file);
  m_file_handler->set_output_filename();

  m_converter->set_thickness_decimals(m_thickness_decimals);
  // Derive what the user did not give from the deck being converted, so the
  // job name and title mean something without extra flags.
  const auto stem = input_file.stem().string();
  m_converter->set_deck_info(
      m_title.empty() ? fmt::format("Model converted from {}", stem) : m_title,
      m_jobname.empty() ? stem : m_jobname, m_units,
      input_file.filename().string());
  m_converter->set_materials(m_materials);
  m_converter->set_input_file(m_file_handler->get_complete_input_filename());
  m_writer->set_filename(m_file_handler->get_output_filename());

  if (not m_converter->is_ready()) {
    spdlog::error("converter is not ready for \"{}\"", input_file.string());
    return false;
  }

  if (not m_converter->parse()) {
    return false;
  }

  const auto& model = m_converter->model();
  if (model.empty()) {
    spdlog::warn("\"{}\" produced an empty model; nothing to write",
                 input_file.string());
    return false;
  }

  warn_on_section_explosion(model);
  return m_writer->write(model);
}

}  // namespace lsdynatoapdl
