#include "about.h"
#include "ui_about.h"

#define VERSION 1
#define BUILD 12523

/**
 * @brief About::About default constructor ui to show information and license.
 * @param parent: pointer type Qwidget
 */
About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    //define window property
    ui->setupUi(this);
    this->setWindowTitle("");
    //define plain text object and content
    ui->plainTextEdit->setReadOnly(true);
    ui->plainTextEdit->appendPlainText(read(":/Resources/Resources/License/License.txt"));
    //set image property
    QPixmap imageObject(":/Resources/Resources/Icon/generic.png");
    imageObject.scaled(120,120);
    //define visualization
    ui->logo->setPixmap(imageObject);
    // add author
    ui->author->setText(QString("LsDynaToAPDL\nFrancesco Argentieri"));
    //add information builder
    ui->version->setText(QString("Version %1").arg(VERSION));
    ui->build->setText(QString("Build %1").arg(BUILD));
    qDebug() << VERSION;
    qDebug() << BUILD;
}

/**
 * @brief About::~About destructor
 */
About::~About()
{
    delete ui;
}

/**
 * @brief About::closeEvent captures the closing event of the window when it is
 *  running.
 * Emit dialogClosed().
 * @param[in] event: pointer closing event
 */
void About::closeEvent(QCloseEvent *event)
{
    if(event)
    {
        emit dialogClosed();
        event->accept();
        close();
    }
    else
        event->ignore();
}

QString read(QString pfileName)
{
    QFile fileInput(pfileName);
    if(!fileInput.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "failed to open file";
        return NULL;
    }
    QTextStream text(&fileInput);
    QString all = text.readAll();
    qDebug() << all;
    fileInput.close();
    return all;
}
