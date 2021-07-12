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
    QElapsedTimer timer;
    timer.start();

    //磁盘扫描状态
    DataCenter::GetInstance()->setScanDiskFinished(false);

    mRunning = true;
    QList<QStorageInfo> list = QStorageInfo::mountedVolumes();
    int count = list.size();
    QString strInfo = "";
    DataCenter::GetInstance()->setPartitionCount(list.size());
    for(int i = 0; i < count; ++i)
    {
//        QElapsedTimer partitionTimer;
//        partitionTimer.start();
        QStorageInfo diskInfo = list.at(i);
        qint64 freeSize = diskInfo.bytesFree();
        qint64 totalSize = diskInfo.bytesTotal();
//        qDebug(diskInfo.displayName().toLatin1());
        qDebug() << "开始处理磁盘：" << diskInfo.name() << " " << diskInfo.rootPath();
        Node *node = new Node();
        node->isDir = true;
        node->name = diskInfo.rootPath();
        DataCenter::GetInstance()->fileTree()->insert(diskInfo.rootPath(), node);
        PartitionIndexThread *pit = new PartitionIndexThread(diskInfo.rootPath());
        mPartitionThreadList.append(pit);
        pit->start();
//        pit->wait();
//        DataCenter::GetInstance()->threadPool()->waitForDone();
//        qint64 useTime = partitionTimer.elapsed();
//        qDebug() << "磁盘:" << diskInfo.name() << " 处理完毕！ 耗时:" << (useTime / 1000) << "秒" << (useTime % 1000) << "毫秒";
    }



    for(int i = 0; i < mPartitionThreadList.size(); i ++)
    {
        PartitionIndexThread *thread = mPartitionThreadList.at(i);
        thread->wait();
    }

//    DataCenter::GetInstance()->threadPool()->waitForDone();

    qDebug() << "所有磁盘扫描完毕!";
    qint64 useTime = timer.elapsed();
    qDebug() << "耗时:" << useTime << "  " << (useTime / 1000) << "秒" << (useTime % 1000) << "毫秒";
    DataCenter::GetInstance()->setScanDiskFinished(true);
}

void IndexThread::stop()
{
    for(int i = 0; i < mPartitionThreadList.size(); i ++)
    {
        PartitionIndexThread *thread = mPartitionThreadList.at(i);
        thread->stop();
    }
}
