// Game.cpp: implementation of the CGame class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "helper.h"
#include <process.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define UM_LOADOK WM_USER + 0x100

CGame::CGame()
{
	m_pD3DDevice = NULL;
	m_pScriptPlayer = NULL;
	m_pCamera = NULL;
	m_pScene = NULL;
	m_pMusicPlayer = NULL;
	m_playMode = PM_FREE;
	m_displayMode = DM_LOADING;
	m_pD3DFont = NULL;
}

CGame::CGame(TCHAR *caption, int cx, int cy, int ms):CApp(caption,cx,cy,ms)
{
	m_pD3DDevice = NULL;
	m_pScriptPlayer = NULL;
	m_pCamera = NULL;
	m_pScene = NULL;
	m_pMusicPlayer = NULL;
	m_playMode = PM_FREE;
	m_displayMode = DM_LOADING;
	m_pD3DFont = NULL;
}


CGame::~CGame()
{
	CleanUp();
}

void CGame::CleanUp()
{
	SAFE_DELETE(m_pMusicPlayer)
	SAFE_DELETE(m_pScriptPlayer)
	SAFE_DELETE(m_pCamera)
	SAFE_DELETE(m_pScene)
	SAFE_RELEASE(m_pD3DFont)
	SAFE_RELEASE(m_pD3DDevice)
}

BOOL CGame::CreateGame(HINSTANCE hInstance, int nCmdShow)
{
	LPTSTR sCmd = GetCommandLine();
	TCHAR lpFn[255];   
	::GetModuleFileName(m_hInstance, lpFn, 255);
	if (_tcslen(sCmd) == _tcslen(lpFn) + 3)
	{
		// 不带参数
		UINT uRet;
		uRet = MessageBox(NULL, _T("请选择运行模式：\n\n“是”：自由模式\n“否”：影片模式"), _T("刘慈欣群星传"), MB_ICONINFORMATION | MB_YESNO);
		if (uRet == IDYES)
			m_playMode = PM_FREE;
		else
			m_playMode = PM_MOVIE;
	}
	else
	{
		// 带参数
		m_playMode = PM_MOVIE;
	}

	if(!CApp::CreateApp(hInstance, nCmdShow, TRUE))
		return FALSE;

	if (FAILED(InitD3D()))
	{
		return FALSE;
	}

	if (FAILED(InitFont()))
	{
		return FALSE;
	}

	_beginthread(InitThread, 0, (PVOID)this);
	//InitThread((PVOID)this);
	return TRUE;
}


HRESULT CGame::InitD3D()
{
	HRESULT hr = 0;

	// Step 1: Create the IDirect3D9 object.

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if( !d3d9 )
	{
		::MessageBox(0, _T("Direct3DCreate9() - FAILED"), 0, 0);
		return E_FAIL;
	}

	// 获取当前Windows桌面的显示模式，保存到定义的d3ddm结构中
	D3DDISPLAYMODE d3ddm;
	if( FAILED( d3d9->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
		return E_FAIL;

	// Step 2: Check for hardware vp.

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT,  D3DDEVTYPE_HAL, &caps);

	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.

	m_d3dpp.BackBufferWidth            = m_cxWindow;
	m_d3dpp.BackBufferHeight           = m_cyWindow;
	m_d3dpp.BackBufferFormat           = d3ddm.Format;
	m_d3dpp.BackBufferCount            = 1;
	m_d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	m_d3dpp.MultiSampleQuality         = 0;
	m_d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	m_d3dpp.hDeviceWindow              = m_hwnd;
	m_d3dpp.Windowed                   = true;
	m_d3dpp.EnableAutoDepthStencil     = true; 
	m_d3dpp.AutoDepthStencilFormat     = D3DFMT_D16;
	m_d3dpp.Flags                      = 0;
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: Create the device.

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		D3DDEVTYPE_HAL,         // device type
		m_hwnd,               // window associated with device
		vp,                 // vertex processing
		&m_d3dpp,             // present parameters
		&m_pD3DDevice);            // return created device

	if( FAILED(hr) )
	{
		SAFE_RELEASE(d3d9); // done with d3d9 object
		::MessageBox(0, _T("CreateDevice() - FAILED"), 0, 0);
		return hr;
	}

	SAFE_RELEASE(d3d9); // done with d3d9 object
	return S_OK;
}


