#ifndef DATACENTER_H
#define DATACENTER_H

#include <QObject>
#include <QMutex>
#include <QFileInfo>
#include <QList>
#include <QThreadPool>

struct Node {
    QString name;
    QList<Node*> childs;
    Node *parent;
    void addChild(Node* node)
    {
        childs.append(node);
        node->parent = this;
    }
};

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
    QThreadPool* threadPool();

    void printNode(Node*, int);

signals:

private:
    static DataCenter *mInstance;
    static QMutex  mMutex;
//    QList<QFileInfo> *mFileList;
//    QList<QString> *mFilePathList;

    QMap<QString, Node*> *mTree;
    QThreadPool *mPool;

};

#endif // DATACENTER_H
