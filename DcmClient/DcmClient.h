#pragma once

#include <QtWidgets/QWidget>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include "SettingWidget.h"
class DcmClient : public QWidget
{
	Q_OBJECT

public:
	DcmClient(QWidget *parent = Q_NULLPTR);

	QSystemTrayIcon *systray;
	void closeEvent(QCloseEvent *event);

private slots:
	void showDetils();
	void showSetting();
	void about();
	void trayActivated(QSystemTrayIcon::ActivationReason reason);
private:
	QAction *action_detils, *action_setting, *action_about, *action_quit;
	SettingWidget *settingWidget;
	void init();
};