void CGame::Refresh()
{
	if (!m_pD3DDevice)
		return;
	
//	TestAndReset();

	m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x0, 1.0f, 0);
	m_pD3DDevice->BeginScene();

	if (m_displayMode == DM_LOADING)
	{
		if (m_pD3DFont)
		{
			TCHAR* sInfo = _T("*=刘慈欣群星传=*\n\n正在载入，请稍候...");
			RECT rect = {0, 0, 800, 600};
			m_pD3DFont->DrawText(NULL, sInfo, -1,
				&rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	else if (m_displayMode == DM_MAIN)
	{
		m_pScene->Draw(m_pD3DDevice);
		if (m_playMode == PM_MOVIE)
			m_pScriptPlayer->RunScript(m_pD3DDevice, m_pCamera);
		if (m_playMode == PM_FREE)
			ProcessInputs();
		D3DXMATRIX*	pmatView = m_pCamera->GetViewMatrix();		// 得到摄像机矩阵.
		m_pD3DDevice->SetTransform( D3DTS_VIEW, pmatView );			// 生成摄像机矩阵

		if (m_pD3DFont && m_playMode == PM_FREE)
		{
			TCHAR* sInfo = _T("*=刘慈欣群星传=*\nA、S、D、W：控制方向\nEsc：退出");
			RECT rect = {10, 10, 200, 150};
			OffsetRect(&rect, -1, 0);
			m_pD3DFont->DrawText(NULL, sInfo, -1,
								&rect, DT_LEFT | DT_WORDBREAK, D3DCOLOR_ARGB(255, 0, 0, 255));
			OffsetRect(&rect, 2, 0);
			m_pD3DFont->DrawText(NULL, sInfo, -1,
				&rect, DT_LEFT | DT_WORDBREAK, D3DCOLOR_ARGB(255, 0, 0, 255));
			OffsetRect(&rect, -1, 1);
			m_pD3DFont->DrawText(NULL, sInfo, -1,
				&rect, DT_LEFT | DT_WORDBREAK, D3DCOLOR_ARGB(255, 0, 0, 255));
			OffsetRect(&rect, 0, -2);
			m_pD3DFont->DrawText(NULL, sInfo, -1,
				&rect, DT_LEFT | DT_WORDBREAK, D3DCOLOR_ARGB(255, 0, 0, 255));
			OffsetRect(&rect, 0, 1);
			m_pD3DFont->DrawText(NULL, sInfo, -1,
				&rect, DT_LEFT | DT_WORDBREAK, D3DCOLOR_ARGB(255, 255, 255, 0));
		}
	}

	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present(0, 0, 0, 0);

}

HRESULT CGame::InitScene()
{
	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI / 4, // 90 - degree
		(float)m_cxWindow / (float)m_cyWindow,
		0.1f,
		300.0f);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &proj);

	//
	// set the camera
	//

	D3DXVECTOR3 position( 0.0f, 0.0f, -2.0f );

	// the camera is targetted at the origin of the world
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);

	// the worlds up vector
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX matView;
	D3DXMatrixLookAtLH( &matView, &position, &target, &up );
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	m_pCamera = new ZCamera;
	m_pCamera->SetView(&position, &target, &up);
	//
	// Switch to wireframe mode.
	//

	//	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//
	// Setup a directional light.
	//

	D3DXVECTOR3 dir(1.0f, 0.0f, 0.25f);
	D3DXCOLOR   color = D3DCOLOR_XRGB(255, 255, 255);
	D3DLIGHT9 dirLight;

	dirLight = InitDirectionalLight(&dir, &color);

	//
	// Set and Enable the light.
	//

	m_pD3DDevice->SetLight(0, &dirLight);
	m_pD3DDevice->LightEnable(0, true);

	//
	// Set lighting related render states.
	//
	//	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, false);

	m_pScene = new CScene;
	return m_pScene->Create(m_pD3DDevice);
}

D3DLIGHT9 CGame::InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = *color * 0.4f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Direction = *direction;

	return light;
}

