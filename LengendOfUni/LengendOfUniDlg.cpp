// LengendOfUniDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "LengendOfUniDlg.h"
#include "EconomyDepartmentPanel.h"
#include "ArmyDepartmentPanel.h"
#include "CultureDepartmentPanel.h"
#include "HumanResDepartmentPanel.h"
#include "AstrophysicsDepartmentPanel.h"
#include "SpaceflightDepartmentPanel.h"
#include "ProtonDepartmentPanel.h"
#include "NuclearDepartmentPanel.h"
#include "EconomyTecDepartmentPanel.h"
#include "CultureTecDepartmentPanel.h"
#include "AstrosociologyDepartmentPanel.h"
#include "EarthCivilization.h"
#include "solarsystemmap.h"
#include "RandomNumber.h"
#include "HistoryDlg.h"
#include "Helper.h"
#include "CheatDlg.h"
#include "ResultDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define YEAR_STRING _T("危机纪元 %d 年")

// CLengendOfUniDlg 对话框




CLengendOfUniDlg::CLengendOfUniDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLengendOfUniDlg::IDD, pParent)
	, m_iValPopulation(0)
	, m_iValEconomy(0)
	, m_iValArmy(0)
	, m_iValCulture(0)
	, m_iValResource(0)
	, m_iValTreachery(0)
	, m_pPlanetPanel(NULL)
	, m_sYear(_T(""))
	, m_sPlayerName(_T(""))
	, m_canReachStarArea(SA_SOLARSYSTEM)
	, m_sCiviLevelName(_T(""))
	, m_bLoadGame(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CLengendOfUniDlg::~CLengendOfUniDlg()
{
	CleanUp();
}


void CLengendOfUniDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_POPULATION, m_iValPopulation);
	DDX_Text(pDX, IDC_STATIC_ECONOMY, m_iValEconomy);
	DDX_Text(pDX, IDC_STATIC_ARMY, m_iValArmy);
	DDX_Text(pDX, IDC_STATIC_CULTURE, m_iValCulture);
	DDX_Text(pDX, IDC_STATIC_RESOURCE, m_iValResource);
	DDX_Text(pDX, IDC_STATIC_TREACHERY, m_iValTreachery);
	DDX_Text(pDX, IDC_STATIC_YEAR, m_sYear);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrFace);
	DDX_Text(pDX, IDC_STATIC_PLAYER, m_sPlayerName);
	DDX_Text(pDX, IDC_STATIC_CIVI, m_sCiviLevelName);
	DDX_Control(pDX, IDC_STATIC_LINK1, m_ctrLink1);
	DDX_Control(pDX, IDC_STATIC_LINK2, m_ctrLink2);
	DDX_Control(pDX, IDC_STATIC_LINK3, m_ctrLink3);
	DDX_Control(pDX, IDC_STATIC_LINK4, m_ctrLink4);
	DDX_Control(pDX, IDC_STATIC_LINK5, m_ctrLink5);
	DDX_Control(pDX, IDC_STATIC_LINK6, m_ctrLink6);
	DDX_Control(pDX, IDC_STATIC_LINK7, m_ctrLink7);
}

BEGIN_MESSAGE_MAP(CLengendOfUniDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_ECONOMY, &CLengendOfUniDlg::OnBnClickedBtnEconomy)
	ON_BN_CLICKED(IDC_BTN_ARMY, &CLengendOfUniDlg::OnBnClickedBtnArmy)
	ON_BN_CLICKED(IDC_BTN_CULTURE, &CLengendOfUniDlg::OnBnClickedBtnCulture)
	ON_BN_CLICKED(IDC_BTN_HUMANRES, &CLengendOfUniDlg::OnBnClickedBtnHumanres)
	ON_BN_CLICKED(IDC_BTN_ASTROSOCIOLOGY, &CLengendOfUniDlg::OnBnClickedBtnAstrosociology)
	ON_BN_CLICKED(IDC_BTN_NUCLEAR, &CLengendOfUniDlg::OnBnClickedBtnNuclear)
	ON_BN_CLICKED(IDC_BTN_SPACEFIGHT, &CLengendOfUniDlg::OnBnClickedBtnSpacefight)
	ON_BN_CLICKED(IDC_BTN_PROTON, &CLengendOfUniDlg::OnBnClickedBtnProton)
	ON_BN_CLICKED(IDC_BTN_ASTROPHYSICS, &CLengendOfUniDlg::OnBnClickedBtnAstrophysics)
	ON_BN_CLICKED(IDC_BTN_CULTURETEC, &CLengendOfUniDlg::OnBnClickedBtnCulturetec)
	ON_BN_CLICKED(IDC_BTN_ECONOMYTEC, &CLengendOfUniDlg::OnBnClickedBtnEconomytec)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_STARSYS1, IDC_BUTTON_STARSYS4, &CLengendOfUniDlg::OnStarSysBtnClick)
	ON_BN_CLICKED(IDC_BUTTON_PLAN_FINISH, &CLengendOfUniDlg::OnBnClickedButtonPlanFinish)
	ON_BN_CLICKED(IDC_BUTTON_PLAN_HISTORY, &CLengendOfUniDlg::OnBnClickedButtonPlanHistory)

	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PLAN_SAVE, &CLengendOfUniDlg::OnBnClickedButtonPlanSave)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CLengendOfUniDlg 消息处理程序

