// LengendOfUniDlg.h : 头文件
//

#pragma once
#include <vector>
#include "planetpanel.h"
#include "StarSystemMap.h"
#include "afxwin.h"
#include "ToolTipDlg.h"
#include "HyperLink.h"

// CLengendOfUniDlg 对话框
class CLengendOfUniDlg : public CDialog
{
	enum PANEL_TYPE
	{
		PT_ECONOMY_PANEL = 0,
		PT_ARMY_PANEL,
		PT_CULTURE_PANEL,
		PT_HUMANRES_PANEL,
		PT_ASTROSOCIOLOGY_PANEL,
		PT_NUCLEAR_PANEL,
		PT_SPACEFIGHT_PANEL,
		PT_PROTON_PANEL,
		PT_ASTROPHYSICS_PANEL,
		PT_CULTURETEC_PANEL,
		PT_ECONOMYTEC_PANEL,
		PT_COUNT,				// 特殊值，用以标识panel type的数量
	};
// 构造
public:
	CLengendOfUniDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CLengendOfUniDlg();

// 对话框数据
	enum { IDD = IDD_MAINDLG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CBrush						m_bkBrush;
	CToolTipDlg					m_toolTipDlg;
	CPlanetPanel*				m_pPlanetPanel;
	std::vector<CDialog*>		m_vecPanel;
	std::vector<CMapBaseDlg*>	m_vecMap;
	std::pair<int, int>			m_pairCurPanel;		// first: 面板类型 0 depPanel , 1 starPanel ; second : 索引
	STAR_AREA					m_CurrentStarArea;
	STAR_AREA					m_canReachStarArea;
public:
	afx_msg void OnBnClickedBtnEconomy();
private:
	void CheckCiviLevelUpgrade();
	BOOL CheckGameOver();
	void InitToolTip();
	void InitLink();
	void SwitchPanel(PANEL_TYPE panelType);
public:
	afx_msg void OnStarSysBtnClick(UINT uID);
	afx_msg void OnBnClickedBtnArmy();
	afx_msg void OnBnClickedBtnCulture();
	afx_msg void OnBnClickedBtnHumanres();
	void CleanUp(void);
	void InitPanels(void);
	void InitOthers();
	afx_msg void OnBnClickedBtnAstrosociology();
	afx_msg void OnBnClickedBtnNuclear();
	afx_msg void OnBnClickedBtnSpacefight();
	afx_msg void OnBnClickedBtnProton();
	afx_msg void OnBnClickedBtnAstrophysics();
	afx_msg void OnBnClickedBtnCulturetec();
	afx_msg void OnBnClickedBtnEconomytec();
	BOOL m_bLoadGame;
	int m_iValPopulation;
	int m_iValEconomy;
	int m_iValArmy;
	int m_iValCulture;
	int m_iValResource;
	int m_iValTreachery;
	void UpdatePlayerPanel();
	void UpdateValues(void);
	void InitMaps(void);
	void SwitchMap(STAR_AREA starArea = SA_COUNT);
	void SwitchStar(int iIndex);
	afx_msg void OnBnClickedButtonPlanFinish();
	CString m_sYear;
	void UpdateYearLabel();
	CStatic m_ctrFace;
	CString m_sPlayerName;
	afx_msg void OnBnClickedButtonPlanHistory();
protected:
	virtual void OnCancel();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_sCiviLevelName;
	CHyperLink m_ctrLink1;
	CHyperLink m_ctrLink2;
	CHyperLink m_ctrLink3;
	CHyperLink m_ctrLink4;
	CHyperLink m_ctrLink5;
	CHyperLink m_ctrLink6;
	CHyperLink m_ctrLink7;
	afx_msg void OnBnClickedButtonPlanSave();
};
