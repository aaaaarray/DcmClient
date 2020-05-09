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

//bool CheckUpdate(int &bForcedUpgrade, QString &strRemoteVersion, QString &strFileUrl)
//{
//	QString strLocalVersion = ReadIniString("Version", "Version", m_gRunConfig);
//	log_info( "version = %s" , strLocalVersion.toStdString().c_str());
//	if (strLocalVersion.length() <= 0)
//	{
//		log_info("lost setting.ini or setting.ini is error");
//		showMessageBox(QMessageBox::Warning, LoadLanguageString("error", "str003"));
//		return true;
//	}
//	HttpRequestModel *m_httpRequestModel = HttpRequestModel::getHttpRequestModel();
//
//	QString strForcedUpgradeVersion;
//	m_httpRequestModel->getVersion(strRemoteVersion, strForcedUpgradeVersion, strFileUrl);
//	if (strRemoteVersion.isEmpty())
//	{
//		return false;
//	}
//
//	QStringList strLocalVersionlist = strLocalVersion.split(".");
//	QStringList strVersionRemotelist = strRemoteVersion.split(".");
//	bool bNew = false;
//	for (int i = 0; i < 4; i++)
//	{
//		int r = strVersionRemotelist.at(i).toInt();
//		int l = strLocalVersionlist.at(i).toInt();
//		if (r > l)
//		{
//			bNew = true;
//			break;
//		}
//		else if (r < l) {
//			bNew = false;
//			break;
//		}
//	}
//	if (bNew && !strForcedUpgradeVersion.isEmpty())
//	{
//		if (strForcedUpgradeVersion == "1")
//			bForcedUpgrade = 1;
//		else if (strForcedUpgradeVersion == "2")
//			bForcedUpgrade = 2;
//	}
//	return bNew;
//
//}
//

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