BOOL CLengendOfUniDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_pPlanetPanel = new CPlanetPanel;
	m_pPlanetPanel->Create(IDD_PLANET_PANEL, this);

	UpdateValues();
	InitPanels();
	SwitchStar(0);
	InitMaps();
	SwitchMap(SA_SOLARSYSTEM);
	UpdateYearLabel();
	UpdatePlayerPanel();
	CenterWindow();

	InitToolTip();
	InitLink();
	InitOthers();
	m_bkBrush.CreateSolidBrush(RGB(115, 109, 99));
	
	if (!m_bLoadGame)
		SetTimer(2008, 500, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLengendOfUniDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CLengendOfUniDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLengendOfUniDlg::InitPanels(void)
{
	CEconomyDepartmentPanel *economyDepPanel = new CEconomyDepartmentPanel;
	economyDepPanel->Create(IDD_ECONOMY_PANEL, this);
	m_vecPanel.push_back(economyDepPanel);

	CArmyDepartmentPanel *armyDepPanel = new CArmyDepartmentPanel;
	armyDepPanel->Create(IDD_ARMY_PANEL, this);
	m_vecPanel.push_back(armyDepPanel);

	CCultureDepartmentPanel *cultureDepPanel = new CCultureDepartmentPanel;
	cultureDepPanel->Create(IDD_CULTURE_PANEL, this);
	m_vecPanel.push_back(cultureDepPanel);

	CHumanResDepartmentPanel *humanresDepPanel = new CHumanResDepartmentPanel;
	humanresDepPanel->Create(IDD_HUMANRES_PANEL, this);
	m_vecPanel.push_back(humanresDepPanel);

	CAstrosociologyDepartmentPanel *astrosociologyDepPanel = new CAstrosociologyDepartmentPanel;
	astrosociologyDepPanel->Create(IDD_ASTROSOCIOLOGY_PANEL, this);
	m_vecPanel.push_back(astrosociologyDepPanel);

	CNuclearDepartmentPanel *nuclearDepPanel = new CNuclearDepartmentPanel;
	nuclearDepPanel->Create(IDD_NUCLEAR_PANEL, this);
	m_vecPanel.push_back(nuclearDepPanel);

	CSpaceflightDepartmentPanel *spaceflightDepPanel = new CSpaceflightDepartmentPanel;
	spaceflightDepPanel->Create(IDD_SPACEFIGHT_PANEL, this);
	m_vecPanel.push_back(spaceflightDepPanel);

	CProtonDepartmentPanel *protonDepPanel = new CProtonDepartmentPanel;
	protonDepPanel->Create(IDD_PROTON_PANEL, this);
	m_vecPanel.push_back(protonDepPanel);

	CAstrophysicsDepartmentPanel *astrophysicsDepPanel = new CAstrophysicsDepartmentPanel;
	astrophysicsDepPanel->Create(IDD_ASTROPHYSICS_PANEL, this);
	m_vecPanel.push_back(astrophysicsDepPanel);

	CCultureTecDepartmentPanel *cultureTecDepPanel = new CCultureTecDepartmentPanel;
	cultureTecDepPanel->Create(IDD_CULTURETEC_PANEL, this);
	m_vecPanel.push_back(cultureTecDepPanel);

	CEconomyTecDepartmentPanel *economyTecDepPanel = new CEconomyTecDepartmentPanel;
	economyTecDepPanel->Create(IDD_ECONOMYTEC_PANEL, this);
	m_vecPanel.push_back(economyTecDepPanel);

}

void CLengendOfUniDlg::SwitchPanel(PANEL_TYPE panelType)
{
	for (int i = 0; i < m_vecPanel.size(); i ++)
		m_vecPanel[i]->ShowWindow(SW_HIDE);
	m_pPlanetPanel->ShowWindow(SW_HIDE);

	CRect  rect;
	GetDlgItem(IDC_FRAME_BOTTOM)->GetWindowRect(rect);
	ScreenToClient(rect);
	rect.InflateRect(-2, -2);
	rect.OffsetRect(1, 1);

	m_vecPanel[panelType]->MoveWindow(rect);
	m_vecPanel[panelType]->ShowWindow(SW_SHOW);
	m_vecPanel[panelType]->OnInitDialog();

	m_pairCurPanel.first = 0;
	m_pairCurPanel.second = (int)panelType;
}


void CLengendOfUniDlg::CleanUp(void)
{
	for (int i = 0; i < m_vecPanel.size(); i ++)
		SAFE_DELETE(m_vecPanel[i])

	m_vecPanel.clear();

	for (int i = 0; i < m_vecMap.size(); i ++)
		SAFE_DELETE(m_vecMap[i])

	m_vecMap.clear();

	SAFE_DELETE(m_pPlanetPanel)
}

void CLengendOfUniDlg::OnBnClickedBtnEconomy()
{
	SwitchPanel(PT_ECONOMY_PANEL);
}

void CLengendOfUniDlg::OnBnClickedBtnArmy()
{
	SwitchPanel(PT_ARMY_PANEL);
}

void CLengendOfUniDlg::OnBnClickedBtnCulture()
{
	SwitchPanel(PT_CULTURE_PANEL);
}

void CLengendOfUniDlg::OnBnClickedBtnHumanres()
{
	SwitchPanel(PT_HUMANRES_PANEL);
}

void CLengendOfUniDlg::OnBnClickedBtnAstrosociology()
{
	SwitchPanel(PT_ASTROSOCIOLOGY_PANEL);
}

void CLengendOfUniDlg::OnBnClickedBtnNuclear()
{
	SwitchPanel(PT_NUCLEAR_PANEL);
}

void CLengendOfUniDlg::OnBnClickedBtnSpacefight()
{
	SwitchPanel(PT_SPACEFIGHT_PANEL);
}

void CLengendOfUniDlg::OnBnClickedBtnProton()
{
	SwitchPanel(PT_PROTON_PANEL);
}

void CLengendOfUniDlg::OnBnClickedBtnAstrophysics()
{
	SwitchPanel(PT_ASTROPHYSICS_PANEL);
}

void CLengendOfUniDlg::OnBnClickedBtnCulturetec()
{
	SwitchPanel(PT_CULTURETEC_PANEL);
}

void CLengendOfUniDlg::OnBnClickedBtnEconomytec()
{
	SwitchPanel(PT_ECONOMYTEC_PANEL);
}

void CLengendOfUniDlg::UpdateValues(void)
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	m_iValPopulation = earthCivi.GetPopulation();
	m_iValEconomy = earthCivi.GetEconomy();
	m_iValArmy = earthCivi.GetArmy();
	m_iValCulture = earthCivi.GetCulture();
	m_iValResource = earthCivi.GetResource();
	m_iValTreachery = earthCivi.GetTreachery();
	UpdateData(FALSE);
}

