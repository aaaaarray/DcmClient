#include "FileItem.h"
#include "IniEx.h"
#include "DirFileEx.h"
#include "def.h"
#include <Windows.h>
#include <QPainter.h>
#include <QDebug>
#include "HttpRequestModel.h"
FileItem::FileItem(QWidget *parent,QString file, DOWNLOADSTATUS status)
	: QWidget(parent)
{
	filePath = file;
	QPalette palette(this->palette());
	palette.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(palette);
	m_typeLabel = new QLabel(this);
	m_fileNameLabel = new QLabel(this);
	m_FileLocationButton = new QPushButton(this);
	m_deleteButton = new QPushButton(this);
	QLabel *m_progressLabel = new QLabel(this);
	QLabel *m_statusLabel = new QLabel(this);
	m_progressBar = new QProgressBar(this);

	m_typeLabel->setGeometry(20, 20, 40, 40);
	m_fileNameLabel->setGeometry(100, 15, 570, 25);
	m_FileLocationButton->setGeometry(725, 15, 22, 22);
	m_deleteButton->setGeometry(748, 15, 22, 22);
	m_progressLabel->setGeometry(100, 45, 200, 22);
	m_statusLabel->setGeometry(680, 45, 90, 22);
	m_progressBar->setGeometry(100, 70, 720, 4);

	m_statusLabel->setAlignment(Qt::AlignRight);


	QString dataDir = ReadIniString("client", "dataDir", Ex_GetRoamingDir() + "config.ini");
	QImage image;
	image.load(g_strResPath + "default.png");
	m_typeLabel->setPixmap(QPixmap::fromImage(image));
	m_fileNameLabel->setText(file);

	
	m_FileLocationButton->setStyleSheet("QPushButton{border-image: url(" + g_strResPath + "filedir.png);}"
		"QPushButton:hover{border-image: url(" + g_strResPath + "filedir.png);}"
		"QPushButton:pressed{border-image: url(" + g_strResPath + "filedir.png);}");
	m_deleteButton->setStyleSheet("QPushButton{border-image: url(" + g_strResPath + "delete.png);}"
		"QPushButton:hover{border-image: url(" + g_strResPath + "delete.png);}"
		"QPushButton:pressed{border-image: url(" + g_strResPath + "delete.png);}");

	m_progressLabel->setText("xxxxxx");
	m_statusLabel->setText("xxxxxx");
}

FileItem::~FileItem()
{
}


void FileItem::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setBrush(QColor(240, 240, 240));
	QPen pen;
	pen.setColor(QColor(240, 240, 240));
	pen.setWidth(0);
	painter.setPen(pen);
	painter.drawRect(0, 0, geometry().width(), 80);
	pen.setColor(QColor(219, 219, 219));
	//pen.setWidth(1);
	painter.setPen(pen);
	painter.drawLine(80, 1, 80, 80);


}



void FileItem::upload()
{
	HttpRequestModel *m_httpRequestModel = HttpRequestModel::getHttpRequestModel();
	if (!m_httpRequestModel->uploadFile(filePath)){

	}

}

