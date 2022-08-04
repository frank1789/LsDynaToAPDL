#ifndef APDL_WRITER_DIALOG_H
#define APDL_WRITER_DIALOG_H

#include <QDialog>
#include <QString>
#include <QTime>
#include <QTimer>

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
  explicit WriterDialog(QWidget *parent = nullptr);
  explicit WriterDialog(const QString &filename, QWidget *parent = nullptr);
  ~WriterDialog() override;

  void setOutputFilename(const QString &filename);

  void process();

 public slots:

 signals:
  void started();
  void stopped();
  void closed();

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
};

}  // namespace apdl

#endif  // LSDYNA_CONVERTER_DIALOG_H