void CLengendOfUniDlg::InitMaps(void)
{
	CSolarSystemMap *solarSysMap = new CSolarSystemMap;
	solarSysMap->Create(IDD_SOLARSYSTEM_MAP, this);
	m_vecMap.push_back(solarSysMap);

	CStarSystemMap *starSysMap = new CStarSystemMap;
	starSysMap->Init(SA_50LIGHTYEAR);
	starSysMap->Create(IDD_STARSYSTEM_MAP, this);
	m_vecMap.push_back(starSysMap);

	starSysMap = new CStarSystemMap;
	starSysMap->Init(SA_1WLIGHTYEAR);
	starSysMap->Create(IDD_STARSYSTEM_MAP, this);
	m_vecMap.push_back(starSysMap);

	starSysMap = new CStarSystemMap;
	starSysMap->Init(SA_GALAXY);
	starSysMap->Create(IDD_STARSYSTEM_MAP, this);
	m_vecMap.push_back(starSysMap);

}

void CLengendOfUniDlg::SwitchMap(STAR_AREA starArea)
{
	if (starArea == SA_COUNT)
		starArea = m_CurrentStarArea;
	for (int i = 0; i < m_vecMap.size(); i ++)
		m_vecMap[i]->ShowWindow(SW_HIDE);

	DWORD dwFrameWidth    = GetSystemMetrics( SM_CXDLGFRAME );
	DWORD dwFrameHeight   = GetSystemMetrics( SM_CYDLGFRAME );
	DWORD dwCaptionHeight = GetSystemMetrics( SM_CYCAPTION );
	CRect rect1, rect2;
	GetDlgItem(IDC_LINE_TOP)->GetWindowRect(rect1);
	ScreenToClient(rect1);
	GetDlgItem(IDC_FRAME_BOTTOM)->GetWindowRect(rect2);
	ScreenToClient(rect2);
	m_vecMap[starArea]->MoveWindow(rect1.left, rect1.bottom + 1, rect1.Width(), rect2.top - rect1.bottom - 2);
	m_vecMap[starArea]->ShowWindow(SW_SHOW);

	m_vecMap[starArea]->UpdateStarBtns();
	m_CurrentStarArea = starArea;
}

