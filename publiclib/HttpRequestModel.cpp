#include "HttpRequestModel.h"
#include "DateUtils.h"
#include <QDir>
#include <map>
#include "DirFileEx.h"
#include "Base64Ex.h"
#include "log.h"
#include "IniEx.h"
#include "ZipEx.h"
#include <iphlpapi.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QFile>
#include <QHttpMultiPart>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

using namespace std;

HttpRequestModel* HttpRequestModel::_value = 0;
HttpRequestModel* HttpRequestModel::getHttpRequestModel()
{
	if (_value == 0)
	{
		_value = new HttpRequestModel();
	}
	return _value;
}
HttpRequestModel::HttpRequestModel()
{
	m_strHttpApi = "";
	m_strHttpApi_Back = "";
	

	g_szCurGroupID = ReadIniString("Organization", "GroupID", m_gRunConfig);
	m_strAPIVersion = "";
	m_strAPIVersion = ReadIniString("APIVersion", "Version", m_gRunConfig);
	if (m_strAPIVersion.isEmpty())
	{
		m_strAPIVersion = "2.2";
	}
	setApi(NULL);
}
void HttpRequestModel::setApi(QString api)
{
	if (api != NULL&&api != "")
	{
		WriteIniString("http", "api", api, Ex_GetRoamingDir() + "config.ini");
	}
	else {
		api = ReadIniString("http", "api", m_gRunConfig);
	}
	m_strHttpApi = api;
	m_strHttpApi_Back = api;
}


HttpRequestModel::~HttpRequestModel()
{

}


QString HttpRequestModel::getDateEncrpt()
{
	QString strDate = DateUtils::getDateTimeUTC8();
	strDate = g_szCurGroupID + "+" + strDate;
	QString key = QString::fromStdString(eclassEncrypt(strDate.toStdString().c_str(), S1_KEY_ECLASS_CLIENT));
	return key;
}

bool HttpRequestModel::InitClient(QString orgId, QString orgName, QString dataDir, QString& clientId, QString &api)
{
	QString mac;
	getMacByGetAdaptersInfo( mac);
	QString strLocalVersion = ReadIniString("Version", "Version", m_gRunConfig);
	QDateTime local(QDateTime::currentDateTime());
	QString localTime = local.toString("yyyyMMddhhmmss");
	QJsonObject json;
	if (clientId != NULL&&clientId != ""){
		json.insert("clientId", clientId);
	}
	json.insert("orgId", orgId);
	json.insert("orgName", orgName);
	json.insert("dataDir", dataDir);
	json.insert("mac", mac);
	json.insert("version", strLocalVersion);
	json.insert("time", localTime);
	QString key = "orgId=" + orgId + "&time=" + localTime;
	json.insert("key", Md5(key));
	qDebug()<< "------------>   "<<QString(QJsonDocument(json).toJson());
	QString response_data;
	int ret = postJsonEx("/base/init", json, response_data);
	if (ret != 0)
	{
		return false;
	}
	QJsonParseError json_error;
	QByteArray bytes = response_data.toUtf8();
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
			api = root["api"].toString();
			return true;
		}
	}
	return true;
}

