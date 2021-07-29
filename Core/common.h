#ifndef COMMON_H
#define COMMON_H

#include <QList>
#include <QStringList>

//文件大小等级
const qint64 FILE_SIZE_TB = 1024 * 1024 * 1024 * (qint64)1024;
const qint64 FILE_SIZE_GB = 1024 * 1024 * 1024;
const qint64 FILE_SIZE_MB = 1024 * 1024;
const qint64 FILE_SIZE_KB = 1024;


//线程池最大线程数
const int MAX_THREAD_COUNT = 100;

//文件类型
//const QString FILE_TYPE_MUSIC[] = {".ape", ".amr", ".aix", ".flac", ".mp3", ".mpc", ".ncm", ".ogg", ".mka", ".wma", ".wav", ".wv", ".aac", ".ac3", ".aif", ".asf", ".mmf", ".musx", ".pcm", ".tta", ".voc", ".vox", ".m4a", ".m4r", ".mpa", ".ra", ".gsm", ".mid"};
//const QString FILE_TYPE_VIDEO[] = {".rm", ".rmvb", ".3gp", ".3g2", ".mpg", ".mpeg", ".mod", ".ts", ".dvr-ms", ".m2ts", ".mts", ".mov", ".qt", ".dv", ".dif", ".wmv", ".asf", ".vfw", ".avi", ".avs", ".amv", ".mkv", ".mp4", ".flv", ".fli", ".dat", ".vob", ".yuv", ".cpk", ".trp", ".ogm"};
//const QString FILE_TYPE_IMAGE[] = {".pfd", ".ptimage", ".ptpage", ".bmp", ".dib", ".rel", ".gif", ".ico", ".heic", ".cur", ".jbg", ".jfif", ".jp2", ".j2k", ".jpc", ".j2c", ".jpg", ".jpe", ".jpeg", ".mng", ".jng", ".png", ".pcx", ".pgx", ".pnm", ".pgm", ".ppm", ".psd", ".ras", ".nef", ".crw", ".cr2", ".mrw", ".raf", ".erf", ".3fr", ".dcr", ".raw", ".rw2", ".dng", ".pef", ".x3f", ".arw", ".sr2", ".mef", ".orf", ".ska", ".svg", ".dxf", ".tga", ".tif", ".tiff", ".wbm", ".wbmp", ".webp", ".wmf", ".emf"};
//const QString FILE_TYPE_DOCUMENT[] = {".doc", ".docx", ".xls", ".xlsx", ".csv", ".ppt", ".pptx", ".pdf", ".txt", ".htm", ".html", ".xml", ".json"};
//const QString FILE_TYPE_COMPRESS[] = {".zip", ".rar", ".gz", ".tar", ".gzip", ".cab", ".uue", ".arj", ".bz2", ".lzh", ".jar", ".ace", ".7-zip", ".z", ".iso"};

const QList<QString> FILE_TYPE_MUSIC = QList<QString>() << ".ape" << ".amr" << ".aix" << ".flac" << ".mp3" << ".mpc" << ".ncm" << ".ogg" << ".mka" << ".wma" << ".wav" << ".wv" << ".aac" << ".ac3" << ".aif" << ".asf" << ".mmf" << ".musx" << ".pcm" << ".tta" << ".voc" << ".vox" << ".m4a" << ".m4r" << ".mpa" << ".ra" << ".gsm" << ".mid";
const QList<QString> FILE_TYPE_VIDEO = QList<QString>() << ".rm" << ".rmvb" << ".3gp" << ".3g2" << ".mpg" << ".mpeg" << ".mod" << ".ts" << ".dvr-ms" << ".m2ts" << ".mts" << ".mov" << ".qt" << ".dv" << ".dif" << ".wmv" << ".asf" << ".vfw" << ".avi" << ".avs" << ".amv" << ".mkv" << ".mp4" << ".flv" << ".fli" << ".dat" << ".vob" << ".yuv" << ".cpk" << ".trp" << ".ogm";
const QList<QString> FILE_TYPE_IMAGE = QList<QString>() << ".pfd" << ".ptimage" << ".ptpage" << ".bmp" << ".dib" << ".rel" << ".gif" << ".ico" << ".heic" << ".cur" << ".jbg" << ".jfif" << ".jp2" << ".j2k" << ".jpc" << ".j2c" << ".jpg" << ".jpe" << ".jpeg" << ".mng" << ".jng" << ".png" << ".pcx" << ".pgx" << ".pnm" << ".pgm" << ".ppm" << ".psd" << ".ras" << ".nef" << ".crw" << ".cr2" << ".mrw" << ".raf" << ".erf" << ".3fr" << ".dcr" << ".raw" << ".rw2" << ".dng" << ".pef" << ".x3f" << ".arw" << ".sr2" << ".mef" << ".orf" << ".ska" << ".svg" << ".dxf" << ".tga" << ".tif" << ".tiff" << ".wbm" << ".wbmp" << ".webp" << ".wmf" << ".emf";
const QList<QString> FILE_TYPE_DOCUMENT = QList<QString>() << ".doc" << ".docx" << ".xls" << ".xlsx" << ".csv" << ".ppt" << ".pptx" << ".pdf" << ".txt" << ".htm" << ".html" << ".xml" << ".json";
const QList<QString> FILE_TYPE_COMPRESS = QList<QString>() << ".zip" << ".rar" << ".gz" << ".tar" << ".gzip" << ".cab" << ".uue" << ".arj" << ".bz2" << ".lzh" << ".jar" << ".ace" << ".7-zip" << ".z" << ".iso";
const QList<QString> FILE_TYPE_EXECUTABLE = QList<QString>() << ".exe" << ".bat" << ".com" << ".msi" << ".vbs" << ".cmd" << ".pkg" << ".run" << ".rpm" << ".deb" << ".app";

