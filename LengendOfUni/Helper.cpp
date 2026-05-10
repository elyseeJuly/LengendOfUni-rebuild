#include "StdAfx.h"
#include "Helper.h"
#include "SetPopulationDlg.h"
#include "choosepersondlg.h"
#include "lengendofuni.h"
#include "SelectArmyDlg.h"
#include "battledlg.h"
#include "msgbox.h"
#include "alignmentdlg.h"
#include "RandomNumber.h"

CHelper::CHelper(void)
{
}

CHelper::~CHelper(void)
{
}

int CHelper::ChangeWorkerCount(int iOldNum)
{
	CSetPopulationDlg dlg;
	dlg.SetOldNum(iOldNum);
	if (dlg.DoModal() == IDOK)
		return dlg.m_iNum;
	return iOldNum;
}

CString CHelper::ChoosePerson()
{
	CChoosePersonDlg dlg;
	dlg.DoModal();

	return dlg.m_sPersonName;
}

void CHelper::ChangePersonDepartment(CString sPersonName, CDepartment* pNewDep)
{
	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	CPerson* pPerson = personMgr.GetPerson(sPersonName);
	if (!pPerson)
		return;

	CDepartment* pDep = pPerson->GetDepartment();
	if (pDep)
	{
		pDep->SetLeader(_T(""));
	}
	if (pNewDep)
	{
		CString sOldLeader;
		sOldLeader = pNewDep->GetLeader();
		CPerson* pOldPerson = personMgr.GetPerson(sOldLeader);
		if (pOldPerson)
			pOldPerson->SetDepartment(NULL);
	}
	pPerson->SetDepartment(pNewDep);
	if (pNewDep)
		pNewDep->SetLeader(sPersonName);
}

int CHelper::SelectStarOfBarback()
{
	CSelectArmyDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (!dlg.m_sSelArmy.IsEmpty())
		{
			int iPos = dlg.m_sSelArmy.Find(_T("基地"));
			if (iPos >= 0)
			{
				CString sName;
				sName = dlg.m_sSelArmy.Left(iPos);
				CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
				CStar *pStar = starMgr.GetStar(sName);
				if (pStar)
				{
					return pStar->GetIndex();
				}
			}
		}
	}

	return -1;
}

void CHelper::StartBattle(int iMyStar, int iEnemyStar, BATTLE_TYPE battleType)
{
	CBattleDlg dlg;
	dlg.Init(iMyStar, iEnemyStar, battleType);
	dlg.DoModal();
}

UINT CHelper::MsgBox(CString sPic, CString sInfo, CString sPersonName, BOOL bYesNoBtn)
{
	CMsgBox msgBox;
	msgBox.Init(sPic, sPersonName, sInfo, bYesNoBtn);
	return msgBox.DoModal();
}

UINT CHelper::PersonMsgBox(CPerson* pPerson, CString sInfo, BOOL bYesNoBtn)
{
	CString sName;
	CString sFace;
	if (pPerson)
	{
		sFace = pPerson->GetFaceFile();
		sName = pPerson->GetName();
	}
	else
	{
		// 若pPerson为空，则是玩家
		sName = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPlayerName();

		HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
		TCHAR lpFn[255];   
		::GetModuleFileName(hModule, lpFn, 255);
		sFace = lpFn;
		int iPos = sFace.ReverseFind('\\');
		sFace = sFace.Left(iPos + 1);
		sFace += _T("images\\player.bmp");
	}

	sName += _T("说：");
	return MsgBox(sFace, sInfo, sName, bYesNoBtn);
}

UINT CHelper::CiviMsgBox(CAlienCivilization* pAlienCivi, CString sInfo, BOOL bYesNoBtn)
{
	CString sName;
	CString sFace;
	if (pAlienCivi)
	{
		sFace = pAlienCivi->GetPic();
		sName = pAlienCivi->GetName();
	}

	sName += _T("执政官说：");
	return MsgBox(sFace, sInfo, sName, bYesNoBtn);

}

