// BarbackDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "BarbackDlg.h"
#include "helper.h"
#include "LengendOfUniDlg.h"

// CBarbackDlg 对话框
#define SOLDIER_TIP _T("士兵： %d　人")
#define WEAPON_PRODUCE_TIP _T("生产进度： %d / %d")

IMPLEMENT_DYNAMIC(CBarbackDlg, CDialog)

CBarbackDlg::CBarbackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBarbackDlg::IDD, pParent)
	, m_sLeader(_T(""))
	, m_sSolderCntTip(_T(""))
	, m_sBarbackName(_T(""))
{
	m_pBarback = NULL;
	m_sProduceWeapon[0].Empty();
	m_sProduceWeapon[1].Empty();
	m_sProduceWeapon[2].Empty();
}

CBarbackDlg::~CBarbackDlg()
{
}

void CBarbackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_LEADER, m_sLeader);
	DDX_Text(pDX, IDC_STATIC_WORKER, m_sSolderCntTip);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrFace);
	DDX_Control(pDX, IDC_LIST_WEAPON, m_ctrWeaponList);
	DDX_Control(pDX, IDC_COMBO_WEAPON, m_ctrWeaponCombo[0]);
	DDX_Control(pDX, IDC_COMBO_WEAPON2, m_ctrWeaponCombo[1]);
	DDX_Control(pDX, IDC_COMBO_WEAPON3, m_ctrWeaponCombo[2]);
	DDX_Text(pDX, IDC_STATIC_PROCESS, m_sProduceWeapon[0]);
	DDX_Text(pDX, IDC_STATIC_PROCESS2, m_sProduceWeapon[1]);
	DDX_Text(pDX, IDC_STATIC_PROCESS3, m_sProduceWeapon[2]);
	DDX_Text(pDX, IDC_STATIC_BARBACKNAME, m_sBarbackName);
}


BEGIN_MESSAGE_MAP(CBarbackDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHGLEADER, &CBarbackDlg::OnBnClickedButtonChgleader)
	ON_BN_CLICKED(IDC_BUTTON_MODNUM, &CBarbackDlg::OnBnClickedButtonModnum)
	ON_BN_CLICKED(IDC_BUTTON_PRODUCE0, &CBarbackDlg::OnBnClickedButtonProduce0)
	ON_BN_CLICKED(IDC_BUTTON_PRODUCE1, &CBarbackDlg::OnBnClickedButtonProduce1)
	ON_BN_CLICKED(IDC_BUTTON_PRODUCE2, &CBarbackDlg::OnBnClickedButtonProduce2)
	ON_CBN_SELCHANGE(IDC_COMBO_WEAPON, &CBarbackDlg::OnCbnSelchangeComboWeapon)
	ON_CBN_SELCHANGE(IDC_COMBO_WEAPON2, &CBarbackDlg::OnCbnSelchangeComboWeapon2)
	ON_CBN_SELCHANGE(IDC_COMBO_WEAPON3, &CBarbackDlg::OnCbnSelchangeComboWeapon3)
END_MESSAGE_MAP()


// CBarbackDlg 消息处理程序

void CBarbackDlg::OnBnClickedButtonChgleader()
{
	if (!m_pBarback)
		return;

	CString sName = CHelper::ChoosePerson();
	if (sName.IsEmpty())
		return;

	m_sLeader = sName;

	CDepartment *pDep = m_pBarback->GetDepartment();
	CHelper::ChangePersonDepartment(m_sLeader, pDep);

	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	CPerson* person = personMgr.GetPerson(m_sLeader);
	if (person)
	{
		HBITMAP hBmp;
		CString sFace;
		sFace = person->GetFaceFile();
		hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sFace, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

		m_ctrFace.SetBitmap(hBmp);
	}

	UpdateData(FALSE);
	Invalidate();
}

