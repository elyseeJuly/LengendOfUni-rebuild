// AstrosociologyTecTreeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "AstrosociologyTecTreeDlg.h"
#include "Helper.h"
#include "LengendOfUniDlg.h"


// CAstrosociologyTecTreeDlg 对话框

IMPLEMENT_DYNAMIC(CAstrosociologyTecTreeDlg, CDialog)

CAstrosociologyTecTreeDlg::CAstrosociologyTecTreeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAstrosociologyTecTreeDlg::IDD, pParent)
	, m_pTecTree(NULL)
	, m_sTecName(_T(""))
{

}

CAstrosociologyTecTreeDlg::~CAstrosociologyTecTreeDlg()
{
}

void CAstrosociologyTecTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_NAME, m_sTecName);
}


BEGIN_MESSAGE_MAP(CAstrosociologyTecTreeDlg, CDialog)

	ON_BN_CLICKED(IDC_RADIO1, &CAstrosociologyTecTreeDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CAstrosociologyTecTreeDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, &CAstrosociologyTecTreeDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CAstrosociologyTecTreeDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO3, &CAstrosociologyTecTreeDlg::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CAstrosociologyTecTreeDlg 消息处理程序

void CAstrosociologyTecTreeDlg::Init(CTecTree* pTecTree)
{
	m_pTecTree = pTecTree;
}

void CAstrosociologyTecTreeDlg::InitTecTreeRadioWhenDef(CTecTreeNode* pNode)
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

BOOL CAstrosociologyTecTreeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString sTecNameArray[] = {_T("宇宙社会学公理"), _T("技术爆炸理论"), _T("猜疑链"), _T("黑暗森林理论"), _T("种族沟通")};
	m_mapTecToCtrl[sTecNameArray[0]] = IDC_RADIO1;
	m_mapTecToCtrl[sTecNameArray[1]] = IDC_RADIO2;
	m_mapTecToCtrl[sTecNameArray[2]] = IDC_RADIO4;
	m_mapTecToCtrl[sTecNameArray[3]] = IDC_RADIO5;
	m_mapTecToCtrl[sTecNameArray[4]] = IDC_RADIO3;

	CTecTreeNode* pNode = NULL;
	for (int i = 0; i < 5; i ++)
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
		GetDlgItem(m_mapTecToCtrl[pNode->GetName()])->CheckRadioButton(IDC_RADIO1, IDC_RADIO5, m_mapTecToCtrl[pNode->GetName()]);
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

void CAstrosociologyTecTreeDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

void CAstrosociologyTecTreeDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (CHelper::DoDevelopTecCost(TT_ASTROSOCIOLOGY, m_sTecName))
	{
		((CLengendOfUniDlg*)this->GetParent())->UpdateValues();
		CDialog::OnOK();
	}
}

void CAstrosociologyTecTreeDlg::OnBnClickedRadio1()
{
	m_sTecName = _T("宇宙社会学公理");
	UpdateData(FALSE);
}

void CAstrosociologyTecTreeDlg::OnBnClickedRadio2()
{
	m_sTecName = _T("技术爆炸理论");
	UpdateData(FALSE);
}

void CAstrosociologyTecTreeDlg::OnBnClickedRadio4()
{
	m_sTecName = _T("猜疑链");
	UpdateData(FALSE);
}

void CAstrosociologyTecTreeDlg::OnBnClickedRadio5()
{
	m_sTecName = _T("黑暗森林理论");
	UpdateData(FALSE);
}

void CAstrosociologyTecTreeDlg::OnBnClickedRadio3()
{
	m_sTecName = _T("种族沟通");
	UpdateData(FALSE);
}

BOOL CAstrosociologyTecTreeDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint pt = pMsg->pt;	
		ScreenToClient( &pt );
		m_toolTipDlg.Move(pt);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CAstrosociologyTecTreeDlg::InitToolTip()
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