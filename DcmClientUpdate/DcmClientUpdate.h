#pragma once

#include <QWidget>
#include <QNetworkAccessManager>

class DcmClientUpdate : public QWidget
{
	Q_OBJECT

public:
	DcmClientUpdate(QString strVersion, QString strFileUrl, QWidget *parent = NULL);

	void ProcessCommand();
private slots:
	void httpDownloadFinished(QNetworkReply *reply);
	void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
private:
	int m_nOldProcess;
	QString m_strFileUrl;
	QString m_strServerVersion;
	QString m_strUpdatePath;
	bool isHttpRedirect(QNetworkReply *reply);
	static QString saveFileName(const QUrl &url);
	bool saveToDisk(const QString &filename, QIODevice *data);
	bool CheckUpdate(QString strVersionRemote);
	QNetworkAccessManager *manager;

};
