#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"
#include "copymovefiledialog.h"
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
#include <QStandardPaths>
#include <QMessageBox>
#include <QDir>
#include <QtNetwork>


const char checkUpdateUrl[] = "https://gitee.com/brok1n/bsearch/raw/master/Api/checkUpdate.json";
const char checkUpdateBakUrl[] = "https://raw.githubusercontent.com/brok1n/bsearch/master/Api/checkUpdate.json";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mWaitResultTimer(new QTimer())
    , mWaitScanDiskTimer(new QTimer())
    , mAboutDialog(nullptr)
    , reply(nullptr)
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

    QString desktopFileName = QApplication::desktopFileName();
    qDebug() << "desktopFileName:" << desktopFileName;

    QString applicationDisplayName = QApplication::applicationDisplayName();
    qDebug() << "applicationDisplayName:" << applicationDisplayName;

    QString platformName = QApplication::platformName();
    qDebug() << "platformName:" << platformName;


    switch (DataCenter::GetInstance()->sortType()) {
    case SORT_TYPE::BY_NAME:
        this->ui->actionSortName->setChecked(true);
        break;
    case SORT_TYPE::BY_SIZE:
        this->ui->actionSortSize->setChecked(true);
        break;
    case SORT_TYPE::BY_PATH:
        this->ui->actionSortPath->setChecked(true);
        break;
    case SORT_TYPE::BY_TYPE:
        this->ui->actionSortType->setChecked(true);
        break;
    case SORT_TYPE::BY_EXT:
        this->ui->actionSortExt->setChecked(true);
        break;
    case SORT_TYPE::BY_CTIME:
        this->ui->actionSortCreateTime->setChecked(true);
        break;
    case SORT_TYPE::BY_MTIME:
        this->ui->actionSortModifyTime->setChecked(true);
        break;
    }

    switch (DataCenter::GetInstance()->sortOrder()) {
    case SORT_ORDER::SORT_ASC:
        this->ui->actionSortAsc->setChecked(true);
        break;
    case SORT_ORDER::SORT_DESC:
        this->ui->actionSortDesc->setChecked(true);
        break;
    }


    QList<QString> testList;

    testList.append("Brok1n");
//    testList.append("brok1n");
    testList.append("Jacklist");
//    testList.append("Jacklist");
    QString key = "brok1n";
    if(testList.contains(key))
    {
        qDebug() << "contains " << key;
    }
    else
    {
        qDebug() << "not contains " << key;
    }


    // ----- 版本更新相关 -----

//#ifndef QT_NO_SSL
    connect(&qnam, &QNetworkAccessManager::sslErrors,
            this, &MainWindow::sslErrors);
//#endif

    // ----- 版本更新相关 -----


//    QString desktopFileName = QApplication::desktopFileName();
//    qDebug() << "desktopFileName:" << desktopFileName;

//    ThreadPoolTest *test = new ThreadPoolTest;
//    test->start();
}

MainWindow::~MainWindow()
{
    qDebug() << "窗口析构函数:" << mPanelId;
    mAboutDialog->deleteLater();
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

    QElapsedTimer flushTimer;
    flushTimer.start();

    this->ui->listWidget->clear();
    int size = resultList->size();
    size = size > 25 ? 25 : size;
    for(int i = 0; i < size; i ++)
    {
        Node *node = resultList->at(i);
        QListWidgetItem *item=new QListWidgetItem(this->ui->listWidget);
        QString fullPath = node->fullPath();
        item->setText(QString("%1 (%2)").arg(node->name).arg(Common::formatFileSize(node->fileSize())));

        QFileInfo fileInfo(fullPath);
        QFileIconProvider fileIcon;
        QIcon icon = fileIcon.icon(fileInfo);


        item->setIcon(icon);
        item->setData(Qt::UserRole, fullPath);

        this->ui->listWidget->addItem(item);
//        this->update();
        QApplication::processEvents();
    }
    this-> ui->statusbar->showMessage(QString("找到 %1 个结果.").arg(resultList->size()));

    this->update();

    qint64 flushTime = flushTimer.elapsed();
    qDebug() << "结果展示完毕! 耗时：" << (flushTime / 1000) << "秒" << (flushTime % 1000) << "毫秒";
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

    QFileInfo dirPathInfo(dirPath);
    if(dirPath.isEmpty())
    {
        qDebug() << "用户取消了.";
        return;
    }
    if(!dirPathInfo.exists())
    {
        QMessageBox msgBox;
        msgBox.setText("目标文件夹不存在!");
        msgBox.exec();
        return;
    }

    QList<QString> files;
    for (int i = 0; i < items.size(); i ++)
    {
        files.append(items.at(i)->data(Qt::UserRole).toString());
    }

    CopyMoveFileDialog *copyMoveFileDialog = new CopyMoveFileDialog(files, dirPath, false);
    copyMoveFileDialog->start();


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

    QFileInfo dirPathInfo(dirPath);
    if(dirPath.isEmpty())
    {
        qDebug() << "用户取消了.";
        return;
    }
    if(!dirPathInfo.exists())
    {
        QMessageBox msgBox;
        msgBox.setText("目标文件夹不存在!");
        msgBox.exec();
        return;
    }

    QList<QString> files;
    for (int i = 0; i < items.size(); i ++)
    {
        files.append(items.at(i)->data(Qt::UserRole).toString());
    }

    CopyMoveFileDialog *copyMoveFileDialog = new CopyMoveFileDialog(files, dirPath, true);
    copyMoveFileDialog->start();

}


