#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(onBtnClicked(bool)));

}

MainWindow::~MainWindow()
{
    delete ui;
    Core::Release();
}

void MainWindow::onBtnClicked(bool checked)
{
    int a = ui->lineEdit->text().toInt();
    int b = ui->lineEdit_2->text().toInt();

    int sum = Core::GetInstance()->add(a, b);
//    int sum = 111;

    ui->label_3->setText(QString("结果为：%1").arg(sum));


}

