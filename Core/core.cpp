#include "core.h"
#include <QDebug>
#include <QThreadPool>
#include "datacenter.h"
#include "indexmanager.h"
#include "searchmanager.h"

Core* Core::mInstance = nullptr;
QMutex  Core::mMutex;

Core::Core(QObject *parent)
    : QObject(parent)
    , mSearchManager(nullptr)
{
    qDebug("Core 创建");

    if(init())
    {
        start();
    }
}

Core::~Core()
{
    for(int i = 0; i < mSearchThreadList.size(); i ++)
    {
        mSearchThreadList.at(i)->stop();
        mSearchThreadList.at(i)->wait();
        mSearchThreadList.at(i)->deleteLater();
    }
    if(mSearchManager != nullptr)
    {
        mSearchManager->stop();
        mSearchManager->wait();
        mSearchManager->deleteLater();
    }
    qDebug("~Core()");
}

bool Core::addControlPanel(qint64 panelId)
{
    if(mPanelIdList.contains(panelId))
    {
        return false;
    }
    mPanelIdList.append(panelId);
    return true;
}

void Core::releaseControlPanel(qint64 panelId)
{
    qDebug() << "release control panel:" << panelId;
    if(mPanelIdList.size() <= 1)
    {
        //控制面板为空，可以销毁了
        Core::Release();
    }
    mPanelIdList.removeOne(panelId);
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
    qDebug() << "Core::Release";
    DataCenter::Release();
    qDebug() << "Core::Release:DataCenter";
    IndexManager::Release();
    qDebug() << "Core::Release:IndexManager";
    delete mInstance;
}

void Core::search(QString key, int fileType)
{
    DataCenter::GetInstance()->setSearchFinished(false);
    if(mSearchManager != nullptr)
    {
        mSearchManager->stop();
        mSearchManager->wait();
        delete mSearchManager;
    }

    mSearchManager = new SearchManager(key, fileType);
    mSearchManager->start();
}

bool Core::init()
{
    return true;
}

bool Core::loadData()
{
    return false;
}

bool Core::saveData()
{
    return false;
}

void Core::start()
{
    IndexManager::GetInstance()->start();
}

void Core::stop()
{
    IndexManager::GetInstance()->stop();
    if(mSearchManager != nullptr)
    {
        mSearchManager->stop();
        mSearchManager->wait();
    }
}