bool HttpRequestModel::updateSetting(QString orgId, QString orgName, QString dataDir, QString clientId){
	QString mac;
	getMacByGetAdaptersInfo(mac);
	QJsonObject json;
	json.insert("orgId", orgId);
	json.insert("orgName", orgName);
	json.insert("dataDir", dataDir);
	json.insert("clientId", clientId);
	json.insert("mac", mac);

	QString response_data;
	int ret = postJsonEx("live/getMediaChannelKey", json, response_data);
	if (ret != 0)
	{
		return false;
	}

	if (!jr->Json_Parse_updateSetting(response_data))
	{
		log_error("Parse fail.");
		return false;
	}

	log_info("Success.");
	return true;
}
bool HttpRequestModel::PackageLog(int nUserID, int nRoomID, bool bCoredump)
{
	QString strMac = "";
	if (!getMacByGetAdaptersInfo(strMac))
	{
		log_error("Get MAC address failed.");
		return false;
	}
	QString groupName = ReadIniString("Organization", "GroupName", m_gRunConfig);
	QString current_date = DateUtils::getDateTimeUTC8("yyyy-MM-dd-hh-mm-ss");
	QString strFilePath = GetLogDir();
	QString strZipName;
	if (bCoredump)
		strZipName = QString("coredump_%1_%2_%3_%4.zip").arg(nRoomID).arg(nUserID).arg(current_date).arg(strMac);
	else
		strZipName = QString("%1_%2_%3_%4.zip").arg(nRoomID).arg(nUserID).arg(current_date).arg(strMac);
	QString strZipPath = strFilePath+ strZipName;
	CZipEx zip;
	zip.CreateZip(strZipPath.toStdWString().c_str(), NULL);
	QDir logPath(strFilePath);
	if (!logPath.exists())
	{
		return false;
	}
	logPath.setFilter(QDir::Files);
	QFileInfoList filelist = logPath.entryInfoList();
	for (auto item:filelist)
	{
		QFileInfo file = item;
		QString strtmpName = file.fileName();
		if (file.fileName() == "." || file.fileName() == ".." || file.suffix().toLower() == "zip" || file.fileName() == "agora_tmp.log" || file.fileName() == (DateUtils::getDateUTC8("yyyy.MM.dd") + "_tmp.log"))
		{
			continue;
		}
		if (bCoredump)
		{
			if (file.fileName() == "agora.log")
			{
				QString strAgoraCopy = GetLogDir() + "agora_copy.log";
				QFile::copy(item.filePath(), strAgoraCopy);
				QFileInfo fileinfo(strAgoraCopy);
				zip.ZipAdd(fileinfo.fileName().toStdWString().c_str(), strAgoraCopy.toStdWString().c_str());
				QFile::remove(strAgoraCopy);
				log_info("file = %s", strAgoraCopy.toStdString().c_str());
				continue;
			}
			else if (file.fileName() == "MiniDump.dmp")
			{
				QString strMiniDump = GetLogDir() + "MiniDump.dmp";
				QFile::copy(item.filePath(), strMiniDump);
				QFileInfo fileinfo(strMiniDump);
				zip.ZipAdd(fileinfo.fileName().toStdWString().c_str(), strMiniDump.toStdWString().c_str());
				QFile::remove(strMiniDump);
				log_info("file = %s", strMiniDump.toStdString().c_str());
				continue;
			}
		}
		QString descFile = item.filePath();
		try {
			zip.ZipAdd(strtmpName.toStdWString().c_str(), descFile.toStdWString().c_str());
		}
		catch (exception e) {
			log_error("%s", e.what());
		}
		
		bool bRet = QFile::remove(item.filePath());
		log_info("file = %s ,remove= %s", item.filePath().toStdString().c_str(), bRet == true ? "true" : "false");
		if (!bRet)
		{
			bRet = QFile::remove(item.filePath());
			log_info("file = %s ,remove= %s", item.filePath().toStdString().c_str(), bRet == true ? "true" : "false");
		}			
	}
	zip.CloseZipEx();
	
	if (sendRequest(nUserID, nRoomID, strZipPath))
		return true;
	return false;
}

bool HttpRequestModel::uploadFile(QString filePath, QString fileHash)
{
	QString orgId = ReadIniString("client", "orgId", Ex_GetRoamingDir() + "config.ini");
	QString orgName = ReadIniString("client", "orgName", Ex_GetRoamingDir() + "config.ini");
	QString clientId = ReadIniString("client", "clientId", Ex_GetRoamingDir() + "config.ini");
	QString mac;

	getMacByGetAdaptersInfo(mac);
	
	
	QDateTime local(QDateTime::currentDateTime());
	QString localTime = local.toString("yyyyMMddhhmmss");
		
	QFileInfo file = QFileInfo(filePath);
	std::map<QString, QString> params;
	params.insert(std::pair<QString, QString>("orgId", orgId));
	params.insert(std::pair<QString, QString>("orgName", orgName));
	params.insert(std::pair<QString, QString>("clientId", clientId));	
	params.insert(std::pair<QString, QString>("fileName", file.fileName()));
	params.insert(std::pair<QString, QString>("mac", mac));
	params.insert(std::pair<QString, QString>("hash", fileHash));
	params.insert(std::pair<QString, QString>("time", localTime));

	//http://127.0.0.1:8011/wylm/portal/file/store
	QString host = m_strHttpApi + "/file/store";
	QString strSlaveUrl = m_strHttpApi_Back + "/file/store";
	QString response_data;
	if (postfile(host, params, filePath, file.fileName(), response_data) != 0)
	{
		if (postfile(strSlaveUrl, params, filePath, file.fileName(), response_data) != 0)
			return false;
	}
	QJsonParseError json_error;
	QByteArray bytes = response_data.toUtf8();
	QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);
	if (json_error.error != QJsonParseError::NoError)
	{
		return false;
	}
	if (parse_doucment.isObject())
	{
		QJsonObject root = parse_doucment.object();
		if (root["code"].toString() == "0000" || root["code"].toString() == "DCM002"){
			return true;
		}
	}
	return false;
}
bool HttpRequestModel::sendRequest(const int nUserID, const int nRoomID, const QString zipfile)
{
	QFileInfo file = QFileInfo(zipfile);
	std::map<QString,QString> params;
	//params.insert(std::pair<QString, QString>("Version", m_strAPIVersion));
	params.insert(std::pair<QString, QString>("UA", "Windows"));
	//params.insert(std::pair<QString, QString>("Source", "windows_log"));
	params.insert(std::pair<QString, QString>("UserID", QString::number(nUserID)));
	params.insert(std::pair<QString, QString>("RoomID", QString::number(nRoomID)));
	params.insert(std::pair<QString, QString>("FileName", file.fileName()));
	params.insert(std::pair<QString, QString>("VerificationKey", getDateEncrpt()));

	QString host = m_strHttpApi + "live/LogUpload";
	QString strSlaveUrl = m_strHttpApi_Back + "live/LogUpload";
	QString response_data;
	if (postfile(host, params, zipfile, file.fileName(), response_data) != 0)
	{
		if (postfile(strSlaveUrl, params, zipfile, file.fileName(), response_data) != 0)
			return false;
	}
	/*if (jr->Json_Parse_LogUpload(response_data))
	{
		QFile::remove(zipfile);
		return true;
	}*/
	return false;
}

