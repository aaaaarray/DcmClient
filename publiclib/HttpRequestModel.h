#pragma once
#include "publiclib_global.h"
#include "def.h"
#include "JsonRobot.h"
#include <map>


class PUBLICLIB_EXPORT HttpRequestModel
{
private:
	HttpRequestModel();
	~HttpRequestModel();
	static HttpRequestModel* _value;
	QString g_szCurGroupID;
public:
	static HttpRequestModel *getHttpRequestModel();
	void setApi(QString api);
	bool getVersion(QString &strVersion, QString& strForcedUpgradeVersion, QString &cstrUrl);
	bool getClientId(QString orgId, QString orgName, QString dataDir, QString& clientId);
	bool updateSetting(QString orgId, QString orgName, QString dataDir, QString clientId);


	bool PackageLog(int nUserID, int nRoomID, bool bCoredump = false);
	
	bool getMacByGetAdaptersInfo(QString& macOUT);
private:
	bool StartStopAgoraRecording(int nRoomID, int nUserID, bool bStart);
	bool sendRequest(const int nUserID, const int nRoomID, const QString zipfile);
	QString m_strSettingIniPath;
	
	QString getDateEncrpt();
	QString m_strAPIVersion;

	QString m_strHttpApi;//api主地址
	QString m_strHttpApi_Back; //api备用url
	
	JsonRobot *jr = JsonRobot::getJsonRobot();
	int post(const QString host, const QString data, QString& response_data);
	int postEx(const QString api, QString data, QString& response_data);

	int postfile(const QString host, std::map<QString, QString> params,
		QString filepath, QString fileName, QString& response_data, QString strType = "zip");
	int postJson(const QString host, const QJsonObject json, QString& response_data);
	int postJsonEx(const QString host, const QJsonObject json, QString& response_data);
};
