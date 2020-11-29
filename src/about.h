#ifndef ABOUT_H
#define ABOUT_H

#include <QCloseEvent>
#include <QDialog>
#include <QString>

namespace Ui {
class About;
}

/**
 * @brief The About class show the current author and version as well as the
 *  license with which it is distributed.
 */
class About : public QDialog {
  Q_OBJECT

 public:
  explicit About(QWidget *parent = nullptr);
  ~About();

 signals:
  void dialogClosed();

 private:
  void closeEvent(QCloseEvent *event);
  QString readLicense(const QString &filename);

  Ui::About *ui;
};

#endif  // ABOUT_H
