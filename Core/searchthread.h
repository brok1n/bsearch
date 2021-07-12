#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>
#include <QObject>
#include "datacenter.h"
#include <functional>

class SearchThread : public QThread
{
    Q_OBJECT
public:
    SearchThread(Node*, QString, int fileType = FILE_TYPE::FILE_ALL);
    ~SearchThread();

    void run();

    void eachNode(Node*, QList<Node*>*, int);

    void stop();

private:
    Node *mNode;
    QString mKey;
    bool mRunning;
    int mFileType;
};

#endif // SEARCHTHREAD_H
