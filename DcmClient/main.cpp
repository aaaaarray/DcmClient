#include "DcmClient.h"
#include "SettingWidget.h"
#include "UpLoadWidget.h"
#include <QtWidgets/QApplication>
#include <QDir>
#include "def.h"
#include "IniEx.h"
#include "DirFileEx.h"
#include "Util.h"
#include <process.h>
#include <shellapi.h>
#pragma comment(lib,"Shell32.lib")
#include "log.h"
#include "HttpRequestModel.h"
#include <qfileinfo.h>
#include <qdesktopservices.h>
#include "DateUtils.h"
#include <qtextcodec.h> 

bool CheckUpdate(int &bForcedUpgrade, QString &strRemoteVersion, QString &strFileUrl)
{
	QString strLocalVersion = ReadIniString("Version", "Version", m_gRunConfig);
	log_info( "version = %s" , strLocalVersion.toStdString().c_str());
	if (strLocalVersion.length() <= 0)
	{
		log_info("lost setting.ini or setting.ini is error");
		showMessageBox(QMessageBox::Warning, LoadLanguageString("error", "str003"));
		return true;
	}
	HttpRequestModel *m_httpRequestModel = HttpRequestModel::getHttpRequestModel();

	QString strForcedUpgradeVersion;
	m_httpRequestModel->getVersion(strRemoteVersion, strForcedUpgradeVersion, strFileUrl);
	if (strRemoteVersion.isEmpty())
	{
		return false;
	}

	QStringList strLocalVersionlist = strLocalVersion.split(".");
	QStringList strVersionRemotelist = strRemoteVersion.split(".");
	bool bNew = false;
	for (int i = 0; i < 4; i++)
	{
		int r = strVersionRemotelist.at(i).toInt();
		int l = strLocalVersionlist.at(i).toInt();
		if (r > l)
		{
			bNew = true;
			break;
		}
		else if (r < l) {
			bNew = false;
			break;
		}
	}
	if (bNew && !strForcedUpgradeVersion.isEmpty())
	{
		if (strForcedUpgradeVersion == "1")
			bForcedUpgrade = 1;
		else if (strForcedUpgradeVersion == "2")
			bForcedUpgrade = 2;
	}
	return bNew;

}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(false);
	g_strResPath = GetRunDir() + "res/default/";//初始化资源路径
	m_gRunConfig = GetRunDir() + "res/setting.ini";

	QString strRunName = a.applicationDisplayName();
	int nCurpid = _getpid();
	int n = AlreadyRunning(strRunName + ".exe", nCurpid);
	if (n>0)
	{
		log_info("%s is AlreadyRunning", strRunName.toStdString().c_str());
		HANDLE   hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)n);
		TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
		//return false;
	}
	setLog();


	int bForcedUpgrade = 0;
	QString strRomteVersion;
	QString strFileUrl;
	if (CheckUpdate(bForcedUpgrade, strRomteVersion, strFileUrl))
	{
		QString strUpdateConfig = Ex_GetTempDir() + "update/config.ini";
		QString strVersion = ReadIniString("UpDate", "Version", strUpdateConfig);
		QString strPath = ReadIniString("UpDate", "Path", strUpdateConfig);
		QString strExeName = ReadIniString("UpDate", "Name", strUpdateConfig);
		QString strStartTime = ReadIniString("UpDate", "StartTime", strUpdateConfig);
		QFileInfo fileInfo(strPath);
		if (bForcedUpgrade > 0)
		{
			if (fileInfo.exists() && !strVersion.isEmpty() && strVersion == strRomteVersion)
			{
				ShellExecute(NULL, L"open", strPath.toStdWString().c_str(), NULL, NULL, SW_HIDE);
				return false;
			}
			int ret = showMessageBox(QMessageBox::Information, LoadLanguageString("tip", "str050"));
			if (QMessageBox::Yes == ret || bForcedUpgrade == 2)
			{
				QDesktopServices::openUrl(QUrl(strFileUrl));
				return false;
			}
		}
		if (!strVersion.isEmpty()
			&& strVersion == strRomteVersion
			&& fileInfo.exists()
			)
		{
			log_info("%s  is downloaded, now install...", strExeName.toStdString().c_str());
			ShellExecute(NULL, L"open", strPath.toStdWString().c_str(), NULL, NULL, SW_HIDE);
			return false;
		}
		else if (!strStartTime.isEmpty())
		{
			int nCurrrentID = 0;
			int n = AlreadyRunning(ReadIniString("Update", "update", m_gRunConfig), nCurrrentID);
			if (n > 0)
			{
				log_info("Update is AlreadyRunning");

				int nTime = DateUtils::getCurrentDateTime();
				int nTimeOld = strStartTime.toInt();
				if (nTime - nTimeOld < 600)
				{
					log_info("Update is AlreadyRunning not more than 10 min");
					goto dlgmain;
				}
				log_info("Update is AlreadyRunning more than 10 min, now kill Update");
				QString cmd = "taskkill /im " + ReadIniString("Update", "update", m_gRunConfig) + " /f";
				ShellExecute(NULL, cmd.toStdWString().c_str(), NULL, NULL, NULL, SW_SHOWNORMAL);
			}

		}
		else
		{
			int nCurrrentID = 0;
			int n = AlreadyRunning(ReadIniString("Update", "update", m_gRunConfig), nCurrrentID);
			if (n > 0)
			{
				log_info("Update is AlreadyRunning and kill");
				QString cmd = "taskkill /im " + ReadIniString("Update", "update", m_gRunConfig) + " /f";
				ShellExecute(NULL, cmd.toStdWString().c_str(), NULL, NULL, NULL, SW_SHOWNORMAL);
			}
		}
		QString strStart = GetRunDir() + ReadIniString("Update", "update", m_gRunConfig);
		if (IsFileExist(strStart))
		{
			log_info("start Update %s " , strStart.toStdString().c_str());
			QString lpParameters = strRomteVersion + " " + strFileUrl;
			ShellExecute(NULL, L"open", strStart.toStdWString().c_str(), lpParameters.toStdWString().c_str(), NULL, SW_HIDE);
		}
		else
		{
			log_info("program lose update ");
		}
	}
dlgmain:
	DcmClient *window = new DcmClient;
	//window->show();
	Q_UNUSED(window);
	/*SettingWidget *setting = new SettingWidget();
	setting->show();*/

	/*DownLoadWidget*download = new DownLoadWidget();
	download->show();*/
	return a.exec();
}
