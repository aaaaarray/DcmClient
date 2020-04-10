#include "DcmClient.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDateTime>
#include <QDebug>


#include <QJsonParseError>
#include <QMenu>
#include <QMessageBox>
#include <QStyle>

#include "IniEx.h"
#include "DirFileEx.h"
DcmClient::DcmClient(QWidget *parent)
	: QWidget(parent)
{
	//setStyleSheet(" color:white; ");

	setWindowTitle(LoadLanguageString("menu", "title"));
	setFixedSize(500, 260);
	move((QApplication::desktop()->width() - QApplication::desktop()->width()) / 2, (QApplication::desktop()->height() - QApplication::desktop()->height()) / 2);
	QWidget *widget = new QWidget;
	//setCentralWidget(widget);
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
	move((QApplication::desktop()->width() - width()) / 2, (QApplication::desktop()->height() - height()) / 2);
	show();
	raise();
	activateWindow();
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