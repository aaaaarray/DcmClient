#include "SettingWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include "IniEx.h"
#include "DirFileEx.h"
#include "HttpRequestModel.h"
#include <QSettings>
#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
SettingWidget::SettingWidget(QWidget *parent)
	: QWidget(parent)
{
	this->setWindowTitle(LoadLanguageString("setting", "title"));
	setFixedSize(500, 440);
	labelOrgId = new QLabel(this);
	lineeditOrgId = new QLineEdit(this);

	labelOrgName = new QLabel(this);
	lineeditOrgName = new QLineEdit(this);

	labelClientId = new QLabel(this);
	lineeditClientId = new QLineEdit(this);

	checkBoxSelfStart = new QCheckBox(this);

	labelBaseDir = new QLabel(this);
	lineeditBaseDir = new QLineEdit(this);
	pushbuttonBaseDir = new QPushButton(this);

	labelBackDir = new QLabel(this);
	lineeditBackDir = new QLineEdit(this);

	pushbuttonCancel = new QPushButton(this);
	pushbuttonOK = new QPushButton(this);

	labelOrgId->setText(LoadLanguageString("setting", "orgId"));
	labelOrgId->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	labelOrgId->setGeometry(20, 20, 100, 40);
	lineeditOrgId->setGeometry(130, 20, 300, 40);
	lineeditOrgId->setEnabled(false);

	labelOrgName->setText(LoadLanguageString("setting", "orgName"));
	labelOrgName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	labelOrgName->setGeometry(20, 80, 100, 40);
	lineeditOrgName->setGeometry(130, 80, 300, 40);
	lineeditOrgName->setEnabled(false);


	labelClientId->setText(LoadLanguageString("setting", "clientId"));
	labelClientId->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	labelClientId->setGeometry(20, 140, 100, 40);
	lineeditClientId->setGeometry(130, 140, 300, 40);
	lineeditClientId->setEnabled(false);

	labelBaseDir->setText(LoadLanguageString("setting", "dataDir"));
	labelBaseDir->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	labelBaseDir->setGeometry(20, 200, 100, 40);
	lineeditBaseDir->setGeometry(130, 200, 210, 40);
	lineeditBaseDir->setEnabled(false);
	pushbuttonBaseDir->setText(LoadLanguageString("setting", "choseDir"));
	pushbuttonBaseDir->setGeometry(350, 200, 80, 40);

	labelBackDir->setText(LoadLanguageString("setting", "dataBackDir"));
	labelBackDir->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	labelBackDir->setGeometry(20, 260, 100, 40);
	lineeditBackDir->setGeometry(130, 260, 210, 40);
	lineeditBackDir->setEnabled(false);

	checkBoxSelfStart->setText(LoadLanguageString("setting", "selfStart"));
	checkBoxSelfStart->setGeometry(100, 320, 200, 40);

	pushbuttonCancel->setText(LoadLanguageString("setting", "cancel"));
	pushbuttonCancel->setGeometry(50, 380, 100, 40);

	pushbuttonOK->setText(LoadLanguageString("setting", "ok"));
	pushbuttonOK->setGeometry(350, 380, 100, 40);

	connect(pushbuttonCancel, SIGNAL(pressed()), this, SLOT(close()));
	connect(pushbuttonOK, SIGNAL(pressed()), this, SLOT(onOk()));
	connect(pushbuttonBaseDir, SIGNAL(pressed()), this, SLOT(onChooseDir()));

	orgId = ReadIniString("client", "orgId", Ex_GetRoamingDir() + "config.ini");
	orgName = ReadIniString("client", "orgName", Ex_GetRoamingDir() + "config.ini");
	if (orgId == "" || orgName == "")
	{
		orgId = ReadIniString("org", "orgId", m_gRunConfig);
		orgName = ReadIniString("org", "orgName", m_gRunConfig);
		if (orgId == "" || orgName == ""){
			QMessageBox::critical(NULL, LoadLanguageString("error", "critical"), LoadLanguageString("error", "errorsetting"), QMessageBox::Yes);
			exit(0);
		}
		WriteIniString("client", "orgId", orgId, Ex_GetRoamingDir() + "config.ini");
		WriteIniString("client", "orgName", orgName, Ex_GetRoamingDir() + "config.ini");
	}
	QString clientId = ReadIniString("client", "clientId", Ex_GetRoamingDir() + "config.ini");
	QString dataDir = ReadIniString("client", "dataDir", Ex_GetRoamingDir() + "config.ini");
	QString backDir = ReadIniString("client", "backDir", Ex_GetRoamingDir() + "config.ini");
	QString autoStart = ReadIniString("client", "autoStart", Ex_GetRoamingDir() + "config.ini");

	lineeditOrgId->setText(orgId);
	lineeditOrgName->setText(orgName);
	lineeditClientId->setText(clientId);
	lineeditBaseDir->setText(dataDir);
	lineeditBackDir->setText(backDir);

	checkBoxSelfStart->setChecked(autoStart == "0" ? false : true);
	

}

