#pragma once

class CDepartment : public CObject
{
	DECLARE_SERIAL(CDepartment)
protected:
	CString		m_sName;
	CString		m_sLeader;
	int			m_iWorkerCnt;
public:
	CDepartment(void);
	virtual ~CDepartment(void);
	virtual void Serialize(CArchive &ar);
	void SetName(CString sVal);
	CString GetName();
	void SetLeader(CString sVal);
	CString GetLeader();
	void SetWorkerCount(int iVal);
	int GetWorkerCount();
	virtual void RunARound(DEPARTMENT_TYPE depType);
};
