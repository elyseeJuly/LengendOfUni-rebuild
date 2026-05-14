// PlanetPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "PlanetPanel.h"
#include "Helper.h"
#include "LengendOfUniDlg.h"
#include "ArmyDepartment.h"

#define CIVI_STRING _T("所属文明：%s")
#define RES_STRING _T("资源：%d/%d")
#define POP_STRING _T("居民：%d/%d")
#define BARBACK_STRING _T("军事基地：%s")
#define STOPE_STRING _T("采矿工厂：%s")
#define FACTORY_STRING _T("加工工厂：%s")
#define CITY_STRING _T("城市：%s")

#define BARBACK_COST 20
#define STOPE_COST 20
#define FACTORY_COST 20
#define CITY_COST 20

// CPlanetPanel 对话框

IMPLEMENT_DYNAMIC(CPlanetPanel, CDialog)

CPlanetPanel::CPlanetPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CPlanetPanel::IDD, pParent)
	, m_sStarName(_T(""))
	, m_sCivi(_T(""))
	, m_sRes(_T(""))
	, m_sPop(_T(""))
	, m_sBarback(_T(""))
	, m_sStope(_T(""))
	, m_sFactory(_T(""))
	, m_sCity(_T(""))
	, m_sLeader(_T(""))
	, m_bPlanet(TRUE)
{

}

CPlanetPanel::~CPlanetPanel()
{
}

void CPlanetPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_STAR, m_sStarName);
	DDX_Text(pDX, IDC_STATIC_CIVI, m_sCivi);
	DDX_Text(pDX, IDC_STAR_RES, m_sRes);
	DDX_Text(pDX, IDC_STAR_POP, m_sPop);
	DDX_Text(pDX, IDC_STATIC_BARBACK, m_sBarback);
	DDX_Text(pDX, IDC_STATIC_STOPE, m_sStope);
	DDX_Text(pDX, IDC_STATIC_FACTORY, m_sFactory);
	DDX_Text(pDX, IDC_STATIC_CITY, m_sCity);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrFace);
//	DDX_Text(pDX, IDC_STATIC_LEADER, m_sLeader);
}


