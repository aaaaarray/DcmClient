#pragma once

#include <QThread>
#include "CritSec.h"
#include "HttpRequestModel.h"
class UploadFileThreadEvent
{
public:
	UploadFileThreadEvent() {}
	~UploadFileThreadEvent() {}
	virtual void onUpdateUploadStatus(QString, int) = 0;
};
class UploadFileThread : public QThread
{
	Q_OBJECT

public:
	UploadFileThread(QObject *parent, UploadFileThreadEvent *pUploadFileThreadEvent);
	~UploadFileThread();
	void addFile(QString);
	QString getFile();

protected:
	virtual void run();
private:
	CritSec m_threadCritSec;
	QList<QString> m_uploadingList;
	HttpRequestModel *m_httpRequestModel;
	UploadFileThreadEvent *m_UploadFileThreadEvent;
};
