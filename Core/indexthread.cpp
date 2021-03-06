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
    this->stop();
    for(int i = 0; i < mPartitionThreadList.size(); i ++)
    {
//        mPartitionThreadList.at(i)->deleteLater();
        delete mPartitionThreadList.at(i);
    }
    mPartitionThreadList.clear();
    qDebug() << "~IndexThread()";
}

void IndexThread::run()
{
    QElapsedTimer timer;
    timer.start();

    //磁盘扫描状态
    DataCenter::GetInstance()->setScanDiskFinished(false);
    QList<QStorageInfo> list = QStorageInfo::mountedVolumes();
    int count = list.size();
    QString strInfo = "";
    DataCenter::GetInstance()->setPartitionCount(list.size());
    for(int i = 0; i < count; ++i)
    {
        if(!DataCenter::GetInstance()->isRunning())
        {
            break;
        }
        QStorageInfo diskInfo = list.at(i);
        //qint64 freeSize = diskInfo.bytesFree();
        qint64 totalSize = diskInfo.bytesTotal();
        qDebug() << "开始处理磁盘：" << diskInfo.name() << " " << diskInfo.rootPath();
        Node *node = Node::create(diskInfo.rootPath(), nullptr);
        node->setFileSize(totalSize);
        DataCenter::GetInstance()->fileTree()->insert(diskInfo.rootPath(), node);
        PartitionIndexThread *pit = new PartitionIndexThread(diskInfo.rootPath());
        mPartitionThreadList.append(pit);
        pit->start();
    }

    for(int i = 0; i < mPartitionThreadList.size(); i ++)
    {
        if(!DataCenter::GetInstance()->isRunning())
        {
            break;
        }
        PartitionIndexThread *thread = mPartitionThreadList.at(i);
        thread->wait();
    }

    if(!DataCenter::GetInstance()->isRunning())
    {
        return;
    }

    qDebug() << "所有磁盘扫描完毕!";
    qint64 useTime = timer.elapsed();
    qDebug() << "耗时:" << useTime << "  " << (useTime / 1000) << "秒" << (useTime % 1000) << "毫秒";

    //开始计算扫描了多少个文件和文件夹
    sumFileAndDirCount();

    DataCenter::GetInstance()->setScanDiskFinished(true);
}

void IndexThread::stop()
{
    for(int i = 0; i < mPartitionThreadList.size(); i ++)
    {
        mPartitionThreadList.at(i)->stop();
    }
    for(int i = 0; i < mPartitionThreadList.size(); i ++)
    {
        mPartitionThreadList.at(i)->wait();
    }
}

void IndexThread::sumFileAndDirCount()
{
    QList<QString> keys = DataCenter::GetInstance()->fileTree()->keys();
    for(int i = 0; i < keys.size(); i ++)
    {
        Node *node = DataCenter::GetInstance()->fileTree()->value(keys.at(i));
        eachNode(node);
    }
}

void IndexThread::eachNode(Node *node)
{
    if(node->isDir())
    {
        DataCenter::GetInstance()->addOneDir();
        QList<Node*> list = node->childs;
        for(int i = 0; i < list.size(); i++)
        {
            eachNode(list.at(i));
        }
    }
    else
    {
        DataCenter::GetInstance()->addOneFile();
    }
}
