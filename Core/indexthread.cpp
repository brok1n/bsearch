#include "indexthread.h"
#include "datacenter.h"

#include <QStorageInfo>
#include <QDebug>
#include <QThreadPool>
#include <QElapsedTimer>

IndexThread::IndexThread()
{

}

IndexThread::~IndexThread()
{
    for(int i = 0; i < mPartitionThreadList.size(); i ++)
    {
        PartitionIndexThread *thread = mPartitionThreadList.at(i);
        thread->deleteLater();
    }

    qDebug() << "~IndexThread()";
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
        QStorageInfo diskInfo = list.at(i);
        //qint64 freeSize = diskInfo.bytesFree();
        qint64 totalSize = diskInfo.bytesTotal();
        qDebug() << "开始处理磁盘：" << diskInfo.name() << " " << diskInfo.rootPath();
        Node *node = Node::create(diskInfo.rootPath(), nullptr, true);
        node->setFileSize(totalSize);
        DataCenter::GetInstance()->fileTree()->insert(diskInfo.rootPath(), node);
        PartitionIndexThread *pit = new PartitionIndexThread(diskInfo.rootPath());
        mPartitionThreadList.append(pit);
        pit->start();
    }

    for(int i = 0; i < mPartitionThreadList.size(); i ++)
    {
        PartitionIndexThread *thread = mPartitionThreadList.at(i);
        thread->wait();
    }

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
        thread->deleteLater();
    }
}
