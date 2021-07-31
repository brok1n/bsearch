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

    void start();

    quint64 getFileCountSize(QList<QString>);

    QString localFileSize(quint64 size);

private slots:
    void startProcess();


private:
    Ui::CopyMoveFileDialog *ui;
    QList<QString> mFiles;  //所有要处理的文件列表
    QString mTargetPath;    //复制/移动 到哪里
    bool mIsMove;   //是否为移动操作
    int mFileNumber; //文件个数
    quint64 mFileSumSize;   //文件总大小
    quint64 mFileRemainSumSize; //剩余总大小
};

#endif // COPYMOVEFILEDIALOG_H
