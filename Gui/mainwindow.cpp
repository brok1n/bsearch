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
#include <QUrl>
#include <QDesktopServices>
#include <QClipboard>
#include <QMimeData>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mWaitResultTimer(new QTimer())
    , mWaitScanDiskTimer(new QTimer())
{
    this->ui->setupUi(this);

    mDefaultIconSize = this->ui->listWidget->iconSize();

//    this->ui->listWidget->setIconSize(QSize(24, 24));
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

    this->ui->statusbar->showMessage("正在扫描磁盘...");
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
        this->ui->statusbar->showMessage("磁盘扫描完毕!");
    }
}

void MainWindow::onListWidgetItemClicked(QListWidgetItem *item)
{
    QString txt = item->text();
    QString fullPath = item->data(Qt::UserRole).toString();
    qDebug() << "点击了：" << txt << " fullPath:" << fullPath;
}

void MainWindow::flushResult()
{
    QList<Node*> *resultList = DataCenter::GetInstance()->resultList();
    this->ui->listWidget->clear();
    for(int i = 0; i < resultList->size(); i ++)
    {
        QListWidgetItem *item=new QListWidgetItem(this->ui->listWidget);
        QString fullPath = resultList->at(i)->fullPath();
        item->setText(QString("%1").arg(resultList->at(i)->name));

        QFileInfo fileInfo(fullPath);
        QFileIconProvider fileIcon;
        QIcon icon = fileIcon.icon(fileInfo);


        item->setIcon(icon);
        item->setData(Qt::UserRole, fullPath);

        this->ui->listWidget->addItem(item);
    }
   this-> ui->statusbar->showMessage(QString("找到 %1 个结果.").arg(resultList->size()));
}

