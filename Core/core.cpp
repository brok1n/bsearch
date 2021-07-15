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
        delete mSearchThreadList.at(i);
    }
    qDebug("Core delete");
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
    mPanelIdList.removeOne(panelId);
    if(mPanelIdList.isEmpty())
    {
        //控制面板为空，可以销毁了
        Core::Release();
    }
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

//    QList<QString> keys = DataCenter::GetInstance()->fileTree()->keys();
//    for(int i = 0; i < keys.size(); i ++)
//    {
//        Node *node = DataCenter::GetInstance()->fileTree()->value(keys.at(i));
//        SearchThread *searchThread = new SearchThread(node, key);
//        mSearchThreadList.append(searchThread);
//        searchThread->start();
//    }
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
    QThreadPool::globalInstance()->setMaxThreadCount(200);
    IndexManager::GetInstance()->start();
//    QElapsedTimer timer;
//    Node *node = new Node();
//    QFileInfo info("E:\\workspace");
//    node->name = info.fileName();
//    PathIndexThread *pathIndexThread = new PathIndexThread(info, node, 0);
//    pathIndexThread->start();
//    pathIndexThread->wait();
//    qint64 useTime = timer.elapsed();
//    pathIndexThread->printNode(node, 1);
//    qDebug() << "use time:" << useTime;
}

bool Core::stop()
{

}