/**-----------------------------------------------------------------------------
* 鼠标输出处理
*------------------------------------------------------------------------------
*/
void CGame::ProcessMouse( void )
{
	static DWORD g_dwMouseX = 0;
	static DWORD g_dwMouseY = 0;
	POINT	pt;
	float	fDelta = 0.001f;	// 鼠标的灵敏度，该值越大移动越多.

	GetCursorPos( &pt );
	int dx = pt.x - g_dwMouseX;	// 鼠标的变化值
	int dy = pt.y - g_dwMouseY;	// 鼠标的变化值

	m_pCamera->RotateLocalX( dy * fDelta );	// 鼠标的Y轴旋转值为3D world的X轴旋转值
	m_pCamera->RotateLocalY( dx * fDelta );	// 鼠标的X轴旋转值为3D world的Y轴旋转值
	D3DXMATRIX*	pmatView = m_pCamera->GetViewMatrix();		// 得到摄像机矩阵.
	m_pD3DDevice->SetTransform( D3DTS_VIEW, pmatView );			// 生成摄像机矩阵


	// 鼠标在窗口中央初始化
	//	SetCursor( NULL );	// 鼠标消失.
	RECT	rc;
	GetClientRect( m_hwnd, &rc );
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;
	ClientToScreen( m_hwnd, &pt );
	SetCursorPos( pt.x, pt.y );
	g_dwMouseX = pt.x;
	g_dwMouseY = pt.y;
}

/**-----------------------------------------------------------------------------
* 键盘输入处理
*------------------------------------------------------------------------------
*/
void CGame::ProcessKey( void )
{
	if( GetAsyncKeyState( 'W' ) ) m_pCamera->MoveLocalZ( 0.1f );	// 摄像机前进!
	if( GetAsyncKeyState( 'S' ) ) m_pCamera->MoveLocalZ( -0.1f );	// 摄像机后退!
	if( GetAsyncKeyState( 'A' ) ) m_pCamera->MoveLocalX( -0.1f );	// 摄像机前进!
	if( GetAsyncKeyState( 'D' ) ) m_pCamera->MoveLocalX( 0.1f );	// 摄像机后退!
	if( GetAsyncKeyState( VK_ESCAPE ) ) PostMessage( m_hwnd, WM_DESTROY, 0, 0L );
//	if( GetAsyncKeyState( VK_LBUTTON ) ) m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
//	if( GetAsyncKeyState( VK_RBUTTON ) ) m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

}

/**-----------------------------------------------------------------------------
* 输入处理
*------------------------------------------------------------------------------
*/
void CGame::ProcessInputs( void )
{
	ProcessMouse();
	ProcessKey();
}

