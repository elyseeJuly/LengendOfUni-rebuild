#pragma once


// CPlanetOperationPanel 对话框

class CPlanetOperationPanel : public CDialog
{
	DECLARE_DYNAMIC(CPlanetOperationPanel)

public:
	CPlanetOperationPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlanetOperationPanel();

// 对话框数据
	enum { IDD = IDD_PLANET_OPT_PANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
