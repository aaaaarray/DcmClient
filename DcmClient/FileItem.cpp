#include "FileItem.h"
#include "IniEx.h"
#include "DirFileEx.h"
#include "def.h"
#include <Windows.h>
#include <QPainter.h>
#include <QDebug>
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include "HttpRequestModel.h"
#include <QProcess>
FileItem::FileItem(QWidget *parent, QString file, UPLOADSTATUS status)
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
	
	QFile filehash(filePath);
	if (filehash.open(QIODevice::ReadOnly))
	{
		
		QCryptographicHash hash(QCryptographicHash::Md5);
		if (!filehash.atEnd())
		{
			hash.addData(filehash.readAll());
			hashValue.append(hash.result().toHex());
		}
		filehash.close();
	}

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

	QFileInfo info(filePath);
	m_progressLabel->setText(LoadLanguageString("upload", "tip") + QString::number(info.size()) + " Bytes");
	if (status == UPLOADING){
		m_statusLabel->setText(LoadLanguageString("upload", "upload"));
	}
	else{
		m_statusLabel->setText(LoadLanguageString("upload", "uploaded"));
	}
	
	m_progressBar->hide();
	connect(m_FileLocationButton, SIGNAL(pressed()), this, SLOT(openFile()));
	connect(m_deleteButton, SIGNAL(pressed()), this, SLOT(deleteFile()));
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
	//m_statusLabel->setText(LoadLanguageString("upload", "uploading"));
	HttpRequestModel *m_httpRequestModel = HttpRequestModel::getHttpRequestModel();
	if (m_httpRequestModel->uploadFile(filePath, hashValue)){
		//upload success
		emit(toDeleteFile(filePath));
	}
	else{
		//upload fail
	}

}

QString FileItem::getFilehash()
{
	return hashValue;
}

void FileItem::openFile(){
	QProcess process;
	QString  str = filePath;
	str.replace("/", "\\"); // 只能识别 "\"
	QString cmd = QString("explorer.exe /select,\"%1\"").arg(str);
	qDebug() << cmd;
	process.startDetached(cmd);
	
}
void FileItem::deleteFile(){

}