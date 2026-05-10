#pragma once
#include "department.h"

class CEconomyDepartment :
	public CDepartment
{
	DECLARE_SERIAL(CEconomyDepartment)

private:
	int		m_iMiningWorkerCnt;
	int		m_iManufactureWorkerCnt;
public:
	CEconomyDepartment(void);
	~CEconomyDepartment(void);
	void Serialize(CArchive &ar);

	void SetMiningWorkerCount(int iVal);
	int GetMiningWorkerCount();
	void SetManufactureWorkerCount(int iVal);
	int GetManufactureWorkerCount();
	void RunARound(DEPARTMENT_TYPE depType);
};
