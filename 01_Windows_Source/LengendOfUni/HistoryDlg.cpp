// HistoryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "HistoryDlg.h"
#include "RandomNumber.h"

// CHistoryDlg 对话框

IMPLEMENT_DYNAMIC(CHistoryDlg, CDialog)

CHistoryDlg::CHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistoryDlg::IDD, pParent)
	, m_sHistoryEdit(_T(""))
{

}

CHistoryDlg::~CHistoryDlg()
{
}

void CHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HISTORY, m_sHistoryEdit);
}


BEGIN_MESSAGE_MAP(CHistoryDlg, CDialog)
END_MESSAGE_MAP()


// CHistoryDlg 消息处理程序

BOOL CHistoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_sHistoryEdit = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetHistory();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