void CLengendOfUniDlg::SwitchStar(int iIndex)
{
	for (int i = 0; i < m_vecPanel.size(); i ++)
		m_vecPanel[i]->ShowWindow(SW_HIDE);

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* star = starMgr.GetStar(iIndex);

//	if (star->GetIsPlanet())
	{
//		m_pPlanetPanel->Init(iIndex);
		CRect  rect;
		GetDlgItem(IDC_FRAME_BOTTOM)->GetWindowRect(rect);
		ScreenToClient(rect);
		rect.InflateRect(-2, -2);
		rect.OffsetRect(1, 1);

		m_pPlanetPanel->MoveWindow(rect);
		m_pPlanetPanel->ShowWindow(SW_SHOW);
		m_pPlanetPanel->Init(iIndex);
		m_pairCurPanel.first = 1;
		m_pairCurPanel.second = iIndex;

		CCity *pCity = star->GetCity();
		if (pCity)
		{
			int iYear = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetYear();
			pCity->FindPerson(iYear);
		}
	}
}

void CLengendOfUniDlg::OnStarSysBtnClick(UINT uID)
{
	STAR_AREA starArea = (STAR_AREA)(uID - IDC_BUTTON_STARSYS1);
	std::vector<CString> vecInfo;
	CString sInfo;
	sInfo = _T("如果说原始人对宇宙的几分钟凝视是看到了一颗宝石，\
其后我们所谓的整个人类文明，不过是弯腰去拾它罢了。\n今天，我们迈出了认识宇宙的第一步！");
	vecInfo.push_back(sInfo);
	sInfo = _T("当生存问题完全解决，当爱情因个体的异化和融和而消失，\
当艺术因过分的精致和晦涩而最终死亡，对宇宙终极美的追求便成为文明存在的惟一寄托。\
\n这也是我们必须走的一步！");
	vecInfo.push_back(sInfo);
	sInfo = _T("也许我们看不到人类洞悉宇宙真理的那一天，但我们坚信我们现在所做的一切，如果人类得以延续万代，\
以后所有的人将在我们的墓前撒下自己的眼泪。这颗叫地球的行星，就是我们永恒的纪念碑！\n银河系，我们来了！");
	vecInfo.push_back(sInfo);

	if (m_canReachStarArea < starArea)
	{
		CHelper::PersonMsgBox(NULL, vecInfo[starArea - 1]);
		m_canReachStarArea = starArea;
	}
	SwitchMap(starArea);
}

