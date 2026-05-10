// BattleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "BattleDlg.h"
#include "RandomNumber.h"
#include "helper.h"
#include <list>
#include <vector>

#define ARMY_ATTACK_INFO _T("%s%s向%s%s发起攻击，%s%s损失 %d 点兵力。\r\n")
#define WEAPON_ATTACK_INFO _T("%s向%s投放了%s，%s各单元损失 %d 点兵力。\r\n")
#define DIE_INFO _T("%s的%s被消灭了。\r\n")
#define SKILL_FAIL_INFO _T("☆我方使用%s技能失败了。\r\n")
#define SEPERATOR_STRING _T("－－－－－－－－－－－－－－－－－－－\r\n")
//#define ENEMY_WIN_ADD_SOLDIER 100

// CBattleDlg 对话框

IMPLEMENT_DYNAMIC(CBattleDlg, CDialog)

CBattleDlg::CBattleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBattleDlg::IDD, pParent)
	, m_sLeader(_T(""))
	, m_sEnemyName(_T(""))
	, m_sInfoEdit(_T(""))
{
	m_iMyStarIndex = 0;
	m_iEnemyStarIndex = 0;
	m_bUseGeneralSkill = FALSE;
}

CBattleDlg::~CBattleDlg()
{
	Clear();
}

void CBattleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_LEADER, m_sLeader);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrFace);
	DDX_Control(pDX, IDC_LIST1, m_ctrlMyArmyListCtrl);
	DDX_Control(pDX, IDC_COMBO_WEAPON, m_ctrMyWeaponCombo);
	DDX_Control(pDX, IDC_LIST2, m_ctrlEnemyListCtrl);
	DDX_Text(pDX, IDC_STATIC_ENEMYNAME, m_sEnemyName);
	DDX_Control(pDX, IDC_STATIC_ENEMYFACE, m_ctrEnemyFace);
	DDX_Control(pDX, IDC_EDIT_INFO, m_ctrInfoEdit);
	DDX_Text(pDX, IDC_EDIT_INFO, m_sInfoEdit);
	DDX_Control(pDX, IDC_COMBO2, m_ctrGeneralSkill);
}


BEGIN_MESSAGE_MAP(CBattleDlg, CDialog)
	ON_MESSAGE(WM_SET_ITEMS, PopTargetComboList)

	ON_BN_CLICKED(IDC_BUTTON_ARMY_ATTACK, &CBattleDlg::OnBnClickedButtonArmyAttack)
	ON_BN_CLICKED(IDC_BUTTON_WEAPON_ATTACK, &CBattleDlg::OnBnClickedButtonWeaponAttack)
	ON_BN_CLICKED(IDC_BUTTON_GENSKILL, &CBattleDlg::OnBnClickedButtonGenskill)
	ON_CBN_SELCHANGE(IDC_COMBO_WEAPON, &CBattleDlg::OnCbnSelchangeComboWeapon)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CBattleDlg::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CBattleDlg 消息处理程序

void CBattleDlg::OnOK()
{
}

void CBattleDlg::OnCancel()
{
	//CDialog::OnCancel();	// for test
}

BOOL CBattleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitEnemyUI();		// 须先初始化敌人
	InitMyUI();
	InitOutputBox();
	InitToolTip();
	InitTitle();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBattleDlg::Init(int iMyStarIndex, int iEnemyStarIndex, BATTLE_TYPE battleType)
{
	m_iMyStarIndex = iMyStarIndex;
	m_iEnemyStarIndex = iEnemyStarIndex;
	m_battleType =battleType;
}

void CBattleDlg::InitMyUI()
{
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pStar = starMgr.GetStar(m_iMyStarIndex);
	if (!pStar)
		return;
	CBarback* pBarback = pStar->GetBarback();
	if (!pBarback)
		return;

	// 初始化头像
	CDepartment *pDep = pBarback->GetDepartment();
	HBITMAP hBmp;
	m_sLeader = pDep->GetLeader();
	if (m_sLeader.IsEmpty())
	{
		m_sLeader = _T("无");

		HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
		TCHAR lpFn[255];   
		::GetModuleFileName(hModule, lpFn, 255);
		CString sFace;
		sFace = lpFn;
		int iPos = sFace.ReverseFind('\\');
		sFace = sFace.Left(iPos + 1);
		sFace += _T("images\\地球.bmp");

		hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sFace, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		m_ctrFace.SetBitmap(hBmp);
	}
	else
	{
		CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
		CPerson* person = personMgr.GetPerson(m_sLeader);
		if (person)
		{
			CString sFace;
			sFace = person->GetFaceFile();
			hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sFace, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
			m_ctrFace.SetBitmap(hBmp);
		}
	}

	// 初始化部队
	InitArmyListCtrl();		// 初始化我方的listctrl

	int iSoldierCnt = pBarback->GetSoldierCount();
	if (iSoldierCnt > 0)
	{
		CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
		SSimpleWeapon *pSimpleWeapon = new SSimpleWeapon;
		pSimpleWeapon->m_iAttack = earthCivi.GetArmy();
		pSimpleWeapon->m_iHp = iSoldierCnt;
		pSimpleWeapon->m_iPriority = 1;	// 常规部队为最低优先级，但地球的比外星的优先级高
		pSimpleWeapon->m_sName = _T("士兵");
		pSimpleWeapon->m_sOriginalName = _T("士兵");
		pSimpleWeapon->m_Type = WT_UNIT;
		pSimpleWeapon->m_iOwner = BS_EARTH;
		m_mapMyArmy[_T("士兵")] = pSimpleWeapon;
	}

	WeaponExList &weaponExLst = pBarback->GetWeaponExList();
	int iIndex = 1;
	CString sFormat;
	WeaponExList::iterator itr;
	for (itr = weaponExLst.begin(); itr != weaponExLst.end(); itr ++)
	{
		CWeaponEx* pWeaponEx = *itr;
		if (!pWeaponEx->GetIsFinish())
			continue;
		CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
		CWeapon& weapon = weaponMgr.GetWeapon(pWeaponEx->GetWeaponName());
		SSimpleWeapon *pSimpleWeapon = new SSimpleWeapon;
		pSimpleWeapon->m_iAttack = weapon.GetAttack();
		pSimpleWeapon->m_iHp = weapon.GetHp();
		pSimpleWeapon->m_iPriority = weapon.GetPriority();
		pSimpleWeapon->m_Type = weapon.GetType();
		pSimpleWeapon->m_iOwner = BS_EARTH;
		pSimpleWeapon->m_sOriginalName = weapon.GetName();
		sFormat.Empty();
		sFormat.Format(_T("%d号.%s"), iIndex, weapon.GetName());
		pSimpleWeapon->m_sName = sFormat;
		iIndex ++;
		m_mapMyArmy[sFormat] = pSimpleWeapon;
	}

	InitGeneralSkill();
	UpdateMyUI();

	UpdateData(FALSE);
}

