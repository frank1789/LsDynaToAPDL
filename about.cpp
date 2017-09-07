#include "about.h"
#include "ui_about.h"
#include <QLabel>
#include <QImage>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    //read(":/Resources/Resources/License/License.txt");
    ui->plainTextEdit->setReadOnly(true);
    ui->plainTextEdit->appendPlainText(read(":/Resources/Resources/License/License.txt"));

    //QImage green(":/Resources/Resources/Icon/generic.icns");

    QPixmap imageObject(":/Resources/Resources/Icon/generic.png");
    ui->label->setPixmap(imageObject);
   // QLabel *imageLabel = new QLabel();
   // imageLabel->setPixmap(green);

}

About::~About()
{
    delete ui;
}

QString read(QString pfileName)
{
    QFile fileInput(pfileName);
    if(!fileInput.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "failed to poen file";
        return NULL;
    }

    QTextStream text(&fileInput);
    QString all = text.readAll();

    qDebug() << all;

    fileInput.close();
    return all;
}