void CLengendOfUniDlg::OnBnClickedButtonPlanFinish()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	if (earthCivi.IsDieOut())
	{
		CHelper::PersonMsgBox(NULL, _T("我们丧失了最后的领土，地球文明已经没有落脚之地了。\n\n一个数百万年的文明，就此消亡。\n（游戏结束）"));
		CDialog::OnCancel();
		return;
	}

	if (((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCiviManager().IsAllCiviConquered())
	{
		((CLengendOfUniApp*)AfxGetApp())->m_game->GetGameEventManager().RunNormalEvent(_T("end"), ET_STRINGINDEX);
		CResultDlg dlg;
		dlg.DoModal();
		CDialog::OnCancel();
		return;
	}

	((CLengendOfUniApp*)AfxGetApp())->m_game->RunARound();
	UpdateYearLabel();
	UpdateValues();

	int iYear = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetYear();
	CString sYear;
	sYear.Format(_T("%d"), iYear);

	((CLengendOfUniApp*)AfxGetApp())->m_game->GetGameEventManager().RunNormalEvent(sYear, ET_INYEAR);
	((CLengendOfUniApp*)AfxGetApp())->m_game->GetGameEventManager().RunRandomEvent();

	if (m_pairCurPanel.first == 0)
		SwitchPanel((PANEL_TYPE)m_pairCurPanel.second);
	else if (m_pairCurPanel.first == 1)
		SwitchStar(m_pairCurPanel.second);

	CheckCiviLevelUpgrade();
	UpdateValues();
	SwitchMap();

	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	BOOL bFinish = tecTreeMgr.IsTecFinished(TT_ASTROPHYSICS, _T("50光年望远镜"));
	if (bFinish)
		GetDlgItem(IDC_BUTTON_STARSYS2)->EnableWindow(TRUE);
	bFinish = tecTreeMgr.IsTecFinished(TT_ASTROPHYSICS, _T("1万光年望远镜"));
	if (bFinish)
		GetDlgItem(IDC_BUTTON_STARSYS3)->EnableWindow(TRUE);
	bFinish = tecTreeMgr.IsTecFinished(TT_ASTROPHYSICS, _T("银河系望远镜"));
	if (bFinish)
		GetDlgItem(IDC_BUTTON_STARSYS4)->EnableWindow(TRUE);

	((CLengendOfUniApp*)AfxGetApp())->m_game->ResetReachStarCount();

	CheckGameOver();
	m_sCiviLevelName.Format(_T("文明等级：%s"), earthCivi.GetCiviLevelString());
	UpdateData(FALSE);
}

void CLengendOfUniDlg::UpdateYearLabel()
{
	int iYear = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetYear();
	m_sYear.Format(YEAR_STRING, iYear);
	UpdateData(FALSE);

}

void CLengendOfUniDlg::UpdatePlayerPanel()
{
	HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
	TCHAR lpFn[255];   
	::GetModuleFileName(hModule, lpFn, 255);
	CString sFace;
	sFace = lpFn;
	int iPos = sFace.ReverseFind('\\');
	sFace = sFace.Left(iPos + 1);
	sFace += _T("images\\");
	sFace += _T("player.bmp");

	HBITMAP hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sFace, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_ctrFace.SetBitmap(hBmp);

	m_sPlayerName.Format(_T("%s\n\n地球首席执政官"), ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPlayerName());

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	m_sCiviLevelName.Format(_T("文明等级：%s"), earthCivi.GetCiviLevelString());

	UpdateData(FALSE);
}
void CLengendOfUniDlg::OnBnClickedButtonPlanHistory()
{
	CHistoryDlg dlg;
	dlg.DoModal();
}

void CLengendOfUniDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (CHelper::PersonMsgBox(NULL, _T("真的要退出游戏吗？"), TRUE) == IDOK)
		CDialog::OnCancel();
}

