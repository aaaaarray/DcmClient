#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include "FileItem.h"

class UpLoadListWidget : public QWidget
{
	Q_OBJECT

public:
	UpLoadListWidget(QWidget *parent, UPLOADSTATUS status);
	~UpLoadListWidget();
	void addFile(QString file, UPLOADSTATUS status);
	void clearFile();
signals:
	void toAddUploadedFile(QString);
private slots:
void deleteUploadingFile(QString filePath);
void deleteUploadedFile(QString filePath);

private:
	QListWidget * m_listFileWidget;
};
