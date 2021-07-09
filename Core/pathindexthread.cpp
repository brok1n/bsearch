#include "datacenter.h"
#include "pathindexthread.h"

#include <QDir>
#include <QDebug>

PathIndexThread::PathIndexThread(QFileInfo info, Node *node, int level)
    : mInfo(info)
    , mRunning(false)
    , mRootNode(node)
    , mLevel(level)
{

}

PathIndexThread::~PathIndexThread()
{
    qDebug() << "删除";
}

void PathIndexThread::run()
{
    qDebug() << "开始遍历：" << mInfo.filePath();
    mRunning = true;
    eachDir(mInfo, mRootNode);
    qDebug() << "PathIndexThread finished:" << mInfo.filePath();
//    qDebug() << "delete:" << mInfo.filePath();
    //    delete this;
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
    dir.setFilter(QDir::Files|QDir::Dirs|QDir::NoDot|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name);
    QFileInfoList fileList = dir.entryInfoList();
    for(int i = 0; i < fileList.size(); i ++)
    {
        QThread::usleep(0);
        if(!mRunning)
        {
            return;
        }
        QFileInfo f = fileList.at(i);
        Node *node = new Node();
        node->name = f.fileName();
//        parent->childs.append(node);
        parent->addChild(node);
        if(f.isDir())
        {
//            qDebug() << "dir:" << f.filePath();
//            eachDir(f, node);
            if(mLevel < 5)
            {
                PathIndexThread *pathThread = new PathIndexThread(f, node, mLevel+1);
    //            mPathThreadList.append(pathThread);
                pathThread->start();
            }
            else
            {
                eachDir(f, node);
            }
        }
        else
        {
//            qDebug() << "file:" << f.filePath();
//            DataCenter::GetInstance()->filePathList()->append(f.filePath());
//            DataCenter::GetInstance()->fileList()->append(f);
        }
    }
}