BOOL CHelper::CanUseEconemy(int iCost, BOOL bAutoReduce)
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	int iEconemy = earthCivi.GetEconomy();
	if (iEconemy < iCost)
		return FALSE;

	if (bAutoReduce)
	{
		iEconemy -= iCost;
		earthCivi.SetEconomy(iEconemy);
	}
	return TRUE;
}

void CHelper::StartAlignment(int iEnemyStar)
{
	CStar *pStar = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager().GetStar(iEnemyStar);
	if (!pStar)
	{
		ASSERT(FALSE);
		return;
	}
	CString sCivi = pStar->GetBelongToCivi();

	CRandomNumber randNum;
	int iDep = randNum.random(11);
	CDepartment *pDep = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization().GetDepartment((DEPARTMENT_TYPE)iDep);
	if (!pDep)
	{
		ASSERT(FALSE);
		return;
	}
	CString sDepName = pDep->GetName();
	CString sLeader = pDep->GetLeader();

	CAlienCivilization *pCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(sCivi);
	if (!pCivi)
	{
		ASSERT(FALSE);
		return;
	}

	CString sInfoArray[] = {_T("地球人，我很高兴你们能产生谈判的想法。"),
						_T("帝国蔑视一切谈判活动，但这次我愿意尝试一下。"),
						_T("如果你能在谈判桌上打赢我，那一定是我喝醉了。"),
						_T("宇宙和平是我民族所希望的，邪恶的战争必将终结。")};
	int iInfoIndex;
	iInfoIndex = randNum.random(4);

	CString sInfo;
	sInfo.Format(_T("%s\n我要求你们派%s的%s部长前来谈判吧。"), sInfoArray[iInfoIndex], sDepName, sLeader);
	if (CHelper::CiviMsgBox(pCivi, sInfo, TRUE) == IDOK)
	{
		if (sLeader.IsEmpty())
		{
			sInfo.Format(_T("很遗憾，%s还没有设置部长，谈判无法进行了。"), sDepName);
			CHelper::PersonMsgBox(NULL, sInfo);
			return;
		}

		CAlignmentDlg dlg;
		dlg.Init(iEnemyStar, sLeader);
		dlg.DoModal();
	}
}

BOOL CHelper::DoDevelopTecCost(TEC_TREE_TYPE tecTreeType, CString sName)
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	CTecTreeNode* pNode = tecTreeMgr.GetTecTreeNode(tecTreeType, sName);
	if (!pNode)
		return TRUE;

	if (pNode->GetInResearch() || pNode->GetFinish())
		return TRUE;

	int iCost = pNode->GetCost();
	int iEconemy = earthCivi.GetEconomy();
	if (iEconemy < iCost)
	{
		CString sMsg;
		sMsg.Format(_T("发展“%s”需要消耗%d点经济。\n我们的经济还不足以发展这项科技。"),
					pNode->GetName(), iCost);
		PersonMsgBox(NULL, sMsg);
		return FALSE;
	}
	else
	{
		iEconemy -= iCost;
		earthCivi.SetEconomy(iEconemy);
		return TRUE;
	}

	return FALSE;
}

void CHelper::AddHistory(CString sHistory)
{
	((CLengendOfUniApp*)AfxGetApp())->m_game->AddHistory(sHistory);
}

UINT CHelper::PersonMsgBox(CString sPersonName, CString sInfo, CString sInfoDef, BOOL bYesNoBtn)
{
	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	if (!sPersonName.IsEmpty())
	{
		CPerson* pPerson = personMgr.GetPerson(sPersonName);
		if (pPerson)
		{
			return PersonMsgBox(pPerson, sInfo, bYesNoBtn);
		}
	}

	return PersonMsgBox(NULL, sInfoDef, bYesNoBtn);
}
