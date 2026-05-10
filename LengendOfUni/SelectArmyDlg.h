#pragma once
#include "afxwin.h"


// CSelectArmyDlg 对话框

class CSelectArmyDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectArmyDlg)

public:
	CSelectArmyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectArmyDlg();

// 对话框数据
	enum { IDD = IDD_SELECT_ARMY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sSelArmy;
	virtual BOOL OnInitDialog();
	CComboBox m_ctrlArmyCombo;
	afx_msg void OnBnClickedOk();
};
