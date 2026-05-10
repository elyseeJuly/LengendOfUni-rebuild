#pragma once
#include "architecture.h"
#include "weapon.h"
#include "department.h"
#include <list>

typedef std::list<CWeaponEx*> WeaponExList;

class CBarback :
	public CArchitecture
{
	DECLARE_SERIAL(CBarback)

	int								m_iSoldierCount;
	WeaponExList					m_lstWeaponEx;
	CDepartment						*m_pDepartment;
	BOOL							m_bIsFriend;	// 只对外星人有效，表示该军营所在星球已经通过结盟。
	int								m_iAlignmentYear;	// 只对外星人有效，最后一次谈判的年份
public:
	void Serialize(CArchive &ar);
	void RunARound();
	void AddWeapon(CWeaponEx* weaponEx);
	void Clear(BOOL bOnlyWeapon = FALSE);
	CBarback(void);
	~CBarback(void);
	CDepartment* GetDepartment()	{ return m_pDepartment; };
	int GetSoldierCount()			{ return m_iSoldierCount; };
	void SetSoldierCount(int iVal)	{ m_iSoldierCount = iVal; };
	void SetIsFriend(BOOL bVal)		{ m_bIsFriend = bVal; };
	BOOL GetIsFriend()				{ return m_bIsFriend; }
	void SetAlignmentYear(int iVal)	{ m_iAlignmentYear = iVal; };
	int GetAlignmentYear()			{ return m_iAlignmentYear; };
	WeaponExList& GetWeaponExList()	{ return m_lstWeaponEx; };
};
