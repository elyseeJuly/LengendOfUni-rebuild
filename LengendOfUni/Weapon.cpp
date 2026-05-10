#include "StdAfx.h"
#include "Weapon.h"
#include "WeaponManager.h"
#include "LengendOfUni.h"

CWeapon::CWeapon(void)
{
	m_iAttack = 0;
	m_iHp = 0;
	m_iTotalBuild = 100;
	m_iBuildPerRound = 40;		// for test
}

CWeapon::~CWeapon(void)
{
}

//----------------------------------

IMPLEMENT_SERIAL(CWeaponEx, CObject, 1)

CWeaponEx::CWeaponEx(void)
{
	m_iCurrentBuild = 0;
}

CWeaponEx::~CWeaponEx(void)
{
}

int CWeaponEx::GetTotalBuild()
{
	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	return weaponMgr.GetWeapon(m_sWeaponName).GetTotalBuild();
}

BOOL CWeaponEx::GetIsFinish()
{
	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	return weaponMgr.GetWeapon(m_sWeaponName).GetTotalBuild() == m_iCurrentBuild;
}

void CWeaponEx::SetFinish(BOOL bFinish)
{
	if (!bFinish)
		m_iCurrentBuild = 0;
	else
	{
		CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
		if (m_sWeaponName.IsEmpty())
		{
			ASSERT(FALSE);
		}
		m_iCurrentBuild = weaponMgr.GetWeapon(m_sWeaponName).GetTotalBuild();
	}
}

int CWeaponEx::GetBuildPerRound()
{
	CWeaponManager& weaponMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager();
	if (m_sWeaponName.IsEmpty())
	{
		ASSERT(FALSE);
	}
	return weaponMgr.GetWeapon(m_sWeaponName).GetBuildPerRound();
}

void CWeaponEx::RunARound()
{
	if (!GetIsFinish())
	{
		// ÔìÎäÆ÷
		int iBuildPerRound = GetBuildPerRound();
		m_iCurrentBuild += iBuildPerRound;
		int iTotalBuild = GetTotalBuild();
		if (m_iCurrentBuild >= iTotalBuild)
			SetFinish();
	}
}

void CWeaponEx::Serialize(CArchive &ar) 
{
	CObject::Serialize(ar); 
	if(ar.IsStoring()) 
	{ 
		ar << m_sWeaponName << m_iCurrentBuild;
	}
	else
	{
		ar >>m_sWeaponName >>m_iCurrentBuild;
	}
}