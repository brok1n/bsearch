#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>

class Preferences : public QObject
{
    Q_OBJECT
public:
    explicit Preferences(QObject *parent = nullptr);

signals:

};

#endif // PREFERENCES_H
