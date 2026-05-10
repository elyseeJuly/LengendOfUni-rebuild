#include "Sphere.h"
#include "helper.h"

CSphere::CSphere(void)
{
	m_pMesh = NULL;
	m_pTexture = NULL;
	m_fYAngle = D3DX_PI * 2.0f;
}

CSphere::~CSphere(void)
{
	SAFE_RELEASE(m_pTexture)
	SAFE_RELEASE(m_pMesh)
}

HRESULT CSphere::Create(TCHAR *sTexture, LPDIRECT3DDEVICE9 pDev, float x, float y, float z, float scale)
{
	// load material
	D3DXCOLOR WHITE( D3DCOLOR_XRGB(255, 255, 255) );
	D3DXCOLOR BLACK( D3DCOLOR_XRGB(0, 0, 0) );
	m_material = CHelper::InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);

	// load texture
	HRESULT hr = D3DXCreateTextureFromFile(pDev, sTexture, &m_pTexture);
	if (FAILED(hr))
	{
		MessageBox(0, _T("load texture failed."), 0, 0);
		return hr;
	}

	// load mesh
	hr = D3DXLoadMeshFromX(_T("data\\sphere.x"), D3DXMESH_MANAGED, pDev,
		NULL, NULL, NULL,
		NULL, &m_pMesh);
	
	if (FAILED(hr))
	{
		MessageBox(0, _T("create mesh failed."), 0, 0);
		return hr;
	}

	// pos matrix
	D3DXMatrixIdentity(&m_matrixPos);
	D3DXMatrixTranslation(&m_matrixPos, x, y, z);
	D3DXMATRIX mat;
	D3DXMatrixScaling(&mat, scale, scale, scale);
	D3DXMatrixMultiply(&m_matrixPos, &mat, &m_matrixPos);
	return S_OK;
}

void CSphere::Draw(LPDIRECT3DDEVICE9 pDev)
{
	D3DXMATRIX Ry;
	D3DXMatrixRotationY(&Ry, m_fYAngle);
	m_fYAngle -= 0.01f;//fTimeDelta;

	// reset angle to zero when angle reaches 2*PI
	if( m_fYAngle <= 0.0f )
		m_fYAngle = D3DX_PI * 2.0f;

	D3DXMatrixMultiply(&Ry, &Ry, &m_matrixPos);

	pDev->SetTransform(D3DTS_WORLD, &Ry);

	pDev->SetMaterial(&m_material);

	pDev->SetTexture(0, m_pTexture);
	pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pMesh->DrawSubset(0);
}

