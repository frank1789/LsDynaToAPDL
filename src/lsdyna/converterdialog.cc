#include "converterdialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

ConverterDialog::ConverterDialog(QWidget *parent) : QDialog(parent) {
  this->setWindowTitle("Progress...");
  this->setupLayout();
  // connect(cancel_btn_.data(), &QPushButton::clicked, [=]() { this->stop();
  // });

  converter_ = QSharedPointer<sintax::lsdyna::ConverterSintax>(
      new sintax::lsdyna::ConverterSintax);
  connect(cancel_btn_.data(), &QPushButton::clicked, [=]() {
    if (converter_->isRunning()) {
      converter_->terminate();
    }
    
  });
}

ConverterDialog::~ConverterDialog() {}

int ConverterDialog::exec() { converter_->run(); }

void ConverterDialog::setInputFile(const QString &filename) {}

void ConverterDialog::setupLayout() {
  grid_layot_ = QSharedPointer<QGridLayout>(new QGridLayout(this));

  // setup progressbar
  pbar_ = QSharedPointer<QProgressBar>(new QProgressBar);
  pbar_->setMinimum(0);
  pbar_->setMaximum(0);

  // setup button
  cancel_btn_ = QSharedPointer<QPushButton>(new QPushButton);
  cancel_btn_->setText("cancel");

  // setup grid
  grid_layot_->addWidget(pbar_.data(), 0, 5);
  //   grid_layot_->addWidget();
  grid_layot_->addWidget(cancel_btn_.data(), 2, 5);
}