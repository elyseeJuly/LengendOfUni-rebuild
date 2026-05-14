#pragma once
#include "architecture.h"

class CFactory :
	public CArchitecture
{
	DECLARE_SERIAL(CFactory)

public:
	CFactory(void);
	~CFactory(void);
	void RunARound(int iWorkerCnt);
	void RunARoundNormal();
	BOOL CanRunARound();
};
