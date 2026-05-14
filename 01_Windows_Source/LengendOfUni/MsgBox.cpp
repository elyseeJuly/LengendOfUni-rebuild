// MsgBox.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "MsgBox.h"


// CMsgBox 对话框

IMPLEMENT_DYNAMIC(CMsgBox, CDialog)

CMsgBox::CMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgBox::IDD, pParent)
	, m_sTalker(_T(""))
	, m_sContent(_T(""))
{

}

CMsgBox::~CMsgBox()
{
}

void CMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrFace);
	DDX_Text(pDX, IDC_STATIC_SPEAKER, m_sTalker);
	DDX_Text(pDX, IDC_STATIC_CONTENT, m_sContent);
}


BEGIN_MESSAGE_MAP(CMsgBox, CDialog)
	ON_BN_CLICKED(IDOK, &CMsgBox::OnBnClickedOk)
END_MESSAGE_MAP()


// CMsgBox 消息处理程序
void CMsgBox::Init(CString sPicPath, CString sTalker, CString sContent, BOOL bYesNoBtn)
{
	m_sPicPath = sPicPath;
	if (sTalker.CompareNoCase(_T("player")) == 0)
		sTalker = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPlayerName();
	m_sTalker = sTalker;
	m_sContent = sContent;
	m_bYesNoBtn = bYesNoBtn;
}

BOOL CMsgBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HBITMAP hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), m_sPicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_ctrFace.SetBitmap(hBmp);

	if (m_bYesNoBtn)
		GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMsgBox::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
