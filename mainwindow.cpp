#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Convert->setDisabled(true);
    ui->Preview->setDisabled(true);
    ui->FileInfo->setText("Dimension: 1 Mb");
    ui->Nodeinfo->setText("Total number node: 0");
    setAcceptDrops(true);
    //  ui->ElemInfo->setText("Total number element shell: 0");

    // instanziate classes to work Lsdyna/APDL
    node = new Node();
    shell = new Shell();
    converter = new ConverterSintaX();
    listOfFile = new QList<QString>;
    manager = new ManageFile();
    indexlist = 0;

    QObject::connect(this, &MainWindow::sizeList, manager, &ManageFile::setSizelist);
    QObject::connect(this, &MainWindow::setFileText, manager, &ManageFile::setFile);
    QObject::connect(manager, &ManageFile::outputfileName, this, &MainWindow::setnameFileText);

}

MainWindow::~MainWindow()
{
    delete ui;

    delete converter;
    delete shell;
    delete node;
    delete listOfFile;
    delete manager;
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


void MainWindow::on_LoadFile_clicked()
{
    //    Dialog* selection = new Dialog(this);
    //    validateLineEdit* validator = new validateLineEdit(this);
    //    selection->exec();
    qDebug()<<"Open file dialog...";
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open file"),"", tr("All files (*.k *.txt)"));
    //  ui->label->setText("Dimension: " + QString().setNum(managefile->getSizeInfo(),'d',2) + " Mb");
    if(!fileNames.isEmpty())
    {
        QString fileName;
        foreach (auto file, fileNames) {
            fileName += file + " ";
            listOfFile->push_back(file);
            ui->lineEdit_original->setText(fileName);
        }
        //active button
        ui->Convert->setEnabled(true);
    }
    else
    {
        qDebug() <<"No input file";
        QMessageBox::warning(this, tr("Warning"), "The document not load.");
    }
    //  delete(validator);
    //  delete(selection);
}

void MainWindow::on_Convert_clicked()
{
    foreach(auto file, *listOfFile)
    {
        qDebug() << file;
        ui->lineEdit_original->setText(file);
        //    ui->lineEdit_converted->setText("new");
        emit setFileText(file);
        manager->convert(converter,node, shell);
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
    qDebug()<<"closing app...";
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
    QString fileName;
    foreach (const QUrl &url, e->mimeData()->urls()) {
        fileName = url.toLocalFile();
        qDebug() << "Dropped file:" << fileName << "(" << listOfFile->size() << ")";
        listOfFile->push_back(fileName);
        emit sizeList(listOfFile->size());
    }
    ui->Convert->setDisabled(false);
}
