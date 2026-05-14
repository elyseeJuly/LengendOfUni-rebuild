#pragma once


// CAboutDlg 对话框

class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)
	enum FILETEXT_TYPE
	{
		FT_AUTHOR,
		FT_MODIFY,
		FT_DONATIVE,
		FT_THANKS,
	};
public:
	CAboutDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void ShowFileText(FILETEXT_TYPE ftype);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_sInfo;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
};
