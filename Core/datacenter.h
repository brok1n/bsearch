#ifndef DATACENTER_H
#define DATACENTER_H

#include <QObject>
#include <QMutex>

class DataCenter : public QObject
{
    Q_OBJECT
private:
    explicit DataCenter(QObject *parent = nullptr);

public:
    static DataCenter* GetInstance();
    static void Release();
    ~DataCenter();


signals:

private:
    static DataCenter *mInstance;
    static QMutex  mMutex;
};

#endif // DATACENTER_H
