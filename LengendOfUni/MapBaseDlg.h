#pragma once
#include "afxwin.h"

class CMapBaseDlg :
	public CDialog
{
public:
	CMapBaseDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	CMapBaseDlg(void);
	~CMapBaseDlg(void);
	virtual void UpdateStarBtns() = 0;
};
