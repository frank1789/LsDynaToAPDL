#ifndef LSDYNA_CONVERTER_DIALOG_H
#define LSDYNA_CONVERTER_DIALOG_H

#include <QDialog>
#include <QString>
#include <QTimer>
#include <QTime>

#include "converter.h"

QT_BEGIN_NAMESPACE
class QGridLayout;
class QProgressBar;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class ConverterDialog : public QDialog {
  Q_OBJECT
 public:
  explicit ConverterDialog(QWidget *parent = nullptr);
  ~ConverterDialog() override;

  void process();

  void setInputFile(const QString &filename);

 public slots:
  void changedProcessedFilename(const QString &filename);

 signals:
  void start();
  void stop();
  void closed();
  void updateProcessedFilename(const QString &filename);

  protected:
  void showEvent(QShowEvent *event) override;

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
  sintax::lsdyna::ConverterSintax *converter_{nullptr};
  QTimer* timer_{nullptr};
  QTime* elapsed_time_{nullptr};
};

#endif  // LSDYNA_CONVERTER_DIALOG_H
