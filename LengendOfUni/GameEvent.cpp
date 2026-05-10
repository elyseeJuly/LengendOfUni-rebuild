#include "StdAfx.h"
#include "GameEvent.h"
#include "Helper.h"
#include "EarthCivilization.h"
#include "LengendOfUni.h"
#include "IniConfig.h"

CGameEvent::CGameEvent(void)
{
	m_iEventValue = 0;
}

CGameEvent::~CGameEvent(void)
{
}

void CGameEvent::Run()
{
	for (int i = 0; i < m_vecTalk.size(); i ++)
	{
		STalk& stalk = m_vecTalk[i];
		CHelper::MsgBox(stalk.m_sPic, stalk.m_sContect, stalk.m_sTalker);
	}

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();

	int iVal = 0;
	switch (m_effect)
	{
	case EE_ADDECONEMY:
		iVal = earthCivi.GetEconomy() + m_iEventValue;
		if (iVal < 0)
			iVal = 0;
		earthCivi.SetEconomy(iVal);
		break;
	case EE_ADDCULTURE:
		iVal = earthCivi.GetCulture() + m_iEventValue;
		if (iVal < 0)
			iVal = 0;
		earthCivi.SetCulture(iVal);
		break;
	case EE_ADDPOP:
		iVal = earthCivi.GetPopulation() + m_iEventValue;
		if (iVal < 0)
			iVal = 0;
		earthCivi.SetPopulation(iVal);
		iVal = earthCivi.GetIdlePopulation() + m_iEventValue;
		earthCivi.SetIdlePopulation(iVal);
		break;
	case EE_ADDTREACHERY:
		iVal = earthCivi.GetTreachery() + m_iEventValue;
		if (iVal < 0)
			iVal = 0;
		earthCivi.SetTreachery(iVal);
		break;
	case EE_WAR:
		{
			CIniConfig iniConfig;
			iniConfig.SetFile(_T("star.ini"));
			CString sSection;
			sSection.Format(_T("%d"), m_iEventValue);
			CString sStarName;
			sStarName = iniConfig.GetString(sSection ,_T("Name"));
			CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
			CStar *pStar = starMgr.GetStar(sStarName);
			if (!pStar)
			{
				ASSERT(FALSE);
				return;
			}
			int iStarIndex = pStar->GetIndex();
			
			if (pStar->GetBarback() == NULL || pStar->GetBelongToCivi().Compare(EARTH_CIVI_NAME) == 0)
			{
				CHelper::PersonMsgBox(NULL, _T("敌人的兵力不足，最终取消了这场战争。"));
			}
			else
				CHelper::StartBattle(0, iStarIndex, BT_DEFEND);

		}
		break;
	default:
		break;
	}
}

void CGameEvent::Init(EVENT_TYPE etype, EVENT_EFFECT eeffect, CString sName, int iEventValue)
{
	m_type = etype;
	m_effect = eeffect;
	m_sName = sName;
	m_iEventValue = iEventValue;
}

void CGameEvent::AddTalk(CString sPic, CString sTalker, CString sContent)
{
	HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
	TCHAR lpFn[255];   
	::GetModuleFileName(hModule, lpFn, 255);
	CString sFace;
	sFace = lpFn;
	int iPos = sFace.ReverseFind('\\');
	sFace = sFace.Left(iPos + 1);
	sFace += _T("images\\");
	sFace += sPic;

	STalk stalk;
	stalk.m_sPic = sFace;
	stalk.m_sTalker = sTalker;
	sContent.Replace(_T("\\n"), _T("\n"));
	stalk.m_sContect = sContent;
	m_vecTalk.push_back(stalk);
}