void CGame::InitScript()
{
	m_pScriptPlayer = new CScriptPlayer;
	// 设定位置和摄像机角度
	m_pScriptPlayer->AddScript(ST_SETPOS, 20.0f, 0.0f, -120.0f);
	m_pScriptPlayer->AddScript(ST_YROTATE, D3DX_PI / 2, 0.0f);
	// 摄像机旋转
	m_pScriptPlayer->AddScript(ST_YROTATE, (-1) * D3DX_PI / 2, 6000.0f, 0.0f, NULL, TRUE);
	// 显示图片并左移
	m_pScriptPlayer->AddScript(ST_SHOWINFO, 2000.0f, 5000.0f, 2000.0f, _T("images\\1.png"), TRUE);
	m_pScriptPlayer->AddScript(ST_XMOVE, -3.214f, 9000.0f, 0.0f);
	m_pScriptPlayer->AddScript(ST_SHOWINFO, 2000.0f, 5000.0f, 2000.0f, _T("images\\2.png"), TRUE);
	m_pScriptPlayer->AddScript(ST_XMOVE, -3.214f, 9000.0f, 0.0f);
	m_pScriptPlayer->AddScript(ST_SHOWINFO, 2000.0f, 6000.0f, 2000.0f, _T("images\\3.png"), TRUE);
	m_pScriptPlayer->AddScript(ST_XMOVE, -3.571f, 10000.0f, 0.0f);
	// 到冥王星
	m_pScriptPlayer->AddScript(ST_ZMOVE, 20.0f, 7272.7f, 0.0f, NULL,TRUE);
	m_pScriptPlayer->AddScript(ST_XMOVE, -20.0f, 7272.7f);
	// 到海王星
	m_pScriptPlayer->AddScript(ST_ZMOVE, 20.0f, 7272.7f, 0.0f, NULL,TRUE);
	m_pScriptPlayer->AddScript(ST_YROTATE, D3DX_PI / 6, 7272.7f);
	// 到土星
	m_pScriptPlayer->AddScript(ST_ZMOVE, 20.0f, 7272.7f, 0.0f, NULL,TRUE);
	m_pScriptPlayer->AddScript(ST_XMOVE, 20.0f, 7272.7f);
	m_pScriptPlayer->AddScript(ST_YROTATE, (-1) * D3DX_PI / 3, 7272.7f);

	// 到木星
	m_pScriptPlayer->AddScript(ST_ZMOVE, 20.0f, 7272.7f, 0.0f, NULL,TRUE);
	// 到火星
	m_pScriptPlayer->AddScript(ST_ZMOVE, 20.0f, 7272.7f, 0.0f, NULL,TRUE);
	m_pScriptPlayer->AddScript(ST_XMOVE, -20.0f, 7272.7f);
	m_pScriptPlayer->AddScript(ST_YROTATE, D3DX_PI / 3, 7272.7f);
	// 接近地球
	m_pScriptPlayer->AddScript(ST_ZMOVE, 10.0f, 3636.0f, 0.0f, NULL,TRUE);
	m_pScriptPlayer->AddScript(ST_XMOVE, 10.0f, 3636.0f);
	m_pScriptPlayer->AddScript(ST_YROTATE, (-1) * D3DX_PI / 6, 3636.0f);
	// 开始绕地球旋转
	m_pScriptPlayer->AddScript(ST_ZMOVE, 10.0f, 4500.0f, 0.0f, NULL,TRUE);
	m_pScriptPlayer->AddScript(ST_XMOVE, 10.0f, 4500.0f);
	m_pScriptPlayer->AddScript(ST_YROTATE, (-1) * D3DX_PI / 2, 4500.0f);
	m_pScriptPlayer->AddScript(ST_ZMOVE, 10.0f, 4500.0f, 0.0f, NULL,TRUE);
	m_pScriptPlayer->AddScript(ST_XMOVE, -10.0f, 4500.0f);
	m_pScriptPlayer->AddScript(ST_YROTATE, (-1) * D3DX_PI / 2, 4500.0f);
	m_pScriptPlayer->AddScript(ST_ZMOVE, -10.0f, 4500.0f, 0.0f, NULL,TRUE);
	m_pScriptPlayer->AddScript(ST_XMOVE, -10.0f, 4500.0f);
	m_pScriptPlayer->AddScript(ST_YROTATE, (-1) * D3DX_PI / 2, 4500.0f);
	m_pScriptPlayer->AddScript(ST_ZMOVE, -10.0f, 4500.0f, 0.0f, NULL,TRUE);
	m_pScriptPlayer->AddScript(ST_XMOVE, 10.0f, 4500.0f);
	m_pScriptPlayer->AddScript(ST_YROTATE, (-1) * D3DX_PI / 2, 4500.0f);
	// 显示图片
	m_pScriptPlayer->AddScript(ST_SHOWINFO, 100.0f, 2300.0f, 100.0f, _T("images\\b1.png"), TRUE);
	m_pScriptPlayer->AddScript(ST_ZMOVE, 0.828f, 2500.0f);
	m_pScriptPlayer->AddScript(ST_SHOWINFO, 100.0f, 2300.0f, 100.0f, _T("images\\b2.png"), TRUE);
	m_pScriptPlayer->AddScript(ST_ZMOVE, 0.828f, 2500.0f);
	m_pScriptPlayer->AddScript(ST_SHOWINFO, 100.0f, 2300.0f, 100.0f, _T("images\\b3.png"), TRUE);
	m_pScriptPlayer->AddScript(ST_ZMOVE, 0.828f, 2500.0f);
	m_pScriptPlayer->AddScript(ST_SHOWINFO, 100.0f, 2300.0f, 100.0f, _T("images\\b4.png"), TRUE);
	m_pScriptPlayer->AddScript(ST_ZMOVE, 0.828f, 2500.0f);
	m_pScriptPlayer->AddScript(ST_SHOWINFO, 2000.0f, 4000.0f, 2000.0f, _T("images\\b5.png"), TRUE);
	m_pScriptPlayer->AddScript(ST_ZMOVE, 2.207f, 8000.0f);
	m_pScriptPlayer->AddScript(ST_SHOWINFO, 2000.0f, 9000.0f, 2000.0f, _T("images\\title.png"), TRUE);
	m_pScriptPlayer->AddScript(ST_ZMOVE, 2.483f, 9000.0f);
	m_pScriptPlayer->AddScript(ST_STARTGAME, 0, 0, 0, NULL, TRUE);

//	m_pScriptPlayer->AddScript(ST_SHOWINFO, 2000.0f, 3000.0f, 2000.0f, _T("images\\b4.png"));
//	m_pScriptPlayer->AddScript(ST_ZMOVE, 10.0f, 5000.0f, 0.0f, NULL,TRUE);
//	m_pScriptPlayer->AddScript(ST_XMOVE, -10.0f, 3000.0f);
//	m_pScriptPlayer->AddScript(ST_YROTATE, D3DX_PI / 4, 3000.0f);
}

