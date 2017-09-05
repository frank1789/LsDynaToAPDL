#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDebug>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_LoadFile_clicked();

    void on_Exit_released();

    void on_Convert_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
