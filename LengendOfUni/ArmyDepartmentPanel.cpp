// ArmyDepartmentPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "ArmyDepartmentPanel.h"
#include "Person.h"
#include "helper.h"
#include "ArmyDepartment.h"
#include "BarbackDlg.h"

// CArmyDepartmentPanel 对话框
#define BOMB_CNT_STRING _T("奇点炸弹数：%d")
#define ZHIZI_CNT_STRING _T("智子数：%d")
#define PERCENT_STRING _T("进度：%d/%d")

IMPLEMENT_DYNAMIC(CArmyDepartmentPanel, CDialog)

CArmyDepartmentPanel::CArmyDepartmentPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CArmyDepartmentPanel::IDD, pParent)
	, m_sLeader(_T(""))
	, m_sBombCnt(_T(""))
	, m_sZhiziCnt(_T(""))
	, m_sBombPercent(_T(""))
	, m_sZhiziPercent(_T(""))
{

}

CArmyDepartmentPanel::~CArmyDepartmentPanel()
{
}

void CArmyDepartmentPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrFace);
	DDX_Text(pDX, IDC_STATIC_LEADER, m_sLeader);
	DDX_Text(pDX, IDC_STATIC_BOMECNT, m_sBombCnt);
	DDX_Text(pDX, IDC_STATIC_ZHIZICNT, m_sZhiziCnt);
	DDX_Text(pDX, IDC_STATIC_BOMB_PERCENT, m_sBombPercent);
	DDX_Text(pDX, IDC_STATIC_ZHIZI_PERCENT, m_sZhiziPercent);
	DDX_Control(pDX, IDC_COMBO_BARBACK, m_ctrBarbackCombo);
}


BEGIN_MESSAGE_MAP(CArmyDepartmentPanel, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHGLEADER, &CArmyDepartmentPanel::OnBnClickedButtonChgleader)
	ON_BN_CLICKED(IDC_BUTTON_PRODUCEBOMB, &CArmyDepartmentPanel::OnBnClickedButtonProducebomb)
	ON_BN_CLICKED(IDC_BUTTON_PRODUCEZHIZI, &CArmyDepartmentPanel::OnBnClickedButtonProducezhizi)
	ON_BN_CLICKED(IDC_BUTTON7, &CArmyDepartmentPanel::OnBnClickedButton7)
END_MESSAGE_MAP()


// CArmyDepartmentPanel 消息处理程序
BOOL CArmyDepartmentPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateUI();
	if (!IsWindow(m_toolTipDlg.GetSafeHwnd()))
		InitToolTip();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CArmyDepartmentPanel::UpdateUI()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CArmyDepartment *department = (CArmyDepartment*)earthCivi.GetDepartment(DT_ARMY);
	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();

	HBITMAP hBmp;
	m_sLeader = department->GetLeader();
	if (m_sLeader.IsEmpty())
	{
		m_sLeader = _T("无");
		UINT iFace = IDB_NONAME;

		hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(iFace));
		m_ctrFace.SetBitmap(hBmp);
	}
	else
	{
		CPerson* person = personMgr.GetPerson(m_sLeader);
		if (person)
		{
			CString sFace;
			sFace = person->GetFaceFile();
			hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sFace, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
			m_ctrFace.SetBitmap(hBmp);
		}
	}

	UpdateWeaponUI();
	InitBarbackCombo();

	UpdateData(FALSE);
	Invalidate();
}
void CArmyDepartmentPanel::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

}

void CArmyDepartmentPanel::OnCancel()
{
}

void CArmyDepartmentPanel::OnBnClickedButtonChgleader()
{
	CString sName = CHelper::ChoosePerson();
	if (sName.IsEmpty())
		return;

	m_sLeader = sName;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CArmyDepartment *department = (CArmyDepartment*)earthCivi.GetDepartment(DT_ARMY);
	CHelper::ChangePersonDepartment(m_sLeader, department);

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

void CArmyDepartmentPanel::OnBnClickedButtonProducebomb()
{
	// 生产奇点炸弹
	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	CWeapon &weapon = weaponMgr.GetWeapon(_T("奇点炸弹"));
	// 比较经济，看是否允许造。可以写一个公用函数
	if (!CHelper::CanUseEconemy(weapon.GetCost(), TRUE))
	{
		CString sInfo = _T("执政官，我们目前的经济实力还不能生产出奇点炸弹。\n作为军事部长，我非常遗憾。只有当强有力的武器装备我军，我们地球文明才能称霸宇宙。");
		CString sInfoDef = _T("我们目前的经济实力还不能生产出奇点炸弹。但是，我们未来会有的！");
		CHelper::PersonMsgBox(m_sLeader, sInfo, sInfoDef);
		return;
	}

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CArmyDepartment *department = (CArmyDepartment*)earthCivi.GetDepartment(DT_ARMY);

	CWeaponEx *weaponEx = new CWeaponEx;
	weaponEx->SetWeaponName(_T("奇点炸弹"));
	department->AddBomb(weaponEx);

	UpdateWeaponUI();
}

void CArmyDepartmentPanel::UpdateWeaponUI()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CArmyDepartment *department = (CArmyDepartment*)earthCivi.GetDepartment(DT_ARMY);

	m_sBombCnt.Format(BOMB_CNT_STRING, department->GetBombCount());
	m_sZhiziCnt.Format(ZHIZI_CNT_STRING, department->GetZhiziCount());

	BOOL bFinish = FALSE;
	BOOL bRet;
	int iCurBuild;
	int iTotalBuild;
	// 奇点炸弹
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	BOOL bTecFinish = tecTreeMgr.IsTecFinished(TT_PROTON, _T("奇点炸弹"));
	if (bTecFinish)
	{
		bRet = department->GetFirstBombState(bFinish, iCurBuild, iTotalBuild);
		if (bRet && !bFinish)
		{
			GetDlgItem(IDC_BUTTON_PRODUCEBOMB)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_BOMB_PERCENT)->ShowWindow(SW_SHOW);
			m_sBombPercent.Format(PERCENT_STRING, iCurBuild, iTotalBuild);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_PRODUCEBOMB)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_BOMB_PERCENT)->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PRODUCEBOMB)->EnableWindow(FALSE);
	}

	// 智子
	bTecFinish = tecTreeMgr.IsTecFinished(TT_PROTON, _T("智子"));
	if (bTecFinish)
	{
		bFinish = FALSE;
		bRet = department->GetFirstZhiziState(bFinish, iCurBuild, iTotalBuild);
		if (bRet && !bFinish)
		{
			GetDlgItem(IDC_BUTTON_PRODUCEZHIZI)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_ZHIZI_PERCENT)->ShowWindow(SW_SHOW);
			m_sZhiziPercent.Format(PERCENT_STRING, iCurBuild, iTotalBuild);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_PRODUCEZHIZI)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_ZHIZI_PERCENT)->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PRODUCEZHIZI)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}
