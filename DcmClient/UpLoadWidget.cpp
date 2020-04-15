#include "UpLoadWidget.h"
#include "IniEx.h"
#include <QDebug>
UpLoadWidget::UpLoadWidget(QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle(LoadLanguageString("upload", "title"));
	setFixedSize(800, 600);

	pushbuttonUpLoading = new QPushButton(this);
	pushbuttonUpLoaded = new QPushButton(this);
	connect(pushbuttonUpLoading, SIGNAL(pressed()), this, SLOT(upLoading()));
	connect(pushbuttonUpLoaded, SIGNAL(pressed()), this, SLOT(upLoaded()));
	pushbuttonUpLoading->setGeometry(0, 0, 250, 30);
	pushbuttonUpLoaded->setGeometry(250, 0, 250, 30);
	pushbuttonUpLoading->setText(LoadLanguageString("upload", "uploading"));
	pushbuttonUpLoaded->setText(LoadLanguageString("upload", "uploaded"));

	m_UpLoadingListWidget = new UpLoadListWidget(this, UPLOADING);
	m_UpLoadedListWidget = new UpLoadListWidget(this, UPLOADED);
	m_UpLoadingListWidget->setGeometry(0, 30, 800, 570);
	m_UpLoadedListWidget->setGeometry(0, 30, 800, 570);
	connect(m_UpLoadingListWidget, SIGNAL(toAddDonwloadedFile(QString)), this, SLOT(addDonwloadedFile(QString)));
	showUploading(true);
}

UpLoadWidget::~UpLoadWidget()
{
}


void UpLoadWidget::showUploading(bool show)
{
	if (show)
	{
		m_UpLoadedListWidget->hide();
		m_UpLoadingListWidget->show();
	}
	else
	{
		m_UpLoadingListWidget->hide();
		m_UpLoadedListWidget->show();
	}
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
}

void UpLoadWidget::addUploadedFile(QString file)
{
	m_UpLoadedListWidget->addFile(file, UPLOADED);
}