#ifndef DISKINDEXTHREAD_H
#define DISKINDEXTHREAD_H

#include <QThread>
#include <QObject>
#include "partitionindexthread.h"

class DiskIndexThread : public QThread
{
    Q_OBJECT
public:
    DiskIndexThread();

private:
    QList<PartitionIndexThread> partitionThreadList;
};

#endif // DISKINDEXTHREAD_H
