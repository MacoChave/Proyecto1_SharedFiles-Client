#include "dialoglogin.h"
#include "ui_dialoglogin.h"

DialogLogIn::DialogLogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLogIn)
{
    ui->setupUi(this);
}

DialogLogIn::~DialogLogIn()
{
    delete ui;
}
