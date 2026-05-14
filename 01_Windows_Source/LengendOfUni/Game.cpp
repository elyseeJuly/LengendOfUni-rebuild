#include "StdAfx.h"
#include "Game.h"

IMPLEMENT_SERIAL(CGame, CObject, 2)

CGame::CGame(void)
{
	m_iYear = 1;
	m_epoch = EP_CRISIS;
	m_iReachStarCount = 0;
	m_earthCivi = NULL;
	m_alienCiviManager = NULL;
	m_personManager = NULL;
	m_starManager = NULL;

}

CGame::~CGame(void)
{
	Clear();
}

void CGame::Init(void)
{
	m_earthCivi = new CEarthCivilization;
	m_alienCiviManager = new CAlienCiviManager;
	m_personManager = new CPersonManager;
	m_starManager = new CStarManager;
	m_starManager->Init();
	m_personManager->Init();
	m_weaponManager.Init();
	m_alienCiviManager->Init();		// 必须先初始化m_starManager后才能初始化m_alienCiviManager
	m_earthCivi->Init();
	m_gameEventManager.Init();
}

CEarthCivilization& CGame::GetEarthCivilization()
{
	return *m_earthCivi;
}

CPersonManager& CGame::GetPersonManager()
{
	return *m_personManager;
}

CStarManager& CGame::GetStarManager()
{
	return *m_starManager;
}

CAlienCivilization* CGame::GetAlienCivi(CString sName)
{
	return m_alienCiviManager->GetAlienCivi(sName);
}

void CGame::Clear()
{
	SAFE_DELETE(m_earthCivi)
	SAFE_DELETE(m_alienCiviManager)
	SAFE_DELETE(m_personManager)
	SAFE_DELETE(m_starManager)
}

CWeaponManager& CGame::GetWeaponManager()
{
	return m_weaponManager;
}

void CGame::RunARound()
{
	m_earthCivi->RunARound();

	m_alienCiviManager->RunARound();

	m_iYear ++;

	EPOCH_TYPE oldEpoch = m_epoch;
	if (m_iYear >= 331) m_epoch = EP_GALAXY;
	else if (m_iYear >= 269) m_epoch = EP_BUNKER;
	else if (m_iYear >= 261) m_epoch = EP_BROADCAST;
	else if (m_iYear >= 201) m_epoch = EP_DETERRENCE;
	else m_epoch = EP_CRISIS;

	if (oldEpoch != m_epoch)
	{
		CString epochNames[] = {_T("危机纪元"), _T("威慑纪元"), _T("广播纪元"), _T("掩体纪元"), _T("银河纪元")};
		CString sHistory;
		sHistory.Format(_T("人类进入了新的历史时期：%s"), epochNames[(int)m_epoch]);
		AddHistory(sHistory);
	}
}

int CGame::GetYear()
{
	return m_iYear;
}

EPOCH_TYPE CGame::GetEpoch()
{
	return m_epoch;
}

void CGame::SetPlayerName(CString sPlayerName)
{
	m_sPlayerName = sPlayerName;
}

CString CGame::GetPlayerName()
{
	return m_sPlayerName;
}

CGameEventManager& CGame::GetGameEventManager()
{
	return m_gameEventManager;
}

void CGame::AddHistory(CString sHistory)
{
	if (!m_sHistory.IsEmpty())
		m_sHistory += _T("\r\n");

	CString epochNames[] = {_T("危机纪元"), _T("威慑纪元"), _T("广播纪元"), _T("掩体纪元"), _T("银河纪元")};
	CString sFormat;
	sFormat.Format(_T("%s %d 年 - "), epochNames[(int)m_epoch], m_iYear);
	m_sHistory = m_sHistory + _T("★ ") + sFormat + sHistory;
}

CString CGame::GetHistory()
{
	return m_sHistory;
}

void CGame::AddReachStarCount()
{
	m_iReachStarCount ++;
}

BOOL CGame::IsReachStarCountLimit()
{
	return m_iReachStarCount >= 3;
}

void CGame::ResetReachStarCount()
{
	m_iReachStarCount = 0;
}

BOOL CGame::IsSophonBlocked()
{
	if (m_iYear < 10) return FALSE; // 危机初显，智子还在路上

	CAlienCivilization* pSanTi = m_alienCiviManager->GetAlienCivi(_T("三体"));
	if (pSanTi && !pSanTi->IsDieOut() && pSanTi->GetFriendshipType() < FT_FRIEND)
	{
		// 三体存在且未灭绝、且对地球不友好，则实施智子封锁
		// 地球是否研发出反制科技
		CTecTreeManager& tecMgr = m_earthCivi->GetTecTreeManager();
		if (tecMgr.IsTecFinished(TT_INFORMATION, _T("550W量子计算机")) ||
			tecMgr.IsTecFinished(TT_PHYSICS, _T("智子工程")))
		{
			return FALSE; // 成功反制
		}
		return TRUE;
	}
	return FALSE;
}

