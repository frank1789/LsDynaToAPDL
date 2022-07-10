#include "about.h"

#include <QDebug>
#include <QFile>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QTextStream>
#include <QVBoxLayout>

#include "version.h"

namespace core {

static auto kLicense = QStringLiteral(":/Resource/LICENSE");
static auto kIcon = QStringLiteral(":/Resource/generic.png");
static auto kProjectName = QStringLiteral("LsDynaToAPDL");
static auto kAuthor = QStringLiteral("Authors:");

template <typename T>
inline void safe_delete(T *&pointer) {
  if (pointer != nullptr) {
    delete pointer;
    pointer = nullptr;
  }
}

/**
 * @brief About::About default constructor ui to show information and license.
 * @param parent: pointer type Qwidget
 */
About::About(QWidget *parent) : QDialog(parent) {
  setWindowTitle(QStringLiteral("About LsDyna To APDL"));
  setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
  setFixedSize(720, 480);
  makeLayout();
  QObject::connect(close_btn_, &QPushButton::clicked, this,
                   [this]() { close(); });

  // lua_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table,
  //                     sol::lib::string, sol::lib::io, sol::lib::coroutine);
  // lua_.script("print('bark bark bark!')");
  //   license_ =
}

void About::showEvent(QShowEvent *event) {
  if (is_opening) {
    return;
  }
  is_opening = true;
  QWidget::showEvent(event);
  qDebug() << "pass here";
  emit openChanged(is_opening);
  // lua_.script_file(
  //     "/Users/francesco/Documents/Project/LsDynaToAPDL/scripts/"
  //     "license-animation.lua");

  is_opening = false;
}

/**
 * @brief About::~About destructor
 */
About::~About() {
  safe_delete(close_btn_);
  safe_delete(about_layout_);
  safe_delete(project_name_label_);
  safe_delete(authors_label_);
  safe_delete(build_label_);
  safe_delete(license_label_);
  safe_delete(icon_label_);
}

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
  is_opening = false;
  emit openChanged(is_opening);
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
  authors_label_ = new QLabel(kAuthor, this);
  project_name_label_ = new QLabel(kProjectName, this);
  build_label_ = new QLabel(compact_version(), this);
  license_label_ = new QLabel("", this);
  license_label_->setGeometry(0, 0, 80, 35);
  icon_label_ = new QLabel(this);
  icon_label_->setPixmap(scaled_img);
  icon_label_->setScaledContents(true);

  // define the layout
  about_layout_ = new QGridLayout(this);
  about_layout_->setGeometry(QRect(0, 0, 710, 470));
  about_layout_->setVerticalSpacing(10);
  about_layout_->setHorizontalSpacing(5);
  about_layout_->addWidget(icon_label_, 0, 0, 2, 1, Qt::AlignHCenter);
  about_layout_->addWidget(project_name_label_, 0, 1);
  about_layout_->addWidget(authors_label_, 1, 1);
  about_layout_->addWidget(build_label_, 3, 1);
  about_layout_->addWidget(license_label_, 4, 1);
  about_layout_->addWidget(close_btn_, 5, 2, Qt::AlignHCenter);

  //  license_ =
  //  lua_.load_file("/Users/francesco/Documents/Project/LsDynaToAPDL/scripts/license-animation.lua");
  //  qDebug() << license_.valid();
  //  if (!license_.valid()) {
  //        sol::error err = license_;
  //        std::cerr << "failed to load string-based script into the program"
  //        << err.what() << std::endl;
  //      }
  //  script();
  //      lua_.script_file("/Users/francesco/Documents/Project/LsDynaToAPDL/scripts/license-animation.lua");

  //  license_();
  // clang-format off
//  lua_.script("print('bark bark bark!')");
  // auto res = lua_.load_file("/Users/francesco/Documents/Project/LsDynaToAPDL/scripts/license-animation.lua");
  // //  auto result =;
  // qDebug() << res.valid();
  // // clang-format on
  // qDebug() << res.stack_index();
  // res();
}

void About::setNewLine(const QString &text) { license_label_->setText(text); }

}  // namespace core