BEGIN_MESSAGE_MAP(CPlanetPanel, CDialog)
//	ON_BN_CLICKED(IDC_BUTTON_CHGLEADER, &CPlanetPanel::OnBnClickedButtonChgleader)
	ON_BN_CLICKED(IDC_BUTTON_BARBACK, &CPlanetPanel::OnBnClickedButtonBarback)
	ON_BN_CLICKED(IDC_BUTTON_STOPE, &CPlanetPanel::OnBnClickedButtonStope)
	ON_BN_CLICKED(IDC_BUTTON_FACTORY, &CPlanetPanel::OnBnClickedButtonFactory)
	ON_BN_CLICKED(IDC_BUTTON_CITY, &CPlanetPanel::OnBnClickedButtonCity)
	ON_BN_CLICKED(IDC_BUTTON_BARBACK_OPEN, &CPlanetPanel::OnBnClickedButtonBarbackOpen)
	ON_BN_CLICKED(IDC_BUTTON_ATTCK, &CPlanetPanel::OnBnClickedButtonAttck)
	ON_BN_CLICKED(IDC_BUTTON_FRIEND, &CPlanetPanel::OnBnClickedButtonFriend)
	ON_BN_CLICKED(IDC_BUTTON_BOMB, &CPlanetPanel::OnBnClickedButtonBomb)
	ON_BN_CLICKED(IDC_BUTTON_SPY, &CPlanetPanel::OnBnClickedButtonSpy)
	ON_BN_CLICKED(IDC_BUTTON1, &CPlanetPanel::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPlanetPanel 消息处理程序

void CPlanetPanel::Init(int iStarIndex)
{
	m_iStarIndex = iStarIndex;

	CString sDefName;
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* star = starMgr.GetStar(iStarIndex);
	if (!star)
	{
		ASSERT(FALSE);
		return;
	}
	
	if (!star->GetIsExist())
	{
		CDialog::OnCancel();
		return;
	}

	if (!star->GetIsFound())
	{
		star->SetIsFound(TRUE);
		sDefName.Format(_T("星%d"), iStarIndex);
		if (star->GetName().Compare(sDefName) != 0)
		{
			((CLengendOfUniDlg*)this->GetParent())->SwitchMap();
			CString sMsg;
			sMsg.Format(_T("发现新的星球：%s"), star->GetName());
			CHelper::PersonMsgBox(NULL, sMsg);
			CHelper::AddHistory(sMsg);
		}
		else if (!star->GetBelongToCivi().IsEmpty())
		{
			((CLengendOfUniDlg*)this->GetParent())->SwitchMap();
		}
	}

	m_sStarName = star->GetName();
	m_bPlanet = star->GetIsPlanet();
	if (m_bPlanet)
		m_sStarName = m_sStarName + _T(" - 行星");
	else
		m_sStarName = m_sStarName + _T(" - 恒星");

	CString sCivi;
	sCivi = star->GetBelongToCivi();
	if (sCivi.IsEmpty())
		m_sCivi.Format(CIVI_STRING, _T("无"));
	else
		m_sCivi.Format(CIVI_STRING, sCivi);

	m_sRes.Format(RES_STRING, star->GetCurrentResource(), star->GetTotalResource());
	m_sPop.Format(POP_STRING, star->GetCurrentPopulation(), star->GetPopulationLimit());

	if (sCivi.Compare(EARTH_CIVI_NAME) != 0 && !sCivi.IsEmpty())	// 被外星人占领
	{
		ShowAlienPlanetCtrl(TRUE);

		// 设置头像
		CAlienCivilization* pAlien = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(sCivi);
		CString sPic;
		sPic = pAlien->GetPic();
		HBITMAP hBmp;
		hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sPic, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		m_ctrFace.SetBitmap(hBmp);

		if (star->GetBarback()->GetIsFriend() ||
			star->GetBarback()->GetAlignmentYear() == ((CLengendOfUniApp*)AfxGetApp())->m_game->GetYear())
			GetDlgItem(IDC_BUTTON_FRIEND)->ShowWindow(SW_HIDE);
	}
	else		// 被地球人占领或无人占领
	{
		ShowAlienPlanetCtrl(FALSE);
		InitNonAlienBtns();
	}

	if (!m_bPlanet)
	{
		GetDlgItem(IDC_STATIC_FACTORY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CITY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_FACTORY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_CITY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAR_POP)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_STAR_POP)->ShowWindow(SW_SHOW);
	}

	InitToolTip();
	UpdateData(FALSE);
	Invalidate();
}

void CPlanetPanel::ShowAlienPlanetCtrl(BOOL bShow)
{
	int iShow = (bShow ? SW_HIDE : SW_SHOW);
	GetDlgItem(IDC_STATIC_BARBACK)->ShowWindow(iShow);
	GetDlgItem(IDC_STATIC_STOPE)->ShowWindow(iShow);
	GetDlgItem(IDC_STATIC_FACTORY)->ShowWindow(iShow);
	GetDlgItem(IDC_STATIC_CITY)->ShowWindow(iShow);
	GetDlgItem(IDC_BUTTON_BARBACK)->ShowWindow(iShow);
	GetDlgItem(IDC_BUTTON_BARBACK_OPEN)->ShowWindow(iShow);
	GetDlgItem(IDC_BUTTON_STOPE)->ShowWindow(iShow);
	GetDlgItem(IDC_BUTTON_FACTORY)->ShowWindow(iShow);
	GetDlgItem(IDC_BUTTON_CITY)->ShowWindow(iShow);
//	GetDlgItem(IDC_STATIC_LEADER)->ShowWindow(iShow);
//	GetDlgItem(IDC_BUTTON_CHGLEADER)->ShowWindow(iShow);

	iShow = (bShow ? SW_SHOW : SW_HIDE);
	// 再显示外星界面
	GetDlgItem(IDC_BUTTON_ATTCK)->ShowWindow(iShow);
	GetDlgItem(IDC_BUTTON_FRIEND)->ShowWindow(iShow);
	GetDlgItem(IDC_BUTTON_BOMB)->ShowWindow(iShow);
	GetDlgItem(IDC_BUTTON_SPY)->ShowWindow(iShow);
}

