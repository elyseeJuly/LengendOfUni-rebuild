#pragma once
#include "afxwin.h"
#include "ToolTipDlg.h"

// CEconomyDepartmentPanel 对话框

class CEconomyDepartmentPanel : public CDialog
{
	DECLARE_DYNAMIC(CEconomyDepartmentPanel)

public:
	CEconomyDepartmentPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEconomyDepartmentPanel();

// 对话框数据
	enum { IDD = IDD_ECONOMY_PANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CToolTipDlg					m_toolTipDlg;
	void InitToolTip();

	DECLARE_MESSAGE_MAP()
public:
	void UpdateUI();
	virtual BOOL OnInitDialog();
	CString m_sLeader;
	CString m_sMiningWorkerCnt;
	CString m_sManufactureWorkerCnt;
	CStatic m_ctrFace;
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonChgleader();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
