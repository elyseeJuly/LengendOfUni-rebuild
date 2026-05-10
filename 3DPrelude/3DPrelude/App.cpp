// app.cpp: 主程序类，由此可派生出游戏的框架.
// Created in 2006/2/21
//////////////////////////////////////////////////////////////////////

#include "app.h"
#include <tchar.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CApp::CApp()
{
	m_bActive=TRUE;
	_tcsncpy(m_caption, _T("我的游戏"), 255);
	m_cxWindow=640;
	m_cyWindow=480;
	m_refreshInMs=50;
}

CApp::CApp(TCHAR *caption,int cx,int cy,int ms)
{
	_tcsncpy(m_caption,caption, 255);
	m_cxWindow=cx;
	m_cyWindow=cy;
	m_refreshInMs=ms;
	m_bActive=TRUE;

};

CApp::~CApp()
{
}

BOOL CApp::InitWindow(HINSTANCE hInstance, int nCmdShow, BOOL bWindow)
{
	WNDCLASS wcex; // 窗口类结构;
    // 设置窗口类结构;
    wcex.style=0;                       // 风格;
    wcex.lpfnWndProc=WinProc;           // 窗口处理程序;
    wcex.cbClsExtra=0;                  // 扩充风格;
    wcex.cbWndExtra=0;                  // 扩充程序;
    wcex.hInstance=hInstance;           // 应用程序hInstance句柄;
    wcex.hIcon=LoadIcon(hInstance,IDI_APPLICATION); // 读入默认的图标;
    wcex.hCursor=LoadCursor(NULL,IDC_ARROW);        // 读入默认鼠标形状;
    wcex.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);//窗口背景;
    wcex.lpszMenuName=NULL;               // 窗口目录;
    wcex.lpszClassName= _T("MyApp"); // 窗口的类名
    // 注册窗口类;
    RegisterClass(&wcex);

    // 创建主窗口;
	int xx=(GetSystemMetrics(SM_CXSCREEN)-m_cxWindow)/2;
	int yy=(GetSystemMetrics(SM_CYSCREEN)-m_cyWindow)/2;
	DWORD dwFrameWidth    = GetSystemMetrics( SM_CXDLGFRAME );
    DWORD dwFrameHeight   = GetSystemMetrics( SM_CYDLGFRAME );
	DWORD dwCaptionHeight = GetSystemMetrics( SM_CYCAPTION );

	if (bWindow)
	{
		m_hwnd=CreateWindow(_T("MyApp"), m_caption,/*WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX*/
							WS_OVERLAPPED| WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
							xx,yy,m_cxWindow+dwFrameWidth*2,m_cyWindow+dwFrameHeight*2+dwCaptionHeight,
							NULL,NULL,hInstance,NULL);
	}
	else
	{
		m_hwnd=CreateWindow(_T("MyApp"), m_caption,WS_POPUP|WS_VISIBLE,
							0,0,GetSystemMetrics(SM_CXSCREEN),
							GetSystemMetrics(SM_CYSCREEN),NULL,NULL,hInstance,NULL);	
	}
    if(!m_hwnd) return FALSE;
    ShowWindow(m_hwnd,nCmdShow); // 显示窗口;
    UpdateWindow(m_hwnd);        // 更新窗口;

    return TRUE;
}

BOOL CApp::CreateApp(HINSTANCE hInstance, int nCmdShow, BOOL bWindow)
{
	m_hInstance=hInstance;
	if (!InitWindow(hInstance, nCmdShow, bWindow))
	{
        MessageBox(GetActiveWindow(), _T("初始化程序失败!"), _T("Error"),MB_OK|MB_ICONERROR);
		return FALSE;
	}
    // 初始化DirectDraw环境，并实现DirectDraw功能;
/*    if (!m_graph->InitDDraw())
    {
        MessageBox(GetActiveWindow(),"初始化DirectDraw时出错!","Error",MB_OK|MB_ICONERROR);
		Quit();
        return FALSE;
    }
    if (!InitDInput())
    {
        MessageBox(GetActiveWindow(),"初始化DirectInput时出错!","Error",MB_OK|MB_ICONERROR);
        Quit();
        return FALSE;
    }
*/
	return TRUE;
}

BOOL CApp::MainLoop()
{
	MSG msg;
	SendMessage(m_hwnd,UM_SETCLASS,(WPARAM)this,0);
	int frame=0;
    // 进入消息循环;
    while(1)
	{
		static int cc,dd=0;
		cc=GetTickCount();
		if(cc>dd+1000)
		{
			dd=cc;
			m_fps=frame;
			frame=0;
		}
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg,NULL,0,0))         // 如果收到WM_QUIT则设置状态
			{
				break;
			} else {
					TranslateMessage(&msg); 
					DispatchMessage(&msg);
			}
		}
//		else if(m_bActive)
		{
			static int aa,bb=0;
			aa=GetTickCount();
			if(aa>bb+m_refreshInMs)
			{
				Refresh();
				bb=aa;
				frame++;
			}
//			else
//				Sleep(0);
		}
//		else
//			WaitMessage();

	}
	CleanUp();
    return msg.wParam;
}
LRESULT CALLBACK CApp::WinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static CApp *pApp=NULL;
    switch(message)
    {
	case UM_SETCLASS:
		pApp=(CApp*)wParam;
		break;
    case WM_ACTIVATEAPP:
        if(pApp)
			pApp->m_bActive=wParam;
        break;
	case WM_LBUTTONDOWN:
		pApp->OnLButtonDown();
		break;
	case WM_RBUTTONDOWN:
		pApp->OnRButtonDown();
		break;
	case WM_LBUTTONUP:
		pApp->OnLButtonUp();
		break;
	case WM_CHAR:
	case WM_IME_CHAR:
	case WM_KEYDOWN:
		pApp->CharInputPro(message,wParam);
		break;
    case WM_DESTROY:    // 退出消息循环;
		PostQuitMessage(0);
        break;
    }

	if (pApp)
		pApp->AppWinProc(hWnd, message, wParam, lParam);
    // 调用缺省消息处理过程;
    return DefWindowProc(hWnd,message,wParam,lParam);
}

void CApp::Quit()
{
	PostMessage(m_hwnd,WM_CLOSE,0,0);
}

void CApp::Refresh()
{

}


void CApp::CleanUp()
{

}

void CApp::OnLButtonDown()
{

}

void CApp::CharInputPro(UINT message, WPARAM wParam)
{

}

void CApp::OnRButtonDown()
{

}

void CApp::OnLButtonUp()
{

}

LRESULT CApp::AppWinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	return S_OK;
}
