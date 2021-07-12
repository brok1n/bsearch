#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTimer>
#include "datacenter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private slots:
    void on_keyEdit_returnPressed();

    void onSearchFinished();

    void onScanDiskFinished();

    void on_filterCBox_currentIndexChanged(int index);

public:

    void flushResult();

    void startSearch();

private:
    Ui::MainWindow *ui;
    QTimer *mWaitResultTimer;
    QTimer *mWaitScanDiskTimer;

};
#endif // MAINWINDOW_H
