#include "searchthread.h"
#include <QDebug>

SearchThread::SearchThread(Node *node, QString key, int fileType)
    : mNode(node)
    , mKey(key)
    , mRunning(false)
    , mFileType(fileType)
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
    QList<Node*> *resultList = DataCenter::GetInstance()->resultList();
    eachNode(mNode, resultList, 0);
    qDebug() << "SearchThread " << mNode->name << " end.";
}

void SearchThread::eachNode(Node *node, QList<Node*> *resultList, int level)
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
            if(mFileType == n->fileType())
            {
                resultList->append(n);
            }
//            qDebug() << n->fullPath();
//            mResultCallback(n);
//            DataCenter::GetInstance()->resultList()->append(n);
        }
        if(!mRunning)
        {
            return;
        }
        eachNode(n, resultList, level+1);
    }
}

void SearchThread::stop()
{
    mRunning = false;
}
