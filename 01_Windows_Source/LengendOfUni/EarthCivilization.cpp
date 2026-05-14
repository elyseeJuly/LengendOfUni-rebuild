#include "StdAfx.h"
#include "EarthCivilization.h"
#include "RandomNumber.h"
#include "EconomyDepartment.h"
#include "StarManager.h"
#include "LengendOfUni.h"
#include "ArmyDepartment.h"
#include "IniConfig.h"
#include <algorithm>

IMPLEMENT_SERIAL(CEarthCivilization, CCivilization, 2)

CEarthCivilization::CEarthCivilization(void)
{
	m_iIdlePopulation = 0;
	m_sSwordholder.Empty();
	m_vecWallfacers.clear();
	m_sLevel = _T("荒蛮");
}

CEarthCivilization::~CEarthCivilization(void)
{
	Clear();
}

void CEarthCivilization::AddWallfacer(CString name)
{
	if (!IsWallfacer(name)) m_vecWallfacers.push_back(name);
}

void CEarthCivilization::RemoveWallfacer(CString name)
{
	std::vector<CString>::iterator it = std::find(m_vecWallfacers.begin(), m_vecWallfacers.end(), name);
	if (it != m_vecWallfacers.end()) m_vecWallfacers.erase(it);
}

BOOL CEarthCivilization::IsWallfacer(CString name)
{
	return std::find(m_vecWallfacers.begin(), m_vecWallfacers.end(), name) != m_vecWallfacers.end();
}

void CEarthCivilization::SetSwordholder(CString name)
{
	m_sSwordholder = name;
}

CString CEarthCivilization::GetSwordholder()
{
	return m_sSwordholder;
}

void CEarthCivilization::SetPopulation(int iVal)
{
	if (iVal < m_iPopulation)
	{
		if (m_tecTreeManager->IsTecFinished(TT_INFORMATION, _T("意识上传")))
		{
			// 数字永生，人口不再因灾害或袭击下降
			return;
		}
	}
	CCivilization::SetPopulation(iVal);
}

void CEarthCivilization::Init()
{
	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* pEarth = starMgr.GetStar(0);

	m_tecTreeManager = new CTecTreeManager;
	m_tecTreeManager->Init();
	const int ciEarth = 0;
	m_lstStars.push_back(ciEarth);
	BuildStope(0, TRUE);
	BuildBarback(0, TRUE);
	BuildFactory(0, TRUE);
	BuildCity(0, TRUE);
/*	CStope *stope = new CStope;
	stope->SetPlanetIndex(ciEarth);
	stope->SetBuildCompleted();
	m_vecStopes.push_back(stope);
	CBarback *barback = new CBarback;
	barback->SetPlanetIndex(ciEarth);
	barback->SetBuildCompleted();
	barback->GetDepartment()->SetName(_T("地球基地"));
	m_vecBarbacks.push_back(barback);
	CFactory *factory = new CFactory;
	factory->SetPlanetIndex(ciEarth);
	factory->SetBuildCompleted();
	m_vecFactories.push_back(factory);
	CCity *city = new CCity;
	city->SetPlanetIndex(ciEarth);
	city->SetBuildCompleted();
	m_vecCities.push_back(city);

	earth->SetStope(stope);
	earth->SetFactory(factory);
	earth->SetBarback(barback);
	earth->SetCity(city);*/

	CString sDepNameArray[] = {_T("经济部"),_T("军事部"),_T("文化部"),_T("人力资源部"),_T("宇宙社会学"),
		_T("核技术"),_T("航天技术"),_T("质子技术"),_T("天体物理"),_T("地球文化"),_T("经济学")};
	for (int i = DT_ECONOMY; i < DT_COUNT; i ++)
	{
		if (i == DT_ECONOMY)
		{
			CEconomyDepartment *dep = new CEconomyDepartment;
			dep->SetName(sDepNameArray[i]);
			m_vecDepartment.push_back(dep);
		}
		else if (i == DT_ARMY)
		{
			CArmyDepartment *dep = new CArmyDepartment;
			dep->SetName(sDepNameArray[i]);
			m_vecDepartment.push_back(dep);
		}
		else
		{
			CDepartment *dep = new CDepartment;
			dep->SetName(sDepNameArray[i]);
			m_vecDepartment.push_back(dep);
		}
		
	}

//	CIniConfig iniConfig(_T("earth.ini"));
	m_iIdlePopulation = m_iPopulation = 65;	//iniConfig.GetInt(_T("earth") ,_T("Population"));
	pEarth->SetCurrentPopulation(m_iPopulation);
	m_iCulture = 0;	//iniConfig.GetInt(_T("earth") ,_T("Culture"));
	m_iEconomy = 100;	//iniConfig.GetInt(_T("earth") ,_T("Economy"));
	m_iResource = 100;	//iniConfig.GetInt(_T("earth") ,_T("Resource"));
	m_iArmy = 10;	//iniConfig.GetInt(_T("earth") ,_T("Army"));
	m_iTreachery = 0;	//iniConfig.GetInt(_T("earth") ,_T("Treachery"));
	m_iCiviLevel = 0;
	m_iPopulationPercent = 0;
	m_iMiningPercent = 30;
	m_iEconomyPercent = 30;
	m_iCulturePercent = 30;
	m_sName = EARTH_CIVI_NAME;

}

