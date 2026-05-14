#pragma once
#include <d3dx9.h>

class CSkyBox
{
	class CVertex
	{
	public:
		CVertex(){};

		float m_x, m_y, m_z;
		float m_u, m_v;

		static const DWORD FVF;
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVBSkyBox;
	IDirect3DTexture9*		m_pTexture;
	D3DMATERIAL9			m_material;

private:
	void FillVertex(CVertex& vertex, float x, float y, float z, float u, float v);
public:
	HRESULT Create(TCHAR *sTexture, LPDIRECT3DDEVICE9 pDev);
	void Draw(LPDIRECT3DDEVICE9 pDev);
	CSkyBox(void);
	~CSkyBox(void);
};
