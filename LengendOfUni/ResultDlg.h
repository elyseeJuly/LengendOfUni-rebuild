#pragma once


// CResultDlg 对话框

class CResultDlg : public CDialog
{
	DECLARE_DYNAMIC(CResultDlg)

public:
	CResultDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResultDlg();

// 对话框数据
	enum { IDD = IDD_RESULT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	CFont	m_font;
public:
	virtual BOOL OnInitDialog();
	CString m_sResult;
	CString m_sScore;
	CString m_sHonor;
};