CDepartment* CEarthCivilization::GetDepartment(DEPARTMENT_TYPE dptType)
{
	if (dptType < DT_ECONOMY || dptType >= DT_COUNT)
		return NULL;

	return m_vecDepartment[dptType];
}


void CEarthCivilization::Clear()
{
	CCivilization::Clear();
	std::list<CStope*>::iterator itrStope;
	for (itrStope = m_lstStopes.begin(); itrStope != m_lstStopes.end(); itrStope ++)
	{
		SAFE_DELETE(*itrStope)
	}
	m_lstStopes.clear();
	std::list<CFactory*>::iterator itrFactory;
	for (itrFactory = m_lstFactories.begin(); itrFactory != m_lstFactories.end(); itrFactory ++)
	{
		SAFE_DELETE(*itrFactory)
	}
	m_lstFactories.clear();
	std::list<CCity*>::iterator itrCity;
	for (itrCity = m_lstCities.begin(); itrCity != m_lstCities.end(); itrCity ++)
	{
		SAFE_DELETE(*itrCity)
	}
	m_lstCities.clear();
	for (int i = 0; i < m_vecDepartment.size(); i ++)
	{
		SAFE_DELETE(m_vecDepartment[i])
	}
	m_vecDepartment.clear();
}

int CEarthCivilization::GetIdlePopulation()
{
	return m_iIdlePopulation;
}

void CEarthCivilization::SetIdlePopulation(int iVal)
{
	m_iIdlePopulation = iVal;
}


void CEarthCivilization::BuildStope(int iStarIndex, BOOL bCompleted)
{
	CStope *stope = new CStope;
	stope->SetPlanetIndex(iStarIndex);
	m_lstStopes.push_back(stope);

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* star = starMgr.GetStar(iStarIndex);
	if (star)
	{
		star->SetStope(stope);
		star->SetBelongToCivi(EARTH_CIVI_NAME);
	}

	if (bCompleted)
		stope->SetBuildCompleted(bCompleted);

	if (find(m_lstStars.begin(), m_lstStars.end(), iStarIndex) == m_lstStars.end())
	{
		m_lstStars.push_back(iStarIndex);
	}

}

void CEarthCivilization::BuildFactory(int iStarIndex, BOOL bCompleted)
{
	CFactory *factory = new CFactory;
	factory->SetPlanetIndex(iStarIndex);
	m_lstFactories.push_back(factory);

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* star = starMgr.GetStar(iStarIndex);
	if (star)
	{
		star->SetFactory(factory);
		star->SetBelongToCivi(EARTH_CIVI_NAME);
	}

	if (bCompleted)
		factory->SetBuildCompleted(bCompleted);

	if (find(m_lstStars.begin(), m_lstStars.end(), iStarIndex) == m_lstStars.end())
	{
		m_lstStars.push_back(iStarIndex);
	}

}

void CEarthCivilization::BuildCity(int iStarIndex, BOOL bCompleted)
{
	CCity *city = new CCity;
	city->SetPlanetIndex(iStarIndex);
	m_lstCities.push_back(city);

	CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
	CStar* star = starMgr.GetStar(iStarIndex);
	if (star)
	{
		star->SetCity(city);
		star->SetBelongToCivi(EARTH_CIVI_NAME);
	}

	if (bCompleted)
		city->SetBuildCompleted(bCompleted);

	if (find(m_lstStars.begin(), m_lstStars.end(), iStarIndex) == m_lstStars.end())
	{
		m_lstStars.push_back(iStarIndex);
	}

}

