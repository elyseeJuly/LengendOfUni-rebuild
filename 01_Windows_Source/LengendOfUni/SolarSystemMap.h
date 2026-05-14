#pragma once

#include <map>
#include "MapBaseDlg.h"
#include "btnst.h"

// CSolarSystemMap 对话框

class CSolarSystemMap : public CMapBaseDlg
{
	DECLARE_DYNAMIC(CSolarSystemMap)

public:
	CSolarSystemMap(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSolarSystemMap();

// 对话框数据
	enum { IDD = IDD_SOLARSYSTEM_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	void UpdateStarBtns();
private:
	std::map<int, UINT>		m_mapStarIndex2CtrId;
public:
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton16();
	CButtonST m_pBtnStar[9];
};
