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

#include <spdlog/spdlog.h>

namespace lsdynatoapdl {

Parser::Parser() :
  m_filehandler(std::make_unique<FileManager>()), 
  m_converter(std::make_unique<syntax::lsdyna::ConverterSyntax>()) {
  spdlog::info("initialize parser");
  //   QDialog(parent),
  //   filemanager_(new FileManager),
  //   converter_(new syntax::lsdyna::ConverterSyntax),
  //   writer_(new apdl::Writer),
  //   timer_(new QTimer),
  //   elapsed_time_(new QTime(0, 0, 0, 0)) {
  // setWindowFlag(Qt::WindowStaysOnTopHint);
  // this->setWindowTitle(kTitleIncipit);
  // this->setupLayout();

  // QObject::connect(cancel_btn_, &QPushButton::clicked, this, [this]() {
  //   qWarning().noquote() << INFOFILE << "cancel requested";
  //   //    worker_->stopOperations();
  //   timer_->stop();
  //   close();
  //   *elapsed_time_ = QTime(0, 0, 0, 0);
  //   // emit closed();
  // });

  // timer_->setInterval(kTimeInterval);

  // QObject::connect(converter_.get(),
  // &syntax::lsdyna::ConverterSyntax::finished,
  //                  this, &Parser::writeToFile);
  // QObject::connect(writer_.get(), &apdl::Writer::finished, this, [this]() {
  //   qDebug().noquote() << INFOFILE << "finished, restore MainWindow";
  //   close();
  //   emit finished();
  // });
}

void Parser::elaborate() {
  m_converter->setInputFile("/Users/francesco/Documents/Projects/LsDynaToAPDL/dynain-trav_nomesh_mod.k");
  if(m_converter->isReady())
  {
    m_converter->parse();
  }
  throw std::runtime_error("my error");
}

// void Parser::elaborateFilename(const QString &filename) {
//   if (filename.isEmpty()) {
//     qWarning().noquote() << INFOFILE << "[WARNING] invalid file:" <<
//     filename; return;
//   }

//   filemanager_->setFilename(filename);
//   this->setWindowTitle(kTitleIncipit + filemanager_->getFilename());
//   in_filename_label_->setText(filemanager_->getCompleteFilename());
//   out_filename_label_->setText(filemanager_->getOutputFile());

//   converter_->setInputFile(filemanager_->getCompleteFilename());
//   writer_->setFilename(filemanager_->getOutputFile());
//   QTimer::singleShot(500, this, [this]() { this->exec(); });
// }

// void Parser::writeToFile() {
//   qDebug().noquote() << INFOFILE << "ready to write on file:"
//                      << filemanager_->getOutputFile();
//   writer_->start();
// }

}  // namespace lsdynatoapdl
