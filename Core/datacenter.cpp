#include "datacenter.h"
#include<QDebug>

DataCenter* DataCenter::mInstance = nullptr;
QMutex  DataCenter::mMutex;

DataCenter::DataCenter(QObject *parent)
    : QObject(parent)
//    , mFileList(new QList<QFileInfo>())
//    , mFilePathList(new QList<QString>())
    , mTree(new QMap<QString, Node*>())
    , mResultList(new QList<Node*>())
    , mSearchFinished(false)
    , mPartitionCount(0)
    , mScanFinishedCount(0)
    , mScanDiskFinished(false)
{
    mPool = new QThreadPool();
    mPool->setMaxThreadCount(10);
    mPool->setExpiryTimeout(-1);
}

DataCenter::~DataCenter()
{
    qDebug("DataCenter delete");
}

//QList<QFileInfo> *DataCenter::fileList()
//{
//    return mFileList;
//}

//QList<QString> *DataCenter::filePathList()
//{
//    return mFilePathList;
//}

QMap<QString, Node *>* DataCenter::fileTree()
{
    return mTree;
}

//QThreadPool* DataCenter::threadPool()
//{
//    return mPool;
//}

QList<Node*>* DataCenter::resultList()
{
    return mResultList;
}

bool DataCenter::isSearchFinished()
{
    return mSearchFinished;
}

void DataCenter::setSearchFinished(bool status)
{
    mSearchFinished = status;
}

bool DataCenter::isScanDiskFinished()
{
    return mScanDiskFinished;
}

void DataCenter::setScanDiskFinished(bool status)
{
    mScanDiskFinished = status;
}

int DataCenter::partitionCount()
{
    return mPartitionCount;
}

void DataCenter::setPartitionCount(int count)
{
    mPartitionCount = count;
    mSingleThreadCount = MAX_THREAD_COUNT / count;
}

int DataCenter::scanFinishedCount()
{
    return mScanFinishedCount;
}

void DataCenter::setScanFinishedCount(int count)
{
    mScanFinishedCount = count;
    mSingleThreadCount = MAX_THREAD_COUNT / (mPartitionCount - count);
}

int DataCenter::singleThreadCount()
{
    return mSingleThreadCount;
}

void DataCenter::setSingleThreadCount(int count)
{
    mSingleThreadCount = count;
}

DataCenter* DataCenter::GetInstance()
{
    if (mInstance == nullptr)
    {
        QMutexLocker locker(&mMutex);
        if (mInstance == nullptr)
        {
            mInstance = new DataCenter();
        }
    }
    return mInstance;
}

void DataCenter::Release()
{
    delete mInstance;
}

void DataCenter::printNode(Node *node, int level)
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
