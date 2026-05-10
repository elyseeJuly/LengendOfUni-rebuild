#pragma once


// CProtonDepartmentPanel 对话框

class CProtonDepartmentPanel : public CDialog
{
	DECLARE_DYNAMIC(CProtonDepartmentPanel)

public:
	CProtonDepartmentPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProtonDepartmentPanel();

// 对话框数据
	enum { IDD = IDD_PROTON_PANEL };

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
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedButtonChgleader();
	afx_msg void OnBnClickedButtonModnum();
};
