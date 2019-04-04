#include "RKUtility.h"

#include <QStandardPaths>
#include <QDir>

RKUtility::RKUtility()
{

}

RKUtility::~RKUtility()
{

}

QString RKUtility::coverCacheDir()
{
    QString p = QString("%1/cover").arg(QStandardPaths::CacheLocation);
    QDir dir(p);
    if (!dir.exists()) {
        dir.mkpath(p);
    }
    return p;
}

QString RKUtility::httpCookieFile()
{
    QString cookie = QString("%1/cookie.txt").arg(QStandardPaths::AppDataLocation);
    return  cookie;
}
