#pragma once

#include <QWidget>
#include "UpLoadListWidget.h"
#include "UploadFileThread.h"
#include <qpushbutton.h>
class UpLoadWidget : public QWidget, public UploadFileThreadEvent
{
	Q_OBJECT

public:
	UpLoadWidget(QWidget *parent = 0);
	~UpLoadWidget();

	virtual void onUpdateUploadStatus(QString, int);
signals:
	void toUpdateUploadStatus(QString, int);
public slots:
	void upLoading();
	void upLoaded();
	//void uploadFail(QString);
	void addFile(QString);
	//void addUploadedFile(QString);
	void clearFile();
	void updateUploadStatus(QString, int);
private:
	QPushButton *pushbuttonUpLoading;
	QPushButton *pushbuttonUpLoaded;
	UploadFileThread *m_UploadFileThread;

	UpLoadListWidget *m_UpLoadingListWidget;
	UpLoadListWidget *m_UpLoadedListWidget;
	void showUploading(bool);
};
