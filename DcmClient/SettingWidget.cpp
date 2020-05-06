#include "SettingWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
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
	QString api;
	HttpRequestModel *m_httpRequestModel = HttpRequestModel::getHttpRequestModel();
	if (m_httpRequestModel->CheckClient(orgId, orgName, dataDir, clientId) == 1)
	{
		WriteIniString("client", "clientId", "", Ex_GetRoamingDir() + "config.ini");
	}
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