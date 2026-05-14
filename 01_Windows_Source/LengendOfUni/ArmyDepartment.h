#pragma once
#include "department.h"
#include "weapon.h"
#include <list>


class CArmyDepartment :
	public CDepartment
{
	DECLARE_SERIAL(CArmyDepartment)

	typedef std::list<CWeaponEx*> WeaponExList;

	WeaponExList					m_lstBomb;
	WeaponExList					m_lstZhizi;

public:
	void RemoveOneBomb();
	void RemoveOneZhizi();
	void RunARound(DEPARTMENT_TYPE depType);
	BOOL GetFirstBombState(BOOL &bFinish, int &iCurProgerss, int &iTotalProgress);
	BOOL GetFirstZhiziState(BOOL &bFinish, int &iCurProgerss, int &iTotalProgress);
	void AddBomb(CWeaponEx* weaponEx);
	void AddZhizi(CWeaponEx* weaponEx);
	int GetBombCount();
	int GetZhiziCount();
	void Clear();
	CArmyDepartment(void);
	~CArmyDepartment(void);
	void Serialize(CArchive &ar);
};
