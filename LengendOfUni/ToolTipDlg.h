#pragma once
#include <map>

// CToolTipDlg 对话框

class CToolTipDlg : public CDialog
{
	DECLARE_DYNAMIC(CToolTipDlg)

	class CTip
	{
	public:
		CWnd*		m_pWnd;
		CString		m_sNormalTip;
		CString		m_sDisableTip;
		CTip()
		{
			m_pWnd = NULL;
		};
	};
public:
	BOOL RemoveTool(CWnd* pWnd);
	BOOL SetTool(CWnd* pWnd, CString sNormalTip, CString sDisableTip = _T(""));
	BOOL AddTool(CWnd* pWnd, CString sNormalTip, CString sDisableTip = _T(""));
	void Move(CPoint &pt);
	CToolTipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CToolTipDlg();

// 对话框数据
	enum { IDD = IDD_TOOLTIP_DLG };

protected:
	void Clear();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	int						m_iWidth;
	int						m_iHeight;
	CBrush					m_bkBrush;
	std::map<CWnd*, CTip*>	m_mapTips;
public:
	CString m_sTip;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
