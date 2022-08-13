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

#ifndef LSDYNA_COVERTER_SINTAX_H
#define LSDYNA_COVERTER_SINTAX_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QVector>

#include "elementparser.h"
#include "keywords.h"

namespace syntax {
namespace lsdyna {

/**
 * @brief The ConverterSyntax class interpreter of the LS-Dyna format, reads the
 * sections of the document by checking that they correspond to the parts of
 * interest, leaves the specific operation when the header in the document
 * changes.
 */
class ConverterSyntax : public QThread {
  Q_OBJECT
 public:
  /**
   * @brief Construct a new ConverterSyntax object
   *
   */
  explicit ConverterSyntax(QObject *parent = nullptr);

  /**
   * @brief Destroy the ConverterSyntax object
   *
   */
  ~ConverterSyntax() override = default;

  /**
   * @brief Checks the current string, if match with a LS-DYNA's keywords.
   *
   * @param line QString holds text
   */
  void testInputLine(const QString &line);

  /**
   * @brief Extract values from the current string.
   *
   * @param line QString holds text
   */
  void parseLine(const QString &line);

  /**
   * @brief Set the Input File object
   *
   * @param filename QString holds the entire path to file.
   */
  void setInputFile(const QString &filename);

  /**
   * @brief Get the Nodes object
   *
   * @return QVector<PropertyNode<quint64, qreal>>
   */
  [[nodiscard]] QVector<PropertyNode<quint64, qreal>> getNodes() const;

  /**
   * @brief Get the Elements object
   *
   * @return QVector<ShellFourNode>
   */
  [[nodiscard]] QVector<ShellFourNode> getElements() const;

  void run() override;

 public slots:
  void filenameChanged(const QString &filename);

 private:
  KeywordDyna doc_section_{};
  QString filename_{};
  QVector<PropertyNode<quint64, qreal>> nodes_{};
  QVector<ShellFourNode> elements_{};
  QSharedPointer<ElementParser> parser_{nullptr};
};

}  // namespace lsdyna
}  // namespace syntax

#endif  // LSDYNA_COVERTER_SINTAX_H
