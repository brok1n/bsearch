#ifndef PATHINDEXTHREAD_H
#define PATHINDEXTHREAD_H

#include <QThread>
#include <QObject>
#include <QFileInfo>

#include "datacenter.h"

class PathIndexThread : public QThread
{
    Q_OBJECT
public:
    PathIndexThread(QFileInfo info, Node *, int);
    ~PathIndexThread();
    void run();

    void stop();
    void eachDir(QFileInfo, Node *);

private:
    QFileInfo mInfo;
    bool mRunning;
    Node *mRootNode;
    int mLevel;
};

#endif // PATHINDEXTHREAD_H
