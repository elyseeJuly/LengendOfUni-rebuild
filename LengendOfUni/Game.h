#pragma once
#include "EarthCivilization.h"
#include "PersonManager.h"
#include "StarManager.h"
#include "WeaponManager.h"
#include "AlienCiviManager.h"
#include "GameEventManager.h"

class CGame : public CObject
{
	DECLARE_SERIAL(CGame)

private:
	CEarthCivilization*							m_earthCivi;
	CAlienCiviManager*							m_alienCiviManager;
	CPersonManager*								m_personManager;
	CStarManager*								m_starManager;
	CWeaponManager								m_weaponManager;
	CGameEventManager							m_gameEventManager;
	CString										m_sPlayerName;			// 玩家的名字
	int											m_iYear;				// 当前年份
	CString										m_sHistory;
	int											m_iReachStarCount;		// 本局到达的（未发现）星球数

public:
	CGame(void);
	~CGame(void);
	void Serialize(CArchive &ar);
	void Init(void);
	void Clear();
	CAlienCiviManager& GetAlienCiviManager();
	CEarthCivilization& GetEarthCivilization();
	CPersonManager& GetPersonManager();
	CStarManager& GetStarManager();
	CWeaponManager& GetWeaponManager();
	CGameEventManager& GetGameEventManager();
	CAlienCivilization* GetAlienCivi(CString sName);
	void RunARound();				// 一回合结束，作相应的数值处理
	int GetYear();
	void SetPlayerName(CString sPlayerName);
	CString GetPlayerName();
	void AddHistory(CString sHistory);
	CString GetHistory();
	void AddReachStarCount();
	BOOL IsReachStarCountLimit();
	void ResetReachStarCount();
};
