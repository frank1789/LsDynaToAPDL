#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDebug>
#include <QMenuBar>
#include <QString>
#include <QDropEvent>
#include "about.h"
#include "reader.h"

#include "node.h"
#include "shell.h"
#include "convertersintax.h"



namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void dragEnterEvent(QDragEnterEvent *e);
  void dropEvent(QDropEvent *e);
public slots:
  void open_about();


private slots:
  void on_LoadFile_clicked();
  void on_Exit_released();
  void on_Convert_clicked();
  void information();
  void on_actionInformazioni_triggered();
  void on_Preview_clicked();

private:
  Ui::MainWindow *ui;

  //declare inforamtion windows
  About *about;
  // declare class to work LsDyna - APDL

  ConverterSintaX *converter;
  Node* node;
  Shell* shell;
  //    LsDynaSintax::Node *node;
  //    LsDynaSintax::Shell *shell;
  //instaziate class to retrive information file
  ManageFile *managefile;
};

#endif // MAINWINDOW_H
