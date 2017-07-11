#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>

#include "convertersintax.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

ConverterSintaX* converter = new ConverterSintaX();

void MainWindow::on_LoadFile_clicked()
{
    qDebug()<<"open file dialog...";
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), "", tr("All Files (*.*)"));
    qDebug()<<"file imported: "<<fileName;
    QFile inputFile(fileName);

    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            converter->getInputLine(in.readLine());
        }
        inputFile.close();
    }

}

void MainWindow::on_Exit_released()
{
    delete converter;
    qDebug()<<"closing app...";
    QApplication::quit();
}
