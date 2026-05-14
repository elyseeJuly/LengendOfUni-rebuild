#include "StdAfx.h"
#include "EconomyDepartment.h"

IMPLEMENT_SERIAL(CEconomyDepartment, CDepartment, 1)

CEconomyDepartment::CEconomyDepartment(void)
{
	m_iMiningWorkerCnt = 0;
	m_iManufactureWorkerCnt = 0;
}

CEconomyDepartment::~CEconomyDepartment(void)
{
}

void CEconomyDepartment::SetMiningWorkerCount(int iVal)
{
	m_iMiningWorkerCnt = iVal;
}

int CEconomyDepartment::GetMiningWorkerCount()
{
	return m_iMiningWorkerCnt;
}

void CEconomyDepartment::SetManufactureWorkerCount(int iVal)
{
	m_iManufactureWorkerCnt = iVal;
}

int CEconomyDepartment::GetManufactureWorkerCount()
{
	return m_iManufactureWorkerCnt;
}

void CEconomyDepartment::RunARound(DEPARTMENT_TYPE depType)
{
/*	if (depType != DT_ECONOMY)
		return;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();

	int iMax = 0;		// 阈值，一局的发展不能超过该值
	iMax = 100;
	int iWeight = 2;
	int iPersonVal = 0;
	if (!m_sLeader.IsEmpty())
	{
		CPerson* pPerson = personMgr.GetPerson(m_sLeader);
		if (pPerson)
			iPersonVal = pPerson->GetEconomy() / 10;
	}

	// 采矿
	BOOL bFinish = tecTreeMgr.IsTecFinished(TT_INTERSTELLAR, _T("采矿技术Ⅲ"));
	if (bFinish)
		iWeight = 5;
	else
	{
		bFinish = tecTreeMgr.IsTecFinished(TT_INTERSTELLAR, _T("采矿技术Ⅱ"));
		if (bFinish)
			iWeight = 4;
		else
		{
			bFinish = tecTreeMgr.IsTecFinished(TT_INTERSTELLAR, _T("采矿技术Ⅰ"));
			if (bFinish)
				iWeight = 3;
		}
	}

	int iAdd = (m_iMiningWorkerCnt + iPersonVal) * iWeight / 2;
	if (iAdd > iMax)
		iAdd = iMax;
	int iCurVal = earthCivi.GetResource();
	iCurVal += iAdd;
	earthCivi.SetResource(iCurVal);

	// 生产
	iWeight = 2;
	bFinish = tecTreeMgr.IsTecFinished(TT_INTERSTELLAR, _T("生产技术Ⅲ"));
	if (bFinish)
		iWeight = 5;
	else
	{
		bFinish = tecTreeMgr.IsTecFinished(TT_INTERSTELLAR, _T("生产技术Ⅱ"));
		if (bFinish)
			iWeight = 4;
		else
		{
			bFinish = tecTreeMgr.IsTecFinished(TT_INTERSTELLAR, _T("生产技术Ⅰ"));
			if (bFinish)
				iWeight = 3;
		}
	}

	iAdd = (m_iManufactureWorkerCnt + iPersonVal) * iWeight / 2;
	if (iAdd > iMax)
		iAdd = iMax;
	int iCurVal = earthCivi.GetEconomy();
	iCurVal += iAdd;
	earthCivi.SetEconomy(iCurVal);*/
}

void CEconomyDepartment::Serialize(CArchive &ar)
{
	CDepartment::Serialize(ar);
	if(ar.IsStoring()) 
	{ 
		ar << m_iMiningWorkerCnt << m_iManufactureWorkerCnt;
	}
	else
	{
		ar >> m_iMiningWorkerCnt >>m_iManufactureWorkerCnt;
	}

}