// ToolTipDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "ToolTipDlg.h"


// CToolTipDlg 对话框

IMPLEMENT_DYNAMIC(CToolTipDlg, CDialog)

CToolTipDlg::CToolTipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolTipDlg::IDD, pParent)
	, m_sTip(_T(""))
{

}

CToolTipDlg::~CToolTipDlg()
{
	Clear();
}

void CToolTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TIP, m_sTip);
}


BEGIN_MESSAGE_MAP(CToolTipDlg, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CToolTipDlg 消息处理程序
void CToolTipDlg::Move(CPoint &pt)
{
	CRect rect, rectScr;
	std::map<CWnd*, CTip*>::iterator itr;
	for (itr = m_mapTips.begin(); itr != m_mapTips.end(); itr ++)
	{
		itr->first->GetWindowRect(rect);
		rectScr = rect;
		itr->first->GetParent()->ScreenToClient(rect);
		if (rect.PtInRect(pt))
		{
			if (itr->first->IsWindowVisible())
				break;
		}
	}
	if (itr == m_mapTips.end())
	{
		ShowWindow(SW_HIDE);
		return;
	}

	BOOL bEnable = itr->first->IsWindowEnabled();
	if (bEnable)	
		m_sTip = itr->second->m_sNormalTip;
	else
		m_sTip = itr->second->m_sDisableTip;
	UpdateData(FALSE);

	CPoint point;
	point.SetPoint(rectScr.left, rectScr.bottom + 3);

	SetWindowPos(&wndTop, point.x, point.y, m_iWidth, m_iHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
}

BOOL CToolTipDlg::AddTool(CWnd* pWnd, CString sNormalTip, CString sDisableTip)
{
	if (m_mapTips.find(pWnd) != m_mapTips.end())
		return FALSE;

	CTip *pTip = new CTip;
	pTip->m_pWnd = pWnd;
	pTip->m_sNormalTip = sNormalTip;
	pTip->m_sDisableTip = sDisableTip;
	m_mapTips[pWnd] = pTip;
	return TRUE;
}

BOOL CToolTipDlg::SetTool(CWnd* pWnd, CString sNormalTip, CString sDisableTip)
{
	if (m_mapTips.find(pWnd) == m_mapTips.end())
		AddTool(pWnd, sNormalTip, sDisableTip);
	else
	{
		m_mapTips[pWnd]->m_sNormalTip = sNormalTip;
		m_mapTips[pWnd]->m_sDisableTip = sDisableTip;
	}

	return TRUE;
}

BOOL CToolTipDlg::RemoveTool(CWnd* pWnd)
{
	std::map<CWnd*, CTip*>::iterator itr;
	if ((itr = m_mapTips.find(pWnd)) == m_mapTips.end())
		return FALSE;

	SAFE_DELETE(itr->second)
	m_mapTips.erase(itr);
	return TRUE;
}

void CToolTipDlg::Clear()
{
	std::map<CWnd*, CTip*>::iterator itr;
	for (itr = m_mapTips.begin(); itr != m_mapTips.end(); itr ++)
	{
		SAFE_DELETE(itr->second)
	}
	m_mapTips.clear();
}

HBRUSH CToolTipDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	switch (nCtlColor)
	{   
		case   CTLCOLOR_STATIC: 
			pDC->SetBkMode(TRANSPARENT);
		case   CTLCOLOR_DLG:   
			return   static_cast<HBRUSH>(m_bkBrush.GetSafeHandle());   
	}   

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CToolTipDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bkBrush.CreateSolidBrush(RGB(209, 209, 209));
	CRect rect;
	GetClientRect(rect);
	m_iWidth = rect.Width();
	m_iHeight = rect.Height();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CToolTipDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if( pMsg->message == WM_MOUSEMOVE )
	{
		ShowWindow(SW_HIDE);
	}

	return CDialog::PreTranslateMessage(pMsg);
}
