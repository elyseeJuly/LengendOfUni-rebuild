#include "StdAfx.h"
#include "Game.h"

IMPLEMENT_SERIAL(CGame, CObject, 1)

CGame::CGame(void)
{
	m_iYear = 1;
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
}

int CGame::GetYear()
{
	return m_iYear;
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

	CString sFormat;
	sFormat.Format(_T("危机纪元 %d 年 - "), m_iYear);
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

CAlienCiviManager& CGame::GetAlienCiviManager()
{
	return *m_alienCiviManager;
}

void CGame::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	if(ar.IsStoring()) 
	{
		ar << m_sPlayerName << m_iYear << m_sHistory << m_iReachStarCount;
		ar << m_starManager << m_earthCivi << m_alienCiviManager << m_personManager;
	}
	else
	{
		ar >> m_sPlayerName >> m_iYear >> m_sHistory >> m_iReachStarCount;
		ar >> m_starManager;
		ar >> m_earthCivi;
		ar >> m_alienCiviManager;
		ar >> m_personManager;
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