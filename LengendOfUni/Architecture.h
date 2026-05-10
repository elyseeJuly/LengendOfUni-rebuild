#pragma once

class CArchitecture : public CObject 
{
	DECLARE_SERIAL(CArchitecture)
protected:
	int		m_iPlanetIndex;
	int		m_iCurrentBuild;	// 建造该建筑的当前工作量
	int		m_iTotalBuild;		// 建造该建筑的工作量
	int		m_iBuildPerRound;	// 每回合造的工作量
public:
	CArchitecture(void);
	~CArchitecture(void);
	virtual void Serialize(CArchive &ar);

	void SetBuildPerRound(int iVal);
	int GetBuildPerRound();
	void SetPlanetIndex(int iVal);
	int GetPlanetIndex();
	void SetCurrentBuild(int iVal);
	int GetCurrentBuild();
	void SetTotalBuild(int iVal);
	int GetTotalBuild();
	void SetBuildCompleted(BOOL bCompleted = TRUE);
	BOOL GetBuildCompleted();
	virtual void RunARound();
};
