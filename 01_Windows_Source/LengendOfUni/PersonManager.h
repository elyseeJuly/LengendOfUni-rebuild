#pragma once
#include "person.h"
#include <map>
typedef std::map<CString, CPerson*> PersonMap;

class CPersonManager : public CObject
{
	DECLARE_SERIAL(CPersonManager)

private:
	PersonMap		m_mapPersonFound;	// 已发现的人物列表
	PersonMap		m_mapPersonHide;	// 未发现的人物列表
	
public:
	void AllPeopleFound();
	CString FindPerson();
	PersonMap& GetPersonFoundMap();
	PersonMap& GetPersonHideMap();

	CPerson* GetPerson(CString sName);
	BOOL SetPersonNoDepartment(CDepartment *pDep);
	void Clear();
	void Init();
	CPersonManager(void);
	~CPersonManager(void);
	void Serialize(CArchive &ar);
};
