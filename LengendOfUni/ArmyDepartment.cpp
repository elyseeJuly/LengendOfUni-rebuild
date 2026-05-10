#include "StdAfx.h"
#include "ArmyDepartment.h"

IMPLEMENT_SERIAL(CArmyDepartment, CDepartment, 1)

CArmyDepartment::CArmyDepartment(void)
{
	// for test
/*	CWeaponEx *weaponEx = new CWeaponEx;
	weaponEx->SetWeaponName(_T("ÆæµãÕ¨µ¯"));
	weaponEx->SetFinish();
	AddBomb(weaponEx);*/

}

CArmyDepartment::~CArmyDepartment(void)
{
	Clear();
}

void CArmyDepartment::Clear()
{
	WeaponExList::iterator itr = m_lstBomb.begin();
	while (itr != m_lstBomb.end())
	{
		SAFE_DELETE(*itr)
		itr ++;
	}
	m_lstBomb.clear();

	itr = m_lstZhizi.begin();
	while (itr != m_lstZhizi.end())
	{
		SAFE_DELETE(*itr)
		itr ++;
	}
	m_lstZhizi.clear();

}


int CArmyDepartment::GetBombCount()
{
	int iCnt = m_lstBomb.size();
	WeaponExList::iterator itr = m_lstBomb.begin();
	if (iCnt > 0)
	{
		if (!(*itr)->GetIsFinish())
			iCnt --;
	}
	return iCnt;
}

int CArmyDepartment::GetZhiziCount()
{
	int iCnt = m_lstZhizi.size();
	WeaponExList::iterator itr = m_lstZhizi.begin();
	if (iCnt > 0)
	{
		if (!(*itr)->GetIsFinish())
			iCnt --;
	}
	return iCnt;
}

void CArmyDepartment::AddBomb(CWeaponEx* weaponEx)
{
	if (weaponEx)
		m_lstBomb.push_front(weaponEx);
}

void CArmyDepartment::AddZhizi(CWeaponEx* weaponEx)
{
	if (weaponEx)
		m_lstZhizi.push_front(weaponEx);
}

BOOL CArmyDepartment::GetFirstBombState(BOOL &bFinish, int &iCurProgerss, int &iTotalProgress)
{
	if (m_lstBomb.size() == 0)
		return FALSE;
	WeaponExList::iterator itr = m_lstBomb.begin();
	bFinish = (*itr)->GetIsFinish();
	iCurProgerss = (*itr)->GetCurrentBuild();
	iTotalProgress = (*itr)->GetTotalBuild();
	return TRUE;
}

BOOL CArmyDepartment::GetFirstZhiziState(BOOL &bFinish, int &iCurProgerss, int &iTotalProgress)
{
	if (m_lstZhizi.size() == 0)
		return FALSE;
	WeaponExList::iterator itr = m_lstZhizi.begin();
	bFinish = (*itr)->GetIsFinish();
	iCurProgerss = (*itr)->GetCurrentBuild();
	iTotalProgress = (*itr)->GetTotalBuild();
	return TRUE;
}

void CArmyDepartment::RunARound(DEPARTMENT_TYPE depType)
{
	if (depType != DT_ARMY)
		return;

	WeaponExList::iterator itr;
	for (itr = m_lstBomb.begin(); itr != m_lstBomb.end(); itr ++)
	{
		// ÔìÎäÆ÷
		(*itr)->RunARound();
	}
	for (itr = m_lstZhizi.begin(); itr != m_lstZhizi.end(); itr ++)
	{
		// ÔìÎäÆ÷
		(*itr)->RunARound();
	}

}

void CArmyDepartment::RemoveOneBomb()
{
	if (m_lstBomb.size() == 0)
		return;

	WeaponExList::iterator itr;
	for (itr = m_lstBomb.begin(); itr != m_lstBomb.end(); itr ++)
	{
		if ((*itr)->GetIsFinish())
		{
			SAFE_DELETE(*itr)
			m_lstBomb.erase(itr);
			break;
		}
	}
}

void CArmyDepartment::RemoveOneZhizi()
{
	if (m_lstZhizi.size() == 0)
		return;

	WeaponExList::iterator itr;
	for (itr = m_lstZhizi.begin(); itr != m_lstZhizi.end(); itr ++)
	{
		if ((*itr)->GetIsFinish())
		{
			SAFE_DELETE(*itr)
			m_lstZhizi.erase(itr);
			break;
		}
	}
}


void CArmyDepartment::Serialize(CArchive &ar)
{
	CDepartment::Serialize(ar);
/*	if(ar.IsStoring()) 
	{ 
		ar << m_lstBomb.size() << m_lstZhizi.size();
	}
	else
	{
		Clear();
		int iBombCnt = 0, iZhiziCnt = 0;
		ar >> iBombCnt >> iZhiziCnt;
		for (int i = 0; i < iBombCnt; i ++)
		{
			CWeaponEx *weaponEx = new CWeaponEx;
			weaponEx->SetWeaponName(_T("ÆæµãÕ¨µ¯"));
			AddBomb(weaponEx);
		}
		for (int i = 0; i < iZhiziCnt; i ++)
		{
			CWeaponEx *weaponEx = new CWeaponEx;
			weaponEx->SetWeaponName(_T("ÖÇ×Ó"));
			AddZhizi(weaponEx);
		}
	}*/

	if(ar.IsStoring()) 
	{ 
		ar << m_lstBomb.size();
		WeaponExList::iterator itr;
		for (itr = m_lstBomb.begin(); itr != m_lstBomb.end(); itr ++)
		{
			ar << (*itr);
		}

		ar << m_lstZhizi.size();
		for (itr = m_lstZhizi.begin(); itr != m_lstZhizi.end(); itr ++)
		{
			ar << (*itr);
		}
	}
	else
	{
		int iVal = 0;
		ar >> iVal;
		for (int i = 0; i < iVal; i ++)
		{
			CWeaponEx* weaponEx = NULL;
			ar >> weaponEx;
			m_lstBomb.push_back(weaponEx);
		}
		ar >> iVal;
		for (int i = 0; i < iVal; i ++)
		{
			CWeaponEx* weaponEx = NULL;
			ar >> weaponEx;
			m_lstZhizi.push_back(weaponEx);
		}
	}
}