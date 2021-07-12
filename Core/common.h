#ifndef COMMON_H
#define COMMON_H

#include <QList>
#include <QStringList>



//线程池最大线程数
const int MAX_THREAD_COUNT = 100;

//文件类型
const QString FILE_TYPE_MUSIC[] = {".ape", ".amr", ".aix", ".flac", ".mp3", ".mpc", ".ncm", ".ogg", ".mka", ".wma", ".wav", ".wv", ".aac", ".ac3", ".aif", ".asf", ".mmf", ".musx", ".pcm", ".tta", ".voc", ".vox", ".m4a", ".m4r", ".mpa", ".ra", ".gsm", ".mid"};
const QString FILE_TYPE_VIDEO[] = {".rm", ".rmvb", ".3gp", ".3g2", ".mpg", ".mpeg", ".mod", ".ts", ".dvr-ms", ".m2ts", ".mts", ".mov", ".qt", ".dv", ".dif", ".wmv", ".asf", ".vfw", ".avi", ".avs", ".amv", ".mkv", ".mp4", ".flv", ".fli", ".dat", ".vob", ".yuv", ".cpk", ".trp", ".ogm"};
const QString FILE_TYPE_IMAGE[] = {".pfd", ".ptimage", ".ptpage", ".bmp", ".dib", ".rel", ".gif", ".ico", ".heic", ".cur", ".jbg", ".jfif", ".jp2", ".j2k", ".jpc", ".j2c", ".jpg", ".jpe", ".jpeg", ".mng", ".jng", ".png", ".pcx", ".pgx", ".pnm", ".pgm", ".ppm", ".psd", ".ras", ".nef", ".crw", ".cr2", ".mrw", ".raf", ".erf", ".3fr", ".dcr", ".raw", ".rw2", ".dng", ".pef", ".x3f", ".arw", ".sr2", ".mef", ".orf", ".ska", ".svg", ".dxf", ".tga", ".tif", ".tiff", ".wbm", ".wbmp", ".webp", ".wmf", ".emf"};
const QString FILE_TYPE_DOCUMENT[] = {".doc", ".docx", ".xls", ".xlsx", ".csv", ".ppt", ".pptx", ".pdf", ".txt", ".htm", ".html", ".xml", ".json", ""};
const QString FILE_TYPE_COMPRESS[] = {".zip", ".rar", ".gz", ".tar", ".gzip", ".cab", ".uue", ".arj", ".bz2", ".lzh", ".jar", ".ace", ".7-zip", ".z", ".iso"};

enum FILE_TYPE
{
    FILE_ALL,
    FILE_MUSIC,
    FILE_VIDEO,
    FILE_IMAGE,
    FILE_DOCUMENT,
    FILE_COMPRESS
};

const QString FILE_TYPE_NAME[6] =
{
    "全部",
    "音乐",
    "视频",
    "图片",
    "文档",
    "压缩包"
};


struct Node {
    QString name = "";
    QString _ext = "";
    QList<Node*> childs;
    bool isDir = false;
    Node *parent = nullptr;
    int _fileType = -1;

    //添加子节点
    void addChild(Node* node)
    {
        childs.append(node);
        node->parent = this;
    }

    //完整路径
    QString fullPath()
    {
        return eachParent(this).replace("//", "/");
    }

    //遍历父节点
    QString eachParent(Node *p)
    {
        if(p->parent == nullptr)
        {
            return p->name;
        }
        else
        {
            return eachParent(p->parent) + "/" + p->name;
        }
    }

    //扩展名
    QString fileExt()
    {
        if(isDir || name.isEmpty() || !name.contains("."))
        {
            return "";
        }
        if(!_ext.isEmpty())
        {
            return _ext;
        }
        QStringList sp = name.split(".");
        if(sp.size() > 1)
        {
            _ext = sp[sp.size()-1].toLower();
        }
        return _ext;
    }

    //文件类型
    int fileType()
    {
        if(_fileType >= 0)
        {
            return _fileType;
        }
        QString ext = fileExt();
        if(ext.isEmpty())
        {
            return 0;
        }

        if(FILE_TYPE_MUSIC->contains(ext))
        {
            _fileType = FILE_TYPE::FILE_MUSIC;
        }
        else if(FILE_TYPE_VIDEO->contains(ext))
        {
            _fileType = FILE_TYPE::FILE_VIDEO;
        }
        else if(FILE_TYPE_IMAGE->contains(ext))
        {
            _fileType = FILE_TYPE::FILE_IMAGE;
        }
        else if(FILE_TYPE_DOCUMENT->contains(ext))
        {
            _fileType = FILE_TYPE::FILE_DOCUMENT;
        }
        else if(FILE_TYPE_COMPRESS->contains(ext))
        {
            _fileType = FILE_TYPE::FILE_COMPRESS;
        }
        else
        {
            _fileType = FILE_TYPE::FILE_ALL;
        }

        return _fileType;
    }
};

class Common
{
public:
    Common();
};

#endif // COMMON_H