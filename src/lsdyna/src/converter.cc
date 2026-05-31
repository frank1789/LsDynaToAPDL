/**
 * @file converter.h
 * @author Francesco Argentieri (francesco.argentieri89@gmaol.com)
 * @brief The ConverterSyntax interpreter of the LS-Dyna format.
 * @version 0.4.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "converter.h"

#include <fmt/std.h>

#include <future>
#include <stdexcept>
#include <string>

#include "common/file_manager.hh"
#include "lsdyna/lsdyna.hh"
#include "lsdyna/parser_element.hh"
#include "lsdyna/parser_node.hh"

namespace syntax {
namespace lsdyna {

class ConverterSyntax::Context {
 public:
  constexpr Context() noexcept = default;

  void set_strategy(std::unique_ptr<Parser> &&parser) {
    m_parser = std::move(parser);
  }

  void parse(const std::string &line) const {
    if (m_parser) {
      m_parser->parse(line);
    }
  }

 private:
  std::unique_ptr<Parser> m_parser{nullptr};
};

ConverterSyntax::ConverterSyntax() :
    // m_logger(spdlog::get(lsdynatoapdl::lsdyna::logger_name)),
    m_context(std::make_unique<Context>()) {
  {
    // if (!m_logger) {
    //   throw std::runtime_error("logger is not initialized");
    // }
  }
}

ConverterSyntax::~ConverterSyntax() noexcept = default;

/**
 * @brief ConverterSyntax::setInputLine
 *
 * @details The function scrolls the document reading the
 * line in input and checking if it contains one of the
 * specified keywords, if the matching is positive the
 * appropriate mode is saved. Otherwise the lines are
 * transparent to the function and the set mode is not changed.
 *
 * @param[in] line_text: line of the document to be analyzed.
 */
void ConverterSyntax::test_input_line(const std::string &line_text) {
  if (line_text.starts_with("$")) {
    m_current_document_section = KeywordDyna::Header;
  }

  if (line_text.starts_with("*KEYWORD")) {
    m_current_document_section = KeywordDyna::KeyWord;
  }

  if (line_text.starts_with("*NODE")) {
    m_current_document_section = KeywordDyna::Node;
    m_context->set_strategy(std::make_unique<ParserNode>());
  }

  if (line_text.starts_with("*ELEMENT_SHELL_THICKNESS")) {
    m_current_document_section = KeywordDyna::ElementShell;
    m_context->set_strategy(std::make_unique<ParserElement>());
  }

  if (line_text.starts_with("*ELEMENT_SOLID")) {
    m_current_document_section = KeywordDyna::ElementSolid;
  }

  if (line_text.starts_with("*INITIAL_STRAIN_SOLID")) {
    m_current_document_section = KeywordDyna::InitialStrainSolid;
  }

  if (line_text.starts_with("*INITIAL_STRESS_SHELL")) {
    m_current_document_section = KeywordDyna::InitialStressShell;
  }

  if (line_text.starts_with("*END")) {
    m_current_document_section = KeywordDyna::End;
  }
}

void ConverterSyntax::parse_line(const std::string &input_line) {
  test_input_line(input_line);
  switch (m_current_document_section) {
    case KeywordDyna::Header:
      break;

    case KeywordDyna::KeyWord:
      break;

    case KeywordDyna::Node: {
      m_context->parse(input_line);
    } break;

    case KeywordDyna::ElementShell: {
      m_context->parse(input_line);
    } break;

    case KeywordDyna::ElementSolid:
      break;

    case KeywordDyna::InitialStrainSolid:
      break;

    case KeywordDyna::InitialStressShell:
      break;

    case KeywordDyna::End:
      break;
  }
}

void ConverterSyntax::parse() {
  if (!m_is_ready) {
    spdlog::error("LsDyna parser is not ready or the input file is not valid");
  }
  [[maybe_unused]] auto result = std::async(
      std::launch::async, &ConverterSyntax::reader, this, m_current_file);
  m_is_ready = false;
  spdlog::debug("thread finished");
}

void ConverterSyntax::reader(const std::string &filename) {
  if (std::ifstream ifs(filename.data()); ifs) {
    std::string line;
    while (std::getline(ifs, line)) {
      parse_line(line);
    }
  } else {
    const auto msg = fmt::format("{}: {}", filename, std::strerror(errno));
    spdlog::error(msg);
    throw std::runtime_error(msg);
  }
}

bool ConverterSyntax::isReady() const { return m_is_ready; }

void ConverterSyntax::set_input_file(const std::string &filename) {
  if (filename != m_current_file) {
    m_current_file = filename;
  }
  m_is_ready = !m_current_file.empty();  // isValidFile(filename);
  spdlog::debug("analyze current file: \"{}\"", filename);
}

}  // namespace lsdyna
}  // namespace syntax
