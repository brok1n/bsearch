#ifndef INDEXMANAGER_H
#define INDEXMANAGER_H

#include "indexthread.h"

#include <QObject>
#include <QObject>
#include <QMutex>
#include <QList>

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
    IndexThread* mIndexThread;

};

#endif // INDEXMANAGER_H
