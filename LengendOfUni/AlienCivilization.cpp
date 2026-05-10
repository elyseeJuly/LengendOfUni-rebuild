#include "StdAfx.h"
#include "AlienCivilization.h"
#include "RandomNumber.h"
#include "StarManager.h"
#include "LengendOfUni.h"
#include "Helper.h"
#include "WeaponManager.h"

IMPLEMENT_SERIAL(CAlienCivilization, CCivilization, 1)

CAlienCivilization::CAlienCivilization(void)
{
	m_friendshipType = FT_NORMAL;
}

CAlienCivilization::~CAlienCivilization(void)
{
}

void CAlienCivilization::InitByRamdon(void)
{
	// 仅供测试时用
	CRandomNumber randNum;
	m_iPopulation = randNum.random(100);
	m_iCulture = randNum.random(100);
	m_iEconomy = randNum.random(100);
	m_iResource = randNum.random(100);
	m_iArmy = randNum.random(100);
	m_iCiviLevel = 2;
	m_iPopulationPercent = randNum.random(100);
	m_iMiningPercent = randNum.random(100);
	m_iEconomyPercent = randNum.random(100);
	m_iCulturePercent = randNum.random(100);
	m_tecTreeManager->Init();
}

void CAlienCivilization::SetPic(CString sVal)
{
	m_sPic = sVal;
}

CString CAlienCivilization::GetPic()
{
	HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
	TCHAR lpFn[255];   
	::GetModuleFileName(hModule, lpFn, 255);
	CString sPic;
	sPic = lpFn;
	int iPos = sPic.ReverseFind('\\');
	sPic = sPic.Left(iPos + 1);
	sPic += _T("images\\");
	sPic += m_sPic;
	return sPic;
}

void CAlienCivilization::RunARound()
{
	if (IsDieOut())
		return;		// 该文明已经被灭绝

	CRandomNumber randNum;
	if (randNum.random(7) == 5)
	{
		// 扩张
		int iStarIndex = randNum.random(200) + 9;
		CStarManager &starMgr = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetStarManager();
		CStar* pStar = starMgr.GetStar(iStarIndex);
		if (!pStar)
			return;

		if (pStar->CanOccupy())
		{
			if (pStar->GetBelongToCivi().Compare(EARTH_CIVI_NAME) == 0)
			{
				// 入侵地球人占据的行星
				if (m_friendshipType < FT_VERYFRIEND)
				{
					int iAlienBarback = randNum.random(m_lstBarbacks.size());
					std::list<CBarback*>::iterator itr = m_lstBarbacks.begin();
					for (int i = 0; i < iAlienBarback; i ++)
					{
						itr ++;
					}
					if (itr != m_lstBarbacks.end())
					{
						if (!(*itr)->GetIsFriend())
						{
							CString sInfo;
							sInfo.Format(_T("目标：占领一切！！！%s 将是我们的领土!"), pStar->GetName());
							CHelper::CiviMsgBox(this, sInfo);
							CHelper::StartBattle(iStarIndex, (*itr)->GetPlanetIndex(), BT_DEFEND);
						}
					}
				}
			}
			else
			{
				// 占领无人星球
				pStar->SetBelongToCivi(m_sName);
				m_lstStars.push_back(iStarIndex);
				BuildBarback(iStarIndex, TRUE);
			}
		}
	}

	std::list<CBarback*>::iterator itr;
	for (itr = m_lstBarbacks.begin(); itr != m_lstBarbacks.end(); itr ++)
	{
		int iSoldierCnt = (*itr)->GetSoldierCount() + 2;
		m_iPopulation += 2;
		if (randNum.random(10) == 5)	// 1/10的机会造兵
		{
			iSoldierCnt += 30;
			m_iPopulation += 30;
		}
		(*itr)->SetSoldierCount(iSoldierCnt);

		if (randNum.random(10) == 5)	// 1/10的机会造武器
		{
			WeaponMap &weaponMap = ((CLengendOfUniApp*)AfxGetApp())->m_game->GetWeaponManager().GetWeaponMap();
			std::vector<CString> vecWeapon;
			WeaponMap::iterator itrWeapon;
			for (itrWeapon = weaponMap.begin(); itrWeapon != weaponMap.end(); itrWeapon ++)
			{
				if (m_iCiviLevel >= itrWeapon->second.GetNeedCiviLevel())
					vecWeapon.push_back(itrWeapon->first);
			}

			int iSelWeapon = randNum.random(vecWeapon.size());
			CWeaponEx *weaponEx = new CWeaponEx;
			weaponEx->SetWeaponName(vecWeapon[iSelWeapon]);
			weaponEx->SetFinish();
			(*itr)->AddWeapon(weaponEx);

		}

	}
}

void CAlienCivilization::AddFriendshipDegree()
{
	if (m_friendshipType < FT_VERYFRIEND)
		m_friendshipType = (FRIENDSHIP_TYPE)(m_friendshipType + 1);
}

void CAlienCivilization::ReduceFriendshipDegree()
{
	if (m_friendshipType > FT_VERYANGRY)
		m_friendshipType = (FRIENDSHIP_TYPE)(m_friendshipType - 1);
}

void CAlienCivilization::SetFriendshipType(FRIENDSHIP_TYPE ftype)
{
	m_friendshipType = ftype;
}

FRIENDSHIP_TYPE CAlienCivilization::GetFriendshipType()
{
	return m_friendshipType;
}

BOOL CAlienCivilization::IsBund()
{
	return m_friendshipType == FT_VERYFRIEND;
}

void CAlienCivilization::Serialize(CArchive &ar)
{
	CCivilization::Serialize(ar); 
	if(ar.IsStoring()) 
	{ 
		int iVal = (int)m_friendshipType;
		ar << m_sPic << iVal;
	}
	else
	{
//		Clear();
		int iVal = 0;
		ar >> m_sPic >> iVal;
		m_friendshipType = (FRIENDSHIP_TYPE)iVal;
	}

}