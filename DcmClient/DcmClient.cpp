#include "DcmClient.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDateTime>
#include <QDebug>


#include <QJsonParseError>
#include <QMenu>
#include <QMessageBox>
#include <QStyle>
#include <Windows.h>
#include "IniEx.h"
#include "DirFileEx.h"

DcmClient::DcmClient(QWidget *parent)
	: QWidget(parent)
{
	//this->hide();
	//setStyleSheet(" color:white; ");
	m_WatcherFileThread = NULL;
	setWindowTitle(LoadLanguageString("menu", "title"));
	//setFixedSize(500, 400);
	//move((QApplication::desktop()->width() - QApplication::desktop()->width()) / 2, (QApplication::desktop()->height() - QApplication::desktop()->height()) / 2);
	setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMinimizeButtonHint);
	setWindowFlags(Qt::Tool);

	systray = new QSystemTrayIcon(this);
	systray->setToolTip(LoadLanguageString("menu", "title"));
	systray->setIcon(QIcon("icon.png"));
	systray->setVisible(true);
	QMenu *traymenu = new QMenu(this);
	action_detils = new QAction(LoadLanguageString("menu", "detils"), traymenu);
	QIcon icon = QApplication::style()->standardIcon(QStyle::SP_ComputerIcon);
	action_detils->setIcon(icon);
	action_setting = new QAction(LoadLanguageString("menu", "setting"), traymenu);
	icon = QApplication::style()->standardIcon(QStyle::SP_DriveNetIcon);
	action_setting->setIcon(icon);
	action_about = new QAction(LoadLanguageString("menu", "about"), traymenu);
	icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
	action_about->setIcon(icon);
	action_quit = new QAction(LoadLanguageString("menu", "quit"), traymenu);
	icon = QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton);
	action_quit->setIcon(icon);
	traymenu->addAction(action_detils);
	traymenu->addAction(action_setting);
	traymenu->addAction(action_about);
	traymenu->addAction(action_quit);
	systray->setContextMenu(traymenu);
	systray->show();
	connect(systray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
	connect(action_detils, SIGNAL(triggered(bool)), this, SLOT(showDetils()));
	connect(action_setting, SIGNAL(triggered(bool)), this, SLOT(showSetting()));
	connect(action_about, SIGNAL(triggered(bool)), this, SLOT(about()));
	connect(action_quit, SIGNAL(triggered()), qApp, SLOT(quit()));
	

	settingWidget = new SettingWidget();
	settingWidget->hide();
	
	upLoadWidget = new UpLoadWidget();
	upLoadWidget->show();
	connect(this, SIGNAL(toStartFileSystemWatcher()), this, SLOT(setFileSystemWatcher()));
	connect(settingWidget, SIGNAL(toStartFileSystemWatcher()), this, SLOT(setFileSystemWatcher()));
	emit(toStartFileSystemWatcher());
}


void DcmClient::closeEvent(QCloseEvent *event)
{
	hide();
	event->ignore();
}
void DcmClient::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
	qDebug() << reason;
	switch (reason) {
	case QSystemTrayIcon::Trigger:
	{
		//systray->showMessage("sstq", "", QSystemTrayIcon::MessageIcon::Information, 9000);
		break;
	}
	case QSystemTrayIcon::DoubleClick:
		showDetils();
		break;
	default:
		break;
	}
}

void DcmClient::showDetils()
{
	upLoadWidget->show();
}
void DcmClient::showSetting()
{
	settingWidget->show();
}
void DcmClient::about()
{
	QMessageBox mb(QMessageBox::NoIcon, "about", "ggg");
	mb.setIconPixmap(QPixmap(":/icon.png"));
	mb.exec();
	//QMessageBox::information(this, "Title", "Content");

}

void DcmClient::init(){
	
}

void DcmClient::setFileSystemWatcher()
{
	QString orgId = ReadIniString("client", "orgId", Ex_GetRoamingDir() + "config.ini");
	QString orgName = ReadIniString("client", "orgName", Ex_GetRoamingDir() + "config.ini");
	QString clientId = ReadIniString("client", "clientId", Ex_GetRoamingDir() + "config.ini");
	QString dataDir = ReadIniString("client", "dataDir", Ex_GetRoamingDir() + "config.ini");
	if (orgId == "" || orgName == "" || clientId == "" || dataDir == ""){
		settingWidget->show();
	}
	else
	{
		if (m_WatcherFileThread != NULL){
			delete m_WatcherFileThread;
			m_WatcherFileThread = NULL;
		}
		m_WatcherFileThread = new WatcherFileThread(this);
		connect(m_WatcherFileThread, SIGNAL(toAddUploadingFile(QString)), upLoadWidget, SLOT(addFile(QString)));
		m_WatcherFileThread->setWatchDir(dataDir);
		m_WatcherFileThread->start();
	}

	
}

void DcmClient::fileChanged(QString path){
	qDebug() << __FUNCTION__ << "fileChanged path = " << path;
}

void DcmClient::dirChanged(QString path)
{
	qDebug() << __FUNCTION__ << "directoryChanged path = " << path;
}