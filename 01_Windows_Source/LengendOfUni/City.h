#pragma once
#include "architecture.h"

class CCity :
	public CArchitecture
{
	DECLARE_SERIAL(CCity)

	int		m_iFindPersonCnt;
	int		m_iCurYear;
public:
	void Serialize(CArchive &ar);
	BOOL FindPerson(int iCurYear);
	CCity(void);
	~CCity(void);
	void RunARound();
};
