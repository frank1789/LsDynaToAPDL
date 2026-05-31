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

#include "core/parser.h"

#include "spdlog/spdlog.h"

namespace lsdynatoapdl {

Parser::Parser() :
    m_file_handler(std::make_unique<FileManager>()),
    m_converter(std::make_unique<syntax::lsdyna::ConverterSyntax>()),
    m_writer(std::make_unique<apdl::Writer>()) {
  spdlog::info("Parser initialized");
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
  //  spdlog::get("multi-sink")->debug("pass here");
}

void Parser::elaborate(const std::string& input_file) {
  const auto t =
      "/Users/francesco/Documents/Projects/LsDynaToAPDL/"
      "dynain-trav_nomesh_mod.k";
  m_file_handler->setInputFilename(t);
  m_file_handler->setOutputFilename();
  m_converter->set_input_file(m_file_handler->getCompleteInputFilename());
  m_writer->set_filename(m_file_handler->getOutputFilename());
  if (m_converter->isReady()) {
    spdlog::info("converter is ready, start parsing");
    m_converter->parse();
    m_writer->write_to_file("Example content to write to file");
  } else {
    spdlog::warn("converter is not ready");
  }
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