void CEarthCivilization::RunARound()
{
	// 部门
	for (int i = DT_ECONOMY; i < DT_COUNT; i ++)
	{
		m_vecDepartment[i]->RunARound((DEPARTMENT_TYPE)i);
	}

	// 军营
	std::list<CBarback*>::iterator itrBarback;
	for (itrBarback = m_lstBarbacks.begin(); itrBarback != m_lstBarbacks.end(); itrBarback ++)
	{
		(*itrBarback)->RunARound();
	}

	// 矿厂
	int iWorkerCnt = ((CEconomyDepartment*)m_vecDepartment[DT_ECONOMY])->GetMiningWorkerCount();
	std::vector<CStope*> vecStope;
	std::list<CStope*>::iterator itrStope;
	for (itrStope = m_lstStopes.begin(); itrStope != m_lstStopes.end(); itrStope ++)
	{
		if ((*itrStope)->CanRunARound())
			vecStope.push_back(*itrStope);
		else
			(*itrStope)->RunARoundNormal();
	}
	if (vecStope.size() > 0)
	{
		iWorkerCnt = iWorkerCnt / vecStope.size();
		for (int i = 0; i < vecStope.size(); i ++)
		{
			vecStope[i]->RunARound(iWorkerCnt);
		}
	}

	// 加工厂
	iWorkerCnt = ((CEconomyDepartment*)m_vecDepartment[DT_ECONOMY])->GetManufactureWorkerCount();
	std::vector<CFactory*> vecFactory;
	std::list<CFactory*>::iterator itrFactory;
	for (itrFactory = m_lstFactories.begin(); itrFactory != m_lstFactories.end(); itrFactory ++)
	{
		if ((*itrFactory)->CanRunARound())
			vecFactory.push_back(*itrFactory);
		else
			(*itrFactory)->RunARoundNormal();
	}
	if (vecFactory.size() > 0)
	{
		iWorkerCnt = iWorkerCnt / vecFactory.size();
		for (int i = 0; i < vecFactory.size(); i ++)
		{
			vecFactory[i]->RunARound(iWorkerCnt);
		}
	}

	// 城市
	std::list<CCity*>::iterator itrCity;
	for (itrCity = m_lstCities.begin(); itrCity != m_lstCities.end(); itrCity ++)
	{
		(*itrCity)->RunARound();
	}

	// 逃亡主义上升
	CRandomNumber randNum;
	int iAdd = randNum.random(3);
	SetTreachery(m_iTreachery + iAdd);
	
	if (randNum.random(5) == 1)
	{
		CString sInfo;
		sInfo.Format(_T("有一批人在思想钢印的干预下变得颓废，人口降低。"));
		((CLengendOfUniApp*)AfxGetApp())->m_game->AddHistory(sInfo);
	}

	// 面壁者效果：每年为地球提供免费的基础防御力加成或科技点
	if (m_vecWallfacers.size() > 0)
	{
		int iBonus = m_vecWallfacers.size() * 5; 
		m_iArmy += iBonus; // 面壁者提升基础防御
	}

	// 执剑人效果：维持威慑
	if (!m_sSwordholder.IsEmpty())
	{
		CPersonManager& pm = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetPersonManager();
		CPerson* pPerson = pm.GetPerson(m_sSwordholder);
		if (pPerson)
		{
			// 根据威慑度决定是否免遭攻击（简化处理：提高军事评价，让敌人不敢攻击）
			m_iArmy += pPerson->GetLeadership() * 2;
		}
	}
}

BOOL CEarthCivilization::CheckCiviLevelUpgrade()
{
	if (m_iCulture >= 1000)
	{
		if (m_iCiviLevel < 4)
		{
			m_iCiviLevel = 4;
			m_sLevel = _T("霸王");
			m_iArmy += 20;
			return TRUE;
		}
	}
	else if (m_iCulture >= 500)
	{
		if (m_iCiviLevel < 3)
		{
			m_iCiviLevel = 3;
			m_sLevel = _T("逐鹿");
			m_iArmy += 20;
			return TRUE;
		}
	}
	else if (m_iCulture >= 200)
	{
		if (m_iCiviLevel < 2)
		{
			m_iCiviLevel = 2;
			m_sLevel = _T("风暴");
			m_iArmy += 20;
			return TRUE;
		}
	}
	else if (m_iCulture >= 70)
	{
		if (m_iCiviLevel < 1)
		{
			m_iCiviLevel = 1;
			m_sLevel = _T("起源");
			m_iArmy += 20;
			return TRUE;
		}
	}
	else
	{
		if (m_iCiviLevel < 0)
		{
			m_iCiviLevel = 0;
			m_sLevel = _T("荒蛮");
			m_iArmy += 20;
			return TRUE;
		}
	}

	return FALSE;
}

CString CEarthCivilization::GetCiviLevelString()
{
	return m_sLevel;
}

std::list<CStope*> &CEarthCivilization::GetStopeList()
{
	return m_lstStopes;
}

