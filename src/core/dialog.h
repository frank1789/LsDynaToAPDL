#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPair>
#include <QScopedPointer>
#include <QVector>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
class QString;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
QT_END_NAMESPACE

class UserDefinedParameters {
 public:
  explicit UserDefinedParameters() noexcept = default;
  explicit UserDefinedParameters(const QString &name, const QString &value)
      : name_(name), value_(value){};

  UserDefinedParameters(const UserDefinedParameters &o);
  UserDefinedParameters(UserDefinedParameters &&o);

  UserDefinedParameters &operator=(const UserDefinedParameters &o);
  UserDefinedParameters &operator=(UserDefinedParameters &&o);

 private:
  auto reflect() const { return std::tie(name_, value_); }
  QString name_;
  QString value_;
};

namespace core {

/**
 * @brief The Dialog class
 */
class Dialog : public QDialog {
  Q_OBJECT

 public:
  explicit Dialog(QWidget *parent = nullptr);

  ~Dialog() override;

 signals:
  void tovalidate(const QString &text);

  void validateNumeric(const QString &text);

 public slots:
  void onMaterialTextChanged(const QString &value);
  void onYoungModulTextChanged(const QString &value);
  void onShearModulTextChanged(const QString &value);
  void onPoissonTextChanged(const QString &value);

 protected:
  auto setFloatNumberValidator(
      const QString &value_range = R"(([0-9]\d+\.[0-9]\d+))");
 private slots:

  void accepted();

  void rejected();

  void addUserDefinedField();

  void on_materialname_textChanged(const QString &arg1);

  void on_youngvalue_textChanged(const QString &arg1);

  void on_shearvalue_textChanged(const QString &arg1);

  void on_poissonvalue_textChanged(const QString &arg1);

 private:
  void addFixedField();

  using MaterialLayout = QPair<QLabel *, QLineEdit *>;

  QScopedPointer<QPushButton> ok_btn_{nullptr};
  QScopedPointer<QPushButton> cancel_btn_{nullptr};
  QScopedPointer<QPushButton> add_field_btn_{nullptr};
  QScopedPointer<QVBoxLayout> fields_layout_{nullptr};
  QScopedPointer<QHBoxLayout> btn_layout_{nullptr};
  QScopedPointer<QHBoxLayout> fixed_layout_{nullptr};

  QVector<QHBoxLayout *> fixed_fields_;

  QScopedPointer<QLabel> mat_label_{nullptr};
  QScopedPointer<QLabel> ym_label_{nullptr};
  QScopedPointer<QLabel> sh_label_{nullptr};
  QScopedPointer<QLabel> psm_label_{nullptr};
  QScopedPointer<QLineEdit> mat_linedt_{nullptr};
  QScopedPointer<QLineEdit> ym_linedt_{nullptr};
  QScopedPointer<QLineEdit> sh_linedt_{nullptr};
  QScopedPointer<QLineEdit> psm_linedt_{nullptr};

  struct input {
    QString _material_;
    QString _youngmodulus_;
    QString _shearmodulus_;
    QString _poissonmodulus_;
  } _datauser;
};

}  // namespace core
#endif  // DIALOG_H
