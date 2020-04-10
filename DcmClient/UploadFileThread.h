#pragma once

#include <QThread>
#include "CritSec.h"
class UploadFileThread : public QThread
{
	Q_OBJECT

public:
	UploadFileThread(QThread *parent);
	~UploadFileThread();
protected:
	virtual void run();
private:
	CritSec m_threadCritSec;

};
