// CheatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "CheatDlg.h"
#include "Helper.h"
#include "LengendOfUniDlg.h"

// CCheatDlg 对话框

IMPLEMENT_DYNAMIC(CCheatDlg, CDialog)

CCheatDlg::CCheatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheatDlg::IDD, pParent)
	, m_sCtrCheatCode(_T(""))
{

}

CCheatDlg::~CCheatDlg()
{
}

void CCheatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sCtrCheatCode);
}


BEGIN_MESSAGE_MAP(CCheatDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCheatDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCheatDlg 消息处理程序

void CCheatDlg::OnBnClickedOk()
{
	UpdateData();
	BOOL bCheatSucc = FALSE;
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	if (m_sCtrCheatCode.CompareNoCase(_T("showmethetec")) == 0)
	{
		// 获得所有技能
		CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
		tecTreeMgr.SetAllTecTreeFinish();
		bCheatSucc = TRUE;
	}

	if (m_sCtrCheatCode.CompareNoCase(_T("ineedmoney")) == 0)
	{
		earthCivi.SetEconomy(99999);
		bCheatSucc = TRUE;
	}

	if (m_sCtrCheatCode.CompareNoCase(_T("babybomb")) == 0)
	{
		int iVal = earthCivi.GetPopulation();
		int iValIdle = earthCivi.GetIdlePopulation();
		iVal += 99999;
		iValIdle += 99999;
		earthCivi.SetPopulation(iVal);
		earthCivi.SetIdlePopulation(iValIdle);
		bCheatSucc = TRUE;
	}

	if (m_sCtrCheatCode.CompareNoCase(_T("iamboss")) == 0)
	{
		CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
		personMgr.AllPeopleFound();
		bCheatSucc = TRUE;
	}

	if (m_sCtrCheatCode.CompareNoCase(_T("learnmore")) == 0)
	{
		earthCivi.SetCulture(1001);
		CLengendOfUniDlg* pParentDlg = (CLengendOfUniDlg*)this->GetParent();
		pParentDlg->GetDlgItem(IDC_BTN_ASTROSOCIOLOGY)->EnableWindow(TRUE);
		pParentDlg->GetDlgItem(IDC_BTN_NUCLEAR)->EnableWindow(TRUE);
		pParentDlg->GetDlgItem(IDC_BTN_PROTON)->EnableWindow(TRUE);
		pParentDlg->GetDlgItem(IDC_BTN_ASTROPHYSICS)->EnableWindow(TRUE);

		bCheatSucc = TRUE;
	}

	if (bCheatSucc)
	{
		CHelper::PersonMsgBox(NULL, _T("Bingo~~"));
	}
	OnOK();
}
