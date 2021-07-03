#include "converterdialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

ConverterDialog::ConverterDialog(QWidget *parent) : QDialog(parent) {
  this->setWindowTitle("Progress...");
  this->setupLayout();
  converter_ = QSharedPointer<sintax::lsdyna::ConverterSintax>(
      new sintax::lsdyna::ConverterSintax(this));
  connect(cancel_btn_.data(), &QPushButton::clicked, [=]() {
    if (converter_->isRunning()) {
      converter_->terminate();
      this->close();
    }
  });

  connect(this, &ConverterDialog::updateProcessedFilename,
          [=](const QString &filename) {
            converter_->filenameChanged(filename);
          });
}

void ConverterDialog::setInputFile(const QString &filename) {
  filename_ = filename;
  emit updateProcessedFilename(filename_);
}

void ConverterDialog::process() {
  this->show();
  QScopedPointer<QThread> thread = QScopedPointer<QThread>(new QThread(this));
  pbar_->moveToThread(thread.data());
  thread->start();
  converter_->run();
}

///////////////////////////////////////////////////////////////////////////////
// Slot
///////////////////////////////////////////////////////////////////////////////

void ConverterDialog::changedProcessedFilename(const QString &filename) {
  if (filename != filename_) {
    this->setInputFile(filename);
  }
}

///////////////////////////////////////////////////////////////////////////////
// Layout
///////////////////////////////////////////////////////////////////////////////

void ConverterDialog::setupLayout() {
  grid_layot_ = QSharedPointer<QGridLayout>(new QGridLayout(this));

  // setup progressbar
  pbar_ = QSharedPointer<QProgressBar>(new QProgressBar(this));
  pbar_->setMinimum(0);
  pbar_->setMaximum(0);

  // setup button
  cancel_btn_ = QSharedPointer<QPushButton>(new QPushButton(this));
  cancel_btn_->setText("cancel");

  // setup grid
  grid_layot_->addWidget(pbar_.data(), 0, 5);
  //   grid_layot_->addWidget();
  grid_layot_->addWidget(cancel_btn_.data(), 2, 5);
}