void CBattleDlg::InitEnemyUI()
{
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pEnemyStar = starMgr.GetStar(m_iEnemyStarIndex);
	if (!pEnemyStar)
		return;
	CBarback* pEnemyBarback = pEnemyStar->GetBarback();
	if (!pEnemyBarback)
		return;

	// 初始化头像
	m_sEnemyName = pEnemyStar->GetBelongToCivi();
	CAlienCivilization* pAlien = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(m_sEnemyName);
	CString sPic;
	sPic = pAlien->GetPic();
	HBITMAP hBmp;
	hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sPic, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_ctrEnemyFace.SetBitmap(hBmp);

	InitEnemyListCtrl();

	int iSoldierCnt = pEnemyBarback->GetSoldierCount();
	if (iSoldierCnt > 0)
	{
		CString sName = pEnemyStar->GetBelongToCivi();
		CAlienCivilization* pAlienCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(sName);
		int iAttack = 1;
		if (pAlienCivi)
			iAttack = pAlienCivi->GetArmy();

		SSimpleWeapon *pSimpleWeapon = new SSimpleWeapon;
		pSimpleWeapon->m_iAttack = iAttack;
		pSimpleWeapon->m_iHp = iSoldierCnt;
		pSimpleWeapon->m_iPriority = 0;	// 常规部队为最低优先级，1为暂用
		pSimpleWeapon->m_sName = _T("士兵");
		pSimpleWeapon->m_sOriginalName = _T("士兵");
		pSimpleWeapon->m_Type = WT_UNIT;
		pSimpleWeapon->m_iOwner = BS_ENEMY;
		m_mapEnemyArmy[_T("士兵")] = pSimpleWeapon;
	}

	WeaponExList &weaponExLstEnemy = pEnemyBarback->GetWeaponExList();
	int iIndex = 1;
	CString sFormat;
	WeaponExList::iterator itr;
	for (itr = weaponExLstEnemy.begin(); itr != weaponExLstEnemy.end(); itr ++)
	{
		CWeaponEx* pWeaponEx = *itr;
		if (!pWeaponEx->GetIsFinish())
			continue;
		CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
		CWeapon& weapon = weaponMgr.GetWeapon(pWeaponEx->GetWeaponName());
		SSimpleWeapon *pSimpleWeapon = new SSimpleWeapon;
		pSimpleWeapon->m_iAttack = weapon.GetAttack();
		pSimpleWeapon->m_iHp = weapon.GetHp();
		pSimpleWeapon->m_iPriority = weapon.GetPriority();
		pSimpleWeapon->m_Type = weapon.GetType();
		pSimpleWeapon->m_iOwner = BS_ENEMY;
		pSimpleWeapon->m_sOriginalName = weapon.GetName();
		sFormat.Empty();
		sFormat.Format(_T("%d号.%s"), iIndex, weapon.GetName());
		pSimpleWeapon->m_sName = sFormat;
		iIndex ++;
		m_mapEnemyArmy[sFormat] = pSimpleWeapon;
	}
	UpdateEnemyUI();
	UpdateData(FALSE);

}

void CBattleDlg::InitOutputBox()
{
	m_sInfoEdit = _T("大战一触即发...\r\n");
	UpdateData(FALSE);
}

void CBattleDlg::InitArmyListCtrl()
{
	m_ctrlMyArmyListCtrl.InsertColumn(0, _T("部队名"), LVCFMT_LEFT, 78);
	m_ctrlMyArmyListCtrl.InsertColumn(1, _T("兵力(hp)"), LVCFMT_LEFT, 58);
	m_ctrlMyArmyListCtrl.InsertColumn(2, _T("攻击目标(点击更改)"), LVCFMT_LEFT, 120);

	m_ctrlMyArmyListCtrl.SetReadOnlyColumns(0);
	m_ctrlMyArmyListCtrl.SetReadOnlyColumns(1);
	m_ctrlMyArmyListCtrl.SetComboColumns(2);

	m_ctrlMyArmyListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

/*	LV_COLUMN lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;	//居中
	lvcol.pszText = _T("部队名");
	lvcol.iSubItem=0;
	lvcol.cx = 70;
	m_ctrlMyArmyListCtrl.InsertColumn(0,&lvcol);
	lvcol.pszText = _T("兵力(hp)");
	lvcol.iSubItem=1;
	m_ctrlMyArmyListCtrl.InsertColumn(1,&lvcol);
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.pszText = _T("攻击目标");
	lvcol.iSubItem=2;
	m_ctrlMyArmyListCtrl.InsertColumn(2,&lvcol);
	//	m_ctrlMyArmyListCtrl.ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SORTASCENDING|LVS_EDITLABELS);
	m_ctrlMyArmyListCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT);*/
}

void CBattleDlg::InsertArmyListCtrl(SSimpleWeapon* pSimpleWeapon)
{
	if (!pSimpleWeapon)
		return;

	CString sFormat;
	int nItem = m_ctrlMyArmyListCtrl.InsertItem(0, pSimpleWeapon->m_sName);
	LV_ITEM lvitem;
	lvitem.mask = LVIF_IMAGE;
	lvitem.iItem = nItem;
	lvitem.iSubItem = 0;
	lvitem.pszText = (LPTSTR)(LPCTSTR)pSimpleWeapon->m_sName;
	m_ctrlMyArmyListCtrl.SetItem(&lvitem);
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = nItem;
	lvitem.iSubItem = 1;
	sFormat.Format(_T("%d"), pSimpleWeapon->m_iHp);
	lvitem.pszText = (LPTSTR)(LPCTSTR)sFormat;
	m_ctrlMyArmyListCtrl.SetItem(&lvitem);

	if (m_mapEnemyArmy.find(pSimpleWeapon->m_sEnemy) == m_mapEnemyArmy.end())
	{
		// 为这件武器挑一个初始的敌人
		SimpleWeaponMap::iterator itr = m_mapEnemyArmy.begin();
		if (itr != m_mapEnemyArmy.end())
		{
			while (itr != m_mapEnemyArmy.end())
			{
				if (itr->second->m_Type == WT_UNIT)
				{
					pSimpleWeapon->m_sEnemy = itr->second->m_sName;
					break;
				}
				itr ++;
			}
		}
		else
			pSimpleWeapon->m_sEnemy = _T("");
	}
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = nItem;
	lvitem.iSubItem = 2;
	lvitem.pszText = (LPTSTR)(LPCTSTR)pSimpleWeapon->m_sEnemy;
	m_ctrlMyArmyListCtrl.SetItem(&lvitem);
}

