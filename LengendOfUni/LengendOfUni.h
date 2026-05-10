// LengendOfUni.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once
#include "Game.h"

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CLengendOfUniApp:
// 有关此类的实现，请参阅 LengendOfUni.cpp
//

class CLengendOfUniApp : public CWinApp
{

public:
	CLengendOfUniApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现
	CGame *m_game;

	DECLARE_MESSAGE_MAP()
protected:
	static LONG WINAPI LOUUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);

};

extern CLengendOfUniApp theApp;