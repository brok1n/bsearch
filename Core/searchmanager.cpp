#include "datacenter.h"
#include "searchmanager.h"
#include <QDebug>

SearchManager::SearchManager(QString key, int fileType)
    : mKey(key)
    , mFileType(fileType)
{

}

SearchManager::~SearchManager()
{
    for(int i = 0; i < mThreadList.size(); i ++)
    {
        SearchThread *thread = mThreadList.at(i);
        thread->stop();
        thread->wait();
        thread->deleteLater();
    }
    qDebug() << "~SearchManager()";
}

void SearchManager::run()
{
    qDebug() << "search:" << mKey << "  type:" << FILE_TYPE_NAME[mFileType];

    DataCenter::GetInstance()->setSearchFinished(false);
    DataCenter::GetInstance()->resultList()->clear();
    QList<QString> keys = DataCenter::GetInstance()->fileTree()->keys();
    for(int i = 0; i < keys.size(); i ++)
    {
        Node *node = DataCenter::GetInstance()->fileTree()->value(keys.at(i));
        SearchThread *searchThread = new SearchThread(node, mKey, mFileType);
        mThreadList.append(searchThread);
        searchThread->start();
    }

    for(int i = 0; i < mThreadList.size(); i ++)
    {
        SearchThread *thread = mThreadList.at(i);
        thread->wait();
    }

    DataCenter::GetInstance()->setSearchFinished(true);
}

void SearchManager::stop()
{
    for(int i = 0; i < mThreadList.size(); i ++)
    {
        SearchThread *thread = mThreadList.at(i);
        thread->stop();
    }
}