void CBattleDlg::Clear()
{
	SimpleWeaponMap::iterator itr = m_mapMyArmy.begin();
	while (itr != m_mapMyArmy.end())
	{
		SAFE_DELETE(itr->second)
		m_mapMyArmy.erase(itr ++);
	}

	itr = m_mapEnemyArmy.begin();
	while (itr != m_mapEnemyArmy.end())
	{
		SAFE_DELETE(itr->second)
		m_mapEnemyArmy.erase(itr ++);
	}
}

void CBattleDlg::UpdateMyUI()
{
	m_ctrlMyArmyListCtrl.DeleteAllItems();
	m_ctrMyWeaponCombo.ResetContent();
	SimpleWeaponMap::iterator itr;
	for (itr = m_mapMyArmy.begin(); itr != m_mapMyArmy.end(); itr ++)
	{
		if (itr->second->m_Type == WT_UNIT)	// 作战单元
		{
			InsertArmyListCtrl(itr->second);
		}
		else if (itr->second->m_Type == WE_EXPENDABLE)		// 消耗性武器
		{
			m_ctrMyWeaponCombo.AddString(itr->second->m_sName);
		}
	}
	m_ctrMyWeaponCombo.SetCurSel(0);

	if (m_bUseGeneralSkill)
	{
		m_ctrGeneralSkill.ResetContent();
		if (m_battleType == BT_ATTACK)
			m_ctrGeneralSkill.AddString(_T("撤军"));
	}
	m_ctrGeneralSkill.SetCurSel(0);
	OnCbnSelchangeComboWeapon();
	OnCbnSelchangeCombo2();

}

void CBattleDlg::InitGeneralSkill()
{
	// 初始化将军技
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pStar = starMgr.GetStar(m_iMyStarIndex);
	if (!pStar)
		return;
	CBarback* pBarback = pStar->GetBarback();
	if (!pBarback)
		return;
	CDepartment *pDep = pBarback->GetDepartment();
	m_sLeader = pDep->GetLeader();

	if (m_sLeader.IsEmpty())
	{
		GetDlgItem(IDC_BUTTON_GENSKILL)->EnableWindow(FALSE);
		return;
	}

	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	CPerson* pPerson = personMgr.GetPerson(m_sLeader);
	if (!pPerson)
	{
		ASSERT(FALSE);
		return;
	}
	if (pPerson->GetArmy() > 50 || pPerson->GetLeadership() > 70)
	{
		m_ctrGeneralSkill.AddString(_T("收买"));
	}
	if (pPerson->GetArmy() > 60 || pPerson->GetLeadership() > 60 || pPerson->GetArt() > 80)
	{
		m_ctrGeneralSkill.AddString(_T("士气"));
	}
	if (pPerson->GetArmy() > 60 || pPerson->GetArt() > 80)
	{
		m_ctrGeneralSkill.AddString(_T("恐吓"));
	}
/*	if (pPerson->GetArmy() > 70 || pPerson->GetLeadership() > 80)
	{
		m_ctrGeneralSkill.AddString(_T("卧底"));
	}*/
	if (m_battleType == BT_ATTACK)
		m_ctrGeneralSkill.AddString(_T("撤军"));

	
}

void CBattleDlg::InsertEnemyListCtrl(SSimpleWeapon* pSimpleWeapon)
{
	if (!pSimpleWeapon)
		return;

	CString sFormat;
	int nItem = m_ctrlEnemyListCtrl.InsertItem(0, pSimpleWeapon->m_sName);
	LV_ITEM lvitem;
	lvitem.mask = LVIF_IMAGE;
	lvitem.iItem = nItem;
	lvitem.iSubItem = 0;
	lvitem.pszText = (LPTSTR)(LPCTSTR)pSimpleWeapon->m_sName;
	m_ctrlEnemyListCtrl.SetItem(&lvitem);
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = nItem;
	lvitem.iSubItem = 1;
	sFormat.Format(_T("%d"), pSimpleWeapon->m_iHp);
	lvitem.pszText = (LPTSTR)(LPCTSTR)sFormat;
	m_ctrlEnemyListCtrl.SetItem(&lvitem);
}

void CBattleDlg::InitEnemyListCtrl()
{
	LV_COLUMN lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;	//居中
	lvcol.pszText = _T("部队名");
	lvcol.iSubItem=0;
	lvcol.cx = 150;
	m_ctrlEnemyListCtrl.InsertColumn(0,&lvcol);
	lvcol.cx = 70;
	lvcol.pszText = _T("兵力(hp)");
	lvcol.iSubItem=1;
	m_ctrlEnemyListCtrl.InsertColumn(1,&lvcol);
	//	m_ctrlMyArmyListCtrl.ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SORTASCENDING|LVS_EDITLABELS);
	m_ctrlEnemyListCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT);

}

void CBattleDlg::UpdateEnemyUI()
{
	m_ctrlEnemyListCtrl.DeleteAllItems();

	SimpleWeaponMap::iterator itr;
	for (itr = m_mapEnemyArmy.begin(); itr != m_mapEnemyArmy.end(); itr ++)
	{
		if (itr->second->m_Type == WT_UNIT)	// 作战单元
		{
			InsertEnemyListCtrl(itr->second);
		}
	}

}

LRESULT CBattleDlg::PopTargetComboList(WPARAM wParam, LPARAM lParam)
{
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());

	// Get the inplace combbox top left
	CRect obWindowRect;

	pInPlaceCombo->GetWindowRect(&obWindowRect);

	CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft()); 

	// Get the active list
	// Get the control window rect
	// If the inplace combobox top left is in the rect then
	// The control is the active control
	m_ctrlMyArmyListCtrl.GetWindowRect(&obWindowRect);

	int iColIndex = wParam;

	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll(); 

	if (obWindowRect.PtInRect(obInPlaceComboTopLeft)) 
	{
		if (2 == iColIndex)
		{
			SimpleWeaponMap::iterator itr;
			for (itr = m_mapEnemyArmy.begin(); itr != m_mapEnemyArmy.end(); itr ++)
			{
				if (itr->second->m_Type == WT_UNIT)	// 作战单元
				{
					pComboList->AddHead(itr->second->m_sName);
				}
			}
		}
	}

	return true;
}

void CBattleDlg::OnBnClickedButtonArmyAttack()
{
	// 军队攻击
	// 更新攻击目标
	int iCnt = m_ctrlMyArmyListCtrl.GetItemCount();
	for (int i = 0; i < iCnt; i ++)
	{
		CString sArmyName = m_ctrlMyArmyListCtrl.GetItemText(i, 0);
		CString sEnemyName = m_ctrlMyArmyListCtrl.GetItemText(i, 2);
		if (m_mapMyArmy.find(sArmyName) != m_mapMyArmy.end())
		{
			m_mapMyArmy[sArmyName]->m_sEnemy = sEnemyName;
		}
	}
	
	BattleProc(AT_UNITS);
}

