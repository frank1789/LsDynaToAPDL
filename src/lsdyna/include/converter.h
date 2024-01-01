/**
 * @file converter.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The ConverterSyntax interpreter of the LsDyna format.
 * @version 0.4.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LSDYNA_CONVERTER_SYNTAX_H
#define LSDYNA_CONVERTER_SYNTAX_H

#include <string>

#include "keywords.h"

namespace syntax {
namespace lsdyna {

/**
 * @brief The ConverterSyntax class interpreter of the LsDyna format, reads the
 * sections of the document by checking that they correspond to the parts of
 * interest, leaves the specific operation when the header in the document
 * changes.
 */
class ConverterSyntax {
 public:
  /**
   * @brief Construct a new ConverterSyntax object
   *
   */
  explicit ConverterSyntax() = default;

  /**
   * @brief Destroy the ConverterSyntax object
   *
   */
  ~ConverterSyntax() = default;

  /**
   * @brief Set the Input File object
   *
   * @param filename std::string holds the entire path to file.
   */
  void setInputFile(const std::string &filename);

  void parse();

  [[nodiscard]] bool isReady() const;

 private:
  /**
   * @brief Checks the current string, if match with a LsDyna keywords.
   *
   * @param line std::string holds text
   */
  void testInputLine(const std::string &line);

  /**
   * @brief Extract values from the current string.
   *
   * @param line std::string holds text
   */
  void parseLine(const std::string &line);

  void reader(const std::string &filename);

  // /**
  //  * @brief Get the Nodes object
  //  *
  //  * @return QVector<PropertyNode<uint64_t, double>>
  //  */
  // [[nodiscard]] QVector<PropertyNode<uint64_t, double>> getNodes() const;

  // /**
  //  * @brief Get the Elements object
  //  *
  //  * @return QVector<ShellFourNode>
  //  */
  // [[nodiscard]] QVector<ShellFourNode> getElements() const;

 private:
  KeywordDyna m_current_document_section{KeywordDyna::Header};
  std::string m_current_file{""};
  bool m_is_ready{false};
  //   QVector<PropertyNode<uint64_t, double>> nodes_{};
  //   QVector<ShellFourNode> elements_{};
  //   QSharedPointer<ElementParser> parser_{nullptr};
};

}  // namespace lsdyna
}  // namespace syntax

#endif  // LSDYNA_CONVERTER_SYNTAX_H
