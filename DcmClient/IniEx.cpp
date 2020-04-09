#include "IniEx.h"
#include <QSettings>
#include <QtWidgets/QApplication>
QString g_strLangagePath = "";
QString m_glanguage = "zh_CN";
IniEx::IniEx()
{
	QString strConfigPath = QApplication::applicationDirPath() + "/";;

	//目前只支持中英两个版本
	if (m_glanguage == "zh_CN")//简体中文
		g_strLangagePath = strConfigPath + "lang_sc.ini";
	else
		g_strLangagePath = strConfigPath + "lang_en.ini";
#ifdef QT_DEBUG
	//g_strLangagePath.replace("Debug", "Release");
#endif
}


IniEx::~IniEx()
{
}
QString LoadLanguageString(QString strGroup, QString strKey)
{
	static IniEx g_IniInitAPP;

	QSettings settings(g_strLangagePath, QSettings::IniFormat);
	settings.setIniCodec("UTF8");
	return settings.value(strGroup + "/" + strKey, "").toString();
}