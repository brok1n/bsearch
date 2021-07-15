#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"
#include "threadpooltest.h"
#include <QDateTime>
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mWaitResultTimer(new QTimer())
    , mWaitScanDiskTimer(new QTimer())
{
    ui->setupUi(this);

    mPanelId = QDateTime::currentDateTime().toMSecsSinceEpoch();

    if(!Core::GetInstance()->addControlPanel(mPanelId))
    {
        QThread::msleep(10);
        mPanelId = QDateTime::currentDateTime().toMSecsSinceEpoch();
        Core::GetInstance()->addControlPanel(mPanelId);
    }

    connect(mWaitScanDiskTimer, SIGNAL(timeout()), this, SLOT(onScanDiskFinished()));
    mWaitScanDiskTimer->start(1000);

    ui->statusbar->showMessage("正在扫描磁盘...");
//    ThreadPoolTest *test = new ThreadPoolTest;
//    test->start();
}

MainWindow::~MainWindow()
{
    qDebug() << "窗口析构函数:" << mPanelId;
    Core::GetInstance()->releaseControlPanel(mPanelId);
    mWaitResultTimer->stop();
    delete mWaitResultTimer;
    mWaitScanDiskTimer->stop();
    delete mWaitScanDiskTimer;
    delete ui;
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
        //设置列表项的文本
        item->setText(QString("%1 (%2)").arg(resultList->at(i)->fullPath()).arg(Common::formatFileSize(resultList->at(i)->fileSize())));
        //加载列表项到列表框
        ui->listWidget->addItem(item);
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
        ui->statusbar->showMessage("搜索关键字为空！不执行搜索。");
        return;
    }

    int fileType = ui->filterCBox->currentIndex();
    qDebug() << "开始搜索:" << key << "   " << FILE_TYPE_NAME[fileType];

    if(ui->contentSearchCbox->isChecked())
    {
        qDebug() << "内容搜索，需要特殊处理，功能暂未实现。";
        ui->statusbar->showMessage("内容搜索，需要特殊处理，功能暂未实现。");
        return;
    }

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

void MainWindow::on_actionAll_toggled(bool arg1)
{
    qDebug() << "所有:" << arg1;
    if(arg1)
    {
        ui->actionMusic->setChecked(false);
        ui->actionVideo->setChecked(false);
        ui->actionPicture->setChecked(false);
        ui->actionDocument->setChecked(false);
        ui->actionCompress->setChecked(false);
        ui->actionExecutable->setChecked(false);
        ui->actionDir->setChecked(false);
        ui->filterCBox->setCurrentIndex(0);

        startSearch();
    }
}

void MainWindow::on_actionMusic_toggled(bool arg1)
{
    qDebug() << "音乐:" << arg1;
    if(arg1)
    {
        ui->actionAll->setChecked(false);
        ui->actionVideo->setChecked(false);
        ui->actionPicture->setChecked(false);
        ui->actionDocument->setChecked(false);
        ui->actionCompress->setChecked(false);
        ui->actionExecutable->setChecked(false);
        ui->actionDir->setChecked(false);
        ui->filterCBox->setCurrentIndex(1);

        startSearch();
    }
}

void MainWindow::on_actionVideo_toggled(bool arg1)
{
    qDebug() << "视频:" << arg1;
    if(arg1)
    {
        ui->actionAll->setChecked(false);
        ui->actionMusic->setChecked(false);
        ui->actionPicture->setChecked(false);
        ui->actionDocument->setChecked(false);
        ui->actionCompress->setChecked(false);
        ui->actionExecutable->setChecked(false);
        ui->actionDir->setChecked(false);
        ui->filterCBox->setCurrentIndex(2);

        startSearch();
    }
}

void MainWindow::on_actionPicture_toggled(bool arg1)
{
    qDebug() << "图片:" << arg1;
    if(arg1)
    {
        ui->actionAll->setChecked(false);
        ui->actionMusic->setChecked(false);
        ui->actionVideo->setChecked(false);
        ui->actionDocument->setChecked(false);
        ui->actionCompress->setChecked(false);
        ui->actionExecutable->setChecked(false);
        ui->actionDir->setChecked(false);
        ui->filterCBox->setCurrentIndex(3);

        startSearch();
    }
}

void MainWindow::on_actionDocument_toggled(bool arg1)
{
    qDebug() << "文档:" << arg1;
    if(arg1)
    {
        ui->actionAll->setChecked(false);
        ui->actionMusic->setChecked(false);
        ui->actionVideo->setChecked(false);
        ui->actionPicture->setChecked(false);
        ui->actionCompress->setChecked(false);
        ui->actionExecutable->setChecked(false);
        ui->actionDir->setChecked(false);
        ui->filterCBox->setCurrentIndex(4);

        startSearch();
    }
}

void MainWindow::on_actionCompress_toggled(bool arg1)
{
    qDebug() << "压缩包:" << arg1;
    if(arg1)
    {
        ui->actionAll->setChecked(false);
        ui->actionMusic->setChecked(false);
        ui->actionVideo->setChecked(false);
        ui->actionPicture->setChecked(false);
        ui->actionDocument->setChecked(false);
        ui->actionExecutable->setChecked(false);
        ui->actionDir->setChecked(false);
        ui->filterCBox->setCurrentIndex(5);

        startSearch();
    }
}

void MainWindow::on_actionExecutable_toggled(bool arg1)
{
    qDebug() << "可执行:" << arg1;
    if(arg1)
    {
        ui->actionAll->setChecked(false);
        ui->actionMusic->setChecked(false);
        ui->actionVideo->setChecked(false);
        ui->actionPicture->setChecked(false);
        ui->actionDocument->setChecked(false);
        ui->actionCompress->setChecked(false);
        ui->actionDir->setChecked(false);
        ui->filterCBox->setCurrentIndex(6);

        startSearch();
    }
}

void MainWindow::on_actionDir_toggled(bool arg1)
{
    qDebug() << "文件夹:" << arg1;
    if(arg1)
    {
        ui->actionAll->setChecked(false);
        ui->actionMusic->setChecked(false);
        ui->actionVideo->setChecked(false);
        ui->actionPicture->setChecked(false);
        ui->actionDocument->setChecked(false);
        ui->actionCompress->setChecked(false);
        ui->actionExecutable->setChecked(false);
        ui->filterCBox->setCurrentIndex(7);

        startSearch();
    }
}

void MainWindow::on_actionTxtContentSearch_toggled(bool arg1)
{
    if(arg1)
    {
        ui->actionAll->setChecked(false);
        ui->actionMusic->setChecked(false);
        ui->actionVideo->setChecked(false);
        ui->actionPicture->setChecked(false);
        ui->actionDocument->setChecked(false);
        ui->actionCompress->setChecked(false);
        ui->actionExecutable->setChecked(false);
        ui->actionDir->setChecked(false);
        ui->contentSearchCbox->setChecked(true);
        ui->filterCBox->setCurrentIndex(4);

        startSearch();
    }
}

void MainWindow::on_actionCreateWindow_triggered()
{
    qDebug() << "创建新窗口";
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
    this->deleteLater();
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
    qApp->quit();
}

void MainWindow::on_actionFixTop_toggled(bool arg1)
{
    if(arg1)
    {
        Qt::WindowFlags m_flags = windowFlags();
        setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
        show();
    }
    else
    {
        setWindowFlags(nullptr);
        show();
    }
}
