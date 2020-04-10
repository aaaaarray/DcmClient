#pragma once
#include "publiclib_global.h"
#include <QString>
PUBLICLIB_EXPORT void WriteIniString(QString strGroup,QString strKey,QString strValue, QString strPath);
PUBLICLIB_EXPORT QString ReadIniString(QString strGroup, QString strKey, QString strPath);
PUBLICLIB_EXPORT QString LoadLanguageString(QString strGroup, QString strKey);