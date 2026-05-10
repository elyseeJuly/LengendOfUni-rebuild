#pragma once
#include "afxcmn.h"
#include "resource.h"


// CSetPopulationDlg 对话框

class CSetPopulationDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetPopulationDlg)

public:
	CSetPopulationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetPopulationDlg();

// 对话框数据
	enum { IDD = IDD_SET_WORKER_CNT_DLG };

protected:
	int m_iOldNum;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iNum;
	afx_msg void OnBnClickedOk();
	CSliderCtrl m_ctrSlider;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void SetOldNum(int iOldNum)		{ m_iOldNum = iOldNum; };
};
