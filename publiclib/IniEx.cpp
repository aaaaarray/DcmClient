#include "IniEx.h"
#include <QSettings>
#include "DirFileEx.h"
#include "def.h"

QString g_strLangagePath = "";

class IniInitAPP
{
public:
	IniInitAPP();
	~IniInitAPP();
};

IniInitAPP::IniInitAPP()
{
	QString strConfigPath = GetRunDir();

	//目前只支持中英两个版本
	if (m_glanguage == "zh_CN")//简体中文
		g_strLangagePath = strConfigPath + "res/lang_zh_CN.ini";
	else if (m_glanguage == "en_US")
		g_strLangagePath = strConfigPath + "res/lang_en_US.ini";
	else if (m_glanguage == "ko_KR")
		g_strLangagePath = strConfigPath + "res/lang_ko_KR.ini";
	else
		g_strLangagePath = strConfigPath + "res/lang_en_US.ini";
#ifdef QT_DEBUG
	g_strLangagePath.replace("Debug", "Release");
#endif
}

IniInitAPP::~IniInitAPP()
{
}

void WriteIniString(QString strGroup, QString strKey, QString strValue, QString strPath)
{
	QSettings settings(strPath, QSettings::IniFormat);
	settings.setIniCodec("UTF8");
	settings.beginGroup(strGroup);
	settings.setValue(strKey, strValue);
	settings.endGroup();
}
QString ReadIniString(QString strGroup, QString strKey, QString strPath)
{
	QSettings settings (strPath, QSettings::IniFormat);
	settings.setIniCodec("UTF8");
	return settings.value(strGroup + "/" + strKey,"").toString();
}

QString LoadLanguageString(QString strGroup, QString strKey)
{
	static IniInitAPP g_IniInitAPP;

	QSettings settings (g_strLangagePath, QSettings::IniFormat);
	settings.setIniCodec("UTF8");
	//settings.setIniCodec("GBK");
	return settings.value(strGroup + "/" + strKey, "").toString();
}