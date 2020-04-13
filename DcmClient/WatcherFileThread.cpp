#include "WatcherFileThread.h"

#include <QDebug.h>
#define BUFFER_SIZE 2048

#define FILE_NAME_LEN 1024

//-------------------------------------------
//#define MAX_DIRS    10
#define MAX_BUFFER  4096

typedef struct _DIRECTORY_INFO {
	HANDLE      hDir;
	TCHAR       lpszDirName[MAX_PATH];
	CHAR        lpBuffer[MAX_BUFFER];
	DWORD       dwBufLength;
	OVERLAPPED  Overlapped;
}DIRECTORY_INFO, *PDIRECTORY_INFO, *LPDIRECTORY_INFO;

DIRECTORY_INFO  DirInfo;

WatcherFileThread::WatcherFileThread(QObject *parent)
	: QThread(parent)
{
}

WatcherFileThread::~WatcherFileThread()
{
}
void WatcherFileThread::setWatchDir(QString watchDir){
	this->watchDir = watchDir;
}
void WatcherFileThread::run()
{
	HANDLE  hCompPort = NULL;                 // Handle To a Completion Port

	// Get a handle to the directory
	DirInfo.hDir = CreateFile(watchDir.toStdWString().c_str(),
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ |
		FILE_SHARE_WRITE |
		FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS |
		FILE_FLAG_OVERLAPPED,
		NULL);

	if (DirInfo.hDir == INVALID_HANDLE_VALUE)
	{
		//Write_Log(LOG_ERROR, _T("Unable to open directory %s. GLE=%d."), wcharFileName, GetLastError());
	}

	// Set up a key(directory info) for each directory
	hCompPort = CreateIoCompletionPort(DirInfo.hDir, hCompPort, (DWORD)&DirInfo, 0);
	WatchDirectories(hCompPort);
}

void WatcherFileThread::WatchDirectories(HANDLE hCompPort)
{
	ReadDirectoryChangesW(DirInfo.hDir, DirInfo.lpBuffer, MAX_BUFFER, TRUE, 
		FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_SIZE,
		&DirInfo.dwBufLength, &DirInfo.Overlapped, NULL);

	DWORD numBytes;
	DWORD cbOffset;
	LPDIRECTORY_INFO di;
	LPOVERLAPPED lpOverlapped;
	PFILE_NOTIFY_INFORMATION fni;
	wchar_t wcFileName[1024] = { 0 };
	TCHAR szFileName[1024] = { 0 };
	do
	{
		GetQueuedCompletionStatus((HANDLE)hCompPort,&numBytes,(LPDWORD)&di,&lpOverlapped,INFINITE);
		if (di)
		{
			fni = (PFILE_NOTIFY_INFORMATION)di->lpBuffer;
			do
			{
				cbOffset = fni->NextEntryOffset;
				lstrcpynW(wcFileName, fni->FileName, fni->FileNameLength / sizeof(WCHAR) + 1);
				wcFileName[fni->FileNameLength / sizeof(WCHAR) + 1] = '\0';
				switch (fni->Action)
				{
				case FILE_ACTION_ADDED:		
					qDebug() << QString::fromWCharArray(wcFileName) << " add";

					break;
				case FILE_ACTION_REMOVED:				
					break;
				case FILE_ACTION_MODIFIED:
					//qDebug() << "time stamp or attribute changed: ";
					break;
				case FILE_ACTION_RENAMED_OLD_NAME:
					//qDebug() << "file name changed - old name: ";
					break;
				case FILE_ACTION_RENAMED_NEW_NAME:
					//qDebug() << "file name changed - new name: ";
					break;
					default: 
						qDebug() << "unknown event: ";
					break;
				}
				fni = (PFILE_NOTIFY_INFORMATION)((LPBYTE)fni + cbOffset);
			} while (cbOffset);
			ReadDirectoryChangesW(di->hDir, di->lpBuffer, MAX_BUFFER, TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE, &di->dwBufLength, &di->Overlapped, NULL);
		}

	} while (di);
	PostQueuedCompletionStatus(hCompPort, 0, 0, NULL);
}


