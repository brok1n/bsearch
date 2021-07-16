#ifndef PARTITIONINDEXTHREAD_H
#define PARTITIONINDEXTHREAD_H

#include <QThread>
#include <QObject>
#include <QThreadPool>
#include "pathindexrunnable.h"

class PartitionIndexThread : public QThread
{
    Q_OBJECT
public:
    PartitionIndexThread(QString rootPath);
    ~PartitionIndexThread();

    void run();

    void stop();

    bool isWinSystemPartition(QString);

private:
    QList<PathIndexRunnable*> mPathThreadList;
    QString mRootPath;
    bool mRunning;
    QThreadPool *mThreadPool;
};

#endif // PARTITIONINDEXTHREAD_H
