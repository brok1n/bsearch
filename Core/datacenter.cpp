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
{
    mPool = new QThreadPool();
    mPool->setMaxThreadCount(200);
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

QThreadPool* DataCenter::threadPool()
{
    return mPool;
}

QList<Node*>* DataCenter::resultList()
{
    return mResultList;
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
