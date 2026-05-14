#pragma once
#include "architecture.h"

class CStope :
	public CArchitecture
{
	DECLARE_SERIAL(CStope)

public:
	CStope(void);
	~CStope(void);
	void RunARound(int iWorkerCnt);
	void RunARoundNormal();
	BOOL CanRunARound();
};