void MainWindow::on_actionCopyToDesktop_triggered()
{
    QList<QListWidgetItem*> items = this->ui->listWidget->selectedItems();
    if(items.isEmpty())
    {
        qDebug() << "没有选择文件!";
    }

    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    qDebug() << "复制到:" << desktopPath;

    QList<QString> files;
    for (int i = 0; i < items.size(); i ++)
    {
        files.append(items.at(i)->data(Qt::UserRole).toString());
    }

    CopyMoveFileDialog *copyMoveFileDialog = new CopyMoveFileDialog(files, desktopPath, false);
    copyMoveFileDialog->start();

}


void MainWindow::on_actionSortName_triggered()
{
    if(this->ui->actionSortName->isChecked())
    {
        this->ui->actionSortSize->setChecked(false);
        this->ui->actionSortPath->setChecked(false);
        this->ui->actionSortType->setChecked(false);
        this->ui->actionSortExt->setChecked(false);
        this->ui->actionSortCreateTime->setChecked(false);
        this->ui->actionSortModifyTime->setChecked(false);
        DataCenter::GetInstance()->setSortType(SORT_TYPE::BY_NAME);
    }
}

void MainWindow::on_actionSortPath_triggered()
{
    if(this->ui->actionSortPath->isChecked())
    {
        this->ui->actionSortName->setChecked(false);
        this->ui->actionSortSize->setChecked(false);
        this->ui->actionSortType->setChecked(false);
        this->ui->actionSortExt->setChecked(false);
        this->ui->actionSortCreateTime->setChecked(false);
        this->ui->actionSortModifyTime->setChecked(false);
        DataCenter::GetInstance()->setSortType(SORT_TYPE::BY_SIZE);
    }
}

void MainWindow::on_actionSortSize_triggered()
{
    if(this->ui->actionSortSize->isChecked())
    {
        this->ui->actionSortName->setChecked(false);
        this->ui->actionSortPath->setChecked(false);
        this->ui->actionSortType->setChecked(false);
        this->ui->actionSortExt->setChecked(false);
        this->ui->actionSortCreateTime->setChecked(false);
        this->ui->actionSortModifyTime->setChecked(false);
        DataCenter::GetInstance()->setSortType(SORT_TYPE::BY_SIZE);
    }
}

void MainWindow::on_actionSortType_triggered()
{
    if(this->ui->actionSortType->isChecked())
    {
        this->ui->actionSortName->setChecked(false);
        this->ui->actionSortPath->setChecked(false);
        this->ui->actionSortSize->setChecked(false);
        this->ui->actionSortExt->setChecked(false);
        this->ui->actionSortCreateTime->setChecked(false);
        this->ui->actionSortModifyTime->setChecked(false);
        DataCenter::GetInstance()->setSortType(SORT_TYPE::BY_TYPE);
    }
}

