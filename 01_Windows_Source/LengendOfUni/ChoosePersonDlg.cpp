// ChoosePersonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "ChoosePersonDlg.h"


// CChoosePersonDlg 对话框
CChoosePersonDlg* CChoosePersonDlg::m_pThis = NULL;

IMPLEMENT_DYNAMIC(CChoosePersonDlg, CDialog)

CChoosePersonDlg::CChoosePersonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChoosePersonDlg::IDD, pParent)
{
	m_sortType = FALSE;
}

CChoosePersonDlg::~CChoosePersonDlg()
{
}

void CChoosePersonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrListCtrl);
}


BEGIN_MESSAGE_MAP(CChoosePersonDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CChoosePersonDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CChoosePersonDlg::OnLvnColumnclickList1)
END_MESSAGE_MAP()


// CChoosePersonDlg 消息处理程序

BOOL CChoosePersonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitListCtrl();
	InitListCtrlData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CChoosePersonDlg::InitListCtrl()
{
	LV_COLUMN lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;//居中
	lvcol.pszText = _T("名字");
	lvcol.iSubItem=0;
	lvcol.cx = 70;
	m_ctrListCtrl.InsertColumn(0,&lvcol);
	lvcol.pszText = _T("所属部门");
	lvcol.iSubItem=1;
	m_ctrListCtrl.InsertColumn(1,&lvcol);
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.pszText = _T("科技能力");
	lvcol.iSubItem=2;
	m_ctrListCtrl.InsertColumn(2,&lvcol);
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.pszText = _T("艺术能力");
	lvcol.iSubItem=3;
	m_ctrListCtrl.InsertColumn(3,&lvcol);
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.pszText = _T("经济能力");
	lvcol.iSubItem=4;
	m_ctrListCtrl.InsertColumn(4,&lvcol);
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.pszText = _T("军事能力");
	lvcol.iSubItem=5;
	m_ctrListCtrl.InsertColumn(5,&lvcol);
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.pszText = _T("领导能力");
	lvcol.iSubItem=6;
	m_ctrListCtrl.InsertColumn(6,&lvcol);
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.pszText = _T("社会能力");
	lvcol.iSubItem=7;
	m_ctrListCtrl.InsertColumn(7,&lvcol);
	lvcol.fmt=LVCFMT_CENTER;
	lvcol.pszText = _T("逃亡主义");
	lvcol.iSubItem=8;
	m_ctrListCtrl.InsertColumn(8,&lvcol);
	//	m_ctrListCtrl.ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SORTASCENDING|LVS_EDITLABELS);
	m_ctrListCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT);
}

void CChoosePersonDlg::InitListCtrlData()
{
	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	PersonMap &personMap = personMgr.GetPersonFoundMap();
	
	for (PersonMap::iterator itr = personMap.begin(); itr != personMap.end(); itr ++)
	{
		InsertListCtrlItem(itr);
	}
}

void CChoosePersonDlg::InsertListCtrlItem(PersonMap::iterator itr)
{
	CString sFormat;
	int nItem = m_ctrListCtrl.InsertItem(0, itr->second->GetName());
	LV_ITEM lvitem;
	lvitem.mask = LVIF_IMAGE;
	lvitem.iItem = nItem;
	lvitem.iSubItem = 0;
	lvitem.pszText = (LPTSTR)(LPCTSTR)itr->second->GetName();
	m_ctrListCtrl.SetItem(&lvitem);
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = nItem;
	lvitem.iSubItem = 1;
	CDepartment *pDep = itr->second->GetDepartment();
	if (pDep)
		sFormat = itr->second->GetDepartment()->GetName();
	else
		sFormat = _T("无");
	lvitem.pszText = (LPTSTR)(LPCTSTR)sFormat;
	m_ctrListCtrl.SetItem(&lvitem);
	lvitem.iSubItem = 2;
	sFormat.Format(_T("%d"), itr->second->GetScience());
	lvitem.pszText = (LPTSTR)(LPCTSTR)sFormat;
	m_ctrListCtrl.SetItem(&lvitem);
	lvitem.iSubItem = 3;
	sFormat.Format(_T("%d"), itr->second->GetArt());
	lvitem.pszText = (LPTSTR)(LPCTSTR)sFormat;
	m_ctrListCtrl.SetItem(&lvitem);
	lvitem.iSubItem = 4;
	sFormat.Format(_T("%d"), itr->second->GetEconomy());
	lvitem.pszText = (LPTSTR)(LPCTSTR)sFormat;
	m_ctrListCtrl.SetItem(&lvitem);
	lvitem.iSubItem = 5;
	sFormat.Format(_T("%d"), itr->second->GetArmy());
	lvitem.pszText = (LPTSTR)(LPCTSTR)sFormat;
	m_ctrListCtrl.SetItem(&lvitem);
	lvitem.iSubItem = 6;
	sFormat.Format(_T("%d"), itr->second->GetLeadership());
	lvitem.pszText = (LPTSTR)(LPCTSTR)sFormat;
	m_ctrListCtrl.SetItem(&lvitem);
	lvitem.iSubItem = 7;
	sFormat.Format(_T("%d"), itr->second->GetSocial());
	lvitem.pszText = (LPTSTR)(LPCTSTR)sFormat;
	m_ctrListCtrl.SetItem(&lvitem);
	lvitem.iSubItem = 8;
	sFormat.Format(_T("%d"), itr->second->GetTreachery());
	lvitem.pszText = (LPTSTR)(LPCTSTR)sFormat;
	m_ctrListCtrl.SetItem(&lvitem);
}
void CChoosePersonDlg::OnBnClickedOk()
{
	int istat = m_ctrListCtrl.GetSelectionMark();	//用istat存放当前选定的是第几项
	m_sPersonName = m_ctrListCtrl.GetItemText(istat, 0);
	OnOK();
}

void CChoosePersonDlg::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_ctrListCtrl.GetItemCount(); i ++)
	{
		m_ctrListCtrl.SetItemData(i,i);
	}
	m_sortType = !m_sortType;
	m_pThis = this;
	m_ctrListCtrl.SortItems((PFNLVCOMPARE)CompareFunc, pNMLV->iSubItem);

	*pResult = 0;
}

int CALLBACK CChoosePersonDlg::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CString text1,text2;
	text1 = m_pThis->m_ctrListCtrl.GetItemText(lParam1, lParamSort);
	text2 = m_pThis->m_ctrListCtrl.GetItemText(lParam2, lParamSort);
	if (lParamSort == 0 || lParamSort == 1)	// 人名或部门排序
	{
		if(m_pThis->m_sortType)
			return (text1.Compare(text2));
		else
			return (text1.Compare(text2) * -1);
	}
	else
	{
		int iVal1 = _ttoi(text1);
		int iVal2 = _ttoi(text2);
		if(m_pThis->m_sortType)
			return iVal1 > iVal2;
		else
			return iVal1 <= iVal2;
	}
}