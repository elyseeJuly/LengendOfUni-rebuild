// SelectArmyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "SelectArmyDlg.h"


// CSelectArmyDlg 对话框

IMPLEMENT_DYNAMIC(CSelectArmyDlg, CDialog)

CSelectArmyDlg::CSelectArmyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectArmyDlg::IDD, pParent)
{

}

CSelectArmyDlg::~CSelectArmyDlg()
{
}

void CSelectArmyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ARMY, m_ctrlArmyCombo);
}


BEGIN_MESSAGE_MAP(CSelectArmyDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSelectArmyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSelectArmyDlg 消息处理程序

BOOL CSelectArmyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	std::list<CBarback*> &lstEarthBarbacks = earthCivi.GetBarbackList();
	std::list<CBarback*>::iterator itr;
	for (itr = lstEarthBarbacks.begin(); itr != lstEarthBarbacks.end(); itr ++)
	{
		if ((*itr)->GetBuildCompleted())
			m_ctrlArmyCombo.AddString((*itr)->GetDepartment()->GetName());
	}
	m_ctrlArmyCombo.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSelectArmyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int iSel = m_ctrlArmyCombo.GetCurSel();
	if (iSel == -1)
		return;
	m_ctrlArmyCombo.GetLBText(iSel, m_sSelArmy);
	OnOK();
}
