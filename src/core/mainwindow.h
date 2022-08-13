#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDropEvent>
#include <QEvent>
#include <QList>
#include <QMainWindow>
#include <QMenuBar>
#include <QMimeData>
#include <QScopedPointer>
#include <QString>

#include "dialog.h"
#include "filemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);

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
  void activateParserDialog();
  void updateProcessedFilename(const QString &filename);
  void showAboutInformation();

 private:
  void closeEvent(QCloseEvent *event);

  Ui::MainWindow *ui{nullptr};
  QScopedPointer<FileManager> manager_{nullptr};
  QList<QString> process_files_;
};

#endif  // MAINWINDOW_H