void CArmyDepartmentPanel::OnBnClickedButtonProducezhizi()
{
	// 生产智子
	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	CWeapon &weapon = weaponMgr.GetWeapon(_T("智子"));
	// 比较经济，看是否允许造。可以写一个公用函数
	if (!CHelper::CanUseEconemy(weapon.GetCost(), TRUE))
	{
		CString sInfo = _T("执政官，我们目前的经济实力还不能生产出智子。\n您也许需要调整我们的经济计划。");
		CString sInfoDef = _T("我们目前的经济实力还不能生产出智子。但是，我们未来会有的！");
		CHelper::PersonMsgBox(m_sLeader, sInfo, sInfoDef);
		return;
	}

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CArmyDepartment *department = (CArmyDepartment*)earthCivi.GetDepartment(DT_ARMY);

	CWeaponEx *weaponEx = new CWeaponEx;
	weaponEx->SetWeaponName(_T("智子"));
	department->AddZhizi(weaponEx);

	UpdateWeaponUI();
}

void CArmyDepartmentPanel::InitBarbackCombo()
{
	m_ctrBarbackCombo.ResetContent();

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	std::list<CBarback*>& lstBarback = earthCivi.GetBarbackList();
	std::list<CBarback*>::iterator itr = lstBarback.begin();
	while (itr != lstBarback.end())
	{
		m_ctrBarbackCombo.AddString((*itr)->GetDepartment()->GetName());
		itr ++;
	}
	m_ctrBarbackCombo.SetCurSel(0);
}
void CArmyDepartmentPanel::OnBnClickedButton7()
{
	// 定位军营
	int iSel = m_ctrBarbackCombo.GetCurSel();
	if (iSel < 0)
		return;
	CString sSel;
	m_ctrBarbackCombo.GetLBText(iSel, sSel);

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	std::list<CBarback*>& lstBarback = earthCivi.GetBarbackList();
	std::list<CBarback*>::iterator itr = lstBarback.begin();
	std::list<CBarback*>::iterator itrSel = lstBarback.end();
	while (itr != lstBarback.end())
	{
		if (sSel.Compare((*itr)->GetDepartment()->GetName()) == 0)
		{
			itrSel = itr;
			break;
		}
		itr ++;
	}
	if (itrSel != lstBarback.end())
	{
		CBarbackDlg dlg;
		dlg.Init(*itr);
		dlg.DoModal();
	}

}

BOOL CArmyDepartmentPanel::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint pt = pMsg->pt;	
		ScreenToClient( &pt );
		m_toolTipDlg.Move(pt);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CArmyDepartmentPanel::InitToolTip()
{
	m_toolTipDlg.Create(IDD_TOOLTIP_DLG, this);
	m_toolTipDlg.ShowWindow(SW_HIDE);
	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	CWeapon &weapon = weaponMgr.GetWeapon(_T("奇点炸弹"));
	CString sInfo;
	sInfo.Format(_T("制造奇点炸弹将消耗%d点经济。奇点炸弹可以直接将一颗星球摧毁。"), weapon.GetCost());
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_PRODUCEBOMB), sInfo, _T("您需要在“质子技术部”发展“奇点炸弹”科技后才能操作。"));
	CWeapon &weapon1 = weaponMgr.GetWeapon(_T("智子"));
	sInfo.Format(_T("制造智子将消耗%d点经济。智子可以监测出其它文明星球的军事实力。"), weapon1.GetCost());
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_PRODUCEZHIZI), sInfo, _T("您需要在“质子技术部”发展“智子”科技后才能操作。"));
}