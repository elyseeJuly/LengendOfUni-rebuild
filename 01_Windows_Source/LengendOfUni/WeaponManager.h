
#pragma once
#include "weapon.h"
#include <map>

typedef std::map<CString, CWeapon> WeaponMap;

class CWeaponManager
{
	WeaponMap		m_mapWeapon;
public:
	WeaponMap &GetWeaponMap();
	CWeapon &GetWeapon(CString sName);
	void Init();
	CWeaponManager(void);
	~CWeaponManager(void);
};
