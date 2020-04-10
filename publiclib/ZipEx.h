#pragma once
#ifndef __ZIPEX_H__
#define __ZIPEX_H__

#include "zip.h"
#include "publiclib_global.h"
class PUBLICLIB_EXPORT CZipEx
{
public:
	CZipEx();
	~CZipEx();

	void CreateZip(const TCHAR *fn, const char *password);
	void ZipAdd(const TCHAR *dstzn, const TCHAR *fn);
	void CloseZipEx();
private:
	HZIP m_Hzip;
};

#endif