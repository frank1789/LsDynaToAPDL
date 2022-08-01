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
#include <QFuture>
#include <QFutureWatcher>
#include <QProgressDialog>
#include <QtConcurrent>

#include "logger_tools.h"
#include "writeapdl.h"

namespace core {

Parser::Parser(QObject* parent)
    : QObject(parent),
      filemanager_(new FileManager),
      converter_(new ConverterDialog) {
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
  converter_->process();
  emit finished();
}

void Parser::readFilename(const QString& filename) { return; }

void Parser::writeToFile() {
  // Create a progress dialog.
  QProgressDialog dialog;
  dialog.setLabelText(
      QStringLiteral("Write to file").arg(QThread::idealThreadCount()));

  // Create a QFutureWatcher and connect signals and slots.
  // Monitor progress changes of the future
  QFutureWatcher<void> futureWatcher;
  QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
  QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
  QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int, int)),
                   &dialog, SLOT(setRange(int, int)));
  QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog,
                   SLOT(setValue(int)));

  // Start the computation.
  futureWatcher.setFuture(QtConcurrent::run([this] {
    QScopedPointer<apdl::Writer> w(
        new apdl::Writer(filemanager_->getOutputfile()));

    w->writeNode();
    w->writeElement();
  }));

  // Display the dialog and start the event loop.
  dialog.exec();

  futureWatcher.waitForFinished();

  // Query the future to check if was canceled.
  qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
}

}  // namespace core
