#include "datacenter.h"
#include<QDebug>

DataCenter* DataCenter::mInstance = nullptr;
QMutex  DataCenter::mMutex;

DataCenter::DataCenter(QObject *parent)
    : QObject(parent)
    , mFileList(new QList<QFileInfo>())
    , mFilePathList(new QList<QString>())
    , mTree(new QMap<QString, Node*>())
{

}

DataCenter::~DataCenter()
{
    qDebug("DataCenter delete");
}

QList<QFileInfo> *DataCenter::fileList()
{
    return mFileList;
}

QList<QString> *DataCenter::filePathList()
{
    return mFilePathList;
}

QMap<QString, Node *> *DataCenter::fileTree()
{
    return mTree;
}

DataCenter *DataCenter::GetInstance()
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
