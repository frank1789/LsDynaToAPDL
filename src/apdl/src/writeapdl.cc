#include "writeapdl.h"

#include <QDebug>
#include <QFileDevice>
#include <QTextStream>

namespace apdl {

Writer::Writer(const QString& filename) : filename_(filename) { ptr_file_.reset(new QFile(filename_)); }

Writer::~Writer() { ptr_file_->close(); }

void Writer::writeNode() {
  if (ptr_file_->open(QIODevice::WriteOnly | QIODevice::Append)) {
    QTextStream stream(ptr_file_.get());
    stream << "/PREP7\n\n";
    // wtite node section
    stream << "! >>>>> node definition <<<<<\n\n";
    //       // node format N, NODE, X, Y, Z, THXY, THYZ, THZX
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
