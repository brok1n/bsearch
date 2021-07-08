#include "indexmanager.h"
#include<QDebug>
#include <QStorageInfo>
#include <QTime>

IndexManager* IndexManager::mInstance = nullptr;
QMutex  IndexManager::mMutex;

IndexManager::IndexManager(QObject *parent) : QObject(parent)
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
    QElapsedTimer elapsedTimer;
    elapsedTimer.start();
    QList<QStorageInfo> list = QStorageInfo::mountedVolumes();

    int count = list.size();
    QString strInfo = "";
    for(int i = 0; i < count; ++i)
    {
        QStorageInfo diskInfo = list.at(i);
        qint64 freeSize = diskInfo.bytesFree();
        qint64 totalSize = diskInfo.bytesTotal();
//        qDebug(diskInfo.displayName().toLatin1());
        qDebug() << "磁盘：" << diskInfo.name() << " " << diskInfo.rootPath();

    }

    qDebug()<<"代码程序运行时间：" << elapsedTimer.elapsed();

}
