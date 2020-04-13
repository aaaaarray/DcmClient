#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include "FileItem.h"

class DownLoadListWidget : public QWidget
{
	Q_OBJECT

public:
	DownLoadListWidget(QWidget *parent, DOWNLOADSTATUS status);
	~DownLoadListWidget();
	void addFile(QString file,  DOWNLOADSTATUS status);
private:
	QListWidget * m_listFileWidget;
};
