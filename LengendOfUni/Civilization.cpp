#include "StdAfx.h"
#include "Civilization.h"
#include "LengendOfUni.h"
#include <algorithm>

IMPLEMENT_SERIAL(CCivilization, CObject, 1)

CCivilization::CCivilization(void)
{
	m_iPopulation = 0;
	m_iCulture = 0;
	m_iEconomy = 0;
	m_iResource = 0;
	m_iArmy = 0;
	m_iTreachery = 0;
	m_iCiviLevel = 0;
	m_sName.Empty();
	m_iPopulationPercent = 0;
	m_iMiningPercent = 0;
	m_iEconomyPercent = 0;
	m_iCulturePercent = 0;
	m_tecTreeManager = NULL;
}

CCivilization::~CCivilization(void)
{
	Clear();
}

void CCivilization::SetPopulation(int iVal)
{
	m_iPopulation = iVal;
}

int CCivilization::GetPopulation()
{
	return m_iPopulation;
}

void CCivilization::SetCulture(int iVal)
{
	m_iCulture = iVal;
}

int CCivilization::GetCulture()
{
	return m_iCulture;
}

void CCivilization::SetEconomy(int iVal)
{
	m_iEconomy = iVal;
}

int CCivilization::GetEconomy()
{
	return m_iEconomy;
}

void CCivilization::SetResource(int iVal)
{
	m_iResource = iVal;
}

int CCivilization::GetResource()
{
	return m_iResource;
}

void CCivilization::SetArmy(int iVal)
{
	m_iArmy = iVal;
}

int CCivilization::GetArmy()
{
	return m_iArmy;
}

void CCivilization::SetTreachery(int iVal)
{
	m_iTreachery = iVal;
	if (iVal > 100)
		m_iTreachery = 100;
	else if (iVal < 0)
		m_iTreachery  =0;
}

int CCivilization::GetTreachery()
{
	return m_iTreachery;
}
void CCivilization::SetCiviLevel(int iVal)
{
	m_iCiviLevel = iVal;
}

int CCivilization::GetCiviLevel()
{
	return m_iCiviLevel;
}

void CCivilization::SetName(CString sVal)
{
	m_sName = sVal;
}

CString CCivilization::GetName()
{
	return m_sName;
}

CTecTreeManager& CCivilization::GetTecTreeManager()
{
	return *m_tecTreeManager;
}

void CCivilization::BuildBarback(int iStarIndex, BOOL bCompleted)
{
	CBarback *barback = new CBarback;
	barback->SetPlanetIndex(iStarIndex);
	m_lstBarbacks.push_back(barback);

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* star = starMgr.GetStar(iStarIndex);
	if (star)
	{
		star->SetBarback(barback);
		star->SetBelongToCivi(m_sName);
		barback->GetDepartment()->SetName(star->GetName() + _T("ป๙ตุ"));
	}
	if (bCompleted)
		barback->SetBuildCompleted(bCompleted);

	if (find(m_lstStars.begin(), m_lstStars.end(), iStarIndex) == m_lstStars.end())
	{
		m_lstStars.push_back(iStarIndex);
	}
}

CBarback* CCivilization::GetBarback(int iStarIndex)
{
	if (find(m_lstStars.begin(), m_lstStars.end(), iStarIndex) != m_lstStars.end())
	{
		CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
		CStar* star = starMgr.GetStar(iStarIndex);
		return star->GetBarback();
	}

	return NULL;
}

void CCivilization::Clear()
{
	std::list<CBarback*>::iterator itr;
	for (itr = m_lstBarbacks.begin(); itr != m_lstBarbacks.end(); itr ++)
	{
		SAFE_DELETE(*itr)
	}
	m_lstBarbacks.clear();
	SAFE_DELETE(m_tecTreeManager)
}

std::list<CBarback*> &CCivilization::GetBarbackList()
{
	return m_lstBarbacks;
}

BOOL CCivilization::RemoveBarback(CBarback* pBarback)
{
	std::list<CBarback*>::iterator itr;
	itr = find(m_lstBarbacks.begin(), m_lstBarbacks.end(), pBarback); 
	if (itr == m_lstBarbacks.end())
		return FALSE;

	CDepartment* pDep = (*itr)->GetDepartment();
	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	personMgr.SetPersonNoDepartment(pDep);

	SAFE_DELETE(*itr)
	m_lstBarbacks.erase(itr);
	return TRUE;
}

BOOL CCivilization::IsDieOut()
{
	return m_lstStars.empty();
}

BOOL CCivilization::RemoveStar(int iStarIndex)
{
	std::list<int>::iterator itr;
	itr = find(m_lstStars.begin(), m_lstStars.end(), iStarIndex); 
	if (itr == m_lstStars.end())
		return FALSE;

	m_lstStars.erase(itr);
	return TRUE;
}

void CCivilization::Serialize(CArchive &ar) 
{ 
	CObject::Serialize(ar); 

	if(ar.IsStoring()) 
	{ 
		ar << m_iPopulation << m_iCulture << m_iEconomy << m_iResource << m_iArmy << m_iTreachery << m_iCiviLevel << m_sName;
		ar << m_tecTreeManager; 

		ar << m_lstStars.size();
		std::list<int>::iterator itr = m_lstStars.begin();
		while (itr != m_lstStars.end())
		{
			ar << (*itr);
			itr ++;
		}

		ar << m_lstBarbacks.size();
		std::list<CBarback*>::iterator itrBarback = m_lstBarbacks.begin();
		while (itrBarback != m_lstBarbacks.end())
		{
			ar << (*itrBarback);
			itrBarback ++;
		}
	} 
	else 
	{ 
		ar >> m_iPopulation >> m_iCulture >> m_iEconomy >> m_iResource >> m_iArmy >> m_iTreachery >> m_iCiviLevel >> m_sName;
		ar >> m_tecTreeManager; 

		int iCnt;
		ar >> iCnt;
		for (int i = 0; i < iCnt; i ++)
		{
			int iValue = 0;
			ar >> iValue;
			m_lstStars.push_back(iValue);
		}

		ar >> iCnt;
		for (int i = 0; i < iCnt; i ++)
		{
			CBarback *pBarback;// = new CBarback;
			ar >> pBarback;
			m_lstBarbacks.push_back(pBarback);
		}
	} 
}  
