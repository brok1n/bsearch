#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>
#include <QObject>
#include "datacenter.h"

class SearchThread : public QThread
{
    Q_OBJECT
public:
    SearchThread(Node*, QString);
    ~SearchThread();

    void run();

    void eachNode(Node*, int);

    void stop();

private:
    Node *mNode;
    QString mKey;
    bool mRunning;
};

#endif // SEARCHTHREAD_H
