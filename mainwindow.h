#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMenuBar>
#include <QString>
#include <QDropEvent>
#include <QEvent>
#include <QMimeData>
#include <QVector>
#include <QList>
#include "node.h"
#include "shell.h"
#include "convertersintax.h"
#include "about.h"
#include "managefile.h"


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

private slots:
    void on_LoadFile_clicked();

    void on_Exit_released();

    void on_Convert_clicked();

    void on_actionInformazioni_triggered();

    void on_Preview_clicked();

    void setnameFileText(const QString &nameFile);

signals:
    void sizeList(const int & size);

    void filetoprocess(int index);

    void setFileText(QString nameFile);

private:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent *event);

    ConverterSintax *converter;

    ManageFile *manager;

    QList<QString>* listOfFile;

    int indexlist;
};

#endif // MAINWINDOW_H
