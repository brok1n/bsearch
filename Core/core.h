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
    bool init();
    void start();
    void stop();

public:
    static Core* GetInstance();
    static void Release();
    ~Core();

public:
    bool addControlPanel(qint64 panelId);   //添加控制窗口
    bool releaseControlPanel(qint64 panelId);   //释放控制窗口  如果释放了全部窗口，则结束Core，并返回true，否则返回false
    int  controlPanelCount();   //控制窗口个数
    void search(QString, int fileType = FILE_TYPE::FILE_ALL);


    bool loadData();
    bool saveData();



private:
    static Core *mInstance;
    static QMutex  mMutex;

    QList<SearchThread*> mSearchThreadList; //搜索线程列表
    SearchManager *mSearchManager;  //搜索管理器
    QList<qint64> mPanelIdList; //控制窗口ID列表
};

#endif // CORE_H
