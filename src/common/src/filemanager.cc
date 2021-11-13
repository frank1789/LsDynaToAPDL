#include "filemanager.h"

#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QScopedPointer>

#include "logger_tools.h"

ManageFile::ManageFile(QObject *parent) : QObject(parent) {}

ManageFile::~ManageFile() { qDebug() << INFOFILE << "dtor ManageFile"; }

void ManageFile::setFilename(const QString &filename) {
  filename_ = filename;
  qDebug() << INFOFILE << "process file:" << filename_;
}

QString ManageFile::getFilename() const { return filename_; }

qint64 ManageFile::extractSizeFile(const QString &filename) {
  QScopedPointer<QFile> file = QScopedPointer<QFile>(new QFile(filename));
  return file->size();
}

QString ManageFile::getOutputfile() const { return new_filename_; }

void ManageFile::setNewFilename(const QString &filename) {
  const QRegularExpression re("(\\.\\w+)");
  const QString replace = "_converted.txt";
  new_filename_ = filename;
  new_filename_.replace(re, replace);
  emit outputFilenameChanged(new_filename_);
  qDebug() << INFOFILE << new_filename_;
}

///////////////////////////////////////////////////////////////////////////////
// Slot
///////////////////////////////////////////////////////////////////////////////

void ManageFile::processedFilename(const QString &filename) {
  setNewFilename(filename);
  const QRegularExpression re(R"((\w+)(\.\w+)+(?!.*(\w+)(\.\w+)+))");
  QRegularExpressionMatch match = re.match(filename);
  if (match.hasMatch()) {
    QString name = match.captured(0);
    setFilename(name);
    emit propertyFileChanged(name, extractSizeFile(filename));
  } else {
    setFilename(filename);
  }
}
