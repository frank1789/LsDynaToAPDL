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

int main(int argc, char* argv[]) {
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
  core::About about;
  core::Parser parser;
  QObject::connect(&w, &MainWindow::showAboutInformation, &about, &core::About::open);
  QObject::connect(&w, &MainWindow::updatePositionWidget, &parser, &core::Parser::centerDialogWindow);
  QObject::connect(&w, &MainWindow::updateProcessedFilename, &parser, [&parser, &w](const QString& infile) {
    w.setDisabled(true);  // set the main window disabled
    parser.elaborateFilename(infile);
  });
  QObject::connect(&parser, &core::Parser::finished, &w, [&w]() { w.setDisabled(false); });
  w.show();

  return a.exec();
}
