#ifndef INDEXTHREAD_H
#define INDEXTHREAD_H

#include <QThread>
#include <QObject>

#include "partitionindexthread.h"

class IndexThread : public QThread
{
    Q_OBJECT
public:
    IndexThread();
    ~IndexThread();

    void run();

    void stop();

private:
    QList<PartitionIndexThread*> mPartitionThreadList;
};

#endif // INDEXTHREAD_H
