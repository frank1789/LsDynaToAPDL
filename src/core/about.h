#ifndef CORE_ABOUT_H
#define CORE_ABOUT_H

#include <QCloseEvent>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

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
  ~About() override;

 signals:
  void dialogClosed();

 private:
  void closeEvent(QCloseEvent *event) override;
  QString readLicense(const QString &filename);

  Ui::About *ui{nullptr};
};

#endif  // CORE_ABOUT_H
