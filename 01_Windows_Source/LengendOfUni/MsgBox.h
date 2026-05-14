#pragma once
#include "afxwin.h"


// CMsgBox 对话框

class CMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CMsgBox)

public:
	CMsgBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMsgBox();

// 对话框数据
	enum { IDD = IDD_MSGBOX_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void Init(CString sPicPath, CString sTalker, CString sContent, BOOL bYesNoBtn);
public:
	CStatic m_ctrFace;
	CString m_sTalker;
	CString m_sContent;
	CString m_sPicPath;
	BOOL	m_bYesNoBtn;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
