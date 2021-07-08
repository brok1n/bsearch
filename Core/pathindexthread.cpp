#include "datacenter.h"
#include "pathindexthread.h"

#include <QDir>
#include <QDebug>

PathIndexThread::PathIndexThread(QFileInfo info, Node *node)
    : mInfo(info)
    , mRunning(false)
    , mRootNode(node)
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
    eachDir(mInfo);
//    qDebug() << "delete:" << mInfo.filePath();
    //    delete this;
}

void PathIndexThread::stop()
{
    mRunning = false;
}

void PathIndexThread::eachDir(QFileInfo info)
{
    if(!mRunning)
    {
        return;
    }
    qDebug() << "PathIndexThread each:" << info.filePath();
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
        mRootNode->childs.append(node);
        if(f.isDir())
        {
            qDebug() << "dir:" << f.filePath();
            eachDir(f);
        }
        else
        {
            qDebug() << "file:" << f.filePath();
//            DataCenter::GetInstance()->filePathList()->append(f.filePath());
//            DataCenter::GetInstance()->fileList()->append(f);
        }
    }
}
