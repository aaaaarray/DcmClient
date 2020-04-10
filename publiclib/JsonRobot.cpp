#include "JsonRobot.h"
#include "Base64Ex.h"
#include <assert.h>
#include "log.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include "DateUtils.h"
#include <QColor>
#include <QRect>
#include <QPoint>
#include <QFont>
using namespace std;
JsonRobot* JsonRobot::_value = 0;
JsonRobot* JsonRobot::getJsonRobot()
{
	if (_value == 0)
	{
		_value = new JsonRobot();
	}

	return _value;
}
JsonRobot::JsonRobot()
{
}


JsonRobot::~JsonRobot()
{
}


bool JsonRobot::Json_Parse_ClientVersion(QString strData, QString& strVersion, QString& strForcedUpgradeVersion, QString&strUrl)
{
	QJsonParseError json_error;
	QByteArray bytes = strData.toUtf8();
	QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);
	if (json_error.error != QJsonParseError::NoError)
	{
		return false;
	}
	if (parse_doucment.isObject())
	{
		QJsonObject root = parse_doucment.object();
		if (!root["Result"].toBool())
			return false;
		strVersion = root["Version"].toString();
		strForcedUpgradeVersion = root["ForcedUpgrade"].toString();
		strUrl = root["Url"].toString();
		return true;
	}
	return false;
}

bool JsonRobot::Json_Parse_ClientId(const QString strData, QString& clientId)
{
	QJsonParseError json_error;
	QByteArray bytes = strData.toUtf8();
	QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);
	if (json_error.error != QJsonParseError::NoError)
	{
		return false;
	}
	if (parse_doucment.isObject())
	{
		QJsonObject root = parse_doucment.object();
		if (root["code"].toString() == "0000"){
			clientId = root["clientId"].toString();
			return true;
		}
	}
	return false;	
}

bool JsonRobot::Json_Parse_updateSetting(const QString strData){
	QJsonParseError json_error;
	QByteArray bytes = strData.toUtf8();
	QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);
	if (json_error.error != QJsonParseError::NoError)
	{
		return false;
	}
	if (parse_doucment.isObject())
	{
		QJsonObject root = parse_doucment.object();
		if (root["code"].toString() == "0000"){
			return true;
		}
	}
	return false;
}

