#pragma once
#include "sphere.h"
#include "skybox.h"
#include <vector>
#include <d3dx9.h>

class CScene
{
	std::vector<CSphere*>		m_vecPlanets;
	CSkyBox*					m_pSkyBox;
public:
	HRESULT Create(LPDIRECT3DDEVICE9 pDev);
	void Draw(LPDIRECT3DDEVICE9 pDev);
	CScene(void);
	~CScene(void);
};
