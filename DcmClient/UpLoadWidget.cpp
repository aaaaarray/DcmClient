#include "UpLoadWidget.h"
#include "IniEx.h"
#include "def.h"
#include <QDebug>
UpLoadWidget::UpLoadWidget(QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle(LoadLanguageString("upload", "title"));
	setFixedSize(800, 600);

	pushbuttonUpLoading = new QPushButton(this);
	pushbuttonUpLoaded = new QPushButton(this);
	m_UploadFileThread = new UploadFileThread(this, this);
	connect(pushbuttonUpLoading, SIGNAL(pressed()), this, SLOT(upLoading()));
	connect(pushbuttonUpLoaded, SIGNAL(pressed()), this, SLOT(upLoaded()));
	connect(this, SIGNAL(toUpdateUploadStatus(QString, int)), this, SLOT(updateUploadStatus(QString, int)));
	pushbuttonUpLoading->setGeometry(100, 0, 30, 30);
	pushbuttonUpLoaded->setGeometry(200, 0, 30, 30);
	pushbuttonUpLoading->setToolTip(LoadLanguageString("upload", "uploading"));
	pushbuttonUpLoaded->setToolTip(LoadLanguageString("upload", "uploaded"));

	m_UpLoadingListWidget = new UpLoadListWidget(this, UPLOADING);
	m_UpLoadedListWidget = new UpLoadListWidget(this, UPLOADED);
	m_UpLoadingListWidget->setGeometry(0, 30, 800, 570);
	m_UpLoadedListWidget->setGeometry(0, 30, 800, 570);
	//connect(m_UpLoadingListWidget, SIGNAL(toAddUploadedFile(QString)), this, SLOT(addUploadedFile(QString)));
	showUploading(true);
}

UpLoadWidget::~UpLoadWidget()
{
}


void UpLoadWidget::showUploading(bool show)
{
	if (show)
	{
		pushbuttonUpLoading->setStyleSheet("QPushButton{border-image: url(" + g_strResPath + "uploading_on.png);}"
			"QPushButton:hover{border-image: url(" + g_strResPath + "uploading_on.png);}"
			"QPushButton:pressed{border-image: url(" + g_strResPath + "uploading_on.png);}");
		pushbuttonUpLoaded->setStyleSheet("QPushButton{border-image: url(" + g_strResPath + "uploaded_off.png);}"
			"QPushButton:hover{border-image: url(" + g_strResPath + "uploaded_off.png);}"
			"QPushButton:pressed{border-image: url(" + g_strResPath + "uploaded_off.png);}");
		m_UpLoadedListWidget->hide();
		m_UpLoadingListWidget->show();
		
	}
	else
	{
		pushbuttonUpLoading->setStyleSheet("QPushButton{border-image: url(" + g_strResPath + "uploading_off.png);}"
			"QPushButton:hover{border-image: url(" + g_strResPath + "uploading_off.png);}"
			"QPushButton:pressed{border-image: url(" + g_strResPath + "uploading_off.png);}");
		pushbuttonUpLoaded->setStyleSheet("QPushButton{border-image: url(" + g_strResPath + "uploaded_on.png);}"
			"QPushButton:hover{border-image: url(" + g_strResPath + "uploaded_on.png);}"
			"QPushButton:pressed{border-image: url(" + g_strResPath + "uploaded_on.png);}");
		m_UpLoadingListWidget->hide();
		m_UpLoadedListWidget->show();
	}
}

void UpLoadWidget::clearFile()
{
	m_UpLoadingListWidget->clearFile();
	m_UpLoadedListWidget->clearFile();
}

void UpLoadWidget::upLoading(){
	showUploading(true);
}
void UpLoadWidget::upLoaded()
{
	showUploading(false);
}

void UpLoadWidget::addFile(QString file){
	m_UpLoadingListWidget->addFile(file, UPLOADING);
	m_UploadFileThread->addFile(file);
}

void UpLoadWidget::onUpdateUploadStatus(QString file, int status)
{
	emit(toUpdateUploadStatus(file, status));
}
void UpLoadWidget::updateUploadStatus(QString file, int status){
	qDebug() <<"UpLoadWidget::updateUploadStatus -> "<< file << "" << status;
	if (status == UPLOADED){
		m_UpLoadingListWidget->deleteUploadingFile(file);
		m_UpLoadedListWidget->addFile(file, UPLOADED);
	}
	else if (status == UPLOADFAIL){
		m_UpLoadingListWidget->deleteUploadingFile(file);
		m_UpLoadingListWidget->addFile(file, UPLOADFAIL);
	}
}

void UpLoadWidget::reUploadingFile(QString filePath)
{
	m_UpLoadingListWidget->deleteUploadingFile(filePath);
	//m_UpLoadingListWidget->addFile(filePath, UPLOADING);
	addFile(filePath);
}
