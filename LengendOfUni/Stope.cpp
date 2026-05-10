#include "StdAfx.h"
#include "Stope.h"
#include "EarthCivilization.h"
#include "LengendOfUni.h"
#include "TecTreeManager.h"
#include "PersonManager.h"
#include "StarManager.h"

IMPLEMENT_SERIAL(CStope, CArchitecture, 1)

CStope::CStope(void)
{
}

CStope::~CStope(void)
{
}

void CStope::RunARound(int iWorkerCnt)
{
//	CArchitecture::RunARound();

	if (iWorkerCnt == 0)
		return;

	if (!GetBuildCompleted())
		return;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pStar = starMgr.GetStar(m_iPlanetIndex);
	if (!pStar)
		return;
	CString sLeader = earthCivi.GetDepartment(DT_ECONOMY)->GetLeader();

	int iLeftRes = pStar->GetCurrentResource();		// 星球有残存的矿
	int iMax = 100;		// 阈值，一局的发展不能超过该值
	int iWeight = 2;	// 权重，不同的采矿技术权重不同
	int iPersonVal = 0;	// 部长辅助值
	if (!sLeader.IsEmpty())
	{
		CPerson* pPerson = personMgr.GetPerson(sLeader);
		if (pPerson)
			iPersonVal = pPerson->GetEconomy() / 20;
	}

	// 采矿
	BOOL bFinish = tecTreeMgr.IsTecFinished(TT_ECONOMYTEC, _T("采矿技术Ⅲ"));
	if (bFinish)
		iWeight = 5;
	else
	{
		bFinish = tecTreeMgr.IsTecFinished(TT_ECONOMYTEC, _T("采矿技术Ⅱ"));
		if (bFinish)
			iWeight = 4;
		else
		{
			bFinish = tecTreeMgr.IsTecFinished(TT_ECONOMYTEC, _T("采矿技术Ⅰ"));
			if (bFinish)
				iWeight = 3;
		}
	}

	int iAdd = (iWorkerCnt + iPersonVal) * iWeight / 2;
	int iTreachery = 100 - earthCivi.GetTreachery();
	iAdd = (iAdd * iTreachery) / 100;
	if (iAdd > iMax)	// 不能超过阈值
		iAdd = iMax;
	if (iAdd > iLeftRes)	// 不能超过星球上现有的资源数
		iAdd = iLeftRes;

	int iCurVal = earthCivi.GetResource();
	iCurVal += iAdd;
	earthCivi.SetResource(iCurVal);
	pStar->SetCurrentResource(iLeftRes - iAdd);
}

BOOL CStope::CanRunARound()
{
	if (!GetBuildCompleted())	// 建筑没完工
		return FALSE;

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pStar = starMgr.GetStar(m_iPlanetIndex);
	if (!pStar)
		return FALSE;
	int iLeftRes = pStar->GetCurrentResource();

	return iLeftRes != 0;		// 没矿
}

void CStope::RunARoundNormal()
{
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pStar = starMgr.GetStar(m_iPlanetIndex);
	if (!pStar)
	{
		ASSERT(FALSE);
		return ;
	}
	BOOL bIsPlanet = pStar->GetIsPlanet();
	CString sTec;
	if (bIsPlanet)
		sTec = _T("行星开发");
	else
		sTec = _T("恒星开发");
		
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	BOOL bFinish = tecTreeMgr.IsTecFinished(TT_SPACEFIGHT, sTec + _T("Ⅲ"));
	if (bFinish)
		SetBuildPerRound(50);
	else
	{
		bFinish = tecTreeMgr.IsTecFinished(TT_SPACEFIGHT, sTec + _T("Ⅱ"));
		if (bFinish)
			SetBuildPerRound(40);
		else
		{
			bFinish = tecTreeMgr.IsTecFinished(TT_SPACEFIGHT, sTec + _T("Ⅰ"));
			if (bFinish)
				SetBuildPerRound(20);
		}
	}

	CArchitecture::RunARound();
}
