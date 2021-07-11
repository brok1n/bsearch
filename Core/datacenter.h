#ifndef DATACENTER_H
#define DATACENTER_H

#include <QObject>
#include <QMutex>
#include <QFileInfo>
#include <QList>
#include <QThreadPool>
#include <QDir>
#include <QMap>

struct Node {
    QString name = "";
    QList<Node*> childs;
    bool isDir = false;
    Node *parent = nullptr;
    void addChild(Node* node)
    {
        childs.append(node);
        node->parent = this;
    }
    QString fullPath()
    {
        return eachParent(this).replace("//", "/");
    }
    QString eachParent(Node *p)
    {
        if(p->parent == nullptr)
        {
            return p->name;
        }
        else
        {
            return eachParent(p->parent) + "/" + p->name;
        }
    }
};

const int MAX_THREAD_COUNT = 100;

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
};

#endif // DATACENTER_H
