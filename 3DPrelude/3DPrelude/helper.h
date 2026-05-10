#pragma once
#include <d3dx9.h>
#include <tchar.h>


#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }

class CHelper
{
public:
	static D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	static int m_WindowWidth;
	static int m_WindowHeight;
};