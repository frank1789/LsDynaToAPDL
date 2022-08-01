#ifndef LSDYNA_CONVERTER_DIALOG_H
#define LSDYNA_CONVERTER_DIALOG_H

#include <QDialog>
#include <QScopedPointer>
#include <QString>

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

 private:
  void setupLayout();
  QString filename_;
  QGridLayout *grid_layot_{nullptr};
  QProgressBar *pbar_{nullptr};
  QLabel *label_{nullptr};
  QPushButton *cancel_btn_{nullptr};
  sintax::lsdyna::ConverterSintax *converter_{nullptr};
};

#endif  // LSDYNA_CONVERTER_DIALOG_H
