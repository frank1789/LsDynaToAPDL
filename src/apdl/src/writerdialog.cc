#include "writerdialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QDebug>

#include "logger_tools.h"

namespace apdl {

constexpr uint8_t kTimeInterval{10};  // [ms]

template <typename T>
inline void safe_delete(T *&pointer) {
  if (pointer != nullptr) {
    delete pointer;
    pointer = nullptr;
  }
}

WriterDialog::WriterDialog(QWidget *parent) :
    QDialog(parent),
    filename_(""),
    timer_(new QTimer),
    elapsed_time_(new QTime(0, 0, 0, 0)) {
  setWindowFlag(Qt::WindowStaysOnTopHint);
  this->setWindowTitle(QStringLiteral("Processing file"));
  this->setupLayout();
  // converter_ = new sintax::lsdyna::ConverterSintax;
  // converter_->wait();
  QObject::connect(cancel_btn_, &QPushButton::clicked, this, [this]() {
    qWarning().noquote() << INFOFILE << "terminate current thread";
    // converter_->quit();
    // converter_->wait();
    timer_->stop();
    *elapsed_time_ = QTime(0, 0, 0, 0);
    close();
    emit closed();
  });

  // QObject::connect(converter_, &sintax::lsdyna::ConverterSintax::finished, this,
  //                  &WriterDialog::close);

  timer_->setInterval(kTimeInterval);
  QObject::connect(timer_, &QTimer::timeout, this, [=]() {
    *elapsed_time_ = elapsed_time_->addMSecs(kTimeInterval);
    time_label_->setText(elapsed_time_->toString("hh:mm:ss.zzz"));
  });
}

WriterDialog::WriterDialog(const QString &filename, QWidget *parent) :
    QDialog(parent),
    filename_(filename),
    timer_(new QTimer),
    elapsed_time_(new QTime(0, 0, 0, 0)) {
  setWindowFlag(Qt::WindowStaysOnTopHint);
  this->setWindowTitle(QStringLiteral("Processing file"));
  this->setupLayout();
  // converter_ = new sintax::lsdyna::ConverterSintax;
  // converter_->wait();
  QObject::connect(cancel_btn_, &QPushButton::clicked, this, [this]() {
    qWarning().noquote() << INFOFILE << "terminate current thread";
    // converter_->quit();
    // converter_->wait();
    timer_->stop();
    *elapsed_time_ = QTime(0, 0, 0, 0);
    close();
    emit closed();
  });

  timer_->setInterval(kTimeInterval);
  QObject::connect(timer_, &QTimer::timeout, this, [=]() {
    *elapsed_time_ = elapsed_time_->addMSecs(kTimeInterval);
    time_label_->setText(elapsed_time_->toString("hh:mm:ss.zzz"));
  });
}

WriterDialog::~WriterDialog() {
  safe_delete(grid_layot_);
  safe_delete(file_label_);
  safe_delete(filename_label_);
  safe_delete(elapsedtime_label_);
  safe_delete(time_label_);
  safe_delete(label_);
  safe_delete(pbar_);
  safe_delete(cancel_btn_);

  // stop thread
  // converter_->quit();
  // converter_->wait();
  // safe_delete(converter_);
  safe_delete(elapsed_time_);
  safe_delete(timer_);
}

void WriterDialog::setOutputFilename(const QString &filename) {
  if (QString::compare(filename_, filename, Qt::CaseInsensitive) != 0) {
    filename_ = filename;
  }
}

void WriterDialog::process() { 
  if(!filename_.isEmpty()) {

  //converter_->run();
  } else {
    qWarning().noquote() << INFOFILE << "invalid filename_";
    return;
  }
}

void WriterDialog::showEvent(QShowEvent *event) {
  qDebug().noquote() << INFOFILE << "ui has shown then execute thread";
  timer_->start();
  // converter_->start();
  Q_UNUSED(event)
}

void WriterDialog::closeEvent(QCloseEvent *e) {
  qDebug().noquote() << INFOFILE << "ui has closed then wait thread";
  timer_->stop();
  Q_UNUSED(e)
}

///////////////////////////////////////////////////////////////////////////////
// Layout
///////////////////////////////////////////////////////////////////////////////

void WriterDialog::setupLayout() {
  grid_layot_ = new QGridLayout(this);

  file_label_ = new QLabel(QStringLiteral("Process file: "));
  elapsedtime_label_ = new QLabel(QStringLiteral("Elapsed time: "));
  time_label_ = new QLabel(elapsed_time_->toString("hh:mm:ss.zzz"));
  filename_label_ = new QLabel(filename_);

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

}  // namespace apdl