void CLengendOfUniDlg::InitToolTip()
{
	m_toolTipDlg.Create(IDD_TOOLTIP_DLG, this);
	m_toolTipDlg.ShowWindow(SW_HIDE);
	CString sInfo;
	sInfo.Format(_T("在%s统治下的地球编年史"), ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPlayerName());
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_PLAN_HISTORY), sInfo);
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_PLAN_FINISH), _T("完成您在本年度的操作后，点击进入下一年"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_PLAN_SAVE), _T("保存您目前的游戏进度"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_STARSYS1), _T("点击进入太阳系"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_STARSYS2), _T("点击进入50光年范围空间"), _T("暂不可用。\n您需要先在“天体物理部”发展“50光年望远镜”技术"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_STARSYS3), _T("点击进入1万光年范围空间"), _T("暂不可用。\n您需要先在“天体物理部”发展“1万光年望远镜”技术"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_STARSYS4), _T("点击进入银河系范围空间"), _T("暂不可用。\n您需要先在“天体物理部”发展“银河系望远镜”技术"));

	m_toolTipDlg.AddTool(GetDlgItem(IDC_BTN_ECONOMY), _T("进入经济部。在这里您可以设置参与生产的人口。经济的发展将使你具备扩张的实力。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BTN_ARMY), _T("进入军事部。在这里您可以生产终级武器，并能定位到已有的军营。它是您运筹帷幄的司令部。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BTN_CULTURE), _T("进入文化部。在这里您可以设置参与文化建设的人口。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BTN_HUMANRES), _T("进入人力资源部。在这里您得以对全民素质进行提升。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BTN_ASTROSOCIOLOGY), _T("进入宇宙社会学研究所。对宇宙社会学的研究将使您在对外星谈判时受益。"),
				_T("只有文明级别达到“风暴”级才能发展宇宙社会学。您需要提升文化值以实现文明等级的提升。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BTN_NUCLEAR), _T("进入核技术研究所。核技术的发展将使您有能力生产热核武器。"),
				_T("只有文明级别达到“起源”级才能发展航天技术。您需要提升文化值以实现文明等级的提升。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BTN_SPACEFIGHT), _T("进入航天技术研究所。航天技术的发展将使您有能力登陆外星球，并建立殖民地。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BTN_PROTON), _T("进入质子技术研究所。当您的质子技术上升到一定程序时，您就可以生产智子和奇点炸弹。"),
				_T("只有文明级别达到“风暴”级才能发展质子技术。您需要提升文化值以实现文明等级的提升。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BTN_ASTROPHYSICS), _T("进入天体物理研究所。如果您想具有更开阔的宇宙视野，您需要发展天体物理技术。"),
				_T("只有文明级别达到“起源”级才能发展天体物理。您需要提升文化值以实现文明等级的提升。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BTN_CULTURETEC), _T("进入地球文化研究所。这些技术将加快人类文明的进程。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BTN_ECONOMYTEC), _T("进入经济学研究所。这些技术将加快人类的经济发展程度。"));

	m_toolTipDlg.AddTool(GetDlgItem(IDC_STATIC_POPULATION), _T("您需要在各部门分配人口数，以使各部门能正常运转。人口数越多对文明发展越有利。城市每年都会产生新的人口，您可以通过修建更多的城市来加速人口增长。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_STATIC_CULTURE), _T("当文化上升到一定值时，将实现文明等级的提升。文明等级提升将使您能够发展更多的科技，并增加一定的战斗力。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_STATIC_ECONOMY), _T("各部门的运转、新殖民地的开辟都需要一定的经济基础。经济由加工工厂制造出来，并会消耗一定的资源。当资源不足时，经济将无法制造。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_STATIC_RESOURCE), _T("资源由采矿工厂提炼出来，并将被加工工厂转化为经济。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_STATIC_ARMY), _T("战争时普通士兵的攻击值。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_STATIC_TREACHERY), _T("逃亡值越高，生产、研发的效率越低。当逃亡值达到100时，民怨沸腾，将发生难以预料的严重后果。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_STATIC_CIVI), _T("文化值   0--70：      荒蛮\n文化值  70--200：   起源\n文化值 200--500：  风暴\n文化值 500--1000：逐鹿\n文化值 >1000：      霸王"));

}

BOOL CLengendOfUniDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint pt = pMsg->pt;	
		ScreenToClient( &pt );
		m_toolTipDlg.Move(pt);
	}
	else if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F4)
		{
			CCheatDlg dlg;
			dlg.DoModal();
			UpdateValues();
			InitOthers();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CLengendOfUniDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int iID = pWnd->GetDlgCtrlID();
	if (iID == IDC_STATIC_PROPERTY || iID == IDC_STATIC_DEPARTMENT || iID == IDC_STATIC_TEC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		return   static_cast<HBRUSH>(m_bkBrush.GetSafeHandle());   
	}
	else if (iID == IDC_STATIC_YEAR)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	return hbr;
}

void CLengendOfUniDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 2008)
	{
		// 刚打开窗口时，执行的脚本　
		KillTimer(nIDEvent);
		int iYear = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetYear();
		CString sYear;
		sYear.Format(_T("%d"), iYear);

		((CLengendOfUniApp*)AfxGetApp())->m_game->GetGameEventManager().RunNormalEvent(sYear, ET_INYEAR);
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CLengendOfUniDlg::CheckGameOver()
{
	UpdateData();
	// 逃亡主义值满，游戏结束
	if (m_iValTreachery >= 100)
	{
		std::vector<CString> vecInfo;
		std::vector<CString> vecSpeaker;
		CString sInfo;
		sInfo = _T("公民们！地球被出卖了！人类被出卖了！文明被出卖了！我们都是一个超级骗局的牺牲品！\n\n\
联合政府中那些人有着阴险的野心！他们编造了这一切，只是为了建立他们的独裁帝国！他们毁了地球！他们毁了人类文\
明！公民们，有良知的公民们！拿起武器，拯救我们的星球！拯救人类文明！我们要推翻联合政府！");
		vecInfo.push_back(sInfo);
		sInfo = _T("雷迪亚兹");
		vecSpeaker.push_back(sInfo);

		sInfo = _T("人类文明已经不可能通过自身的力量来改善了。我们的理想是请三体文明改造人类文明，遏制人类的\
疯狂和邪恶，让地球再次成为一个和谐繁荣、没有罪恶的世界。认同我们理想的人越来越多，我们的组织在急剧扩大中，成\
员遍布整个世界。\n\n消灭人类暴政！");
		vecInfo.push_back(sInfo);
		sInfo = _T("伊文斯");
		vecSpeaker.push_back(sInfo);

		sInfo = _T("政府开始实施新经济政策，这苦日子就要来了。不过话说回来，苦日子算个屁，我们这岁数的又不是\
没过过，大不了就当回到六零年呗。\n\n只是可怜了孩子们。唉~~");
		vecInfo.push_back(sInfo);
		sInfo = _T("苗福全");
		vecSpeaker.push_back(sInfo);

		CRandomNumber randNum;
		int iRand = randNum.random(vecInfo.size());
		CHelper::PersonMsgBox(vecSpeaker[iRand], vecInfo[iRand], _T(""));

		HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
		TCHAR lpFn[255];   
		::GetModuleFileName(hModule, lpFn, 255);
		CString sFace;
		sFace = lpFn;
		int iPos = sFace.ReverseFind('\\');
		sFace = sFace.Left(iPos + 1);
		sFace += _T("images\\报纸.bmp");

		sInfo.Format(_T("本报讯：持续数月的战争今日终于结束，反叛军占领了地球联盟总部，并就地枪决了包括%s在内的数名地球联盟高官。\
“蓝色联合”组织随即成立，并宣布取代“地球联盟”成为全球最大的国际性组织。据悉，该组织将致力于开展与外星文明的投降谈判事宜。\
\n\n（游戏结束）"), ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPlayerName());
		CHelper::MsgBox(sFace, sInfo, _T("地球晨报"));
		CDialog::OnCancel();
		return TRUE;
	}

	return FALSE;
}

void CLengendOfUniDlg::CheckCiviLevelUpgrade()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	BOOL bCheck = earthCivi.CheckCiviLevelUpgrade();
	if (bCheck)
	{
		CString sFunc;
		int iCiviLevel = earthCivi.GetCiviLevel();
		if (iCiviLevel == 0)
		{
			sFunc = _T("");
		}
		else if (iCiviLevel == 1)
		{
			sFunc = _T("\n\n我们现在可以发展天体物理和核技术了。同时战斗力提升20点");
			GetDlgItem(IDC_BTN_NUCLEAR)->EnableWindow();
			GetDlgItem(IDC_BTN_ASTROPHYSICS)->EnableWindow();
		}
		else if (iCiviLevel == 2)
		{
			sFunc = _T("\n\n我们现在可以发展质子技术和宇宙社会学了。同时战斗力提升20点");
			GetDlgItem(IDC_BTN_ASTROSOCIOLOGY)->EnableWindow();
			GetDlgItem(IDC_BTN_PROTON)->EnableWindow();
		}
		else if (iCiviLevel == 3)
		{
			sFunc = _T("\n\n我军战斗力提升20点");
		}
		else
		{
			sFunc = _T("\n\n我军战斗力提升20点");
		}

		CString sLevel = earthCivi.GetCiviLevelString();
		CString sTip;
		sTip.Format(_T("（升级了！）\n地球文明升级到“%s”级别。%s"), sLevel, sFunc);

		CHelper::PersonMsgBox(NULL, sTip);

	}
}

