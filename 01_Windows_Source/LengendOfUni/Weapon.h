#pragma once
#include "define.h"

class CWeapon
{
private:
	CString			m_sName;
	WEAPON_TYPE		m_type;
	TEC_TREE_TYPE	m_sDependTecType;
	CString			m_sDependTecName;		// 需要学会此技能才能制造、使用该武器
	int				m_iAttack;				// 攻击力
	int				m_iHp;					// hp值
	int				m_iTotalBuild;			// 建造该建筑的工作量
	int				m_iBuildPerRound;		// 每回合造的工作量
	int				m_iCost;				// 制造成本
	int				m_iPriority;			// 优先级
	int				m_iNeedCiviLevel;		// 制造该武器所需的文明等级(仅针对外星人)
public:
	CWeapon(void);
	~CWeapon(void);
	void SetName(CString sName)					{ m_sName = sName; };
	CString GetName()							{ return m_sName; };
	void SetDependTecName(CString sName)		{ m_sDependTecName = sName; };
	CString GetDependTecName()					{ return m_sDependTecName; };
	void SetDependTecType(TEC_TREE_TYPE ttype)	{ m_sDependTecType = ttype; };
	TEC_TREE_TYPE GetDependTecType()			{ return m_sDependTecType; };
	void SetTotalBuild(int iVal)				{ m_iTotalBuild = iVal; };
	int GetTotalBuild()							{ return m_iTotalBuild; };
	void SetBuildPerRound(int iVal)				{ m_iBuildPerRound = iVal; };
	int GetBuildPerRound()						{ return m_iBuildPerRound; };
	void SetCost(int iVal)						{ m_iCost = iVal; };
	int GetCost()								{ return m_iCost; };
	void SetType(WEAPON_TYPE wtype)				{ m_type = wtype; };			
	WEAPON_TYPE GetType()						{ return m_type; };
	int GetHp()									{ return m_iHp; };
	void SetHp(int iVal)						{ m_iHp = iVal; };
	int GetPriority()							{ return m_iPriority; };
	void SetPriority(int iVal)					{ m_iPriority = iVal; };
	int GetAttack()								{ return m_iAttack; };
	void SetAttack(int iVal)					{ m_iAttack = iVal; };
	int GetNeedCiviLevel()						{ return m_iNeedCiviLevel; };
	void SetNeedCiviLevel(int iVal)				{ m_iNeedCiviLevel = iVal; };
};

class CWeaponEx : public CObject
{
	DECLARE_SERIAL(CWeaponEx)

private:
	CString			m_sWeaponName;		// 武器名
	int				m_iCurrentBuild;	// 建造该建筑的当前工作量
public:
	CWeaponEx(void);
	~CWeaponEx(void);
	void Serialize(CArchive &ar);

	void SetFinish(BOOL bFinish = TRUE);
	int GetTotalBuild();
	int GetBuildPerRound();
	BOOL GetIsFinish();
	void RunARound();
	int GetCurrentBuild()						{ return m_iCurrentBuild; };
	void SetWeaponName(CString sName)			{ m_sWeaponName = sName; };
	CString GetWeaponName()						{ return m_sWeaponName; };
};