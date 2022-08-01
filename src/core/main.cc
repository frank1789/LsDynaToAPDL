/**
 * @file main.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief The main file.
 * @version 0.1
 * @date 2022-07-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <QApplication>
#include <QTimer>

#include "about.h"
#include "customassert.h"
#include "mainwindow.h"
#include "parser.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  QScopedPointer<core::About> about_widget(new core::About);
  QScopedPointer<core::Parser> parser(new core::Parser);
  // clang-format off
  QObject::connect(&w, &MainWindow::showAboutInformation, about_widget.get(), &core::About::open);
  QObject::connect(&w, &MainWindow::updateProcessedFilename, parser.get(), &core::Parser::elaborateFilename);
//  QObject::connect(parser.get(), &core::Parser::finished, parser.get(), &w, &MainWindow::triggerNextFile);
  // clang-format on
  w.show();

  return a.exec();
}
