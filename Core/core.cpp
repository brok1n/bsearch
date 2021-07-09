#include "core.h"
#include <QDebug>
#include <QThreadPool>
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
