#pragma once
#include "AlienCivilization.h"
#include <map>


class CAlienCiviManager : public CObject
{
	DECLARE_SERIAL(CAlienCiviManager)

	std::map<CString, CAlienCivilization*>		m_mapAlienCivi;

public:
	BOOL IsAllCiviConquered();
	void RunARound();
	void Clear();
	void Init();
	CAlienCivilization* GetAlienCivi(CString sName);
	CAlienCiviManager(void);
	~CAlienCiviManager(void);
	void Serialize(CArchive &ar);
	std::map<CString, CAlienCivilization*> &GetAlienCiviMap();
	void GetDieOutCivi(CString &sCivis, int &iCnt);
	void GetBundCivi(CString &sCivis, int &iCnt);

};
