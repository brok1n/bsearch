#include "common.h"

Common::Common()
{

}

QString Common::formatFileSize(qint64 size)
{
    if(size > FILE_SIZE_TB)
    {
        return QString("%1TB").arg(size / FILE_SIZE_TB);
    }
    else if(size > FILE_SIZE_GB)
    {
        return QString("%1GB").arg(size / FILE_SIZE_GB);
    }
    else if(size > FILE_SIZE_MB)
    {
        return QString("%1MB").arg(size / FILE_SIZE_MB);
    }
    else if(size > FILE_SIZE_KB)
    {
        return QString("%1KB").arg(size / FILE_SIZE_KB);
    }
    return QString("%1字节").arg(size);
}

QString Common::fileTypeToName(FILE_TYPE tp)
{
    switch (tp) {
    case FILE_TYPE::FILE_MUSIC:
        return FILE_MUSIC_NAME;
        break;
    case FILE_TYPE::FILE_VIDEO:
        return FILE_VIDEO_NAME;
        break;
    case FILE_TYPE::FILE_IMAGE:
        return FILE_IMAGE_NAME;
        break;
    case FILE_TYPE::FILE_DOCUMENT:
        return FILE_DOCUMENT_NAME;
        break;
    case FILE_TYPE::FILE_COMPRESS:
        return FILE_COMPRESS_NAME;
        break;
    case FILE_TYPE::FILE_EXECUTABLE:
        return FILE_EXECUTABLE_NAME;
        break;
    case FILE_TYPE::FILE_DIR:
        return FILE_DIR_NAME;
        break;
    }
    return FILE_OTHER_NAME;
}
