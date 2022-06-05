#include "about.h"

#include <QDebug>
#include <QFile>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QTextStream>
#include <QVBoxLayout>

#include "ui_about.h"
#include "version.h"

namespace core {

static auto kLicense = QStringLiteral(":/Resource/LICENSE");
static auto kIcon = QStringLiteral(":/Resource/generic.png");
static auto kAuthor = QStringLiteral("LsDynaToAPDL\nFrancesco Argentieri");

/**
 * @brief About::About default constructor ui to show information and license.
 * @param parent: pointer type Qwidget
 */
About::About(QWidget *parent) : QDialog(parent) {
  setWindowTitle(QStringLiteral("About LsDyna To APDL"));
  setFixedSize(sizeHint());
  makeLayout();
  QObject::connect(close_btn_, &QPushButton::clicked, this,
                   [this]() { close(); });
}

/**
 * @brief About::~About destructor
 */
About::~About() = default;

/**
 * @brief About::closeEvent captures the closing event of the window when it is
 *  running.
 * Emit dialogClosed().
 * @param[in] event: pointer closing event
 */
void About::closeEvent(QCloseEvent *event) {
  emit dialogClosed();
  event->accept();
  close();
}

QString About::readLicense(const QString &filename) {
  QFile fileInput(filename);
  if (!fileInput.open(QFile::ReadOnly | QFile::Text)) {
    qWarning() << "[WARNING] failed to open file" << filename;
    return "";
  }
  QTextStream text(&fileInput);
  QString all = text.readAll();
  fileInput.close();
  return all;
}

void About::makeLayout() {
  // set image property
  QPixmap icon_image(kIcon);
  auto scaled_img = icon_image.scaled(120, 120, Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation);

  close_btn_ = new QPushButton(QStringLiteral("close"), this);
  author_label_ = new QLabel(kAuthor, this);
  build_label_ = new QLabel(compact_version(), this);
  license_label_ =
      new QLabel(/*readLicense(kLicense)*/
                 "Prova teseto molto lungo Prova teseto molto lungo Prova "
                 "teseto molto lungo\n Prova teseto molto lungo ",
                 this);
  icon_label_ = new QLabel(this);
  icon_label_->setPixmap(scaled_img);

  // define vertical layout
  about_layout_ = new QGridLayout(this);
  about_layout_->setVerticalSpacing(10);
  about_layout_->setHorizontalSpacing(5);
  about_layout_->addWidget(icon_label_, 0, 0, 2, 1, Qt::AlignHCenter);
  about_layout_->addWidget(author_label_, 0, 1);
  about_layout_->addWidget(build_label_, 1, 1);
  about_layout_->addWidget(license_label_, 2, 1);
  about_layout_->addWidget(close_btn_, 3, 2, Qt::AlignHCenter);
}

}  // namespace core
