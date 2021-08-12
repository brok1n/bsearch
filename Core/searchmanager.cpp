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
    QElapsedTimer searchTimer;
    searchTimer.start();

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

    //搜索完毕，排序
    QList<Node*> *resultList = DataCenter::GetInstance()->resultList();

    QElapsedTimer sequenceTimer;
    sequenceTimer.start();

    bool sequence;
    switch (DataCenter::GetInstance()->sortOrder())
    {
    case SORT_ORDER::SORT_ASC:
        sequence = true;
        break;
    case SORT_ORDER::SORT_DESC:
        sequence = false;
        break;
    }

    switch (DataCenter::GetInstance()->sortType())
    {
    case SORT_TYPE::BY_NAME:
        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
            return sequence ? node1->name < node2->name : node1->name > node2->name;
        });
        break;
    case SORT_TYPE::BY_SIZE:
        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
            return sequence ? node1->_fileSize < node2->_fileSize : node1->_fileSize > node2->_fileSize;
        });
        break;
    case SORT_TYPE::BY_PATH:
        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
            return sequence ? node1->_fullPath < node2->_fullPath : node1->_fullPath > node2->_fullPath;
        });
        break;
    case SORT_TYPE::BY_TYPE:
        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
            return sequence ? node1->_fileType < node2->_fileType : node1->_fileType > node2->_fileType;
        });
        break;
    case SORT_TYPE::BY_EXT:
        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
            return sequence ? node1->_ext < node2->_ext : node1->_ext > node2->_ext;
        });
        break;
    case SORT_TYPE::BY_CTIME:
        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
            return sequence ? node1->createTime < node2->createTime : node1->createTime > node2->createTime;
        });
        break;
    case SORT_TYPE::BY_MTIME:
        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
            return sequence ? node1->modifyTime < node2->modifyTime : node1->modifyTime > node2->modifyTime;
        });
        break;
    }

    qint64 seqTime = sequenceTimer.elapsed();
    qDebug() << "排序完毕:" << " 耗时：" << (seqTime / 1000) << "秒" << (seqTime % 1000) << "毫秒";

//    switch (DataCenter::GetInstance()->sortType() | DataCenter::GetInstance()->sortOrder())
//    {
//    case SORT_TYPE::BY_NAME | SORT_ORDER::SORT_ASC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->name < node2->name;
//        });
//        break;
//    case SORT_TYPE::BY_NAME | SORT_ORDER::SORT_DESC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->name > node2->name;
//        });
//        break;
//    case SORT_TYPE::BY_SIZE | SORT_ORDER::SORT_ASC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->_fileSize < node2->_fileSize;
//        });
//        break;
//    case SORT_TYPE::BY_SIZE | SORT_ORDER::SORT_DESC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->_fileSize > node2->_fileSize;
//        });
//        break;
//    case SORT_TYPE::BY_PATH | SORT_ORDER::SORT_ASC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->_fullPath < node2->_fullPath;
//        });
//        break;
//    case SORT_TYPE::BY_PATH | SORT_ORDER::SORT_DESC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->_fullPath > node2->_fullPath;
//        });
//        break;
//    case SORT_TYPE::BY_TYPE | SORT_ORDER::SORT_ASC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->_fileType < node2->_fileType;
//        });
//        break;
//    case SORT_TYPE::BY_TYPE | SORT_ORDER::SORT_DESC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->_fileType > node2->_fileType;
//        });
//        break;
//    case SORT_TYPE::BY_EXT | SORT_ORDER::SORT_ASC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->_ext < node2->_ext;
//        });
//        break;
//    case SORT_TYPE::BY_EXT | SORT_ORDER::SORT_DESC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->_ext > node2->_ext;
//        });
//        break;
//    case SORT_TYPE::BY_CTIME | SORT_ORDER::SORT_ASC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->createTime < node2->createTime;
//        });
//        break;
//    case SORT_TYPE::BY_CTIME | SORT_ORDER::SORT_DESC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->createTime > node2->createTime;
//        });
//        break;
//    case SORT_TYPE::BY_MTIME | SORT_ORDER::SORT_ASC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->modifyTime < node2->modifyTime;
//        });
//        break;
//    case SORT_TYPE::BY_MTIME | SORT_ORDER::SORT_DESC:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->modifyTime > node2->modifyTime;
//        });
//        break;
//    }

    DataCenter::GetInstance()->setSearchFinished(true);

    qint64 searchTime = searchTimer.elapsed();
    qDebug() << "搜索完毕! 耗时：" << (searchTime / 1000) << "秒" << (searchTime % 1000) << "毫秒";

}

void SearchManager::stop()
{
    for(int i = 0; i < mThreadList.size(); i ++)
    {
        SearchThread *thread = mThreadList.at(i);
        thread->stop();
    }
}
