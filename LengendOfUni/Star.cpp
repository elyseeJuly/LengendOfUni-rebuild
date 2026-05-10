#include "StdAfx.h"
#include "Star.h"

IMPLEMENT_SERIAL(CStar, CObject, 1)

CStar::CStar(void)
{
	m_iIndex = 0;				// 编号
	m_sName.Empty();				// 名字
	m_bIsPlanet = FALSE;			// 是行星吗
	m_iMigrationLevel = 0;		// 移民级别
	m_iTotalResource = 0;		// 总共资源
	m_iCurrentResource = 0;		// 当前资源
	m_bExist = TRUE;				// 是否还存在（也可能被毁灭了）
	m_sBelongToCivi.Empty();		// 属于哪个文明
	m_iPopulationLimit = 0;		// 人口上限
	m_iCurrentPopulation = 0;
	m_bFound = FALSE;				// 是否已发现(已发现显示真名，未发现显示编号)

	m_pBarback = NULL;
	m_pStope = NULL;
	m_pFactory = NULL;
	m_pCity = NULL;
	m_pDepartment = NULL;//new CDepartment;
}

CStar::~CStar(void)
{
	Clear();
}


void CStar::Clear()
{
	SAFE_DELETE(m_pDepartment)
}

void CStar::SetName(CString sName)
{
	m_sName = sName;

/*	if (!m_pDepartment)
		m_pDepartment = new CDepartment;

	CString sDepName;	
	sDepName = m_sName;
	sDepName += _T("基地");
	m_pDepartment->SetName(sDepName);*/
}

BOOL CStar::CanOccupy()
{
	if (!m_bIsPlanet)	// 非行星
		return FALSE;

	if (m_sBelongToCivi.Compare(EARTH_CIVI_NAME) == 0 && m_pBarback != NULL)
		return TRUE;

	if (!m_sBelongToCivi.IsEmpty())		// 已被占领
		return FALSE;

	return TRUE;
}

void CStar::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	if(ar.IsStoring()) 
	{
		ar << m_iIndex << m_sName << m_bIsPlanet << m_iMigrationLevel << m_iTotalResource
			<< m_iCurrentResource << m_bExist << m_sBelongToCivi << m_iPopulationLimit
			<< m_iCurrentPopulation << m_bFound;
	}
	else
	{
		ar >> m_iIndex >> m_sName >> m_bIsPlanet >> m_iMigrationLevel >> m_iTotalResource
			>> m_iCurrentResource >> m_bExist >> m_sBelongToCivi >> m_iPopulationLimit
			>> m_iCurrentPopulation >> m_bFound;
		// 建筑会在存档加载完毕后，在CGame里统一设置
	}
}
