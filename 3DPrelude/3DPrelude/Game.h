#pragma once


// 包含Direct3D头文件
#include "App.h"
#include "scene.h"
#include "ScriptPlayer.h"
#include "zcamera.h"
#include "../../MusicPlayer/musicplayer.h"

class CGame :public CApp
{
	enum PLAY_MODE
	{
		PM_FREE,
		PM_MOVIE,
	};
	enum DISPLAY_MODE
	{
		DM_LOADING,
		DM_MAIN,
	};

	D3DPRESENT_PARAMETERS	m_d3dpp;
	LPDIRECT3DDEVICE9       m_pD3DDevice; // 定义Direct3D设备指针
	LPD3DXFONT				m_pD3DFont;
	CScriptPlayer			*m_pScriptPlayer;
	ZCamera					*m_pCamera;
	CScene					*m_pScene;
	CMusicPlayer			*m_pMusicPlayer;
	PLAY_MODE				m_playMode;
	DISPLAY_MODE			m_displayMode;

public:
	BOOL CreateGame(HINSTANCE hInstance, int nCmdShow);
	CGame(TCHAR *caption,int cx=640,int cy=480,int ms=50);
	CGame();
	virtual ~CGame();

private:
	void ProcessMouse( void );
	void ProcessKey( void );
	void ProcessInputs( void );
	void CleanUp();
	void Refresh();
	HRESULT InitD3D();
	HRESULT InitScene();
	void InitScript();
	void InitMusicPlayer();
	HRESULT InitFont();
	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	LRESULT AppWinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	static void InitThread(PVOID pThis);
	void TestAndReset();
};

