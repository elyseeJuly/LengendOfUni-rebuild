// ResultDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "ResultDlg.h"


// CResultDlg 对话框

IMPLEMENT_DYNAMIC(CResultDlg, CDialog)

CResultDlg::CResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResultDlg::IDD, pParent)
	, m_sResult(_T(""))
	, m_sScore(_T(""))
{

}

CResultDlg::~CResultDlg()
{
}

void CResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_RESULT, m_sResult);
	DDX_Text(pDX, IDC_STATIC_SCORE, m_sScore);
}


BEGIN_MESSAGE_MAP(CResultDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CResultDlg 消息处理程序

void CResultDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CFont* pFont = dc.SelectObject(&m_font);
	CRect rect;
	GetClientRect(rect);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255, 0, 0));
	dc.DrawText(m_sHonor, rect, DT_CENTER | DT_TOP);
	dc.SelectObject(pFont);
}

BOOL CResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_font.CreateFont(70, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("宋体"));

	CString sCiviDieout, sCiviBund;
	int iCntDieout = 0, iCntBund = 0, iCntTotal = 0;
	((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCiviManager().GetDieOutCivi(sCiviDieout, iCntDieout);
	((CLengendOfUniApp*)AfxGetApp())->m_game->GetAlienCiviManager().GetBundCivi(sCiviBund, iCntBund);
	if (sCiviDieout.IsEmpty())
		sCiviDieout = _T("无");
	if (sCiviBund.IsEmpty())
		sCiviBund = _T("无");

	iCntTotal = iCntDieout + iCntBund;
	m_sResult = _T("灭绝的文明：") + sCiviDieout + _T("\n\n结盟的文明：") + sCiviBund;

	if (iCntDieout == 0)
		m_sHonor = _T("神圣");
	else if (iCntDieout == iCntTotal)
		m_sHonor = _T("战神");
	else if (iCntDieout == iCntTotal / 2)
		m_sHonor = _T("纵横家");
	else
	{
		float fRate = (float)iCntDieout / (float)iCntTotal;
		if (fRate < 0.25f)
			m_sHonor = _T("文明使者");
		else if (fRate < 0.5f)
			m_sHonor = _T("和平主义");
		else if (fRate < 0.75f)
			m_sHonor = _T("军国主义");
		else if (fRate < 1.00f)
			m_sHonor = _T("战争狂人");
	}

	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	int iScore;
	iScore = earthCivi.GetPopulation() + earthCivi.GetCulture() + earthCivi.GetArmy() - earthCivi.GetTreachery() * 2;
	m_sScore.Format(_T("%s的积分： %d"), ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPlayerName(), iScore);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
