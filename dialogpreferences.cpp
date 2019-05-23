#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"

DialogPreferences::DialogPreferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPreferences)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

DialogPreferences::~DialogPreferences()
{
    delete ui;
}
