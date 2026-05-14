// HumanResDepartmentPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "HumanResDepartmentPanel.h"
#include "Department.h"
#include "Person.h"
#include "helper.h"
#include "RandomNumber.h"
#include "LengendOfUniDlg.h"

// CHumanResDepartmentPanel 对话框

IMPLEMENT_DYNAMIC(CHumanResDepartmentPanel, CDialog)

CHumanResDepartmentPanel::CHumanResDepartmentPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CHumanResDepartmentPanel::IDD, pParent)
	, m_sLeader(_T(""))
{
	m_iCurYearPrac = 0;
	m_iCurYearEdu = 0;
}

CHumanResDepartmentPanel::~CHumanResDepartmentPanel()
{
}

void CHumanResDepartmentPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrFace);
	DDX_Text(pDX, IDC_STATIC_LEADER, m_sLeader);
}


BEGIN_MESSAGE_MAP(CHumanResDepartmentPanel, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHGLEADER, &CHumanResDepartmentPanel::OnBnClickedButtonChgleader)
	ON_BN_CLICKED(IDC_BUTTON_ARMY_PRACTICE, &CHumanResDepartmentPanel::OnBnClickedButtonArmyPractice)
	ON_BN_CLICKED(IDC_BUTTON_EDUCATION, &CHumanResDepartmentPanel::OnBnClickedButtonEducation)
END_MESSAGE_MAP()


// CHumanResDepartmentPanel 消息处理程序
BOOL CHumanResDepartmentPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateUI();
	if (!IsWindow(m_toolTipDlg.GetSafeHwnd()))
		InitToolTip();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHumanResDepartmentPanel::UpdateUI()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CDepartment *department = (CDepartment*)earthCivi.GetDepartment(DT_HUMANRES);
	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();

	HBITMAP hBmp;
	m_sLeader = department->GetLeader();
	if (m_sLeader.IsEmpty())
	{
		m_sLeader = _T("无");
		UINT iFace = IDB_NONAME;

		hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(iFace));
		m_ctrFace.SetBitmap(hBmp);
	}
	else
	{
		CPerson* person = personMgr.GetPerson(m_sLeader);
		if (person)
		{
			CString sFace;
			sFace = person->GetFaceFile();
			hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sFace, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
			m_ctrFace.SetBitmap(hBmp);
		}
	}

	UpdateButtonUI();
	UpdateData(FALSE);
	Invalidate();
}
void CHumanResDepartmentPanel::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

}

void CHumanResDepartmentPanel::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

}

void CHumanResDepartmentPanel::OnBnClickedButtonChgleader()
{
	CString sName = CHelper::ChoosePerson();
	if (sName.IsEmpty())
		return;

	m_sLeader = sName;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CDepartment *department = (CDepartment*)earthCivi.GetDepartment(DT_HUMANRES);
	CHelper::ChangePersonDepartment(m_sLeader, department);

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
	Invalidate();
}

void CHumanResDepartmentPanel::OnBnClickedButtonArmyPractice()
{
	// 军事训练
	BOOL bRet = CHelper::CanUseEconemy(10, TRUE);
	if (!bRet)
	{
		CString sInfo = _T("执政官，我们的经济少于 10 点，无法进行军事训练。/n为了地球文明的未来，您不可以忽视在军事训练上的投入。");
		CString sInfoDef = _T("我们的经济少于 10 点，无法进行军事训练！");
		CHelper::PersonMsgBox(m_sLeader, sInfo, sInfoDef);
		return;
	}

	CRandomNumber randNum;
	int iAdd = 3 + randNum.random(3);
	CString sFormat;
	sFormat.Format(_T("本次军事训练，我军的战斗力提升了 %d 点。"), iAdd);
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	iAdd += earthCivi.GetArmy();
	earthCivi.SetArmy(iAdd);
	((CLengendOfUniDlg*)this->GetParent())->UpdateValues();
	m_iCurYearPrac = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetYear();

	UpdateButtonUI();
	CHelper::PersonMsgBox(m_sLeader, sFormat, sFormat);
}

void CHumanResDepartmentPanel::UpdateButtonUI()
{
	int iCurYear = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetYear();
	if (iCurYear == m_iCurYearPrac)
	{
		GetDlgItem(IDC_BUTTON_ARMY_PRACTICE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ARMY_PRACTICE)->EnableWindow(TRUE);
	}
	if (iCurYear == m_iCurYearEdu)
	{
		GetDlgItem(IDC_BUTTON_EDUCATION)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_EDUCATION)->EnableWindow(TRUE);
	}
}
void CHumanResDepartmentPanel::OnBnClickedButtonEducation()
{
	// 全民教育
	BOOL bRet = CHelper::CanUseEconemy(10, TRUE);
	if (!bRet)
	{
		CString sInfo = _T("执政官，我们的经济少于 10 点，无法进行全民教育。/n百年大计，教育为本啊！执政官，希望您没有忘记。");
		CString sInfoDef = _T("我们的经济少于 10 点，无法进行全民教育！");
		CHelper::PersonMsgBox(m_sLeader, sInfo, sInfoDef);
		return;
	}

	CRandomNumber randNum;
	int iAdd = 5 + randNum.random(5);
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	int iCurTreachery = earthCivi.GetTreachery();
	if (iAdd > iCurTreachery)
		iAdd = iCurTreachery;
	iCurTreachery -= iAdd;
	CString sFormat;
	sFormat.Format(_T("通过本次全民教育，全民逃亡主义下降 %d 点。"), iAdd);
	earthCivi.SetTreachery(iCurTreachery);
	((CLengendOfUniDlg*)this->GetParent())->UpdateValues();
	m_iCurYearEdu = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetYear();

	UpdateButtonUI();
	CHelper::PersonMsgBox(m_sLeader, sFormat, sFormat);
}

BOOL CHumanResDepartmentPanel::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CPoint pt = pMsg->pt;	
		ScreenToClient( &pt );
		m_toolTipDlg.Move(pt);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CHumanResDepartmentPanel::InitToolTip()
{
	m_toolTipDlg.Create(IDD_TOOLTIP_DLG, this);
	m_toolTipDlg.ShowWindow(SW_HIDE);
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_ARMY_PRACTICE), _T("通过训练提升我军的战斗力。消耗10点经济。"), _T("每年只能进行一次军事训练。"));
	m_toolTipDlg.AddTool(GetDlgItem(IDC_BUTTON_EDUCATION), _T("通过全民教育降低公民的逃亡主义倾向。消耗10点经济。"), _T("每年只能进行一次全民教育。"));
}
