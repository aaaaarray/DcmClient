#include "def.h"
#include <QLocale>

QString g_strResPath = "";
QString m_glanguage = ""; //QLocale::system().name();
QString m_gRunConfig= "";
int showMessageBox(QMessageBox::Icon type, QString strMsg)
{
	QMessageBox information;
	information.setIcon(type);
	information.setText(strMsg);
	information.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
	information.setButtonText(QMessageBox::Yes, LoadLanguageString("notify", "str001"));
	information.setButtonText(QMessageBox::No, LoadLanguageString("notify", "str002"));
	information.setDefaultButton(QMessageBox::Yes);
	return information.exec();
}