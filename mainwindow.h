#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDebug>
#include <QMenuBar>
#include "about.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void open_about();

private slots:
    void on_LoadFile_clicked();

    void on_Exit_released();

    void on_Convert_clicked();

    void information();

    void on_actionInformazioni_triggered();

private:
    Ui::MainWindow *ui;
    About *about;
};

#endif // MAINWINDOW_H
