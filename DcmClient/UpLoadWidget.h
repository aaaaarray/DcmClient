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
	void addFile(QString);
	void clearFile();
	void updateUploadStatus(QString, int);
	void reUploadingFile(QString filePath);
private:
	QPushButton *pushbuttonUpLoading;
	QPushButton *pushbuttonUpLoaded;
	UploadFileThread *m_UploadFileThread;

	UpLoadListWidget *m_UpLoadingListWidget;
	UpLoadListWidget *m_UpLoadedListWidget;
	void showUploading(bool);
};
