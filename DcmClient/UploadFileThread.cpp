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

void UploadFileThread::addFile(QString filePath)
{
	{
		AutoLock autoLock(&m_threadCritSec);
		m_uploadingList.push_back(filePath);
	}
	if (!this->isRunning()){
		this->start();
	}
}


QString UploadFileThread::getFile(){
	AutoLock autoLock(&m_threadCritSec);
	if (m_uploadingList.count() > 0){
		qDebug() << "==============================================" << m_uploadingList.count();
		QString filePath = m_uploadingList.at(0);
		m_uploadingList.pop_front();
		return filePath;
	}
	return NULL;
}

void UploadFileThread::run()
{
	while (true)
	{
		QString filePath = getFile();
		
		if (filePath == NULL){			
			return;
		}
		
		qDebug() << "==============================================" << filePath;
		if (m_httpRequestModel->uploadFile(filePath)){
			m_UploadFileThreadEvent->onUpdateUploadStatus(filePath, 1);
		}
		else{
			addFile(filePath);
		}
		
	}
}