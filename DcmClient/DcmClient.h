#pragma once

#include <QtWidgets/QWidget>
#include <QCloseEvent>
#include <QSystemTrayIcon>

#include "SettingWidget.h"
#include "DownLoadWidget.h"
#include "WatcherFileThread.h"

class DcmClient : public QWidget
{
	Q_OBJECT

public:
	DcmClient(QWidget *parent = Q_NULLPTR);

	QSystemTrayIcon *systray;
	void closeEvent(QCloseEvent *event);
signals:
	void toStartFileSystemWatcher();
private slots:
	void showDetils();
	void showSetting();
	void about();
	void trayActivated(QSystemTrayIcon::ActivationReason reason);
	void fileChanged(QString path);
	void dirChanged(QString path);
	void setFileSystemWatcher();
private:
	QAction *action_detils, *action_setting, *action_about, *action_quit;
	SettingWidget *settingWidget;
	DownLoadWidget *downLoadWidget;
	
	WatcherFileThread *m_WatcherFileThread;
	void init();
	
	
	


};
