// AstrophysicsTecTreeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "AstrophysicsTecTreeDlg.h"
#include "Helper.h"
#include "LengendOfUniDlg.h"

// CAstrophysicsTecTreeDlg 对话框

IMPLEMENT_DYNAMIC(CAstrophysicsTecTreeDlg, CDialog)

CAstrophysicsTecTreeDlg::CAstrophysicsTecTreeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAstrophysicsTecTreeDlg::IDD, pParent)
	, m_sTecName(_T(""))
	, m_pTecTree(NULL)
{

}

CAstrophysicsTecTreeDlg::~CAstrophysicsTecTreeDlg()
{
}

void CAstrophysicsTecTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_NAME, m_sTecName);
}


BEGIN_MESSAGE_MAP(CAstrophysicsTecTreeDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO2, &CAstrophysicsTecTreeDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, &CAstrophysicsTecTreeDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CAstrophysicsTecTreeDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &CAstrophysicsTecTreeDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CAstrophysicsTecTreeDlg::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO3, &CAstrophysicsTecTreeDlg::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CAstrophysicsTecTreeDlg 消息处理程序

void CAstrophysicsTecTreeDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

void CAstrophysicsTecTreeDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (CHelper::DoDevelopTecCost(TT_ASTROPHYSICS, m_sTecName))
	{
		((CLengendOfUniDlg*)this->GetParent())->UpdateValues();
		CDialog::OnOK();
	}
}


void CAstrophysicsTecTreeDlg::Init(CTecTree* pTecTree)
{
	m_pTecTree = pTecTree;
}

void CAstrophysicsTecTreeDlg::InitTecTreeRadioWhenDef(CTecTreeNode* pNode)
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

BOOL CAstrophysicsTecTreeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString sTecNameArray[] = {_T("50光年望远镜"), _T("1万光年望远镜"), _T("银河系望远镜"), _T("太阳电波放大(50光年)"), _T("太阳电波放大(1万光年)"), _T("太阳电波放大(银河系)")};
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
void CAstrophysicsTecTreeDlg::OnBnClickedRadio2()
{
	m_sTecName = _T("50光年望远镜");
	UpdateData(FALSE);
}

void CAstrophysicsTecTreeDlg::OnBnClickedRadio4()
{
	m_sTecName = _T("1万光年望远镜");
	UpdateData(FALSE);
}

void CAstrophysicsTecTreeDlg::OnBnClickedRadio5()
{
	m_sTecName = _T("银河系望远镜");
	UpdateData(FALSE);
}

void CAstrophysicsTecTreeDlg::OnBnClickedRadio6()
{
	m_sTecName = _T("太阳电波放大(50光年)");
	UpdateData(FALSE);
}

void CAstrophysicsTecTreeDlg::OnBnClickedRadio7()
{
	m_sTecName = _T("太阳电波放大(1万光年)");
	UpdateData(FALSE);
}

void CAstrophysicsTecTreeDlg::OnBnClickedRadio3()
{
	m_sTecName = _T("太阳电波放大(银河系)");
	UpdateData(FALSE);
}

BOOL CAstrophysicsTecTreeDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint pt = pMsg->pt;	
		ScreenToClient( &pt );
		m_toolTipDlg.Move(pt);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CAstrophysicsTecTreeDlg::InitToolTip()
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