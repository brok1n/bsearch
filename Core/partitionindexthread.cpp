#include "datacenter.h"
#include "partitionindexthread.h"
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QThreadPool>
#include <QStandardPaths>

PartitionIndexThread::PartitionIndexThread(QString rootPath)
    : mRootPath(rootPath)
    , mRunning(false)
{
    mThreadPool = new QThreadPool;
    mThreadPool->setMaxThreadCount(DataCenter::GetInstance()->singleThreadCount());
    mThreadPool->setExpiryTimeout(-1);
}

PartitionIndexThread::~PartitionIndexThread()
{

}

void PartitionIndexThread::run()
{
//    qDebug("PartitionIndexThread run:");
    QElapsedTimer partitionTimer;
    partitionTimer.start();
    mRunning = true;
    QDir dir(mRootPath);
    if(!dir.isReadable())
    {
        mRunning = false;
        return;
    }
    dir.setFilter(QDir::Readable|QDir::Dirs|QDir::NoDot|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
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
            Node *node = new Node();
            node->name = homePathList.at(i);
            node->isDir = true;
            lastNode->addChild(node);
            lastNode = node;
        }

        //3D目录
        QString home3DPath = QString(homePath).append('/').append("3D Objects");
        QFileInfo home3DInfo(home3DPath);
        Node *home3DNode = new Node();
        home3DNode->name = home3DInfo.fileName();
        home3DNode->isDir = true;
        lastNode->addChild(home3DNode);
        PathIndexThread *pathThread = new PathIndexThread(home3DInfo, home3DNode, 1, mThreadPool);
        mPathThreadList.append(pathThread);
        mThreadPool->start(pathThread);

        //Videos目录
        QString homeVideosPath = QString(homePath).append('/').append("Videos");
        QFileInfo homeVideosInfo(homeVideosPath);
        Node *homeVideosNode = new Node();
        homeVideosNode->isDir = true;
        homeVideosNode->name = homeVideosInfo.fileName();
        lastNode->addChild(homeVideosNode);
        pathThread = new PathIndexThread(homeVideosInfo, homeVideosNode, 1, mThreadPool);
        mPathThreadList.append(pathThread);
        mThreadPool->start(pathThread);


        //Pictures目录
        QString homePicturesPath = QString(homePath).append('/').append("Pictures");
        QFileInfo homePicturesInfo(homePicturesPath);
        Node *homePicturesNode = new Node();
        homePicturesNode->isDir = true;
        homePicturesNode->name = homePicturesInfo.fileName();
        lastNode->addChild(homePicturesNode);
        pathThread = new PathIndexThread(homePicturesInfo, homePicturesNode, 1, mThreadPool);
        mPathThreadList.append(pathThread);
        mThreadPool->start(pathThread);

        //Documents目录
        QString homeDocumentsPath = QString(homePath).append('/').append("Documents");
        QFileInfo homeDocumentsInfo(homeDocumentsPath);
        Node *homeDocumentsNode = new Node();
        homeDocumentsNode->isDir = true;
        homeDocumentsNode->name = homeDocumentsInfo.fileName();
        lastNode->addChild(homeDocumentsNode);
        pathThread = new PathIndexThread(homeDocumentsInfo, homeDocumentsNode, 1, mThreadPool);
        mPathThreadList.append(pathThread);
        mThreadPool->start(pathThread);

        //Downloads目录
        QString homeDownloadsPath = QString(homePath).append('/').append("Downloads");
        QFileInfo homeDownloadsInfo(homeDownloadsPath);
        Node *homeDownloadsNode = new Node();
        homeDownloadsNode->isDir = true;
        homeDownloadsNode->name = homeDownloadsInfo.fileName();
        lastNode->addChild(homeDownloadsNode);
        pathThread = new PathIndexThread(homeDownloadsInfo, homeDownloadsNode, 1, mThreadPool);
        mPathThreadList.append(pathThread);
        mThreadPool->start(pathThread);

        //Music目录
        QString homeMusicPath = QString(homePath).append('/').append("Music");
        QFileInfo homeMusicInfo(homeMusicPath);
        Node *homeMusicNode = new Node();
        homeMusicNode->isDir = true;
        homeMusicNode->name = homeMusicInfo.fileName();
        lastNode->addChild(homeMusicNode);
        pathThread = new PathIndexThread(homeMusicInfo, homeMusicNode, 1, mThreadPool);
        mPathThreadList.append(pathThread);
        mThreadPool->start(pathThread);

        //Desktop目录
        QString homeDesktopPath = QString(homePath).append('/').append("Desktop");
        QFileInfo homeDesktopInfo(homeDesktopPath);
        Node *homeDesktopNode = new Node();
        homeDesktopNode->isDir = true;
        homeDesktopNode->name = homeDesktopInfo.fileName();
        lastNode->addChild(homeDesktopNode);
        pathThread = new PathIndexThread(homeDesktopInfo, homeDesktopNode, 1, mThreadPool);
        mPathThreadList.append(pathThread);
        mThreadPool->start(pathThread);

        DataCenter::GetInstance()->printNode(rootNode, 0);

        mThreadPool->waitForDone();

        return;
    }

    //遍历分区文件列表
    for(int i = 0; i < fileList.size(); i ++)
    {
        if(!mRunning)
        {
            break;
        }
        QFileInfo fileInfo = fileList.at(i);
        Node *node = new Node();
        node->name = fileInfo.fileName();
        rootNode->addChild(node);
        if(fileInfo.isDir())
        {
            node->isDir = true;
            if(node->name.startsWith("$") || node->name.startsWith("."))
            {
                continue;
            }
//          qDebug() << "pathIndex:" << fileInfo.filePath();
            PathIndexThread *pathThread = new PathIndexThread(fileInfo, node, 0, mThreadPool);
            mPathThreadList.append(pathThread);
            mThreadPool->start(pathThread);
//                pathThread->start();
//                QThreadPool::globalInstance()->start(pathThread);
//                break;
        }
        else
        {
//                DataCenter::GetInstance()->fileList()->append(fileInfo);
        }
    }

    mThreadPool->waitForDone();
    qint64 useTime = partitionTimer.elapsed();
    qDebug() << "分区处理完毕:" << mRootPath << " 耗时：" << (useTime / 1000) << "秒" << (useTime % 1000) << "毫秒";

    DataCenter::GetInstance()->setScanFinishedCount(DataCenter::GetInstance()->scanFinishedCount() + 1);

}

void PartitionIndexThread::stop()
{
    mRunning = false;
    for(int i = 0; i < mPathThreadList.size(); i ++)
    {
        PathIndexThread *thread = mPathThreadList.at(i);
        thread->stop();
    }
}

bool PartitionIndexThread::isWinSystemPartition(QString path)
{
    QDir dir(path);
    dir.setFilter(QDir::Readable|QDir::Dirs|QDir::NoDot|QDir::NoDotAndDotDot);
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
