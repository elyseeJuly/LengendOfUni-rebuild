#pragma once
#include <queue>
#include <list>
#include <tchar.h>
#include "InfoBoard.h"
#include "ZCamera.h"

enum SCRIPT_TYPE
{
	ST_SETPOS,
	ST_XMOVE,
	ST_ZMOVE,
	ST_YROTATE,
	ST_SHOWINFO,
	ST_STARTGAME,
};

class CScriptPlayer
{
	struct SScript
	{
		SCRIPT_TYPE		m_scriptType;
		float			m_fArg1;
		float			m_fArg2;
		float			m_fArg3;
		TCHAR			m_sArg[255];
		int				m_iIndex;

		SScript()
		{
			m_fArg1 = 0.0f;
			m_fArg2 = 0.0f;
			m_fArg3 = 0.0f;
			ZeroMemory(m_sArg, 255);
		};
	};

	std::queue<SScript>		m_queScript;
	std::list<SScript>		m_lstCurrentScript;
	int						m_iIndexCounter;
	CInfoBoard*				m_pInfoBoard;

public:
	BOOL RunScript(LPDIRECT3DDEVICE9 pDev, ZCamera *pCamera);
	void AddScript(SCRIPT_TYPE scriptType, float arg1, float arg2 = 0.0f,
					float arg3 = 0.0f, TCHAR *sArg = NULL, BOOL bNext = FALSE);
	CScriptPlayer(void);
	~CScriptPlayer(void);
};
