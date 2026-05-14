// AlignmentDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "AlignmentDlg.h"
#include "RandomNumber.h"
#include "Helper.h"

#define MP_STRING _T("精神：%d/%d")
#define ALIGNMENG_TIMER_ID 1984
#define RESUMEMP_TIMER_ID 1985
// CAlignmentDlg 对话框

IMPLEMENT_DYNAMIC(CAlignmentDlg, CDialog)

CAlignmentDlg::CAlignmentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlignmentDlg::IDD, pParent)
	, m_sLeaderName(_T(""))
	, m_sEnemyName(_T(""))
	, m_sLeaderMP(_T(""))
	, m_sInfo(_T(""))
{
	m_iTotalMP = 0;
	m_iCurMP = 0;
	m_iEnemyStarIndex = 0;
	m_iAlignmentCountDown = 0;
	m_iEnemyTec = 0;
}

CAlignmentDlg::~CAlignmentDlg()
{
}

void CAlignmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_LEADER, m_sLeaderName);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrFace);
	DDX_Control(pDX, IDC_STATIC_ENEMYFACE, m_ctrEnemyFace);
	DDX_Text(pDX, IDC_STATIC_ENEMYNAME, m_sEnemyName);
	DDX_Text(pDX, IDC_STATIC_MP, m_sLeaderMP);
	DDX_Control(pDX, IDC_PROGRESS_ASIGNMENT, m_ctrProgress);
	DDX_Text(pDX, IDC_STATIC_INFO, m_sInfo);
}


BEGIN_MESSAGE_MAP(CAlignmentDlg, CDialog)
	ON_WM_TIMER()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON1, IDC_BUTTON6, &CAlignmentDlg::OnFuncBtnClick)
END_MESSAGE_MAP()


// CAlignmentDlg 消息处理程序

void CAlignmentDlg::Init(int iEnemyStar, CString sLeaderName)
{
	m_iEnemyStarIndex = iEnemyStar;
	m_sLeaderName = sLeaderName;
}

BOOL CAlignmentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitFace();
	InitBtns();
	DoEnemySpeak();
	SetTimer(RESUMEMP_TIMER_ID, 500, NULL);
	UpdateData(FALSE);
	InitToolTip();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAlignmentDlg::InitFace()
{
	// 我方头像
	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	CPerson* person = personMgr.GetPerson(m_sLeaderName);
	if (person)
	{
		CString sFace;
		sFace = person->GetFaceFile();
		HBITMAP hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sFace, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

		m_ctrFace.SetBitmap(hBmp);

		m_iCurMP = m_iTotalMP = (person->GetLeadership() * 4 + person->GetSocial() * 6) / 10;
		m_sLeaderMP.Format(MP_STRING, m_iCurMP, m_iTotalMP);
	}

	// 对方头像
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pEnemyStar = starMgr.GetStar(m_iEnemyStarIndex);
	if (!pEnemyStar)
		return;
	m_sEnemyName = pEnemyStar->GetBelongToCivi();
	CAlienCivilization* pAlien = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(m_sEnemyName);
	CString sPic;
	sPic = pAlien->GetPic();
	HBITMAP hBmp;
	hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sPic, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_ctrEnemyFace.SetBitmap(hBmp);
}

void CAlignmentDlg::InitBtns()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();

	CString sTecArray[] = {_T("宇宙社会学公理"), _T("技术爆炸理论"), _T("猜疑链"), _T("黑暗森林理论"), _T("种族沟通")};
	UINT uBaseID = IDC_BUTTON1;

	for (int i = 0; i < 5; i ++)
	{
		BOOL bFinish = tecTreeMgr.IsTecFinished(TT_PHYSICS, sTecArray[i]);
		if (bFinish)
			GetDlgItem(uBaseID + i)->EnableWindow();
		else
			GetDlgItem(uBaseID + i)->EnableWindow(FALSE);
	}

	m_ctrProgress.SetRange(0, 100);
	m_ctrProgress.SetPos(50);
}

