#include "DcmClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(false);
	DcmClient *window = new DcmClient;
	Q_UNUSED(window);
	return a.exec();
}