void MainWindow::on_actionSortExt_triggered()
{
    if(this->ui->actionSortExt->isChecked())
    {
        this->ui->actionSortName->setChecked(false);
        this->ui->actionSortPath->setChecked(false);
        this->ui->actionSortSize->setChecked(false);
        this->ui->actionSortType->setChecked(false);
        this->ui->actionSortCreateTime->setChecked(false);
        this->ui->actionSortModifyTime->setChecked(false);
        DataCenter::GetInstance()->setSortType(SORT_TYPE::BY_EXT);
    }
}

void MainWindow::on_actionSortModifyTime_triggered()
{
    if(this->ui->actionSortModifyTime->isChecked())
    {
        this->ui->actionSortName->setChecked(false);
        this->ui->actionSortPath->setChecked(false);
        this->ui->actionSortSize->setChecked(false);
        this->ui->actionSortType->setChecked(false);
        this->ui->actionSortExt->setChecked(false);
        this->ui->actionSortCreateTime->setChecked(false);
        DataCenter::GetInstance()->setSortType(SORT_TYPE::BY_MTIME);
    }
}

void MainWindow::on_actionSortCreateTime_triggered()
{
    if(this->ui->actionSortCreateTime->isChecked())
    {
        this->ui->actionSortName->setChecked(false);
        this->ui->actionSortPath->setChecked(false);
        this->ui->actionSortSize->setChecked(false);
        this->ui->actionSortType->setChecked(false);
        this->ui->actionSortExt->setChecked(false);
        this->ui->actionSortModifyTime->setChecked(false);
        DataCenter::GetInstance()->setSortType(SORT_TYPE::BY_CTIME);
    }
}

void MainWindow::on_actionSortAsc_triggered()
{
    if(this->ui->actionSortAsc->isChecked())
    {
        this->ui->actionSortDesc->setChecked(false);
        DataCenter::GetInstance()->setSortOrder(SORT_ORDER::SORT_ASC);
    }
}

void MainWindow::on_actionSortDesc_triggered()
{
    if(this->ui->actionSortDesc->isChecked())
    {
        this->ui->actionSortAsc->setChecked(false);
        DataCenter::GetInstance()->setSortOrder(SORT_ORDER::SORT_DESC);
    }
}

void MainWindow::on_actionMatchCase_triggered()
{
    if(this->ui->actionMatchCase->isChecked())
    {
        DataCenter::GetInstance()->setIgnoreCase(false);
    }
    else
    {
        DataCenter::GetInstance()->setIgnoreCase(true);
    }
}

void MainWindow::on_actionAboutBSearch_triggered()
{
    if(mAboutDialog == nullptr)
    {
        mAboutDialog = new AboutDialog();
    }
    mAboutDialog->show();
}

void MainWindow::on_actionCheckUpdate_triggered()
{
    QString currentPath = QDir::currentPath();
    qDebug() << "currentPath:" << currentPath;

    qDebug()<<"QSslSocket="<<QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();

    reply = qnam.get(QNetworkRequest(QUrl(checkUpdateUrl)));
    connect(reply, &QNetworkReply::finished, this, &MainWindow::httpFinished);
    connect(reply, &QIODevice::readyRead, this, &MainWindow::httpReadyRead);

}

void MainWindow::httpFinished()
{
    if(reply->error() == QNetworkReply::NoError)
    {
       QByteArray jsonData = reply->readAll();
       qDebug() << "http finished size:" << jsonData.size();
    }
}

void MainWindow::httpReadyRead()
{


}


//#ifndef QT_NO_SSL
void MainWindow::sslErrors(QNetworkReply *, const QList<QSslError> &errors)
{
    QString errorString;
    for (const QSslError &error : errors) {
        if (!errorString.isEmpty())
            errorString += '\n';
        errorString += error.errorString();
    }

    if (QMessageBox::warning(this, tr("SSL Errors"),
                             tr("One or more SSL errors has occurred:\n%1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
        reply->ignoreSslErrors();
    }
}
//#endif


void MainWindow::error(QNetworkReply::NetworkError)
{
    //如果发生网络错误，就用备用地址重新检测
    qDebug() << "网络错误！使用备用地址！";
    reply = qnam.get(QNetworkRequest(QUrl(checkUpdateBakUrl)));
    connect(reply, &QNetworkReply::finished, this, &MainWindow::httpFinished);
    connect(reply, &QIODevice::readyRead, this, &MainWindow::httpReadyRead);
}