void CBattleDlg::OnBnClickedButtonWeaponAttack()
{
	// 武器攻击
	int iSel = m_ctrMyWeaponCombo.GetCurSel();
	CString sSel;
	sSel.Empty();
	if (iSel > -1)
		m_ctrMyWeaponCombo.GetLBText(iSel, sSel);
	if (sSel.IsEmpty())
	{
		CString sInfo = _T("见鬼，我军已经没有武器可用于攻击。");
		CHelper::PersonMsgBox(m_sLeader, sInfo, sInfo);
		return;
	}

	BattleProc(AT_BOMB);
}

void CBattleDlg::OnBnClickedButtonGenskill()
{
	// 将军技
	BattleProc(AT_GENERAL_SKILL);
}

void CBattleDlg::BattleProc(ATTACK_TYPE iAttackType)
{
	UINT uID;
	switch (iAttackType)
	{
	case AT_UNITS:
		uID = IDC_BUTTON_ARMY_ATTACK;
		break;
	case AT_BOMB:
		uID = IDC_BUTTON_WEAPON_ATTACK;
		break;
	case AT_GENERAL_SKILL:
		uID = IDC_BUTTON_GENSKILL;
		break;
	}
	GetDlgItem(uID)->EnableWindow(FALSE);

	BATTLE_RESULT battleRes = BR_INBATTLE;
	if (iAttackType == AT_GENERAL_SKILL)
	{
		if (DoGeneralSkill())
			return;	// 撤军了
		battleRes = GetBattleResult();
		if (battleRes != BR_INBATTLE)
			ShowBattleResult(battleRes);
	}

	if (battleRes == BR_INBATTLE)
	{
		// 敌人使用哪种攻击方式
		BOOL bHasBomb = FALSE;
		SimpleWeaponMap::iterator itr = m_mapEnemyArmy.begin();
		while (itr != m_mapEnemyArmy.end())
		{
			if (itr->second->m_Type == WE_EXPENDABLE)
			{
				bHasBomb = TRUE;
				break;
			}
			itr ++;
		}
		ATTACK_TYPE iEnemyAttackType = AT_UNITS;
		if (bHasBomb)
		{
			CRandomNumber randNum;
			iEnemyAttackType = (ATTACK_TYPE)randNum.random(3);
			if (iEnemyAttackType == 2)
				iEnemyAttackType = AT_UNITS;
		}
		CString sEnemyBomb;
		if (iEnemyAttackType == AT_BOMB)
		{
			// 给敌人挑个炸弹
			std::vector<SSimpleWeapon*> vecWeapon;
			SimpleWeaponMap::iterator itr = m_mapEnemyArmy.begin();
			while (itr != m_mapEnemyArmy.end())
			{
				if (itr->second->m_Type == WE_EXPENDABLE)
				{
					vecWeapon.push_back(itr->second);
				}
				itr ++;
			}
			CRandomNumber randNum;
			int iRand = randNum.random(vecWeapon.size());
			sEnemyBomb = vecWeapon[iRand]->m_sName;
		}

		// 敌我攻击方式都塞到list里统一处理
		std::list<SSimpleWeapon*> lstWeapon;
		if (iAttackType == AT_UNITS)
		{
			SimpleWeaponMap::iterator itr = m_mapMyArmy.begin();
			while (itr != m_mapMyArmy.end())
			{
				if (itr->second->m_Type == (WEAPON_TYPE)iAttackType)
				{
					lstWeapon.push_back(itr->second);
				}
				itr ++;
			}
		}
		else if (iAttackType == AT_BOMB)
		{
			int iSel = m_ctrMyWeaponCombo.GetCurSel();
			CString sSel;
			m_ctrMyWeaponCombo.GetLBText(iSel, sSel);
			if (m_mapMyArmy.find(sSel) != m_mapMyArmy.end())
			{
				lstWeapon.push_back(m_mapMyArmy[sSel]);
			}

		}

		if (iEnemyAttackType == AT_UNITS)
		{
			itr = m_mapEnemyArmy.begin();
			while (itr != m_mapEnemyArmy.end())
			{
				if (itr->second->m_Type == (WEAPON_TYPE)iEnemyAttackType)
				{
					lstWeapon.push_back(itr->second);
				}
				itr ++;
			}
		}
		else if (iEnemyAttackType == AT_BOMB)
		{
			if (m_mapEnemyArmy.find(sEnemyBomb) != m_mapEnemyArmy.end())
			{
				lstWeapon.push_back(m_mapEnemyArmy[sEnemyBomb]);
			}
		}
		lstWeapon.sort(CompareFunObj());

		// 遍历一遍所有参战武器，计算战况
		std::list<SSimpleWeapon*>::iterator itrLst;
		for (itrLst = lstWeapon.begin(); itrLst != lstWeapon.end(); itrLst ++)
		{
			// 先看看该武器是否还在(可能已在上一轮被消灭)
			if (*itrLst == NULL)
				continue;
/*			if ((*itrLst)->m_iOwner == BS_EARTH)
			{
				if (m_mapMyArmy.find((*itrLst)->m_sName) == m_mapMyArmy.end())
					continue;
			}
			else if ((*itrLst)->m_iOwner == BS_ENEMY)
			{
				if (m_mapEnemyArmy.find((*itrLst)->m_sName) == m_mapEnemyArmy.end())
					continue;
			}*/
			
			AttackProc(*itrLst, &lstWeapon);

			if ((*itrLst)->m_Type == AT_BOMB)
			{
				// 使用完炸弹，删掉
				CString sName = (*itrLst)->m_sName;
				BATTLE_SIDE battleSide = (*itrLst)->m_iOwner;
				SAFE_DELETE(*itrLst);
				if (battleSide == BS_EARTH)
					m_mapMyArmy.erase(sName);
				else if (battleSide == BS_ENEMY)
					m_mapEnemyArmy.erase(sName);

			}
			battleRes = GetBattleResult();
			if (battleRes != BR_INBATTLE)
			{
				ShowBattleResult(battleRes);
				break;
			}
		}
	}

	OutputBattleInfo(SEPERATOR_STRING);
	GetDlgItem(uID)->EnableWindow(TRUE);
	UpdateMyUI();
	UpdateEnemyUI();
	if (battleRes != BR_INBATTLE)
	{
		// 大战结束了
		// !这里还可以放放音乐
		DoBattleResult(battleRes);
		CDialog::OnOK();
	}
}

