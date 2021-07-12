#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <QThread>
#include <QObject>
#include "common.h"
#include "searchthread.h"

class SearchManager : public QThread
{
    Q_OBJECT
public:
    SearchManager(QString, int fileType = FILE_TYPE::FILE_ALL);
    ~SearchManager();

    void run();
    void stop();

private:
    QString mKey;
    int mFileType;
    QList<SearchThread*> mThreadList;
};

#endif // SEARCHMANAGER_H
