#pragma once
#include "civilization.h"
#include "Stope.h"
#include "Factory.h"
#include "City.h"
#include "Department.h"
#include <vector>
#include <list>

class CEarthCivilization :
	public CCivilization
{
	DECLARE_SERIAL(CEarthCivilization)

private:
//	std::list<CString>			m_lstMyPerson;
	std::list<CStope*>			m_lstStopes;
	std::list<CFactory*>		m_lstFactories;
	std::list<CCity*>			m_lstCities;
	std::vector<CDepartment*>	m_vecDepartment;
	int							m_iIdlePopulation;	// 空闲人口
	CString						m_sLevel;

public:
	BOOL RemoveStope(int iStarIndex);
	BOOL RemoveFactory(int iStarIndex);
	BOOL RemoveCity(int iStarIndex);
	std::list<CStope*> &GetStopeList();
	std::list<CFactory*> &GetFactoryList();
	std::list<CCity*> &GetCityList();
	void Serialize(CArchive &ar);
	CString GetCiviLevelString();
	BOOL CheckCiviLevelUpgrade();
	void RunARound();				// 一回合结束，作相应的数值处理
	void BuildStope(int iStarIndex, BOOL bCompleted = FALSE);
	void BuildFactory(int iStarIndex, BOOL bCompleted = FALSE);
	void BuildCity(int iStarIndex, BOOL bCompleted = FALSE);
	int GetIdlePopulation();
	void SetIdlePopulation(int iVal);
	CDepartment* GetDepartment(DEPARTMENT_TYPE dptType);
	void Init();
	void Clear();
	CEarthCivilization(void);
	~CEarthCivilization(void);
};
