#ifndef CORE_H
#define CORE_H

//#include "Core_global.h"
#include <QObject>
#include <QMutex>

//class CORE_EXPORT Core
class Core : public QObject
{
    Q_OBJECT
private:
    explicit Core(QObject *parent = nullptr);

public:
    static Core* GetInstance();
    static void Release();
    ~Core();

public:
    int add(int a, int b);

    bool init();
    bool loadData();
    bool saveData();

    bool start();
    bool stop();

private:
    static Core *mInstance;
    static QMutex  mMutex;
};

#endif // CORE_H
