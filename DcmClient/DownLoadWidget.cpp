#include "DownLoadWidget.h"
#include "IniEx.h"
#include <QDebug>
DownLoadWidget::DownLoadWidget(QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle(LoadLanguageString("download", "title"));
	setFixedSize(800, 600);

	pushbuttonDownLoading = new QPushButton(this);
	pushbuttonDownLoaded = new QPushButton(this);
	connect(pushbuttonDownLoading, SIGNAL(pressed()), this, SLOT(donwLoading()));
	connect(pushbuttonDownLoaded, SIGNAL(pressed()), this, SLOT(downLoaded()));
	pushbuttonDownLoading->setGeometry(0, 0, 250, 30);
	pushbuttonDownLoaded->setGeometry(250, 0, 250, 30);
	pushbuttonDownLoading->setText(LoadLanguageString("download", "downloading"));
	pushbuttonDownLoaded->setText(LoadLanguageString("download", "downloaded"));

	m_DownLoadingListWidget = new DownLoadListWidget(this, DOWNLOADING);
	m_DownLoadedListWidget = new DownLoadListWidget(this, DOWNLOADED);
	m_DownLoadingListWidget->setGeometry(0, 30, 800, 570);
	m_DownLoadedListWidget->setGeometry(0, 30, 800, 570);

	showDownloading(true);
}

DownLoadWidget::~DownLoadWidget()
{
}


void DownLoadWidget::showDownloading(bool show)
{
	if (show)
	{
		m_DownLoadedListWidget->hide();
		m_DownLoadingListWidget->show();
	}
	else
	{
		m_DownLoadingListWidget->hide();
		m_DownLoadedListWidget->show();
	}
}

void DownLoadWidget::donwLoading(){
	showDownloading(true);
}
void DownLoadWidget::downLoaded()
{
	showDownloading(false);
}

void DownLoadWidget::addFile(QString file){
	m_DownLoadingListWidget->addFile(file, DOWNLOADING);
}