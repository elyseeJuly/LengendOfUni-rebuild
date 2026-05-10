#pragma once
#include "ToolTipDlg.h"

#include <map>
// CAstrosociologyTecTreeDlg 对话框

class CAstrosociologyTecTreeDlg : public CDialog
{
	DECLARE_DYNAMIC(CAstrosociologyTecTreeDlg)

public:
	CAstrosociologyTecTreeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAstrosociologyTecTreeDlg();

// 对话框数据
	enum { IDD = IDD_ASTROSOCIOLOGY_TECTREE_DLG };

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
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnBnClickedRadio1();
	CString m_sTecName;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio3();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
