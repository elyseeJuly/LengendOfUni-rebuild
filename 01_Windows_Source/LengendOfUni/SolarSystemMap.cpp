// SolarSystemMap.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "SolarSystemMap.h"
#include "LengendOfUniDlg.h"


// CSolarSystemMap 对话框

IMPLEMENT_DYNAMIC(CSolarSystemMap, CDialog)

CSolarSystemMap::CSolarSystemMap(CWnd* pParent /*=NULL*/)
	: CMapBaseDlg(CSolarSystemMap::IDD, pParent)
{

}

CSolarSystemMap::~CSolarSystemMap()
{
}

void CSolarSystemMap::DoDataExchange(CDataExchange* pDX)
{
	CMapBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON4, m_pBtnStar[0]);
	DDX_Control(pDX, IDC_BUTTON1, m_pBtnStar[1]);
	DDX_Control(pDX, IDC_BUTTON2, m_pBtnStar[2]);
	DDX_Control(pDX, IDC_BUTTON3, m_pBtnStar[3]);
	DDX_Control(pDX, IDC_BUTTON5, m_pBtnStar[4]);
	DDX_Control(pDX, IDC_BUTTON13, m_pBtnStar[5]);
	DDX_Control(pDX, IDC_BUTTON14, m_pBtnStar[6]);
	DDX_Control(pDX, IDC_BUTTON15, m_pBtnStar[7]);
	DDX_Control(pDX, IDC_BUTTON16, m_pBtnStar[8]);
}


BEGIN_MESSAGE_MAP(CSolarSystemMap, CDialog)
	ON_BN_CLICKED(IDOK, &CSolarSystemMap::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSolarSystemMap::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON4, &CSolarSystemMap::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CSolarSystemMap::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSolarSystemMap::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSolarSystemMap::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CSolarSystemMap::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON13, &CSolarSystemMap::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CSolarSystemMap::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CSolarSystemMap::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &CSolarSystemMap::OnBnClickedButton16)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSolarSystemMap 消息处理程序

void CSolarSystemMap::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	return;
}

void CSolarSystemMap::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	return;
}

BOOL CSolarSystemMap::OnInitDialog()
{
	CMapBaseDlg::OnInitDialog();

	m_mapStarIndex2CtrId[1] = IDC_BUTTON1;
	m_mapStarIndex2CtrId[2] = IDC_BUTTON2;
	m_mapStarIndex2CtrId[3] = IDC_BUTTON3;
	m_mapStarIndex2CtrId[0] = IDC_BUTTON4;
	m_mapStarIndex2CtrId[4] = IDC_BUTTON5;
	m_mapStarIndex2CtrId[5] = IDC_BUTTON13;
	m_mapStarIndex2CtrId[6] = IDC_BUTTON14;
	m_mapStarIndex2CtrId[7] = IDC_BUTTON15;
	m_mapStarIndex2CtrId[8] = IDC_BUTTON16;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSolarSystemMap::OnBnClickedButton4()
{
	// 地球
	((CLengendOfUniDlg*)this->GetParent())->SwitchStar(0);
}

void CSolarSystemMap::OnBnClickedButton1()
{
	// 太阳
	((CLengendOfUniDlg*)this->GetParent())->SwitchStar(1);
}

void CSolarSystemMap::OnBnClickedButton2()
{
	// 水星
	((CLengendOfUniDlg*)this->GetParent())->SwitchStar(2);
}

void CSolarSystemMap::OnBnClickedButton3()
{
	// 金星
	((CLengendOfUniDlg*)this->GetParent())->SwitchStar(3);
}

void CSolarSystemMap::OnBnClickedButton5()
{
	// 火星
	((CLengendOfUniDlg*)this->GetParent())->SwitchStar(4);
}

void CSolarSystemMap::OnBnClickedButton13()
{
	// 木星
	((CLengendOfUniDlg*)this->GetParent())->SwitchStar(5);
}

void CSolarSystemMap::OnBnClickedButton14()
{
	// 土星
	((CLengendOfUniDlg*)this->GetParent())->SwitchStar(6);
}

void CSolarSystemMap::OnBnClickedButton15()
{
	// 天王星
	((CLengendOfUniDlg*)this->GetParent())->SwitchStar(7);
}

void CSolarSystemMap::OnBnClickedButton16()
{
	// 海王星
	((CLengendOfUniDlg*)this->GetParent())->SwitchStar(8);
}

void CSolarSystemMap::UpdateStarBtns()
{
	for (int i = 0; i < 9; i ++)
	{
		CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
		CStar *pStar = starMgr.GetStar(i);
		if (pStar)
		{
			if (pStar->GetIsExist())
				GetDlgItem(m_mapStarIndex2CtrId[i])->ShowWindow(SW_SHOW);
			else
				GetDlgItem(m_mapStarIndex2CtrId[i])->ShowWindow(SW_HIDE);

			if (pStar->GetBelongToCivi().Compare(EARTH_CIVI_NAME) == 0)
			{
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(204, 204, 204));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(204, 204, 204));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(51, 102, 153));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(51, 102, 153));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(51, 102, 153));
			}
			else if (pStar->GetIsFound() && !pStar->GetBelongToCivi().IsEmpty())
			{
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(102, 102, 102));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(102, 102, 102));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(255, 153, 0));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 153, 0));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(255, 153, 0));
			}
			else
			{
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(102, 102, 102));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(102, 102, 102));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(153, 153, 153));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(153, 153, 153));
				m_pBtnStar[i].SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(153, 153, 153));
			}
		}
	}
}

