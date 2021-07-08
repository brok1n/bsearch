#ifndef PARTITIONINDEXTHREAD_H
#define PARTITIONINDEXTHREAD_H

#include <QThread>
#include <QObject>

#include "pathindexthread.h"

class PartitionIndexThread : public QThread
{
    Q_OBJECT
public:
    PartitionIndexThread();


private:
    QList<PathIndexThread> pathThreadList();
};

#endif // PARTITIONINDEXTHREAD_H
