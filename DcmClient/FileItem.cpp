#include "FileItem.h"

FileItem::FileItem(QWidget *parent,QString file, DOWNLOADSTATUS status)
	: QWidget(parent)
{
	QPalette palette(this->palette());
	QColor backcorlor(45, 45, 45);
	palette.setColor(QPalette::Background, backcorlor);
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
	m_deleteButton->setGeometry(750, 15, 22, 22);
	m_progressLabel->setGeometry(100, 45, 200, 22);
	m_statusLabel->setGeometry(680, 45, 90, 22);
	m_progressBar->setGeometry(100, 70, 650, 4);


	m_statusLabel->setAlignment(Qt::AlignRight);
}

FileItem::~FileItem()
{
}
