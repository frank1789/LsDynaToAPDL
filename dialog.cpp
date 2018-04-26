#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QToolTip>
#include <QValidator>



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //empty string to set parameter from user
    _datauser._material_.clear();
    _datauser._youngmodulus_.clear();
    _datauser._shearmodulus_.clear();
    _datauser._poissonmodulus_.clear();
    ui->materialname->setToolTip("prova");
    ui->youngvalue->setToolTip("Young's modulus (E) describes tensile elasticity, "
                               "or the tendency of an object to deform along an "
                               "axis when opposing forces are applied along that "
                               "axis;\n it is defined as the ratio of tensile stress "
                               "to tensile strain.\n It is often referred to simply "
                               "as the elastic modulus.");
    ui->shearvalue->setToolTip("The shear modulus or modulus of rigidity (G) "
                               "describes an object's tendency to shear (the "
                               "deformation of shape at constant volume) when "
                               "acted upon by opposing forces;\n it is defined as "
                               "shear stress over shear strain.\n The shear modulus "
                               "is part of the derivation of viscosity.");
    ui->poissonvalue->setToolTip("Poisson's ratio, denoted by the Greek letter 'nu', "
                                 "is the negative of the ratio of (signed) transverse "
                                 "strain to (signed) axial strain.\n For small values of "
                                 "these changes, is the amount of transversal expansion "
                                 "divided by the amount of axial compression.");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    qDebug() << "Pressed OK";
    qDebug() << "Data insered from user:" << _datauser._material_ << _datauser._youngmodulus_ << _datauser._shearmodulus_ << _datauser._poissonmodulus_;
}

void Dialog::on_buttonBox_rejected()
{
    qDebug() << "Pressed Cancel";
}

void Dialog::on_Dialog_accepted()
{
    qDebug() << "Pressed OK";
}

void Dialog::on_Dialog_rejected()
{
    qDebug() << "Pressed Cancel";
}

void Dialog::on_materialname_textChanged(const QString &arg1)
{
    qDebug() << arg1;
    _datauser._material_ = arg1;
}

void Dialog::on_youngvalue_textChanged(const QString &arg1)
{
    qDebug() << arg1;
    _datauser._youngmodulus_ = arg1;
}

void Dialog::on_shearvalue_textChanged(const QString &arg1)
{
    qDebug() << arg1;
    _datauser._shearmodulus_ = arg1;
}

void Dialog::on_poissonvalue_textChanged(const QString &arg1)
{
    qDebug() << arg1;
    _datauser._poissonmodulus_ = arg1;
}

//void Dialog::sendData()
//{
//    emit _datauser;
//}

