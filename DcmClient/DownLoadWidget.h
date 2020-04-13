#pragma once

#include <QWidget>
#include "DownLoadListWidget.h"
#include <qpushbutton.h>
class DownLoadWidget : public QWidget
{
	Q_OBJECT

public:
	DownLoadWidget(QWidget *parent = 0);
	~DownLoadWidget();
public slots:
	void donwLoading();
	void downLoaded();
	void addFile(QString);
private:
	QPushButton *pushbuttonDownLoading;
	QPushButton *pushbuttonDownLoaded;

	DownLoadListWidget *m_DownLoadingListWidget;
	DownLoadListWidget *m_DownLoadedListWidget;
	void showDownloading(bool);
};
