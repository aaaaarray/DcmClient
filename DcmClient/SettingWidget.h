#pragma once

#include <QWidget>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcheckbox.h>
class SettingWidget : public QWidget
{
	Q_OBJECT

public:
	SettingWidget(QWidget *parent = 0);
	~SettingWidget();
	void init();
private:
	QLabel * labelOrgId;
	QLineEdit * lineeditOrgId;

	QLabel * labelOrgName;
	QLineEdit *lineeditOrgName;

	QLabel * labelClientId;
	QLineEdit *lineeditClientId;

	QCheckBox* checkBoxSelfStart;


	QLabel * labelBaseDir;
	QLineEdit *lineeditBaseDir;
	QPushButton *pushbuttonBaseDir;

	QLabel * labelBackDir;
	QLineEdit *lineeditBackDir;

	QPushButton *pushbuttonCancel;
	QPushButton *pushbuttonOK;
signals:
	void toStartFileSystemWatcher();
private slots:
	void onOk();
	void onChoseDir();
};
