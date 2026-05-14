// LengendOfUni.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "LogOnDlg.h"
#include "CrashDump.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLengendOfUniApp

BEGIN_MESSAGE_MAP(CLengendOfUniApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLengendOfUniApp 构造

CLengendOfUniApp::CLengendOfUniApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLengendOfUniApp 对象

CLengendOfUniApp theApp;


// CLengendOfUniApp 初始化

BOOL CLengendOfUniApp::InitInstance()
{
	::SetUnhandledExceptionFilter(LOUUnhandledExceptionFilter);
	//_CrtSetBreakAlloc(3316);	// 检查内存泄露用

	// 根据命令行判断，是进入游戏还是放动画
	CString sCmd = GetCommandLine();
	HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
	TCHAR lpFn[255];   
	::GetModuleFileName(hModule, lpFn, 255);
	CString sCmdReal = sCmd.Mid(_tcslen(lpFn) + 3);	// 去掉""和一个空格
	if (sCmdReal.IsEmpty())
	{
		// 不带参数，放动画
		CString sPath;
		CString sDir;
		sPath = lpFn;
		int iPos = sPath.ReverseFind('\\');
		sDir = sPath = sPath.Left(iPos + 1);
		sPath += _T("3DPrelude.exe");

		SHELLEXECUTEINFO info;   
		TCHAR *szVerb = _T("open"); 
		TCHAR *szPara = _T("movie");
		ZeroMemory(&info,  sizeof(info));   

		info.cbSize   =   sizeof(SHELLEXECUTEINFO);   
		info.hwnd     =   (HWND)GetDesktopWindow();   
		info.fMask   =   SEE_MASK_NOCLOSEPROCESS;   
		info.lpVerb   =   szVerb;   
		info.lpFile   = sPath; 
		info.nShow   =   SW_SHOWDEFAULT;
		info.lpParameters = szPara;
		info.lpDirectory = (LPCTSTR)sDir;
		if(ShellExecuteEx(&info))
			return FALSE;
	}

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	//SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	m_game = NULL;
	CLogOnDlg dlgLogon;
	m_pMainWnd = &dlgLogon;
	dlgLogon.DoModal();

	SAFE_DELETE(m_game)
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

LONG WINAPI CLengendOfUniApp::LOUUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	CCrashDump::RecordExceptionInfo(pExceptionInfo);

	return 1;	//::UnhandledExceptionFilter( pExceptionInfo );
}