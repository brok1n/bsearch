#include "indexthread.h"
#include "datacenter.h"

#include <QStorageInfo>
#include <QDebug>
#include <QThreadPool>

IndexThread::IndexThread()
{

}

IndexThread::~IndexThread()
{
    for(int i = 0; i < mPartitionThreadList.size(); i ++)
    {
        mPartitionThreadList[i]->stop();
    }
}

void IndexThread::run()
{
    mRunning = true;
    QList<QStorageInfo> list = QStorageInfo::mountedVolumes();
    int count = list.size();
    QString strInfo = "";
    for(int i = 0; i < count; ++i)
    {
        QStorageInfo diskInfo = list.at(i);
        qint64 freeSize = diskInfo.bytesFree();
        qint64 totalSize = diskInfo.bytesTotal();
//        qDebug(diskInfo.displayName().toLatin1());
        qDebug() << "磁盘：" << diskInfo.name() << " " << diskInfo.rootPath();
        Node *node = new Node();
        node->name = diskInfo.rootPath();
        DataCenter::GetInstance()->fileTree()->insert(diskInfo.rootPath(), node);
        PartitionIndexThread *pit = new PartitionIndexThread(diskInfo.rootPath());
        mPartitionThreadList.append(pit);
        pit->start();
    }

    DataCenter::GetInstance()->threadPool()->waitForDone();

    for(int i = 0; i < mPartitionThreadList.size(); i ++)
    {
        PartitionIndexThread *thread = mPartitionThreadList.at(i);
        thread->wait();
    }

    qDebug() << "所有磁盘扫描完毕!";

}

void IndexThread::stop()
{
    for(int i = 0; i < mPartitionThreadList.size(); i ++)
    {
        PartitionIndexThread *thread = mPartitionThreadList.at(i);
        thread->stop();
    }
}
