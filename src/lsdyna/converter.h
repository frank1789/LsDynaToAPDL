#ifndef LSDYNA_COVERTER_SINTAX_H
#define LSDYNA_COVERTER_SINTAX_H

#include <QString>

#include "keywords.h"

namespace sintax {
namespace lsdyna {

/**
 * @brief The ConverterSintax class interpreter of the LsDyna format, reads the
 * sections of the document by checking that they correspond to the parts of
 * interest, leaves the specific operation when the header in the document
 * changes.
 */
class ConverterSintax {
 public:
 /**
  * @brief Construct a new Converter Sintax object
  * 
  */
  explicit ConverterSintax() = default;

  /**
   * @brief Destroy the Converter Sintax object
   * 
   */
  ~ConverterSintax() = default;

  // void testInputLine(const QString &linefile, Node *node, Shell *shell);
  // void parseLine(const QString &line);

  // void test(const QString &p_linefile, Node *node, Shell *shell);

 private:
  sintax::lsdyna::KeywordDyna doc_section_;
};

} // namespace lsdyna
} // namespace sintax

#endif // LSDYNA_COVERTER_SINTAX_H