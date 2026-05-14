// StarSystemMap.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "StarSystemMap.h"
#include "LengendOfUniDlg.h"
#include "helper.h"

// CStarSystemMap 对话框

IMPLEMENT_DYNAMIC(CStarSystemMap, CDialog)

CStarSystemMap::CStarSystemMap(CWnd* pParent /*=NULL*/)
	: CMapBaseDlg(CStarSystemMap::IDD, pParent)
{
	m_iStartBtnID = 0;
}

CStarSystemMap::~CStarSystemMap()
{
	Clear();
}

void CStarSystemMap::DoDataExchange(CDataExchange* pDX)
{
	CMapBaseDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStarSystemMap, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, 9, 209, &CStarSystemMap::OnStarBtnClick)
END_MESSAGE_MAP()


// CStarSystemMap 消息处理程序

BOOL CStarSystemMap::OnInitDialog()
{
	CMapBaseDlg::OnInitDialog();

	int iStarCnt;
	int iIdOffset;
	switch (m_starArea)
	{
	case SA_50LIGHTYEAR:
		iStarCnt = 40;
		iIdOffset = 9;
		break;
	case SA_1WLIGHTYEAR:
		iStarCnt = 60;
		iIdOffset = 49;
		break;
	case SA_GALAXY:
		iStarCnt = 100;
		iIdOffset = 109;
		break;
	default:
		break;
	}
	InitStars(iStarCnt, iIdOffset);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CStarSystemMap::OnCancel()
{
}

void CStarSystemMap::OnOK()
{
}

void CStarSystemMap::Init(STAR_AREA starArea)
{
	m_starArea = starArea;
}

void CStarSystemMap::Clear()
{
	for (int i = 0; i < m_vecButtons.size(); i ++)
	{
		SAFE_DELETE(m_vecButtons[i])
	}
}

void CStarSystemMap::InitStars(int iStarCnt, int iIdOffset)
{
	m_iStartBtnID = iIdOffset;
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	int iWidth = 52;
	int iHDis = 5;
	int iHeight = 2750 / iStarCnt - 5;
	int iVDis = 5;

	CFont *pFont = GetFont();

	for (int i = 0; i < iStarCnt; i ++)
	{
		int iStarIndex = i + iIdOffset;
		CString sStarName;
		CStar* pStar = starMgr.GetStar(iStarIndex);
		if (!pStar)
		{
			ASSERT(0);
			return;
		}
		if (pStar->GetIsFound())
			sStarName = pStar->GetName();
		else
			sStarName.Format(_T("星%d"), iStarIndex);

		int j, k;
		j = i / 10;
		k = i % 10;
		CButtonST *pBtn = new CButtonST;
		pBtn->Create(sStarName, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
			CRect(10 + k * (iWidth + iHDis), 10 + j * (iHeight + iVDis), 10 + k * (iWidth + iHDis) + iWidth, 10 + j * (iHeight + iVDis) + iHeight),
			this, iStarIndex);

		pBtn->SetFont(pFont);
		pBtn->ShowWindow(SW_SHOW);
		m_vecButtons.push_back(pBtn);
	}

}

void CStarSystemMap::OnStarBtnClick(UINT uID)
{
	CString sTec;
	switch (m_starArea)
	{
	case SA_SOLARSYSTEM:
		break;
	case SA_50LIGHTYEAR:
		sTec = _T("10%光速飞船");
		break;
	case SA_1WLIGHTYEAR:
		sTec = _T("50%光速飞船");
		break;
	case SA_GALAXY:
		sTec = _T("99%光速飞船");
		break;
	};

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	BOOL bFinish = tecTreeMgr.IsTecFinished(TT_MILITARY,sTec);
	if (!bFinish)
	{
		CString sInfo;
		sInfo.Format(_T("我们的航天技术还不足以登上这座星球。\n我们需要“航天技术”部掌握“%s”的技术。"), sTec);
		CHelper::PersonMsgBox(NULL, sInfo);
		return;
	}

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pStar = starMgr.GetStar(uID);
	if (!pStar)
	{
		ATLASSERT(FALSE);
		return;
	}
	if (!pStar->GetIsFound())
	{
		if (((CLengendOfUniApp*)AfxGetApp())->m_game->IsReachStarCountLimit()) // 达到每局可发现星球数的上限
		{
			CHelper::PersonMsgBox(NULL, _T("今年我们登上的星球已经够多了，也许我们应该把机会留给明年。"));
			return;
		}

		((CLengendOfUniApp*)AfxGetApp())->m_game->AddReachStarCount();
	}

	((CLengendOfUniDlg*)this->GetParent())->SwitchStar(uID);
}

void CStarSystemMap::UpdateStarBtns()
{
	for (int i = 0; i < m_vecButtons.size(); i ++)
	{
		CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
		CStar *pStar = starMgr.GetStar(m_iStartBtnID + i);
		if (pStar)
		{
			if (pStar->GetIsExist())
				m_vecButtons[i]->ShowWindow(SW_SHOW);
			else
				m_vecButtons[i]->ShowWindow(SW_HIDE);

			if (pStar->GetIsFound())
				m_vecButtons[i]->SetWindowText(pStar->GetName());
			
			CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
			CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
			BOOL bFinish;
			if (m_starArea == SA_50LIGHTYEAR)
				bFinish = tecTreeMgr.IsTecFinished(TT_INTERSTELLAR, _T("太阳电波放大(50光年)"));
			else if (m_starArea == SA_1WLIGHTYEAR)
				bFinish = tecTreeMgr.IsTecFinished(TT_INTERSTELLAR, _T("太阳电波放大(1万光年)"));
			else if (m_starArea == SA_GALAXY)
				bFinish = tecTreeMgr.IsTecFinished(TT_INTERSTELLAR, _T("太阳电波放大(银河系)"));
			if (!bFinish)
			{
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(102, 102, 102));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(102, 102, 102));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(153, 153, 153));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(153, 153, 153));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(153, 153, 153));
				continue;	// 未掌握技术，星图上不用颜色标注。
			}

			if (pStar->GetBelongToCivi().Compare(EARTH_CIVI_NAME) == 0)
			{
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 255, 255));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(204, 204, 204));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(204, 204, 204));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(51, 102, 153));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(51, 102, 153));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(51, 102, 153));
			}
			else if (pStar->GetIsFound() && !pStar->GetBelongToCivi().IsEmpty())
			{
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(102, 102, 102));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(102, 102, 102));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(255, 153, 0));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 153, 0));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(255, 153, 0));
			}
			else
			{
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(102, 102, 102));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(102, 102, 102));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(153, 153, 153));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(153, 153, 153));
				m_vecButtons[i]->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(153, 153, 153));
			}
		}
	}
}