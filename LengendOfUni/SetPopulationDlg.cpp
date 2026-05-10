// SetPopulationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "SetPopulationDlg.h"


// CSetPopulationDlg 对话框

IMPLEMENT_DYNAMIC(CSetPopulationDlg, CDialog)

CSetPopulationDlg::CSetPopulationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetPopulationDlg::IDD, pParent)
	, m_iNum(0)
	, m_iOldNum(0)
{

}

CSetPopulationDlg::~CSetPopulationDlg()
{
}

void CSetPopulationDlg::DoDataExchange(CDataExchange* pDX)
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	int iIdlePop = earthCivi.GetIdlePopulation();

	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM, m_iNum);
	DDV_MinMaxInt(pDX, m_iNum, 0, iIdlePop + m_iOldNum);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrSlider);
}


BEGIN_MESSAGE_MAP(CSetPopulationDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetPopulationDlg::OnBnClickedOk)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CSetPopulationDlg 消息处理程序

void CSetPopulationDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL CSetPopulationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	int iIdlePop = earthCivi.GetIdlePopulation();

	m_ctrSlider.SetRange(0, iIdlePop + m_iOldNum);
	//m_ctrSlider.SetBuddy(GetDlgItem(IDC_EDIT_NUM), FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSetPopulationDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_iNum = m_ctrSlider.GetPos();
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
