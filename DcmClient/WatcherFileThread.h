#pragma once

#include <QThread>
#include <Windows.h>
class WatcherFileThread : public QThread
{
	Q_OBJECT

public:
	WatcherFileThread(QObject *parent);
	~WatcherFileThread();
	void setWatchDir(QString watchDir);
signals:
	void toAddUploadingFile(QString);
protected:
	virtual void run();
	QString watchDir;
private:
	void WatchDirectories(HANDLE hCompPort);
	void StartWatcher(const TCHAR* szDirName);
	void listFile(QString path);
};
