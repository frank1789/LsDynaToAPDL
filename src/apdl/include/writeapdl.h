
#ifndef WRITE_APDL_H
#define WRITE_APDL_H

// #include <QString>

// #include "node.h"
// #include "shell.h"

// void writeAPDL(const QString &FileName, Node *node, Shell *shell);

#include <QFile>
#include <QScopedPointer>
#include <QString>

namespace apdl {

class Writer {
 public:
  explicit Writer(const QString& filename);
  ~Writer();

  void writeNode();
  void writeElement();

 private:
  QString filename_;
  QScopedPointer<QFile> ptr_file_{nullptr};
};

}  // namespace apdl

#endif  // WRITE_APDL_H
