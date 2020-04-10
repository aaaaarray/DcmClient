#include "AutoDump.h"
#include "log.h"
#include <tchar.h>
#include <QString>
inline bool CALLBACK MiniDumpCallback(PVOID         pParam,
	const PMINIDUMP_CALLBACK_INPUT   pInput,
	PMINIDUMP_CALLBACK_OUTPUT        pOutput)

{
	bool bRet = false;
	char szEchoStr[MAX_PATH];
	char szModulePath[MAX_PATH];
	if ((!pInput) || (!pOutput))
		return false;
	//
	switch (pInput->CallbackType)
	{
	case ModuleCallback:
		WideCharToMultiByte(
			CP_ACP,
			WC_COMPOSITECHECK,
			pInput->Module.FullPath,
			-1,
			szModulePath,
			MAX_PATH,
			NULL,
			NULL
		);
		log_info("szModulePath:%s", QString::fromWCharArray(pInput->Module.FullPath).toStdString().c_str());
		bRet = true;
		break;
	case IncludeModuleCallback:
		bRet = true;
		break;
	case ThreadCallback:
		bRet = true;
		break;
	case IncludeThreadCallback:
		bRet = true;
		break;
	case MemoryCallback:
		bRet = true;
		break;
	}

	return bRet;
}

//创建Dump文件 
void CreateMiniDump(EXCEPTION_POINTERS* pep, LPCTSTR strFileName)
{
	log_info("fileName:%s", QString::fromWCharArray(strFileName).toStdString().c_str());

	DWORD	nRetCode = 0;
	HMODULE	hModule = NULL;
	MINIDUMP_TYPE mdtype = MiniDumpNormal;

	HANDLE hFile = CreateFile(strFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei;
		mdei.ThreadId = GetCurrentThreadId();
		mdei.ExceptionPointers = pep;
		mdei.ClientPointers = FALSE;

		MINIDUMP_CALLBACK_INFORMATION mci;
		mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;
		mci.CallbackParam = 0;
		MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)0x0000ffff;

		pMiniDumpWriteDump	MiniDumpWriteDump = NULL;
		hModule = LoadLibrary(_T("DbgHelp.dll"));
		if (hModule == NULL)
		{
			nRetCode = GetLastError();
			//sprintf(szErrStr, "LoadLibrary: %x\n", nRetCode);
			goto EXIT;
		}
		MiniDumpWriteDump = (pMiniDumpWriteDump)GetProcAddress(hModule, "MiniDumpWriteDump");
		if (!MiniDumpWriteDump)
		{
			nRetCode = GetLastError();
			//sprintf(szErrStr, "GetProcAddress: %x\n", nRetCode);
			log_error("GetProcAddress MiniDumpWriteDump error :%d", nRetCode);
			goto EXIT;
		}
		//
		nRetCode = MiniDumpWriteDump(
			GetCurrentProcess(),	// hProcess
			GetCurrentProcessId(),	// ProcessId
			hFile,				// HANDLE
			mdtype,
			&mdei,					// PMINIDUMP_EXCEPTION_INFORMATION
			NULL,					// PMINIDUMP_USER_STREAM_INFORMATION
			&mci					// PMINIDUMP_CALLBACK_INFORMATION
		);
		if (!nRetCode)
		{
			nRetCode = GetLastError();
			//sprintf(szErrStr, "MiniDumpWriteDump: %x\n", nRetCode);
			log_error("MiniDumpWriteDump error :%d", nRetCode);
			goto EXIT;
		}

		CloseHandle(hFile);
	}

EXIT:
	if (hFile)
		CloseHandle(hFile);
	if (hModule)
		FreeLibrary(hModule);
}