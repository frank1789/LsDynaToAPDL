#ifndef ABOUT_H
#define ABOUT_H

#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QImage>
#include <QLabel>
#include <QTextStream>

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
  explicit About(QWidget *parent = 0);
  ~About();

 signals:
  void dialogClosed();

 private:
  void closeEvent(QCloseEvent *event);

  Ui::About *ui;
};

QString read(QString pfileName);

#endif  // ABOUT_H
