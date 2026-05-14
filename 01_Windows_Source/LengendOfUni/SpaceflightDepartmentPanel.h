#pragma once


// CSpaceflightDepartmentPanel 对话框

class CSpaceflightDepartmentPanel : public CDialog
{
	DECLARE_DYNAMIC(CSpaceflightDepartmentPanel)

public:
	CSpaceflightDepartmentPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpaceflightDepartmentPanel();

// 对话框数据
	enum { IDD = IDD_SPACEFIGHT_PANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSettarget();
	void UpdateUI();
	virtual BOOL OnInitDialog();
	CStatic m_ctrFace;
	CString m_sLeader;
	CString m_sWorker;
	CString m_sDirection;
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButtonChgleader();
	afx_msg void OnBnClickedButtonModnum();
};