CAlienCiviManager& CGame::GetAlienCiviManager()
{
	return *m_alienCiviManager;
}

void CGame::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	int nVersion = ar.IsStoring() ? 2 : 1;
	
	if(ar.IsStoring()) 
	{
		ar << nVersion;
		ar << m_sPlayerName << m_iYear << m_sHistory << m_iReachStarCount;
		ar << m_starManager << m_earthCivi << m_alienCiviManager << m_personManager;
		int iEpoch = (int)m_epoch;
		ar << iEpoch;
	}
	else
	{
		ar >> nVersion;
		ar >> m_sPlayerName >> m_iYear >> m_sHistory >> m_iReachStarCount;
		ar >> m_starManager;
		ar >> m_earthCivi;
		ar >> m_alienCiviManager;
		ar >> m_personManager;
		
		if (nVersion >= 2)
		{
			int iEpoch = 0;
			ar >> iEpoch;
			m_epoch = (EPOCH_TYPE)iEpoch;
		}
		else
		{
			// Fallback logic
			if (m_iYear >= 331) m_epoch = EP_GALAXY;
			else if (m_iYear >= 269) m_epoch = EP_BUNKER;
			else if (m_iYear >= 261) m_epoch = EP_BROADCAST;
			else if (m_iYear >= 201) m_epoch = EP_DETERRENCE;
			else m_epoch = EP_CRISIS;
		}
		
		m_weaponManager.Init();
		m_gameEventManager.Init();

		// 设定星球上的建筑
		std::list<CBarback*> &barbackLst = m_earthCivi->GetBarbackList();
		std::list<CBarback*>::iterator itrBarback;
		for (itrBarback = barbackLst.begin(); itrBarback != barbackLst.end(); itrBarback ++)
		{
			CStar *pStar = m_starManager->GetStar((*itrBarback)->GetPlanetIndex());
			if (pStar)
				pStar->SetBarback(*itrBarback);
		}

		std::list<CStope*> &stopeLst = m_earthCivi->GetStopeList();
		std::list<CStope*>::iterator itrStope;
		for (itrStope = stopeLst.begin(); itrStope != stopeLst.end(); itrStope ++)
		{
			CStar *pStar = m_starManager->GetStar((*itrStope)->GetPlanetIndex());
			if (pStar)
				pStar->SetStope(*itrStope);
		}

		std::list<CFactory*> &factoryLst = m_earthCivi->GetFactoryList();
		std::list<CFactory*>::iterator itrFac;
		for (itrFac = factoryLst.begin(); itrFac != factoryLst.end(); itrFac ++)
		{
			CStar *pStar = m_starManager->GetStar((*itrFac)->GetPlanetIndex());
			if (pStar)
				pStar->SetFactory(*itrFac);
		}

		std::list<CCity*> &cityLst = m_earthCivi->GetCityList();
		std::list<CCity*>::iterator itrcity;
		for (itrcity = cityLst.begin(); itrcity != cityLst.end(); itrcity ++)
		{
			CStar *pStar = m_starManager->GetStar((*itrcity)->GetPlanetIndex());
			if (pStar)
				pStar->SetCity(*itrcity);
		}

		std::map<CString, CAlienCivilization*> &mapAlien = m_alienCiviManager->GetAlienCiviMap();
		std::map<CString, CAlienCivilization*>::iterator itr;
		for (itr = mapAlien.begin(); itr != mapAlien.end(); itr ++)
		{
			std::list<CBarback*> &barLst = itr->second->GetBarbackList();
			for (itrBarback = barLst.begin(); itrBarback != barLst.end(); itrBarback ++)
			{
				CStar *pStar = m_starManager->GetStar((*itrBarback)->GetPlanetIndex());
				if (pStar)
					pStar->SetBarback(*itrBarback);
			}

		}

		// 设定人物的所属部门
		for (int i = DT_ECONOMY; i < DT_COUNT; i ++)
		{
			CDepartment *dep = m_earthCivi->GetDepartment((DEPARTMENT_TYPE)i);
			CString sLeader = dep->GetLeader();
			if (!sLeader.IsEmpty())
			{
				m_personManager->GetPerson(sLeader)->SetDepartment(dep);
			}
		}
		for (itrBarback = barbackLst.begin(); itrBarback != barbackLst.end(); itrBarback ++)
		{
			CDepartment *dep = (*itrBarback)->GetDepartment();
			if (dep)
			{
				CString sLeader = dep->GetLeader();
				if (!sLeader.IsEmpty())
				{
					m_personManager->GetPerson(sLeader)->SetDepartment(dep);
				}
			}
		}
	}
}