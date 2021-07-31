#include "copymovefiledialog.h"
#include "ui_copymovefiledialog.h"

#include <QFileInfo>
#include <QTimer>
#include <QDebug>
#include <QLocale>

CopyMoveFileDialog::CopyMoveFileDialog(QList<QString> files, QString targetPath, bool isMove, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CopyMoveFileDialog)
    , mFiles(files)
    , mTargetPath(targetPath)
    , mIsMove(isMove)
    , mFileNumber(files.size())
    , mFileSumSize(getFileCountSize(mFiles))
    , mFileRemainSumSize(mFileSumSize)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags()|Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);

    if(mIsMove)
    {
        setWindowTitle("正在移动...");
    }
    else
    {
        setWindowTitle("正在复制...");
    }

    this->ui->fileCountLabel->setText(QString::number(mFileNumber));

//    if(mFileNumber > 0)
//    {
//        QFileInfo firstFileInfo(mFiles.at(0));
//        QFileInfo firstFileDirInfo(firstFileInfo.absolutePath());
//        this->ui->sourcePathBtn->setText(firstFileDirInfo.fileName());

//        this->ui->currentFileNameLabel->setText(firstFileInfo.fileName());

//        this->ui->remainItemsLabel->setText(QString("%1 (%2)").arg(mFileNumber).arg(mFileSumSize));
//    }


    QFileInfo targetPathInfo(mTargetPath);
    this->ui->targetPathBtn->setText(targetPathInfo.fileName());

    this->ui->progressBar->setMinimum(0);
    this->ui->progressBar->setMaximum(100);
    this->ui->progressBar->setValue(0);



}

CopyMoveFileDialog::~CopyMoveFileDialog()
{
    delete ui;
}

void CopyMoveFileDialog::start()
{
    this->show();
    QTimer::singleShot(500, this, SLOT(startProcess()));
}

quint64 CopyMoveFileDialog::getFileCountSize(QList<QString> files)
{
    quint64 size = 0;
    for (int i = 0;i < files.size();i++ )
    {
        QFileInfo info(files.at(i));
        size += info.size();
    }
    return size;
}

QString CopyMoveFileDialog::localFileSize(quint64 size)
{
    QLocale locale;
    QString valueText = locale.formattedDataSize(size);
    return valueText;
}

void CopyMoveFileDialog::startProcess()
{
    qDebug() << "开始处理文件";
    for (int i = 0; i < mFileNumber; i++)
    {
        QString filePath = mFiles.at(i);
        QFileInfo fileInfo(filePath);
        QFileInfo fileDirInfo(fileInfo.absolutePath());
        this->ui->sourcePathBtn->setText(fileDirInfo.fileName());
        this->ui->currentFileNameLabel->setText(fileInfo.fileName());
        QApplication::processEvents();
        this->ui->remainItemsLabel->setText(QString("%1 (%2)").arg(mFileNumber - i).arg(localFileSize(mFileRemainSumSize)));

        //目标
        QFileInfo target(mTargetPath + "/" + fileInfo.fileName());



    }
}
