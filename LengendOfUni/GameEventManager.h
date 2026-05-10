#pragma once
#include "gameevent.h"
#include <vector>
#include <map>

class CGameEventManager
{
	std::map<CString, CGameEvent*>		m_mapNormalEvent;
	std::vector<CGameEvent*>			m_vecRandomEvent;
public:
	BOOL RunRandomEvent();
	BOOL RunNormalEvent(CString sName, EVENT_TYPE etype);
	void Init();
	void Clear();
	CGameEventManager(void);
	~CGameEventManager(void);
};
