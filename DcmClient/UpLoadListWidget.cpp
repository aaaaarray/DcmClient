#include "UpLoadListWidget.h"
#include <QDebug>
UpLoadListWidget::UpLoadListWidget(QWidget *parent, UPLOADSTATUS status)
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

UpLoadListWidget::~UpLoadListWidget()
{
}

void UpLoadListWidget::addFile(QString file, UPLOADSTATUS status)
{
	if (status == UPLOADING){
		qDebug() << "addDonwloadingFile -> " << file;
		FileItem *fileitem = new FileItem(m_listFileWidget->parentWidget(), file, status);
		connect(fileitem, SIGNAL(toDeleteFile(QString)), this, SLOT(deleteFile(QString)));
		QListWidgetItem* item = new QListWidgetItem(m_listFileWidget);
		item->setBackgroundColor(Qt::gray);
		int width = m_listFileWidget->width();
		fileitem->setFixedWidth(width );
		item->setSizeHint(QSize(width, 100));
		item->setText(file);
		m_listFileWidget->insertItem(m_listFileWidget->count(), item);
		m_listFileWidget->setItemWidget(item, fileitem);
		m_listFileWidget->show();
		fileitem->upload();
	}
	else{
		qDebug() << "addDonwloadedFile -> " << file;
		FileItem *fileitem = new FileItem(m_listFileWidget->parentWidget(), file, status);
		connect(fileitem, SIGNAL(toDeleteFile(QString)), this, SLOT(deleteFile(QString)));
		QListWidgetItem* item = new QListWidgetItem(m_listFileWidget);
		item->setBackgroundColor(Qt::gray);
		int width = m_listFileWidget->width();
		fileitem->setFixedWidth(width);
		item->setSizeHint(QSize(width, 100));
		item->setText(file);
		m_listFileWidget->insertItem(m_listFileWidget->count(), item);
		m_listFileWidget->setItemWidget(item, fileitem);
		m_listFileWidget->show();
	}
}

void UpLoadListWidget::deleteFile(QString filePath)
{
	int row = 0;
	QString line;
	while (row<(m_listFileWidget->count()))
	{
		QString text= m_listFileWidget->item(row)->text();
		if (text == filePath){
			m_listFileWidget->takeItem(row);
			emit(toAddDonwloadedFile(filePath));
		}
		
		row++;
	}
}

