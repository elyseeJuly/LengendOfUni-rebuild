#pragma once

#include "ToolTipDlg.h"

// CNuclearTecTreeDlg 对话框

class CNuclearTecTreeDlg : public CDialog
{
	DECLARE_DYNAMIC(CNuclearTecTreeDlg)

public:
	CNuclearTecTreeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNuclearTecTreeDlg();

// 对话框数据
	enum { IDD = IDD_NUCLEAR_TECTREE_DLG };
public:
	void Init(CTecTree* pTecTree);
	void InitTecTreeRadioWhenDef(CTecTreeNode* pNode);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CToolTipDlg					m_toolTipDlg;
	void InitToolTip();
	CTecTree* m_pTecTree;
	std::map<CString, UINT>		m_mapTecToCtrl;

public:
	CString m_sTecName;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio3();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
