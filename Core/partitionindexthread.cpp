#include "datacenter.h"
#include "partitionindexthread.h"
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QThreadPool>

PartitionIndexThread::PartitionIndexThread(QString rootPath)
    : mRootPath(rootPath)
    , mRunning(false)
{

}

void PartitionIndexThread::run()
{
//    qDebug("PartitionIndexThread run:");
    mRunning = true;
    if(isWinSystemPartition(mRootPath)) {
        qDebug() << "is Windows Partition";
        return;
    }
    while (mRunning) {
//        qDebug() << "path:" << mRootPath;
        QDir dir(mRootPath);
        if(!dir.isReadable())
        {
            mRunning = false;
            return;
        }
        dir.setFilter(QDir::Readable|QDir::Dirs|QDir::NoDot|QDir::NoDotAndDotDot);
        dir.setSorting(QDir::DirsFirst);
        QFileInfoList fileList = dir.entryInfoList();

        Node *rootNode = DataCenter::GetInstance()->fileTree()->value(mRootPath);

        for(int i = 0; i < fileList.size(); i ++)
        {
            QFileInfo fileInfo = fileList.at(i);
            Node *node = new Node();
            node->name = fileInfo.fileName();
            rootNode->addChild(node);
            if(fileInfo.isDir())
            {
                if(node->name.startsWith("$") || node->name.startsWith(".") ||
                        node->name.contains("Program Files") || node->name == "ProgramData" || node->name == "Windows" || node->name == "Temp")
                {
                    continue;
                }
//                qDebug() << "pathIndex:" << fileInfo.filePath();
                PathIndexThread *pathThread = new PathIndexThread(fileInfo, node, 0);
                mPathThreadList.append(pathThread);
                DataCenter::GetInstance()->threadPool()->start(pathThread);
//                pathThread->start();
//                QThreadPool::globalInstance()->start(pathThread);
//                break;
            }
            else
            {
//                DataCenter::GetInstance()->fileList()->append(fileInfo);
            }
        }
        break;
//        qDebug() << "files:" << DataCenter::GetInstance()->fileList()->size();
//        QThread::sleep(600);
    }

//    for(int i = 0; i < mPathThreadList.size(); i ++)
//    {
//        PathIndexThread *thread = mPathThreadList.at(i);
//        thread->
////        qDebug() << "path index thread exit.";
////        delete thread;
//    }


//    if( DataCenter::GetInstance()->fileTree()->contains(mRootPath) )
//    {
//        Node *node = DataCenter::GetInstance()->fileTree()->value(mRootPath);
//        DataCenter::GetInstance()->printNode(node, 1);
//    }
//    qDebug() << "分区处理完毕:" << mRootPath;

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
