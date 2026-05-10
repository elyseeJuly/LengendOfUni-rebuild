#pragma once
#include "civilization.h"

class CAlienCivilization :
	public CCivilization
{
	DECLARE_SERIAL(CAlienCivilization)

protected:
	CString				m_sPic;
	FRIENDSHIP_TYPE		m_friendshipType;
public:
	CAlienCivilization(void);
	~CAlienCivilization(void);
	void Serialize(CArchive &ar);
	void InitByRamdon(void);
	void SetPic(CString sVal);
	CString GetPic();
	void RunARound();				// 一回合结束，作相应的数值处理
	void AddFriendshipDegree();
	void ReduceFriendshipDegree();
	void SetFriendshipType(FRIENDSHIP_TYPE ftype);
	FRIENDSHIP_TYPE GetFriendshipType();
	BOOL IsBund();			// 是否同盟（关系为非常友好）
};
