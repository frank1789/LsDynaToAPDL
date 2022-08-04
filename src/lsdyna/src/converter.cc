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

// #include "elementfactory.h"
// #include "elementproperty.h"
// #include "logger_tools.h"
// #include "node.h"

#include <fmt/std.h>

#include <string>
#include <thread>

#include "filemanager.h"
#include "lsdyna/lsdyna.h"

namespace syntax {
namespace lsdyna {

void reader_fun(const std::string &filename);

ConverterSyntax::ConverterSyntax() :
    m_logger(spdlog::get(lsdynatoapdl::lsdyna::logger_name.data())) {
  if (m_logger) {
    m_logger->debug("initialize converter syntax");
  }
}

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
void ConverterSyntax::testInputLine(const std::string &line_text) {
  if (line_text.starts_with("$")) {
    m_current_document_section = KeywordDyna::Header;
  }
  if (line_text.starts_with("*KEYWORD")) {
    m_current_document_section = KeywordDyna::KeyWord;
  }
  if (line_text.starts_with("*NODE")) {
    m_current_document_section = KeywordDyna::Node;
  }
  if (line_text.starts_with("*ELEMENT_SHELL_THICKNESS")) {
    m_current_document_section = KeywordDyna::ElementShell;
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
  if (m_logger) {
    m_logger->debug("parse section \"{}\"", m_current_document_section);
  }
}

void ConverterSyntax::parseLine(const std::string &line) {
  testInputLine(line);
  switch (m_current_document_section) {
    case KeywordDyna::Header:
      break;

    case KeywordDyna::KeyWord:
      break;

    case KeywordDyna::Node: {
      // auto node = Node::parseNode(line);
      // nodes_.push_back(node);
    } break;

    case KeywordDyna::ElementShell: {
      // parser_->createParser(ShellType::FourNode);
      // auto shell_four = parser_->parseElement<ShellFourNode>(line);
      // elements_.push_back(shell_four);
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
    if (m_logger) {
      m_logger->error(
          "LsDyna parser is not ready or the input file is not valid");
    }
    std::exit(1002);
  }

  std::thread reader(&ConverterSyntax::reader, this, m_current_file);
  if (m_logger) {
    m_logger->debug("create reader thread {}", reader.get_id());
  }
  if (reader.joinable()) {
    reader.join();
  }
  m_is_ready = false;
  if (m_logger) {
    m_logger->debug("thread has finished and reset the ready flag");
  }
}

void ConverterSyntax::reader(const std::string &filename) {
  if (std::ifstream ifs(filename.data()); ifs) {
    std::string line;
    // read all lines and put them in the set:
    while (std::getline(ifs, line)) {
      parseLine(line);
    }
  } else {
    const auto msg = fmt::format("{}: {}", filename, std::strerror(errno));
    if (m_logger) {
      m_logger->error(msg);
    }
    throw std::runtime_error(msg);
  }
}

bool ConverterSyntax::isReady() const { return m_is_ready; }

void ConverterSyntax::setInputFile(const std::string &filename) {
  if (filename != m_current_file) {
    m_current_file = filename;
  }
  m_is_ready = !m_current_file.empty() && FileManager::isValidFile(filename);
  if (m_logger) {
    m_logger->debug("analyze current file: \"{}\"", filename);
  }
}

// QVector<PropertyNode<uint64_t, double>> ConverterSyntax::getNodes() const {
//   return nodes_;
// }

// QVector<ShellFourNode> ConverterSyntax::getElements() const {
//   return elements_;
// }

}  // namespace lsdyna
}  // namespace syntax
