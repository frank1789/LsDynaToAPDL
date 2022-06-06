#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QString>

namespace Ui {
class Dialog;
}

/**
 * @brief The Dialog class
 */
class Dialog : public QDialog {
  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = nullptr);

  ~Dialog();

signals:
  void tovalidate(const QString &text);

  void validateNumeric(const QString &text);

  //    void sendData();

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

  void on_Dialog_accepted();

  void on_Dialog_rejected();

  void on_materialname_textChanged(const QString &arg1);

  void on_youngvalue_textChanged(const QString &arg1);

  void on_shearvalue_textChanged(const QString &arg1);

  void on_poissonvalue_textChanged(const QString &arg1);

private:
  Ui::Dialog *ui;

  struct input {
    QString _material_;
    QString _youngmodulus_;
    QString _shearmodulus_;
    QString _poissonmodulus_;
  } _datauser;
};

#endif // DIALOG_H
