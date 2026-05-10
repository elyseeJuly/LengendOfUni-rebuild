#pragma once
#include "Stope.h"
#include "Barback.h"
#include "Factory.h"
#include "City.h"
#include "Department.h"

class CStar : public CObject
{
	DECLARE_SERIAL(CStar)

	int			m_iIndex;				// 编号
	CString		m_sName;				// 名字
	BOOL		m_bIsPlanet;			// 是行星吗
	int			m_iMigrationLevel;		// 移民级别？该版本暂时不用
	int			m_iTotalResource;		// 总共资源
	int			m_iCurrentResource;		// 当前资源
	BOOL		m_bExist;				// 是否还存在（也可能被毁灭了）
	CString		m_sBelongToCivi;		// 属于哪个文明
	int			m_iPopulationLimit;		// 人口上限
	int			m_iCurrentPopulation;	// 当前人口
	BOOL		m_bFound;				// 是否已发现(已发现显示真名，未发现显示编号)
	CBarback*	m_pBarback;
	CStope*		m_pStope;
	CFactory*	m_pFactory;
	CCity*		m_pCity;
	CDepartment	*m_pDepartment;			// 星球的执政者。该版本暂时不用

public:
	int GetIndex()		{ return m_iIndex; };
	void SetIndex(int iIndex)		{ m_iIndex = iIndex; };
	CString GetName()		{ return m_sName; };
	void SetName(CString sName);
	BOOL GetIsPlanet()	{ return m_bIsPlanet; };
	void SetIsPlanet(BOOL bVal)	{ m_bIsPlanet = bVal; };
	int GetTotalResource()		{ return m_iTotalResource; };
	void SetTotalResource(int iVal)		{ m_iTotalResource = iVal; };
	int GetCurrentResource()		{ return m_iCurrentResource; };
	void SetCurrentResource(int iVal)		{ m_iCurrentResource = iVal; };
	BOOL GetIsExist()	{ return m_bExist; };
	void SetIsExist(BOOL bVal)	{ m_bExist = bVal; };
	CString GetBelongToCivi()	{ return m_sBelongToCivi; };
	void SetBelongToCivi(CString sVal)		{ m_sBelongToCivi = sVal; };
	int GetPopulationLimit()		{ return m_iPopulationLimit; };
	void SetPopulationLimit(int iVal)		{ m_iPopulationLimit = iVal; };
	int GetCurrentPopulation()		{ return m_iCurrentPopulation; };
	void SetCurrentPopulation(int iVal)		{ m_iCurrentPopulation = iVal; };
	BOOL GetIsFound()	{ return m_bFound; };
	void SetIsFound(BOOL bVal)	{ m_bFound = bVal; };
	CBarback* GetBarback()		{ return m_pBarback; };
	void SetBarback(CBarback* pVal)		{ m_pBarback = pVal; };
	CStope* GetStope()		{ return m_pStope; };
	void SetStope(CStope* pVal)		{ m_pStope = pVal; };
	CFactory* GetFactory()		{ return m_pFactory; };
	void SetFactory(CFactory* pVal)		{ m_pFactory = pVal; };
	CCity* GetCity()		{ return m_pCity; };
	void SetCity(CCity* pVal)		{ m_pCity = pVal; };
	CDepartment* GetDepartment()	{ return m_pDepartment; };
	BOOL CanOccupy();

	CStar(void);
	~CStar(void);
	void Serialize(CArchive &ar);
	void Clear();
};
