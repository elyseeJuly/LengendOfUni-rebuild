#pragma once
#pragma comment(lib, "Dbghelp")

class CCrashDump
{
public:
	static BOOL RecordExceptionInfo(const PEXCEPTION_POINTERS &excpInfo);
	static void DumpMiniDump(HANDLE hFile, PEXCEPTION_POINTERS excpInfo);
	CCrashDump(void);
	~CCrashDump(void);
};