void CBattleDlg::AttackProc(SSimpleWeapon *pWeapon, std::list<SSimpleWeapon*>* pLstWeapon)
{
	CString sOutputInfo;
	const CString sMyName = _T("我方");
	if (pWeapon->m_iOwner == BS_EARTH)	// 我方武器
	{
		if (pWeapon->m_Type == WT_UNIT)
		{
			if (pWeapon->m_sEnemy.IsEmpty())
			{
				ASSERT(0);
				return;
			}
			if (m_mapEnemyArmy.find(pWeapon->m_sEnemy) == m_mapEnemyArmy.end())
			{
				// 找不到敌人，可能敌人已经被上一个单位干掉了。
				if (m_mapEnemyArmy.empty())
				{
					ASSERT(0);
					return;
				}
				SimpleWeaponMap::iterator itr = m_mapEnemyArmy.begin();
				while (itr != m_mapEnemyArmy.end())
				{
					if (itr->second->m_Type == WT_UNIT)
					{
						pWeapon->m_sEnemy = itr->second->m_sName;
						break;
					}
					itr ++;
				}
			}
			m_mapEnemyArmy[pWeapon->m_sEnemy]->m_iHp -= pWeapon->m_iAttack;
			sOutputInfo.Format(ARMY_ATTACK_INFO, _T("☆") + sMyName, pWeapon->m_sName, m_sEnemyName,
								pWeapon->m_sEnemy, m_sEnemyName, pWeapon->m_sEnemy, pWeapon->m_iAttack);
			if (m_mapEnemyArmy[pWeapon->m_sEnemy]->m_iHp <= 0)
			{
				// 删掉作战序列中的被消灭者
				if (pLstWeapon)
				{
					std::list<SSimpleWeapon*>::iterator itrWeaponLst = (*pLstWeapon).begin();
					while (itrWeaponLst != (*pLstWeapon).end())
					{
						if (*itrWeaponLst != NULL)
						{
							if ((*itrWeaponLst)->m_sName.Compare(pWeapon->m_sEnemy) == 0 && (*itrWeaponLst)->m_iOwner == BS_ENEMY)
							{
								*itrWeaponLst = NULL;
								break;
							}
						}
						itrWeaponLst ++;
					}
				}
				SAFE_DELETE(m_mapEnemyArmy[pWeapon->m_sEnemy])
				m_mapEnemyArmy.erase(pWeapon->m_sEnemy);
				CString sFormat;
				sFormat.Format(DIE_INFO, _T("★") + m_sEnemyName, pWeapon->m_sEnemy);
				sOutputInfo += sFormat;
			}
		}
		else	// 使用炸弹
		{
			std::list<SSimpleWeapon*> lstWeapon;
			SimpleWeaponMap::iterator itr = m_mapEnemyArmy.begin();
			while (itr != m_mapEnemyArmy.end())
			{
				if (itr->second->m_Type == WT_UNIT)
					lstWeapon.push_back(itr->second);
				itr ++;
			}
			if (lstWeapon.empty())
			{
				ASSERT(0);
				return;
			}
			int iAttack = pWeapon->m_iAttack / lstWeapon.size();
			sOutputInfo.Format(WEAPON_ATTACK_INFO, _T("☆") + sMyName, m_sEnemyName,
				pWeapon->m_sName, m_sEnemyName, iAttack);
			
			std::list<SSimpleWeapon*>::iterator itrLst;
			for (itrLst = lstWeapon.begin(); itrLst != lstWeapon.end(); itrLst ++)
			{
				(*itrLst)->m_iHp -= iAttack;
				if ((*itrLst)->m_iHp <= 0)
				{
					CString sName = (*itrLst)->m_sName;
					// 删掉作战序列中的被消灭者
					if (pLstWeapon)
					{
						std::list<SSimpleWeapon*>::iterator itrWeaponLst = (*pLstWeapon).begin();
						while (itrWeaponLst != (*pLstWeapon).end())
						{
							if (*itrWeaponLst != NULL)
							{
								if ((*itrWeaponLst)->m_sName.Compare(sName) == 0 && (*itrWeaponLst)->m_iOwner == BS_ENEMY)
								{
									*itrWeaponLst = NULL;
									break;
								}
							}
							itrWeaponLst ++;
						}
					}

					SAFE_DELETE(m_mapEnemyArmy[sName])
					m_mapEnemyArmy.erase(sName);
					CString sFormat;
					sFormat.Format(DIE_INFO, _T("★") + m_sEnemyName, sName);
					sOutputInfo += sFormat;
				}
				else
				{
					m_mapEnemyArmy[(*itrLst)->m_sName]->m_iHp = (*itrLst)->m_iHp;
				}
			}
		}
	}
	else if (pWeapon->m_iOwner == BS_ENEMY)	// 敌方武器
	{
		if (pWeapon->m_Type == WT_UNIT)
		{
			if (m_mapMyArmy.empty())
			{
				ASSERT(0);
				return;
			}
			std::vector<SSimpleWeapon*> vecWeapon;
			SimpleWeaponMap::iterator itr = m_mapMyArmy.begin();
			while (itr != m_mapMyArmy.end())
			{
				if (itr->second->m_Type == WT_UNIT)
					vecWeapon.push_back(itr->second);
				itr ++;
			}
			CRandomNumber randNum;
			int iObj = randNum.random(vecWeapon.size());
			pWeapon->m_sEnemy = vecWeapon[iObj]->m_sName;

			m_mapMyArmy[pWeapon->m_sEnemy]->m_iHp -= pWeapon->m_iAttack;
			sOutputInfo.Format(ARMY_ATTACK_INFO, _T("★") + m_sEnemyName, pWeapon->m_sName,
				sMyName, pWeapon->m_sEnemy, sMyName, pWeapon->m_sEnemy, pWeapon->m_iAttack);
			if (m_mapMyArmy[pWeapon->m_sEnemy]->m_iHp <= 0)
			{
				// 删掉作战序列中的被消灭者
				if (pLstWeapon)
				{
					std::list<SSimpleWeapon*>::iterator itrWeaponLst = (*pLstWeapon).begin();
					while (itrWeaponLst != (*pLstWeapon).end())
					{
						if (*itrWeaponLst != NULL)
						{
							if ((*itrWeaponLst)->m_sName.Compare(pWeapon->m_sEnemy) == 0 && (*itrWeaponLst)->m_iOwner == BS_EARTH)
							{
								*itrWeaponLst = NULL;
								break;
							}
						}
						itrWeaponLst ++;
					}
				}
				SAFE_DELETE(m_mapMyArmy[pWeapon->m_sEnemy])
				m_mapMyArmy.erase(pWeapon->m_sEnemy);
				CString sFormat;
				sFormat.Format(DIE_INFO, _T("☆") + sMyName, pWeapon->m_sEnemy);
				sOutputInfo += sFormat;
			}
		}
		else		// 扔炸弹
		{
			std::list<SSimpleWeapon*> lstWeapon;
			SimpleWeaponMap::iterator itr = m_mapMyArmy.begin();
			while (itr != m_mapMyArmy.end())
			{
				if (itr->second->m_Type == WT_UNIT)
					lstWeapon.push_back(itr->second);
				itr ++;
			}
			if (lstWeapon.empty())
			{
				ASSERT(0);
				return;
			}
			int iAttack = pWeapon->m_iAttack / lstWeapon.size();
			sOutputInfo.Format(WEAPON_ATTACK_INFO, _T("★") + m_sEnemyName, sMyName,
				pWeapon->m_sName, sMyName, iAttack);

			std::list<SSimpleWeapon*>::iterator itrLst;
			for (itrLst = lstWeapon.begin(); itrLst != lstWeapon.end(); itrLst ++)
			{
				(*itrLst)->m_iHp -= iAttack;
				if ((*itrLst)->m_iHp <= 0)
				{
					CString sName = (*itrLst)->m_sName;
					// 删掉作战序列中的被消灭者
					if (pLstWeapon)
					{
						std::list<SSimpleWeapon*>::iterator itrWeaponLst = (*pLstWeapon).begin();
						while (itrWeaponLst != (*pLstWeapon).end())
						{
							if (*itrWeaponLst != NULL)
							{
								if ((*itrWeaponLst)->m_sName.Compare(sName) == 0 && (*itrWeaponLst)->m_iOwner == BS_EARTH)
								{
									*itrWeaponLst = NULL;
									break;
								}
							}
							itrWeaponLst ++;
						}
					}
					SAFE_DELETE(m_mapMyArmy[sName])
					m_mapMyArmy.erase(sName);
					CString sFormat;
					sFormat.Format(DIE_INFO, _T("☆") + sMyName, sName);
					sOutputInfo += sFormat;
				}
				else
				{
					m_mapMyArmy[(*itrLst)->m_sName]->m_iHp = (*itrLst)->m_iHp;
				}
			}
		}
	}

	OutputBattleInfo(sOutputInfo);
}

