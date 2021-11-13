#include "about.h"

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QLabel>
#include <QTextStream>

#include "ui_about.h"
#include "version.h"

/**
 * @brief About::About default constructor ui to show information and license.
 * @param parent: pointer type Qwidget
 */
About::About(QWidget *parent) : QDialog(parent), ui(new Ui::About) {
  // define window property
  ui->setupUi(this);
  this->setWindowTitle("");
  // define plain text object and content
  ui->plainTextEdit->setReadOnly(true);
  ui->plainTextEdit->appendPlainText(
      readLicense(":/Resources/Resources/License/License.txt"));
  // set image property
  QPixmap imageObject(":/Resources/Resources/Icon/generic.png");
  imageObject.scaled(120, 120);
  // define visualization
  ui->logo->setPixmap(imageObject);
  // add author
  ui->author->setText(QString("LsDynaToAPDL\nFrancesco Argentieri"));
  // add information builder
  auto version = QString::fromStdString(compact_version());
  ui->version->setText(QString("Version %1").arg(version));
  ui->build->setText(QString("Build %1").arg(kBuild));
}

/**
 * @brief About::~About destructor
 */
About::~About() { delete ui; }

/**
 * @brief About::closeEvent captures the closing event of the window when it is
 *  running.
 * Emit dialogClosed().
 * @param[in] event: pointer closing event
 */
void About::closeEvent(QCloseEvent *event) {
  if (event != nullptr) {
    emit dialogClosed();
    event->accept();
    close();
  } else {
    event->ignore();
  }
}

QString About::readLicense(const QString &filename) {
  QFile fileInput(filename);
  if (!fileInput.open(QFile::ReadOnly | QFile::Text)) {
    qDebug() << "failed to open file";
    return "";
  }
  QTextStream text(&fileInput);
  QString all = text.readAll();
  fileInput.close();
  return all;
}
