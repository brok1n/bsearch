#include "datacenter.h"
#include "pathindexrunnable.h"

#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QThreadPool>

PathIndexRunnable::PathIndexRunnable(QFileInfo info, Node *node, int level, QThreadPool *pool)
    : mInfo(info)
    , mRunning(false)
    , mRootNode(node)
    , mLevel(level)
    , mThreadPool(pool)
{

}

PathIndexRunnable::~PathIndexRunnable()
{
//    qDebug() << "~PathIndexRunnable():" << mInfo.filePath();
}

void PathIndexRunnable::run()
{

//    qDebug() << "开始遍历：" << mInfo.filePath();
    mRunning = true;
    eachDir(mInfo, mRootNode);
}


void PathIndexRunnable::printNode(Node *node, int level)
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


void PathIndexRunnable::stop()
{
    mRunning = false;
    for(int i = 0; i < mPathIndexRunnableList.size(); i ++)
    {
        PathIndexRunnable *runnable = mPathIndexRunnableList.at(i);
        runnable->stop();
    }
}

void PathIndexRunnable::eachDir(QFileInfo info, Node *parent)
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
        node->setFileSize(f.size());
        if(f.isDir())
        {
            node->isDir = true;
            QString tmpName = node->name.toLower();
            if(tmpName.startsWith(".") || tmpName.startsWith("_") || tmpName.startsWith("node_modules"))
            {
                continue;
            }
//            qDebug() << "dir:" << f.filePath();
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
                PathIndexRunnable *pathThread = new PathIndexRunnable(f, node, mLevel+1, mThreadPool);
                mPathIndexRunnableList.append(pathThread);
                mThreadPool->start(pathThread);
            }
            else
            {
                eachDir(f, node);
            }
        }
    }
}
