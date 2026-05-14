#include "InfoBoard.h"
#include "helper.h"

CInfoBoard::CInfoBoard(void)
{
	m_pSprite = NULL;
	m_pTexture = NULL;
	m_iWidth = 0;
	m_iHeight = 0;
	m_fTotalTime = 0.0f;
	m_fLastTime = 0.0f;
	m_displayStep = DS_SHOWING;
}

CInfoBoard::~CInfoBoard(void)
{
	SAFE_RELEASE(m_pTexture)
	SAFE_RELEASE(m_pSprite)
}

HRESULT CInfoBoard::Create(TCHAR *sTexture, LPDIRECT3DDEVICE9 pDev, float fShowingTime, float fDisplayTime, float fHidingTime)
{
	m_fShowingTime = fShowingTime;
	m_fDisplayTime = fDisplayTime;
	m_fHidingTime = fHidingTime;
	// init sprite
	if(FAILED(D3DXCreateSprite(pDev, &m_pSprite)))
	{
		MessageBox(NULL, _T("Create sprite failed!"), _T("ERROR"), MB_OK);
		return E_FAIL;
	}

	// load texture
	HRESULT hr = D3DXCreateTextureFromFile(pDev, sTexture, &m_pTexture);
	if (FAILED(hr))
	{
		MessageBox(0, _T("load texture failed."), 0, 0);
		return hr;
	}

	// 图片的长宽应是2的幂，否则会被拉长(dx相当BT的设定)
	D3DXIMAGE_INFO imageInfo;
	if(SUCCEEDED(D3DXGetImageInfoFromFile(sTexture, &imageInfo)))
	{
		m_iWidth = imageInfo.Width;
		m_iHeight = imageInfo.Height;
	}

	return S_OK;
}

void CInfoBoard::Draw(LPDIRECT3DDEVICE9 pDev)
{
	float fCurTime = GetTickCount();
	if (m_fLastTime > 0.0f)
		m_fTotalTime += (fCurTime - m_fLastTime);
	m_fLastTime = fCurTime;

	float fAlphaBlend;
	switch (m_displayStep)
	{
	case DS_SHOWING:
		fAlphaBlend = (m_fTotalTime / m_fShowingTime) * 255.0f;
		if (fAlphaBlend >= 255.0f)
			fAlphaBlend = 255.0f;
		if (m_fTotalTime >= m_fShowingTime)
		{
			m_displayStep = DS_DISPLAY;
			m_fTotalTime = 0.0f;
		}
		break;
	case DS_DISPLAY:
		fAlphaBlend = 255.0f;
		if (m_fTotalTime >= m_fDisplayTime)
		{
			m_displayStep = DS_HIDING;
			m_fTotalTime = 0.0f;
		}
		break;
	case DS_HIDING:
		fAlphaBlend = 255.0f - (m_fTotalTime / m_fHidingTime) * 255.0f;
		if (fAlphaBlend <= 0.0f)
			fAlphaBlend = 0.0f;
		if (m_fTotalTime >= m_fHidingTime)
		{
			m_displayStep = DS_END;
			m_fTotalTime = 0.0f;
		}
		break;
	case DS_END:
		fAlphaBlend = 0.0f;
		break;
	}

	float fLeft, fTop;
	fLeft = (CHelper::m_WindowWidth - m_iWidth) / 2.0f;
	fTop = (CHelper::m_WindowHeight - m_iHeight) / 2.0f;
	D3DXVECTOR3 pos(fLeft, fTop, 0);
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pSprite->Draw(m_pTexture, NULL, NULL, &pos, D3DCOLOR_RGBA(255, 255, 255, (int)fAlphaBlend));
	m_pSprite->End();
}

BOOL CInfoBoard::IsDisplayOver()
{
	return m_displayStep == DS_END;
}