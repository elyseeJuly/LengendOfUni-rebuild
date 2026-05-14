#pragma once
#include "star.h"
#include <vector>

class CStarManager : public CObject
{
	DECLARE_SERIAL(CStarManager)

	typedef std::vector<CStar*> StarVector;			// 区域内所有星球
	typedef std::vector<int> StarIndexVector;		// 区域内所有星球的编号
	typedef std::vector<StarIndexVector> StarMapVector;		// 银河尺度内的所有区域的星球
private:
	StarVector			m_starVector;
	StarMapVector		m_starMapVector;	// 在该版本里没啥用
public:
	CStarManager(void);
	~CStarManager(void);
	void Serialize(CArchive &ar);
	void Init();
	CStar* GetStar(int iIndex);
	CStar* GetStar(CString sName);
	void Clear();
	void RemoveStar(int iStarIndex);
};
