#include "datacenter.h"
#include "partitionindexthread.h"
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QThreadPool>
#include <QStandardPaths>
#include <QElapsedTimer>

PartitionIndexThread::PartitionIndexThread(QString rootPath)
    : mRootPath(rootPath)
{
    mThreadPool = new QThreadPool;
    mThreadPool->setMaxThreadCount(DataCenter::GetInstance()->singleThreadCount());
    mThreadPool->setExpiryTimeout(-1);
}

PartitionIndexThread::~PartitionIndexThread()
{
    this->stop();
    delete mThreadPool;
//    mThreadPool->deleteLater();
    qDebug() << "~PartitionIndexThread():" << mRootPath;
}

void PartitionIndexThread::run()
{
    qDebug("PartitionIndexThread run:");
    QElapsedTimer partitionTimer;
    partitionTimer.start();
    QDir dir(mRootPath);
    if(!dir.isReadable())
    {
        return;
    }
//    dir.setFilter(QDir::Readable|QDir::Dirs|QDir::NoDot|QDir::NoDotAndDotDot);
    dir.setFilter(QDir::NoDotAndDotDot|QDir::AllEntries);
    dir.setSorting(QDir::DirsLast);
    QFileInfoList fileList = dir.entryInfoList();

    //分区根节点
    Node *rootNode = DataCenter::GetInstance()->fileTree()->value(mRootPath);

    //单独处理windows系统分区
    if(isWinSystemPartition(mRootPath)) {
        qDebug() << "该分区为Windows系统分区";
        QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        //windows的系统盘只需要扫描几个特定目录即可
        qDebug() << "用户目录:" << homePath;

        QStringList homePathList = homePath.split('/');

        Node *lastNode = rootNode;
        //从第二个元素开始遍历，因为第一个元素是盘符
        for(int i = 1; i < homePathList.size(); i ++)
        {
            Node *node = Node::create(homePathList.at(i), lastNode, true);
            lastNode = node;
        }

        //3D目录
        QString home3DPath = QString(homePath).append('/').append("3D Objects");
        QFileInfo home3DInfo(home3DPath);
        Node *home3DNode = Node::create(home3DInfo.fileName(), lastNode, true);
        PathIndexRunnable *pathThread = new PathIndexRunnable(home3DInfo, home3DNode, 1, mThreadPool);
        mThreadPool->start(pathThread);

        //Videos目录
        QString homeVideosPath = QString(homePath).append('/').append("Videos");
        QFileInfo homeVideosInfo(homeVideosPath);
        Node *homeVideosNode = Node::create(homeVideosInfo.fileName(), lastNode, true);
        pathThread = new PathIndexRunnable(homeVideosInfo, homeVideosNode, 1, mThreadPool);
        mThreadPool->start(pathThread);


        //Pictures目录
        QString homePicturesPath = QString(homePath).append('/').append("Pictures");
        QFileInfo homePicturesInfo(homePicturesPath);
        Node *homePicturesNode = Node::create(homePicturesInfo.fileName(), lastNode, true);
        pathThread = new PathIndexRunnable(homePicturesInfo, homePicturesNode, 1, mThreadPool);
        mThreadPool->start(pathThread);

        //Documents目录
        QString homeDocumentsPath = QString(homePath).append('/').append("Documents");
        QFileInfo homeDocumentsInfo(homeDocumentsPath);
        Node *homeDocumentsNode = Node::create(homeDocumentsInfo.fileName(), lastNode, true);
        pathThread = new PathIndexRunnable(homeDocumentsInfo, homeDocumentsNode, 1, mThreadPool);
        mThreadPool->start(pathThread);

        //Downloads目录
        QString homeDownloadsPath = QString(homePath).append('/').append("Downloads");
        QFileInfo homeDownloadsInfo(homeDownloadsPath);
        Node *homeDownloadsNode = Node::create(homeDownloadsInfo.fileName(), lastNode, true);
        pathThread = new PathIndexRunnable(homeDownloadsInfo, homeDownloadsNode, 1, mThreadPool);
        mThreadPool->start(pathThread);

        //Music目录
        QString homeMusicPath = QString(homePath).append('/').append("Music");
        QFileInfo homeMusicInfo(homeMusicPath);
        Node *homeMusicNode = Node::create(homeMusicInfo.fileName(), lastNode, true);
        pathThread = new PathIndexRunnable(homeMusicInfo, homeMusicNode, 1, mThreadPool);
        mThreadPool->start(pathThread);

        //Desktop目录
        QString homeDesktopPath = QString(homePath).append('/').append("Desktop");
        QFileInfo homeDesktopInfo(homeDesktopPath);
        Node *homeDesktopNode = Node::create(homeDesktopInfo.fileName(), lastNode, true);
        pathThread = new PathIndexRunnable(homeDesktopInfo, homeDesktopNode, 1, mThreadPool);
        mThreadPool->start(pathThread);

        DataCenter::GetInstance()->printNode(rootNode, 0);

    }
    else
    {
        //遍历分区文件列表
        for(int i = 0; i < fileList.size(); i ++)
        {
            if(!DataCenter::GetInstance()->isRunning())
            {
                break;
            }
            QFileInfo fileInfo = fileList.at(i);
            Node *node = Node::create(fileInfo.fileName(), rootNode);
            node->setFileSize(fileInfo.size());
            if(fileInfo.isDir())
            {
                node->isDir = true;
    //            if(node->name.startsWith("$") || node->name.startsWith("."))
    //            {
    //                continue;
    //            }
    //          qDebug() << "pathIndex:" << fileInfo.filePath();
                PathIndexRunnable *pathThread = new PathIndexRunnable(fileInfo, node, 0, mThreadPool);
                mThreadPool->start(pathThread);
            }
        }
    }

    mThreadPool->waitForDone();

    if(!DataCenter::GetInstance()->isRunning())
    {
        return;
    }

    qint64 useTime = partitionTimer.elapsed();
    qDebug() << "分区处理完毕:" << mRootPath << " 耗时：" << (useTime / 1000) << "秒" << (useTime % 1000) << "毫秒";

    DataCenter::GetInstance()->setScanFinishedCount(DataCenter::GetInstance()->scanFinishedCount() + 1);
}

void PartitionIndexThread::stop()
{
    mThreadPool->setMaxThreadCount(0);
    mThreadPool->clear();
    mThreadPool->releaseThread();
}

bool PartitionIndexThread::isWinSystemPartition(QString path)
{
    QDir dir(path);
    dir.setFilter(QDir::NoDotAndDotDot|QDir::AllEntries);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList fileList = dir.entryInfoList();
    int status = 0;
    for(int i = 0; i < fileList.size(); i ++)
    {
        QFileInfo f = fileList.at(i);
        if(f.fileName() == "Windows" || f.fileName() == "Users" || f.fileName().contains("Program Files")) {
            status ++;
        }
    }
    if(status >= 3)
    {
        return true;
    }
    return false;
}
