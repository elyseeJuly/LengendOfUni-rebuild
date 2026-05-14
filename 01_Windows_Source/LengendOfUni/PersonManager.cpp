#include "StdAfx.h"
#include "PersonManager.h"
#include "RandomNumber.h"
#include "IniConfig.h"

IMPLEMENT_SERIAL(CPersonManager, CObject, 1)

CPersonManager::CPersonManager(void)
{
}

CPersonManager::~CPersonManager(void)
{
	Clear();
}

void CPersonManager::Init()
{
//	CString sPersonNameArray[] = {_T("丁仪"),_T("罗辑"),_T("林云"),_T("杨冬"),_T("叶文洁"),_T("大史"),_T("伊依"),_T("汪淼"),_T("沈渊"),
//		_T("霍金"),_T("常伟思"),_T("庄颜"),_T("章北海"),_T("水娃"),_T("华华"),_T("希恩斯"),_T("雷迪亚兹"),
//		_T("泰勒"),_T("伊文斯"),_T("东方延绪"),_T("雷志成"),_T("杨卫宁"),_T("严井"),_T("白冰"),_T("苗福全"),
//		_T("滑膛"),_T("朱汉扬"),_T("刘慈欣")};

	CIniConfig iniConfig(_T("person.ini"));
	int iCnt;
	iCnt = iniConfig.GetInt(_T("count"), _T("count"));

	for (int i = 0; i < iCnt; i ++)
	{
		CPerson *pPerson = new CPerson;
		CString sSection;
		sSection.Format(_T("%d"), i);
		pPerson->SetName(iniConfig.GetString(sSection ,_T("Name")));
		pPerson->SetArmy(iniConfig.GetInt(sSection ,_T("Army")));
		pPerson->SetEconomy(iniConfig.GetInt(sSection ,_T("Economy")));
		pPerson->SetLeadership(iniConfig.GetInt(sSection ,_T("Leadership")));
		pPerson->SetArt(iniConfig.GetInt(sSection ,_T("Art")));
		pPerson->SetScience(iniConfig.GetInt(sSection ,_T("Science")));
		pPerson->SetTreachery(iniConfig.GetInt(sSection ,_T("Treachery")));
		pPerson->SetSocial(iniConfig.GetInt(sSection ,_T("Social")));

/*		HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
		TCHAR lpFn[255];   
		::GetModuleFileName(hModule, lpFn, 255);
		CString sFace;
		sFace = lpFn;
		int iPos = sFace.ReverseFind('\\');
		sFace = sFace.Left(iPos + 1);
		sFace += _T("images\\");*/
		CString sFace;
		sFace = pPerson->GetName();
		sFace += _T(".bmp");
		pPerson->SetFaceFile(sFace);

		m_mapPersonHide[pPerson->GetName()] = pPerson;
	}
}

CPerson* CPersonManager::GetPerson(CString sName)
{
	if (m_mapPersonFound.find(sName) != m_mapPersonFound.end())
		return m_mapPersonFound[sName];

	if (m_mapPersonHide.find(sName) != m_mapPersonHide.end())
		return m_mapPersonHide[sName];

	return NULL;
}

PersonMap& CPersonManager::GetPersonFoundMap()
{
	return m_mapPersonFound;
}
PersonMap& CPersonManager::GetPersonHideMap()
{
	return m_mapPersonHide;
}

void CPersonManager::Clear()
{
	PersonMap::iterator itr;
	for (itr = m_mapPersonFound.begin(); itr != m_mapPersonFound.end(); itr ++)
	{
		SAFE_DELETE(itr->second)
	}
	m_mapPersonFound.clear();

	for (itr = m_mapPersonHide.begin(); itr != m_mapPersonHide.end(); itr ++)
	{
		SAFE_DELETE(itr->second)
	}
	m_mapPersonHide.clear();

}

CString CPersonManager::FindPerson()
{
	if (m_mapPersonHide.size() == 0)
		return _T("");

	CRandomNumber randNum;
	int iPerson = randNum.random(m_mapPersonHide.size());
	PersonMap::iterator itr;
	for (int i = 0; i <= iPerson; i ++)
	{
		if (i == 0)
			itr = m_mapPersonHide.begin();
		else
			itr ++;
	}
	
	CString sName = itr->first;
	m_mapPersonFound[sName] = itr->second;
	m_mapPersonHide.erase(itr);

	return sName;
}

void CPersonManager::Serialize(CArchive &ar)
{
	CObject::Serialize(ar); 
	if(ar.IsStoring()) 
	{ 
		ar << m_mapPersonFound.size();
		PersonMap::iterator itr;
		for (itr = m_mapPersonFound.begin(); itr != m_mapPersonFound.end(); itr ++)
		{
			ar << itr->second;
		}

		ar << m_mapPersonHide.size();
		for (itr = m_mapPersonHide.begin(); itr != m_mapPersonHide.end(); itr ++)
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
			CPerson *pPerson;// = new CPerson;
			ar >> pPerson;
			m_mapPersonFound[pPerson->GetName()] = pPerson;
		}

		ar >> iVal;
		for (int i = 0; i < iVal; i ++)
		{
			CPerson *pPerson;// = new CPerson;
			ar >> pPerson;
			m_mapPersonHide[pPerson->GetName()] = pPerson;
		}
	}

}

void CPersonManager::AllPeopleFound()
{
	if (m_mapPersonHide.size() == 0)
		return;

	PersonMap::iterator itr;
	for (itr = m_mapPersonHide.begin(); itr != m_mapPersonHide.end(); itr ++)
	{
		m_mapPersonFound[itr->first] = itr->second;
	}
	m_mapPersonHide.clear();
}

BOOL CPersonManager::SetPersonNoDepartment(CDepartment *pDep)
{
	PersonMap::iterator itr;
	for (itr = m_mapPersonFound.begin(); itr != m_mapPersonFound.end(); itr ++)
	{
		if (itr->second->GetDepartment() == pDep)
		{
			itr->second->SetDepartment(NULL);
			return TRUE;
		}
	}

	return FALSE;
}