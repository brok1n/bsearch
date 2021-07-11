#include "datacenter.h"
#include "searchmanager.h"

SearchManager::SearchManager(QString key)
    : mKey(key)
{

}

SearchManager::~SearchManager()
{

}

void SearchManager::run()
{
    DataCenter::GetInstance()->setSearchFinished(false);
    QList<QString> keys = DataCenter::GetInstance()->fileTree()->keys();
    for(int i = 0; i < keys.size(); i ++)
    {
        Node *node = DataCenter::GetInstance()->fileTree()->value(keys.at(i));
        SearchThread *searchThread = new SearchThread(node, mKey);
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
