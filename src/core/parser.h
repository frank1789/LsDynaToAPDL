/**
 * @file parser.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The parser class holds the components and permit the transformation
 * for both syntax.
 * @version 0.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LSDYNA_TO_APDL_PARSER_H
#define LSDYNA_TO_APDL_PARSER_H

#include <QDialog>
#include <QScopedPointer>

#include "converter.h"
#include "filemanager.h"
#include "writeapdl.h"

QT_BEGIN_NAMESPACE
class QGridLayout;
class QProgressBar;
class QLabel;
class QPushButton;
class QTime;
class QTimer;
QT_END_NAMESPACE

namespace core {

class Parser : public QDialog {
  Q_OBJECT

 public:
  explicit Parser(QWidget *parent = nullptr);
  ~Parser() override;

 public slots:
  void elaborateFilename(const QString &filename);
  void writeToFile();

 signals:

 protected:
  void showEvent(QShowEvent *event) override;
  void closeEvent(QCloseEvent *event) override;

 private:
  QScopedPointer<FileManager> filemanager_{nullptr};
  QScopedPointer<syntax::lsdyna::ConverterSyntax> converter_{nullptr};
  QScopedPointer<apdl::Writer> writer_{nullptr};

  void setupLayout();
  QGridLayout *grid_layot_{nullptr};
  QLabel *in_file_label_{nullptr};
  QLabel *out_file_label_{nullptr};
  QLabel *in_filename_label_{nullptr};
  QLabel *out_filename_label_{nullptr};
  QLabel *elapsedtime_label_{nullptr};
  QLabel *time_label_{nullptr};
  QLabel *label_{nullptr};
  QProgressBar *pbar_{nullptr};
  QPushButton *cancel_btn_{nullptr};
  QTimer *timer_{nullptr};
  QTime *elapsed_time_{nullptr};
};

}  // namespace core

#endif  // LSDYNA_TO_APDL_PARSER_H
