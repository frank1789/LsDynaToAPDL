/**
 * @file parser.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The parser implementation.
 * @version 0.4.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "parser.h"

#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QTime>
#include <QTimer>

#include "logger_tools.h"

namespace core {

constexpr uint8_t kTimeInterval{10};  // [ms]
static const auto kTitleIncipit = QStringLiteral("Processing file: ");

template <typename T>
inline void safe_delete(T *&pointer) {
  if (pointer != nullptr) {
    delete pointer;
    pointer = nullptr;
  }
}

Parser::Parser(QWidget *parent) :
    QDialog(parent),
    filemanager_(new FileManager),
    converter_(new syntax::lsdyna::ConverterSyntax),
    writer_(new apdl::Writer),
    timer_(new QTimer),
    elapsed_time_(new QTime(0, 0, 0, 0)) {
  setWindowFlag(Qt::WindowStaysOnTopHint);
  this->setWindowTitle(kTitleIncipit);
  this->setupLayout();

  QObject::connect(cancel_btn_, &QPushButton::clicked, this, [this]() {
    qWarning().noquote() << INFOFILE << "cancel requested";
    //    worker_->stopOperations();
    timer_->stop();
    close();
    *elapsed_time_ = QTime(0, 0, 0, 0);
    // emit closed();
  });

  timer_->setInterval(kTimeInterval);

  QObject::connect(converter_.get(), &syntax::lsdyna::ConverterSyntax::finished, this, &Parser::writeToFile);
  QObject::connect(writer_.get(), &apdl::Writer::finished, this, [this]() {
    qDebug().noquote() << INFOFILE << "finished, restore MainWindow";
    close();
    emit finished();
  });
}

Parser::~Parser() {
  safe_delete(grid_layot_);
  safe_delete(in_file_label_);
  safe_delete(in_filename_label_);
  safe_delete(out_file_label_);
  safe_delete(out_filename_label_);
  safe_delete(elapsedtime_label_);
  safe_delete(time_label_);
  safe_delete(label_);
  safe_delete(pbar_);
  safe_delete(cancel_btn_);
  safe_delete(elapsed_time_);
  safe_delete(timer_);
}

void Parser::showEvent(QShowEvent *event) {
  qDebug().noquote() << INFOFILE << "ui has shown then execute thread";
  timer_->start();
  converter_->start();
  Q_UNUSED(event)
}

void Parser::closeEvent(QCloseEvent *event) {
  qDebug().noquote() << INFOFILE << "ui has closed then wait thread";
  timer_->stop();
  Q_UNUSED(event)
}

///////////////////////////////////////////////////////////////////////////////
// Layout
///////////////////////////////////////////////////////////////////////////////

void Parser::setupLayout() {
  grid_layot_ = new QGridLayout(this);

  in_file_label_ = new QLabel(QStringLiteral("input file: "));
  out_file_label_ = new QLabel(QStringLiteral("output file: "));
  elapsedtime_label_ = new QLabel(QStringLiteral("Elapsed time: "));
  time_label_ = new QLabel(elapsed_time_->toString("hh:mm:ss.zzz"));
  in_filename_label_ = new QLabel("");
  out_filename_label_ = new QLabel("");

  // setup progress-bar
  pbar_ = new QProgressBar;
  pbar_->setMinimum(0);
  pbar_->setMaximum(0);

  // setup button
  cancel_btn_ = new QPushButton;
  cancel_btn_->setText(QStringLiteral("cancel"));

  // setup grid
  grid_layot_->addWidget(in_file_label_, 0, 0, 1, 1);
  grid_layot_->addWidget(in_filename_label_, 0, 1, 1, 3);
  grid_layot_->addWidget(out_file_label_, 1, 0, 1, 1);
  grid_layot_->addWidget(out_filename_label_, 1, 1, 1, 3);
  grid_layot_->addWidget(elapsedtime_label_, 2, 0, 1, 1);
  grid_layot_->addWidget(time_label_, 2, 1, 1, 3);
  grid_layot_->addWidget(pbar_, 3, 0, 1, 5);
  grid_layot_->addWidget(cancel_btn_, 4, 4, 1, 1);

  QObject::connect(timer_, &QTimer::timeout, this, [=]() {
    *elapsed_time_ = elapsed_time_->addMSecs(kTimeInterval);
    time_label_->setText(elapsed_time_->toString("hh:mm:ss.zzz"));
  });
}

void Parser::elaborateFilename(const QString &filename) {
  if (filename.isEmpty()) {
    qWarning().noquote() << INFOFILE << "[WARNING] invalid file:" << filename;
    return;
  }

  filemanager_->setFilename(filename);
  this->setWindowTitle(kTitleIncipit + filemanager_->getFilename());
  in_filename_label_->setText(filemanager_->getCompleteFilename());
  out_filename_label_->setText(filemanager_->getOutputFile());

  converter_->setInputFile(filemanager_->getCompleteFilename());
  writer_->setFilename(filemanager_->getOutputFile());
  QTimer::singleShot(500, this, [this]() { this->exec(); });
}

void Parser::writeToFile() {
  qDebug().noquote() << INFOFILE << "ready to write on file:" << filemanager_->getOutputFile();
  writer_->start();
}

}  // namespace core
