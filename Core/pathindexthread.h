#ifndef PATHINDEXTHREAD_H
#define PATHINDEXTHREAD_H

#include <QThread>
#include <QObject>

class PathIndexThread : public QThread
{
    Q_OBJECT
public:
    PathIndexThread();
};

#endif // PATHINDEXTHREAD_H
