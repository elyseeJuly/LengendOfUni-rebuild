// AboutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "AboutDlg.h"


// CAboutDlg 对话框

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
	, m_sInfo(_T(""))
{

}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sInfo);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO2, &CAboutDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CAboutDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CAboutDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CAboutDlg::OnBnClickedRadio4)
END_MESSAGE_MAP()


// CAboutDlg 消息处理程序

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
	ShowFileText(FT_AUTHOR);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAboutDlg::ShowFileText(FILETEXT_TYPE ftype)
{
	CString sFile;
	HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
	TCHAR lpFn[255];   
	::GetModuleFileName(hModule, lpFn, 255);
	sFile = lpFn;
	int iPos = sFile.ReverseFind('\\');
	sFile = sFile.Left(iPos + 1);

	switch (ftype)
	{
	case FT_AUTHOR:
		sFile += _T("作者的话.txt");
		break;
	case FT_MODIFY:
		sFile += _T("修改游戏.txt");
		break;
	case FT_DONATIVE:
		sFile += _T("捐款.txt");
		break;
	case FT_THANKS:
		sFile += _T("鸣谢.txt");
		break;

	default:
		break;
	}

	CFile fileText(sFile, CFile::modeRead); 
	int iLength = fileText.GetLength();
	char *pTxt = new char[iLength + 1];
	fileText.Read(pTxt, iLength);
	pTxt[iLength] = '\0';
	m_sInfo = CA2T(pTxt);
	fileText.Close();
	delete []pTxt;

	UpdateData(FALSE);
}
void CAboutDlg::OnBnClickedRadio2()
{
	ShowFileText(FT_MODIFY);
}

void CAboutDlg::OnBnClickedRadio1()
{
	ShowFileText(FT_AUTHOR);
}

void CAboutDlg::OnBnClickedRadio3()
{
	ShowFileText(FT_DONATIVE);
}

void CAboutDlg::OnBnClickedRadio4()
{
	ShowFileText(FT_THANKS);
}
	