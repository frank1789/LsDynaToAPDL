/**
 * @file mainwindow.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The MainWindow gui.
 * @version 0.4.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QTimer>

#include "logger_tools.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), manager_(new FileManager()) {
  ui->setupUi(this);
  ui->Convert->setDisabled(true);
  ui->Preview->setDisabled(true);
  ui->information_file->setText("Process file: ");
  ui->dimensionfile->setText("Dimension: ");
  ui->FileInfo->setText("Dimension: 1 Mb");
  ui->Nodeinfo->setText("Total number node: 0");
  setAcceptDrops(true);
  //  ui->ElemInfo->setText("Total number element shell: 0");
  process_files_.reserve(16);
}

MainWindow::~MainWindow() {
  if (ui != nullptr) {
    delete ui;
    ui = nullptr;
  }
}

void MainWindow::setnameFileText(const QString &filename) {
  qDebug() << "called";
  ui->lineEdit_converted->setText(filename);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  event->accept();
  QApplication::quit();
}

void MainWindow::setPropertyFile(const QString &filename, quint64 dimension) {
  ui->information_file->setText("Process file: " + filename);
  ui->dimensionfile->setText("Dimension: " + QString::number(dimension));
}

void MainWindow::on_LoadFile_clicked() {
  qDebug() << "Open file dialog";
  QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open file"), "", tr("All files (*.k *.txt)"));
  if (!fileNames.isEmpty()) {
    QString fileName;
    foreach (auto file, fileNames) {
      fileName += file + " ";
      process_files_.push_back(file);
      ui->lineEdit_original->setText(fileName);
      ui->lineEdit_converted->setText(manager_->getOutputFile());
    }
    // activate button convert
    ui->Convert->setEnabled(true);
  } else {
    QMessageBox::warning(this, tr("Warning"), QStringLiteral("The document is not load."));
  }
}

void MainWindow::on_Convert_clicked() {
  foreach (auto current_file, process_files_) {
    manager_->setFilename(current_file);
    ui->lineEdit_original->setText(current_file);
    ui->lineEdit_converted->setText(manager_->getOutputFile());
    ui->dimensionfile->setText(QString::number(manager_->getFilesize()) + QStringLiteral(" byte"));
    QTimer::singleShot(500, this, [this, current_file]() { emit updateProcessedFilename(current_file); });
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
  //  SIGNAL(progressRangeChanged(int,int)), &dialog,
  //  SLOT(setRange(int,int))); QObject::connect(&futureWatcher,
  //  SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
  //  // Start to read file.
  ////  futureWatcher.setFuture(QtConcurrent::run(read,
  /// managefile->getfileName(), converter, node, shell));
  //  // Display the dialog and start the event loop.
  //  dialog.exec();
  //  futureWatcher.waitForFinished();
  //  // Query the future to check if was canceled.
  //  qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
}

void MainWindow::on_Exit_released() {
  qDebug().noquote() << INFOFILE << "release resources and close app";
  QApplication::quit();
}

void MainWindow::on_actionInformazioni_triggered() { emit showAboutInformation(); }

void MainWindow::dragEnterEvent(QDragEnterEvent *e) {
  if (e->mimeData()->hasUrls()) {
    e->acceptProposedAction();
  }
}

void MainWindow::dropEvent(QDropEvent *e) {
  foreach (const QUrl &url, e->mimeData()->urls()) {
    auto filename = url.toLocalFile();
    qDebug() << INFOFILE << "Dropped a file:" << filename << "(" << process_files_.size() << ")";
    process_files_.push_back(filename);
  }
  ui->Convert->setDisabled(false);
}
