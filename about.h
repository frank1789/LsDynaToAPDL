#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QLabel>
#include <QImage>


namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

private:
    Ui::About *ui;
};

QString read(QString pfileName);

#endif // ABOUT_H
