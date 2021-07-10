#include "datacenter.h"
#include "searchthread.h"
#include <QDebug>

SearchThread::SearchThread(Node *node, QString key)
    : mNode(node)
    , mKey(key)
    , mRunning(false)
{

}

SearchThread::~SearchThread()
{

}

void SearchThread::run()
{
    if(mNode->childs.isEmpty())
    {
        return;
    }
    mRunning = true;
    qDebug() << "SearchThread " << mNode->name << " start.";
    eachNode(mNode, 0);
    qDebug() << "SearchThread " << mNode->name << " end.";
}

void SearchThread::eachNode(Node *node, int level)
{
    if(!mRunning)
    {
        return;
    }
    for(int i = 0; i < node->childs.size(); i ++)
    {
        Node *n = node->childs.at(i);
        if(n->name.contains(mKey))
        {
            qDebug() << n->name;
//            DataCenter::GetInstance()->resultList()->append(n);
        }
        if(!mRunning)
        {
            return;
        }
        eachNode(n, level+1);
    }
}

void SearchThread::stop()
{
    mRunning = false;
}
