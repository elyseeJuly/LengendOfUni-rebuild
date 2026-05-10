// EconomyDepartmentPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "EconomyDepartmentPanel.h"
#include "EconomyDepartment.h"
#include "Person.h"
#include "helper.h"

#define MINING_TIP _T("采矿人口： %d 人")
#define MANUFACTURE_TIP _T("生产人口： %d 人")
// CEconomyDepartmentPanel 对话框

IMPLEMENT_DYNAMIC(CEconomyDepartmentPanel, CDialog)

CEconomyDepartmentPanel::CEconomyDepartmentPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CEconomyDepartmentPanel::IDD, pParent)
	, m_sLeader(_T(""))
	, m_sMiningWorkerCnt(_T(""))
	, m_sManufactureWorkerCnt(_T(""))
{

}

CEconomyDepartmentPanel::~CEconomyDepartmentPanel()
{
}

void CEconomyDepartmentPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_LEADER, m_sLeader);
	DDX_Text(pDX, IDC_STATIC_MINING, m_sMiningWorkerCnt);
	DDX_Text(pDX, IDC_STATIC_MANUFACTURE, m_sManufactureWorkerCnt);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrFace);
}


BEGIN_MESSAGE_MAP(CEconomyDepartmentPanel, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CEconomyDepartmentPanel::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CEconomyDepartmentPanel::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_CHGLEADER, &CEconomyDepartmentPanel::OnBnClickedButtonChgleader)
END_MESSAGE_MAP()


// CEconomyDepartmentPanel 消息处理程序

BOOL CEconomyDepartmentPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateUI();
	if (!IsWindow(m_toolTipDlg.GetSafeHwnd()))
		InitToolTip();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CEconomyDepartmentPanel::UpdateUI()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CEconomyDepartment *department = (CEconomyDepartment*)earthCivi.GetDepartment(DT_ECONOMY);
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

	m_sMiningWorkerCnt.Format(MINING_TIP, department->GetMiningWorkerCount());
	m_sManufactureWorkerCnt.Format(MANUFACTURE_TIP, department->GetManufactureWorkerCount());

	UpdateData(FALSE);
	Invalidate();
}
void CEconomyDepartmentPanel::OnOK()
{

}

void CEconomyDepartmentPanel::OnCancel()
{

}

void CEconomyDepartmentPanel::OnBnClickedButton2()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CEconomyDepartment *department = (CEconomyDepartment*)earthCivi.GetDepartment(DT_ECONOMY);
	int iNewCnt, iOldCnt;
	iOldCnt = department->GetMiningWorkerCount();
	iNewCnt = CHelper::ChangeWorkerCount(iOldCnt);
	m_sMiningWorkerCnt.Format(MINING_TIP, iNewCnt);
	department->SetMiningWorkerCount(iNewCnt);

	int iIdlePop = earthCivi.GetIdlePopulation();
	earthCivi.SetIdlePopulation(iIdlePop + iOldCnt - iNewCnt);

	UpdateData(FALSE);
}

void CEconomyDepartmentPanel::OnBnClickedButton3()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CEconomyDepartment *department = (CEconomyDepartment*)earthCivi.GetDepartment(DT_ECONOMY);
	int iNewCnt, iOldCnt;
	iOldCnt = department->GetManufactureWorkerCount();
	iNewCnt = CHelper::ChangeWorkerCount(iOldCnt);
	m_sManufactureWorkerCnt.Format(MANUFACTURE_TIP, iNewCnt);
	department->SetManufactureWorkerCount(iNewCnt);

	int iIdlePop = earthCivi.GetIdlePopulation();
	earthCivi.SetIdlePopulation(iIdlePop + iOldCnt - iNewCnt);

	UpdateData(FALSE);
}

void CEconomyDepartmentPanel::OnBnClickedButtonChgleader()
{
	CString sName = CHelper::ChoosePerson();
	if (sName.IsEmpty())
		return;

	m_sLeader = sName;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CDepartment *department = (CDepartment*)earthCivi.GetDepartment(DT_ECONOMY);
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

void CEconomyDepartmentPanel::InitToolTip()
{
	m_toolTipDlg.Create(IDD_TOOLTIP_DLG, this);
	m_toolTipDlg.ShowWindow(SW_HIDE);
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON2), _T("设置参与采矿工作的人口数。人口数越大，工作成就越高。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON3), _T("设置参与生产工作的人口数。人口数越大，工作成就越高。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_CHGLEADER), _T("设置该部门的部长。优秀的部长将给该部门带来更高的效率。"));
}

BOOL CEconomyDepartmentPanel::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint pt = pMsg->pt;	
		ScreenToClient( &pt );
		m_toolTipDlg.Move(pt);
	}
	return CDialog::PreTranslateMessage(pMsg);
}
