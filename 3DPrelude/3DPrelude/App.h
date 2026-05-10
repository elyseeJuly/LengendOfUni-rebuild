// app.h: 主程序类，由此可派生出游戏的框架.
// Created in 2006/2/21
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_EDITOR_H__0FA15F0B_462D_4890_B50m_AEED163D83E4__INCLUDED_)
#define AFX_EDITOR_H__0FA15F0B_462D_4890_B50m_AEED163D83E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#define UM_SETCLASS			WM_USER+0x101					//自己定义的消息
class CApp  
{
protected:
	int m_refreshInMs;			//多少毫秒刷新一次
	TCHAR m_caption[255];		//窗口标题
	int m_cxWindow;				//窗口宽
	int m_cyWindow;
	HINSTANCE m_hInstance;
	int m_fps;
	HWND m_hwnd;
	BOOL m_bActive;				//是否激活
protected:
	virtual void OnRButtonDown();
	virtual void CharInputPro(UINT message,WPARAM wParam);
	virtual void CleanUp();
	virtual void Refresh();
	virtual void OnLButtonDown();
	virtual void OnLButtonUp();
	virtual LRESULT AppWinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	void Quit();
	BOOL InitWindow(HINSTANCE hInstance, int nCmdShow, BOOL bWindow);
	static LRESULT CALLBACK WinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

public:
	BOOL MainLoop();
	BOOL CreateApp(HINSTANCE hInstance, int nCmdShow, BOOL bWindow = TRUE);
	CApp();
	CApp(TCHAR *caption,int cx=640,int cy=480,int ms=50);
	virtual ~CApp();

};

#endif // !defined(AFX_EDITOR_H__0FA15F0B_462D_4890_B50m_AEED163D83E4__INCLUDED_)
