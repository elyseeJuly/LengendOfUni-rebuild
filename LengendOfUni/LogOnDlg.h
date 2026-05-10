#pragma once
#include "afxwin.h"
#include "../MusicPlayer/musicplayer.h"


// CLogOnDlg 对话框

class CLogOnDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogOnDlg)

public:
	CLogOnDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogOnDlg();

// 对话框数据
	enum { IDD = IDD_LOGON_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void InitMusic(BOOL bForceInit = FALSE);

	DECLARE_MESSAGE_MAP()
public:
	CString m_sPlayerName;
	afx_msg LRESULT OnMsgMusicNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CStatic m_ctrlTitle;
	virtual BOOL OnInitDialog();
private:
	CMusicPlayer*				m_pMusicPlayer;

public:
	BOOL m_bPlayMusic;
	afx_msg void OnBnClickedCheckMusic();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedButtonLoad();
};
