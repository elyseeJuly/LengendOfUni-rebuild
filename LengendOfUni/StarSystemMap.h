#pragma once
#include <vector>
#include "MapBaseDlg.h"
#include "btnst.h"

// CStarSystemMap 对话框

class CStarSystemMap : public CMapBaseDlg
{
	DECLARE_DYNAMIC(CStarSystemMap)

public:
	CStarSystemMap(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStarSystemMap();

// 对话框数据
	enum { IDD = IDD_STARSYSTEM_MAP };

protected:
	afx_msg void OnStarBtnClick(UINT uID);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void UpdateStarBtns();
	void Clear();
	void Init(STAR_AREA starArea);
	void InitStars(int iStarCnt, int iIdOffset);
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
	virtual void OnOK();

private:
	std::vector<CButtonST*>	m_vecButtons;	
	STAR_AREA				m_starArea;
	int						m_iStartBtnID;
};
