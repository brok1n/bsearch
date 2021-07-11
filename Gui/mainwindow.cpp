#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"
#include "threadpooltest.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mWaitResultTimer(new QTimer())
{
    ui->setupUi(this);

//    ThreadPoolTest *test = new ThreadPoolTest;
//    test->start();
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
    ui->listWidget->clear();
    QString key = ui->keyEdit->text();
    Core::GetInstance()->search(key);
    disconnect(mWaitResultTimer, SIGNAL(timeout()), this, SLOT(onSearchFinished()));
    connect(mWaitResultTimer, SIGNAL(timeout()), this, SLOT(onSearchFinished()));
    mWaitResultTimer->start(1000);
}

void MainWindow::onSearchFinished()
{
    if(DataCenter::GetInstance()->isSearchFinished())
    {
        disconnect(mWaitResultTimer, SIGNAL(timeout()), this, SLOT(onSearchFinished()));
        mWaitResultTimer->stop();
        flushResult();
        qDebug() << "搜索完毕！";
    }
}

void MainWindow::flushResult()
{
    QList<Node*> *resultList = DataCenter::GetInstance()->resultList();
    for(int i = 0; i < resultList->size(); i ++)
    {
        QListWidgetItem *item=new QListWidgetItem(ui->listWidget);

        item->setText(resultList->at(i)->fullPath());   //设置列表项的文本

        ui->listWidget->addItem(item);  //加载列表项到列表框
    }
}
