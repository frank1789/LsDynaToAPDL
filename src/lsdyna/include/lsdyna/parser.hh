/**
 * @file parser.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief
 * @version 0.1.0
 * @date 2024-12-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LSDYNA_TO_APDL_LSDYNA_PARSER_HH
#define LSDYNA_TO_APDL_LSDYNA_PARSER_HH

#include <string>

class Parser {
 public:
  virtual ~Parser() noexcept = default;

  Parser(const Parser &) = delete;
  Parser(Parser &&) noexcept = delete;
  Parser &operator=(const Parser &) = delete;
  Parser &operator=(Parser &&) noexcept = delete;

  virtual void parse(const std::string &line) const = 0;

 protected:
  constexpr Parser() noexcept = default;
};

#endif  // LSDYNA_TO_APDL_LSDYNA_PARSER_HH