bool HttpRequestModel::getMacByGetAdaptersInfo(QString& macOUT)
{
	bool ret = false;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL)
		return false;
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		if (pAdapterInfo == NULL)
			return false;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
	{
		for (PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
		{
			// È·±£ÊÇÒÔÌ«Íø
			if (pAdapter->Type != MIB_IF_TYPE_ETHERNET && pAdapter->Type != IF_TYPE_IEEE80211)
				continue;
			// È·±£MACµØÖ·µÄ³¤¶ÈÎª 00-00-00-00-00-00
			if (pAdapter->AddressLength != 6)
				continue;
			char acMAC[32];
			sprintf(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
				int(pAdapter->Address[0]),
				int(pAdapter->Address[1]),
				int(pAdapter->Address[2]),
				int(pAdapter->Address[3]),
				int(pAdapter->Address[4]),
				int(pAdapter->Address[5]));
			macOUT = acMAC;
			ret = true;
			break;
		}
	}

	free(pAdapterInfo);
	return ret;
}

QString HttpRequestModel::Md5(QString value)
{
	QString md5;
	QByteArray ba, bb;
	QCryptographicHash md(QCryptographicHash::Md5);
	ba.append(value);
	md.addData(ba);
	bb = md.result();
	md5.append(bb.toHex());
	return md5;
}

bool HttpRequestModel::getVersion(QString &strVersion, QString& strForcedUpgradeVersion, QString &cstrUrl)
{
	QJsonObject json;
	json.insert("UA", "Windows");
	json.insert("VerificationKey", getDateEncrpt());

	QString response_data;
	int ret = postJsonEx("/base/getClientVersion", json, response_data);
	if (ret != 0)
	{
		return false;
	}

	strVersion = "";
	bool bResult = jr->Json_Parse_ClientVersion(response_data, strVersion, strForcedUpgradeVersion, cstrUrl);
	if (!bResult)
	{
		log_error("Parse client version json error");
		return false;
	}
	log_info("Parse client version json success, Version = %s", strVersion.toStdString().c_str());
	return true;
}

int HttpRequestModel::postJson(const QString host, const QJsonObject json, QString& response_data)
{
	log_info("host = %s", host.toStdString().c_str());
	
	QUrl url(host);
	QJsonDocument document;
	document.setObject(json);
	QByteArray dataArray = document.toJson(QJsonDocument::Compact);
	log_info("req data = %s", dataArray.toStdString().c_str());
	QTimer timer;
	timer.setInterval(5000);
	timer.setSingleShot(true);
	// ¹¹ÔìÇëÇó
	QNetworkRequest request;
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setUrl(url);
	QEventLoop waitLoop;
	QNetworkAccessManager* connection = new QNetworkAccessManager();
	QNetworkReply* reply = connection->post(request, dataArray);
	QObject::connect(&timer, SIGNAL(timeout()), &waitLoop, SLOT(quit()));
	QObject::connect(reply, SIGNAL(finished()), &waitLoop, SLOT(quit()));
	timer.start();
	waitLoop.exec();
	int errorCode = -1;
	if (timer.isActive())
	{
		timer.stop();
		errorCode = reply->error();
		if (errorCode != 0) {
			log_error("post %s fail ,errorCode = %d", host.toStdString().c_str(), errorCode);
		}
		else {
			response_data = reply->readAll();
			log_info("rsp = %s", response_data.toStdString().c_str());
		}
	}
	else
	{
		log_error("post %s timeout", host.toStdString().c_str());
		QObject::disconnect(reply, &QNetworkReply::finished, &waitLoop, &QEventLoop::quit);
		reply->abort();
		reply->deleteLater();
	}

	delete reply;
	delete connection;
	return errorCode;
}

