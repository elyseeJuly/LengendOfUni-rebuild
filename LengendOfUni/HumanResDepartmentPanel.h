#pragma once
#include "afxwin.h"
#include "ToolTipDlg.h"


// CHumanResDepartmentPanel 对话框

class CHumanResDepartmentPanel : public CDialog
{
	DECLARE_DYNAMIC(CHumanResDepartmentPanel)

public:
	CHumanResDepartmentPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHumanResDepartmentPanel();

// 对话框数据
	enum { IDD = IDD_HUMANRES_PANEL };

protected:
	void UpdateButtonUI();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CToolTipDlg					m_toolTipDlg;
	void InitToolTip();
	DECLARE_MESSAGE_MAP()
public:
	void UpdateUI();
	virtual BOOL OnInitDialog();
	CStatic m_ctrFace;
	CString m_sLeader;
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedButtonChgleader();
	afx_msg void OnBnClickedButtonArmyPractice();
private:
	int m_iCurYearPrac;
	int m_iCurYearEdu;
public:
	afx_msg void OnBnClickedButtonEducation();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
