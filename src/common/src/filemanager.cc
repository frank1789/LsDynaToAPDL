#include "filemanager.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFuture>
#include <QRegularExpression>
#include <QScopedPointer>
#include <QThread>

#include "logger_tools.h"
#include <QtConcurrent/QtConcurrentRun>

FileManager::FileManager(QObject *parent) : QObject(parent) {}

FileManager::~FileManager() { qDebug().noquote() << INFOFILE << "dtor FileManager"; }

void FileManager::setFilename(const char *filename) { setFilename(QString::fromLatin1(filename)); }

void FileManager::setFilename(const std::string &filename) { setFilename(QString::fromStdString(filename)); }

void FileManager::setFilename(const QString &filename) {
  if (isValidFile(filename)) {
    qDebug().noquote() << INFOFILE << "process valid file:" << filename;
    QFile f(filename);
    QFileInfo fileInfo(f.fileName());
    complete_filename_ = fileInfo.absoluteFilePath();
    filename_ = fileInfo.fileName();
    extractFileFeatures(filename);
    setNewFilename(filename);
  }
}

QString FileManager::getFilename() const { return filename_; }

QString FileManager::getCompleteFilename() const { return complete_filename_; }

QString FileManager::getOutputFile() const { return out_filename_; }

void FileManager::setNewFilename(const QString &filename) {
  static const QRegularExpression re("(\\.\\w+)");
  static const QString replace = "_converted.txt";
  out_filename_ = filename;
  out_filename_.replace(re, replace);
  emit outputFilenameChanged(out_filename_);
  qDebug().noquote() << INFOFILE << out_filename_;
}

void FileManager::extractFileFeatures(const QString &filename) {
  QFile file(filename);
  file_size_ = file.size();
  sizeFileChanged(file_size_);

  // TO-DO maybe analize the file contents
  //  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
  //    uint64_t counter_line = 0;

  //    QFuture<void> t3 = QtConcurrent::run(
  //        [&file](uint64_t *c) {
  //          QTextStream in(&file);
  //          while (!in.atEnd()) {
  //            (*c)++;
  //            auto str = in.readLine();
  //            std::cout << "found n lines " << *c << " " << str.toStdString()
  //                      << std::endl;
  //          }
  //        },
  //        &counter_line);

  //    t3.waitForFinished();
  //    std::cout << "found n lines " << counter_line << std::endl;
  //    file.close();
  //  }
}

bool FileManager::isValidFile(const QString &filename) {
  bool file_exists = QFileInfo::exists(filename) && QFileInfo(filename).isFile();
  if (file_exists) {
    return true;
  }

  return false;
}
