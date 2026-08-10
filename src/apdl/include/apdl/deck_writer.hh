/**
 * @file deck_writer.hh
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Serialises an APDL model into a deck readable by Ansys Mechanical.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_APDL_DECK_WRITER_HH
#define LSDYNA_TO_APDL_APDL_DECK_WRITER_HH

#include <filesystem>
#include <iosfwd>
#include <string>

#include "apdl/model.hh"

namespace lsdynatoapdl::apdl {

/**
 * @brief Writes an APDL deck for a @ref Model.
 *
 * @details The emitted deck follows the order Ansys expects: the preprocessor
 * is entered, element types and sections are declared, then geometry and mesh
 * are defined, and finally the preprocessor is left.
 *
 * @code
 * /PREP7
 * ET,1,SHELL181
 * SECTYPE,1,SHELL,,shell_1
 * SECDATA,3.978858,1,0,3
 * N,1162886,-25.5204048,320.880554,-161.429962
 * TYPE,1
 * MAT,1
 * SECNUM,1
 * EN,1229174,1189339,1162886,1189341,1189340
 * FINISH
 * @endcode
 *
 * `TYPE`, `MAT` and `SECNUM` are modal in APDL: they apply to every element
 * defined afterwards. Elements are therefore written grouped by section, and
 * `SECNUM` is re-emitted only when the section changes.
 */
class DeckWriter {
 public:
  DeckWriter() = default;

  /**
   * @brief Set the destination file.
   *
   * @param filename path of the deck to produce.
   */
  void set_filename(std::filesystem::path filename);

  /**
   * @brief Destination file currently configured.
   */
  [[nodiscard]] auto filename() const noexcept -> const std::filesystem::path& {
    return m_filename;
  }

  /**
   * @brief Write @p model to the configured file.
   *
   * @param model the model to serialise.
   * @return true when the deck was written completely.
   */
  [[nodiscard]] auto write(const Model& model) const -> bool;

  /**
   * @brief Write @p model to an arbitrary stream.
   *
   * @details Kept separate from @ref write so the deck can be produced in
   * memory, which is what the tests do.
   *
   * @param model the model to serialise.
   * @param os destination stream.
   */
  static void write(const Model& model, std::ostream& os);

 private:
  std::filesystem::path m_filename;
};

}  // namespace lsdynatoapdl::apdl

#endif  // LSDYNA_TO_APDL_APDL_DECK_WRITER_HH
