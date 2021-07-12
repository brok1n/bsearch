#ifndef CORE_H
#define CORE_H

//#include "Core_global.h"


#include <QObject>
#include <QMutex>
#include <functional>
#include "common.h"

class SearchThread;
class SearchManager;
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
    void search(QString, int fileType = FILE_TYPE::FILE_ALL);

    bool init();
    bool loadData();
    bool saveData();

    bool start();
    bool stop();

private:
    static Core *mInstance;
    static QMutex  mMutex;

    QList<SearchThread*> mSearchThreadList;
    SearchManager *mSearchManager;
};

#endif // CORE_H