void CBattleDlg::OutputBattleInfo(CString sInfo)
{
	m_sInfoEdit += sInfo;
	UpdateData(FALSE);
	m_ctrInfoEdit.SetSel(m_sInfoEdit.GetLength(), m_sInfoEdit.GetLength());
}

void CBattleDlg::DoBattleResult(BATTLE_RESULT battleRes)
{
	UpdateBothSidesPopulation();

	CAlienCivilization* pAlien = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(m_sEnemyName);
	pAlien->ReduceFriendshipDegree();
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pStar = NULL;

	CString sHistory;
	switch (battleRes)
	{
	case BR_WIN:
		{
			pStar = starMgr.GetStar(m_iEnemyStarIndex);
			if (!pStar)
			{
				ASSERT(0);
				return;
			}
			CBarback* pBarback = pStar->GetBarback();
			if (!pBarback)
			{
				ASSERT(0);
				return;
			}

			if (m_battleType == BT_ATTACK)
			{
				sHistory.Format(_T("我们在进攻%s星球的战役中打败了%s，取得了胜利，未来是我们的！"),
					pStar->GetName(), pAlien->GetName());
				pAlien->RemoveBarback(pBarback);
				pAlien->RemoveStar(m_iEnemyStarIndex);
				pStar->SetBarback(NULL);
				pStar->SetBelongToCivi(_T(""));
			}
			else if (m_battleType == BT_DEFEND)
			{
				pBarback->Clear(TRUE);
				pStar = starMgr.GetStar(m_iMyStarIndex);
				if (!pStar)
				{
					ASSERT(0);
					return;
				}

				sHistory.Format(_T("%s对我们在%s星球的基地发动进攻，但他们的阴谋没有得逞，我们取得了胜利！"),
								pAlien->GetName(), pStar->GetName());
			}

			SetWeaponToBarback(BS_EARTH);
			if (pAlien->IsDieOut())
			{
				// 敌对文明灭绝
				CString sTip;
				sTip.Format(_T("灿烂的星空，竟成了我们的坟墓...\n\n（%s灭绝）"), pAlien->GetName());
				CHelper::CiviMsgBox(pAlien, sTip);
				sTip.Format(_T("%s灭绝"), pAlien->GetName());
				CHelper::AddHistory(sTip);
			}
		}
		break;
	case BR_LOSE:
		{
			pStar = starMgr.GetStar(m_iMyStarIndex);
			if (!pStar)
			{
				ASSERT(0);
				return;
			}
			CBarback* pBarback = pStar->GetBarback();
			if (!pBarback)
			{
				ASSERT(0);
				return;
			}

			CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
			if (m_battleType == BT_DEFEND)
			{

				earthCivi.RemoveBarback(pBarback);
				earthCivi.RemoveStope(m_iMyStarIndex);
				earthCivi.RemoveFactory(m_iMyStarIndex);
				earthCivi.RemoveCity(m_iMyStarIndex);
				earthCivi.RemoveStar(m_iMyStarIndex);
				pStar->SetBarback(NULL);
				pStar->SetStope(NULL);
				pStar->SetFactory(NULL);
				pStar->SetCity(NULL);
				pStar->SetBelongToCivi(m_sEnemyName);
				pAlien->BuildBarback(m_iMyStarIndex, TRUE);	// 敌人占领了我的星球

				// 处理敌人的老星球。
				pStar = starMgr.GetStar(m_iEnemyStarIndex);
				if (!pStar)
				{
					ASSERT(0);
					return;
				}
				pBarback = pStar->GetBarback();
				if (!pBarback)
				{
					ASSERT(0);
					return;
				}
				pBarback->Clear(TRUE);

				pStar = starMgr.GetStar(m_iMyStarIndex);
				if (!pStar)
				{
					ASSERT(0);
					return;
				}

				sHistory.Format(_T("%s对我们在%s星球的基地发动突然袭击，我们被打败，不得不放弃基地。"),
					pAlien->GetName(), pStar->GetName());
	//			pBarback->SetSoldierCount(ENEMY_WIN_ADD_SOLDIER);	// 留敌人的100兵在老星球(后来想想没必要了)
			}
			else if (m_battleType == BT_ATTACK)
			{
				pStar = starMgr.GetStar(m_iEnemyStarIndex);
				if (!pStar)
				{
					ASSERT(0);
					return;
				}
				sHistory.Format(_T("我们在进攻%s星球的战役中被%s打败了，也许我们需要更多的支援。"),
					pStar->GetName(), pAlien->GetName());
				pBarback->Clear(TRUE);
			}

			SetWeaponToBarback(BS_ENEMY);
		}
		break;
	case BR_DRAW:
		{
			pStar = starMgr.GetStar(m_iEnemyStarIndex);
			if (!pStar)
			{
				ASSERT(0);
				return;
			}
			if (m_battleType == BT_DEFEND)
			{
				sHistory.Format(_T("%s对我们在%s星球的基地发动袭击，我们奋力反击，最终打成平手。"),
					pAlien->GetName(), pStar->GetName());
			}
			else if (m_battleType == BT_ATTACK)
			{
				sHistory.Format(_T("我们在进攻%s星球的战役遭到%s的顽强抵抗了，不得已只能选择撤退。"),
					pStar->GetName(), pAlien->GetName());
			}
			ShowBattleResult(BR_DRAW);
			SetWeaponToBarback(BS_EARTH);
			SetWeaponToBarback(BS_ENEMY);
		}
		break;
	default:
		break;
	}
	CHelper::AddHistory(sHistory);
}

