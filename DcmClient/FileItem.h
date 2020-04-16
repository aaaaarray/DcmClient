#pragma once

#include <QWidget>
#include <qpushbutton.h>
#include <qlabel.h>
#include <QProgressBar>
#include <QContextMenuEvent>
enum UPLOADSTATUS
{
	UPLOADING,
	UPLOADED,
	UPLOADFAIL
};
class FileItem : public QWidget
{
	Q_OBJECT

public:
	FileItem(QWidget *parent, QString file, UPLOADSTATUS status);
	~FileItem();
	void upload();
	QString getFilehash();
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void contextMenuEvent(QContextMenuEvent *event);
private:
	QLabel *m_typeLabel;//文件格式缩略图
	QLabel *m_fileNameLabel;//文件名
	QPushButton *m_FileLocationButton;//打开文件位置
	QPushButton *m_deleteButton;//删除
	QLabel *m_progressLabel;//显示进度状态
	QLabel *m_statusLabel;//显示状态或者上传速度
	QProgressBar *m_progressBar;//上传进度条	
	QMenu *pMenu;
	QString filePath;
	
	QString  hashValue;
	UPLOADSTATUS m_status;
signals:
	void toDeleteFile(QString filePath);
	void toUploadFail(QString);
private slots:
	void openFile();
	void deleteFile();
	void onTaskBoxContextMenuEvent();
};