void CAlignmentDlg::DoEnemySpeak()
{
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pEnemyStar = starMgr.GetStar(m_iEnemyStarIndex);
	if (!pEnemyStar)
		return;
	m_sEnemyName = pEnemyStar->GetBelongToCivi();
	CAlienCivilization* pAlien = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(m_sEnemyName);
	int iCiviLevel = pAlien->GetCiviLevel();
	std::vector<CString> vecStr;

	CString sInfoArray[] = {_T("%s执政官：尊贵的客人，你应该知道“生存是文明的第一需要，文明不断扩张，但宇宙中物质问题不变”的道理吧？"),
		_T("%s执政官：你要有足够理由让我相信你们地球文明有可能成为一个强大的文明。"),
		_T("%s执政官：我们凭什么信得过你？"),
		_T("%s执政官：这宇宙就像一座黑暗森林，每个文明都是带枪的猎人。你们得拥有强大的理论工具才是靠得住的。"),
		_T("%s执政官：你我两种不同的文明有着天壤之别的价值标准，地球人学会与异种生物的沟通技术了吗？")};
	CString sTecArray[] = {_T("宇宙社会学公理"), _T("技术爆炸理论"), _T("猜疑链"), _T("黑暗森林理论"), _T("种族沟通")};

	int iMax = (iCiviLevel / 2 + 1) * 2;
	if (iMax > 5)
		iMax = 5;
	for (int i = 0; i < iMax; i ++)
	{
		CString sAlignment1, sAlignment2, sAlignment;
		sAlignment1.Format(sInfoArray[i], pAlien->GetName());
		sAlignment2.Format(_T("%s：使用“%s”。"), m_sLeaderName, sTecArray[i]);
		sAlignment.Format(_T("%s\n%s"), sAlignment1, sAlignment2);
		vecStr.push_back(sAlignment);
	}

	CRandomNumber randNum;
	int iRand = randNum.random(vecStr.size());
	m_sTipInfoShown = vecStr[iRand];
	m_iEnemyTec = iRand;

	m_iAlignmentCountDown = 10;
	m_sInfo = m_sTipInfoShown;
	UpdateData(FALSE);
	SetTimer(ALIGNMENG_TIMER_ID, 1000, NULL);
}
void CAlignmentDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == ALIGNMENG_TIMER_ID)
	{
		// 谈判
		m_sInfo.Format(_T("%s\t%d 秒"), m_sTipInfoShown, m_iAlignmentCountDown);
		m_iAlignmentCountDown --;

		if (m_iAlignmentCountDown < 0)
		{
			int iTecValueArray[] = {-10, -14, -17, -20, -25};

			KillTimer(nIDEvent);
			m_ctrProgress.OffsetPos(iTecValueArray[m_iEnemyTec]);

			if (m_ctrProgress.GetPos() <= 0)
			{
				// 进度条归零了，玩家输了
				Lose();
			}
			else
				DoEnemySpeak();
		}
		GetDlgItem(IDC_STATIC_TIP)->ShowWindow(SW_HIDE);

	}
	else if (nIDEvent == RESUMEMP_TIMER_ID)
	{
		// 恢复mp
		if (m_iCurMP < m_iTotalMP)
			m_iCurMP ++;
		m_sLeaderMP.Format(MP_STRING, m_iCurMP, m_iTotalMP);
	
	}
	UpdateData(FALSE);

	CDialog::OnTimer(nIDEvent);
}

