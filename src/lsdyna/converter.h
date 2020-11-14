#ifndef LSDYNA_COVERTER_SINTAX_H
#define LSDYNA_COVERTER_SINTAX_H

#include <QString>
#include <QObject>

#include "keywords.h"

namespace sintax {
namespace lsdyna {

/**
 * @brief The ConverterSintax class interpreter of the LsDyna format, reads the
 * sections of the document by checking that they correspond to the parts of
 * interest, leaves the specific operation when the header in the document
 * changes.
 */
class ConverterSintax : QObject{
  Q_OBJECT
 public:
 /**
  * @brief Construct a new Converter Sintax object
  * 
  */
  explicit ConverterSintax(QObject *parent=nullptr);

  /**
   * @brief Destroy the Converter Sintax object
   * 
   */
  ~ConverterSintax() = default;

  /**
   * @brief 
   * 
   * @param linefile 
   */
  void testInputLine(const QString &line);

  /**
   * @brief 
   * 
   * @param line 
   */
  void parseLine(const QString &line);


 private:
  sintax::lsdyna::KeywordDyna doc_section_;
};

} // namespace lsdyna
} // namespace sintax

#endif // LSDYNA_COVERTER_SINTAX_H