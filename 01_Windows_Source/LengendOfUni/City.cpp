#include "StdAfx.h"
#include "City.h"
#include "EarthCivilization.h"
#include "LengendOfUni.h"
#include "TecTreeManager.h"
#include "PersonManager.h"
#include "StarManager.h"
#include "RandomNumber.h"
#include "Helper.h"

#define MAX_FIND_PERSON 2

IMPLEMENT_SERIAL(CCity, CArchitecture, 1)

CCity::CCity(void)
{
	m_iTotalBuild = 400;
	m_iFindPersonCnt = 0;
	m_iCurYear = 1;
}

CCity::~CCity(void)
{
}

void CCity::RunARound()
{
	CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
	CTecTreeManager &tecTreeMgr = earthCivi.GetTecTreeManager();
	BOOL bFinish = tecTreeMgr.IsTecFinished(TT_MILITARY, _T("行星殖民Ⅲ"));
	if (bFinish)
		SetBuildPerRound(50);
	else
	{
		bFinish = tecTreeMgr.IsTecFinished(TT_MILITARY, _T("行星殖民Ⅱ"));
		if (bFinish)
			SetBuildPerRound(40);
		else
		{
			bFinish = tecTreeMgr.IsTecFinished(TT_MILITARY, _T("行星殖民Ⅰ"));
			if (bFinish)
				SetBuildPerRound(20);
		}
	}

	CArchitecture::RunARound();

	if (!GetBuildCompleted())
		return;

	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pStar = starMgr.GetStar(m_iPlanetIndex);
	if (!pStar)
		return;

	int iCurPop = pStar->GetCurrentPopulation();
	int iPopLimit = pStar->GetPopulationLimit();
	if (iCurPop == iPopLimit)
		return;
	int iMax = 30;		// 阈值，一局的发展不能超过该值
	int iWeight = 2;
	bFinish = tecTreeMgr.IsTecFinished(TT_INTERSTELLAR, _T("人口Ⅲ"));
	if (bFinish)
		iWeight = 5;
	else
	{
		bFinish = tecTreeMgr.IsTecFinished(TT_INTERSTELLAR, _T("人口Ⅱ"));
		if (bFinish)
			iWeight = 4;
		else
		{
			bFinish = tecTreeMgr.IsTecFinished(TT_INTERSTELLAR, _T("人口Ⅰ"));
			if (bFinish)
				iWeight = 3;
		}
	}

//	int iCulture = earthCivi.GetCulture();
//	iCulture /= 10;
	int iAdd;
	iAdd = 5 * iWeight / 2;
	if (iAdd > iMax)
		iAdd = iMax;
	if (iAdd > iPopLimit - iCurPop)
		iAdd = iPopLimit - iCurPop;
	int iPop = earthCivi.GetPopulation();
	iPop += iAdd;
	earthCivi.SetPopulation(iPop);
	earthCivi.SetIdlePopulation(earthCivi.GetIdlePopulation() + iAdd);
	
	iCurPop += iAdd;
	pStar->SetCurrentPopulation(iCurPop);
}

BOOL CCity::FindPerson(int iCurYear)
{
	if (m_iCurYear == iCurYear)
		return FALSE;

	m_iCurYear = iCurYear;

	if (m_iFindPersonCnt >= MAX_FIND_PERSON)
		return FALSE;

	CRandomNumber randNum;
	int iRand = randNum.random(4);
	if (iRand != 2)	// 1/4 的机会
		return FALSE;

	CPersonManager &personMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
	CString sName = personMgr.FindPerson();
	if (!sName.IsEmpty())
	{
		CString sTalk[] = {_T("美妙人生的关键在于你能迷上什么东西。"),
							_T("在一个不可知的宇宙里，我的心脏懒得跳动了。"),
							_T("宇宙的最不可理解之处在于它是可以理解的；宇宙的最可理解之处在于它是不可理解的。"),
							_T("我们都是阴沟里的虫子，但总还是得有人仰望星空。"),
							_T("我们必须抱有希望，这并不是因为希望真的存在，而是因为我们要做高贵的人 。"),
							_T("我很不幸地不麻木, 所以难以生存下去。"),
							_T("理想主义者和玩世不恭的人都觉得对方很可怜，可他们实际都很幸运。每一种生活方式都是合理的，不需要相互理解。"),
							_T("现在人们应该明白了，就算真的有上帝，他也一定是个王八蛋。"),
							_T("世界上有很多东西值得为之献出生命,但没有东西值得为之睡觉。"),
							_T("有的人满足于老婆孩子热炕头，从不向与己无关的尘世之外扫一眼；有的人则用尽全部生命，只为看一眼人类从未见过的事物。这两种人我都做过，但我知道现在我该选择什么。")};
		iRand = randNum.random(5);
		CString sFormat;
		sFormat.Format(_T("（发现新人物！）\n我叫%s。%s我愿意为我们的星球效命！"), sName, sTalk[iRand]);
		CHelper::PersonMsgBox(personMgr.GetPerson(sName), sFormat);
		m_iFindPersonCnt ++;
		return TRUE;
	}

	return FALSE;
}

void CCity::Serialize(CArchive &ar)
{
	CArchitecture::Serialize(ar);
	if(ar.IsStoring()) 
	{ 
		ar << m_iFindPersonCnt << m_iCurYear;
	}
	else
	{
		ar >> m_iFindPersonCnt >> m_iCurYear;
	}

}
