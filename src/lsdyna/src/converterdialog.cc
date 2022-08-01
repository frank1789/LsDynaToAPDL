#include "converterdialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

ConverterDialog::ConverterDialog(QWidget *parent) : QDialog(parent) {
  this->setWindowTitle(QStringLiteral("Processing file"));
  this->setupLayout();
  converter_ = new sintax::lsdyna::ConverterSintax;
  QObject::connect(cancel_btn_, &QPushButton::clicked, this, [this]() {
    qWarning().noquote() << INFOFILE << "terminate current thread";
    converter_->quit();
    close();
    emit closed();
  });

  QObject::connect(converter_, &sintax::lsdyna::ConverterSintax::finished, this,
                   &ConverterDialog::close);

  connect(this, &ConverterDialog::updateProcessedFilename, this,
          [this](const QString &filename) {
            converter_->filenameChanged(filename);
          });
  setWindowFlag(Qt::WindowStaysOnTopHint);
}

ConverterDialog::~ConverterDialog() {
  delete grid_layot_;
  grid_layot_ = nullptr;
  delete pbar_;
  pbar_ = nullptr;
  delete label_;
  label_ = nullptr;
  delete cancel_btn_;
  cancel_btn_ = nullptr;
  delete converter_;
  converter_ = nullptr;
}

void ConverterDialog::setInputFile(const QString &filename) {
  filename_ = filename;
  emit updateProcessedFilename(filename_);
}

void ConverterDialog::process() { converter_->run(); }

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
  grid_layot_ = new QGridLayout(this);

  // setup progressbar
  pbar_ = new QProgressBar;
  pbar_->setMinimum(0);
  pbar_->setMaximum(0);

  // setup button
  cancel_btn_ = new QPushButton;
  cancel_btn_->setText(QStringLiteral("cancel"));

  // setup grid
  grid_layot_->addWidget(pbar_, 0, 5);
  //   grid_layot_->addWidget();
  grid_layot_->addWidget(cancel_btn_, 2, 5);
}
