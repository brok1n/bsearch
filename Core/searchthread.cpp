#include "searchthread.h"
#include <QDebug>

SearchThread::SearchThread(Node *node, QString key, int fileType)
    : mNode(node)
    , mKey(key)
    , mRunning(false)
    , mFileType(fileType)
{
    //是否忽略大小写处理
    if(DataCenter::GetInstance()->ignoreCase())
    {
        mCaseSensitivity = Qt::CaseInsensitive;
    }
    else
    {
        mCaseSensitivity = Qt::CaseSensitive;
    }
}

SearchThread::~SearchThread()
{
    mNode = nullptr;
    qDebug() << "~SearchThread()";
}

void SearchThread::run()
{
    if(mNode->childs.isEmpty())
    {
        return;
    }
    mRunning = true;
    qDebug() << "SearchThread " << mNode->name << "  " << mKey << ":" << mFileType << " start.";
    QList<Node*> *resultList = DataCenter::GetInstance()->resultList();
    eachNode(mNode, resultList, 0);
    qDebug() << "SearchThread " << mNode->name << "  " << mKey << ":" << mFileType << " end.";
}

void SearchThread::eachNode(Node *node, QList<Node*> *resultList, int level)
{
    if(!mRunning)
    {
        return;
    }
    for(int i = 0; i < node->childs.size(); i ++)
    {
        if(!mRunning)
        {
            break;
        }
        Node *n = node->childs.at(i);
//        if(n->fileExt().contains(mKey) || n->name.contains(mKey))
        if(n->name.contains(mKey, mCaseSensitivity))
        {
            if(mFileType == FILE_TYPE::FILE_ALL || mFileType == n->fileType())
            {
                resultList->append(n);
            }
        }
        if(!n->childs.isEmpty())
        {
            eachNode(n, resultList, level+1);
        }
    }
}

void SearchThread::stop()
{
    mRunning = false;
}
