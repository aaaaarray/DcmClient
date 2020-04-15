#pragma once

#include <QWidget>
#include "UpLoadListWidget.h"
#include <qpushbutton.h>
class UpLoadWidget : public QWidget
{
	Q_OBJECT

public:
	UpLoadWidget(QWidget *parent = 0);
	~UpLoadWidget();
public slots:
	void upLoading();
	void upLoaded();
	void addFile(QString);
	void addUploadedFile(QString);
private:
	QPushButton *pushbuttonUpLoading;
	QPushButton *pushbuttonUpLoaded;

	UpLoadListWidget *m_UpLoadingListWidget;
	UpLoadListWidget *m_UpLoadedListWidget;
	void showUploading(bool);
};
