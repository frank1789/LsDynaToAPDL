#ifndef CORE_ABOUT_H
#define CORE_ABOUT_H

#include <QCloseEvent>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QString;
class QPushButton;
class QGridLayout;
class QLabel;
QT_END_NAMESPACE

namespace core {

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

  QPushButton *close_btn_{nullptr};
  QGridLayout *about_layout_{nullptr};
  QLabel *author_label_{nullptr};
  QLabel *build_label_{nullptr};
  QLabel *license_label_{nullptr};
  QLabel *icon_label_{nullptr};
  void makeLayout();
};

}  // namespace core

#endif  // CORE_ABOUT_H
