/**
 * @file writerdialog.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief UI shows information while writes the output file.
 * @version 0.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef APDL_WRITER_DIALOG_H
#define APDL_WRITER_DIALOG_H

#include <QDialog>
#include <QTime>
#include <QTimer>

#include "writeapdl.h"

QT_BEGIN_NAMESPACE
class QGridLayout;
class QProgressBar;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

namespace apdl {

class WriterDialog : public QDialog {
  Q_OBJECT
 public:
  /**
   * @brief Construct a new Writer Dialog object
   *
   * @param parent QObject parent pointer
   */
  explicit WriterDialog(QWidget *parent = nullptr);

  /**
   * @brief Destroy the Writer Dialog object
   *
   */
  ~WriterDialog() override;

  /**
   * @brief Set the Output Filename object
   *
   * @param filename QString holds the output filename
   */
  void setOutputFilename(const QString &filename);

 signals:
  void started();
  void stopped();
  void closed();
  void outputFilenameChanged();

 protected:
  void showEvent(QShowEvent *event) override;
  void closeEvent(QCloseEvent *e) override;

 private:
  void setupLayout();
  QString filename_;
  QGridLayout *grid_layot_{nullptr};
  QLabel *file_label_{nullptr};
  QLabel *filename_label_{nullptr};
  QLabel *elapsedtime_label_{nullptr};
  QLabel *time_label_{nullptr};
  QLabel *label_{nullptr};
  QProgressBar *pbar_{nullptr};
  QPushButton *cancel_btn_{nullptr};
  QTimer *timer_{nullptr};
  QTime *elapsed_time_{nullptr};
  Writer *converter_{nullptr};
};

}  // namespace apdl

#endif  // APDL_WRITER_DIALOG_H
