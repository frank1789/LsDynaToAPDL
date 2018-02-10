#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtConcurrent"
#include "QThread"
#include "QMessageBox"
#include "QEvent"
#include "writeapdl.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->Convert->setDisabled(true);
  ui->Preview->setDisabled(true);
  ui->label->setText("Dimension: 0 Mb");
  ui->Nodeinfo->setText("Total number node: 0");
  setAcceptDrops(true);
  //  ui->ElemInfo->setText("Total number element shell: 0");

  // instanziate classes to work Lsdyna/APDL

  node = new Node();
  shell = new Shell();
  converter = new ConverterSintaX();
  managefile = new ManageFile();
}

MainWindow::~MainWindow()
{
  delete ui;
}

//void read2() {};

void MainWindow::on_LoadFile_clicked()
{
  qDebug()<<"Open file dialog...";
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"),"", tr("All files (*.k *.txt)"));
  //instaziate class to retrive information file
  managefile->setFile(fileName);
  ui->label->setText("Dimension: " + QString().setNum(managefile->getSizeInfo(),'d',2) + " Mb");
  if(fileName != "")
    {
      ui->lineEdit->setText(fileName);
      //active button
      ui->Convert->setEnabled(true);
      ui->Preview->setEnabled(true);
    }
  else
    {
      qDebug() <<"No input file";
      QMessageBox::warning(this, tr("Warning"), "The document has not been loaded.");
    }
  managefile->setNewfileName(); //update the output line edit
  ui->lineEdit_2->setText(managefile->getNewfileName());
}

void MainWindow::on_Convert_clicked()
{
  {
    //read file
    QProgressDialog dialog;  // Create a progress dialog.
    dialog.setLabelText(QString("Reading file..."));
    // Create a QFutureWatcher and connect signals and slots.
    QFutureWatcher<void> futureWatcher;
    QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
    QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
    QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
    // Start to read file
    futureWatcher.setFuture(QtConcurrent::run(read, managefile->getfileName(), converter, node, shell));
    // Display the dialog and start the event loop.
    dialog.exec();
    futureWatcher.waitForFinished();
    // Query the future to check if was canceled.
    qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
  }
  {
    // write file
    QProgressDialog dialog; // Create a progress dialog.
    dialog.setLabelText(QString("Writing file..."));
    // Create a QFutureWatcher and connect signals and slots.
    QFutureWatcher<void> futureWatcher;
    QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
    QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
    QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
    //Start to write file
    futureWatcher.setFuture(QtConcurrent::run(writeAPDL, managefile->getNewfileName(), node, shell));
    // Display the dialog and start the event loop.
    dialog.exec();
    futureWatcher.waitForFinished();
    // Query the future to check if was canceled.
    qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
  }
}

void MainWindow::on_Preview_clicked()
{
  QProgressDialog dialog; // Create a progress dialog.
  dialog.setLabelText(QString("Reading file..."));
  // Create a QFutureWatcher and connect signals and slots.
  QFutureWatcher<void> futureWatcher;
  QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
  QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
  QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
  QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
  // Start to read file.
  futureWatcher.setFuture(QtConcurrent::run(read, managefile->getfileName(), converter, node, shell));
  // Display the dialog and start the event loop.
  dialog.exec();
  futureWatcher.waitForFinished();
  // Query the future to check if was canceled.
  qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
}

void MainWindow::on_Exit_released()
{
  //clear the heap
  delete converter;
  delete shell;
  delete node;
  delete managefile;
  //    delete about;
  qDebug()<<"closing app...";
  QApplication::quit();
}

void MainWindow::information()
{
  qDebug() <<"success!";
}

void MainWindow::open_about()
{

}

void MainWindow::on_actionInformazioni_triggered()
{
  about = new About(this);
  about->show();
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
        qDebug() << "Dropped file:" << fileName;
    }

    ui->lineEdit->setText(fileName);
}
