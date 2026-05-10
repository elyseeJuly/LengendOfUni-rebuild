#include "StdAfx.h"
#include "Person.h"
#include "RandomNumber.h"
#include "EarthCivilization.h"
#include "LengendOfUni.h"

IMPLEMENT_SERIAL(CPerson, CObject, 1)

CPerson::CPerson(void)
{
	m_sName.Empty();
	m_iTreachery = 0;
	m_iScience = 0;
	m_iArt = 0;
	m_iEconomy = 0;
	m_iArmy = 0;
	m_iLeadership = 0;
	m_iSocial = 0;
	m_sFaceFile = _T("");
	m_pDepartment = NULL;
}

CPerson::~CPerson(void)
{
}

void CPerson::SetName(CString sVal)
{
	m_sName = sVal;
}

CString CPerson::GetName()
{
	return m_sName;
}
void CPerson::SetTreachery(int iVal)
{
	m_iTreachery = iVal;
}

int CPerson::GetTreachery()
{
	return m_iTreachery;
}

void CPerson::SetScience(int iVal)
{
	m_iScience = iVal;
}

int CPerson::GetScience()
{
	return m_iScience;
}

void CPerson::SetArt(int iVal)
{
	m_iArt = iVal;
}

int CPerson::GetArt()
{
	return m_iArt;
}

void CPerson::SetEconomy(int iVal)
{
	m_iEconomy = iVal;
}

int CPerson::GetEconomy()
{
	return m_iEconomy;
}

void CPerson::SetArmy(int iVal)
{
	m_iArmy = iVal;
}

int CPerson::GetArmy()
{
	return m_iArmy;
}

void CPerson::SetLeadership(int iVal)
{
	m_iLeadership = iVal;
}

int CPerson::GetLeadership()
{
	return m_iLeadership;
}

void CPerson::SetSocial(int iVal)
{
	m_iSocial = iVal;
}

int CPerson::GetSocial()
{
	return m_iSocial;
}

void CPerson::SetFaceFile(CString sVal)
{
	m_sFaceFile = sVal;
}
CString CPerson::GetFaceFile()
{
	HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
	TCHAR lpFn[255];   
	::GetModuleFileName(hModule, lpFn, 255);
	CString sFace;
	sFace = lpFn;
	int iPos = sFace.ReverseFind('\\');
	sFace = sFace.Left(iPos + 1);
	sFace += _T("images\\");
	sFace += m_sFaceFile;
	return sFace;
}

void CPerson::InitByRandom(void)
{
	// 测试时用
	CRandomNumber randNum;

	m_iTreachery = randNum.random(100);
	m_iScience = randNum.random(100);
	m_iArt = randNum.random(100);
	m_iEconomy = randNum.random(100);
	m_iArmy = randNum.random(100);
	m_iLeadership = randNum.random(100);
	m_iSocial = randNum.random(100);
}

void CPerson::SetDepartment(CDepartment* dep)
{
	m_pDepartment = dep;
}

CDepartment* CPerson::GetDepartment()
{
	return m_pDepartment;
}

void CPerson::Serialize(CArchive &ar)
{
	CObject::Serialize(ar); 
	
	if(ar.IsStoring()) 
	{ 
		ar << m_sName << m_sFaceFile << m_iTreachery << m_iScience << m_iArt << m_iEconomy << m_iArmy << m_iLeadership << m_iSocial;
/*		// 用不着存部门了
		int iVal;
		if (m_pDepartment)
		{
			iVal = 1;
			ar << iVal << m_pDepartment->GetName();
		}
		else
		{
			iVal = 0;
			ar << iVal;
		}*/
	}
	else
	{
		ar >> m_sName >> m_sFaceFile >> m_iTreachery >> m_iScience >> m_iArt >> m_iEconomy >> m_iArmy >> m_iLeadership >> m_iSocial;
/*		// 用不着读部门了，这会在game完全读取后做
		int iVal = 0;
		ar >> iVal;
		if (iVal)
		{
			CString sVal;
			ar >> sVal;
			CEarthCivilization &earthCivi = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetEarthCivilization();
			m_pDepartment = earthCivi.GetDepartment(sVal);
		}*/
	}
}
