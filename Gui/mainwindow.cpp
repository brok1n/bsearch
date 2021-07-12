#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"
#include "threadpooltest.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mWaitResultTimer(new QTimer())
    , mWaitScanDiskTimer(new QTimer())
{
    ui->setupUi(this);

    connect(mWaitScanDiskTimer, SIGNAL(timeout()), this, SLOT(onScanDiskFinished()));
    mWaitScanDiskTimer->start(1000);

    ui->statusbar->showMessage("正在扫描磁盘...");
//    ThreadPoolTest *test = new ThreadPoolTest;
//    test->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    Core::Release();
}

void MainWindow::on_keyEdit_returnPressed()
{
    startSearch();
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

void MainWindow::onScanDiskFinished()
{
    if(DataCenter::GetInstance()->isScanDiskFinished())
    {
        disconnect(mWaitScanDiskTimer, SIGNAL(timeout()), this, SLOT(onScanDiskFinished()));
        mWaitScanDiskTimer->stop();
        ui->statusbar->showMessage("磁盘扫描完毕!");
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
    ui->statusbar->showMessage(QString("找到 %1 个结果.").arg(resultList->size()));
}

void MainWindow::startSearch()
{
    ui->listWidget->clear();
    QString key = ui->keyEdit->text();
    if(key.isEmpty())
    {
        qDebug() << "搜索关键字为空！不执行搜索。";
        return;
    }

    int fileType = ui->filterCBox->currentIndex();
    qDebug() << "开始搜索:" << key << "   " << FILE_TYPE_NAME[fileType];

    Core::GetInstance()->search(key, fileType);
//    Core::GetInstance()->search(key);
    disconnect(mWaitResultTimer, SIGNAL(timeout()), this, SLOT(onSearchFinished()));
    connect(mWaitResultTimer, SIGNAL(timeout()), this, SLOT(onSearchFinished()));
    mWaitResultTimer->start(1000);
}

void MainWindow::on_filterCBox_currentIndexChanged(int index)
{
    QString filterTxt = ui->filterCBox->itemText(index);
    qDebug() << "用户选择了:" << filterTxt;
    startSearch();
}
