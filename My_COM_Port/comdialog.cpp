#include "comdialog.h"
#include "ui_comdialog.h"

ComDialog::ComDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComDialog)
{
    ui->setupUi(this);
}

ComDialog::~ComDialog()
{
    delete ui;
}
