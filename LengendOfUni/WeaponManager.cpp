#include "StdAfx.h"
#include "WeaponManager.h"
#include "IniConfig.h"

CWeaponManager::CWeaponManager(void)
{
}

CWeaponManager::~CWeaponManager(void)
{
}

void CWeaponManager::Init()
{
//	CString sWeaponNameArray[] = {_T("小型战舰"),_T("中型战舰"),_T("巨型战舰"),_T("宏化部队"),_T("球状闪电"),_T("小行星级氢弹"),
//		_T("行星级氢弹"),_T("恒星级氢弹"),_T("智子"),_T("奇点炸弹")};
//	WEAPON_TYPE typeArray[] = {WT_UNIT, WT_UNIT, WT_UNIT, WT_UNIT, WE_EXPENDABLE, WE_EXPENDABLE, WE_EXPENDABLE, WE_EXPENDABLE, WE_SPY, WE_SUPERBOMB};

	CIniConfig iniConfig(_T("weapon.ini"));
	int iCnt;
	iCnt = iniConfig.GetInt(_T("count"), _T("count"));

	for (int i = 0; i < iCnt; i ++)
	{
		CString sSection;
		sSection.Format(_T("%d"), i);

		CWeapon weapon;
		weapon.SetName(iniConfig.GetString(sSection ,_T("Name")));
		weapon.SetTotalBuild(iniConfig.GetInt(sSection ,_T("BuildWork")));
		weapon.SetDependTecType((TEC_TREE_TYPE)iniConfig.GetInt(sSection ,_T("DependTecType")));
		weapon.SetDependTecName(iniConfig.GetString(sSection ,_T("DependTecName")));
		weapon.SetType((WEAPON_TYPE)iniConfig.GetInt(sSection ,_T("Type")));
		weapon.SetHp(iniConfig.GetInt(sSection ,_T("Hp")));
		weapon.SetAttack(iniConfig.GetInt(sSection ,_T("Attack")));
		weapon.SetPriority(iniConfig.GetInt(sSection ,_T("Priority")));
		weapon.SetBuildPerRound(iniConfig.GetInt(sSection ,_T("BuildPerRound")));
		weapon.SetCost(iniConfig.GetInt(sSection ,_T("Cost")));
		weapon.SetNeedCiviLevel(iniConfig.GetInt(sSection ,_T("NeedCiviLevel")));
		m_mapWeapon[weapon.GetName()] = weapon;
	}
}

CWeapon &CWeaponManager::GetWeapon(CString sName)
{
	return m_mapWeapon[sName];
}

WeaponMap &CWeaponManager::GetWeaponMap()
{
	return m_mapWeapon;
}