enum FILE_TYPE
{
    FILE_ALL,
    FILE_MUSIC,
    FILE_VIDEO,
    FILE_IMAGE,
    FILE_DOCUMENT,
    FILE_COMPRESS,
    FILE_EXECUTABLE,
    FILE_DIR
};

const QString FILE_TYPE_NAME[8] =
{
    "全部",
    "音乐",
    "视频",
    "图片",
    "文档",
    "压缩包",
    "可执行文件",
    "文件夹"
};


struct Node {
    //文件名
    QString name = "";
    //文件扩展名
    QString _ext = "";
    //子文件列表
    QList<Node*> childs;
    //是否是文件夹
    bool isDir = false;
    //父节点 所属文件夹
    Node *parent = nullptr;
    //文件类型 音乐、视频、图片、文档、压缩包、可执行文件
    int _fileType = -1;
    //文件大小 64位
    qint64 _fileSize = 0;
    //完整路径
    QString _fullPath = "";


    static Node* create(QString name="", Node *parent=nullptr, bool isdir=false, int file_type=-1, QString ext="")
    {
        Node *node = new Node();
        node->name = name;
        node->_ext = ext;
        node->isDir =  isdir;
        if(parent != nullptr)
        {
            parent->childs.append(node);
        }
        node->parent = parent;
        node->_fileType = file_type;
        return node;
    }

    //添加子节点
    void addChild(Node* node)
    {
        childs.append(node);
        node->parent = this;
    }

    //完整路径
    QString dir()
    {
        if(isDir)
        {
            return eachParent(this).replace("//", "/");
        }
        else if(this->parent == nullptr)
        {
            return "";
        }
        else
        {
            return eachParent(this->parent).replace("//", "/");
        }
    }

    //完整路径
    QString fullPath()
    {
        if(_fullPath.isEmpty())
        {
            _fullPath = eachParent(this).replace("//", "/");
        }
        return _fullPath;
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
        //如果文件类型处理过，就不再处理文件扩展名。
        if(_fileType >= 0)
        {
            return _ext;
        }
        //文件夹、或空文件、或没有扩展名，扩展名都为空
        if(isDir || name.isEmpty() || !name.contains("."))
        {
            _ext = "";
            return "";
        }
        //有扩展名，直接返回扩展名
        if(!_ext.isEmpty())
        {
            return _ext;
        }
        //找扩展名
        QStringList sp = name.split(".");
        if(sp.size() > 1)
        {
            _ext = QString(".").append(sp[sp.size()-1].toLower());
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
            _fileType = 0;
            return _fileType;
        }

        if(FILE_TYPE_MUSIC.contains(ext))
        {
            _fileType = FILE_TYPE::FILE_MUSIC;
        }
        else if(FILE_TYPE_VIDEO.contains(ext))
        {
            _fileType = FILE_TYPE::FILE_VIDEO;
        }
        else if(FILE_TYPE_IMAGE.contains(ext))
        {
            _fileType = FILE_TYPE::FILE_IMAGE;
        }
        else if(FILE_TYPE_DOCUMENT.contains(ext))
        {
            _fileType = FILE_TYPE::FILE_DOCUMENT;
        }
        else if(FILE_TYPE_COMPRESS.contains(ext))
        {
            _fileType = FILE_TYPE::FILE_COMPRESS;
        }
        else if(FILE_TYPE_EXECUTABLE.contains(ext))
        {
            _fileType = FILE_TYPE::FILE_EXECUTABLE;
        }
        else if(isDir)
        {
            _fileType = FILE_TYPE::FILE_DIR;
        }
        else
        {
            _fileType = FILE_TYPE::FILE_ALL;
        }

        return _fileType;
    }

    void setFileSize(qint64 size)
    {
        this->_fileSize = size;
    }

    qint64 fileSize()
    {
        return this->_fileSize;
    }
};

class Common
{
public:
    Common();


    static QString formatFileSize(qint64 size);
};

#endif // COMMON_H
