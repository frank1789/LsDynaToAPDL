#ifndef CREDIT_H
#define CREDIT_H

#include <QWidget>
#include <ui_credit.h>

class Credit : public QWidget, private Ui_Form
{
    Q_OBJECT

public:
    Credit(QWidget *parent = 0);
    ~Credit();

signals:

private:
    Credit *ui_credit;
};

#endif // CREDIT_H