void CPlanetPanel::InitNonAlienBtns()
{
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* star = starMgr.GetStar(m_iStarIndex);

	if (star->GetBarback() == NULL)
	{
		m_sBarback.Format(BARBACK_STRING, _T("无"));
		GetDlgItem(IDC_BUTTON_BARBACK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_BARBACK_OPEN)->ShowWindow(SW_HIDE);
	}
	else
	{
		CBarback* pBarback = star->GetBarback();
		int iCrtBuild = pBarback->GetCurrentBuild();
		int iTtlBuild = pBarback->GetTotalBuild();
		if (iCrtBuild == iTtlBuild)	// 已建
		{
			m_sBarback.Format(BARBACK_STRING, _T("已建"));
			GetDlgItem(IDC_BUTTON_BARBACK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_BARBACK_OPEN)->ShowWindow(SW_SHOW);
		}
		else	// 建造中
		{
			CString sFormat;
			sFormat.Format(_T("建造中 %d/%d"), iCrtBuild, iTtlBuild);
			m_sBarback.Format(BARBACK_STRING, sFormat);
			GetDlgItem(IDC_BUTTON_BARBACK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_BARBACK_OPEN)->ShowWindow(SW_HIDE);
		}
	}

	if (star->GetStope() == NULL)
	{
		m_sStope.Format(STOPE_STRING, _T("无"));
		GetDlgItem(IDC_BUTTON_STOPE)->ShowWindow(SW_SHOW);
	}
	else
	{
		CStope* pStope = star->GetStope();
		int iCrtBuild = pStope->GetCurrentBuild();
		int iTtlBuild = pStope->GetTotalBuild();
		if (iCrtBuild == iTtlBuild)	// 已建
		{
			m_sStope.Format(STOPE_STRING, _T("已建"));
			GetDlgItem(IDC_BUTTON_STOPE)->ShowWindow(SW_HIDE);
		}
		else	// 建造中
		{
			CString sFormat;
			sFormat.Format(_T("建造中 %d/%d"), iCrtBuild, iTtlBuild);
			m_sStope.Format(STOPE_STRING, sFormat);
			GetDlgItem(IDC_BUTTON_STOPE)->ShowWindow(SW_HIDE);
		}
	}

	CString sFactoryName = _T("加工工厂：%s");
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	if (tecTreeMgr.IsTecFinished(TT_AEROSPACE, _T("行星发动机Ⅰ型")))
	{
		sFactoryName = _T("行星发动机：%s");
	}

	if (star->GetFactory() == NULL)
	{
		m_sFactory.Format(sFactoryName, _T("无"));
		GetDlgItem(IDC_BUTTON_FACTORY)->ShowWindow(SW_SHOW);
	}
	else
	{
		CFactory* pFactory = star->GetFactory();
		int iCrtBuild = pFactory->GetCurrentBuild();
		int iTtlBuild = pFactory->GetTotalBuild();
		if (iCrtBuild == iTtlBuild)	// 已建
		{
			m_sFactory.Format(sFactoryName, _T("已建"));
			GetDlgItem(IDC_BUTTON_FACTORY)->ShowWindow(SW_HIDE);
		}
		else	// 建造中
		{
			CString sFormat;
			sFormat.Format(_T("建造中 %d/%d"), iCrtBuild, iTtlBuild);
			m_sFactory.Format(sFactoryName, sFormat);
			GetDlgItem(IDC_BUTTON_FACTORY)->ShowWindow(SW_HIDE);
		}
	}

	if (star->GetCity() == NULL)
	{
		m_sCity.Format(CITY_STRING, _T("无"));
		GetDlgItem(IDC_BUTTON_CITY)->ShowWindow(SW_SHOW);
	}
	else
	{
		CCity* pCity = star->GetCity();
		int iCrtBuild = pCity->GetCurrentBuild();
		int iTtlBuild = pCity->GetTotalBuild();
		if (iCrtBuild == iTtlBuild)	// 已建
		{
			m_sCity.Format(CITY_STRING, _T("已建"));
			GetDlgItem(IDC_BUTTON_CITY)->ShowWindow(SW_HIDE);
		}
		else	// 建造中
		{
			CString sFormat;
			sFormat.Format(_T("建造中 %d/%d"), iCrtBuild, iTtlBuild);
			m_sCity.Format(CITY_STRING, sFormat);
			GetDlgItem(IDC_BUTTON_CITY)->ShowWindow(SW_HIDE);
		}
	}

	// 根据技能决定是否显示按钮
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	BOOL bFinish;
	if (m_bPlanet)
	{
		bFinish = tecTreeMgr.IsTecFinished(TT_MILITARY, _T("行星开发Ⅰ"));
		if (bFinish)
			GetDlgItem(IDC_BUTTON_STOPE)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_BUTTON_STOPE)->EnableWindow(FALSE);
	}
	else
	{
		 bFinish = tecTreeMgr.IsTecFinished(TT_MILITARY, _T("恒星开发Ⅰ"));
		if (bFinish)
			GetDlgItem(IDC_BUTTON_STOPE)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_BUTTON_STOPE)->EnableWindow(FALSE);
	}
	bFinish = tecTreeMgr.IsTecFinished(TT_MILITARY, _T("行星建设Ⅰ"));
	if (bFinish)
		GetDlgItem(IDC_BUTTON_FACTORY)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_BUTTON_FACTORY)->EnableWindow(FALSE);
	bFinish = tecTreeMgr.IsTecFinished(TT_MILITARY, _T("行星殖民Ⅰ"));
	if (bFinish)
		GetDlgItem(IDC_BUTTON_CITY)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_BUTTON_CITY)->EnableWindow(FALSE);

	CString sName = star->GetBelongToCivi();
	if (sName.IsEmpty())		// 此星球未占领
	{
		HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
		TCHAR lpFn[255];   
		::GetModuleFileName(hModule, lpFn, 255);
		CString sPic;
		sPic = lpFn;
		int iPos = sPic.ReverseFind('\\');
		sPic = sPic.Left(iPos + 1);
		if (m_bPlanet)
			sPic += _T("images\\行星.bmp");
		else
			sPic += _T("images\\恒星.bmp");

		HBITMAP hBmp;
		hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sPic, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		m_ctrFace.SetBitmap(hBmp);
//		GetDlgItem(IDC_STATIC_LEADER)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_BUTTON_CHGLEADER)->ShowWindow(SW_HIDE);
	}
	else if (sName.Compare(EARTH_CIVI_NAME) == 0)	// 此星球属于地球文明
	{
		HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
		TCHAR lpFn[255];   
		::GetModuleFileName(hModule, lpFn, 255);
		CString sPic;
		sPic = lpFn;
		int iPos = sPic.ReverseFind('\\');
		sPic = sPic.Left(iPos + 1);
		sPic += _T("images\\地球.bmp");
		HBITMAP hBmp;
		hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sPic, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		m_ctrFace.SetBitmap(hBmp);

/*		CDepartment *pDep = star->GetDepartment();
		if (pDep)
		{
			HBITMAP hBmp;
			m_sLeader = pDep->GetLeader();
			if (m_sLeader.IsEmpty())
			{
				m_sLeader = _T("无");
				UINT iFace = IDB_NONAME;

				hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(iFace));
				m_ctrFace.SetBitmap(hBmp);
			}
			else
			{
				CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
				CPerson* person = personMgr.GetPerson(m_sLeader);
				if (person)
				{
					CString sFace;
					sFace = person->GetFaceFile();
					hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sFace, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
					m_ctrFace.SetBitmap(hBmp);
				}
			}
		}*/
	}

}
void CPlanetPanel::OnBnClickedButtonChgleader()
{
/*	CString sName = CHelper::ChoosePerson();
	if (sName.IsEmpty())
		return;

	m_sLeader = sName;

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* star = starMgr.GetStar(m_iStarIndex);
	CDepartment *pDep = star->GetDepartment();
	CHelper::ChangePersonDepartment(m_sLeader, pDep);

	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	CPerson* person = personMgr.GetPerson(m_sLeader);
	if (person)
	{
		HBITMAP hBmp;
		CString sFace;
		sFace = person->GetFaceFile();
		hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sFace, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

		m_ctrFace.SetBitmap(hBmp);
	}

	UpdateData(FALSE);
	Invalidate();*/
}

