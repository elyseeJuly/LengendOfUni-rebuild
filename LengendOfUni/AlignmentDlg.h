#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ToolTipDlg.h"


// CAlignmentDlg 对话框

class CAlignmentDlg : public CDialog
{
	DECLARE_DYNAMIC(CAlignmentDlg)

public:
	void Init(int iEnemyStar, CString sLeaderName);
	CAlignmentDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAlignmentDlg();

// 对话框数据
	enum { IDD = IDD_ALIGNMENT_DLG };

protected:
	void Win();
	void Lose();
	void DoEnemySpeak();
	void InitFace();
	void InitBtns();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	int		m_iEnemyStarIndex;
	int		m_iTotalMP;
	int		m_iCurMP;
	CString m_sTipInfoShown;
	int		m_iAlignmentCountDown;
	int		m_iEnemyTec;
	CToolTipDlg					m_toolTipDlg;
	void InitToolTip();

public:
	virtual BOOL OnInitDialog();
	CString m_sLeaderName;
	CStatic m_ctrFace;
	CStatic m_ctrEnemyFace;
	CString m_sEnemyName;
	CString m_sLeaderMP;
	CProgressCtrl m_ctrProgress;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnFuncBtnClick(UINT uID);
	CString m_sInfo;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnCancel();
	virtual void OnOK();
};
