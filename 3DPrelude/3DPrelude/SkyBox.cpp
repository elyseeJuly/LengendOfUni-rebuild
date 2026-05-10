#include "SkyBox.h"
#include "helper.h"

const DWORD CSkyBox::CVertex::FVF = D3DFVF_XYZ|D3DFVF_TEX1;

CSkyBox::CSkyBox(void)
{
	m_pVBSkyBox = NULL;
	m_pTexture = NULL;
}

CSkyBox::~CSkyBox(void)
{
	SAFE_RELEASE(m_pVBSkyBox)
	SAFE_RELEASE(m_pTexture)
}

HRESULT CSkyBox::Create(TCHAR *sTexture, LPDIRECT3DDEVICE9 pDev)
{
	if( FAILED( pDev->CreateVertexBuffer(10 * sizeof(CVertex), 
		0, CVertex::FVF, D3DPOOL_DEFAULT, &m_pVBSkyBox, 0)))
	{
		return E_FAIL;
	}

	CVertex* pVertices = NULL;
	if (FAILED(m_pVBSkyBox->Lock(0, 10 * sizeof(CVertex), (void**)&pVertices, 0 )))
		return E_FAIL;

	FillVertex( pVertices[0], -100.0f,-100.0f,-140.0f, 0.0f, 1.0f );
	FillVertex( pVertices[1], -100.0f, 100.0f,-140.0f, 0.0f, 0.0f );
	FillVertex( pVertices[2], -100.0f,-100.0f, 60.0f, 0.25f, 1.0f );
	FillVertex( pVertices[3], -100.0f, 100.0f, 60.0f, 0.25f, 0.0f );
	FillVertex( pVertices[4],  100.0f,-100.0f, 60.0f, 0.50, 1.0f );
	FillVertex( pVertices[5],  100.0f, 100.0f, 60.0f, 0.50f, 0.0f );
	FillVertex( pVertices[6],  100.0f,-100.0f,-140.0f, 0.75f, 1.0f );
	FillVertex( pVertices[7],  100.0f, 100.0f,-140.0f, 0.75f, 0.0f );
	FillVertex( pVertices[8], -100.0f,-100.0f,-140.0f, 1.0f, 1.0f );
	FillVertex( pVertices[9], -100.0f, 100.0f,-140.0f, 1.0f, 0.0f );

	m_pVBSkyBox->Unlock();

	// load material
	D3DXCOLOR WHITE( D3DCOLOR_XRGB(255, 255, 255) );
	D3DXCOLOR BLACK( D3DCOLOR_XRGB(0, 0, 0) );
	m_material = CHelper::InitMtrl(WHITE, WHITE, WHITE, WHITE, 2.0f);

	// load texture
	HRESULT hr = D3DXCreateTextureFromFile(pDev, sTexture, &m_pTexture);
	if (FAILED(hr))
	{
		MessageBox(0, _T("load sky box texture failed."), 0, 0);
		return hr;
	}

	return S_OK;
}

void CSkyBox::FillVertex(CVertex& vertex, float x, float y, float z, float u, float v)
{
	vertex.m_x = x;
	vertex.m_y = y;
	vertex.m_z = z;
	vertex.m_u = u;
	vertex.m_v = v;
}

void CSkyBox::Draw(LPDIRECT3DDEVICE9 pDev)
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	pDev->SetTransform(D3DTS_WORLD, &matWorld);

	pDev->SetMaterial(&m_material);

	pDev->SetTexture(0, m_pTexture);
	pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDev->SetStreamSource(0, m_pVBSkyBox, 0, sizeof(CVertex));
	pDev->SetFVF(CVertex::FVF);
	pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 8);
}