SettingWidget::~SettingWidget()
{
}

void SettingWidget::init(){
	QString clientId = ReadIniString("client", "clientId", Ex_GetRoamingDir() + "config.ini");
	QString dataDir = ReadIniString("client", "dataDir", Ex_GetRoamingDir() + "config.ini");
	QString clientUrl;
	HttpRequestModel *m_httpRequestModel = HttpRequestModel::getHttpRequestModel();
	if (m_httpRequestModel->CheckClient(orgId, orgName, dataDir, clientId, clientUrl) == 1)
	{
		WriteIniString("client", "clientId", "", Ex_GetRoamingDir() + "config.ini");
	}

	checkUpdate(clientUrl);

	emit(toStartFileSystemWatcher());
}


void SettingWidget::onOk()
{
	//if (lineeditOrgId->text() == ""){
	//	QMessageBox::critical(NULL, LoadLanguageString("error", "critical"), LoadLanguageString("error", "emptyOrgId"), QMessageBox::Yes);
	//	return;
	//}
	//if (lineeditOrgName->text() == ""){
	//	QMessageBox::critical(NULL, LoadLanguageString("error", "critical"), LoadLanguageString("error", "emptyOrgName"), QMessageBox::Yes);
	//	return;
	//}
	if (lineeditBaseDir->text() == ""){
		QMessageBox::critical(NULL, LoadLanguageString("error", "critical"), LoadLanguageString("error", "emptyDataDir"), QMessageBox::Yes);
		return;
	}
	if (lineeditBackDir->text() == ""){
		QMessageBox::critical(NULL, LoadLanguageString("error", "critical"), LoadLanguageString("error", "emptyBackDataDir"), QMessageBox::Yes);
		return;
	}
	if (lineeditBackDir->text() == lineeditBaseDir->text()){
		QMessageBox::critical(NULL, LoadLanguageString("error", "critical"), LoadLanguageString("error", "DataEqualBackDir"), QMessageBox::Yes);
		return;
	}
	
	if (!IsDirectoryExist(lineeditBaseDir->text())){
		QMessageBox::StandardButton result = QMessageBox::information(NULL, LoadLanguageString("setting", "tip"), lineeditBaseDir->text() + LoadLanguageString("setting", "dircreate"), QMessageBox::Yes | QMessageBox::No);
		if (result == QMessageBox::Yes)
		{
			if (Ex_CreateDiretory(lineeditBaseDir->text())){
				QMessageBox::information(NULL, LoadLanguageString("setting", "tip"), LoadLanguageString("setting", "dirCreateSucc"), QMessageBox::Yes);
			}
			else{
				QMessageBox::critical(NULL, LoadLanguageString("setting", "tip"), LoadLanguageString("setting", "dirCreateFail"), QMessageBox::Yes);
				lineeditBaseDir->clear();
				return;
			}
		}
		else{
			lineeditBaseDir->clear();
			return;
		}
	}

	if (!IsDirectoryExist(lineeditBackDir->text())){
		QMessageBox::StandardButton result = QMessageBox::information(NULL, LoadLanguageString("setting", "tip"), lineeditBackDir->text() + LoadLanguageString("setting", "dircreate"), QMessageBox::Yes | QMessageBox::No);
		if (result == QMessageBox::Yes)
		{
			if (Ex_CreateDiretory(lineeditBackDir->text())){
				QMessageBox::information(NULL, LoadLanguageString("setting", "tip"), LoadLanguageString("setting", "dirCreateSucc"), QMessageBox::Yes);
			}
			else{
				QMessageBox::critical(NULL, LoadLanguageString("setting", "tip"), LoadLanguageString("setting", "dirCreateFail"), QMessageBox::Yes);
				lineeditBackDir->clear();
				return;
			}
		}
		else{
			lineeditBackDir->clear();
			return;
		}
	}
	QString clientId = lineeditClientId->text();
	QString dataDir = lineeditBaseDir->text();
	QString backDir = lineeditBackDir->text();
	QString api;
	HttpRequestModel *m_httpRequestModel = HttpRequestModel::getHttpRequestModel();

	QString application_name = QApplication::applicationName();
	QSettings *settings = new QSettings(REG_RUN, QSettings::NativeFormat);
	if (checkBoxSelfStart->isChecked()){
		QString application_path = QApplication::applicationFilePath();
		settings->setValue(application_name, application_path.replace("/", "\\"));
		WriteIniString("client", "autoStart","1", Ex_GetRoamingDir() + "config.ini");
	}
	else{
		settings->remove(application_name);
		WriteIniString("client", "autoStart", "0", Ex_GetRoamingDir() + "config.ini");
	}
	delete settings;

	if (m_httpRequestModel->InitClient(orgId, orgName, dataDir, clientId, api) == true)
	{
		/*WriteIniString("client", "orgId", orgId, Ex_GetRoamingDir() + "config.ini");
		WriteIniString("client", "orgName", orgName, Ex_GetRoamingDir() + "config.ini");*/
		if (clientId != NULL&&clientId != ""){
			lineeditClientId->setText(clientId);
			WriteIniString("client", "clientId", clientId, Ex_GetRoamingDir() + "config.ini");
		}
		WriteIniString("client", "dataDir", dataDir, Ex_GetRoamingDir() + "config.ini");
		WriteIniString("client", "backDir", backDir, Ex_GetRoamingDir() + "config.ini");
		WriteIniString("client", "api", api, Ex_GetRoamingDir() + "config.ini");
		QMessageBox::information(NULL, LoadLanguageString("setting", "tip"), LoadLanguageString("setting", "succ"), QMessageBox::Yes);

		this->hide();
		emit(toStartFileSystemWatcher());
	}
	else
	{
		QMessageBox::critical(NULL, LoadLanguageString("setting", "tip"), LoadLanguageString("setting", "fail"), QMessageBox::Yes);
		//lineeditBaseDir->clear();
		lineeditClientId->clear();
		WriteIniString("client", "clientId", "", Ex_GetRoamingDir() + "config.ini");
		return;
	}

}

