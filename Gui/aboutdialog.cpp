#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog| Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint);

}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_closeBtn_clicked()
{
    this->close();
}
