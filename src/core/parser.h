/**
 * @file parser.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The parser class holds the components and permit the transfomation for
 * both syntax.
 * @version 0.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LSDYNA_TO_APDL_PARSER_H
#define LSDYNA_TO_APDL_PARSER_H

#include <QObject>
#include <QScopedPointer>

#include "converterdialog.h"
#include "filemanager.h"

namespace core {

class Parser : public QObject {
  Q_OBJECT

 public:
  explicit Parser(QObject* paren = nullptr);

 public slots:
  void elaborateFilename(const QString& filename);
  void writeToFile();

 signals:
  void elaborationFinished(bool);
  void elaborationCanceled(bool);
  void finished();

 private:
  QScopedPointer<FileManager> filemanager_{nullptr};
  QScopedPointer<ConverterDialog> converter_{nullptr};
  void readFilename(const QString& filename);
};

}  // namespace core

#endif  // LSDYNA_TO_APDL_PARSER_H