void CPlanetPanel::OnBnClickedButtonBarback()
{
	if (!BuildAsk(_T("军事基地"), BARBACK_COST))
		return;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	earthCivi.BuildBarback(m_iStarIndex);
	Init(m_iStarIndex);
	((CLengendOfUniDlg*)this->GetParent())->SwitchMap();
}

void CPlanetPanel::OnBnClickedButtonStope()
{
	if (!BuildAsk(_T("采矿工厂"), STOPE_COST))
		return;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	earthCivi.BuildStope(m_iStarIndex);
	Init(m_iStarIndex);
	((CLengendOfUniDlg*)this->GetParent())->SwitchMap();
}

void CPlanetPanel::OnBnClickedButtonFactory()
{
	if (!BuildAsk(_T("加工工厂"), FACTORY_COST))
		return;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	earthCivi.BuildFactory(m_iStarIndex);
	Init(m_iStarIndex);
	((CLengendOfUniDlg*)this->GetParent())->SwitchMap();
}

void CPlanetPanel::OnBnClickedButtonCity()
{
	if (!BuildAsk(_T("城市"), CITY_COST))
		return;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	earthCivi.BuildCity(m_iStarIndex);
	Init(m_iStarIndex);
	((CLengendOfUniDlg*)this->GetParent())->SwitchMap();
}

