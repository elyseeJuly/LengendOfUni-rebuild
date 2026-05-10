#pragma once
#include "department.h"
#include "barback.h"
#include "person.h"
#include "AlienCivilization.h"

class CHelper
{
public:
	CHelper(void);
	~CHelper(void);
	static int ChangeWorkerCount(int iOldNum);
	static CString ChoosePerson();
	static void ChangePersonDepartment(CString sPersonName, CDepartment* pNewDep);
	static int SelectStarOfBarback();
	static void StartBattle(int iMyStar, int iEnemyStar, BATTLE_TYPE battleType);
	static void StartAlignment(int iEnemyStar);
	static UINT MsgBox(CString sPic, CString sInfo, CString sPersonName, BOOL bYesNoBtn = FALSE);
	static UINT PersonMsgBox(CPerson* pPerson, CString sInfo, BOOL bYesNoBtn = FALSE);
	static UINT PersonMsgBox(CString sPersonName, CString sInfo, CString sInfoDef, BOOL bYesNoBtn = FALSE);
	static UINT CiviMsgBox(CAlienCivilization* pAlienCivi, CString sInfo, BOOL bYesNoBtn = FALSE);
	static BOOL CanUseEconemy(int iCost, BOOL bAutoReduce = FALSE);
	static BOOL DoDevelopTecCost(TEC_TREE_TYPE tecTreeType, CString sName);
	static void AddHistory(CString sHistory);
};
