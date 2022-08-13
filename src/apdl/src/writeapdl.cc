/**
 * @file writeapdl.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Writer is a class that writes information within APDL syntax.
 * @version 0.4.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "writeapdl.h"

#include <QDebug>
#include <QFileDevice>
#include <QTextStream>

#include "logger_tools.h"

namespace apdl {

Writer::Writer(QObject *parent) : QThread(parent) { wait(); }

Writer::Writer(const QString &filename, QObject *parent) : QThread(parent), filename_(filename) {
  wait();
  ptr_file_.reset(new QFile(filename_));
}

void Writer::setFilename(const QString &filename) {
  if (!filename.isEmpty()) {
    filename_ = filename;
  }
}

void Writer::run() {
  if (!filename_.isEmpty()) {
    if (ptr_file_ == nullptr) {
      ptr_file_.reset(new QFile(filename_));
      if (ptr_file_->open(QIODevice::WriteOnly | QIODevice::Text)) {
        /* Do something */
      }

      ptr_file_->close();
    }
  } else {
    qWarning().noquote() << INFOFILE << "invalid or empty filename, please use setFilename(const QString &filename)";
    quit();
  }
}

void Writer::writeNode() {
  if (ptr_file_->open(QIODevice::WriteOnly | QIODevice::Append)) {
    QTextStream stream(ptr_file_.get());
    stream << "/PREP7\n\n";
    // write node section
    stream << "! >>>>> node definition <<<<<\n\n";
    // node format N, NODE, X, Y, Z, THXY, THYZ, THZX
    stream << "\n\n! >>>>> end node definition <<<<<\n\n";
  }

  ptr_file_->close();
}

void Writer::writeElement() {
  if (ptr_file_->open(QIODevice::WriteOnly | QIODevice::Append)) {
    QTextStream stream(ptr_file_.get());
    stream << "! >>>>> elements shell element <<<<<\n\n";
    stream << "! >>>>> end elements definition <<<<<\n\n";
  }

  ptr_file_->close();
}
}  // namespace apdl
