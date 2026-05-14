#pragma once
#include "afxcmn.h"
#include "PersonManager.h"

// CChoosePersonDlg 对话框

class CChoosePersonDlg : public CDialog
{
	DECLARE_DYNAMIC(CChoosePersonDlg)

public:
	CChoosePersonDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChoosePersonDlg();

// 对话框数据
	enum { IDD = IDD_CHOOSE_MAN };

protected:
	BOOL m_sortType;
	static CChoosePersonDlg *m_pThis;

	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void InitListCtrl();
	void InitListCtrlData();
	void InsertListCtrlItem(PersonMap::iterator itr);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sPersonName;
	virtual BOOL OnInitDialog();
	CListCtrl m_ctrListCtrl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
