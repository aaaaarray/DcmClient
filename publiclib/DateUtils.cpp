#include "DateUtils.h"
#include<QDateTime>
#include<QTimeZone>
#include<QLocale.h>
DateUtils::DateUtils()
{
}


DateUtils::~DateUtils()
{
}

QString DateUtils::getZone()
{
	QDateTime currentTime = QDateTime::currentDateTime();
	uint timeT = currentTime.toTime_t();
	QTimeZone currentZone = currentTime.timeZone();
	QString currentZoneId = currentZone.id();
	
	return currentZone.comment();
}

QString DateUtils::getDateUTC8(const char* format)
{
	QDateTime currentTime = QDateTime::currentDateTime();
	QTimeZone currentZone = currentTime.timeZone();
	QString comment = currentZone.comment();
	QString utc = comment.mid(comment.indexOf("+") + 1, 5);
	QStringList utcarr = utc.split(":");
	uint t = currentTime.toTime_t() + ((8 - utcarr.at(0).toInt()) * 60 - utcarr.at(1).toInt()) * 60;
	return QDateTime::fromTime_t(t).toString(format);
}

QString DateUtils::getTimeUTC8(const char* format)
{
	QDateTime currentTime = QDateTime::currentDateTime();
	QTimeZone currentZone = currentTime.timeZone();
	QString comment = currentZone.comment();
	QString utc = comment.mid(comment.indexOf("+") + 1, 5);
	QStringList utcarr = utc.split(":");
	uint t = currentTime.toTime_t() + ((8 - utcarr.at(0).toInt()) * 60 - utcarr.at(1).toInt()) * 60;
	return QDateTime::fromTime_t(t).toString(format);
}

QString DateUtils::getDateTimeUTC8(const char* format)
{
	QDateTime currentTime = QDateTime::currentDateTime();
	QTimeZone currentZone = currentTime.timeZone();
	QString comment = currentZone.comment();
	QString utc = comment.mid(comment.indexOf("+") + 1, 5);
	QStringList utcarr = utc.split(":");
	uint t = currentTime.toTime_t() + ((8 - utcarr.at(0).toInt()) * 60 - utcarr.at(1).toInt()) * 60;
	return QDateTime::fromTime_t(t).toString(format);
}

QString DateUtils::getDate(const char* format)
{
	QDate date = QDate::currentDate();
	return date.toString(format);
}

QString DateUtils::getTime(const char* format)
{
	QTime time = QTime::currentTime();
	return time.toString(format);
}

QString DateUtils::getDateTime(const char* format)
{
	QDateTime currentTime = QDateTime::currentDateTime();
	return currentTime.toString(format);
}

uint DateUtils::getCurrentDateTime()
{
	QDateTime time = QDateTime::currentDateTime();
	return time.toTime_t();
}