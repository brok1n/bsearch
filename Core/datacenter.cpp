#include "datacenter.h"
#include<QDebug>

DataCenter* DataCenter::mInstance = nullptr;
QMutex  DataCenter::mMutex;

DataCenter::DataCenter(QObject *parent) : QObject(parent)
{

}

DataCenter::~DataCenter()
{
    qDebug("DataCenter delete");
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
