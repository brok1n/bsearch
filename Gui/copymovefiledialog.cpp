#include "copymovefiledialog.h"
#include "ui_copymovefiledialog.h"

#include <QFileInfo>
#include <QTimer>
#include <QDebug>
#include <QLocale>
#include <QUrl>
#include <QDesktopServices>
#include <QThread>

CopyMoveFileDialog::CopyMoveFileDialog(QList<QString> files, QString targetPath, bool isMove, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CopyMoveFileDialog)
    , mFiles(files)
    , mTargetPath(targetPath)
    , mIsMove(isMove)
    , mFileNumber(files.size())
    , mFileSumSize(getFileCountSize(mFiles))
    , mFileProcessSize(0)
    , mCurrentFilePath("")
    , mWaitUserSelection(false)
    , mIgnoreCurrentFile(false)
    , mCancel(false)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags()|Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);

    flushWindowTitle();

    this->setProcessFileNumber(mFileNumber);


    QFileInfo targetPathInfo(mTargetPath);
    this->setTargetDir(targetPathInfo.fileName());

    this->ui->progressBar->setMinimum(0);
    this->ui->progressBar->setMaximum(100);
    this->ui->progressBar->setValue(0);

}

CopyMoveFileDialog::~CopyMoveFileDialog()
{
    qDebug() << "释放复制/移动窗口...";
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

void CopyMoveFileDialog::setSourceDir(QString dirName)
{
    if(dirName.length() > 15)
    {
        dirName = dirName.mid(0, 15);
        dirName.append("...");
    }
    this->ui->sourcePathBtn->setText(dirName);
    this->ui->sourcePathBtn2->setText(dirName);
}

void CopyMoveFileDialog::setTargetDir(QString dirName)
{
    if(dirName.length() > 15)
    {
        dirName = dirName.mid(0, 15);
        dirName.append("...");
    }
    this->ui->targetPathBtn->setText(dirName);
    this->ui->targetPathBtn2->setText(dirName);
}

void CopyMoveFileDialog::setProcessFileNumber(int number)
{
    this->ui->fileCountLabel->setText(QString::number(number));
    this->ui->fileCountLabel2->setText(QString::number(number));
}

void CopyMoveFileDialog::flushWindowTitle()
{
    if(mWaitUserSelection)
    {
        this->ui->stackedWidget->setCurrentIndex(1);
        this->setWindowTitle("替换或跳过文件");
    }
    else
    {
        this->ui->stackedWidget->setCurrentIndex(0);
        if(mIsMove)
        {
            setWindowTitle("正在移动...");
        }
        else
        {
            setWindowTitle("正在复制...");
        }
    }

}

void CopyMoveFileDialog::closeEvent(QCloseEvent *event)
{
    mCancel = true;
    event->ignore();
}

void CopyMoveFileDialog::startProcess()
{
    qDebug() << "开始处理文件";
    int process = 0;
    for (int i = 0; i < mFileNumber; i++)
    {
        mCurrentFilePath = mFiles.at(i);
        QFileInfo fileInfo(mCurrentFilePath);
        QFileInfo fileDirInfo(fileInfo.absolutePath());
        this->setSourceDir(fileDirInfo.fileName());
        this->ui->currentFileNameLabel->setText(fileInfo.fileName());
        this->ui->remainItemsLabel->setText(QString("%1 (%2)").arg(mFileNumber - i).arg(localFileSize(mFileSumSize - mFileProcessSize)));
        QApplication::processEvents();

        if(!fileInfo.exists())
        {
            qDebug() << "源文件不存在！略过";
            continue;
        }

        //目标
        QString targetFilePath = mTargetPath + "/" + fileInfo.fileName();
        QFileInfo target(targetFilePath);

        if(target.exists())
        {
            qDebug() << "文件已存在:" << mCurrentFilePath;
            mWaitUserSelection = true;
            mIgnoreCurrentFile = false;
            this->ui->existsFileNameLabel->setText(fileInfo.fileName());
            flushWindowTitle();
            while (mWaitUserSelection) {
                QApplication::processEvents();
                QThread::msleep(1);
            }
            flushWindowTitle();
            this->ui->stackedWidget->setCurrentIndex(0);

            //如果用户选择了忽略该文件，就跳过这个文件的复制，否则 就覆盖写入
            if(mIgnoreCurrentFile)
            {
                continue;
            }
        }

        QFile out(target.absoluteFilePath());
        out.open(QIODevice::WriteOnly);


        QFile input(mCurrentFilePath);
        input.open(QIODevice::ReadOnly);


        while (!input.atEnd()) {
            //一次度128Kb
            QByteArray data = input.read(131072);
            out.write(data);
            out.flush();
            mFileProcessSize += data.length();
            this->ui->remainItemsLabel->setText(QString("%1 (%2)").arg(mFileNumber - i).arg(localFileSize(mFileSumSize - mFileProcessSize)));

            process = (int)((float)mFileProcessSize / (float)mFileSumSize * 100);
            this->ui->progressBar->setValue(process);
            qDebug() << "process:" << process;
            QApplication::processEvents();
            if(mCancel)
            {
                break;
            }
        }
        input.close();
        out.close();
        QApplication::processEvents();
        if(mIsMove)
        {
            QFile currentFile(mCurrentFilePath);
            currentFile.remove();
        }
        if(mCancel)
        {
            QFile targetFile(targetFilePath);
            targetFile.remove();
            break;
        }
        QApplication::processEvents();
    }

    QApplication::processEvents();
    if(mCancel)
    {
        this->ui->remainTimeLabel->setText("用户取消");
        this->ui->remainItemsLabel->setText("用户取消");
    }
    else
    {
        this->ui->progressBar->setValue(100);
        this->ui->remainTimeLabel->setText("处理完成");
        this->ui->remainItemsLabel->setText("处理完成");
    }

    QApplication::processEvents();
    QTimer::singleShot(1000, this, SLOT(onProcessFinished()));

}

void CopyMoveFileDialog::onProcessFinished()
{
    QApplication::processEvents();
    this->close();
    this->deleteLater();
}

void CopyMoveFileDialog::on_sourcePathBtn_clicked()
{
    if(!mCurrentFilePath.isEmpty())
    {
        QFileInfo currentFileInfo(mCurrentFilePath);
        QUrl _url = QUrl::fromLocalFile(currentFileInfo.absolutePath());
        QDesktopServices::openUrl(_url);
    }
}


void CopyMoveFileDialog::on_targetPathBtn_clicked()
{
    if(!mTargetPath.isEmpty())
    {
        QUrl _url = QUrl::fromLocalFile(mTargetPath);
        QDesktopServices::openUrl(_url);
    }
}


void CopyMoveFileDialog::on_replaceBtn_clicked()
{
    mWaitUserSelection = false;
}


void CopyMoveFileDialog::on_ignoreBtn_clicked()
{
    mWaitUserSelection = false;
    mIgnoreCurrentFile = true;
}
