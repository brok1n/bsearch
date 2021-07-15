#include "datacenter.h"
#include "pathindexthread.h"

#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QThreadPool>

PathIndexThread::PathIndexThread(QFileInfo info, Node *node, int level, QThreadPool *pool)
    : mInfo(info)
    , mRunning(false)
    , mRootNode(node)
    , mLevel(level)
    , mThreadPool(pool)
{

}

PathIndexThread::~PathIndexThread()
{
//    qDebug() << "PathIndexThread 删除" << mInfo.filePath();
}

void PathIndexThread::run()
{

//    qDebug() << "开始遍历：" << mInfo.filePath();
    mRunning = true;
    eachDir(mInfo, mRootNode);
//    for(int i = 0; i < mPathIndexThreadList.size(); i ++)
//    {
//        PathIndexThread *thread = mPathIndexThreadList.at(i);
//        thread->wait();
//    }
//    qDebug() << "PathIndexThread finished:" << mInfo.filePath();
//    printNode(mRootNode, 1);
//    qDebug() << "delete:" << mInfo.filePath();
//    delete this;
}


void PathIndexThread::printNode(Node *node, int level)
{
    QString tmpName = "";
    for(int i = 0; i < level * 5; i ++)
    {
        tmpName.append(' ');
    }
    tmpName.append(node->name);
    qDebug() <<  tmpName.toStdString().data();
    for(int i = 0; i < node->childs.size(); i ++)
    {
        Node *n = node->childs.at(i);
        printNode(n, level+1);
    }
}


void PathIndexThread::stop()
{
    mRunning = false;
}

void PathIndexThread::eachDir(QFileInfo info, Node *parent)
{
    if(!mRunning)
    {
        return;
    }
//    qDebug() << "PathIndexThread each:" << info.filePath();
    QDir dir(info.filePath());
    dir.setFilter(QDir::Readable|QDir::Hidden|QDir::Files|QDir::NoSymLinks|QDir::Dirs|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsLast);
    QFileInfoList fileList = dir.entryInfoList();
    for(int i = 0; i < fileList.size(); i ++)
    {
        QThread::usleep(0);
        if(!mRunning)
        {
            return;
        }
        QFileInfo f = fileList.at(i);
        Node *node = Node::create(f.fileName(), parent);
        if(f.isDir())
        {
            node->isDir = true;
            QString tmpName = node->name.toLower();
            if(tmpName.startsWith(".") || tmpName.startsWith("_") || tmpName.startsWith("node_modules"))
            {
                continue;
            }
//            qDebug() << "dir:" << f.filePath();
//            eachDir(f, node);
            int level = 5;
            if(DataCenter::GetInstance()->scanFinishedCount() >= DataCenter::GetInstance()->partitionCount() / 2)
            {
                level = 7;
            }
            if(DataCenter::GetInstance()->singleThreadCount() != mThreadPool->maxThreadCount())
            {
                mThreadPool->setMaxThreadCount(DataCenter::GetInstance()->singleThreadCount());
            }
            if(mLevel < level)
            {
                PathIndexThread *pathThread = new PathIndexThread(f, node, mLevel+1, mThreadPool);
                mPathIndexThreadList.append(pathThread);
//                pathThread->start();
                mThreadPool->start(pathThread);
//                QThreadPool::globalInstance()->start(pathThread);
            }
            else
            {
                eachDir(f, node);
            }
        }
        else
        {
            node->setFileSize(f.size());
//            qDebug() << "file:" << f.filePath();
//            DataCenter::GetInstance()->filePathList()->append(f.filePath());
//            DataCenter::GetInstance()->fileList()->append(f);
        }
    }
}
