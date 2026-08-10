/**
 * @file converter.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Reads a LS-DYNA keyword deck into an APDL model.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_LSDYNA_CONVERTER_HH
#define LSDYNA_TO_APDL_LSDYNA_CONVERTER_HH

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "apdl/material.hh"
#include "apdl/model.hh"
#include "lsdyna/keywords.hh"

namespace lsdynatoapdl::lsdyna {

/**
 * @brief Interprets a LS-DYNA keyword deck.
 *
 * @details The deck is a sequence of sections introduced by a `*KEYWORD`
 * line. The converter tracks which section is active and delegates each data
 * line to the parser registered for it, ignoring the sections that have no
 * APDL counterpart. Parsers are held behind a strategy so a new section only
 * needs a new @ref Parser, not a change to this class.
 *
 * @code
 * ConverterSyntax converter;
 * converter.set_input_file("model.k");
 * if (converter.parse()) {
 *   writer.write(converter.model());
 * }
 * @endcode
 */
class ConverterSyntax {
 public:
  ConverterSyntax();
  ~ConverterSyntax() noexcept;

  ConverterSyntax(const ConverterSyntax&) = delete;
  ConverterSyntax(ConverterSyntax&&) noexcept = delete;
  auto operator=(const ConverterSyntax&) -> ConverterSyntax& = delete;
  auto operator=(ConverterSyntax&&) noexcept -> ConverterSyntax& = delete;

  /**
   * @brief Set the deck to read.
   *
   * @param filename path to an existing, readable LS-DYNA deck.
   */
  void set_input_file(std::filesystem::path filename);

  /**
   * @brief Whether a readable input file has been set.
   */
  [[nodiscard]] auto is_ready() const noexcept -> bool;

  /**
   * @brief Read the whole deck.
   *
   * @return true when the file was read to the end.
   */
  [[nodiscard]] auto parse() -> bool;

  /**
   * @brief Consume a single line, as if it had been read from the deck.
   *
   * @details Exposed so a section can be exercised without a file on disk.
   *
   * @param line the raw line, without the trailing newline.
   */
  void parse_line(std::string_view line);

  /**
   * @brief The model built so far.
   */
  [[nodiscard]] auto model() const noexcept -> const apdl::Model&;

  /**
   * @brief Decimal places kept when grouping shell thicknesses into sections.
   *
   * @details Must be set before parsing. A converted deck often carries a
   * marginally different thickness on every element, which at full precision
   * yields one APDL section per element; rounding merges them.
   *
   * @param decimals number of decimal places to keep.
   */
  void set_thickness_decimals(int decimals);

  /**
   * @brief Set what the deck's preamble should say.
   *
   * @param title analysis title for /TITLE and the /COM banner.
   * @param jobname job name for /FILNAM.
   * @param units unit system for /UNIT.
   * @param source name of the deck being converted.
   */
  void set_deck_info(std::string title, std::string jobname, std::string units,
                     std::string source);

  /**
   * @brief Declare the materials the deck should carry.
   */
  void set_materials(std::vector<apdl::Material> materials);

  /**
   * @brief The section currently being read.
   */
  [[nodiscard]] auto current_section() const noexcept -> KeywordDyna;

 private:
  /**
   * @brief Switch section when @p line opens a new keyword.
   *
   * @param line the raw line.
   */
  void test_input_line(std::string_view line);

  class Context;
  std::unique_ptr<Context> m_context;
};

}  // namespace lsdynatoapdl::lsdyna

#endif  // LSDYNA_TO_APDL_LSDYNA_CONVERTER_HH
