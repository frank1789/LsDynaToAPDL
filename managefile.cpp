#include "managefile.h"
#include "reader.h"
#include "writeapdl.h"
#include <QDebug>
#include <QThread>
#include <QRegularExpression>
#include <QtConcurrent>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>

/**
 * @brief ManageFile::ManageFile default constructor.
 */
ManageFile::ManageFile()
{
    _counter = 0;
    _numofFile = 0;
    _fileName.clear();
}

/**
 * @brief ManageFile::~ManageFile destructor
 */
ManageFile::~ManageFile()
{
    qDebug() << "call ~ManageFile()";
}

/**
 * @brief ManageFile::setSizelist
 * @param size
 */
void ManageFile::setSizelist(int size)
{
    qDebug() << "recive list of file:";
    qDebug() << "\t" << size;
    _numofFile = size;
}

void ManageFile::print()
{
    qDebug() << "recived size list" << _numofFile;
}

/**
 * @brief ManageFile::setPropertyFile
 */
void ManageFile::setPropertyFile()
{
    QString name;
    QFile *file = new QFile(_fileName);
    QRegularExpression re("(\\w+?-\\w+\\.\\w+)$");//extract name
    QRegularExpressionMatch match = re.match(_fileName);
    if (match.hasMatch()) {
        name = match.captured(0);
    }
    emit getPropertyFile(file->size(), name);
    delete file;
}

/**
 * @brief ManageFile::setFile
 * @param fileName
 */
void ManageFile::setFile(const QString &fileName)
{
    _fileName = fileName;
    qDebug()<<"selected file to operate:" << _fileName;
    getfileName();
}

/**
 * @brief ManageFile::getfileName
 */
void ManageFile::getfileName()
{
    QRegularExpression re("(\\w+?-\\w+\\.\\w+)$");//extract name
    QRegularExpressionMatch match = re.match(_fileName);
    if (match.hasMatch()) {
        QString name =  match.captured(0);
        setNewfileName();
        emit outputfileName(_NewfileName);
    }
    setPropertyFile();
}

/**
 * @brief ManageFile::setNewfileName
 */
void ManageFile::setNewfileName()
{
    _NewfileName = _fileName;
    QRegularExpression re("(\\.\\w+)$");//extract name
    re.match(_NewfileName);
    _replace = "ApdlConverted.txt";
    qDebug() << _NewfileName.replace(re, _replace);
}

int ManageFile::getSizelistFile()
{
    return _numofFile;
}

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
void ManageFile::convert(ConverterSintax *pConverter, Node* node, Shell* shell)
{
    //read file
    {
        QProgressDialog dialog;  // Create a progress dialog.
        dialog.setLabelText(QString("Reading file..."));
        // Create a QFutureWatcher and connect signals and slots.
        QFutureWatcher<void> futureWatcher;
        QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
        QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
        QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
        QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
        // Start to read file
        futureWatcher.setFuture(QtConcurrent::run(read, _fileName, pConverter, node, shell));
        // Display the dialog and start the event loop.
        dialog.exec();
        futureWatcher.waitForFinished();
        // Query the future to check if was canceled.
        qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
    }
    // write file
    {
        QProgressDialog dialog; // Create a progress dialog.
        dialog.setLabelText(QString("Writing file..."));
        // Create a QFutureWatcher and connect signals and slots.
        QFutureWatcher<void> futureWatcher;
        QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
        QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
        QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
        QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
        //Start to write file
        futureWatcher.setFuture(QtConcurrent::run(writeAPDL, _NewfileName, node, shell));
        // Display the dialog and start the event loop.
        dialog.exec();
        futureWatcher.waitForFinished();
        // Query the future to check if was canceled.
        qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
    }
}

