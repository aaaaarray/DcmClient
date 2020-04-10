#pragma once
#include "publiclib_global.h"
class PUBLICLIB_EXPORT DateUtils
{
public:
	DateUtils();
	~DateUtils();
	static QString getZone();

	static QString getDateUTC8(const char* format = "yyyy-MM-dd");
	static QString getTimeUTC8(const char* format = "hh:mm:ss");
	static QString getDateTimeUTC8(const char* format = "yyyy-MM-dd hh:mm:ss");

	static QString getDate(const char* format = "yyyy-MM-dd");
	static QString getTime(const char* format = "hh:mm:ss.zzz");
	static QString getDateTime(const char* format = "yyyy-MM-dd hh:mm:ss.zzz");

	static uint getCurrentDateTime();
};