void MainWindow::startSearch()
{
//    ui->listWidget->clear();
    QString key = this->ui->keyEdit->text();
    if(key.isEmpty())
    {
        qDebug() << "搜索关键字为空！不执行搜索。";
        this->ui->statusbar->showMessage("搜索关键字为空！不执行搜索。");
        return;
    }

    int fileType = this->ui->filterCBox->currentIndex();
    qDebug() << "开始搜索:" << key << "   " << FILE_TYPE_NAME[fileType];

    if(this->ui->contentSearchCbox->isChecked())
    {
        qDebug() << "内容搜索，需要特殊处理，功能暂未实现。";
        this->ui->statusbar->showMessage("内容搜索，需要特殊处理，功能暂未实现。");
        this->ui->contentSearchCbox->setChecked(false);
        this->ui->actionTxtContentSearch->setChecked(false);
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
    QString filterTxt = this->ui->filterCBox->itemText(index);
    qDebug() << "用户选择了:" << filterTxt;
    startSearch();
}

void MainWindow::on_actionAll_toggled(bool checked)
{
    qDebug() << "所有:" << checked;
    if(checked)
    {
        this->ui->actionMusic->setChecked(false);
        this->ui->actionVideo->setChecked(false);
        this->ui->actionPicture->setChecked(false);
        this->ui->actionDocument->setChecked(false);
        this->ui->actionCompress->setChecked(false);
        this->ui->actionExecutable->setChecked(false);
        this->ui->actionDir->setChecked(false);
        this->ui->filterCBox->setCurrentIndex(0);

        startSearch();
    }
}

void MainWindow::on_actionMusic_toggled(bool checked)
{
    qDebug() << "音乐:" << checked;
    if(checked)
    {
        this->ui->actionAll->setChecked(false);
        this->ui->actionVideo->setChecked(false);
        this->ui->actionPicture->setChecked(false);
        this->ui->actionDocument->setChecked(false);
        this->ui->actionCompress->setChecked(false);
        this->ui->actionExecutable->setChecked(false);
        this->ui->actionDir->setChecked(false);
        this->ui->filterCBox->setCurrentIndex(1);

        startSearch();
    }
}

void MainWindow::on_actionVideo_toggled(bool checked)
{
    qDebug() << "视频:" << checked;
    if(checked)
    {
        this->ui->actionAll->setChecked(false);
        this->ui->actionMusic->setChecked(false);
        this->ui->actionPicture->setChecked(false);
        this->ui->actionDocument->setChecked(false);
        this->ui->actionCompress->setChecked(false);
        this->ui->actionExecutable->setChecked(false);
        this->ui->actionDir->setChecked(false);
        this->ui->filterCBox->setCurrentIndex(2);

        startSearch();
    }
}

void MainWindow::on_actionPicture_toggled(bool checked)
{
    qDebug() << "图片:" << checked;
    if(checked)
    {
        this->ui->actionAll->setChecked(false);
        this->ui->actionMusic->setChecked(false);
        this->ui->actionVideo->setChecked(false);
        this->ui->actionDocument->setChecked(false);
        this->ui->actionCompress->setChecked(false);
        this->ui->actionExecutable->setChecked(false);
        this->ui->actionDir->setChecked(false);
        this->ui->filterCBox->setCurrentIndex(3);

        startSearch();
    }
}

void MainWindow::on_actionDocument_toggled(bool checked)
{
    qDebug() << "文档:" << checked;
    if(checked)
    {
        this->ui->actionAll->setChecked(false);
        this->ui->actionMusic->setChecked(false);
        this->ui->actionVideo->setChecked(false);
        this->ui->actionPicture->setChecked(false);
        this->ui->actionCompress->setChecked(false);
        this->ui->actionExecutable->setChecked(false);
        this->ui->actionDir->setChecked(false);
        this->ui->filterCBox->setCurrentIndex(4);

        startSearch();
    }
}

void MainWindow::on_actionCompress_toggled(bool checked)
{
    qDebug() << "压缩包:" << checked;
    if(checked)
    {
        this->ui->actionAll->setChecked(false);
        this->ui->actionMusic->setChecked(false);
        this->ui->actionVideo->setChecked(false);
        this->ui->actionPicture->setChecked(false);
        this->ui->actionDocument->setChecked(false);
        this->ui->actionExecutable->setChecked(false);
        this->ui->actionDir->setChecked(false);
        this->ui->filterCBox->setCurrentIndex(5);

        startSearch();
    }
}

void MainWindow::on_actionExecutable_toggled(bool checked)
{
    qDebug() << "可执行:" << checked;
    if(checked)
    {
        this->ui->actionAll->setChecked(false);
        this->ui->actionMusic->setChecked(false);
        this->ui->actionVideo->setChecked(false);
        this->ui->actionPicture->setChecked(false);
        this->ui->actionDocument->setChecked(false);
        this->ui->actionCompress->setChecked(false);
        this->ui->actionDir->setChecked(false);
        this->ui->filterCBox->setCurrentIndex(6);

        startSearch();
    }
}

void MainWindow::on_actionDir_toggled(bool checked)
{
    qDebug() << "文件夹:" << checked;
    if(checked)
    {
        this->ui->actionAll->setChecked(false);
        this->ui->actionMusic->setChecked(false);
        this->ui->actionVideo->setChecked(false);
        this->ui->actionPicture->setChecked(false);
        this->ui->actionDocument->setChecked(false);
        this->ui->actionCompress->setChecked(false);
        this->ui->actionExecutable->setChecked(false);
        this->ui->filterCBox->setCurrentIndex(7);

        startSearch();
    }
}

void MainWindow::on_actionTxtContentSearch_toggled(bool checked)
{
    if(checked)
    {
        this->ui->actionAll->setChecked(false);
        this->ui->actionMusic->setChecked(false);
        this->ui->actionVideo->setChecked(false);
        this->ui->actionPicture->setChecked(false);
        this->ui->actionDocument->setChecked(false);
        this->ui->actionCompress->setChecked(false);
        this->ui->actionExecutable->setChecked(false);
        this->ui->actionDir->setChecked(false);
        this->ui->contentSearchCbox->setChecked(true);
        this->ui->filterCBox->setCurrentIndex(4);

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

void MainWindow::on_actionSelectAll_triggered()
{
    this->ui->listWidget->selectAll();
}

void MainWindow::on_actionReverseSelect_triggered()
{
    for(int i = 0; i < this->ui->listWidget->count(); i ++)
    {
        QListWidgetItem *item = this->ui->listWidget->item(i);
        item->setSelected(!item->isSelected());
    }
}

void MainWindow::on_actionDefaultSize_triggered()
{
    this->ui->actionDefaultSize->setChecked(true);
    this->ui->actionMidSize->setChecked(false);
    this->ui->actionBigSize->setChecked(false);
    this->ui->listWidget->setIconSize(mDefaultIconSize);
}

void MainWindow::on_actionMidSize_triggered()
{
    this->ui->actionDefaultSize->setChecked(false);
    this->ui->actionMidSize->setChecked(true);
    this->ui->actionBigSize->setChecked(false);
    this->ui->listWidget->setIconSize(QSize(22, 22));
}

void MainWindow::on_actionBigSize_triggered()
{
    this->ui->actionDefaultSize->setChecked(false);
    this->ui->actionMidSize->setChecked(false);
    this->ui->actionBigSize->setChecked(true);
    this->ui->listWidget->setIconSize(QSize(32, 32));
}

void MainWindow::on_actionFlush_triggered()
{
    //刷新
    flushResult();
}


void MainWindow::on_actionOpenInFolder_triggered()
{
    QList<QListWidgetItem*> items = this->ui->listWidget->selectedItems();
    if(items.isEmpty())
    {
        qDebug() << "没有选择文件!";
    }
    QListWidgetItem *item = items.at(0);

    QString filePath = item->data(Qt::UserRole).toString();

    qDebug() << "select file:" << filePath;
    QFileInfo fileInfo(filePath);

    QUrl _url = QUrl::fromLocalFile(fileInfo.absolutePath());
    QDesktopServices::openUrl(_url);
}

void MainWindow::on_actionOpenFile_triggered()
{
    QList<QListWidgetItem*> items = this->ui->listWidget->selectedItems();
    if(items.isEmpty())
    {
        qDebug() << "没有选择文件!";
    }
    QListWidgetItem *item = items.at(0);

    QString filePath = item->data(Qt::UserRole).toString();

    QUrl _url = QUrl::fromLocalFile(filePath);
    QDesktopServices::openUrl(_url);
}


void MainWindow::on_actionCopyTo_triggered()
{
    QList<QListWidgetItem*> items = this->ui->listWidget->selectedItems();
    if(items.isEmpty())
    {
        qDebug() << "没有选择文件!";
    }
    QListWidgetItem *item = items.at(0);

    QString filePath = item->data(Qt::UserRole).toString();

    qDebug() << "copy to file:" << filePath;

    QString dirPath = QFileDialog::getExistingDirectory(this, "复制到", "./", QFileDialog::ShowDirsOnly);
    qDebug() << "复制到:" << dirPath;



}


void MainWindow::on_actionMoveTo_triggered()
{
    QList<QListWidgetItem*> items = this->ui->listWidget->selectedItems();
    if(items.isEmpty())
    {
        qDebug() << "没有选择文件!";
    }
    QListWidgetItem *item = items.at(0);

    QString filePath = item->data(Qt::UserRole).toString();

    qDebug() << "move to file:" << filePath;

    QString dirPath = QFileDialog::getExistingDirectory(this, "移动到", "./", QFileDialog::ShowDirsOnly);
    qDebug() << "移动到:" << dirPath;

}

