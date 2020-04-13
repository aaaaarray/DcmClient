#include "DcmClient.h"
#include "SettingWidget.h"
#include "DownLoadWidget.h"
#include <QtWidgets/QApplication>
#include "def.h"
#include "IniEx.h"
#include "DirFileEx.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(false);
	m_gRunConfig = GetRunDir() + "res/setting.ini";
	DcmClient *window = new DcmClient;
	//window->show();
	Q_UNUSED(window);
	/*SettingWidget *setting = new SettingWidget();
	setting->show();*/

	/*DownLoadWidget*download = new DownLoadWidget();
	download->show();*/
	return a.exec();
}
