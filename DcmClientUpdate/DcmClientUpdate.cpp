#include "DcmClientUpdate.h"
#include "DirFileEx.h"
#include "IniEx.h"
#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "HttpRequestModel.h"
#include <Windows.h>
#include <shellapi.h>
#pragma comment(lib,"Shell32.lib")
#include "log.h"
DcmClientUpdate::DcmClientUpdate(QString strVersion, QString strFileUrl, QWidget *parent)
	: QWidget(parent)
	, m_strServerVersion(strVersion)
	, m_strFileUrl(strFileUrl)
{
	m_nOldProcess = 0;
	this->hide();
}

void DcmClientUpdate::ProcessCommand()
{
	{
		if (m_strServerVersion.isEmpty() || m_strFileUrl.isEmpty())
		{
			HttpRequestModel *m_httpRequestModel = HttpRequestModel::getHttpRequestModel();
			m_httpRequestModel->setApi(NULL);
			QString strForcedUpgradeVersion;
			if (!m_httpRequestModel->getVersion(m_strServerVersion, strForcedUpgradeVersion, m_strFileUrl))
			{
				log_info( "Get New ClientVersionInfo fail");
				exit(0);
			}
		}

		m_strFileUrl = m_strFileUrl.left(m_strFileUrl.length() - 3);
		m_strFileUrl += "exe";
		log_info("Get New ClientVersionInfo success,address= %s", m_strFileUrl.toStdString().c_str());
		m_strUpdatePath = Ex_GetTempDir() + "update/";
		if (CheckUpdate(m_strServerVersion))
		{
			QString path = m_strUpdatePath + "config.ini";
			QString strConfigVersion = ReadIniString("Update", "Version", path);
			QString strConfigHasDownload = ReadIniString("Update", "HasDownload", path);
			QString strConfigFilename = ReadIniString("Update", "Filename", path);
			QString strConfigPathAll = ReadIniString("Update", "FilePath", path);
			QString strGroupName = ReadIniString("Organization", "GroupName", m_gRunConfig);
			QFileInfo fileInfo(strConfigPathAll);
			if (strConfigVersion == m_strServerVersion
				&& strConfigHasDownload == "1"
				&& !strConfigPathAll.isEmpty()
				&& fileInfo.exists()
				)
			{
				log_info("%s .exe is already download,path=%s ", strGroupName.toStdString().c_str(), strConfigPathAll.toStdString().c_str());

				QString strKill = "taskkill /im " + strGroupName + ".exe /f";
				ShellExecute(NULL, strKill.toStdWString().c_str(), NULL, NULL, NULL, SW_HIDE);
				ShellExecute(NULL, L"open", strConfigPathAll.toStdWString().c_str(), NULL, NULL, SW_HIDE);
				exit(0);
			}
			else
			{
				log_info("begin update .......");
				DeleteDir(m_strUpdatePath);
				Ex_CreateDiretory(m_strUpdatePath);
				QDateTime time = QDateTime::currentDateTime();
				QString strTime = QString::number(time.toTime_t());
				WriteIniString("Update", "StartTime", strTime, path);
				manager = new QNetworkAccessManager(this);
				QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(m_strFileUrl)));
				connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
					SLOT(downloadProgress(qint64, qint64)));
				connect(manager, SIGNAL(finished(QNetworkReply*)),
					this, SLOT(httpDownloadFinished(QNetworkReply*)));
			}
		}
		else {
			exit(0);
		}
	}

	//exit(0);
}


bool DcmClientUpdate::CheckUpdate(QString strVersionRemote)
{
	QString strLocalVersion = ReadIniString("Version", "Version", m_gRunConfig);
	if (strLocalVersion.isEmpty() || strVersionRemote.isEmpty())
	{
		log_info("LocalVersion=%s, VersionRemote=%s", strLocalVersion.toStdWString().c_str(), strVersionRemote.toStdWString().c_str());
		return false;
	}

	QStringList strLocalVersionlist = strLocalVersion.split(".");
	QStringList strVersionRemotelist = strVersionRemote.split(".");
	for (int i = 0; i < 4; i++)
	{
		int r = strVersionRemotelist.at(i).toInt();
		int l = strLocalVersionlist.at(i).toInt();
		if (r > l)
		{
			return true;
		}
		else if (r < l)
			return false;
	}
	return false;
}

void DcmClientUpdate::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	int nProgress = bytesReceived * 100 / bytesTotal;
	if (m_nOldProcess == nProgress)
	{
		return;
	}
	m_nOldProcess = nProgress;
	log_info("%d", nProgress);
}


bool DcmClientUpdate::isHttpRedirect(QNetworkReply *reply)
{
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	return statusCode == 301 || statusCode == 302 || statusCode == 303
		|| statusCode == 305 || statusCode == 307 || statusCode == 308;
}
void DcmClientUpdate::httpDownloadFinished(QNetworkReply *reply)
{
	QUrl url = reply->url();
	if (reply->error()) {
		log_info("Download of %s failed:%s ", url.toEncoded().constData(), reply->errorString().toStdString().c_str());
	}
	else {
		if (isHttpRedirect(reply)) {
			log_info("Download Request was redirected.");
		}
		else {
			QString filename = saveFileName(url);
			if (saveToDisk(filename, reply)) {
				log_info("Download of %s succeeded (saved to %s)", url.toEncoded().constData(), filename.toStdString().c_str());
			}
		}
	}
	reply->deleteLater();
	exit(0);
}
QString DcmClientUpdate::saveFileName(const QUrl &url)
{
	QString path = url.path();
	QString basename = QFileInfo(path).fileName();

	if (basename.isEmpty())
		basename = "download";

	if (QFile::exists(basename)) {
		int i = 0;
		basename += '.';
		while (QFile::exists(basename + QString::number(i)))
			++i;
		basename += QString::number(i);
	}

	return basename;
}

bool DcmClientUpdate::saveToDisk(const QString &filename, QIODevice *data)
{
	QString filePath = m_strUpdatePath + filename;
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly))
	{
		log_info("Could not open %s for writing: %s", filePath.toStdString().c_str(), file.errorString().toStdString().c_str());
		return false;
	}

	file.write(data->readAll());
	file.close();
	WriteIniString("Update", "Version", m_strServerVersion, m_strUpdatePath + "config.ini");
	WriteIniString("Update", "Name", filename, m_strUpdatePath + "config.ini");
	WriteIniString("Update", "Path", filePath, m_strUpdatePath + "config.ini");
	return true;
}