void CAlignmentDlg::OnFuncBtnClick(UINT uID)
{
	int iTecValueArray[] = {10, 14, 17, 20, 25};
	int iCostMPArray[] = {20, 26, 32, 35, 40};

	if (uID == IDC_BUTTON6)
	{
		int iReduce = -1 * iTecValueArray[m_iEnemyTec];
		m_ctrProgress.OffsetPos(iReduce);
		m_sInfo.Empty();
		UpdateData(FALSE);
	}
	else
	{
		int iTec = uID - IDC_BUTTON1;
		if (iCostMPArray[iTec] > m_iCurMP)
		{
			MessageBeep(MB_ICONEXCLAMATION);
			GetDlgItem(IDC_STATIC_TIP)->ShowWindow(SW_SHOW);
			return;
		}

		m_iCurMP -= iCostMPArray[iTec];

		if (iTec == m_iEnemyTec)
		{
			m_ctrProgress.OffsetPos(iTecValueArray[iTec]);
		}
		else
		{
			int iReduce = -1 * iTecValueArray[iTec] * 2 / 3;
			m_ctrProgress.OffsetPos(iReduce);
		}
	}

	KillTimer(ALIGNMENG_TIMER_ID);
	if (m_ctrProgress.GetPos() <= 0)
	{
		// 进度条归零了，玩家输了
		Lose();
	}
	else if (m_ctrProgress.GetPos() >= 100)
	{
		// 进度条满，玩家赢了
		
		Win();
	}
	else
		DoEnemySpeak();

}


void CAlignmentDlg::Win()
{
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pEnemyStar = starMgr.GetStar(m_iEnemyStarIndex);
	if (pEnemyStar)
	{
		pEnemyStar->GetBarback()->SetIsFriend(TRUE);
	}

	KillTimer(RESUMEMP_TIMER_ID);
	CAlienCivilization* pAlien = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(m_sEnemyName);
	pAlien->AddFriendshipDegree();
	if (pAlien->IsBund())
		CHelper::CiviMsgBox(pAlien, _T("我很高兴，地球人，我们对你们的技术水平和谈判能力非常满意，你们是我们最合适的盟友。\n\n（结为牢固同盟）"));
	else
		CHelper::CiviMsgBox(pAlien, _T("不错的文明，我们会和你们合作的。"));
	if (pEnemyStar)
	{
		CString sHistory;
		sHistory.Format(_T("我们派出%s与%s星球的%s进行了和平谈判，并取得了显著的成果。"),
			m_sLeaderName, pEnemyStar->GetName(), pAlien->GetName());
		CHelper::AddHistory(sHistory);
	}
	CDialog::OnOK();
}

void CAlignmentDlg::Lose()
{
	KillTimer(RESUMEMP_TIMER_ID);
	CAlienCivilization* pAlien = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCivi(m_sEnemyName);
	CHelper::CiviMsgBox(pAlien, _T("地球人，我们对你们的技术水平和谈判能力非常失望，你们不是我们理想的盟友。"));

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pEnemyStar = starMgr.GetStar(m_iEnemyStarIndex);
	if (pEnemyStar)
	{
		CString sHistory;
		sHistory.Format(_T("我们派出%s与%s星球的%s进行了和平谈判，结果没有任何进展。"),
			m_sLeaderName, pEnemyStar->GetName(), pAlien->GetName());
		CHelper::AddHistory(sHistory);
	}

	CDialog::OnCancel();
}
BOOL CAlignmentDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint pt = pMsg->pt;	
		ScreenToClient( &pt );
		m_toolTipDlg.Move(pt);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CAlignmentDlg::InitToolTip()
{
	m_toolTipDlg.Create(IDD_TOOLTIP_DLG, this);
	m_toolTipDlg.ShowWindow(SW_HIDE);

	int iCostMPArray[] = {20, 26, 32, 35, 40};

	for (UINT uIndex = IDC_BUTTON1; uIndex <= IDC_BUTTON5; uIndex ++)
	{
		CString sInfo;
		sInfo.Format(_T("消耗 %d 点精神"), iCostMPArray[uIndex - IDC_BUTTON1]);
		m_toolTipDlg.AddTool(GetDlgItem(uIndex), sInfo, _T("尚未掌握此技术。请先在“宇宙社会学研究所”进行研究。"));
	}
}
void CAlignmentDlg::OnCancel()
{

}

void CAlignmentDlg::OnOK()
{
}
