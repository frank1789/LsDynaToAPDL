/**
 * @file converter.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Implementation of the LS-DYNA deck converter.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "lsdyna/converter.hh"

#include <spdlog/spdlog.h>

#include <fstream>
#include <string>
#include <system_error>
#include <utility>

#include "lsdyna/fields.hh"
#include "lsdyna/parser_element.hh"
#include "lsdyna/parser_node.hh"

namespace lsdynatoapdl::lsdyna {

/**
 * @brief Holds the model, the active parser and the reader state.
 */
class ConverterSyntax::Context {
 public:
  Context() = default;

  void set_strategy(std::unique_ptr<Parser>&& parser) {
    m_parser = std::move(parser);
  }

  void clear_strategy() { m_parser.reset(); }

  void parse(std::string_view line) const {
    if (m_parser) {
      m_parser->parse(line);
    }
  }

  [[nodiscard]] auto model() noexcept -> apdl::Model& { return m_model; }

  [[nodiscard]] auto model() const noexcept -> const apdl::Model& {
    return m_model;
  }

  [[nodiscard]] auto section() const noexcept -> KeywordDyna {
    return m_section;
  }
  void set_section(KeywordDyna section) noexcept { m_section = section; }

  [[nodiscard]] auto input_file() const noexcept
      -> const std::filesystem::path& {
    return m_input_file;
  }
  void set_input_file(std::filesystem::path file) {
    m_input_file = std::move(file);
  }

  [[nodiscard]] auto ready() const noexcept -> bool { return m_ready; }
  void set_ready(bool ready) noexcept { m_ready = ready; }

 private:
  apdl::Model m_model;
  std::unique_ptr<Parser> m_parser;
  KeywordDyna m_section{KeywordDyna::Header};
  std::filesystem::path m_input_file;
  bool m_ready{false};
};

ConverterSyntax::ConverterSyntax() : m_context(std::make_unique<Context>()) {}

ConverterSyntax::~ConverterSyntax() noexcept = default;

void ConverterSyntax::set_input_file(std::filesystem::path filename) {
  m_context->set_input_file(std::move(filename));

  std::error_code error;
  const auto exists =
      std::filesystem::is_regular_file(m_context->input_file(), error);
  m_context->set_ready(not error && exists);

  if (m_context->ready()) {
    spdlog::debug("input deck set to \"{}\"", m_context->input_file().string());
  } else {
    spdlog::error("\"{}\" is not a readable file",
                  m_context->input_file().string());
  }
}

auto ConverterSyntax::is_ready() const noexcept -> bool {
  return m_context->ready();
}

auto ConverterSyntax::model() const noexcept -> const apdl::Model& {
  return m_context->model();
}

auto ConverterSyntax::current_section() const noexcept -> KeywordDyna {
  return m_context->section();
}

void ConverterSyntax::set_thickness_decimals(int decimals) {
  m_context->model().set_thickness_decimals(decimals);
}

void ConverterSyntax::set_deck_info(std::string title, std::string jobname,
                                    std::string units, std::string source) {
  auto& model = m_context->model();
  model.set_title(std::move(title));
  model.set_jobname(std::move(jobname));
  model.set_units(std::move(units));
  model.set_source(std::move(source));
}

void ConverterSyntax::set_materials(std::vector<apdl::Material> materials) {
  auto& model = m_context->model();
  for (auto& material : materials) {
    static_cast<void>(model.add_material(std::move(material)));
  }
}

/**
 * @details Selects the parser for the section the line opens. Sections
 * without an APDL counterpart clear the strategy, so their data lines are
 * discarded instead of being fed to the parser of the previous section — the
 * bug that would otherwise turn `*INITIAL_STRESS_SHELL` rows into nodes.
 */
void ConverterSyntax::test_input_line(std::string_view line) {
  if (is_comment(line)) {
    m_context->set_section(KeywordDyna::Header);
    return;
  }

  if (not is_keyword(line)) {
    return;
  }

  if (line.starts_with("*NODE")) {
    m_context->set_section(KeywordDyna::Node);
    m_context->set_strategy(std::make_unique<ParserNode>(m_context->model()));
  } else if (line.starts_with("*ELEMENT_SHELL")) {
    m_context->set_section(KeywordDyna::ElementShell);
    m_context->set_strategy(
        std::make_unique<ParserElement>(m_context->model()));
  } else if (line.starts_with("*ELEMENT_SOLID")) {
    m_context->set_section(KeywordDyna::ElementSolid);
    m_context->clear_strategy();
  } else if (line.starts_with("*INITIAL_STRAIN")) {
    m_context->set_section(KeywordDyna::InitialStrainSolid);
    m_context->clear_strategy();
  } else if (line.starts_with("*INITIAL_STRESS")) {
    m_context->set_section(KeywordDyna::InitialStressShell);
    m_context->clear_strategy();
  } else if (line.starts_with("*END")) {
    m_context->set_section(KeywordDyna::End);
    m_context->clear_strategy();
  } else {
    m_context->set_section(KeywordDyna::KeyWord);
    m_context->clear_strategy();
  }
}

void ConverterSyntax::parse_line(std::string_view line) {
  test_input_line(line);
  m_context->parse(line);
}

auto ConverterSyntax::parse() -> bool {
  if (not m_context->ready()) {
    spdlog::error("no readable input deck has been set");
    return false;
  }

  std::ifstream ifs(m_context->input_file());
  if (not ifs) {
    spdlog::error("cannot open \"{}\"", m_context->input_file().string());
    return false;
  }

  std::string line;
  std::size_t line_number{0};
  while (std::getline(ifs, line)) {
    ++line_number;
    parse_line(line);
  }

  if (ifs.bad()) {
    spdlog::error("error while reading \"{}\" at line {}",
                  m_context->input_file().string(), line_number);
    return false;
  }

  spdlog::info("read {} lines: {} nodes, {} shells", line_number,
               m_context->model().nodes().size(),
               m_context->model().shells().size());
  return true;
}

}  // namespace lsdynatoapdl::lsdyna
