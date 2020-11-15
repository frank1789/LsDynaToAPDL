#include "managefile.h"

#include <QDebug>
#include <QFileDialog>
#include <QRegularExpression>
#include <QScopedPointer>
#include <QThread>

#include "converterdialog.h"
#include "logger_tools.h"

/**
 * @brief ManageFile::ManageFile default constructor.
 */
ManageFile::ManageFile(QObject *parent)
    : QObject(parent), counter_(0), number_of_file_(0) {}

/**
 * @brief ManageFile::~ManageFile destructor
 */
ManageFile::~ManageFile() { qDebug() << INFOFILE << "dtor ManageFile"; }

/**
 * @brief ManageFile::setSizelist
 * @param size
 */
void ManageFile::setSizelist(int size) {
  qDebug() << INFOFILE << "recive list of file:"
           << "\t" << size;
  number_of_file_ = static_cast<unsigned int>(size);
}

void ManageFile::print() {
  qDebug() << INFOFILE << "recived size list" << number_of_file_;
}

/**
 * @brief ManageFile::setPropertyFile
 */
void ManageFile::setPropertyFile() {
  QString name{""};
  QScopedPointer<QFile> file = QScopedPointer<QFile>(new QFile(filename_));
  QRegularExpression re("(\\w+?-\\w+\\.\\w+)$");  // extract name
  QRegularExpressionMatch match = re.match(filename_);
  if (match.hasMatch()) {
    name = match.captured(0);
  }
  emit getPropertyFile(file->size(), name);
}

/**
 * @brief ManageFile::setFile
 * @param fileName
 */
void ManageFile::setFile(const QString &filename) {
  filename_ = filename;
  getfileName();
  qDebug() << INFOFILE << "selected file to operate:" << filename_;
}

/**
 * @brief ManageFile::getfileName
 */
void ManageFile::getfileName() {
  // extract name
  QRegularExpression re("(\\w+?-\\w+\\.\\w+)$");
  QRegularExpressionMatch match = re.match(filename_);
  if (match.hasMatch()) {
    QString name = match.captured(0);
    setNewfileName();
    emit outputfileName(newfilename_);
  }
  setPropertyFile();
}

/**
 * @brief ManageFile::setNewfileName
 */
void ManageFile::setNewfileName() {
  newfilename_ = filename_;
  // extract name
  QRegularExpression re("(\\.\\w+)$");
  re.match(newfilename_);
  replace_ = "ApdlConverted.txt";
  qDebug() << INFOFILE << newfilename_.replace(re, replace_);
}

int ManageFile::getSizelistFile() { return number_of_file_; }

/**
 * @brief ManageFile::convert start the conversion process by reading and
 * decrypting the input file and then returning the output file to APDL.
 * @param[in] pConverter: pointer type Covertersintax.
 * @param[in] node: pointer type class Node.
 * @param[in] shell: pointer type class Shell.
 * @details In the convert method a read thread is launched for the source
 *  file by starting a wait mask during the process that closes at the end.
 *  It is possible to interrupt the process via button "cancel" you exit
 *  the function.
 *  In a similar way, another thread is launched for the sciage, also with
 *  the relative mashera of advancement it is possible to cancel.
 */

void ManageFile::convert() {
  auto convert_dialog = QScopedPointer<ConverterDialog>(new ConverterDialog());
  convert_dialog->exec();

  // //read file
  // {
  //     QProgressDialog dialog;  // Create a progress dialog.
  //     dialog.setLabelText(QString("Reading file..."));
  //     // Create a QFutureWatcher and connect signals and slots.
  //     QFutureWatcher<void> futureWatcher;
  //     QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog,
  //     SLOT(reset())); QObject::connect(&dialog, SIGNAL(canceled()),
  //     &futureWatcher, SLOT(cancel())); QObject::connect(&futureWatcher,
  //     SIGNAL(progressRangeChanged(int,int)), &dialog,
  //     SLOT(setRange(int,int))); QObject::connect(&futureWatcher,
  //     SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
  //     // Start to read file
  //     futureWatcher.setFuture(QtConcurrent::run(read, _fileName, pConverter,
  //     node, shell));
  //     // Display the dialog and start the event loop.
  //     dialog.exec();
  //     futureWatcher.waitForFinished();
  //     // Query the future to check if was canceled.
  //     qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
  // }
  // // write file
  // {
  //     QProgressDialog dialog; // Create a progress dialog.
  //     dialog.setLabelText(QString("Writing file..."));
  //     // Create a QFutureWatcher and connect signals and slots.
  //     QFutureWatcher<void> futureWatcher;
  //     QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog,
  //     SLOT(reset())); QObject::connect(&dialog, SIGNAL(canceled()),
  //     &futureWatcher, SLOT(cancel())); QObject::connect(&futureWatcher,
  //     SIGNAL(progressRangeChanged(int,int)), &dialog,
  //     SLOT(setRange(int,int))); QObject::connect(&futureWatcher,
  //     SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
  //     //Start to write file
  //     futureWatcher.setFuture(QtConcurrent::run(writeAPDL, _NewfileName,
  //     node, shell));
  //     // Display the dialog and start the event loop.
  //     dialog.exec();
  //     futureWatcher.waitForFinished();
  //     // Query the future to check if was canceled.
  //     qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
  // }
}
