#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QString>
#include <QDropEvent>
#include <QEvent>
#include <QMimeData>
#include <QVector>
#include <QList>
#include <QScopedPointer>

#include "filemanager.h"
#include "converterdialog.h"


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

    void setnameFileText(const QString &filename);


    void setPropertyFile(const QString &filename, quint64 dimension);

signals:
    void sizeList(const int &size);

    void filetoprocess(int index);

    void updateProcessedFilename(const QString &filename);

private:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent *event);

    QScopedPointer<ConverterDialog> converter_dialog_{nullptr};
    QScopedPointer<ManageFile> manager_{nullptr};

    QList<QString>* listOfFile;

    int indexlist;
};

#endif // MAINWINDOW_H
