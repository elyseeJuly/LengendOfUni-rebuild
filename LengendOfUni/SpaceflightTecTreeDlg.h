#pragma once
#include "ToolTipDlg.h"


// CSpaceflightTecTreeDlg 对话框

class CSpaceflightTecTreeDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpaceflightTecTreeDlg)

public:
	CSpaceflightTecTreeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpaceflightTecTreeDlg();

// 对话框数据
	enum { IDD = IDD_SPACEFIGHT_TECTREE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void Init(CTecTree* pTecTree);
	void InitTecTreeRadioWhenDef(CTecTreeNode* pNode);
private:
	CToolTipDlg					m_toolTipDlg;
	void InitToolTip();
	CTecTree* m_pTecTree;
	std::map<CString, UINT>		m_mapTecToCtrl;

protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
public:
	CString m_sTecName;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio12();
	afx_msg void OnBnClickedRadio14();
	afx_msg void OnBnClickedRadio15();
	afx_msg void OnBnClickedRadio16();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio11();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio13();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
