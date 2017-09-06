#include "credit.h"

Credit::Credit(QWidget *parent) :
    QWidget(parent),
    ui_credit(new Credit)
{
    ui_credit->setupUi(this);
    ui_credit->label->setText("Test Credit 1");
    ui_credit->label_2->setText("credit due");
}

Credit::~Credit()
{

}
