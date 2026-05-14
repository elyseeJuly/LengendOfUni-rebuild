#include "StdAfx.h"
#include "CrashDump.h"
#include <DbgHelp.h>

CCrashDump::CCrashDump(void)
{
}

CCrashDump::~CCrashDump(void)
{
}

BOOL CCrashDump::RecordExceptionInfo(const PEXCEPTION_POINTERS &excpInfo)
{
	CString sFileName;
	CTime timeData = CTime::GetCurrentTime();
	CString sTime;
	sTime = timeData.Format(_T("%Y%m%d%H%M%S.dmp"));

	HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
	TCHAR lpFn[255];   
	::GetModuleFileName(hModule, lpFn, 255);
	sFileName = lpFn;
	int iPos = sFileName.ReverseFind('\\');
	sFileName = sFileName.Left(iPos + 1);

	CString sShortFileName;
	sShortFileName = _T("dump_");
	sShortFileName += sTime;
	sFileName += sShortFileName;

	// Create the file
	HANDLE hMiniDumpFile = CreateFile(
		sFileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
		NULL);

	// Write the minidump to the file
	if (hMiniDumpFile != INVALID_HANDLE_VALUE)
	{
		DumpMiniDump(hMiniDumpFile, excpInfo);

		// Close file
		CloseHandle(hMiniDumpFile);
		CString sInfo;
		sInfo.Format(_T("很抱歉，“刘慈欣群星传”因为某种原因发生错误，即将关闭。\n\n我希望您能将游戏目录下的 “%s” 文件发送给我，\
以便我能找到并修复这个bug。我的邮箱是 ourshow@tom.com\n\n衷心感谢 :-)"), sShortFileName);
		AfxMessageBox(sInfo, MB_ICONERROR);
		return TRUE;
	}

	return FALSE;
}

void CCrashDump::DumpMiniDump(HANDLE hFile, PEXCEPTION_POINTERS excpInfo)
{
	if (excpInfo == NULL) 
	{
		// Generate exception to get proper context in dump
		__try 
		{
			OutputDebugString(_T("raising exception\r\n"));
			RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL);
		} 
		__except(DumpMiniDump(hFile, GetExceptionInformation()),
			EXCEPTION_CONTINUE_EXECUTION) 
		{
		}
	} 
	else
	{
		OutputDebugString(_T("writing minidump\r\n"));
		MINIDUMP_EXCEPTION_INFORMATION eInfo;
		eInfo.ThreadId = GetCurrentThreadId();
		eInfo.ExceptionPointers = excpInfo;
		eInfo.ClientPointers = FALSE;

		// note:  MiniDumpWithIndirectlyReferencedMemory does not work on Win98
		MiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			hFile,
			MiniDumpNormal,
			excpInfo ? &eInfo : NULL,
			NULL,
			NULL);
	}
}
