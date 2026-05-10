#include "StdAfx.h"
#include "Factory.h"
#include "EarthCivilization.h"
#include "LengendOfUni.h"
#include "TecTreeManager.h"
#include "PersonManager.h"
#include "StarManager.h"

IMPLEMENT_SERIAL(CFactory, CArchitecture, 1)

CFactory::CFactory(void)
{
	m_iTotalBuild = 200;
}

CFactory::~CFactory(void)
{
}

void CFactory::RunARound(int iWorkerCnt)
{
	CArchitecture::RunARound();

	if (iWorkerCnt == 0)
		return;

	if (!GetBuildCompleted())
		return;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	CString sLeader = earthCivi.GetDepartment(DT_ECONOMY)->GetLeader();

	int iMax = 100;		// 阈值，一局的发展不能超过该值
	int iWeight = 2;	// 权重，不同的采矿技术权重不同
	int iPersonVal = 0;	// 部长辅助值
	if (!sLeader.IsEmpty())
	{
		CPerson* pPerson = personMgr.GetPerson(sLeader);
		if (pPerson)
			iPersonVal = pPerson->GetEconomy() / 30;
	}

	// 生产
	BOOL bFinish = tecTreeMgr.IsTecFinished(TT_ECONOMYTEC, _T("生产技术Ⅲ"));
	if (bFinish)
		iWeight = 5;
	else
	{
		bFinish = tecTreeMgr.IsTecFinished(TT_ECONOMYTEC, _T("生产技术Ⅱ"));
		if (bFinish)
			iWeight = 4;
		else
		{
			bFinish = tecTreeMgr.IsTecFinished(TT_ECONOMYTEC, _T("生产技术Ⅰ"));
			if (bFinish)
				iWeight = 3;
		}
	}

	bFinish = tecTreeMgr.IsTecFinished(TT_PROTON, _T("质能转换")); 

	int iAdd = (iWorkerCnt + iPersonVal) * iWeight / 2;
	int iTreachery = 100 - earthCivi.GetTreachery();
	iAdd = (iAdd * iTreachery) / 100;

	if (iAdd > iMax)
		iAdd = iMax;
	if (!bFinish)	// 掌握技能转换后，即生产无须消耗资源
	{
		int iCurRes = earthCivi.GetResource();
		if (iAdd * 2 > iCurRes)		// 生成1点经济要耗2点资源
			iAdd = iCurRes / 2;
		iCurRes -= (iAdd * 2);
		earthCivi.SetResource(iCurRes);
	}
	int iCurVal = earthCivi.GetEconomy();
	iCurVal += iAdd;
	earthCivi.SetEconomy(iCurVal);
}

BOOL CFactory::CanRunARound()
{
	if (!GetBuildCompleted())	// 建筑没完工
		return FALSE;

	return TRUE;
}

void CFactory::RunARoundNormal()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	BOOL bFinish = tecTreeMgr.IsTecFinished(TT_SPACEFIGHT, _T("行星建设Ⅲ"));
	if (bFinish)
		SetBuildPerRound(50);
	else
	{
		bFinish = tecTreeMgr.IsTecFinished(TT_SPACEFIGHT, _T("行星建设Ⅱ"));
		if (bFinish)
			SetBuildPerRound(40);
		else
		{
			bFinish = tecTreeMgr.IsTecFinished(TT_SPACEFIGHT, _T("行星建设Ⅰ"));
			if (bFinish)
				SetBuildPerRound(20);
		}
	}

	CArchitecture::RunARound();
}