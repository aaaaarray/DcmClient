#include "DownLoadListWidget.h"
#include <QDebug>
DownLoadListWidget::DownLoadListWidget(QWidget *parent, DOWNLOADSTATUS status)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	
	m_listFileWidget = new QListWidget(this);
	m_listFileWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	m_listFileWidget->setSortingEnabled(true);
	m_listFileWidget->sortItems();
	m_listFileWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_listFileWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_listFileWidget->setGeometry(10,10,780,550);
	m_listFileWidget->setStyleSheet("QListWidget{outline:0px;}"
		"QListWidget::Item{background:white; }"
		"QListWidget::Item:hover{background:white; }"
		"QListWidget::item:selected{background:white; }");


	
}

DownLoadListWidget::~DownLoadListWidget()
{
}

void DownLoadListWidget::addFile(QString file, DOWNLOADSTATUS status)
{
	if (status == DOWNLOADING){
		FileItem *fileitem = new FileItem(m_listFileWidget->parentWidget(), file, status);
		QListWidgetItem* item = new QListWidgetItem(m_listFileWidget);
		item->setBackgroundColor(Qt::gray);
		int width = m_listFileWidget->width();
		fileitem->setFixedWidth(width - 10);
		item->setSizeHint(QSize(width, 40));

		m_listFileWidget->insertItem(m_listFileWidget->count(), item);
		m_listFileWidget->setItemWidget(item, fileitem);
		m_listFileWidget->show();
	}
}