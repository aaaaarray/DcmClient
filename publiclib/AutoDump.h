#pragma once
#include "publiclib_global.h"
//#include "def.h"
//#include <imagehlp.h> 

#include <stdlib.h> 
#include <windows.h>

#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")  


//typedef BOOL(WINAPI *PGetModuleHandleEx)(DWORD dwFlags, LPCTSTR lpModuleName, HMODULE *phModule);
//VOID CreateDump(struct _EXCEPTION_POINTERS *pExceptionPointers);



typedef bool(WINAPI *pMiniDumpWriteDump)(
	HANDLE,	// hProcess
	DWORD,	// hThreadID
	HANDLE,	// hFile
	MINIDUMP_TYPE,
	PMINIDUMP_EXCEPTION_INFORMATION,
	PMINIDUMP_USER_STREAM_INFORMATION,
	PMINIDUMP_CALLBACK_INFORMATION
	);

inline bool CALLBACK MiniDumpCallback(PVOID pParam,
	const PMINIDUMP_CALLBACK_INPUT   pInput,
	PMINIDUMP_CALLBACK_OUTPUT        pOutput);
void PUBLICLIB_EXPORT CreateMiniDump(EXCEPTION_POINTERS* pep, LPCTSTR strFileName);