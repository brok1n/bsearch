#ifndef INDEXMANAGER_H
#define INDEXMANAGER_H

#include <QObject>
#include <QObject>
#include <QMutex>
#include <QList>

#include "diskindexthread.h"

class IndexManager : QObject
{
    Q_OBJECT
private:
    IndexManager(QObject *parent = nullptr);
public:
    static IndexManager* GetInstance();
    static void Release();
    ~IndexManager();

    bool start();

private:
    static IndexManager *mInstance;
    static QMutex  mMutex;

    QList<DiskIndexThread> diskThreadList;
};

#endif // INDEXMANAGER_H
