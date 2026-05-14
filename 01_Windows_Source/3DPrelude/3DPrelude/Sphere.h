#pragma once
#include <d3dx9.h>

class CSphere
{
	ID3DXMesh*				m_pMesh;
	IDirect3DTexture9*		m_pTexture;
	D3DMATERIAL9			m_material;
	float					m_fYAngle;
	D3DXMATRIX				m_matrixPos;	

public:
	HRESULT Create(TCHAR *sTexture, LPDIRECT3DDEVICE9 pDev, float x, float y, float z, float scale);
	void Draw(LPDIRECT3DDEVICE9 pDev);
	CSphere(void);
	~CSphere(void);
};
