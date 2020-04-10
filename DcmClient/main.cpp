#include "DcmClient.h"
#include "SettingWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(false);
	DcmClient *window = new DcmClient;
	window->show();
	//Q_UNUSED(window);
	/*SettingWidget *setting = new SettingWidget();
	setting->show();*/
	return a.exec();
}