void CPlanetPanel::OnBnClickedButtonBarbackOpen()
{
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* star = starMgr.GetStar(m_iStarIndex);
	if (star->GetBarback() == NULL)
		return;

	CBarbackDlg dlg;
	dlg.Init(star->GetBarback());
	dlg.DoModal();
}

void CPlanetPanel::OnOK()
{
}

void CPlanetPanel::OnCancel()
{
}

void CPlanetPanel::OnBnClickedButtonAttck()
{
	// 攻击此星球
	int iStar = CHelper::SelectStarOfBarback();
	if (iStar == -1)
		return;

	CHelper::StartBattle(iStar, m_iStarIndex, BT_ATTACK);
	CLengendOfUniDlg* pDlg = (CLengendOfUniDlg*)GetParent();
	pDlg->UpdateValues();
	Init(m_iStarIndex);
	((CLengendOfUniDlg*)this->GetParent())->SwitchMap();
}

void CPlanetPanel::OnBnClickedButtonFriend()
{
	// 结盟
	CHelper::StartAlignment(m_iStarIndex);
	GetDlgItem(IDC_BUTTON_FRIEND)->ShowWindow(SW_HIDE);

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* star = starMgr.GetStar(m_iStarIndex);
	if (!star)
	{
		ASSERT(FALSE);
		return;
	}
	int iYear = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetYear();
	star->GetBarback()->SetAlignmentYear(iYear);
	Init(m_iStarIndex);
}

