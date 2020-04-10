#include "DirFileEx.h"
#include "log.h"
#include "Util.h"
#include "def.h"
#include "IniEx.h"
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QtWidgets/QApplication>
static QString g_szPathMulti;//D:/C++/eClass5.0/bin/Debug/
static QString g_szeClassLogPath; //C:/Users/lenovo/AppData/Local/Temp/ChelClass/log/
static QString g_szeClassDocumentPath; // C:/Users/lenovo/AppData/Local/Temp/ChelClass/document/
static QString g_szeClassTempFolder;
static QString g_szeClassRoamingFolder;

const QString GetRunDir()
{
	if (g_szPathMulti.isEmpty())
	{
		g_szPathMulti = QApplication::applicationDirPath() + "/";
#ifdef QT_DEBUG
		g_szPathMulti.replace("Debug", "Release");
#endif
	}	
	return g_szPathMulti;
}

bool Ex_CreateDiretory(QString pszDir)
{
	QDir temp;
	if (temp.exists(pszDir))
		return true;
	if (temp.mkpath(pszDir))
		return true;
	return false;
}

bool DeleteDir(const QString &dirName)
{
	QDir directory(dirName);
	if (!directory.exists())
	{
		return true;
	}
	QString srcPath = QDir::toNativeSeparators(dirName);
	if (!srcPath.endsWith(QDir::separator()))
		srcPath += QDir::separator();
	QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
	bool error = false;
	for (QStringList::size_type i = 0; i != fileNames.size(); ++i)
	{
		QString filePath = srcPath + fileNames.at(i);
		QFileInfo fileInfo(filePath);
		if (fileInfo.isFile() || fileInfo.isSymLink())
		{
			QFile::setPermissions(filePath, QFile::WriteOwner);
			if (!QFile::remove(filePath))
			{
				log_error("remove file %s faild!", filePath.toStdString().c_str());
				error = true;
			}
		}
		else if (fileInfo.isDir())
		{
			if (!DeleteDir(filePath))
			{
				error = true;
			}
		}
	}
	if (!directory.rmdir(QDir::toNativeSeparators(directory.path())))
	{
		log_error("remove dir %s faild!", directory.path().toStdString().c_str());
		error = true;
	}
	return !error;
}

//检查文件是否存在
bool IsFileExist(const QString pszFileName)
{
	if (pszFileName.isEmpty())
	{
		return false;
	}
	QFileInfo file(pszFileName);

	if (file.exists()==false)
	{
		return false;
	}

	return true;
}

//检查目录是否存在
bool IsDirectoryExist(const QString fullPath)
{
	QDir dir(fullPath);
	if (dir.exists())
	{
		return true;
	}
	return false;
}

//Log和document都放在tmp目录下
const QString GetLogDir()
{
	if (g_szeClassLogPath.isEmpty())
	{
		QString cstrTmpPath = Ex_GetTempDir() + "log/";
		if (!Ex_CreateDiretory(cstrTmpPath.toStdString().c_str()))
		{
			QString cstrLogPath = GetRunDir() + "log/";
			g_szeClassLogPath = cstrLogPath;
		}
		else
		{
			g_szeClassLogPath = cstrTmpPath;
		}
	}
	return g_szeClassLogPath;
}

//Log和document都放在tmp目录下
const QString GetDocumentDir()
{
	if (g_szeClassDocumentPath.isEmpty())
	{
		QString cstrTmpPath = Ex_GetTempDir() + "document/";
		if (!Ex_CreateDiretory(cstrTmpPath.toStdString().c_str()))
		{
			log_error("CreateDiretory error:%s.", cstrTmpPath.toStdString().c_str());
			g_szeClassDocumentPath = GetRunDir() + "/document/";
		}
		else
		{
			g_szeClassDocumentPath = cstrTmpPath;
		}
	}		
	return g_szeClassDocumentPath;
}


const QString Ex_GetTempDir()
{
	if (g_szeClassTempFolder.isEmpty())
	{
		QString name = ReadIniString("Organization", "GroupName", m_gRunConfig);
		g_szeClassTempFolder = QDir::tempPath() + "/" + name + "/";
	}	
	return g_szeClassTempFolder;
}

const QString Ex_GetRoamingDir()
{
	if (g_szeClassRoamingFolder.isEmpty())
	{
		QString strSavePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		QString name = ReadIniString("Organization", "GroupName", m_gRunConfig);
		int n = strSavePath.lastIndexOf("/") + 1;
		g_szeClassRoamingFolder = strSavePath.left(n) + name + "/";
	}

	return g_szeClassRoamingFolder;
}


bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
	toDir.replace("\\", "/");
	if (sourceDir == toDir) {
		return true;
	}
	if (!QFile::exists(sourceDir)) {
		return false;
	}
	QDir *createfile = new QDir;
	bool exist = createfile->exists(toDir);
	if (exist) {
		if (coverFileIfExist) {
			createfile->remove(toDir);
		}
	}//end if

	if (!QFile::copy(sourceDir, toDir))
	{
		return false;
	}
	return true;
}