int HttpRequestModel::postJsonEx(const QString host, const QJsonObject json, QString& response_data)
{
	int nRet = -1;
	if ((nRet = postJson(m_strHttpApi + host, json, response_data)) != 0)
	{
		if ((nRet = postJson(m_strHttpApi_Back + host, json, response_data)) != 0)
		{
			log_info("%s postJson fail nRet=%d", host.toStdString().c_str(), nRet);
			return nRet;
		}
	}
	return nRet;
}

int HttpRequestModel::post(const QString host, const QString data, QString& response_data)
{
	log_info("host = %s", host.toStdString().c_str());
	log_info("req data = %s", data.toStdString().c_str());
	QTimer timer;
	timer.setInterval(5000);
	timer.setSingleShot(true);
	QNetworkRequest request(host);

	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	request.setHeader(QNetworkRequest::ContentLengthHeader, data.length());

	QEventLoop waitLoop;
	QNetworkAccessManager* connection = new QNetworkAccessManager();
	QNetworkReply* reply = connection->post(request, data.toLatin1());
	QObject::connect(&timer, SIGNAL(timeout()), &waitLoop, SLOT(quit()));
	QObject::connect(reply, SIGNAL(finished()), &waitLoop, SLOT(quit()));
	timer.start();
	waitLoop.exec();
	int errorCode = -1;
	if (timer.isActive())
	{
		timer.stop();
		errorCode = reply->error();
		if (errorCode != 0) {
			log_error("post %s fail ,errorCode = %d", host.toStdString().c_str(), errorCode);
		}
		else {
			response_data = reply->readAll();
			log_info("rsp = %s", response_data.toStdString().c_str());
		}
	}
	else
	{
		log_error("post %s timeout", host.toStdString().c_str());
		QObject::disconnect(reply, &QNetworkReply::finished, &waitLoop, &QEventLoop::quit);
		reply->abort();
		reply->deleteLater();
	}

	delete reply;
	delete connection;
	return errorCode;
}

int HttpRequestModel::postEx(const QString api, QString data, QString& response_data)
{
	QString host = m_strHttpApi + api;
	QString strSlaveUrl = m_strHttpApi_Back + api;
	data.replace("/", "%2f");
	data.replace("+", "%2b");
	data.replace(" ", "%20");
	int nRet = -1;
	if ((nRet = post(host, data, response_data)) != 0)
	{
		if ((nRet = post(strSlaveUrl, data, response_data)) != 0)
		{
			log_info("%s post fail nRet=%d", host.toStdString().c_str(), nRet);
			return nRet;
		}
	}
	return nRet;
}

int HttpRequestModel::postfile(const QString host, std::map<QString, QString> params, QString filepath, QString fileName, QString& response_data, QString strType)
{
	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	//multiPart->setBoundary("----WebKitFormBoundaryCJsAP52jsafp27FY");
	std::map<QString, QString>::iterator it = params.begin();
	for (it = params.begin(); it != params.end(); ++it)
	{
		QHttpPart childPart;
		childPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data;name=\"" + it->first + "\""));
		childPart.setBody(it->second.toLatin1());
		multiPart->append(childPart);
	}
	QHttpPart filePart;
	filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data;name=\"file\";filename=\"" + fileName.toLatin1() + "\""));
	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly))
		return -1;
	filePart.setBodyDevice(&file);
	file.setParent(multiPart);
	multiPart->append(filePart);

	QNetworkAccessManager *connection = new QNetworkAccessManager;
	QNetworkRequest request(QUrl(host.toStdString().c_str()));
	QNetworkReply *reply = connection->post(request, multiPart);
	QEventLoop waitLoop;
	QObject::connect(reply, SIGNAL(finished()), &waitLoop, SLOT(quit()));
	waitLoop.exec();
	int errorCode = reply->error();
	if (errorCode != 0) {
		log_error("error %d %s", errorCode, reply->errorString().toStdString().c_str());
	}
	else {
		response_data = reply->readAll();
	}
	delete reply;
	delete connection;
	return errorCode;
}