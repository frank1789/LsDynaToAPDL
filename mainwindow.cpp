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
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->label->setText("Dimension: 0 Mb");
    ui->Nodeinfo->setText("Total number node: 0");
    ui->ElemInfo->setText("Total number element shell: 0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

ConverterSintaX* converter = new ConverterSintaX();
LsDynaSintax::Node* test = new LsDynaSintax::Node();
LsDynaSintax::ElementShell* testshell = new LsDynaSintax::ElementShell();

/*
int countLineNumber(QString pfileinput)
{
    qDebug()<<"Enter line counter function, file imported: " << pfileinput;
    //set counter 0
    int CountNumberLine = 0;
    QFile inputFile(pfileinput);
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            //qDebug() << line;
            CountNumberLine += 1;
            //qDebug() <<"update line number:"<<CountNumberLine;
        }
    }
    inputFile.close();
    return CountNumberLine;
}
*/
QString fileName;
void MainWindow::on_LoadFile_clicked()
{
    qDebug()<<"Open file dialog...";
    fileName = QFileDialog::getOpenFileName(this,tr("Open file"), "", tr("All Files (*.*)"));
    qDebug()<<fileName;
    if(fileName != NULL)
    {
        //initilalize the counter line for progress bar
        int CountNumberLine = 0;

        //update progress bar
        //ui->progressBar->setMaximum(countLineNumber(fileName));
        //update text bar input file
        ui->lineEdit->setText(fileName);

        //read file
        QFile inputFile(fileName);

        //update group information file
        double sizeFile = inputFile.size()/(1048576);
        ui->label->setText("Dimension: " + QString().setNum(sizeFile,'d',2) + " Mb");

        //open file
        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&inputFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                CountNumberLine += 1;

                //store data
                converter->setInputLine(line, test, testshell);

                //refresh progress bar
                ui->progressBar->setValue(CountNumberLine);
            }
        }

        //close file
        inputFile.close();

        //active conversion
        ui->Convert->setEnabled(true);
        ui->lineEdit_2->setText(fileName);
    }
    int *numnode = new int(test->getNodeStructure().size());
    ui->Nodeinfo->setText("Total number node: " + QString(*numnode) );
    ui->ElemInfo->setText("Total number element shell: " + QString(testshell->getElementStructure().size()));
    qDebug()<<fileName;
}


void MainWindow::on_Convert_clicked()
{
   //
    QRegularExpression re("(\\w+.\\w+)$");
    re.match(fileName);
    QString replace = "ApdlConverted.txt";
    qDebug() << fileName.replace(re, replace);
    APDLsintax::Writer(fileName, test->getNodeStructure(), testshell->getElementStructure());

    //APDLsintax::Node* node = new APDLsintax::Node(test->getNodeStructure());
    //node->setWriter();
    //delete test;
}

void MainWindow::on_Exit_released()
{
    //delete converter;
    qDebug()<<"closing app...";
    delete test;
    delete testshell;
    QApplication::quit();
}

