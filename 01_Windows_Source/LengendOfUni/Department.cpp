#include "StdAfx.h"
#include "Department.h"
#include "EarthCivilization.h"
#include "LengendOfUni.h"
#include "TecTreeManager.h"
#include "PersonManager.h"
#include "StarManager.h"

IMPLEMENT_SERIAL(CDepartment, CObject, 1)

CDepartment::CDepartment(void)
{
	m_iWorkerCnt = 0;
	m_sName.Empty();
	m_sLeader.Empty();
}

CDepartment::~CDepartment(void)
{
}

void CDepartment::SetName(CString sVal)
{
	m_sName = sVal;
}

CString CDepartment::GetName()
{
	return m_sName;
}

void CDepartment::SetLeader(CString sVal)
{
	m_sLeader = sVal;
}

CString CDepartment::GetLeader()
{
	return m_sLeader;
}

void CDepartment::SetWorkerCount(int iVal)
{
	m_iWorkerCnt = iVal;
}

int CDepartment::GetWorkerCount()
{
	return m_iWorkerCnt;
}

void CDepartment::RunARound(DEPARTMENT_TYPE depType)
{
	if (depType < 0 || depType > DT_COUNT)
		return;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();

	int iMax = 0;		// 阈值，一局的发展不能超过该值
	TEC_TREE_TYPE tecType = TT_COUNT;
	switch (depType)
	{
	case DT_CULTURE:
		{
			iMax = 100;
			int iWeight = 2;
			int iPersonVal = 0;
			BOOL bFinish = tecTreeMgr.IsTecFinished(TT_INFORMATION, _T("思想钢印Ⅲ"));
			if (bFinish)
				iWeight = 5;
			else
			{
				bFinish = tecTreeMgr.IsTecFinished(TT_INFORMATION, _T("思想钢印Ⅱ"));
				if (bFinish)
					iWeight = 4;
				else
				{
					bFinish = tecTreeMgr.IsTecFinished(TT_INFORMATION, _T("思想钢印Ⅰ"));
					if (bFinish)
						iWeight = 3;
				}
			}

			if (!m_sLeader.IsEmpty())
			{
				CPerson* pPerson = personMgr.GetPerson(m_sLeader);
				if (pPerson)
					iPersonVal = pPerson->GetSocial() / 5;
			}

			int iAdd = (m_iWorkerCnt + iPersonVal) * iWeight / 20;
			if (iAdd > iMax)
				iAdd = iMax;
			int iCulture = earthCivi.GetCulture();
			iCulture += iAdd;
			earthCivi.SetCulture(iCulture);
		}
		break;
	case DT_ASTROSOCIOLOGY:		// 宇宙社会学 -> 基础物理
		tecType = TT_PHYSICS;
		break;
	case DT_NUCLEAR:			// 核技术 -> 航天工程
		tecType = TT_AEROSPACE;
		break;
	case DT_SPACEFIGHT:			// 航天技术 -> 军事武器
		tecType = TT_MILITARY;
		break;
	case DT_PROTON:				// 质子技术 -> 信息与智能
		tecType = TT_INFORMATION;
		break;
	case DT_ASTROPHYSICS:		// 天体物理 -> 星际文明
		tecType = TT_INTERSTELLAR;
		break;
	case DT_CULTURETEC:			// 地球文化研究所 -> 废弃
		tecType = TT_COUNT;
		break;
	case DT_ECONOMYTEC:			// 经济学研究所 -> 废弃
		tecType = TT_COUNT;
		break;
	default:
		break;
	}

	if (tecType < TT_COUNT)
	{
		// 是科技研究所
		int iPersonVal = 0;
		iMax = 100;
		CTecTreeNode* pNode = tecTreeMgr.GetTecTree(tecType)->GetResearchTecNode();
		if (pNode)
		{
			if (!m_sLeader.IsEmpty())
			{
				CPerson* pPerson = personMgr.GetPerson(m_sLeader);
				if (pPerson)
					iPersonVal = pPerson->GetScience() / 10;
			}

			int iAdd = m_iWorkerCnt + iPersonVal;
			int iTreachery = 100 - earthCivi.GetTreachery();
			iAdd = (iAdd * iTreachery) / 100;

			CGame* pGame = ((CLengendOfUniApp*)AfxGetApp())->m_game;
			if (pGame && pGame->IsSophonBlocked())
			{
				iAdd = iAdd / 10; // 降低90%
				if (iAdd < 1 && (m_iWorkerCnt + iPersonVal) > 0) iAdd = 1; // 至少保留1点
			}

			if (iAdd > iMax)
				iAdd = iMax;
			int iCur = pNode->GetCurrentWorkload();
			iCur += iAdd;
			pNode->SetCurrentWorkload(iCur);
			if (iCur >= pNode->GetTotalWorkload())
				tecTreeMgr.GetTecTree(tecType)->SetResearchTecNode(NULL);
		}
	}
}

void CDepartment::Serialize(CArchive &ar)
{
	CObject::Serialize(ar); 
	if(ar.IsStoring()) 
	{ 
		ar << m_sName << m_sLeader << m_iWorkerCnt;
	}
	else
	{
		ar >> m_sName >> m_sLeader >> m_iWorkerCnt;
	}

}