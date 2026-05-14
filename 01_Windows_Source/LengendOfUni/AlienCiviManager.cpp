#include "StdAfx.h"
#include "AlienCiviManager.h"
#include "IniConfig.h"
#include "LengendOfUni.h"

IMPLEMENT_SERIAL(CAlienCiviManager, CObject, 1)

CAlienCiviManager::CAlienCiviManager(void)
{
}

CAlienCiviManager::~CAlienCiviManager(void)
{
	Clear();
}

void CAlienCiviManager::Init()
{
//	CString sAlienNameArray[] = {_T("吞食帝国"),_T("三体文明"),_T("李白文明"),_T("碎星者"),_T("排险者"),
//		_T("碳基联邦"),_T("上帝文明"),_T("哥哥文明"),_T("低温艺术家"),_T("波江座文明"),_T("绿洋星文明")};
	CIniConfig iniConfig(_T("alien.ini"));
	int iCnt;
	iCnt = iniConfig.GetInt(_T("count"), _T("count"));

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();

	for(int i = 0; i < iCnt; i ++)
	{
		iniConfig.SetFile(_T("alien.ini"));
		CString sSection;
		sSection.Format(_T("%d"), i);
		CAlienCivilization *pAlienCivi = new CAlienCivilization;
		pAlienCivi->SetName(iniConfig.GetString(sSection ,_T("Name")));
		pAlienCivi->SetPopulation(iniConfig.GetInt(sSection ,_T("Population")));
		pAlienCivi->SetCulture(iniConfig.GetInt(sSection ,_T("Culture")));
		pAlienCivi->SetEconomy(iniConfig.GetInt(sSection ,_T("Economy")));
		pAlienCivi->SetCiviLevel(iniConfig.GetInt(sSection ,_T("CiviLevel")));
		pAlienCivi->SetArmy(iniConfig.GetInt(sSection ,_T("Army")));
		pAlienCivi->SetPersonality((AI_PERSONALITY)iniConfig.GetInt(sSection, _T("Personality")));
		int iStarIndex;
		iStarIndex = iniConfig.GetInt(sSection ,_T("StarIndex"));
		iniConfig.SetFile(_T("star.ini"));
		sSection.Format(_T("%d"), iStarIndex);
		CString sStarName;
		sStarName = iniConfig.GetString(sSection ,_T("Name"));
		CStar *pStar = starMgr.GetStar(sStarName);
		if (!pStar)
		{
			ASSERT(FALSE);
			return;
		}
		iStarIndex = pStar->GetIndex();

		pAlienCivi->BuildBarback(iStarIndex, TRUE);
		pAlienCivi->GetBarback(iStarIndex)->SetSoldierCount(pAlienCivi->GetPopulation());

/*		HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
		TCHAR lpFn[255];   
		::GetModuleFileName(hModule, lpFn, 255);
		CString sPic;
		sPic = lpFn;
		int iPos = sPic.ReverseFind('\\');
		sPic = sPic.Left(iPos + 1);
		sPic += _T("images\\");*/
		CString sPic;
		sPic = pAlienCivi->GetName();
		sPic += _T(".bmp");

		pAlienCivi->SetPic(sPic);
		m_mapAlienCivi[pAlienCivi->GetName()] = pAlienCivi;
	}

	// for test
	// 吞食帝国在海王星上设了50万精兵
/*	m_mapAlienCivi[_T("吞食帝国")]->BuildBarback(8, TRUE);
	CBarback* pBarback = m_mapAlienCivi[_T("吞食帝国")]->GetBarback(8);
	if (pBarback)
	{
		pBarback->SetSoldierCount(50);
		CWeaponEx *weaponEx = new CWeaponEx;
		weaponEx->SetWeaponName(_T("宏化部队"));
		weaponEx->SetFinish();
		pBarback->AddWeapon(weaponEx);
		CWeaponEx *weaponEx1 = new CWeaponEx;
		weaponEx1->SetWeaponName(_T("恒星级氢弹"));
		weaponEx1->SetFinish();
		pBarback->AddWeapon(weaponEx1);
	}*/
	// end test
}

CAlienCivilization* CAlienCiviManager::GetAlienCivi(CString sName)
{
	if (m_mapAlienCivi.find(sName) == m_mapAlienCivi.end())
		return NULL;

	return m_mapAlienCivi[sName];
}

void CAlienCiviManager::Clear()
{
	std::map<CString, CAlienCivilization*>::iterator itr;
	for (itr = m_mapAlienCivi.begin(); itr != m_mapAlienCivi.end(); itr ++)
	{
		SAFE_DELETE(itr->second)
	}
	m_mapAlienCivi.clear();
}

void CAlienCiviManager::RunARound()
{
	std::map<CString, CAlienCivilization*>::iterator itr;
	for (itr = m_mapAlienCivi.begin(); itr != m_mapAlienCivi.end(); itr ++)
	{
		itr->second->RunARound();
	}
}

BOOL CAlienCiviManager::IsAllCiviConquered()
{
	BOOL bAllConquered = TRUE;
	std::map<CString, CAlienCivilization*>::iterator itr;
	for (itr = m_mapAlienCivi.begin(); itr != m_mapAlienCivi.end(); itr ++)
	{
		if (!itr->second->IsDieOut() && !itr->second->IsBund())
		{
			bAllConquered = FALSE;
			break;
		}
	}

	return bAllConquered;
}

std::map<CString, CAlienCivilization*> &CAlienCiviManager::GetAlienCiviMap()
{
	return m_mapAlienCivi;
}

void CAlienCiviManager::Serialize(CArchive &ar)
{
	CObject::Serialize(ar); 
	if(ar.IsStoring()) 
	{ 
		ar << m_mapAlienCivi.size();
		std::map<CString, CAlienCivilization*>::iterator itr;
		for (itr = m_mapAlienCivi.begin(); itr != m_mapAlienCivi.end(); itr ++)
		{
			ar << itr->second;
		}
	}
	else
	{
		int iVal = 0;
		ar >> iVal;
		for (int i = 0; i < iVal; i ++)
		{
			CAlienCivilization *pAlienCivi;// = new CAlienCivilization;
			ar >> pAlienCivi;
			m_mapAlienCivi[pAlienCivi->GetName()] = pAlienCivi;
		}
	}

}

void CAlienCiviManager::GetDieOutCivi(CString &sCivis, int &iCnt)
{
	iCnt = 0;
	sCivis.Empty();
	std::map<CString, CAlienCivilization*>::iterator itr;
	for (itr = m_mapAlienCivi.begin(); itr != m_mapAlienCivi.end(); itr ++)
	{
		if (itr->second->IsDieOut())
		{
			iCnt ++;
			sCivis += itr->second->GetName();
			sCivis += _T("  ");
		}
	}
}

void CAlienCiviManager::GetBundCivi(CString &sCivis, int &iCnt)
{
	iCnt = 0;
	sCivis.Empty();
	std::map<CString, CAlienCivilization*>::iterator itr;
	for (itr = m_mapAlienCivi.begin(); itr != m_mapAlienCivi.end(); itr ++)
	{
		if (itr->second->IsBund())
		{
			iCnt ++;
			sCivis += itr->second->GetName();
			sCivis += _T("  ");
		}
	}
}
