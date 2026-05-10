// CultureTecTreeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "CultureTecTreeDlg.h"
#include "Helper.h"
#include "LengendOfUniDlg.h"


// CCultureTecTreeDlg 对话框

IMPLEMENT_DYNAMIC(CCultureTecTreeDlg, CDialog)

CCultureTecTreeDlg::CCultureTecTreeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCultureTecTreeDlg::IDD, pParent)
	, m_sTecName(_T(""))
	, m_pTecTree(NULL)
{

}

CCultureTecTreeDlg::~CCultureTecTreeDlg()
{
}

void CCultureTecTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_NAME, m_sTecName);
}


BEGIN_MESSAGE_MAP(CCultureTecTreeDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO2, &CCultureTecTreeDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, &CCultureTecTreeDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CCultureTecTreeDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &CCultureTecTreeDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CCultureTecTreeDlg::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO3, &CCultureTecTreeDlg::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CCultureTecTreeDlg 消息处理程序

void CCultureTecTreeDlg::Init(CTecTree* pTecTree)
{
	m_pTecTree = pTecTree;
}

void CCultureTecTreeDlg::InitTecTreeRadioWhenDef(CTecTreeNode* pNode)
{
	if (pNode->GetFinish())
	{
		std::map<CString, CTecTreeNode*>::iterator itr = pNode->m_mapChildrenNode.begin();
		while (itr != pNode->m_mapChildrenNode.end())
		{
			InitTecTreeRadioWhenDef(itr->second);
			itr ++;
		}

	}
	else
	{
		GetDlgItem(m_mapTecToCtrl[pNode->GetName()])->ShowWindow(SW_SHOW);
		GetDlgItem(m_mapTecToCtrl[pNode->GetName()])->EnableWindow(TRUE);
	}
}

BOOL CCultureTecTreeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString sTecNameArray[] = {_T("思想钢印Ⅰ"), _T("思想钢印Ⅱ"), _T("思想钢印Ⅲ"), _T("人口Ⅰ"), _T("人口Ⅱ"), _T("人口Ⅲ")};
	m_mapTecToCtrl[sTecNameArray[0]] = IDC_RADIO2;
	m_mapTecToCtrl[sTecNameArray[1]] = IDC_RADIO4;
	m_mapTecToCtrl[sTecNameArray[2]] = IDC_RADIO5;
	m_mapTecToCtrl[sTecNameArray[3]] = IDC_RADIO6;
	m_mapTecToCtrl[sTecNameArray[4]] = IDC_RADIO7;
	m_mapTecToCtrl[sTecNameArray[5]] = IDC_RADIO3;
	CTecTreeNode* pNode = NULL;
	for (int i = 0; i < 6; i ++)
	{
		pNode = m_pTecTree->GetChildNode(sTecNameArray[i], NULL);
		if (pNode == NULL)
			continue;
		if (pNode->GetFinish())
		{
			GetDlgItem(m_mapTecToCtrl[sTecNameArray[i]])->ShowWindow(SW_SHOW);
			GetDlgItem(m_mapTecToCtrl[sTecNameArray[i]])->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(m_mapTecToCtrl[sTecNameArray[i]])->ShowWindow(SW_HIDE);
		}
	}

	pNode = m_pTecTree->GetResearchTecNode();
	if (pNode)
	{
		// 有技术正在研究中
		GetDlgItem(m_mapTecToCtrl[pNode->GetName()])->ShowWindow(SW_SHOW);
		GetDlgItem(m_mapTecToCtrl[pNode->GetName()])->EnableWindow(TRUE);
		GetDlgItem(m_mapTecToCtrl[pNode->GetName()])->CheckRadioButton(IDC_RADIO2, IDC_RADIO7, m_mapTecToCtrl[pNode->GetName()]);
		m_sTecName = pNode->GetName();
	}
	else
	{
		// 没技术在研究
		InitTecTreeRadioWhenDef(m_pTecTree->GetChildNode(_T("root")));
		m_sTecName = _T("无");
	}
	UpdateData(FALSE);
	InitToolTip();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCultureTecTreeDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (CHelper::DoDevelopTecCost(TT_CULTURETEC, m_sTecName))
	{
		((CLengendOfUniDlg*)this->GetParent())->UpdateValues();
		CDialog::OnOK();
	}
}

void CCultureTecTreeDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

void CCultureTecTreeDlg::OnBnClickedRadio2()
{
	m_sTecName = _T("思想钢印Ⅰ");
	UpdateData(FALSE);
}

void CCultureTecTreeDlg::OnBnClickedRadio4()
{
	m_sTecName = _T("思想钢印Ⅱ");
	UpdateData(FALSE);
}

void CCultureTecTreeDlg::OnBnClickedRadio5()
{
	m_sTecName = _T("思想钢印Ⅲ");
	UpdateData(FALSE);
}

void CCultureTecTreeDlg::OnBnClickedRadio6()
{
	m_sTecName = _T("人口Ⅰ");
	UpdateData(FALSE);
}

void CCultureTecTreeDlg::OnBnClickedRadio7()
{
	m_sTecName = _T("人口Ⅱ");
	UpdateData(FALSE);
}

void CCultureTecTreeDlg::OnBnClickedRadio3()
{
	m_sTecName = _T("人口Ⅲ");
	UpdateData(FALSE);
}

BOOL CCultureTecTreeDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint pt = pMsg->pt;	
		ScreenToClient( &pt );
		m_toolTipDlg.Move(pt);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CCultureTecTreeDlg::InitToolTip()
{
	m_toolTipDlg.Create(IDD_TOOLTIP_DLG, this);
	m_toolTipDlg.ShowWindow(SW_HIDE);

	std::map<CString, UINT>::iterator itr;
	CTecTreeNode* pNode = NULL;
	for (itr = m_mapTecToCtrl.begin(); itr != m_mapTecToCtrl.end(); itr ++)
	{
		pNode = m_pTecTree->GetChildNode(itr->first, NULL);
		if (pNode == NULL)
			continue;
		CString sFunc;
		sFunc = pNode->GetTip();
		m_toolTipDlg.AddTool(GetDlgItem(itr->second), sFunc, _T("（已完成）") + sFunc);

	}
}