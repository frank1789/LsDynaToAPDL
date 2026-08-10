/**
 * @file parser.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Strategy interface for the parsers of the LS-DYNA card sections.
 * @version 0.1.0
 * @date 2024-12-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LSDYNA_TO_APDL_LSDYNA_PARSER_HH
#define LSDYNA_TO_APDL_LSDYNA_PARSER_HH

#include <string_view>

namespace lsdynatoapdl::lsdyna {

/**
 * @brief Parses the lines of one LS-DYNA keyword section.
 *
 * @details Runtime polymorphism is the right tool here — unlike for the APDL
 * entities, which use CRTP — because the active parser is chosen from the
 * keyword read at runtime, and the cost of one virtual call is amortised over
 * an entire card line.
 *
 * A section is a stateful stream: `*ELEMENT_SHELL_THICKNESS` spreads one
 * element over two consecutive lines, so `parse` is not const and instances
 * must not be shared between sections.
 */
class Parser {
 public:
  virtual ~Parser() noexcept = default;

  Parser(const Parser&) = delete;
  Parser(Parser&&) noexcept = delete;
  auto operator=(const Parser&) -> Parser& = delete;
  auto operator=(Parser&&) noexcept -> Parser& = delete;

  /**
   * @brief Consume one line belonging to the section.
   *
   * @param line the raw card line, without the trailing newline.
   */
  virtual void parse(std::string_view line) = 0;

  /**
   * @brief Number of records successfully read by this parser.
   */
  [[nodiscard]] virtual auto accepted() const noexcept -> std::size_t = 0;

 protected:
  constexpr Parser() noexcept = default;
};

}  // namespace lsdynatoapdl::lsdyna

#endif  // LSDYNA_TO_APDL_LSDYNA_PARSER_HH
