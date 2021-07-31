#ifndef COPYMOVEFILEDIALOG_H
#define COPYMOVEFILEDIALOG_H

#include <QDialog>

namespace Ui {
class CopyMoveFileDialog;
}

class CopyMoveFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CopyMoveFileDialog(QList<QString> files, QString targetPath, bool isMove = false, QWidget *parent = nullptr);
    ~CopyMoveFileDialog();

    //开始复制或移动
    void start();

    //获取传入的列表文件总文件大小
    quint64 getFileCountSize(QList<QString>);

    //将文件大小(byte)转换成本地显示格式 Kb  Mb  Gb ...
    QString localFileSize(quint64 size);

    //设置界面显示的源文件所在文件夹
    void setSourceDir(QString);
    //设置界面显示的目标文件夹
    void setTargetDir(QString);
    //设置复制文件个数
    void setProcessFileNumber(int number);

    //刷新标题
    void flushWindowTitle();

private slots:
    //开始处理，为了不阻塞start方法，这里使用了个定时器启动的新方法处理
    void startProcess();

    //处理完毕
    void onProcessFinished();

    //源文件所在目录按钮被点击
    void on_sourcePathBtn_clicked();
    //目标目录按钮被点击
    void on_targetPathBtn_clicked();

    void on_replaceBtn_clicked();

    void on_ignoreBtn_clicked();

    void on_diffBtn_clicked();

private:
    Ui::CopyMoveFileDialog *ui;
    QList<QString> mFiles;  //所有要处理的文件列表
    QString mTargetPath;    //复制/移动 到哪里
    bool mIsMove;   //是否为移动操作
    int mFileNumber; //文件个数
    quint64 mFileSumSize;   //文件总大小
    quint64 mFileProcessSize; //已处理大小
    QString mCurrentFilePath;   //当前在处理的文件路径
    bool mWaitUserSelection;    //是否等待用户选择
};

#endif // COPYMOVEFILEDIALOG_H