std::list<CFactory*> &CEarthCivilization::GetFactoryList()
{
	return m_lstFactories;
}
std::list<CCity*> &CEarthCivilization::GetCityList()
{
	return m_lstCities;
}

void CEarthCivilization::Serialize(CArchive &ar)
{
	CCivilization::Serialize(ar); 
	int nVersion = ar.IsStoring() ? 2 : 1;

	if(ar.IsStoring()) 
	{
		ar << nVersion;
		ar << m_sLevel << m_iIdlePopulation;
		
		ar << m_vecWallfacers.size();
		for (int i = 0; i < m_vecWallfacers.size(); i++)
		{
			ar << m_vecWallfacers[i];
		}
		ar << m_sSwordholder;

		ar << m_lstStopes.size();
		std::list<CStope*>::iterator itrStope = m_lstStopes.begin();
		while (itrStope != m_lstStopes.end())
		{
			ar << (*itrStope);
			itrStope ++;
		}

		ar << m_lstFactories.size();
		std::list<CFactory*>::iterator itrFac = m_lstFactories.begin();
		while (itrFac != m_lstFactories.end())
		{
			ar << (*itrFac);
			itrFac ++;
		}

		ar << m_lstCities.size();
		std::list<CCity*>::iterator itrCity = m_lstCities.begin();
		while (itrCity != m_lstCities.end())
		{
			ar << (*itrCity);
			itrCity ++;
		}

		ar << m_vecDepartment.size();
		for (int i = 0; i < m_vecDepartment.size(); i ++)
		{
			ar << m_vecDepartment[i];
		}
	}
	else
	{
		ar >> nVersion;
		ar >> m_sLevel >> m_iIdlePopulation;
		
		if (nVersion >= 2)
		{
			int iWallfacerCount = 0;
			ar >> iWallfacerCount;
			for (int i = 0; i < iWallfacerCount; i++)
			{
				CString name;
				ar >> name;
				m_vecWallfacers.push_back(name);
			}
			ar >> m_sSwordholder;
		}

		int iVal = 0;
		ar >> iVal;
		for (int i = 0; i < iVal; i ++)
		{
			CStope *pStope;// = new CStope;
			ar >> pStope;
			m_lstStopes.push_back(pStope);
		}

		ar >> iVal;
		for (int i = 0; i < iVal; i ++)
		{
			CFactory *pFac;// = new CFactory;
			ar >> pFac;
			m_lstFactories.push_back(pFac);
		}

		ar >> iVal;
		for (int i = 0; i < iVal; i ++)
		{
			CCity *pCity;// = new CCity;
			ar >> pCity;
			m_lstCities.push_back(pCity);
		}

		ar >> iVal;
		for (int i = DT_ECONOMY; i < iVal; i ++)
		{
			if (i == DT_ECONOMY)
			{
				CEconomyDepartment *dep;// = new CEconomyDepartment;
				ar >> dep;
				m_vecDepartment.push_back(dep);
			}
			else if (i == DT_ARMY)
			{
				CArmyDepartment *dep;// = new CArmyDepartment;
				ar >> dep;
				m_vecDepartment.push_back(dep);
			}
			else
			{
				CDepartment *dep;// = new CDepartment;
				ar >> dep;
				m_vecDepartment.push_back(dep);
			}

		}
	}

}

BOOL CEarthCivilization::RemoveStope(int iStarIndex)
{
	std::list<CStope*>::iterator itr;
	for (itr = m_lstStopes.begin(); itr != m_lstStopes.end(); itr ++)
	{
		if ((*itr)->GetPlanetIndex() == iStarIndex)
		{
			SAFE_DELETE(*itr)
			itr = m_lstStopes.erase(itr);
			return TRUE;
		}
	}

	return FALSE;
}
BOOL CEarthCivilization::RemoveFactory(int iStarIndex)
{
	std::list<CFactory*>::iterator itr;
	for (itr = m_lstFactories.begin(); itr != m_lstFactories.end(); itr ++)
	{
		if ((*itr)->GetPlanetIndex() == iStarIndex)
		{
			SAFE_DELETE(*itr)
			itr = m_lstFactories.erase(itr);
			return TRUE;
		}
	}

	return FALSE;

}
BOOL CEarthCivilization::RemoveCity(int iStarIndex)
{
	std::list<CCity*>::iterator itr;
	for (itr = m_lstCities.begin(); itr != m_lstCities.end(); itr ++)
	{
		if ((*itr)->GetPlanetIndex() == iStarIndex)
		{
			SAFE_DELETE(*itr)
			itr = m_lstCities.erase(itr);
			return TRUE;
		}
	}

	return FALSE;

}
