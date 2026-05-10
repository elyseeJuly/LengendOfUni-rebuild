#pragma once
#include <d3dx9.h>

class CInfoBoard
{
	enum DISPLAY_STEP
	{
		DS_SHOWING,
		DS_DISPLAY,
		DS_HIDING,
		DS_END,
	};
	ID3DXSprite*			m_pSprite;
	IDirect3DTexture9*		m_pTexture;
	int						m_iWidth;
	int						m_iHeight;
	float					m_fShowingTime;
	float					m_fDisplayTime;
	float					m_fHidingTime;
	float					m_fTotalTime;
	float					m_fLastTime;
	DISPLAY_STEP			m_displayStep;
public:
	BOOL IsDisplayOver();
	HRESULT Create(TCHAR *sTexture, LPDIRECT3DDEVICE9 pDev, float fShowingTime, float fDisplayTime, float fHidingTime);
	void Draw(LPDIRECT3DDEVICE9 pDev);
	CInfoBoard(void);
	~CInfoBoard(void);
};