void CPlanetPanel::OnBnClickedButtonBomb()
{
	// 炸掉这个星球
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CArmyDepartment *pDep = (CArmyDepartment*)earthCivi.GetDepartment(DT_ARMY);
	if (pDep->GetBombCount() == 0)
	{
		CHelper::PersonMsgBox(NULL, _T("我军没有奇点炸弹。可以在“军事部”制造奇点炸弹"));
		return;
	}

	if (CHelper::PersonMsgBox(NULL, _T("真的要炸掉这颗星球吗？"), TRUE) == IDOK)
	{
		pDep->RemoveOneBomb();

		CString sStarName;
		CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
		CStar* pStar = starMgr.GetStar(m_iStarIndex);
		if (!pStar)
		{
			ASSERT(FALSE);
			return;
		}
		CString sCivi = pStar->GetBelongToCivi();
		CAlienCivilization* pAlien = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(sCivi);
		CBarback *pBarback = NULL;
		pBarback = pStar->GetBarback();
		if (!pBarback)
		{
			ASSERT(FALSE);
			return;
		}
		pAlien->RemoveBarback(pBarback);
		pAlien->RemoveStar(m_iStarIndex);


		sStarName = pStar->GetName();
		starMgr.RemoveStar(m_iStarIndex);
		CLengendOfUniDlg* pDlg = (CLengendOfUniDlg*)GetParent();
		pDlg->SwitchMap();
		
		CString sTip;
		sTip.Format(_T("我们对%s星球使用了奇点炸弹。%s被炸成了粉末，永远成为宇宙中的一个历史名词。"), sStarName, sStarName);
		CHelper::PersonMsgBox(NULL, sTip);
		CHelper::AddHistory(sTip);
		if (pAlien->IsDieOut())
		{
			// 敌对文明灭绝
			CString sTip;
			sTip.Format(_T("这块最后的栖息地，竟成了一个伟大文明的坟墓...\n\n（%s灭绝）"), pAlien->GetName());
			CHelper::CiviMsgBox(pAlien, sTip);
			sTip.Format(_T("%s灭绝"), pAlien->GetName());
			CHelper::AddHistory(sTip);
		}
		pDlg->SwitchStar(0);
		ShowWindow(SW_HIDE);
	}
}

void CPlanetPanel::OnBnClickedButtonSpy()
{
	// 刺探
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CArmyDepartment *pDep = (CArmyDepartment*)earthCivi.GetDepartment(DT_ARMY);
	if (pDep->GetZhiziCount() == 0)
	{
		CHelper::PersonMsgBox(NULL, _T("我军没有智子。可以在“军事部”制造智子"));

		return;
	}

	if (CHelper::PersonMsgBox(NULL, _T("真的要在这颗星球上释放智子吗？"), TRUE) == IDOK)
	{
		pDep->RemoveOneZhizi();
		CString sInfo;
		CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
		CStar* pStar = starMgr.GetStar(m_iStarIndex);
		if (pStar)
		{
			CBarback* pBarback = pStar->GetBarback();
			if (pBarback)
			{
				int iSoldierCnt = pBarback->GetSoldierCount();
				sInfo.Format(_T("智子刺探回报－－本星球所属文明战备：\n士兵：%d\n拥有部队及武器：\n"), iSoldierCnt);
				WeaponExList& weaponExList = pBarback->GetWeaponExList();
				WeaponExList::iterator itr;
				if (weaponExList.empty())
					sInfo += _T("无");
				for (itr = weaponExList.begin(); itr != weaponExList.end(); itr ++)
				{
					CString sName = (*itr)->GetWeaponName();
					sInfo += sName;
					sInfo += _T("　");
				}
			}
		}
		CHelper::PersonMsgBox(NULL, sInfo);
	}
}

void CPlanetPanel::OnBnClickedButton1()
{
	CHelper::StartBattle(m_iStarIndex, 8, BT_DEFEND);
	CLengendOfUniDlg* pDlg = (CLengendOfUniDlg*)GetParent();
	pDlg->UpdateValues();
	Init(m_iStarIndex);
	((CLengendOfUniDlg*)this->GetParent())->SwitchMap();

}