void CGame::InitMusicPlayer()
{
	m_pMusicPlayer = new CMusicPlayer(m_hwnd);
	m_pMusicPlayer->AddMusic(_T("music\\prelude.mp3"));
	m_pMusicPlayer->Play(TRUE);
}

LRESULT CGame::AppWinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	if (message == UM_MUSICNOTIFY && m_pMusicPlayer)
	{
		m_pMusicPlayer->HandleEvent();
	}
	else if (message == UM_LOADOK)
	{
		InitMusicPlayer();
		m_displayMode = DM_MAIN;
	}
	else if (message == WM_CLOSE && m_playMode == PM_MOVIE)
	{
		::ShowWindow(m_hwnd, SW_HIDE);

		TCHAR szDir[255], szPath[255];
		::GetCurrentDirectory(255, szDir);
		_tcsncpy(szPath, szDir, 255);
		_tcscat(szPath, _T("\\lou.exe"));
		SHELLEXECUTEINFO info;   
		TCHAR *szVerb = _T("open"); 
		TCHAR *szPara = _T("play");
		ZeroMemory(&info,  sizeof(info));   

		info.cbSize   =   sizeof(SHELLEXECUTEINFO);   
		info.hwnd     =   (HWND)GetDesktopWindow();   
		info.fMask   =   SEE_MASK_NOCLOSEPROCESS;   
		info.lpVerb   =   szVerb;   
		info.lpFile   = szPath; 
		info.nShow   =   SW_SHOWDEFAULT;
		info.lpParameters = szPara;
		info.lpDirectory = szDir;
		ShellExecuteEx(&info);
	}
	return S_OK;
}

HRESULT CGame::InitFont()
{
	if (m_pD3DFont)
		return S_OK;

	D3DXFONT_DESC fontDesc;
	ZeroMemory(&fontDesc, sizeof(fontDesc));
	fontDesc.CharSet = DEFAULT_CHARSET;
	_tcsncpy(fontDesc.FaceName, _T("宋体"), 4);
	fontDesc.Weight = FW_NORMAL;
	fontDesc.Height = 14;

	HRESULT hr = D3DXCreateFontIndirect(m_pD3DDevice, &fontDesc, &m_pD3DFont);
	if (FAILED(hr))
		::MessageBox(0, _T("InitFont() - FAILED"), 0, 0);

	return hr;
}

void CGame::InitThread(PVOID pThis)
{
	CGame* pGame = (CGame*)pThis;

	if (FAILED(pGame->InitScene()))
	{
		PostMessage(pGame->m_hwnd, WM_CLOSE, NULL, NULL);
		return ;
	}

	pGame->InitScript();

	PostMessage(pGame->m_hwnd, UM_LOADOK, NULL, NULL);
}

void CGame::TestAndReset()
{
	// 嗯，这个函数基本没啥用了。
	HRESULT hr;
	hr = m_pD3DDevice->TestCooperativeLevel();
	if (FAILED(hr))
	{
		if (hr == D3DERR_DEVICENOTRESET)
		{
			m_pD3DDevice->Reset(&m_d3dpp);
			m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
			m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, false);
		}
	}
}