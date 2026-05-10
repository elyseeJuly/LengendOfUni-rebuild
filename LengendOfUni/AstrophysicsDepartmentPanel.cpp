// AstrophysicsDepartmentPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "AstrophysicsDepartmentPanel.h"
#include "AstrophysicsTecTreeDlg.h"
#include "helper.h"


// CAstrophysicsDepartmentPanel 对话框
#define WORKER_TIP _T("研究人口： %d 人")
#define DIRECTION_TIP _T("研究方向： %s（进度　%d%%）")
IMPLEMENT_DYNAMIC(CAstrophysicsDepartmentPanel, CDialog)

CAstrophysicsDepartmentPanel::CAstrophysicsDepartmentPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CAstrophysicsDepartmentPanel::IDD, pParent)
	, m_sWorker(_T(""))
	, m_sDirection(_T(""))
{

}

CAstrophysicsDepartmentPanel::~CAstrophysicsDepartmentPanel()
{
}

void CAstrophysicsDepartmentPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FACE, m_ctrFace);
	DDX_Text(pDX, IDC_STATIC_LEADER, m_sLeader);
	DDX_Text(pDX, IDC_STATIC_WORKER, m_sWorker);
	DDX_Text(pDX, IDC_STATIC_DIRECTION, m_sDirection);
}


BEGIN_MESSAGE_MAP(CAstrophysicsDepartmentPanel, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETTARGET, &CAstrophysicsDepartmentPanel::OnBnClickedButtonSettarget)
	ON_BN_CLICKED(IDC_BUTTON_CHGLEADER, &CAstrophysicsDepartmentPanel::OnBnClickedButtonChgleader)
	ON_BN_CLICKED(IDC_BUTTON_MODNUM, &CAstrophysicsDepartmentPanel::OnBnClickedButtonModnum)
END_MESSAGE_MAP()


// CAstrophysicsDepartmentPanel 消息处理程序

void CAstrophysicsDepartmentPanel::OnBnClickedButtonSettarget()
{
	CAstrophysicsTecTreeDlg dlg;
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTree* pTree = earthCivi.GetTecTreeManager().GetTecTree(TT_ASTROPHYSICS);

	dlg.Init(pTree);
	if (dlg.DoModal() == IDOK)
	{
		CTecTreeNode* pNode = pTree->GetChildNode(dlg.m_sTecName);
		if (pNode)
		{
			if (!pNode->GetInResearch())
			{
				// 设置该技术为在研究
				pNode->SetInResearch(TRUE);
				pTree->SetResearchTecNode(pNode);
				m_sDirection.Format(DIRECTION_TIP, dlg.m_sTecName, 0);
				UpdateData(FALSE);
			}
		}
	}}


BOOL CAstrophysicsDepartmentPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAstrophysicsDepartmentPanel::UpdateUI()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CDepartment *department = (CDepartment*)earthCivi.GetDepartment(DT_ASTROPHYSICS);
	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	CTecTreeNode* pNode = earthCivi.GetTecTreeManager().GetTecTree(TT_ASTROPHYSICS)->GetResearchTecNode();

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
	if (pNode)
	{
		m_sDirection.Format(DIRECTION_TIP, pNode->GetName(), pNode->GetCurrentWorkload() * 100 / pNode->GetTotalWorkload());
	}
	else
	{
		m_sDirection.Format(DIRECTION_TIP, _T("空"), 0);
	}

	UpdateData(FALSE);
	Invalidate();
}
void CAstrophysicsDepartmentPanel::OnCancel()
{

}

void CAstrophysicsDepartmentPanel::OnOK()
{

}

void CAstrophysicsDepartmentPanel::OnBnClickedButtonChgleader()
{
	CString sName = CHelper::ChoosePerson();
	if (sName.IsEmpty())
		return;

	m_sLeader = sName;

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CDepartment *department = (CDepartment*)earthCivi.GetDepartment(DT_ASTROPHYSICS);
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

void CAstrophysicsDepartmentPanel::OnBnClickedButtonModnum()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CDepartment *department = (CDepartment*)earthCivi.GetDepartment(DT_ASTROPHYSICS);
	int iNewCnt, iOldCnt;
	iOldCnt = department->GetWorkerCount();
	iNewCnt = CHelper::ChangeWorkerCount(iOldCnt);
	m_sWorker.Format(WORKER_TIP, iNewCnt);
	department->SetWorkerCount(iNewCnt);

	int iIdlePop = earthCivi.GetIdlePopulation();
	earthCivi.SetIdlePopulation(iIdlePop + iOldCnt - iNewCnt);

	UpdateData(FALSE);
}
