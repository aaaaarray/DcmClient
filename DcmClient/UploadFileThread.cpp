#include "UploadFileThread.h"
#include "AutoLock.h"
#include <QDebug>
UploadFileThread::UploadFileThread(QObject *parent, UploadFileThreadEvent *pUploadFileThreadEvent)
	: QThread(parent)
	, m_UploadFileThreadEvent(pUploadFileThreadEvent)
{
	m_httpRequestModel = HttpRequestModel::getHttpRequestModel();
}

UploadFileThread::~UploadFileThread()
{
}

void UploadFileThread::addFile(QString filePath, UPLOADSTATUS status)
{
	if (status == UPLOADING){
		{
			AutoLock autoLock(&m_threadCritSec);
			m_uploadingList.push_back(filePath);
		}
		if (!this->isRunning()){
			this->start();
		}
	}
	else{
		AutoLock autoLock(&m_threadCritSec);
		m_uploadFailList.push_back(filePath);
	}
	
}


QString UploadFileThread::getFile(UPLOADSTATUS status){
	AutoLock autoLock(&m_threadCritSec);
	QString filePath = NULL;
	if (status == UPLOADING){		
		if (m_uploadingList.count() > 0){
			filePath = m_uploadingList.at(0);
			m_uploadingList.pop_front();
		}
	}
	else{
		if (m_uploadFailList.count() > 0){
			filePath = m_uploadFailList.at(0);
			m_uploadFailList.pop_front();
		}
	}

	return filePath;
}

void UploadFileThread::run()
{
	while (true)
	{
		QString filePath = getFile();
		
		if (filePath == NULL){			
			return;
		}
		if (m_httpRequestModel->uploadFile(filePath)){
			m_UploadFileThreadEvent->onUpdateUploadStatus(filePath, 1);
		}
		else{
			m_UploadFileThreadEvent->onUpdateUploadStatus(filePath, 2);
			addFile(filePath, UPLOADFAIL);

		}
	}
}