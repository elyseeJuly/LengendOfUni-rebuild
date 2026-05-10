#pragma once
#include "afxwin.h"
#include "ToolTipDlg.h"


// CArmyDepartmentPanel 对话框

class CArmyDepartmentPanel : public CDialog
{
	DECLARE_DYNAMIC(CArmyDepartmentPanel)

public:
	CArmyDepartmentPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CArmyDepartmentPanel();

// 对话框数据
	enum { IDD = IDD_ARMY_PANEL };

protected:
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
	void UpdateWeaponUI();
	void InitBarbackCombo();
public:
	afx_msg void OnBnClickedButtonChgleader();
	CString m_sBombCnt;
	CString m_sZhiziCnt;
	afx_msg void OnBnClickedButtonProducebomb();
	CString m_sBombPercent;
	CString m_sZhiziPercent;
	afx_msg void OnBnClickedButtonProducezhizi();
	CComboBox m_ctrBarbackCombo;
	afx_msg void OnBnClickedButton7();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
