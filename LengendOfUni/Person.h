#pragma once
#include "department.h"

class CPerson : public CObject
{
	DECLARE_SERIAL(CPerson)

private:
	CString m_sName;
	CString	m_sFaceFile;
	int		m_iTreachery;		// 逃亡主义
	int		m_iScience;			// 科技能力
	int		m_iArt;				// 艺术能力
	int		m_iEconomy;			// 经济能力
	int		m_iArmy;			// 军事能力
	int		m_iLeadership;		// 领导能力
	int		m_iSocial;			// 社会能力

	CDepartment*		m_pDepartment;		//所属部门
public:
	CPerson(void);
	~CPerson(void);
	void Serialize(CArchive &ar);

	void SetName(CString sVal);
	CString GetName();
	void SetTreachery(int iVal);
	int GetTreachery();
	void SetScience(int iVal);
	int GetScience();
	void SetArt(int iVal);
	int GetArt();
	void SetEconomy(int iVal);
	int GetEconomy();
	void SetArmy(int iVal);
	int GetArmy();
	void SetLeadership(int iVal);
	int GetLeadership();
	void SetSocial(int iVal);
	int GetSocial();
	void SetFaceFile(CString sVal);
	CString GetFaceFile();
	void SetDepartment(CDepartment* dep);
	CDepartment* GetDepartment();

	void InitByRandom(void);
};
