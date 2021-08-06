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


//    QElapsedTimer mergeTimer;
//    mergeTimer.start();

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

//    qint64 mergeUseTime = mergeTimer.elapsed();
//    qDebug() << "排序完毕:" << " 耗时：" << (mergeUseTime / 1000) << "秒" << (mergeUseTime % 1000) << "毫秒";

//    switch (DataCenter::GetInstance()->sortOrder())
//    {
//    case SORT_ORDER::SORT_ASC:
//        排序符号 = "<";
//        break;
//    case SORT_ORDER::SORT_DESC:
//        排序符号 = ">";
//        break;
//    }

//    switch (DataCenter::GetInstance()->sortType())
//    {
//    case SORT_TYPE::BY_NAME:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->name 排序符号 node2->name;
//        });
//        break;
//    }


//    排序符号 = "";
//    switch (DataCenter::GetInstance()->sortOrder())
//    {
//    case SORT_ORDER::SORT_ASC:
//        排序符号 = "<";
//        break;
//    case SORT_ORDER::SORT_DESC:
//        排序符号 = ">";
//        break;
//    }

//    switch (DataCenter::GetInstance()->sortType())
//    {
//    case SORT_TYPE::BY_NAME:
//        qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//            return node1->name 排序符号 node2->name;
//        });
//        break;
//    }


//    switch (DataCenter::GetInstance()->sortType())
//    {
//    case SORT_TYPE::BY_NAME:
//        switch (DataCenter::GetInstance()->sortOrder())
//        {
//        case SORT_ORDER::SORT_ASC:
//            qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//                return node1->name < node2->name;
//            });
//        case SORT_ORDER::SORT_DESC:
//            qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) -> bool {
//                return node1->name > node2->name;
//            });
//        }
//        break;
//    case SORT_TYPE::BY_SIZE:
//        sortKey1 = QString::number(node1->_fileSize);
//        sortKey2 = QString::number(node2->_fileSize);
//        break;
//    case SORT_TYPE::BY_PATH:
//        sortKey1 = node1->_fullPath;
//        sortKey2 = node2->_fullPath;
//        break;
//    case SORT_TYPE::BY_TYPE:
//        sortKey1 = QString::number(node1->_fileType);
//        sortKey2 = QString::number(node2->_fileType);
//        break;
//    case SORT_TYPE::BY_EXT:
//        sortKey1 = node1->_ext;
//        sortKey2 = node2->_ext;
//        break;
//    case SORT_TYPE::BY_CTIME:
//        sortKey1 = QString::number(node1->createTime);
//        sortKey2 = QString::number(node2->createTime);
//        break;
//    case SORT_TYPE::BY_MTIME:
//        sortKey1 = QString::number(node1->modifyTime);
//        sortKey2 = QString::number(node2->modifyTime);
//        break;
//    }

//    qSort(resultList->begin(), resultList->end(), [=](const Node *node1, const Node *node2) ->bool {
//        QString sortKey1 = "";
//        QString sortKey2 = "";
//        switch (DataCenter::GetInstance()->sortType()) {
//        case SORT_TYPE::BY_NAME:
//            sortKey1 = node1->name;
//            sortKey2 = node2->name;
//            break;
//        case SORT_TYPE::BY_SIZE:
//            sortKey1 = QString::number(node1->_fileSize);
//            sortKey2 = QString::number(node2->_fileSize);
//            break;
//        case SORT_TYPE::BY_PATH:
//            sortKey1 = node1->_fullPath;
//            sortKey2 = node2->_fullPath;
//            break;
//        case SORT_TYPE::BY_TYPE:
//            sortKey1 = QString::number(node1->_fileType);
//            sortKey2 = QString::number(node2->_fileType);
//            break;
//        case SORT_TYPE::BY_EXT:
//            sortKey1 = node1->_ext;
//            sortKey2 = node2->_ext;
//            break;
//        case SORT_TYPE::BY_CTIME:
//            sortKey1 = QString::number(node1->createTime);
//            sortKey2 = QString::number(node2->createTime);
//            break;
//        case SORT_TYPE::BY_MTIME:
//            sortKey1 = QString::number(node1->modifyTime);
//            sortKey2 = QString::number(node2->modifyTime);
//            break;
//        }

//        switch (DataCenter::GetInstance()->sortOrder()) {
//        case SORT_ORDER::SORT_ASC:
//            return sortKey1 < sortKey2;
//        case SORT_ORDER::SORT_DESC:
//            return sortKey1 > sortKey2;
//        }
//        return false;
//    });

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
