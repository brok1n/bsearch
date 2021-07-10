#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    Core::Release();
}

void MainWindow::on_keyEdit_textChanged(const QString &arg1)
{
    qDebug() << "keyEdit:" << arg1;
}

void MainWindow::on_keyEdit_returnPressed()
{
    QString key = ui->keyEdit->text();
    Core::GetInstance()->search(key);
}
