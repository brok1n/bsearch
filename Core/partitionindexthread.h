#ifndef PARTITIONINDEXTHREAD_H
#define PARTITIONINDEXTHREAD_H

#include <QThread>
#include <QObject>

#include "pathindexthread.h"

class PartitionIndexThread : public QThread
{
    Q_OBJECT
public:
    PartitionIndexThread(QString rootPath);

    void run();

    void stop();

    bool isWinSystemPartition(QString);

private:
    QList<PathIndexThread*> mPathThreadList;
    QString mRootPath;
    bool mRunning;
};

#endif // PARTITIONINDEXTHREAD_H
