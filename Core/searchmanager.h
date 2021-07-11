#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include "searchthread.h"

#include <QThread>
#include <QObject>

class SearchManager : public QThread
{
    Q_OBJECT
public:
    SearchManager(QString);
    ~SearchManager();

    void run();
    void stop();

private:
    QString mKey;
    QList<SearchThread*> mThreadList;
};

#endif // SEARCHMANAGER_H
