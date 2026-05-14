// SpaceflightTecTreeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "SpaceflightTecTreeDlg.h"
#include "Helper.h"
#include "LengendOfUniDlg.h"


// CSpaceflightTecTreeDlg 对话框

IMPLEMENT_DYNAMIC(CSpaceflightTecTreeDlg, CDialog)

CSpaceflightTecTreeDlg::CSpaceflightTecTreeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpaceflightTecTreeDlg::IDD, pParent)
	, m_pTecTree(NULL)
	, m_sTecName(_T(""))
{

}

CSpaceflightTecTreeDlg::~CSpaceflightTecTreeDlg()
{
}

void CSpaceflightTecTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TECNAME, m_sTecName);
}


BEGIN_MESSAGE_MAP(CSpaceflightTecTreeDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO2, &CSpaceflightTecTreeDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO5, &CSpaceflightTecTreeDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO3, &CSpaceflightTecTreeDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO6, &CSpaceflightTecTreeDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO9, &CSpaceflightTecTreeDlg::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO12, &CSpaceflightTecTreeDlg::OnBnClickedRadio12)
	ON_BN_CLICKED(IDC_RADIO14, &CSpaceflightTecTreeDlg::OnBnClickedRadio14)
	ON_BN_CLICKED(IDC_RADIO15, &CSpaceflightTecTreeDlg::OnBnClickedRadio15)
	ON_BN_CLICKED(IDC_RADIO16, &CSpaceflightTecTreeDlg::OnBnClickedRadio16)
	ON_BN_CLICKED(IDC_RADIO4, &CSpaceflightTecTreeDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO8, &CSpaceflightTecTreeDlg::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO11, &CSpaceflightTecTreeDlg::OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_RADIO7, &CSpaceflightTecTreeDlg::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO10, &CSpaceflightTecTreeDlg::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO13, &CSpaceflightTecTreeDlg::OnBnClickedRadio13)
END_MESSAGE_MAP()


// CSpaceflightTecTreeDlg 消息处理程序

void CSpaceflightTecTreeDlg::Init(CTecTree* pTecTree)
{
	m_pTecTree = pTecTree;
}

void CSpaceflightTecTreeDlg::InitTecTreeRadioWhenDef(CTecTreeNode* pNode)
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

BOOL CSpaceflightTecTreeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString sTecNameArray[] = { _T("10%光速飞船"), _T("50%光速飞船"), _T("99%光速飞船"),
								_T("行星开发Ⅰ"), _T("行星开发Ⅱ"), _T("行星开发Ⅲ"),
								_T("行星建设Ⅰ"), _T("行星建设Ⅱ"), _T("行星建设Ⅲ"),
								_T("行星殖民Ⅰ"), _T("行星殖民Ⅱ"), _T("行星殖民Ⅲ"),
								_T("恒星开发Ⅰ"), _T("恒星开发Ⅱ"), _T("恒星开发Ⅲ")};
	m_mapTecToCtrl[sTecNameArray[0]] = IDC_RADIO2;
	m_mapTecToCtrl[sTecNameArray[1]] = IDC_RADIO5;
	m_mapTecToCtrl[sTecNameArray[2]] = IDC_RADIO3;
	m_mapTecToCtrl[sTecNameArray[3]] = IDC_RADIO6;
	m_mapTecToCtrl[sTecNameArray[4]] = IDC_RADIO9;
	m_mapTecToCtrl[sTecNameArray[5]] = IDC_RADIO12;
	m_mapTecToCtrl[sTecNameArray[6]] = IDC_RADIO4;
	m_mapTecToCtrl[sTecNameArray[7]] = IDC_RADIO8;
	m_mapTecToCtrl[sTecNameArray[8]] = IDC_RADIO11;
	m_mapTecToCtrl[sTecNameArray[9]] = IDC_RADIO7;
	m_mapTecToCtrl[sTecNameArray[10]] = IDC_RADIO10;
	m_mapTecToCtrl[sTecNameArray[11]] = IDC_RADIO13;
	m_mapTecToCtrl[sTecNameArray[12]] = IDC_RADIO14;
	m_mapTecToCtrl[sTecNameArray[13]] = IDC_RADIO15;
	m_mapTecToCtrl[sTecNameArray[14]] = IDC_RADIO16;

	CTecTreeNode* pNode = NULL;
	for (int i = 0; i < 15; i ++)
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
		GetDlgItem(m_mapTecToCtrl[pNode->GetName()])->CheckRadioButton(IDC_RADIO2, IDC_RADIO16, m_mapTecToCtrl[pNode->GetName()]);
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
void CSpaceflightTecTreeDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (CHelper::DoDevelopTecCost(TT_MILITARY, m_sTecName))
	{
		((CLengendOfUniDlg*)this->GetParent())->UpdateValues();
		CDialog::OnOK();
	}
}

void CSpaceflightTecTreeDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio2()
{
	m_sTecName = _T("10%光速飞船");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio5()
{
	m_sTecName = _T("50%光速飞船");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio3()
{
	m_sTecName = _T("99%光速飞船");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio6()
{
	m_sTecName = _T("行星开发Ⅰ");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio9()
{
	m_sTecName = _T("行星开发Ⅱ");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio12()
{
	m_sTecName = _T("行星开发Ⅲ");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio14()
{
	m_sTecName = _T("恒星开发Ⅰ");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio15()
{
	m_sTecName = _T("恒星开发Ⅱ");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio16()
{
	m_sTecName = _T("恒星开发Ⅲ");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio4()
{
	m_sTecName = _T("行星建设Ⅰ");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio8()
{
	m_sTecName = _T("行星建设Ⅱ");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio11()
{
	m_sTecName = _T("行星建设Ⅲ");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio7()
{
	m_sTecName = _T("行星殖民Ⅰ");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio10()
{
	m_sTecName = _T("行星殖民Ⅱ");
	UpdateData(FALSE);
}

void CSpaceflightTecTreeDlg::OnBnClickedRadio13()
{
	m_sTecName = _T("行星殖民Ⅲ");
	UpdateData(FALSE);
}

BOOL CSpaceflightTecTreeDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint pt = pMsg->pt;	
		ScreenToClient( &pt );
		m_toolTipDlg.Move(pt);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CSpaceflightTecTreeDlg::InitToolTip()
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