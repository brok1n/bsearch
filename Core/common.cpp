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