BOOL CPlanetPanel::BuildAsk(CString sBuilding, int iCost)
{
	CString sInfo;
	sInfo.Format(_T("建造%s需要消耗 %d 点经济，确定要建造吗？"), sBuilding, iCost);
	if (CHelper::PersonMsgBox(NULL, sInfo, TRUE) == IDOK)
	{
		// 比较经济，看是否允许造。可以写一个公用函数
		if (!CHelper::CanUseEconemy(iCost, TRUE))
		{
			CHelper::PersonMsgBox(NULL, _T("目前我们的经济实力还不够，无法建造该建筑。"));
			return FALSE;
		}

		CLengendOfUniDlg* pDlg = (CLengendOfUniDlg*)GetParent();
		pDlg->UpdateValues();

		return TRUE;
	}
	return FALSE;
}

void CPlanetPanel::InitToolTip()
{
	if (!IsWindow(m_toolTipDlg.GetSafeHwnd()))
	{
		m_toolTipDlg.Create(IDD_TOOLTIP_DLG, this);
		m_toolTipDlg.ShowWindow(SW_HIDE);
		m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_BARBACK), _T("建造军事基地将消耗20点经济。建成后您即可以在该星球设置军队。"));
		m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_STOPE), _T("建造采矿工厂将消耗20点经济。建成后每年该工厂都会开采本星球的资源直至星球的资源耗尽。"),
					_T("您需要先在“航天技术”部发展“行星开发Ⅰ”或“恒星开发Ⅰ”才能建造。"));
		m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_FACTORY), _T("建造加工工厂将消耗20点经济。建成后每年该工厂都会将现有的一部分资源转化成经济。"),
					_T("您需要先在“航天技术”部发展“行星建设Ⅰ”才能建造。"));
		m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_CITY), _T("建造城市将消耗20点经济。建成后每年城市都会为您增加一定量的人口。"),
					_T("您需要先在“航天技术”部发展“行星殖民Ⅰ”才能建造。"));
		m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_BARBACK_OPEN), _T("打开军事基地，配置本星球的军力。"));

		m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_ATTCK), _T("对该星球的文明发动攻击。"));
		m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_FRIEND), _T("与该星球的文明进行结盟谈判。"));
		m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_BOMB), _T("使用“奇点炸弹”炸掉该星球。您可以在“军事部”生产“奇点炸弹”。"));
		m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_SPY), _T("使用“智子”刺探该星球的军事实力。您可以在“军事部”生产“智子”。"));
	}

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* star = starMgr.GetStar(m_iStarIndex);
	if (!star)
	{
		ASSERT(FALSE);
		return;
	}
	CString sCivi = star->GetBelongToCivi();
	CString sInfo;
	if (sCivi.Compare(EARTH_CIVI_NAME) == 0)
		sInfo = _T("这个星球属于地球文明！");
	else if (!sCivi.IsEmpty())
	{
		CAlienCivilization* pAlien = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(sCivi);

		CString sFrd;
		switch (pAlien->GetFriendshipType())
		{
		case FT_VERYANGRY:
			sFrd = _T("仇恨");
			break;
		case FT_ANGRY:
			sFrd = _T("敌对");
			break;
		case FT_NORMAL:
			sFrd = _T("普通");
			break;
		case FT_FRIEND:
			sFrd = _T("友好");
			break;
		case FT_VERYFRIEND:
			sFrd = _T("亲密");
			break;
		}
		sInfo.Format(_T("这个星球属于%s。\n\n关系：%s"), sCivi, sFrd);
	}
	else
	{
		if (star->GetIsPlanet())
			sInfo = _T("这是一颗行星。");
		else
			sInfo = _T("这是一颗恒星。");
	}

	m_toolTipDlg.SetTool(GetDlgItem(IDC_STATIC_FACE), sInfo);
}
BOOL CPlanetPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CPlanetPanel::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint pt = pMsg->pt;	
		ScreenToClient( &pt );
		m_toolTipDlg.Move(pt);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CPlanetPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int iID = pWnd->GetDlgCtrlID();
	if (iID == IDC_STATIC_STAR)
	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}

	return hbr;
}
