#ifndef PATHINDEXTHREAD_H
#define PATHINDEXTHREAD_H

#include <QThread>
//#include <QObject>
#include <QFileInfo>
#include <QRunnable>

#include "datacenter.h"

//class PathIndexThread : public QThread
class PathIndexThread : public QRunnable
{
//    Q_OBJECT
public:
    PathIndexThread(QFileInfo info, Node *, int, QThreadPool*);
    ~PathIndexThread();
    void run();

    void stop();
    void eachDir(QFileInfo, Node*);

    void printNode(Node*, int);

private:
    QFileInfo mInfo;
    bool mRunning;
    Node *mRootNode;
    int mLevel;
    QList<PathIndexThread*> mPathIndexThreadList;
    QThreadPool *mThreadPool;

};

#endif // PATHINDEXTHREAD_H
