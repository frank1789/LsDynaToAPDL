#ifndef LSDYNA_COVERTER_SINTAX_H
#define LSDYNA_COVERTER_SINTAX_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QVector>

#include "elementparser.h"
#include "keywords.h"

namespace sintax {
namespace lsdyna {

/**
 * @brief The ConverterSintax class interpreter of the LsDyna format, reads the
 * sections of the document by checking that they correspond to the parts of
 * interest, leaves the specific operation when the header in the document
 * changes.
 */
class ConverterSintax : public QThread {
  Q_OBJECT
 public:
  /**
   * @brief Construct a new Converter Sintax object
   *
   */
  explicit ConverterSintax(QObject *parent = nullptr);

  /**
   * @brief Destroy the Converter Sintax object
   *
   */
  ~ConverterSintax() override = default;

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

  void run() override;

  void setInputFile(const QString &filename);
  [[nodiscard]] QString getFilename() const;

  [[nodiscard]] QVector<PropertyNode<quint64, qreal>> getNodes() const;
  [[nodiscard]] QVector<ShellFourNode> getElements() const;

 public slots:
  void filenameChanged(const QString &filename);

 private:
  sintax::lsdyna::KeywordDyna doc_section_{};
  QString filename_{};
  QVector<PropertyNode<quint64, qreal>> nodes_{};
  QVector<ShellFourNode> elements_{};
  QSharedPointer<ElementParser> parser_{nullptr};

  //  std::function<ShellElement<quint64, quint64, qreal, 4>(const QString &)>
  //      function_parser_{nullptr};
};

}  // namespace lsdyna
}  // namespace sintax

#endif  // LSDYNA_COVERTER_SINTAX_H