CBattleDlg::BATTLE_RESULT CBattleDlg::GetBattleResult()
{
	BATTLE_RESULT battleRes = BR_INBATTLE;
	SimpleWeaponMap::iterator itr = m_mapMyArmy.begin();
	while (itr != m_mapMyArmy.end())
	{
		if (itr->second->m_Type == WT_UNIT)
		{
			break;
		}
		itr ++;
	}
	if (itr == m_mapMyArmy.end())
		battleRes = BR_LOSE;

	itr = m_mapEnemyArmy.begin();
	while (itr != m_mapEnemyArmy.end())
	{
		if (itr->second->m_Type == WT_UNIT)
		{
			break;
		}
		itr ++;
	}
	if (itr == m_mapEnemyArmy.end())
		battleRes = BR_WIN;

	return battleRes;
}

void CBattleDlg::SetWeaponToBarback(BATTLE_SIDE iType)
{
	// 将武器还原到原军营。只处理获胜方，或平局时的双方
	int iStar;
	SimpleWeaponMap mapWeapon;
	if (iType == BS_EARTH)
	{
		iStar = m_iMyStarIndex;
		mapWeapon = m_mapMyArmy;
	}
	else if (iType == BS_ENEMY)
	{
		if (m_battleType == BT_DEFEND)
			iStar = m_iMyStarIndex;		// 胜利后的敌军全移到占领的星球
		else
			iStar = m_iEnemyStarIndex;
		mapWeapon = m_mapEnemyArmy;
	}

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pStar = starMgr.GetStar(iStar);
	if (!pStar)
		return;
	CBarback* pBarback = pStar->GetBarback();
	if (!pBarback)
		return;

	int iOldParbackPop = pBarback->GetSoldierCount();
	pBarback->Clear(TRUE);

	SimpleWeaponMap::iterator itr;
	for (itr = mapWeapon.begin(); itr != mapWeapon.end(); itr ++)
	{
		if (itr->second->m_sName.Compare(_T("士兵")) == 0)
			continue;
		CWeaponEx *weaponEx = new CWeaponEx;
		weaponEx->SetWeaponName(itr->second->m_sOriginalName);
		weaponEx->SetFinish();
		pBarback->AddWeapon(weaponEx);
	}
	if (mapWeapon.find(_T("士兵")) != mapWeapon.end())
	{
		int iSoldierCnt;
		if (iType == BS_EARTH)	// 因为已方可能因使用“士气”而使士兵数比原来的多，这里要判断一下
			iSoldierCnt = mapWeapon[_T("士兵")]->m_iHp > iOldParbackPop ? iOldParbackPop : mapWeapon[_T("士兵")]->m_iHp;
		else
			iSoldierCnt = mapWeapon[_T("士兵")]->m_iHp;
		pBarback->SetSoldierCount(iSoldierCnt);
	}
	else
		pBarback->SetSoldierCount(0);

}

void CBattleDlg::UpdateBothSidesPopulation()
{
	// 此函数应在DoBattleResult()之前运行
	CAlienCivilization* pAlien = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(m_sEnemyName);
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pStar = starMgr.GetStar(m_iEnemyStarIndex);
	if (!pStar)
	{
		ASSERT(0);
		return;
	}
	CBarback* pAlienBarback = pStar->GetBarback();
	if (!pAlienBarback)
	{
		ASSERT(0);
		return;
	}
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	pStar = starMgr.GetStar(m_iMyStarIndex);
	if (!pStar)
	{
		ASSERT(0);
		return;
	}
	CBarback* pEarthBarback = pStar->GetBarback();
	if (!pEarthBarback)
	{
		ASSERT(0);
		return;
	}

	int iPop;
	int iCurSoldier;

	if (m_mapMyArmy.find(_T("士兵")) == m_mapMyArmy.end())
		iCurSoldier = 0;
	else
		iCurSoldier  = m_mapMyArmy[_T("士兵")]->m_iHp;
	iPop = earthCivi.GetPopulation() - (pEarthBarback->GetSoldierCount() - iCurSoldier);
	earthCivi.SetPopulation(iPop);

	if (m_mapEnemyArmy.find(_T("士兵")) == m_mapEnemyArmy.end())
		iCurSoldier = 0;
	else
		iCurSoldier  = m_mapEnemyArmy[_T("士兵")]->m_iHp;
	iPop = pAlien->GetPopulation() - (pAlienBarback->GetSoldierCount() - iCurSoldier);
	pAlien->SetPopulation(iPop);
}

