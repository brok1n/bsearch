#include "core.h"
#include<QDebug>
#include "datacenter.h"
#include "indexmanager.h"

Core* Core::mInstance = nullptr;
QMutex  Core::mMutex;

Core::Core(QObject *parent) : QObject(parent)
{
    qDebug("Core 创建");

    if(init())
    {
        start();
    }
}

Core::~Core()
{
    qDebug("Core delete");
}

Core *Core::GetInstance()
{
    if (mInstance == nullptr)
    {
        QMutexLocker locker(&mMutex);
        if (mInstance == nullptr)
        {
            mInstance = new Core();
        }
    }
    return mInstance;
}

void Core::Release()
{
    DataCenter::Release();
    IndexManager::Release();
    delete mInstance;
}

int Core::add(int a, int b)
{
    qDebug("add ");
    return a + b;
}

bool Core::init()
{
    return true;
}

bool Core::loadData()
{

}

bool Core::saveData()
{

}

bool Core::start()
{
    IndexManager::GetInstance()->start();
}

bool Core::stop()
{

}
