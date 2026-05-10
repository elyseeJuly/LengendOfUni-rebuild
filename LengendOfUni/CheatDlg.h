#pragma once


// CCheatDlg 对话框

class CCheatDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheatDlg)

public:
	CCheatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheatDlg();

// 对话框数据
	enum { IDD = IDD_CHEAT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sCtrCheatCode;
	afx_msg void OnBnClickedOk();
};
