#pragma once
#include "TecTreeManager.h"
#include "Barback.h"
#include <list>

class CCivilization : public CObject 
{
	DECLARE_SERIAL(CCivilization)
protected:
	int		m_iPopulation;
	int		m_iCulture;
	int		m_iEconomy;
	int		m_iResource;
	int		m_iArmy;			// 战斗力
	int		m_iTreachery;		// 逃亡主义（民众背叛值）
	int		m_iCiviLevel;		// 共5个等级
	CString	m_sName;
	int		m_iPopulationPercent;	// 没啥用了
	int		m_iMiningPercent;	// 没啥用了
	int		m_iEconomyPercent;	// 没啥用了
	int		m_iCulturePercent;	// 没啥用了
	CTecTreeManager *m_tecTreeManager;

	std::list<int>			m_lstStars;
	std::list<CBarback*>	m_lstBarbacks;

public:
	CCivilization(void);
	~CCivilization(void);
	void Serialize(CArchive &ar);


	void SetPopulation(int iVal);
	int GetPopulation();
	void SetCulture(int iVal);
	int GetCulture();
	void SetEconomy(int iVal);
	int GetEconomy();
	void SetResource(int iVal);
	int GetResource();
	void SetArmy(int iVal);
	int GetArmy();
	void SetTreachery(int iVal);
	int GetTreachery();
	void SetCiviLevel(int iVal);
	int GetCiviLevel();
	void SetName(CString sVal);
	CString GetName();
	CTecTreeManager& GetTecTreeManager();

	void BuildBarback(int iStarIndex, BOOL bCompleted = FALSE);
	CBarback* GetBarback(int iStarIndex);
	std::list<CBarback*> &GetBarbackList();
	BOOL RemoveBarback(CBarback* pBarback);
	BOOL RemoveStar(int iStarIndex);
	void Clear();
	BOOL IsDieOut();		// 是否被灭绝（没有星球）

};