void CBarbackDlg::OnBnClickedButtonModnum()
{
	if (!m_pBarback)
		return;

	int iNewCnt, iOldCnt;
	iOldCnt = m_pBarback->GetSoldierCount();
	iNewCnt = CHelper::ChangeWorkerCount(iOldCnt);
	m_sSolderCntTip.Format(SOLDIER_TIP, iNewCnt);
	m_pBarback->SetSoldierCount(iNewCnt);

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	int iIdlePop = earthCivi.GetIdlePopulation();
	earthCivi.SetIdlePopulation(iIdlePop + iOldCnt - iNewCnt);

	UpdateData(FALSE);
}


void CBarbackDlg::Init(CBarback* pBarback)
{
	if (!pBarback)
		return;

	m_pBarback = pBarback;
}

void CBarbackDlg::InitUI()
{
	if (!m_pBarback)
		return;

	m_sBarbackName = m_pBarback->GetDepartment()->GetName();

	// 初始化头像
	CDepartment *pDep = m_pBarback->GetDepartment();
	HBITMAP hBmp;
	m_sLeader = pDep->GetLeader();
	if (m_sLeader.IsEmpty())
	{
		m_sLeader = _T("无");
		UINT iFace = IDB_NONAME;

		hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(iFace));
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


	// 初始化兵数
	m_sSolderCntTip.Format(SOLDIER_TIP, m_pBarback->GetSoldierCount());

	// 初始化武器
	int iComboIndex = 0;
	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	WeaponExList& weaponExLst = m_pBarback->GetWeaponExList();
	WeaponExList::iterator itr = weaponExLst.begin();

	while (itr != weaponExLst.end())
	{
		if ((*itr)->GetIsFinish())
		{
			// 已建武器
			m_ctrWeaponList.AddString((*itr)->GetWeaponName());
		}
		else
		{
			if (iComboIndex < 3)
			{
				// 在建武器
				m_ctrWeaponCombo[iComboIndex].AddString((*itr)->GetWeaponName());
				m_ctrWeaponCombo[iComboIndex].SetCurSel(0);
				m_sProduceWeapon[iComboIndex].Format(WEAPON_PRODUCE_TIP, (*itr)->GetCurrentBuild(), (*itr)->GetTotalBuild());
				GetDlgItem(IDC_BUTTON_PRODUCE0 + iComboIndex)->EnableWindow(FALSE);
				iComboIndex ++;
			}
		}
		itr ++;
	}

	if (iComboIndex < 3)
	{
		// 可建的武器
		CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
		CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
		WeaponMap &weaponMap = weaponMgr.GetWeaponMap();

		for (int i = iComboIndex; i < 3; i ++)
		{
			WeaponMap::iterator itr = weaponMap.begin();
			while (itr != weaponMap.end())
			{
				WEAPON_TYPE weaponType = itr->second.GetType();
				if (weaponType == WT_UNIT || weaponType == WE_EXPENDABLE)
				{
					CString sTec = itr->second.GetDependTecName();
					TEC_TREE_TYPE ttype = itr->second.GetDependTecType();
					if (!sTec.IsEmpty())
					{
						BOOL bFinish = tecTreeMgr.IsTecFinished(ttype, sTec);
						if (bFinish)
						{
							m_ctrWeaponCombo[i].AddString(itr->second.GetName());
							m_sProduceWeapon[i].Empty();
						}
					}
				}
				itr ++;
			}
			m_ctrWeaponCombo[i].SetCurSel(0);
		}
	}

	UpdateData(FALSE);
	Invalidate();
}

