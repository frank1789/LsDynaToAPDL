#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "node.h"
#include "shell.h"


#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include "logger_tools.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Convert->setDisabled(true);
    ui->Preview->setDisabled(true);
    ui->information_file->setText("Process file: ");
    ui->dimensionfile->setText("Dimension: ");
    ui->FileInfo->setText("Dimension: 1 Mb");
    ui->Nodeinfo->setText("Total number node: 0");
    setAcceptDrops(true);
    //  ui->ElemInfo->setText("Total number element shell: 0");

    // instanziate classes to work Lsdyna/APDL
    listOfFile = new QList<QString>;
    // converter = new ConverterSintax();

    manager_.reset(new ManageFile());
    indexlist = 0;

    // connect slot
    QObject::connect(this, &MainWindow::sizeList, manager_.data(), &ManageFile::setSizelist);
    QObject::connect(this, &MainWindow::setFileText, manager_.data(), &ManageFile::setFile);
    QObject::connect(manager_.data(), &ManageFile::outputfileName, this, &MainWindow::setnameFileText);
    QObject::connect(manager_.data(), &ManageFile::getPropertyFile, this, &MainWindow::setPropertyFile);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete listOfFile;
}

void MainWindow::setnameFileText(const QString &nameFile)
{
    ui->lineEdit_converted->setText(nameFile);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    QApplication::quit();
}

void MainWindow::setPropertyFile(const qint64 &dimension, const QString &label)
{
    ui->information_file->setText("Process file: " + label);
    ui->dimensionfile->setText("Dimension: " + QString::number(dimension));
}


void MainWindow::on_LoadFile_clicked()
{
    qDebug()<<"Open file dialog...";
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open file"),"", tr("All files (*.k *.txt)"));
    if(!fileNames.isEmpty()) {
        QString fileName;
        foreach (auto file, fileNames) {
            fileName += file + " ";
            listOfFile->push_back(file);
            ui->lineEdit_original->setText(fileName);
        }
        //active button
        ui->Convert->setEnabled(true);
    }
    else {
        qDebug() <<"No input file";
        QMessageBox::warning(this, tr("Warning"), "The document not load.");
    }
    //  delete(validator);
    //  delete(selection);
}

void MainWindow::on_Convert_clicked()
{
    //    Dialog* selection = new Dialog(this);
    //    validateLineEdit* validator = new validateLineEdit(this);
    //    selection->exec();
    foreach(auto file, *listOfFile)
    {
        // Node  *node = new Node;
        // Shell *shell = new Shell;
        qDebug() << file;
        ui->lineEdit_original->setText(file);
        //    ui->lineEdit_converted->setText("new");
        emit setFileText(file);
        manager_->convert();
        // delete node;
        // delete shell;
    }
}

void MainWindow::on_Preview_clicked()
{
    //  QProgressDialog dialog; // Create a progress dialog.
    //  dialog.setLabelText(QString("Reading file..."));
    //  // Create a QFutureWatcher and connect signals and slots.
    //  QFutureWatcher<void> futureWatcher;
    //  QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
    //  QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
    //  QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    //  QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
    //  // Start to read file.
    ////  futureWatcher.setFuture(QtConcurrent::run(read, managefile->getfileName(), converter, node, shell));
    //  // Display the dialog and start the event loop.
    //  dialog.exec();
    //  futureWatcher.waitForFinished();
    //  // Query the future to check if was canceled.
    //  qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
}



void MainWindow::on_Exit_released()
{
    qDebug() << INFOFILE << "release resources and close app";
    QApplication::quit();
}

void MainWindow::on_actionInformazioni_triggered()
{
    About about;
    about.exec();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    QString filename = {};
    foreach (const QUrl &url, e->mimeData()->urls()) {
        filename = url.toLocalFile();
        qDebug() << INFOFILE << "Dropped a file:" << filename << "(" << listOfFile->size() << ")";
        listOfFile->push_back(filename);
        emit sizeList(listOfFile->size());
    }
    ui->Convert->setDisabled(false);
    ui->lineEdit_original->setText(filename);
}