void CLengendOfUniDlg::InitLink()
{
	m_ctrLink1.SetURL(_T("http://www.ourshow2003.com"));
	m_ctrLink2.SetURL(_T("http://www.ourshow2003.com/zjutindex.htm"));
	m_ctrLink3.SetURL(_T("http://www.ourshow2003.com/guoshang"));
	m_ctrLink4.SetURL(_T("http://www.ourshow2003.com/nmw"));
	m_ctrLink5.SetURL(_T("http://www.ourshow2003.com/lou"));
	m_ctrLink6.SetURL(_T("http://www.ourshow2003.com/news/tormoo.asp"));
	m_ctrLink7.SetURL(_T("http://www.ourshow2003.com/documents/juankuan.htm"));
}
void CLengendOfUniDlg::OnBnClickedButtonPlanSave()
{
	CString sFileName;
	CTime timeData = CTime::GetCurrentTime();
	CString sTime;
	sTime = timeData.Format(_T("%Y%m%d%H%M%S.lsv"));

	HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
	TCHAR lpFn[255];   
	::GetModuleFileName(hModule, lpFn, 255);
	sFileName = lpFn;
	int iPos = sFileName.ReverseFind('\\');
	sFileName = sFileName.Left(iPos + 1);
	sFileName += _T("saves\\");
	sFileName += sTime;

	CFileDialog FileDialog(FALSE, NULL, sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("刘慈欣群星传存档文件 (*.lsv)|*.lsv||"));
	if(FileDialog.DoModal()==IDOK)
	{
		sFileName = FileDialog.GetPathName();
		if (sFileName.Right(4) != _T(".lsv"))
			sFileName += _T(".lsv");

		CFile fileStore(sFileName, CFile::modeWrite | CFile::modeCreate); 
		CArchive arStore(&fileStore, CArchive::store); 
		CString sFlag = _T("LegendOfUni");	// 标识此乃刘慈欣群星传的游戏存档
		arStore << sFlag;
		arStore << (((CLengendOfUniApp*)AfxGetApp())->m_game);
		arStore.Close();
		fileStore.Close(); 

		CHelper::PersonMsgBox(NULL, _T("存档成功。"));
	}

}

void CLengendOfUniDlg::InitOthers()
{
	// 科技部门按钮，看是否enable
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	int iCiviLevel = earthCivi.GetCiviLevel();
	if (iCiviLevel >= 1)
	{
		GetDlgItem(IDC_BTN_NUCLEAR)->EnableWindow();
		GetDlgItem(IDC_BTN_ASTROPHYSICS)->EnableWindow();
	}
	if (iCiviLevel >= 2)
	{
		GetDlgItem(IDC_BTN_ASTROSOCIOLOGY)->EnableWindow();
		GetDlgItem(IDC_BTN_PROTON)->EnableWindow();
	}

	// 星系切换按钮，看是否enable
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	BOOL bFinish = tecTreeMgr.IsTecFinished(TT_ASTROPHYSICS, _T("50光年望远镜"));
	if (bFinish)
	{
		m_canReachStarArea = SA_50LIGHTYEAR;
		GetDlgItem(IDC_BUTTON_STARSYS2)->EnableWindow(TRUE);
	}
	bFinish = tecTreeMgr.IsTecFinished(TT_ASTROPHYSICS, _T("1万光年望远镜"));
	if (bFinish)
	{
		m_canReachStarArea = SA_1WLIGHTYEAR;
		GetDlgItem(IDC_BUTTON_STARSYS3)->EnableWindow(TRUE);
	}
	bFinish = tecTreeMgr.IsTecFinished(TT_ASTROPHYSICS, _T("银河系望远镜"));
	if (bFinish)
	{
		m_canReachStarArea = SA_GALAXY;
		GetDlgItem(IDC_BUTTON_STARSYS4)->EnableWindow(TRUE);
	}
}

