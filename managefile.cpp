#include "managefile.h"
#include "reader.h"
#include "writeapdl.h"
#include <QDebug>
#include <QRegularExpression>
#include <QtConcurrent>
#include <QThread>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>


ManageFile::ManageFile()
{
    _counter = 0;
    _numofFile = 0;
    _fileName.clear();
}

ManageFile::~ManageFile()
{
    qDebug() << "call ~ManageFile()";
}

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



void ManageFile::setFile(const QString &fileName)
{
    _fileName = fileName;
    qDebug()<<"selected file to operate:" << _fileName;
    getfileName();
}

void ManageFile::getfileName()
{
    QRegularExpression re("(\\w+?-\\w+\\.\\w+)$");//extract name
    QRegularExpressionMatch match = re.match(_fileName);
    if (match.hasMatch()) {
        QString name =  match.captured(0);
        setNewfileName();
        emit outputfileName(_NewfileName);
    }
}

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

void ManageFile::convert(ConverterSintaX* pConverter, Node* node, Shell* shell)
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
        emit done(true);
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

