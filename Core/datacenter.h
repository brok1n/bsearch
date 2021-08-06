#ifndef DATACENTER_H
#define DATACENTER_H

#include <QObject>
#include <QMutex>
#include <QFileInfo>
#include <QList>
#include <QThreadPool>
#include <QDir>
#include <QMap>

#include "common.h"

class DataCenter : public QObject
{
    Q_OBJECT
private:
    explicit DataCenter(QObject *parent = nullptr);

public:
    static DataCenter* GetInstance();
    static void Release();
    ~DataCenter();

    QMap<QString, Node*>* fileTree();
    QList<Node*>* resultList();

    bool isSearchFinished();
    void setSearchFinished(bool);

    bool isScanDiskFinished();
    void setScanDiskFinished(bool);

    int partitionCount();
    void setPartitionCount(int);

    int scanFinishedCount();
    void setScanFinishedCount(int);

    int singleThreadCount();
    void setSingleThreadCount(int);

    bool isRunning();
    void setRunning(bool);

    void printNode(Node*, int);

    int sortType();
    void setSortType(int);

    int sortOrder();
    void setSortOrder(int);

signals:

private:
    static DataCenter *mInstance;
    static QMutex  mMutex;

    //所有分区文件Node列表，分区跟路径 => 跟节点Node
    QMap<QString, Node*> *mTree;
    //搜索结果列表
    QList<Node*> *mResultList;
    //是否搜索完毕
    bool mSearchFinished;
    //总分区数量
    int mPartitionCount;
    //分区扫描完毕数量
    int mScanFinishedCount;
    //单分区扫描线程数
    int mSingleThreadCount;
    //磁盘扫描是否完毕
    bool mScanDiskFinished;
    //运行状态
    bool mIsRunning;
    //排序类型
    int mSortType;
    //排序方式
    int mSortOrder;

};

#endif // DATACENTER_H
