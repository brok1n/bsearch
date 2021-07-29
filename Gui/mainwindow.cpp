#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"
#include "threadpooltest.h"
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QRect>
#include <QThread>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QFileIconProvider>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mWaitResultTimer(new QTimer())
    , mWaitScanDiskTimer(new QTimer())
{
    ui->setupUi(this);

    this->ui->listWidget->setIconSize(QSize(24, 24));
    connect(this->ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onListWidgetItemClicked(QListWidgetItem*)));

    //如果窗口数量 > 0
    int controlPanelCount = Core::GetInstance()->controlPanelCount();
    if(controlPanelCount > 0)
    {
        //获取可用桌面大小
        QScreen *screen = QGuiApplication::screenAt(this->pos());
        QRect deskRect = screen->availableGeometry();
        this->setGeometry(deskRect.width() / 2 - width() / 2 + controlPanelCount * 30, deskRect.height() / 2 - height() / 2 + controlPanelCount * 30, width(), height());
        //多窗口时，让子窗口被关闭后自动释放
        setAttribute(Qt::WA_DeleteOnClose);
    }

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
    qDebug() << "窗口析构函数:" << mPanelId << ":end.";
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

void MainWindow::onListWidgetItemClicked(QListWidgetItem *item)
{
    QString txt = item->text();
    QString fullPath = item->data(1).toString();
    qDebug() << "点击了：" << txt << " fullPath:" << fullPath;
}

void MainWindow::flushResult()
{
    QList<Node*> *resultList = DataCenter::GetInstance()->resultList();
    ui->listWidget->clear();
    for(int i = 0; i < resultList->size(); i ++)
    {
        QListWidgetItem *item=new QListWidgetItem(ui->listWidget);
        //设置列表项的文本
//        item->setText(QString("%1 (%2)").arg(resultList->at(i)->fullPath()).arg(Common::formatFileSize(resultList->at(i)->fileSize())));
        QString fullPath = resultList->at(i)->fullPath();
        item->setText(QString("%1").arg(resultList->at(i)->name));
        QFileInfo fileInfo(fullPath);
        QFileIconProvider fileIcon;
        QIcon icon = fileIcon.icon(fileInfo);
        item->setData(1, fullPath);
        item->setIcon(icon);

        //加载列表项到列表框
        ui->listWidget->addItem(item);
    }
    ui->statusbar->showMessage(QString("找到 %1 个结果.").arg(resultList->size()));
}

void MainWindow::startSearch()
{
//    ui->listWidget->clear();
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

void MainWindow::on_actionAll_toggled(bool checked)
{
    qDebug() << "所有:" << checked;
    if(checked)
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

void MainWindow::on_actionMusic_toggled(bool checked)
{
    qDebug() << "音乐:" << checked;
    if(checked)
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

void MainWindow::on_actionVideo_toggled(bool checked)
{
    qDebug() << "视频:" << checked;
    if(checked)
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

void MainWindow::on_actionPicture_toggled(bool checked)
{
    qDebug() << "图片:" << checked;
    if(checked)
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

void MainWindow::on_actionDocument_toggled(bool checked)
{
    qDebug() << "文档:" << checked;
    if(checked)
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

void MainWindow::on_actionCompress_toggled(bool checked)
{
    qDebug() << "压缩包:" << checked;
    if(checked)
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

void MainWindow::on_actionExecutable_toggled(bool checked)
{
    qDebug() << "可执行:" << checked;
    if(checked)
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

void MainWindow::on_actionDir_toggled(bool checked)
{
    qDebug() << "文件夹:" << checked;
    if(checked)
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

void MainWindow::on_actionTxtContentSearch_toggled(bool checked)
{
    if(checked)
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
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
    if(Core::GetInstance()->controlPanelCount() > 1)
    {
        Core::Release();
    }
    qApp->quit();
}

void MainWindow::on_actionFixTop_toggled(bool checked)
{
    if(checked)
    {
        Qt::WindowFlags m_flags = windowFlags();
        setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
        show();
    }
    else
    {
        setWindowFlags(windowFlags() ^ Qt::WindowStaysOnTopHint);
        show();
    }
}

void MainWindow::on_actionDetail_toggled(bool checked)
{
    if(checked)
    {
        this->ui->actionSuperBigIcon->setChecked(false);
        this->ui->actionMidIcon->setChecked(false);
        this->ui->actionBigIcon->setChecked(false);
        this->ui->listWidget->setViewMode(QListWidget::ListMode);
        this->ui->listWidget->setFlow(QListWidget::TopToBottom);
    }
    else
    {

    }
}


void MainWindow::on_actionMidIcon_toggled(bool checked)
{
    if(checked)
    {
        this->ui->actionSuperBigIcon->setChecked(false);
        this->ui->actionBigIcon->setChecked(false);
        this->ui->actionDetail->setChecked(false);
        this->ui->listWidget->setViewMode(QListWidget::IconMode);
//        this->ui->listWidget->setIconSize(QSize(100, 100));
//        this->ui->listWidget->setSpacing(10);
//        this->ui->listWidget->setResizeMode(QListWidget::Adjust);
//        this->ui->listWidget->setMovement(QListWidget::Static);
        this->ui->listWidget->setFlow(QListWidget::LeftToRight);
    }
    else
    {

    }
}


void MainWindow::on_actionBigIcon_toggled(bool checked)
{
    if(checked)
    {
        this->ui->actionSuperBigIcon->setChecked(false);
        this->ui->actionMidIcon->setChecked(false);
        this->ui->actionDetail->setChecked(false);
        this->ui->listWidget->setViewMode(QListWidget::IconMode);
//        this->ui->listWidget->setIconSize(QSize(256, 256));
//        this->ui->listWidget->setSpacing(10);
//        this->ui->listWidget->setResizeMode(QListWidget::Adjust);
//        this->ui->listWidget->setMovement(QListWidget::Static);
        this->ui->listWidget->setFlow(QListWidget::LeftToRight);
    }
    else
    {

    }
}


void MainWindow::on_actionSuperBigIcon_toggled(bool checked)
{
    if(checked)
    {
        this->ui->actionBigIcon->setChecked(false);
        this->ui->actionMidIcon->setChecked(false);
        this->ui->actionDetail->setChecked(false);
        this->ui->listWidget->setViewMode(QListWidget::IconMode);
//        this->ui->listWidget->setIconSize(QSize(512, 512));
//        this->ui->listWidget->setSpacing(10);
//        this->ui->listWidget->setResizeMode(QListWidget::Adjust);
//        this->ui->listWidget->setMovement(QListWidget::Static);
        this->ui->listWidget->setFlow(QListWidget::LeftToRight);

    }
    else
    {

    }
}

