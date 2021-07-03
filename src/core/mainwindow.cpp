#include "mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include "logger_tools.h"
#include "node.h"
//#include "shell.h"
#include "ui_mainwindow.h"

#include "about.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
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
  converter_dialog_.reset(new ConverterDialog(this));
  indexlist = 0;

  // connect slot
  // connect(this, &MainWindow::sizeList, manager_.data(), &ManageFile::setSizelist);
  connect(this, &MainWindow::updateProcessedFilename, [=](const QString &filename){ manager_->processedFilename(filename);});
  connect(this, &MainWindow::updateProcessedFilename, [=](const QString &filename){ converter_dialog_->changedProcessedFilename(filename);});
  connect(manager_.data(), &ManageFile::updatePropertyFile, this, &MainWindow::setPropertyFile);
  connect(manager_.data(), &ManageFile::changeOutputFilename, this, &MainWindow::setnameFileText);
}

MainWindow::~MainWindow() {
  delete ui;
  delete listOfFile;
}

void MainWindow::setnameFileText(const QString &filename)
{
  qDebug() << "called";
    ui->lineEdit_converted->setText(filename);
}


void MainWindow::closeEvent(QCloseEvent *event) {
  event->accept();
  QApplication::quit();
}

void MainWindow::setPropertyFile(const QString &filename, quint64 dimension){
  ui->information_file->setText("Process file: " + filename);
  ui->dimensionfile->setText("Dimension: " + QString::number(dimension));
}

void MainWindow::on_LoadFile_clicked() {
  qDebug() << "Open file dialog...";
  QStringList fileNames = QFileDialog::getOpenFileNames(
      this, tr("Open file"), "", tr("All files (*.k *.txt)"));
  if (!fileNames.isEmpty()) {
    QString fileName;
    foreach (auto file, fileNames) {
      fileName += file + " ";
      listOfFile->push_back(file);
      ui->lineEdit_original->setText(fileName);
    }
    // activate button convert
    ui->Convert->setEnabled(true);
  } else {
    QMessageBox::warning(this, tr("Warning"), "The document is not load.");
  }
}

void MainWindow::on_Convert_clicked() {
  foreach (auto file, *listOfFile) {
    qDebug() << INFOFILE << "process file:" << file;
    ui->lineEdit_original->setText(file);
    emit updateProcessedFilename(file);
    converter_dialog_->open();
    converter_dialog_->process();
    // delete node;
    // delete shell;
  }
}

void MainWindow::on_Preview_clicked() {
  //  QProgressDialog dialog; // Create a progress dialog.
  //  dialog.setLabelText(QString("Reading file..."));
  //  // Create a QFutureWatcher and connect signals and slots.
  //  QFutureWatcher<void> futureWatcher;
  //  QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog,make
  //  SLOT(reset())); QObject::connect(&dialog, SIGNAL(canceled()),
  //  &futureWatcher, SLOT(cancel())); QObject::connect(&futureWatcher,
  //  SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
  //  QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)),
  //  &dialog, SLOT(setValue(int)));
  //  // Start to read file.
  ////  futureWatcher.setFuture(QtConcurrent::run(read,
  ///managefile->getfileName(), converter, node, shell));
  //  // Display the dialog and start the event loop.
  //  dialog.exec();
  //  futureWatcher.waitForFinished();
  //  // Query the future to check if was canceled.
  //  qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
}

void MainWindow::on_Exit_released() {
  qDebug() << INFOFILE << "release resources and close app";
  QApplication::quit();
}

void MainWindow::on_actionInformazioni_triggered() {
  About about;
  about.exec();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e) {
  if (e->mimeData()->hasUrls()) {
    e->acceptProposedAction();
  }
}

void MainWindow::dropEvent(QDropEvent *e) {
  QString filename = {};
  foreach (const QUrl &url, e->mimeData()->urls()) {
    filename = url.toLocalFile();
    qDebug() << INFOFILE << "Dropped a file:" << filename << "("
             << listOfFile->size() << ")";
    listOfFile->push_back(filename);
    emit sizeList(listOfFile->size());
  }
  ui->Convert->setDisabled(false);
  ui->lineEdit_original->setText(filename);
}
