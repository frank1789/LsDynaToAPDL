/**
 * @file main.cc
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief 
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

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  QScopedPointer<core::About> about_widget(new core::About);
  QObject::connect(&w, &MainWindow::showAboutInformation, about_widget.get(), &core::About::open);
  w.show();

  return a.exec();
}
