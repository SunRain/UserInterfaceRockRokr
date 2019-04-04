#ifndef RKUTILITY_H
#define RKUTILITY_H

#include <QString>

class RKUtility
{
public:
    explicit RKUtility();
    ~RKUtility();

    static QString coverCacheDir();

    static QString httpCookieFile();

};

#endif // RKUTILITY_H
