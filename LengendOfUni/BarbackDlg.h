#pragma once
#include "afxwin.h"
#include "ToolTipDlg.h"


// CBarbackDlg 对话框

class CBarbackDlg : public CDialog
{
	DECLARE_DYNAMIC(CBarbackDlg)

public:
	CBarbackDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBarbackDlg();

// 对话框数据
	enum { IDD = IDD_BARBACK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonChgleader();
	afx_msg void OnBnClickedButtonModnum();

	void Init(CBarback* pBarback);
	void InitUI();
private:
	CToolTipDlg					m_toolTipDlg;
	void InitToolTip();
	CBarback		*m_pBarback;
public:
	CString m_sLeader;
	CString m_sSolderCntTip;
	CStatic m_ctrFace;
	CListBox m_ctrWeaponList;
	CComboBox m_ctrWeaponCombo[3];
	CString m_sProduceWeapon[3];
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonProduce0();
	afx_msg void OnBnClickedButtonProduce1();
	afx_msg void OnBnClickedButtonProduce2();
	CString m_sBarbackName;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeComboWeapon();
	afx_msg void OnCbnSelchangeComboWeapon2();
	afx_msg void OnCbnSelchangeComboWeapon3();
};
