#include "Scene.h"
#include "helper.h"

CScene::CScene(void)
{
	m_pSkyBox = NULL;
}

CScene::~CScene(void)
{
	SAFE_DELETE(m_pSkyBox)
	for (int i = 0; i < m_vecPlanets.size(); i ++)
	{
		SAFE_DELETE(m_vecPlanets[i])
		m_vecPlanets.clear();
	}
}

HRESULT CScene::Create(LPDIRECT3DDEVICE9 pDev)
{
	HRESULT hr;

	TCHAR sTextureArray[][255] = {_T("images\\earth.jpg"), _T("images\\mars.jpg"),
								_T("images\\jupiter.jpg"), _T("images\\saturn.jpg"),
								_T("images\\uranus.jpg"), _T("images\\neptune.jpg")};
	float fScaleArray[] = {1.0f, 0.8f, 2.0f, 1.8f, 1.5f, 1.5f};
	for (int i = 0; i < 6; i ++)
	{
		CSphere *pPlanet = new CSphere;
		hr = pPlanet->Create(sTextureArray[i], pDev, 0.0f, 0.0f, i * (-20.f), fScaleArray[i]);
		if (FAILED(hr))
		{
			return hr;
		}
		m_vecPlanets.push_back(pPlanet);
	}

	m_pSkyBox = new CSkyBox;

	hr = m_pSkyBox->Create(_T("images\\skybox.jpg"), pDev);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

void CScene::Draw(LPDIRECT3DDEVICE9 pDev)
{
	for (int i = 0; i < m_vecPlanets.size(); i ++)
	{
		m_vecPlanets[i]->Draw(pDev);
	}

	m_pSkyBox->Draw(pDev);
}