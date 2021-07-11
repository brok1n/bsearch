#ifndef THREADPOOLTEST_H
#define THREADPOOLTEST_H

#include <QThread>
#include <QObject>
#include <QRunnable>

class TestRunnable : public QRunnable
{
public:
    TestRunnable(int);
    void run();

private:
    int mSleepTime;
};

class ThreadPoolTest : public QThread
{
    Q_OBJECT
public:
    ThreadPoolTest();

    void run();
};

#endif // THREADPOOLTEST_H
