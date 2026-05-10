#include "StdAfx.h"
#include "GameEventManager.h"
#include "RandomNumber.h"
#include "IniConfig.h"

CGameEventManager::CGameEventManager(void)
{
}

CGameEventManager::~CGameEventManager(void)
{
	Clear();
}

void CGameEventManager::Clear()
{
	std::map<CString, CGameEvent*>::iterator itr;
	for (itr = m_mapNormalEvent.begin(); itr != m_mapNormalEvent.end(); itr ++)
	{
		SAFE_DELETE(itr->second)
	}
	m_mapNormalEvent.clear();

	std::vector<CGameEvent*>::iterator itrVec;
	for (itrVec = m_vecRandomEvent.begin(); itrVec != m_vecRandomEvent.end(); itrVec ++)
	{
		SAFE_DELETE(*itrVec)
	}
	m_vecRandomEvent.clear();

}

void CGameEventManager::Init()
{
	// 初始化普通事件
	CIniConfig iniConfig(_T("gameevent.ini"));
	int iCnt;
	iCnt = iniConfig.GetInt(_T("count"), _T("count"));

	for (int i = 0; i < iCnt; i ++)
	{
		CString sSection;
		sSection.Format(_T("%d"), i);

		CString sName  = iniConfig.GetString(sSection ,_T("Name"));
		int iEventType = iniConfig.GetInt(sSection ,_T("EventType"));
		int iEventEffect = iniConfig.GetInt(sSection ,_T("EventEffect"));
		int iEventValue = iniConfig.GetInt(sSection ,_T("EventValue"));
		CGameEvent *pEvent = new CGameEvent;
		pEvent->Init((EVENT_TYPE)iEventType, (EVENT_EFFECT)iEventEffect, sName, iEventValue);

		int iTalkCnt = iniConfig.GetInt(sSection, _T("TalkCount"));
		for (int j = 0; j < iTalkCnt; j ++)
		{
			CString sFormat;
			sFormat.Format(_T("Talk%d_Pic"), j);
			CString sPic = iniConfig.GetString(sSection ,sFormat);
			sFormat.Format(_T("Talk%d_Talker"), j);
			CString sTalker = iniConfig.GetString(sSection ,sFormat);
			sFormat.Format(_T("Talk%d_Content"), j);
			CString sContent = iniConfig.GetString(sSection ,sFormat);
			pEvent->AddTalk(sPic, sTalker, sContent);
		}
		m_mapNormalEvent[sName] = pEvent;
	}

	// 初始化随机事件
	iniConfig.SetFile(_T("randomevent.ini"));
	iCnt = iniConfig.GetInt(_T("count"), _T("count"));

	for (int i = 0; i < iCnt; i ++)
	{
		CString sSection;
		sSection.Format(_T("%d"), i);

		int iEventEffect = iniConfig.GetInt(sSection ,_T("EventEffect"));
		int iEventValue = iniConfig.GetInt(sSection ,_T("EventValue"));
		CGameEvent *pEvent = new CGameEvent;
		pEvent->Init(ET_RANDOM, (EVENT_EFFECT)iEventEffect, _T(""), iEventValue);

		int iTalkCnt = iniConfig.GetInt(sSection, _T("TalkCount"));
		for (int j = 0; j < iTalkCnt; j ++)
		{
			CString sFormat;
			sFormat.Format(_T("Talk%d_Pic"), j);
			CString sPic = iniConfig.GetString(sSection ,sFormat);
			sFormat.Format(_T("Talk%d_Talker"), j);
			CString sTalker = iniConfig.GetString(sSection ,sFormat);
			sFormat.Format(_T("Talk%d_Content"), j);
			CString sContent = iniConfig.GetString(sSection ,sFormat);
			pEvent->AddTalk(sPic, sTalker, sContent);
		}
		m_vecRandomEvent.push_back(pEvent);
	}
}

BOOL CGameEventManager::RunNormalEvent(CString sName, EVENT_TYPE etype)
{
	if (m_mapNormalEvent.find(sName) == m_mapNormalEvent.end())
		return FALSE;

	if (m_mapNormalEvent[sName]->GetType() != etype)
		return FALSE;

	m_mapNormalEvent[sName]->Run();
	return TRUE;
}

BOOL CGameEventManager::RunRandomEvent()
{
	CRandomNumber randNum;
	int iIndex = randNum.random(m_vecRandomEvent.size() * 10);
	if (iIndex >= m_vecRandomEvent.size())
		return FALSE;

	m_vecRandomEvent[iIndex]->Run();
	return TRUE;
}