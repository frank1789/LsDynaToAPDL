/**
 * @file about.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-07-10
 *
 * @copyright Copyright (c) 2022
 *
 */

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

  void setNewLine(const QString &text);
 signals:
  void dialogClosed();
  void openChanged(bool);

 protected:
  // void showEvent(QShowEvent *event);
  void showEvent(QShowEvent *event) override;

 private:
  void closeEvent(QCloseEvent *event) override;
  void makeLayout();
  QString readLicense(const QString &filename);

  // sol::state lua_{};

  QPushButton *close_btn_{nullptr};
  QGridLayout *about_layout_{nullptr};
  QLabel *project_name_label_{nullptr};
  QLabel *authors_label_{nullptr};
  QLabel *build_label_{nullptr};
  QLabel *license_label_{nullptr};
  QLabel *icon_label_{nullptr};
  bool is_opening{false};
};

}  // namespace core

#endif  // CORE_ABOUT_H
