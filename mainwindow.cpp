#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "convertersintax.h"
#include "reader.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Convert->setDisabled(true);
    ui->label->setText("Dimension: 0 Mb");
    ui->Nodeinfo->setText("Total number node: 0");
    ui->ElemInfo->setText("Total number element shell: 0");

    //connect(ui->actionInformazioni, SIGNAL(triggered()), this, SLOT(open_about()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

ConverterSintaX* converter = new ConverterSintaX();
LsDynaSintax::Node* node = new LsDynaSintax::Node();
LsDynaSintax::ElementShell* shell = new LsDynaSintax::ElementShell();




QString fileName;


void MainWindow::on_LoadFile_clicked()
{
    qDebug()<<"Open file dialog...";
    fileName = QFileDialog::getOpenFileName(this,tr("Open file"), "", tr("All Files (*.*)"));
    if(fileName != "")
    {
        ui->lineEdit->setText(fileName);

        // Create a progress dialog.
        QProgressDialog dialog;
        dialog.setLabelText(QString("Reading file..."));

        // Create a QFutureWatcher and connect signals and slots.
        QFutureWatcher<void> futureWatcher;
        QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
        QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
        QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
        QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

        // Start the computation.
        futureWatcher.setFuture(QtConcurrent::run(read, fileName, converter, node, shell));

        // Display the dialog and start the event loop.
        dialog.exec();

        futureWatcher.waitForFinished();

        // Query the future to check if was canceled.
        qDebug() << "Canceled?" << futureWatcher.future().isCanceled();
    }
    else
    {
        qDebug() <<"Missing input file, launch information message.";
        QMessageBox::information(this, tr("Info"), "The document has not been loaded.");
    }

    //--------------------------------------------------------------------------------------<
    //    if(fileName != NULL)
    //    {
    //        //initilalize the counter line for progress bar
    //        int CountNumberLine = 0;

    //        //update progress bar
    //        //ui->progressBar->setMaximum(countLineNumber(fileName));
    //        //update text bar input file
    //        ui->lineEdit->setText(fileName);

    //        //read file
    //        QFile inputFile(fileName);

    //        //update group information file
    //        double sizeFile = inputFile.size()/(1048576);
    //        ui->label->setText("Dimension: " + QString().setNum(sizeFile,'d',2) + " Mb");

    //        //open file
    //        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    //        {
    //            QTextStream in(&inputFile);
    //            while (!in.atEnd())
    //            {
    //                QString line = in.readLine();
    //                CountNumberLine += 1;

    //                //store data
    //converter->setInputLine(futureWatcher.result(), test, testshell);

    //                //refresh progress bar
    //                ui->progressBar->setValue(CountNumberLine);
    //            }
    //        }

    //active conversion
    ui->Convert->setEnabled(true);
    ui->lineEdit_2->setText(fileName);
}


void MainWindow::on_Convert_clicked()
{
    QRegularExpression re("(\\w+.\\w+)$");
    re.match(fileName);
    QString replace = "ApdlConverted.txt";
    qDebug() << fileName.replace(re, replace);

    // Create a progress dialog.
    QProgressDialog dialog;
    dialog.setLabelText(QString("Writing file..."));

    // Create a QFutureWatcher and connect signals and slots.
    QFutureWatcher<void> futureWatcher;
    QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
    QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
    QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

    // Start the computation.
    futureWatcher.setFuture(QtConcurrent::run(APDLsintax::Writer,fileName, node->getNodeStructure(), shell->getElementStructure()));

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
    delete node;
    delete shell;

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
