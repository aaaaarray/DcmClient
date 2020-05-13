#include "UploadFileThread.h"
#include "AutoLock.h"
#include "log.h"
#include "DateUtils.h"
#include <QDir>
#include "IniEx.h"
#include "DirFileEx.h"
#include "Util.h"
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
			QString dataDir = ReadIniString("client", "dataDir", Ex_GetRoamingDir() + "config.ini");
			DeleteEmptyDir(dataDir);
			Ex_CreateDiretory(dataDir);

			QString szLogPathFile = GetLogDir() + DateUtils::getDateUTC8("yyyy.MM.dd") + "_tmp.log";
			log_set_fp(szLogPathFile.toStdString().c_str());
			if (!m_httpRequestModel->PackageLog()){
				log_error("fail");
			}				
			else{
				log_info("success");
				setLog();
			}
				
			return;
		}
		if (m_httpRequestModel->uploadFile(filePath)){
			m_UploadFileThreadEvent->onUpdateUploadStatus(filePath, 1);
			moveFile(filePath);
		}
		else{
			m_UploadFileThreadEvent->onUpdateUploadStatus(filePath, 2);
			addFile(filePath, UPLOADFAIL);

		}
	}
}

void UploadFileThread::moveFile(QString file)
{
	QString dataDir = ReadIniString("client", "dataDir", Ex_GetRoamingDir() + "config.ini");
	QString backDir = ReadIniString("client", "backDir", Ex_GetRoamingDir() + "config.ini");
	QString target = file;
	target= target.mid(dataDir.length());

	target = backDir+target;
	target = target.mid(0, target.lastIndexOf("/") + 1);//QStringLiteral
	log_info("move %s to %s", file.toStdString().c_str(), target.toStdString().c_str());
	moveFileToPath(file, target);
}
