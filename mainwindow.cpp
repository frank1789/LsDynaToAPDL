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
    ui->Convert->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

ConverterSintaX* converter = new ConverterSintaX();
LsDynaSintax::Node* test = new LsDynaSintax::Node();
void MainWindow::on_LoadFile_clicked()
{
    qDebug()<<"open file dialog...";
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), "", tr("All Files (*.*)"));
    if(fileName != NULL)
    {
        ui->lineEdit->setText(fileName);
        ui->Convert->setEnabled(true);
        qDebug()<<"file imported: "<<fileName;
        QFile inputFile(fileName);

        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);
            while (!in.atEnd())
            {
                converter->getInputLine(in.readLine(), test);
            }
            inputFile.close();
        }
    }

}


void MainWindow::on_Convert_clicked()
{
    //ui->Convert->
    APDLsintax::Node* node = new APDLsintax::Node(test->getNodeStructure());
    node->setNodeWriter();
    delete test;
}

void MainWindow::on_Exit_released()
{
    delete converter;
    qDebug()<<"closing app...";
    QApplication::quit();
}
