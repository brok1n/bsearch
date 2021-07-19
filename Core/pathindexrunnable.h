#ifndef PATHINDEXRUNNABLE_H
#define PATHINDEXRUNNABLE_H

#include <QThread>
//#include <QObject>
#include <QFileInfo>
#include <QRunnable>

#include "datacenter.h"

class PathIndexRunnable : public QRunnable
{
//    Q_OBJECT
public:
    PathIndexRunnable(QFileInfo info, Node *, int, QThreadPool*);
    ~PathIndexRunnable();
    void run();

    void eachDir(QFileInfo, Node*);

    void printNode(Node*, int);

private:
    QFileInfo mInfo;
    Node *mRootNode;
    int mLevel;
//    QList<PathIndexRunnable*> mPathIndexRunnableList;
    QThreadPool *mThreadPool;

};

#endif // PATHINDEXRUNNABLE_H
