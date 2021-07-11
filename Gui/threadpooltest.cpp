#include "threadpooltest.h"

#include <QThread>
#include <QThreadPool>
#include <QDebug>

ThreadPoolTest::ThreadPoolTest()
{

}

void ThreadPoolTest::run()
{
    QThreadPool pool;
    pool.setMaxThreadCount(2);

//    pool.start(new TestRunnable(100));
//    qDebug() << "添加了一个100秒的任务";
//    pool.start(new TestRunnable(5));
//    qDebug() << "添加了一个5秒的任务";
//    pool.start(new TestRunnable(3));
//    qDebug() << "添加了一个3秒的任务";
////    pool.setMaxThreadCount(100);
//    for(int i = 0; i < 100; i ++)
//    {
//        pool.start(new TestRunnable(50));
//        qDebug() << "添加了一个任务";
//    }

//    qDebug() << "任务添加完毕，开始休眠";
//    for(int i = 2; i < 10; i ++)
//    {
//        QThread::sleep(i);
//        pool.setMaxThreadCount(i);
//        qDebug() << "更新了最大任务数:" << i;
//    }
//    qDebug() << "休眠完毕";


    pool.waitForDone();

}

TestRunnable::TestRunnable(int t)
    : mSleepTime(t)
{

}

void TestRunnable::run()
{
    qDebug() << "开始执行任务";
    QThread::sleep(mSleepTime);
    qDebug() << "任务结束";
}
