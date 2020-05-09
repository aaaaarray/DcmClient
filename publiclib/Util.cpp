#include "Util.h"
#include "def.h"
#include <QColor>
#include <locale>  
#include <codecvt> 
#include "log.h"
#include <Windows.h>
#include <iphlpapi.h>
#include <tlhelp32.h> 
#pragma comment(lib,"Iphlpapi.lib")



int AlreadyRunning(const QString& exe,int nCurrentID)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		return -1;
	}
	DWORD dwPid = -1;
	bool nRes = false;
	while (Process32Next(hProcessSnap, &pe32))
	{
		//将WCHAR转成const char*  
		int iLn = WideCharToMultiByte(CP_UTF8, 0, const_cast<LPWSTR> (pe32.szExeFile), static_cast<int>(sizeof(pe32.szExeFile)), NULL, 0, NULL, NULL);
		std::string result(iLn, 0);
		WideCharToMultiByte(CP_UTF8, 0, pe32.szExeFile, static_cast<int>(sizeof(pe32.szExeFile)), const_cast<LPSTR> (result.c_str()), iLn, NULL, NULL);
		if (0 == strcmp(exe.toStdString().c_str(), result.c_str()))
		{
			dwPid = pe32.th32ProcessID;
			if (((int)dwPid)!=nCurrentID)
			{
				nRes = true;
				break;
			}
			
		}
	}
	CloseHandle(hProcessSnap);
	if (nRes)
	{
		return (int)dwPid;
	}
	return -1;
}

QString GetOSVersoin(int& nHVersion, int& nLVersion)
{
	OSVERSIONINFO   osver;
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osver);
	QString strOSName;
	if (osver.dwPlatformId == 2)
	{
		nHVersion = osver.dwMajorVersion;
		nLVersion = osver.dwMinorVersion;
		if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
		{
			strOSName = "XP";
		}
		else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
		{
			strOSName = "2003";
		}
		else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0)
		{
			strOSName = "Vista&2008";
		}
		else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)
		{
			strOSName = "2008R2&Win7";
		}
		else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
		{
			strOSName = "Win8";
		}
		else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3)
		{
			strOSName = "Win8.1";
		}
		else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 4)
		{
			strOSName = "Win10";
		}
		else if (osver.dwMajorVersion == 10 && osver.dwMinorVersion == 0)
		{
			strOSName = "Win10";
		}
		else
		{
			strOSName = "unknown";
		}
	}
	return strOSName;
}

void SetStyleSheet(QPushButton *pb, QString strName)
{
	if (pb&&!strName.isEmpty())
	{
		pb->setStyleSheet("QPushButton{border-image: url(" + g_strResPath + strName + ".png);}"
			"QPushButton:hover{border-image: url(" + g_strResPath + strName + "_hover.png);}"
			"QPushButton:pressed{border-image: url(" + g_strResPath + strName + "_pressed.png);}");
	}
}

//根据中英文获取图片名字
QString GetPicNameByLanguage(QString strName)
{
	QString strRet = "";

	//目前只支持中英两个版本
	if (m_glanguage == "zh_CN")//简体中文
		strRet =  strName;
	else
		strRet = strName + "_en";

	return strRet;
}
#include "IniEx.h"
#include "DirFileEx.h"
#include <QDir>
#include "DateUtils.h"
void setLog(){
	QString g_szLogPath = GetLogDir();
	QDir *temp = new QDir;
	bool exist = temp->exists(g_szLogPath);
	if (!exist)
	{
		bool ok = false;
		ok = temp->mkpath(g_szLogPath);
	}
	QString szLogPathFiletmp = g_szLogPath + DateUtils::getDateUTC8("yyyy.MM.dd") + "_tmp.log";

	QString szLogPathFile = g_szLogPath + DateUtils::getDateUTC8("yyyy.MM.dd") + ".log";
	if (IsFileExist(szLogPathFiletmp) && !IsFileExist(szLogPathFile))
	{
		QFile file(szLogPathFiletmp);
		file.rename(szLogPathFile);
		QFile::remove(szLogPathFiletmp);
	}
	log_set_fp(szLogPathFile.toStdString().c_str());
#ifdef QT_DEBUG
	log_set_console(true);
#else
	log_set_console(false);
#endif
	log_set_level(LOG_TRACE);
}