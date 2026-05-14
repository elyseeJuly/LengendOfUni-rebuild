#pragma once
#include "afxwin.h"
#include "BarbackDlg.h"
#include "ToolTipDlg.h"

// CPlanetPanel 对话框

class CPlanetPanel : public CDialog
{
	DECLARE_DYNAMIC(CPlanetPanel)

public:
	CPlanetPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlanetPanel();

// 对话框数据
	enum { IDD = IDD_PLANET_PANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void Init(int iStarIndex);
	void InitNonAlienBtns();
private:
	void ShowAlienPlanetCtrl(BOOL bShow);		// 显示外星或非外星界面
	BOOL BuildAsk(CString sBuilding, int iCost);

private:
	CToolTipDlg	m_toolTipDlg;
	int		m_iStarIndex;
	BOOL	m_bPlanet;
public:
	CString m_sBarback;
	CString m_sStope;
	CString m_sFactory;
	CString m_sCity;
	CString m_sStarName;
	CString m_sCivi;
	CString m_sRes;
	CString m_sPop;
	CStatic m_ctrFace;
	CString m_sLeader;
	afx_msg void OnBnClickedButtonChgleader();
	afx_msg void OnBnClickedButtonBarback();
	afx_msg void OnBnClickedButtonStope();
	afx_msg void OnBnClickedButtonFactory();
	afx_msg void OnBnClickedButtonCity();
	afx_msg void OnBnClickedButtonBarbackOpen();
protected:
	void InitToolTip();
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedButtonAttck();
	afx_msg void OnBnClickedButtonFriend();
	afx_msg void OnBnClickedButtonBomb();
	afx_msg void OnBnClickedButtonSpy();
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
