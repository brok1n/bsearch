#include "datacenter.h"
#include<QDebug>

DataCenter* DataCenter::mInstance = nullptr;
QMutex  DataCenter::mMutex;

DataCenter::DataCenter(QObject *parent)
    : QObject(parent)
    , mTree(new QMap<QString, Node*>())
    , mResultList(new QList<Node*>())
    , mSearchFinished(false)
    , mPartitionCount(0)
    , mScanFinishedCount(0)
    , mScanDiskFinished(false)
    , mIsRunning(true)
    , mSortType(SORT_TYPE::BY_NAME)
    , mSortOrder(SORT_ORDER::SORT_ASC)
    , mIgnoreCase(true)
{

}

DataCenter::~DataCenter()
{
    mTree->clear();
    delete mTree;
    mResultList->clear();
    delete mResultList;
    qDebug("~DataCenter()");
}

QMap<QString, Node *>* DataCenter::fileTree()
{
    return mTree;
}

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
    int tmpPartitionCount = mPartitionCount - count;
    if(tmpPartitionCount <= 0)
    {
        mSingleThreadCount = MAX_THREAD_COUNT;
    }
    else
    {
        mSingleThreadCount = MAX_THREAD_COUNT / tmpPartitionCount;
    }
}

int DataCenter::singleThreadCount()
{
    return mSingleThreadCount;
}

void DataCenter::setSingleThreadCount(int count)
{
    mSingleThreadCount = count;
}

bool DataCenter::isRunning()
{
    return mIsRunning;
}

void DataCenter::setRunning(bool status)
{
    mIsRunning = status;
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

int DataCenter::sortType()
{
    return mSortType;
}

void DataCenter::setSortType(int tp)
{
    this->mSortType = tp;
}

int DataCenter::sortOrder()
{
    return mSortOrder;
}

void DataCenter::setSortOrder(int od)
{
    this->mSortOrder = od;
}

bool DataCenter::ignoreCase()
{
    return mIgnoreCase;
}

void DataCenter::setIgnoreCase(bool cs)
{
    this->mIgnoreCase = cs;
}
