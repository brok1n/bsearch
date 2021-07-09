#include "datacenter.h"
#include "partitionindexthread.h"
#include <QDebug>
#include <QDir>
#include <QFileInfoList>

PartitionIndexThread::PartitionIndexThread(QString rootPath)
    : mRootPath(rootPath)
    , mRunning(false)
{

}

void PartitionIndexThread::run()
{
    qDebug("PartitionIndexThread run:");
    mRunning = true;
    while (mRunning) {
        qDebug() << "path:" << mRootPath;
        QDir dir(mRootPath);
        if(!dir.isReadable())
        {
            mRunning = false;
            return;
        }
        dir.setFilter(QDir::Dirs|QDir::NoDot|QDir::NoDotAndDotDot);
        dir.setSorting(QDir::Name);
        QFileInfoList fileList = dir.entryInfoList();

        Node *rootNode = DataCenter::GetInstance()->fileTree()->value(mRootPath);

        for(int i = 0; i < fileList.size(); i ++)
        {
            QFileInfo fileInfo = fileList.at(i);
            Node *node = new Node();
            node->name = fileInfo.fileName();
            rootNode->addChild(node);
//            rootNode->childs.append(node);
            if(fileInfo.isDir())
            {
//                if(!fileInfo.filePath().contains("Windows"))
//                {
//                    continue;
//                }
//                qDebug() << "pathIndex:" << fileInfo.filePath();
                PathIndexThread *pathThread = new PathIndexThread(fileInfo, node, 0);
                mPathThreadList.append(pathThread);
                pathThread->start();
//                break;
            }
            else
            {
                DataCenter::GetInstance()->fileList()->append(fileInfo);
            }
        }
        break;
        qDebug() << "files:" << DataCenter::GetInstance()->fileList()->size();
//        QThread::sleep(600);
    }

    for(int i = 0; i < mPathThreadList.size(); i ++)
    {
        PathIndexThread *thread = mPathThreadList.at(i);
        thread->wait();
        qDebug() << "path index thread exit.";
//        delete thread;
    }

    qDebug() << "分区处理完毕:" << mRootPath;
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
