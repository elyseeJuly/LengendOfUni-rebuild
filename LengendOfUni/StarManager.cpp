#include "StdAfx.h"
#include "StarManager.h"
#include "IniConfig.h"
#include "RandomNumber.h"
#include <algorithm>

IMPLEMENT_SERIAL(CStarManager, CObject, 1)

CStarManager::CStarManager(void)
{
}

CStarManager::~CStarManager(void)
{
	Clear();
}

void CStarManager::Init()
{
	for (int i = 0; i < SA_COUNT; i ++)
	{
		StarIndexVector starIndexMap;
		m_starMapVector.push_back(starIndexMap);
	}

	// 先简单初始化所有星球，再读配置
	CRandomNumber randNum;
	for (int i = 0; i < 209; i ++)
	{
		CStar *pStar;
		if (i < 9)
			pStar = NULL;
		else
		{
			pStar = new CStar;
			CString sStarName;
			sStarName.Format(_T("星%d"), i);
			pStar->SetIndex(i);
			pStar->SetName(sStarName);
			pStar->SetIsPlanet(TRUE);

			int iValue;
			iValue = randNum.random(4000) + 1;	// for test
			pStar->SetTotalResource(iValue);
			pStar->SetCurrentResource(iValue);
			iValue = randNum.random(300) + 10;
			pStar->SetPopulationLimit(iValue);
			if (randNum.random(5) == 2)		// 1/5的可能是恒星
				pStar->SetIsPlanet(FALSE);
		}

		m_starVector.push_back(pStar);
		// m_starMapVector似乎没啥用了，再看
		if (i < 9)
			m_starMapVector[SA_SOLARSYSTEM].push_back(i);
		else if (i < 49)
			m_starMapVector[SA_50LIGHTYEAR].push_back(i);
		else if (i < 109)
			m_starMapVector[SA_1WLIGHTYEAR].push_back(i);
		else
			m_starMapVector[SA_GALAXY].push_back(i);
	}

	// 开始读配置
	CIniConfig iniConfig(_T("star.ini"));
	int iCnt;
	iCnt = iniConfig.GetInt(_T("count"), _T("count"));
	std::vector<int> vecIndex;

	for (int i = 0; i < iCnt; i ++)
	{
		CString sSection;
		sSection.Format(_T("%d"), i);
		CStar *pStar = new CStar;
		CString sStarName = iniConfig.GetString(sSection ,_T("Name"));
		pStar->SetName(sStarName);
		BOOL bPlanet = (BOOL)iniConfig.GetInt(sSection ,_T("IsPlanet"));
		pStar->SetIsPlanet(bPlanet);
		int iRes = iniConfig.GetInt(sSection ,_T("Res"));
		pStar->SetTotalResource(iRes);
		pStar->SetCurrentResource(iRes);
		int iPopLimit = iniConfig.GetInt(sSection ,_T("PopLimit"));
		pStar->SetPopulationLimit(iPopLimit);

		int iIndex;
		if (i < 9)
		{
			pStar->SetIsFound(TRUE);
			iIndex = i;
		}
		else
		{
			int iStarSys = iniConfig.GetInt(sSection ,_T("StarSys"));
			int iCount;
			int iStartPos;
			while (1)
			{
				if (iStarSys == 1)
				{
					iCount = 40;
					iStartPos = 9;
				}
				else if (iStarSys == 2)
				{
					iCount = 60;
					iStartPos = 49;
				}
				else if (iStarSys == 3)
				{
					iCount = 100;
					iStartPos = 109;
				}
				iIndex = iStartPos + randNum.random(iCount);
				if (find(vecIndex.begin(), vecIndex.end(), iIndex) == vecIndex.end())
				{
					vecIndex.push_back(iIndex);
					break;
				}
			}
		}

		pStar->SetIndex(iIndex);
		SAFE_DELETE(m_starVector[iIndex])
		m_starVector[iIndex] = pStar;
	}


/*	CString sStarNameArray[] = {_T("地球"),_T("太阳"),_T("水星"),_T("金星"),_T("火星"),
		_T("木星"),_T("土星"),_T("天王星"),_T("海王星")};

	for (int i = 0; i < 9; i ++)
	{
		CStar *star = new CStar;
		star->SetIndex(i);
		star->SetName(sStarNameArray[i]);
		if (i == 1)
			star->SetIsPlanet(FALSE);
		else
			star->SetIsPlanet(TRUE);
		if (i == 0)
			star->SetBelongToCivi(EARTH_CIVI_NAME);
//		if (i == 8)		// for test
//			star->SetBelongToCivi(_T("吞食帝国"));
		star->SetIsFound(TRUE);
		star->SetTotalResource(2000);	// 临时给个值
		star->SetCurrentResource(2000);	// 临时给个值
		star->SetPopulationLimit(100);	// 临时给个值

		m_starVector.push_back(star);
		m_starMapVector[SA_SOLARSYSTEM].push_back(i);
	}
	for (int i = 9; i < 209; i ++)
	{
		CString sStarName;
		sStarName.Format(_T("星%d"), i);
		CStar *star = new CStar;
		star->SetIndex(i);
		star->SetName(sStarName);
		star->SetIsPlanet(TRUE);
		star->SetTotalResource(2000);	// 临时给个值
		star->SetCurrentResource(2000);	// 临时给个值
		star->SetPopulationLimit(100);	// 临时给个值
		CRandomNumber randNum;
		if (randNum.random(5) == 2)		// 1/5的可能是恒星
			star->SetIsPlanet(FALSE);

		m_starVector.push_back(star);
		// m_starMapVector似乎没啥用了，再看
		if (i < 49)
			m_starMapVector[SA_50LIGHTYEAR].push_back(i);
		else if (i < 109)
			m_starMapVector[SA_1WLIGHTYEAR].push_back(i);
		else
			m_starMapVector[SA_GALAXY].push_back(i);
	}*/
}

CStar* CStarManager::GetStar(int iIndex)
{
	if (iIndex >= m_starVector.size())
		return NULL;

	return m_starVector[iIndex];
}

void CStarManager::Clear()
{
	for (int i = 0; i < m_starVector.size(); i++)
	{
		SAFE_DELETE(m_starVector[i])
	}
	m_starVector.clear();
}

CStar* CStarManager::GetStar(CString sName)
{
	for (int i = 0; i < m_starVector.size(); i++)
	{
		if (m_starVector[i]->GetName().Compare(sName) == 0)
			return m_starVector[i];
	}

	return NULL;
}

void CStarManager::RemoveStar(int iStarIndex)
{
	if (iStarIndex < 0 || iStarIndex >= m_starVector.size())
		return;

	m_starVector[iStarIndex]->SetIsExist(FALSE);
}

void CStarManager::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	if(ar.IsStoring()) 
	{
		ar << m_starVector.size();
		for (int i = 0; i < m_starVector.size(); i ++)
		{
			ar << m_starVector[i];
		}
	}
	else
	{
		int iVal;
		ar >> iVal;
		for (int i = 0; i < iVal; i ++)
		{
			CStar *pStar = NULL;
			//pStar = new CStar;
			ar >> pStar;
			m_starVector.push_back(pStar);
		}
	}
}