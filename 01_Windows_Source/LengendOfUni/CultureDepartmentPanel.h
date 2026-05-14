#pragma once
#include "afxwin.h"


// CCultureDepartmentPanel 对话框

class CCultureDepartmentPanel : public CDialog
{
	DECLARE_DYNAMIC(CCultureDepartmentPanel)

public:
	CCultureDepartmentPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCultureDepartmentPanel();

// 对话框数据
	enum { IDD = IDD_CULTURE_PANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void UpdateUI();
	virtual BOOL OnInitDialog();
	CStatic m_ctrFace;
	CString m_sWorker;
	CString m_sLeader;
	afx_msg void OnBnClickedButtonModcount();
	afx_msg void OnBnClickedButtonChgleader();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
