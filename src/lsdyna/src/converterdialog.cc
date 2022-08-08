#include "converterdialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

constexpr uint8_t kTimeInterval{10};

template <typename T>
inline void safe_delete(T *&pointer) {
  if (pointer != nullptr) {
    delete pointer;
    pointer = nullptr;
  }
}

ConverterDialog::ConverterDialog(QWidget *parent) :
    QDialog(parent), timer_(new QTimer), elapsed_time_(new QTime(0, 0, 0, 0)) {
  setWindowFlag(Qt::WindowStaysOnTopHint);
  this->setWindowTitle(QStringLiteral("Processing file"));
  this->setupLayout();
  converter_ = new syntax::lsdyna::ConverterSyntax;
  converter_->wait();
  QObject::connect(cancel_btn_, &QPushButton::clicked, this, [this]() {
    qWarning().noquote() << INFOFILE << "terminate current thread";
    converter_->quit();
    converter_->wait();
    timer_->stop();
    *elapsed_time_ = QTime(0, 0, 0, 0);
    close();
    emit closed();
  });

  QObject::connect(converter_, &syntax::lsdyna::ConverterSyntax::finished, this, &ConverterDialog::close);

  connect(this, &ConverterDialog::updateProcessedFilename, this,
          [this](const QString &filename) { converter_->filenameChanged(filename); });

  timer_->setInterval(kTimeInterval);
  QObject::connect(timer_, &QTimer::timeout, this, [=]() {
    *elapsed_time_ = elapsed_time_->addMSecs(kTimeInterval);
    time_label_->setText(elapsed_time_->toString("hh:mm:ss.zzz"));
  });
}

ConverterDialog::~ConverterDialog() {
  safe_delete(grid_layot_);
  safe_delete(file_label_);
  safe_delete(filename_label_);
  safe_delete(elapsedtime_label_);
  safe_delete(time_label_);
  safe_delete(label_);
  safe_delete(pbar_);
  safe_delete(cancel_btn_);

  // stop thread
  converter_->quit();
  converter_->wait();
  safe_delete(converter_);
  safe_delete(elapsed_time_);
  safe_delete(timer_);
}

void ConverterDialog::setInputFile(const QString &filename) {
  filename_ = filename;
  filename_label_->setText(filename);
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

void ConverterDialog::showEvent(QShowEvent *event) {
  qDebug().noquote() << INFOFILE << "ui has shown then execut thread";
  timer_->start();
  converter_->start();
  Q_UNUSED(event)
}

///////////////////////////////////////////////////////////////////////////////
// Layout
///////////////////////////////////////////////////////////////////////////////

void ConverterDialog::setupLayout() {
  grid_layot_ = new QGridLayout(this);

  file_label_ = new QLabel(QStringLiteral("Process file: "));
  elapsedtime_label_ = new QLabel(QStringLiteral("Elapsed time: "));
  time_label_ = new QLabel(elapsed_time_->toString("hh:mm:ss.zzz"));
  filename_label_ = new QLabel("");

  // setup progressbar
  pbar_ = new QProgressBar;
  pbar_->setMinimum(0);
  pbar_->setMaximum(0);

  // setup button
  cancel_btn_ = new QPushButton;
  cancel_btn_->setText(QStringLiteral("cancel"));

  // setup grid
  grid_layot_->addWidget(file_label_, 0, 0, 1, 1);
  grid_layot_->addWidget(filename_label_, 0, 1, 1, 3);
  grid_layot_->addWidget(elapsedtime_label_, 1, 0, 1, 1);
  grid_layot_->addWidget(time_label_, 1, 1, 1, 3);
  grid_layot_->addWidget(pbar_, 2, 0, 1, 5);
  grid_layot_->addWidget(cancel_btn_, 3, 4, 1, 1);
}