BOOL CBarbackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitUI();
	InitToolTip();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBarbackDlg::OnBnClickedButtonProduce0()
{
	if (!m_pBarback)
		return;

	int iSel = m_ctrWeaponCombo[0].GetCurSel();
	CString sSel;
	m_ctrWeaponCombo[0].GetLBText(iSel, sSel);
	if (sSel.IsEmpty())
		return;

	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	CWeapon &weapon = weaponMgr.GetWeapon(sSel);
	// 比较经济，看是否允许造。可以写一个公用函数
	if (!CHelper::CanUseEconemy(weapon.GetCost(), TRUE))
	{
		CString sInfo;
		sInfo.Format(_T("执政官，我们的经济少于 %d 点，无法生产这个武器。/n我请求您增加军费，让前线的兄弟能用上最新的武器。地球文明的存亡，全在于我们的军队是否强大。"), weapon.GetCost());
		CString sInfoDef;
		sInfoDef.Format(_T("我们的经济少于 %d 点，无法生产这个武器。"), weapon.GetCost());
		CHelper::PersonMsgBox(m_sLeader, sInfo, sInfoDef);
		return;
	}
	CLengendOfUniDlg* pDlg = (CLengendOfUniDlg*)GetParent();
	pDlg->UpdateValues();


	CWeaponEx *weaponEx = new CWeaponEx;
	weaponEx->SetWeaponName(sSel);
	m_pBarback->AddWeapon(weaponEx);
	GetDlgItem(IDC_BUTTON_PRODUCE0)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_WEAPON)->EnableWindow(FALSE);
}

void CBarbackDlg::OnBnClickedButtonProduce1()
{
	if (!m_pBarback)
		return;

	int iSel = m_ctrWeaponCombo[1].GetCurSel();
	CString sSel;
	m_ctrWeaponCombo[1].GetLBText(iSel, sSel);
	if (sSel.IsEmpty())
		return;

	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	CWeapon &weapon = weaponMgr.GetWeapon(sSel);
	// 比较经济，看是否允许造。可以写一个公用函数
	if (!CHelper::CanUseEconemy(weapon.GetCost(), TRUE))
	{
		CString sInfo;
		sInfo.Format(_T("执政官，我们的经济少于 %d 点，无法生产这个武器。/n我请求您增加军费，让前线的兄弟能用上最新的武器。地球文明的存亡，全在于我们的军队是否强大。"), weapon.GetCost());
		CString sInfoDef;
		sInfoDef.Format(_T("我们的经济少于 %d 点，无法生产这个武器。"), weapon.GetCost());
		CHelper::PersonMsgBox(m_sLeader, sInfo, sInfoDef);
		return;
	}
	CLengendOfUniDlg* pDlg = (CLengendOfUniDlg*)GetParent();
	pDlg->UpdateValues();

	CWeaponEx *weaponEx = new CWeaponEx;
	weaponEx->SetWeaponName(sSel);
	m_pBarback->AddWeapon(weaponEx);
	GetDlgItem(IDC_BUTTON_PRODUCE1)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_WEAPON2)->EnableWindow(FALSE);
}

void CBarbackDlg::OnBnClickedButtonProduce2()
{
	if (!m_pBarback)
		return;

	int iSel = m_ctrWeaponCombo[2].GetCurSel();
	CString sSel;
	m_ctrWeaponCombo[2].GetLBText(iSel, sSel);
	if (sSel.IsEmpty())
		return;

	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	CWeapon &weapon = weaponMgr.GetWeapon(sSel);
	// 比较经济，看是否允许造。可以写一个公用函数
	if (!CHelper::CanUseEconemy(weapon.GetCost(), TRUE))
	{
		CString sInfo;
		sInfo.Format(_T("执政官，我们的经济少于 %d 点，无法生产这个武器。我请求您增加军费，让前线的兄弟能用上最新的武器。地球文明的存亡，全在于我们的军队是否强大。"), weapon.GetCost());
		CString sInfoDef;
		sInfoDef.Format(_T("我们的经济少于 %d 点，无法生产这个武器。"), weapon.GetCost());
		CHelper::PersonMsgBox(m_sLeader, sInfo, sInfoDef);
		return;
	}
	CLengendOfUniDlg* pDlg = (CLengendOfUniDlg*)GetParent();
	pDlg->UpdateValues();

	CWeaponEx *weaponEx = new CWeaponEx;
	weaponEx->SetWeaponName(sSel);
	m_pBarback->AddWeapon(weaponEx);
	GetDlgItem(IDC_BUTTON_PRODUCE2)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_WEAPON3)->EnableWindow(FALSE);
}