BOOL CBattleDlg::DoGeneralSkill()
{
	int iSel = m_ctrGeneralSkill.GetCurSel();
	CString sSel;
	if (iSel > -1)
		m_ctrGeneralSkill.GetLBText(iSel, sSel);
	if (sSel.IsEmpty())
		return FALSE;

	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	CPerson* pPerson = personMgr.GetPerson(m_sLeader);
	if (!pPerson)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bFailed = FALSE;
	if (sSel.Compare(_T("收买")) == 0)
	{
		CRandomNumber randNum;
		int iTotalVal = pPerson->GetArmy() + pPerson->GetLeadership();
		if (randNum.random(200) <= iTotalVal)
		{
			int iMoney = randNum.random(200);		// 收买的价码，临时设的 for test
			iMoney += 200;
//			iMoney = 1;	// for test
			CString sFormat;
			sFormat.Format(_T("该死的地球人，我可以为你们透露一些情报，但你们得给我 %d 点经济作报酬才行。"), iMoney);
			CAlienCivilization* pAlien = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(m_sEnemyName);

			if (CHelper::CiviMsgBox(pAlien, sFormat, TRUE) == IDOK)
			{
				CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
				int iEconomy = earthCivi.GetEconomy();
				if (iEconomy < iMoney)
				{
					CString sInfo = _T("你开的价码太高了，这简直是讹诈！");
					CHelper::PersonMsgBox(m_sLeader, sInfo, sInfo);
				}
				else
				{
					CHelper::CiviMsgBox(pAlien, _T("你作出了明智的选择，虫子。现在，我将为你服务，地球人！"));
					earthCivi.SetEconomy(iEconomy - iMoney);
					SSimpleWeapon *pWeapon = new SSimpleWeapon;
					pWeapon->m_iOwner = BS_EARTH;
					pWeapon->m_iAttack = 230;		// 收买的威力，临时设的 for test
					pWeapon->m_Type = WE_EXPENDABLE;
					pWeapon->m_sName = _T("收买技");
					AttackProc(pWeapon);
					SAFE_DELETE(pWeapon)
				}
			}
		}
		else
		{
			bFailed = TRUE;
		}
	}
	else if (sSel.Compare(_T("士气")) == 0)
	{
		CRandomNumber randNum;
		int iTotalVal = pPerson->GetArmy() + pPerson->GetArt() + pPerson->GetLeadership();
		if (randNum.random(300) <= iTotalVal)
		{
			if (m_mapMyArmy.find(_T("士兵")) != m_mapMyArmy.end())
				m_mapMyArmy[_T("士兵")]->m_iHp *= 2;

			CString sInfo = _T("下面不到一个小时内，这里的士兵将会加入一场光荣的战斗。“人类”这个词----对我们\
来说今天应该有新的意义。为了我们的自由，为了我们的梦想，我们需要赢下这场战役。现在，握紧你们\
干渴的兵器，让那片星海在你们的怒火中震颤吧！\n(士兵兵力上升了一倍)");
			CHelper::PersonMsgBox(m_sLeader, sInfo, sInfo);
		}
		else
		{
			bFailed = TRUE;
		}
	}
	else if (sSel.Compare(_T("恐吓")) == 0)
	{
		CRandomNumber randNum;
		int iTotalVal = pPerson->GetArmy() + pPerson->GetArt();
		if (randNum.random(200) <= iTotalVal)
		{
			CString sInfo;
			sInfo.Format(_T("%s军，强大的地球军队已经将你们完全包围，你们将为你们无知的抵抗付出代价！"), m_sEnemyName);
			CHelper::PersonMsgBox(m_sLeader, sInfo, sInfo);
			CAlienCivilization* pAlien = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(m_sEnemyName);
			CHelper::CiviMsgBox(pAlien, _T("我闻到了死亡的味道。"));

			SSimpleWeapon *pWeapon = new SSimpleWeapon;
			pWeapon->m_iOwner = BS_EARTH;
			pWeapon->m_iAttack = 200;		// 恐吓的威力，临时设的 for test
			pWeapon->m_Type = WE_EXPENDABLE;
			pWeapon->m_sName = _T("恐吓技");
			AttackProc(pWeapon);
			SAFE_DELETE(pWeapon)
		}
		else
		{
			bFailed = TRUE;
		}
	}
/*	case _T("卧底"):
		CRandomNumber randNum;
		int iTotalVal = pPerson->GetTreachery();
		if (randNum.random(100) > iTotalVal)
		{

		}
		break;*/
	else if (sSel.Compare(_T("撤军")) == 0)
	{
		DoBattleResult(BR_DRAW);
		CDialog::OnOK();
		return TRUE;
	}

	CString sInfo;
	if (bFailed)
	{
		sInfo.Format(SKILL_FAIL_INFO, sSel);
		OutputBattleInfo(sInfo);
	}
	m_bUseGeneralSkill = TRUE;
	return FALSE;
}

void CBattleDlg::ShowBattleResult(BATTLE_RESULT battleRes)
{
	if (battleRes == BR_WIN) //取胜
	{
		CString sInfo = _T("我们赢得了这场战争！\n士兵们，你们的勇气让我震惊，你们的行动将告诉我们的公民\
，我们地球文明在宇宙中会扮演什么样的角色！");
		CHelper::PersonMsgBox(m_sLeader, sInfo, sInfo);
	}
	else if (battleRes == BR_LOSE) // 失败
	{
		CString sInfo = _T("我承认，失败的滋味并不好！但是现在，士兵们，公民们，我们必须面对这样的现实，\
这暂时的现实。未来的一天，我们会重返旧地，夺回我们的领土！");
		CHelper::PersonMsgBox(m_sLeader, sInfo, sInfo);
	}
	else if (battleRes == BR_DRAW) // 平局
	{
		CString sInfo = _T("敌人太强大了！撤退也许不是一个勇敢者的选择，但绝对是一个智者的选择。总有一天我们会回来。");
		CHelper::PersonMsgBox(m_sLeader, sInfo, sInfo);
	}

}
void CBattleDlg::OnCbnSelchangeComboWeapon()
{
	int iSel = m_ctrMyWeaponCombo.GetCurSel();
	CString sSel;
	if (iSel > -1)
		m_ctrMyWeaponCombo.GetLBText(iSel, sSel);
	if (sSel.IsEmpty())
	{
		m_toolTipDlg.RemoveTool(GetDlgItem(IDC_COMBO_WEAPON));
		return;
	}

	CString sInfo;
	sInfo.Format(_T("名称：%s\n战斗力：%d"), m_mapMyArmy[sSel]->m_sName, m_mapMyArmy[sSel]->m_iAttack);
	m_toolTipDlg.SetTool(GetDlgItem(IDC_COMBO_WEAPON), sInfo, sInfo);
}

void CBattleDlg::OnCbnSelchangeCombo2()
{
	int iSel = m_ctrGeneralSkill.GetCurSel();
	CString sSel;
	if (iSel > -1)
		m_ctrGeneralSkill.GetLBText(iSel, sSel);
	if (sSel.IsEmpty())
	{
		m_toolTipDlg.RemoveTool(GetDlgItem(IDC_COMBO2));
		return;
	}

	CString sInfo;
	if (sSel == _T("收买"))
	{
		sInfo = _T("用一定的经济值收买敌方将领，达到对敌军的破坏效果。");
	}
	else if (sSel == _T("士气"))
	{
		sInfo = _T("通过鼓舞士气加强我军的战斗力。");
	}
	else if (sSel == _T("恐吓"))
	{
		sInfo = _T("以我军实力恐吓敌人，使其自动退兵。");
	}
	else if (sSel == _T("撤军"))
	{
		sInfo = _T("全军撤退。");
	}
	m_toolTipDlg.SetTool(GetDlgItem(IDC_COMBO2), sInfo, sInfo);
}

BOOL CBattleDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint pt = pMsg->pt;	
		ScreenToClient( &pt );
		m_toolTipDlg.Move(pt);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CBattleDlg::InitToolTip()
{
	m_toolTipDlg.Create(IDD_TOOLTIP_DLG, this);
	m_toolTipDlg.ShowWindow(SW_HIDE);
	OnCbnSelchangeComboWeapon();
	OnCbnSelchangeCombo2();

}

void CBattleDlg::InitTitle()
{
	int iStarIndex;
	if (m_battleType == BT_ATTACK)
		iStarIndex = m_iEnemyStarIndex;
	else 
		iStarIndex = m_iMyStarIndex;
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pStar = starMgr.GetStar(iStarIndex);
	if (!pStar)
	{
		ASSERT(FALSE);
		return;
	}
	
	CString sTitle;
	sTitle.Format(_T("战场： %s"), pStar->GetName());
	SetWindowText(sTitle);
}