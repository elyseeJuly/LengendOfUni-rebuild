#include "StdAfx.h"
#include "Architecture.h"
#include "StarManager.h"
#include "LengendOfUni.h"

IMPLEMENT_SERIAL(CArchitecture, CObject, 1)

CArchitecture::CArchitecture(void)
{
	m_iPlanetIndex = 0;
	m_iCurrentBuild = 0;
	m_iTotalBuild  = 100;
	m_iBuildPerRound = 20;
}

CArchitecture::~CArchitecture(void)
{
}

void CArchitecture::SetPlanetIndex(int iVal)
{
	m_iPlanetIndex = iVal;
}

int CArchitecture::GetPlanetIndex()
{
	return m_iPlanetIndex;
}

void CArchitecture::SetCurrentBuild(int iVal)
{
	m_iCurrentBuild = iVal;
}

int CArchitecture::GetCurrentBuild()
{
	return m_iCurrentBuild;
}

void CArchitecture::SetTotalBuild(int iVal)
{
	m_iTotalBuild = iVal;
}

int CArchitecture::GetTotalBuild()
{
	return m_iTotalBuild;
}

void CArchitecture::SetBuildCompleted(BOOL bCompleted)
{
	if (bCompleted)
		m_iCurrentBuild = m_iTotalBuild;
	else
		m_iCurrentBuild = 0;
}

BOOL CArchitecture::GetBuildCompleted()
{
	return m_iCurrentBuild >= m_iTotalBuild;
}

void CArchitecture::SetBuildPerRound(int iVal)
{
	m_iBuildPerRound = iVal;
}

int CArchitecture::GetBuildPerRound()
{
	return m_iBuildPerRound;
}

void CArchitecture::RunARound()
{
	if (!GetBuildCompleted())
	{
		// Ôì½¨Öþ
		m_iCurrentBuild += m_iBuildPerRound;
		if (GetBuildCompleted())
		{
			SetBuildCompleted();
		}
	}
}

void CArchitecture::Serialize(CArchive &ar) 
{
	CObject::Serialize(ar); 
	if(ar.IsStoring()) 
	{ 
		ar << m_iPlanetIndex << m_iCurrentBuild << m_iTotalBuild << m_iBuildPerRound;
	}
	else
	{
		ar >>m_iPlanetIndex >>m_iCurrentBuild >>m_iTotalBuild >>m_iBuildPerRound;

/*		CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
		CStar *pStar = starMgr.GetStar(m_iPlanetIndex);
		if (pStar == NULL)
			return;

		if (IsKindOf(RUNTIME_CLASS(CBarback)))
			pStar->SetBarback((CBarback*)this);
		else if (IsKindOf(RUNTIME_CLASS(CStope)))
			pStar->SetStope((CStope*)this);
		else if (IsKindOf(RUNTIME_CLASS(CFactory)))
			pStar->SetFactory((CFactory*)this);
		else if (IsKindOf(RUNTIME_CLASS(CCity)))
			pStar->SetCity((CCity*)this);*/
	}
}