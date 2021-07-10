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

    delete mInstance;
}

IndexManager::~IndexManager()
{

    qDebug("IndexManager delete");
}

bool IndexManager::start()
{

    mIndexThread = new IndexThread();
    mIndexThread->start();
//    mIndexThread->wait();


//    QList<QString> keys = DataCenter::GetInstance()->fileTree()->keys();
//    for(int i = 0; i < keys.size(); i ++)
//    {
//        Node *node = DataCenter::GetInstance()->fileTree()->value(keys.at(i));

//        DataCenter::GetInstance()->printNode(node, 0);
//    }

}



