#pragma once

#include <string>
#include <list>
#include "def.h"



class PUBLICLIB_EXPORT JsonRobot
{
private:
	JsonRobot();
	~JsonRobot();
	static JsonRobot* _value;
public:
	static JsonRobot *getJsonRobot();
	bool Json_Parse_ClientId(const QString strData, QString& clientId);
	bool Json_Parse_updateSetting(const QString strData);
	bool Json_Parse_ClientVersion(QString strData, QString& strVersion, QString& strForcedUpgradeVersion, QString&strUrl);

};

