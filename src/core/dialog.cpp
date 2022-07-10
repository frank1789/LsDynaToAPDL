#include "dialog.h"

#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolTip>
#include <QValidator>

#include "ui_dialog.h"

namespace core {

static constexpr quint8 kMinimumFields{16};

Dialog::Dialog(QWidget *parent) : QDialog(parent) {
  fixed_fields_.reserve(kMinimumFields);
  ok_btn_.reset(new QPushButton(QStringLiteral("OK")));
  cancel_btn_.reset(new QPushButton(QStringLiteral("Cancel")));
  add_field_btn_.reset(new QPushButton(QStringLiteral("+")));
  mat_linedt_.reset(new QLineEdit());
  ym_linedt_.reset(new QLineEdit());
  sh_linedt_.reset(new QLineEdit());
  psm_linedt_.reset(new QLineEdit());

  // connect button
  QObject::connect(ok_btn_.get(), &QPushButton::clicked, this,
                   &Dialog::accepted);
  QObject::connect(cancel_btn_.get(), &QPushButton::clicked, this,
                   &Dialog::rejected);

  QObject::connect(add_field_btn_.get(), &QPushButton::clicked, this,
                   &Dialog::addUserDefinedField);

  QObject::connect(mat_linedt_.get(), &QLineEdit::textChanged, this,
                   &Dialog::onMaterialTextChanged);
  QObject::connect(ym_linedt_.get(), &QLineEdit::textChanged, this,
                   &Dialog::onYoungModulTextChanged);
  QObject::connect(sh_linedt_.get(), &QLineEdit::textChanged, this,
                   &Dialog::onShearModulTextChanged);
  QObject::connect(psm_linedt_.get(), &QLineEdit::textChanged, this,
                   &Dialog::onPoissonTextChanged);

  addFixedField();

  // empty string to set parameter from user
  _datauser._material_.clear();
  _datauser._youngmodulus_.clear();
  _datauser._shearmodulus_.clear();
  _datauser._poissonmodulus_.clear();
  //  ui->materialname->setToolTip("prova");
  //  ui->youngvalue->setToolTip(
  //      "Young's modulus (E) describes tensile elasticity, "
  //      "or the tendency of an object to deform along an "
  //      "axis when opposing forces are applied along that "
  //      "axis;\n it is defined as the ratio of tensile stress "
  //      "to tensile strain.\n It is often referred to simply "
  //      "as the elastic modulus.");
  //  ui->shearvalue->setToolTip(
  //      "The shear modulus or modulus of rigidity (G) "
  //      "describes an object's tendency to shear (the "
  //      "deformation of shape at constant volume) when "
  //      "acted upon by opposing forces;\n it is defined as "
  //      "shear stress over shear strain.\n The shear modulus "
  //      "is part of the derivation of viscosity.");
  //  ui->poissonvalue->setToolTip(
  //      "Poisson's ratio, denoted by the Greek letter 'nu', "
  //      "is the negative of the ratio of (signed) transverse "
  //      "strain to (signed) axial strain.\n For small values of "
  //      "these changes, is the amount of transversal expansion "
  //      "divided by the amount of axial compression.");
}

Dialog::~Dialog() {
  qDeleteAll(fixed_fields_);
  qDebug() << "pass here after close";
}

void Dialog::onMaterialTextChanged(const QString &value) { qDebug() << value; }

auto Dialog::setFloatNumberValidator(const QString &value_range) {
  QRegExp value_regex("^" + value_range + "$");
  QRegExpValidator *number_validator = new QRegExpValidator(value_regex, this);
  return number_validator;
}

void Dialog::onYoungModulTextChanged(const QString &value) {
  ym_linedt_->setValidator(setFloatNumberValidator());
  qDebug() << value;
}

void Dialog::onShearModulTextChanged(const QString &value) {
  sh_linedt_->setValidator(setFloatNumberValidator());
  qDebug() << value;
}

void Dialog::onPoissonTextChanged(const QString &value) {
  psm_linedt_->setValidator(
      setFloatNumberValidator(R"((\+?\-?[0-9]\d+\.[0-9]\d+))"));
  if (psm_linedt_->hasAcceptableInput()) {
    qDebug() << value;
  }
}

void Dialog::accepted() {
  qDebug() << "Pressed OK";
  //  qDebug() << "Data insered from user:" << _datauser._material_
  //           << _datauser._youngmodulus_ << _datauser._shearmodulus_
  //           << _datauser._poissonmodulus_;
  this->close();
}

void Dialog::rejected() {
  qDebug() << "Pressed Cancel";
  this->close();
}

void Dialog::on_materialname_textChanged(const QString &arg1) {
  qDebug() << arg1;
  _datauser._material_ = arg1;
}

void Dialog::on_youngvalue_textChanged(const QString &arg1) {
  qDebug() << arg1;
  _datauser._youngmodulus_ = arg1;
}

void Dialog::on_shearvalue_textChanged(const QString &arg1) {
  qDebug() << arg1;
  _datauser._shearmodulus_ = arg1;
}

void Dialog::on_poissonvalue_textChanged(const QString &arg1) {
  qDebug() << arg1;
  _datauser._poissonmodulus_ = arg1;
}

// void Dialog::sendData()
//{
//    emit _datauser;
//}

void Dialog::addFixedField() {
  btn_layout_.reset(new QHBoxLayout);

  mat_label_.reset(new QLabel(QStringLiteral("Material name")));
  ym_label_.reset(new QLabel(QStringLiteral("Young's modulus (E)")));
  sh_label_.reset(new QLabel(QStringLiteral("The shear modulus (G)")));
  psm_label_.reset(new QLabel(QStringLiteral("Poisson's ratio")));

  fields_layout_.reset(new QVBoxLayout(this));

  //  fixed_layout_.reset(new QHBoxLayout);

  for (std::size_t i = 0; i < 4; i++) {
    fixed_fields_.push_back(new QHBoxLayout());
  }

  fixed_fields_[0]->addWidget(mat_label_.get(), Qt::AlignLeft);
  fixed_fields_[1]->addWidget(ym_label_.get(), Qt::AlignLeft);
  fixed_fields_[2]->addWidget(sh_label_.get(), Qt::AlignLeft);
  fixed_fields_[3]->addWidget(psm_label_.get(), Qt::AlignLeft);
  fixed_fields_[0]->addWidget(mat_linedt_.get());
  fixed_fields_[1]->addWidget(ym_linedt_.get());
  fixed_fields_[2]->addWidget(sh_linedt_.get());
  fixed_fields_[3]->addWidget(psm_linedt_.get());

  for (auto fixed_field : qAsConst(fixed_fields_)) {
    fields_layout_->addLayout(fixed_field);
  }

  btn_layout_->addWidget(ok_btn_.get(), Qt::AlignRight);
  btn_layout_->addWidget(cancel_btn_.get(), Qt::AlignRight);
  btn_layout_->addWidget(add_field_btn_.get(), Qt::AlignRight);
  btn_layout_->setAlignment(Qt::AlignBottom);

  //  layout_->addLayout(row_layout_.get(), c + 3, r + 1);

  fields_layout_->addLayout(btn_layout_.get());
}

void Dialog::addUserDefinedField() {
  auto last = fixed_fields_.size();

  fixed_fields_.push_back(new QHBoxLayout);
  fixed_fields_[last]->addWidget(new QLineEdit);
  fixed_fields_[last]->addWidget(new QLineEdit);
  for (auto i = 0; i < last; i++) {
    fields_layout_->addLayout(fixed_fields_[i]);
  }
}

}  // namespace core
