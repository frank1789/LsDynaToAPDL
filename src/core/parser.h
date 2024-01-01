/**
 * @file parser.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The parser class holds the components and permit the transformation
 * for both syntax.
 * @version 0.4.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LSDYNA_TO_APDL_CORE_PARSER_H
#define LSDYNA_TO_APDL_CORE_PARSER_H

#include <memory>

#include "converter.h"
#include "filemanager.h"

namespace lsdynatoapdl {

class Parser {
 public:
 explicit Parser();
  // void elaborateFilename(const std::filesystem::path &filename);
  // void writeToFile();

  // void finished();
  void elaborate();

 private:
 std::unique_ptr<FileManager> m_filehandler{nullptr};
  std::unique_ptr<syntax::lsdyna::ConverterSyntax> m_converter{nullptr};
};

}  // namespace lsdynatoapdl

#endif  // LSDYNA_TO_APDL_CORE_PARSER_H
