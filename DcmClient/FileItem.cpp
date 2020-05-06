#include "FileItem.h"
#include "IniEx.h"
#include "DirFileEx.h"
#include "def.h"
#include <Windows.h>
#include <QPainter.h>
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include "HttpRequestModel.h"
#include <QProcess>
#include <QMenu>
#include <QAction>
FileItem::FileItem(QWidget *parent, QString file, UPLOADSTATUS status)
	: QWidget(parent)
	, m_status(status)
	, m_contextMenu(NULL)
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
	m_progressLabel = new QLabel(this);
	m_statusLabel = new QLabel(this);
	m_progressBar = new QProgressBar(this);
	connect(m_FileLocationButton, SIGNAL(pressed()), this, SLOT(openFile()));
	connect(m_deleteButton, SIGNAL(pressed()), this, SLOT(deleteFile()));

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

	QFileInfo info(filePath);
	m_progressLabel->setText(LoadLanguageString("upload", "tip") + QString::number(info.size()) + " Bytes");
	if (m_status == UPLOADING){
		m_statusLabel->setText(LoadLanguageString("upload", "upload"));
	}
	else if (m_status == UPLOADED){
		m_statusLabel->setText(LoadLanguageString("upload", "uploaded"));
		//上传成功后删除文件 
        //todo
		//m_FileLocationButton->hide();
		//QFile::remove(filePath);
	}
	else{
		m_statusLabel->setText(LoadLanguageString("upload", "fail"));
		m_contextMenu = new QMenu(this);

		QAction *reUpload = new QAction(LoadLanguageString("upload", "reUpload"), this);
		reUpload->setData(1);
		m_contextMenu->addAction(reUpload);
		m_contextMenu->setStyleSheet("QMenu{background-color: rgb(238,242,241);border: none;}"
			"QMenu::item{background-color: rgb(238,242,241);color: rgb(0, 0, 0);}"
			"QMenu::item:selected{background-color: rgb(238,242,241);color: rgb(0, 0, 0);}");
		connect(reUpload, SIGNAL(triggered()), this, SLOT(onTaskBoxContextMenuEvent()));		
	}	
	m_progressBar->hide();	
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

void FileItem::openFile(){
	QProcess process;
	QString  str = filePath;
	str.replace("/", "\\"); // 只能识别 "\"
	QString cmd = QString("explorer.exe /select,\"%1\"").arg(str);
	process.startDetached(cmd);	
}
void FileItem::deleteFile(){
	emit(toDeleteFile(filePath));
}
void FileItem::onTaskBoxContextMenuEvent()
{
	QAction *pEven = qobject_cast<QAction *>(this->sender());

	//获取发送信息类型 1:新建任务 
	int iType = pEven->data().toInt();

	switch (iType)
	{
	case 1:
		//todo 
		emit(toReUpload(filePath));
		break;
	default:
		break;
	}
}

void FileItem::contextMenuEvent(QContextMenuEvent *event){
	if (m_status == UPLOADFAIL&&m_contextMenu != NULL){
		m_contextMenu->exec(cursor().pos());
	}
}