#include "DcmClientUpdate.h"
#include <QtWidgets/QApplication>
#include "IniEx.h"
#include "def.h"
#include "DirFileEx.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	m_gRunConfig = GetRunDir() + "res/setting.ini";

	QString strVersion;
	QString strFileUrl;
	if (argc == 3)
	{
		strVersion = argv[1];
		strFileUrl = argv[2];
	}
	DcmClientUpdate updateeclass(strVersion, strFileUrl);
	updateeclass.ProcessCommand();
	return a.exec();
}
