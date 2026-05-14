#include "StdAfx.h"
#include "Barback.h"

IMPLEMENT_SERIAL(CBarback, CArchitecture, 1)

CBarback::CBarback(void)
{
	m_iTotalBuild = 100;
	m_iSoldierCount = 0;
	m_iAlignmentYear = 0;
	m_pDepartment = new CDepartment;
	m_bIsFriend = FALSE;
}

CBarback::~CBarback(void)
{
	Clear();
}

void CBarback::Clear(BOOL bOnlyWeapon)
{
	if (!bOnlyWeapon)
		SAFE_DELETE(m_pDepartment)
	WeaponExList::iterator itr = m_lstWeaponEx.begin();
	while (itr != m_lstWeaponEx.end())
	{
		SAFE_DELETE(*itr)
		m_lstWeaponEx.erase(itr ++);
	}
	m_iSoldierCount = 0;
}

void CBarback::AddWeapon(CWeaponEx* weaponEx)
{
	if (!weaponEx)
		return;

	m_lstWeaponEx.push_back(weaponEx);
}

void CBarback::RunARound()
{
	CArchitecture::RunARound();

	WeaponExList::iterator itr;
	for (itr = m_lstWeaponEx.begin(); itr != m_lstWeaponEx.end(); itr ++)
	{
		// ÔìÎäÆ÷
		(*itr)->RunARound();
	}
}

void CBarback::Serialize(CArchive &ar)
{
	CArchitecture::Serialize(ar);
	if(ar.IsStoring()) 
	{ 
		ar << m_iSoldierCount << m_bIsFriend << m_iAlignmentYear;
		ar << m_lstWeaponEx.size();
		WeaponExList::iterator itr = m_lstWeaponEx.begin();
		while (itr != m_lstWeaponEx.end())
		{
			ar << (*itr);
			itr ++;
		}
		ar << m_pDepartment;
	}
	else
	{
		ar >>m_iSoldierCount >>m_bIsFriend >>m_iAlignmentYear;
		int iVal = 0;
		ar >> iVal;
		for (int i = 0; i < iVal; i ++)
		{
			CWeaponEx* pWeaponEx;	// = new CWeaponEx;
			ar >> pWeaponEx;
			m_lstWeaponEx.push_back(pWeaponEx);
		}
		SAFE_DELETE(m_pDepartment)
		ar >> m_pDepartment;
	}

}