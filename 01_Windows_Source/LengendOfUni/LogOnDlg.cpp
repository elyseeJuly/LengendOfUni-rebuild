// LogOnDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "LogOnDlg.h"
#include "aboutdlg.h"
#include "LengendOfUniDlg.h"
#include "IniConfig.h"

// CLogOnDlg 对话框

IMPLEMENT_DYNAMIC(CLogOnDlg, CDialog)

CLogOnDlg::CLogOnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogOnDlg::IDD, pParent)
	, m_sPlayerName(_T(""))
	, m_pMusicPlayer(NULL)
	, m_bPlayMusic(FALSE)
{

}

CLogOnDlg::~CLogOnDlg()
{
	SAFE_DELETE(m_pMusicPlayer)
}

void CLogOnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sPlayerName);
	DDV_MaxChars(pDX, m_sPlayerName, 10);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_ctrlTitle);
	DDX_Check(pDX, IDC_CHECK_MUSIC, m_bPlayMusic);
}


BEGIN_MESSAGE_MAP(CLogOnDlg, CDialog)
	ON_MESSAGE(UM_MUSICNOTIFY, &CLogOnDlg::OnMsgMusicNotify)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CLogOnDlg::OnBnClickedButtonAbout)
	ON_BN_CLICKED(IDOK, &CLogOnDlg::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHECK_MUSIC, &CLogOnDlg::OnBnClickedCheckMusic)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CLogOnDlg::OnBnClickedButtonLoad)
END_MESSAGE_MAP()


// CLogOnDlg 消息处理程序

void CLogOnDlg::OnBnClickedButtonAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CLogOnDlg::OnBnClickedOk()
{
	UpdateData();
	if (m_sPlayerName.IsEmpty())
	{
		MessageBox(_T("请输入地球执政官的名字！"), _T("刘慈欣群星传"), MB_ICONWARNING);
		return;
	}

	ShowWindow(SW_HIDE);
	((CLengendOfUniApp*)AfxGetApp())->m_game = new CGame;
	((CLengendOfUniApp*)AfxGetApp())->m_game->Init();

	((CLengendOfUniApp*)AfxGetApp())->m_game->SetPlayerName(m_sPlayerName);

	CLengendOfUniDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
	}
	SAFE_DELETE(((CLengendOfUniApp*)AfxGetApp())->m_game)
	ShowWindow(SW_SHOW);

}

BOOL CLogOnDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CLogOnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
	TCHAR lpFn[255];   
	::GetModuleFileName(hModule, lpFn, 255);
	CString sFace;
	sFace = lpFn;
	int iPos = sFace.ReverseFind('\\');
	sFace = sFace.Left(iPos + 1);
	sFace += _T("images\\");
	sFace += _T("title.bmp");
	HBITMAP hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sFace, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_ctrlTitle.SetBitmap(hBmp);

	m_sPlayerName = _T("刘大宝");
	InitMusic();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CLogOnDlg::OnMsgMusicNotify(WPARAM wParam, LPARAM lParam)
{
	if (m_pMusicPlayer)
		m_pMusicPlayer->HandleEvent();
	return S_OK;
}

void CLogOnDlg::InitMusic(BOOL bForceInit)
{

	CIniConfig iniConfig(_T("music.ini"));
	if (!bForceInit)
	{
		m_bPlayMusic = (BOOL)iniConfig.GetInt(_T("count"), _T("play"));
		if (!m_bPlayMusic)		// 不播放音乐
			return;
	}

	m_pMusicPlayer = new CMusicPlayer(GetSafeHwnd());
	int iCnt;
	iCnt = iniConfig.GetInt(_T("count"), _T("count"));

	for(int i = 0; i < iCnt; i ++)
	{
		CString sSection;
		sSection.Format(_T("%d"), i);
		CString sName = iniConfig.GetString(sSection ,_T("Name"));

		HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
		TCHAR lpFn[255];   
		::GetModuleFileName(hModule, lpFn, 255);
		CString sPath;
		sPath = lpFn;
		int iPos = sPath.ReverseFind('\\');
		sPath = sPath.Left(iPos + 1);
		sPath += _T("music\\");
		sPath += sName;

		m_pMusicPlayer->AddMusic(sPath.GetBuffer(-1));
	}
	m_pMusicPlayer->Play();
}

void CLogOnDlg::OnBnClickedCheckMusic()
{
	UpdateData();
	if (m_bPlayMusic)
	{
		if (m_pMusicPlayer)
			m_pMusicPlayer->Play();
		else
			InitMusic(TRUE);
	}
	else
	{
		if (m_pMusicPlayer)
			m_pMusicPlayer->Pause();
	}
}

void CLogOnDlg::OnCancel()
{
	UpdateData();
	CIniConfig iniConfig(_T("music.ini"));
	iniConfig.SetInt(_T("count"), _T("play"), (int)m_bPlayMusic);
	CDialog::OnCancel();
}

void CLogOnDlg::OnBnClickedButtonLoad()
{
	CString sFileName;

	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("刘慈欣群星传存档文件 (*.lsv)|*.lsv||"));
	if(FileDialog.DoModal()==IDOK)
	{
		sFileName = FileDialog.GetPathName();

		CFile fileLoad(sFileName, CFile::modeRead); 
		CArchive arLoad(&fileLoad, CArchive::load); 

		CString sFlag;
		arLoad >> sFlag;
		if (sFlag.CompareNoCase(_T("LegendOfUni")) != 0)
		{
			AfxMessageBox(_T("这不是有效的“刘慈欣群星传”存档，无法载入。"));
			fileLoad.Close();  
			return;
		}
		SAFE_DELETE(((CLengendOfUniApp*)AfxGetApp())->m_game)
		arLoad >> (((CLengendOfUniApp*)AfxGetApp())->m_game); 
		arLoad.Close();
		fileLoad.Close();  

		ShowWindow(SW_HIDE);
		CLengendOfUniDlg dlg;
		dlg.m_bLoadGame = TRUE;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: 在此处放置处理何时用“确定”来关闭
			//  对话框的代码
		}
		else if (nResponse == IDCANCEL)
		{
		}

		SAFE_DELETE(((CLengendOfUniApp*)AfxGetApp())->m_game)
		ShowWindow(SW_SHOW);

	}
}
