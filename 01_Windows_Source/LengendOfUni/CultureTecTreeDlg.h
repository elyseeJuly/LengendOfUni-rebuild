#pragma once

#include "ToolTipDlg.h"

// CCultureTecTreeDlg 对话框

class CCultureTecTreeDlg : public CDialog
{
	DECLARE_DYNAMIC(CCultureTecTreeDlg)

public:
	CCultureTecTreeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCultureTecTreeDlg();

// 对话框数据
	enum { IDD = IDD_CULTURE_TECTREE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CToolTipDlg					m_toolTipDlg;
	void InitToolTip();

	DECLARE_MESSAGE_MAP()
public:
	void Init(CTecTree* pTecTree);
	void InitTecTreeRadioWhenDef(CTecTreeNode* pNode);
private:
	CTecTree* m_pTecTree;
	std::map<CString, UINT>		m_mapTecToCtrl;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	CString m_sTecName;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio3();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
