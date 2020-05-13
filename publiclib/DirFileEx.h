#pragma once
#include "publiclib_global.h"
#include <QString>
PUBLICLIB_EXPORT const QString GetRunDir();
PUBLICLIB_EXPORT bool  Ex_CreateDiretory(QString pszDir);
PUBLICLIB_EXPORT bool  IsFileExist(QString pszFileName);
PUBLICLIB_EXPORT bool  IsDirectoryExist(QString psDirName);
PUBLICLIB_EXPORT const QString GetLogDir();
PUBLICLIB_EXPORT const QString GetDocumentDir();
//PUBLICLIB_EXPORT const QString GetScreenshotDir();
//PUBLICLIB_EXPORT const QString GetTempFolder();
//PUBLICLIB_EXPORT const QString GetAppdataDir();
PUBLICLIB_EXPORT bool DeleteDir(const QString dirName);
PUBLICLIB_EXPORT bool DeleteEmptyDir(const QString dirName);
PUBLICLIB_EXPORT const QString Ex_GetTempDir();
PUBLICLIB_EXPORT const QString Ex_GetRoamingDir();
PUBLICLIB_EXPORT bool copyFileToPath(QString source, QString dir);
PUBLICLIB_EXPORT bool moveFileToPath(QString source, QString dir);