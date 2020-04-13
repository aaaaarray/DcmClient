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
	m_listFileWidget->setGeometry(0,0,800,550);
	/*m_listFileWidget->setStyleSheet("QListWidget{outline:0px;}"
		"QListWidget::Item{background:white; }"
		"QListWidget::Item:hover{background:white; }"
		"QListWidget::item:selected{background:white; }");*/

	m_listFileWidget->setStyleSheet(QString::fromUtf8("QScrollBar:vertical {"
		"    border: 1px solid #fff;"
		"    background:white;"
		"    width:10px;    "
		"    margin: 0px 0px 0px 0px;"
		"}"
		"QScrollBar::handle:vertical {"
		"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
		"    stop: 0 rgb(45, 45, 45), stop: 0.5 rgb(45, 45, 45), stop:1 rgb(45, 45, 45));"
		"    min-height: 0px;"
		"}"
		"QScrollBar::add-line:vertical {"
		"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
		"    stop: 0 rgb(45, 45, 45), stop: 0.5 rgb(45, 45, 45),  stop:1 rgb(45, 45, 45));"
		"    height: 0px;"
		"    subcontrol-position: bottom;"
		"    subcontrol-origin: margin;"
		"}"
		"QScrollBar::sub-line:vertical {"
		"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
		"    stop: 0  rgb(45, 45, 45), stop: 0.5 rgb(45, 45, 45),  stop:1 rgb(45, 45, 45));"
		"    height: 0 px;"
		"    subcontrol-position: top;"
		"    subcontrol-origin: margin;"
		"}"
		"QListWidget{outline:0px;background:white}"
		"QListWidget::Item{background:white; }"
		"QListWidget::Item:hover{background:white; }"
		"QListWidget::item:selected{background:white; }"
		"QListWidget::item:selected:!active{background:white; }"
		));
	
}

DownLoadListWidget::~DownLoadListWidget()
{
}

void DownLoadListWidget::addFile(QString file, DOWNLOADSTATUS status)
{
	if (status == DOWNLOADING){
		qDebug() << "addFile -> " << file;
		FileItem *fileitem = new FileItem(m_listFileWidget->parentWidget(), file, status);
		QListWidgetItem* item = new QListWidgetItem(m_listFileWidget);
		item->setBackgroundColor(Qt::gray);
		int width = m_listFileWidget->width();
		fileitem->setFixedWidth(width );
		item->setSizeHint(QSize(width, 100));

		m_listFileWidget->insertItem(m_listFileWidget->count(), item);
		m_listFileWidget->setItemWidget(item, fileitem);
		m_listFileWidget->show();
	}
}