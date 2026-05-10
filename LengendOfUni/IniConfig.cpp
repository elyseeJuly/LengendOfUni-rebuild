#include "StdAfx.h"
#include "IniConfig.h"

CIniConfig::CIniConfig(CString sFile)
{
	SetFile(sFile);
}

CIniConfig::CIniConfig(void)
{
}

CIniConfig::~CIniConfig(void)
{
}

void CIniConfig::SetFile(CString sFile)
{
	HMODULE hModule = (HMODULE)AfxGetApp()->m_hInstance;   
	TCHAR lpFn[255];   
	::GetModuleFileName(hModule, lpFn, 255);
	CString sPath;
	sPath = lpFn;
	int iPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(iPos + 1);
	sPath += _T("data\\");
	sPath += sFile;
	m_sIniFile = sPath;
}

int CIniConfig::GetInt(CString sSection, CString sItem)
{
	if (m_sIniFile.IsEmpty())
	{
		ASSERT(0);
		return 0;
	}

	return (BOOL)::GetPrivateProfileInt(sSection, sItem, 0, m_sIniFile); 

}

void CIniConfig::SetInt(CString sSection, CString sItem, int iVal)
{
	if (m_sIniFile.IsEmpty())
	{
		ASSERT(0);
		return ;
	}

	TCHAR szValue[65];
	_itot(iVal, szValue, 65);
	::WritePrivateProfileString(sSection, sItem, szValue, m_sIniFile); 
}

CString CIniConfig::GetString(CString sSection, CString sItem)
{
	if (m_sIniFile.IsEmpty())
	{
		ASSERT(0);
		return _T("");
	}

	TCHAR szValue[255];

	GetPrivateProfileString(sSection, sItem, _T(""), szValue, 255, m_sIniFile); 
	return szValue;
}

void CIniConfig::SetString(CString sSection, CString sItem, CString sVal)
{
	if (m_sIniFile.IsEmpty())
	{
		ASSERT(0);
		return ;
	}

	::WritePrivateProfileString(sSection, sItem, sVal, m_sIniFile); 

}