#include "datacenter.h"
#include "indexmanager.h"
#include<QDebug>
#include <QTime>

IndexManager* IndexManager::mInstance = nullptr;
QMutex  IndexManager::mMutex;

IndexManager::IndexManager(QObject *parent)
    : QObject(parent)
    , mIndexThread(nullptr)
{

}

IndexManager *IndexManager::GetInstance()
{
    if (mInstance == nullptr)
    {
        QMutexLocker locker(&mMutex);
        if (mInstance == nullptr)
        {
            mInstance = new IndexManager();
        }
    }
    return mInstance;
}

void IndexManager::Release()
{
    qDebug() << "IndexManager::Release()";
    delete mInstance;
}

IndexManager::~IndexManager()
{
    mInstance->stop();
    delete  mIndexThread;
    qDebug("~IndexManager():end");
}

void IndexManager::start()
{
    mIndexThread = new IndexThread();
    mIndexThread->start();
}

void IndexManager::stop()
{
    qDebug() << "IndexManager:stop()";
    mIndexThread->stop();
    mIndexThread->wait();
    qDebug() << "IndexManager:stop():end";
}



