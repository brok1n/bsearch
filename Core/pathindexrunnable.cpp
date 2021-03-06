#include "datacenter.h"
#include "pathindexrunnable.h"

#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QThreadPool>

PathIndexRunnable::PathIndexRunnable(QFileInfo info, Node *node, int level, QThreadPool *pool)
    : mInfo(info)
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

void PathIndexRunnable::eachDir(QFileInfo info, Node *parent)
{
    if(!DataCenter::GetInstance()->isRunning())
    {
        return;
    }
//    qDebug() << "PathIndexThread each:" << info.filePath();
    QDir dir(info.filePath());
//    dir.setFilter(QDir::Readable|QDir::Hidden|QDir::Files|QDir::NoSymLinks|QDir::Dirs|QDir::NoDotAndDotDot);
    dir.setFilter(QDir::NoDotAndDotDot|QDir::AllEntries);
    dir.setSorting(QDir::DirsLast);
    QFileInfoList fileList = dir.entryInfoList();
    for(int i = 0; i < fileList.size(); i ++)
    {
        if(!DataCenter::GetInstance()->isRunning())
        {
            return;
        }
        QThread::usleep(0);
        QFileInfo f = fileList.at(i);
        Node *node = Node::create(f.fileName(), parent);
        node->setFileSize(f.size());
        if(DataCenter::GetInstance()->singleThreadCount() != mThreadPool->maxThreadCount())
        {
            mThreadPool->setMaxThreadCount(DataCenter::GetInstance()->singleThreadCount());
            qDebug() << "maxThreadCount change to " << DataCenter::GetInstance()->singleThreadCount();
        }
        if(f.isDir())
        {
            QString tmpName = node->name.toLower();
//            if(tmpName.startsWith(".") || tmpName.startsWith("_") || tmpName.startsWith("node_modules"))
//            {
//                continue;
//            }
//            qDebug() << "dir:" << f.filePath();
            int level = 5;
            if(DataCenter::GetInstance()->scanFinishedCount() >= DataCenter::GetInstance()->partitionCount() / 2)
            {
                level = 7;
            }
            if(mLevel < level)
            {
                PathIndexRunnable *pathThread = new PathIndexRunnable(f, node, mLevel+1, mThreadPool);
                mThreadPool->start(pathThread);
            }
            else
            {
                if(!DataCenter::GetInstance()->isRunning())
                {
                    return;
                }
                eachDir(f, node);
                if(!DataCenter::GetInstance()->isRunning())
                {
                    return;
                }
            }
        }
    }
}
