// CultureDepartmentPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "CultureDepartmentPanel.h"
#include "Department.h"
#include "Person.h"
#include "helper.h"

// CCultureDepartmentPanel 对话框

IMPLEMENT_DYNAMIC(CCultureDepartmentPanel, CDialog)
#define WORKER_TIP _T("文化人口： %d 人")

CCultureDepartmentPanel::CCultureDepartmentPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CCultureDepartmentPanel::IDD, pParent)
	, m_sWorker(_T(""))
	, m_sLeader(_T(""))
{

}

CCultureDepartmentPanel::~CCultureDepartmentPanel()
{
}

void CCultureDepartmentPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrFace);
	DDX_Text(pDX, IDC_STATIC_POPULATION, m_sWorker);
	DDX_Text(pDX, IDC_STATIC_LEADER, m_sLeader);
}


BEGIN_MESSAGE_MAP(CCultureDepartmentPanel, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MODCOUNT, &CCultureDepartmentPanel::OnBnClickedButtonModcount)
	ON_BN_CLICKED(IDC_BUTTON_CHGLEADER, &CCultureDepartmentPanel::OnBnClickedButtonChgleader)
END_MESSAGE_MAP()


// CCultureDepartmentPanel 消息处理程序

BOOL CCultureDepartmentPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCultureDepartmentPanel::UpdateUI()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CDepartment *department = (CDepartment*)earthCivi.GetDepartment(DT_CULTURE);
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

	m_sWorker.Format(WORKER_TIP, department->GetWorkerCount());

	UpdateData(FALSE);
	Invalidate();
}
void CCultureDepartmentPanel::OnBnClickedButtonModcount()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CDepartment *department = (CDepartment*)earthCivi.GetDepartment(DT_CULTURE);
	int iNewCnt, iOldCnt;
	iOldCnt = department->GetWorkerCount();
	iNewCnt = CHelper::ChangeWorkerCount(iOldCnt);
	m_sWorker.Format(WORKER_TIP, iNewCnt);
	department->SetWorkerCount(iNewCnt);
	
	int iIdlePop = earthCivi.GetIdlePopulation();
	earthCivi.SetIdlePopulation(iIdlePop + iOldCnt - iNewCnt);

	UpdateData(FALSE);
}

void CCultureDepartmentPanel::OnBnClickedButtonChgleader()
{
	CString sName = CHelper::ChoosePerson();
	if (sName.IsEmpty())
		return;

	m_sLeader = sName;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CDepartment *department = (CDepartment*)earthCivi.GetDepartment(DT_CULTURE);
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

void CCultureDepartmentPanel::OnOK()
{
}

void CCultureDepartmentPanel::OnCancel()
{
}
