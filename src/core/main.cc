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

#include <iostream>

#include <QApplication>

#include "about.h"
#include "mainwindow.h"
#include "parser.h"
#include "version.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  std::cout <<
      R"( 
   _       _____ _____                 _______                      _ _  
  | |     / ____|  __ \               |__   __|     /\             | | | 
  | |    | (___ | |  | |_   _ _ __   __ _| | ___   /  \   _ __   __| | | 
  | |     \___ \| |  | | | | | '_ \ / _` | |/ _ \ / /\ \ | '_ \ / _` | | 
  | |____ ____) | |__| | |_| | | | | (_| | | (_) / ____ \| |_) | (_| | | 
  |______|_____/|_____/ \__, |_| |_|\__,_|_|\___/_/    \_\ .__/ \__,_|_| 
                        __/ |                           | |              
                       |___/                            |_|              
 )" << std::endl;
  std::cout << "version " << compact_version().toStdString() << std::endl;
  std::cout << "author: Francesco Argentieri (francesco.argentieri89@gmail.com)\n\n";
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