BOOL CBarbackDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint pt = pMsg->pt;	
		ScreenToClient( &pt );
		m_toolTipDlg.Move(pt);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CBarbackDlg::InitToolTip()
{
	m_toolTipDlg.Create(IDD_TOOLTIP_DLG, this);
	m_toolTipDlg.ShowWindow(SW_HIDE);
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_CHGLEADER), _T("优秀的将军将使我军的战斗力提升，并能提供更多的将军技能。"));
//	m_toolTipDlg.AddTool(GetDlgItem(IDC_COMBO_WEAPON), _T("123456"));
//	m_toolTipDlg.AddTool(GetDlgItem(IDC_COMBO_WEAPON2), _T("abcdefg"));
//	m_toolTipDlg.AddTool(GetDlgItem(IDC_COMBO_WEAPON3), _T("123456"));
	OnCbnSelchangeComboWeapon();
	OnCbnSelchangeComboWeapon2();
	OnCbnSelchangeComboWeapon3();

}
void CBarbackDlg::OnCbnSelchangeComboWeapon()
{
	int iSel = m_ctrWeaponCombo[0].GetCurSel();
	if (iSel < 0)
		return;
	CString sSel;
	m_ctrWeaponCombo[0].GetLBText(iSel, sSel);
	if (sSel.IsEmpty())
		return;

	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	CWeapon &weapon = weaponMgr.GetWeapon(sSel);

	CString sWeaponType[] = {_T("作战单元"), _T("炸弹")};
	CString sInfo;
	sInfo.Format(_T("名称：%s\n类型：%s\n造价：%d\n战斗力：%d"),
				weapon.GetName(), sWeaponType[weapon.GetType()],
				weapon.GetCost(), weapon.GetAttack());
	if (weapon.GetType() == WT_UNIT)
	{
		CString sAppend;
		sAppend.Format(_T("\n兵力：%d"), weapon.GetHp());
		sInfo += sAppend;
	}
	m_toolTipDlg.SetTool(GetDlgItem(IDC_COMBO_WEAPON), sInfo, sInfo);
}

void CBarbackDlg::OnCbnSelchangeComboWeapon2()
{
	int iSel = m_ctrWeaponCombo[1].GetCurSel();
	if (iSel < 0)
		return;
	CString sSel;
	m_ctrWeaponCombo[1].GetLBText(iSel, sSel);
	if (sSel.IsEmpty())
		return;

	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	CWeapon &weapon = weaponMgr.GetWeapon(sSel);

	CString sWeaponType[] = {_T("作战单元"), _T("炸弹")};
	CString sInfo;
	sInfo.Format(_T("名称：%s\n类型：%s\n造价：%d\n战斗力：%d"),
		weapon.GetName(), sWeaponType[weapon.GetType()],
		weapon.GetCost(), weapon.GetAttack());
	if (weapon.GetType() == WT_UNIT)
	{
		CString sAppend;
		sAppend.Format(_T("\n兵力：%d"), weapon.GetHp());
		sInfo += sAppend;
	}
	m_toolTipDlg.SetTool(GetDlgItem(IDC_COMBO_WEAPON2), sInfo, sInfo);
}

void CBarbackDlg::OnCbnSelchangeComboWeapon3()
{
	int iSel = m_ctrWeaponCombo[2].GetCurSel();
	if (iSel < 0)
		return;
	CString sSel;
	m_ctrWeaponCombo[2].GetLBText(iSel, sSel);
	if (sSel.IsEmpty())
		return;

	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	CWeapon &weapon = weaponMgr.GetWeapon(sSel);

	CString sWeaponType[] = {_T("作战单元"), _T("炸弹")};
	CString sInfo;
	sInfo.Format(_T("名称：%s\n类型：%s\n造价：%d\n战斗力：%d"),
		weapon.GetName(), sWeaponType[weapon.GetType()],
		weapon.GetCost(), weapon.GetAttack());
	if (weapon.GetType() == WT_UNIT)
	{
		CString sAppend;
		sAppend.Format(_T("\n兵力：%d"), weapon.GetHp());
		sInfo += sAppend;
	}
	m_toolTipDlg.SetTool(GetDlgItem(IDC_COMBO_WEAPON3), sInfo, sInfo);
}
