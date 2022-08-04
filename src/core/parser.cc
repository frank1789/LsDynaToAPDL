/**
 * @file parser.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The parser implementation.
 * @version 0.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "parser.h"

#include <QDebug>

#include "logger_tools.h"
#include "writeapdl.h"

namespace core {

Parser::Parser(QObject* parent) :
    QObject(parent),
    filemanager_(new FileManager),
    converter_(new ConverterDialog),
    writer_(new apdl::WriterDialog) {
  QObject::connect(converter_.get(), &ConverterDialog::closed, this, [=]() {
    qDebug().noquote() << INFOFILE << "check for the next file";
    emit finished();
  });

  //  QObject::connect(converter_.get(), &ConverterDialog::closed, this,
  //  [=](){});

  QObject::connect(this, &Parser::finished, this, &Parser::writeToFile);
}

void Parser::elaborateFilename(const QString& filename) {
  if (filename.isEmpty()) {
    qWarning().noquote() << INFOFILE << "[WARNING] invalid file:" << filename;
    return;
  }

  filemanager_->setFilename(filename);
  converter_->setInputFile(filename);
  converter_->exec();
  emit finished();
}

void Parser::writeToFile() {
  qDebug().noquote() << INFOFILE << "ready to write on file:"
                     << filemanager_->getOutputfile();
  writer_->setOutputFilename(filemanager_->getOutputfile());
  writer_->exec();
}

}  // namespace core