void SettingWidget::onChooseDir()
{
	QString srcDirPath = QFileDialog::getExistingDirectory(this, LoadLanguageString("setting", "choseDir"), "/");

	if (srcDirPath.isEmpty())
	{
		return;
	}
	lineeditBaseDir->setText(srcDirPath);
	lineeditBackDir->setText(srcDirPath+"_back");
}
#include "log.h"
#include  <windows.h>
#include <process.h>
#include <shellapi.h>
#pragma comment(lib,"Shell32.lib")
#include <qdesktopservices.h>
#include "DateUtils.h"
#include "Util.h"
void SettingWidget::checkUpdate(QString clientUrl){
	//clientUrl = "http://106.39.114.85/download/opvideo_setup_v2.3.4.1.exe";
	QString strRemoteVersion;
	strRemoteVersion = clientUrl.mid(clientUrl.lastIndexOf("v") + 1);
	strRemoteVersion = strRemoteVersion.mid(0, strRemoteVersion.lastIndexOf("."));
	QString strLocalVersion = ReadIniString("Version", "Version", m_gRunConfig);
		QStringList strLocalVersionlist = strLocalVersion.split(".");
		QStringList strVersionRemotelist = strRemoteVersion.split(".");
		for (int i = 0; i < 4; i++)
		{
			int r = strVersionRemotelist.at(i).toInt();
			int l = strLocalVersionlist.at(i).toInt();
			if (r > l)
			{;
				break;
			}
			else if (r < l) {
				return;
			}
		}
		
	
		QString strUpdateConfig = Ex_GetTempDir() + "update/config.ini";
		QString strVersion = ReadIniString("UpDate", "Version", strUpdateConfig);
		QString strPath = ReadIniString("UpDate", "Path", strUpdateConfig);
		QString strExeName = ReadIniString("UpDate", "Name", strUpdateConfig);
		QString strStartTime = ReadIniString("UpDate", "StartTime", strUpdateConfig);
		QFileInfo fileInfo(strPath);

		if (!strVersion.isEmpty()
			&& strVersion == strRemoteVersion
			&& fileInfo.exists()
			)
		{
			log_info("%s  is downloaded, now install...", strExeName.toStdString().c_str());
			ShellExecute(NULL, L"open", strPath.toStdWString().c_str(), NULL, NULL, SW_HIDE);
			return;
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
					return;
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
			log_info("start Update %s ", strStart.toStdString().c_str());
			QString lpParameters = strRemoteVersion + " " + clientUrl;
			ShellExecute(NULL, L"open", strStart.toStdWString().c_str(), lpParameters.toStdWString().c_str(), NULL, SW_HIDE);
		}
		else
		{
			log_info("program lose update ");
		}
	
}