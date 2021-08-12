#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTimer>
#include <QListWidgetItem>
#include "datacenter.h"
#include "aboutdialog.h"
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QSslError>
#include <QMenu>
#include <QLabel>

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

    void onListWidgetItemClicked(QListWidgetItem*);

    void on_filterCBox_currentIndexChanged(int index);

    void on_actionAll_toggled(bool arg1);

    void on_actionMusic_toggled(bool arg1);

    void on_actionVideo_toggled(bool arg1);

    void on_actionPicture_toggled(bool arg1);

    void on_actionDocument_toggled(bool arg1);

    void on_actionCompress_toggled(bool arg1);

    void on_actionExecutable_toggled(bool arg1);

    void on_actionDir_toggled(bool arg1);

    void on_actionTxtContentSearch_toggled(bool arg1);

    void on_actionCreateWindow_triggered();

    void on_actionClose_triggered();

    void on_actionQuit_triggered();

    void on_actionFixTop_toggled(bool arg1);

    void on_actionSelectAll_triggered();

    void on_actionReverseSelect_triggered();

    void on_actionDefaultSize_triggered();

    void on_actionMidSize_triggered();

    void on_actionBigSize_triggered();

    void on_actionFlush_triggered();

    void on_actionOpenInFolder_triggered();

    void on_actionOpenFile_triggered();

    void on_actionCopyTo_triggered();

    void on_actionMoveTo_triggered();

    void on_actionCopyToDesktop_triggered();

    void on_actionSortName_triggered();

    void on_actionSortPath_triggered();

    void on_actionSortSize_triggered();

    void on_actionSortType_triggered();

    void on_actionSortExt_triggered();

    void on_actionSortModifyTime_triggered();

    void on_actionSortCreateTime_triggered();

    void on_actionSortAsc_triggered();

    void on_actionSortDesc_triggered();

    void on_actionMatchCase_triggered();

    void on_actionAboutBSearch_triggered();

    void on_actionCheckUpdate_triggered();

    void httpFinished();
    void httpReadyRead();
    void sslErrors(QNetworkReply *, const QList<QSslError> &errors);
    void error(QNetworkReply::NetworkError);

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void timeUpdate();

public:

    void flushResult();

    void startSearch();

    void showMsg(QString);

private:
    Ui::MainWindow *ui;
    QTimer *mWaitResultTimer;
    QTimer *mWaitScanDiskTimer;
    QTimer *mTimeTimer;
    qint64 mPanelId;
    QSize mDefaultIconSize;
    AboutDialog *mAboutDialog;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QMenu *mContextMenu;
    QLabel *mStatusMsgLabel;
    QLabel *mStatusTimeLabel;
    QLabel *mStatusFileCountLabel;

};
#endif // MAINWINDOW_H
