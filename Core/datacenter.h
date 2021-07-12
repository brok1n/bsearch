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

//    QList<QFileInfo>* fileList();
//    QList<QString>* filePathList();
    QMap<QString, Node*>* fileTree();
//    QThreadPool* threadPool();
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

    void printNode(Node*, int);

signals:

private:
    static DataCenter *mInstance;
    static QMutex  mMutex;
//    QList<QFileInfo> *mFileList;
//    QList<QString> *mFilePathList;

    QMap<QString, Node*> *mTree;
    QThreadPool *mPool;
    QList<Node*> *mResultList;
    bool mSearchFinished;
    int mPartitionCount;
    int mScanFinishedCount;
    int mSingleThreadCount;
    bool mScanDiskFinished;
};

#endif // DATACENTER_H
