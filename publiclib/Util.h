#pragma once
#include <QColor>
#include <QPushButton>

#include "publiclib_global.h"
PUBLICLIB_EXPORT int AlreadyRunning(const QString& exe, int nCurrentID);
PUBLICLIB_EXPORT QString GetOSVersoin(int& nHVersion, int& nLVersion);
PUBLICLIB_EXPORT void SetStyleSheet(QPushButton *pb, QString strName);
PUBLICLIB_EXPORT QString GetPicNameByLanguage(QString strName);