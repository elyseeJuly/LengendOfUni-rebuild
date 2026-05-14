#pragma once

class CIniConfig
{
private:
	CString m_sIniFile;
public:
	void SetString(CString sSection, CString sItem, CString sVal);
	CString GetString(CString sSection, CString sItem);
	void SetInt(CString sSection, CString sItem, int iVal);
	int GetInt(CString sSection, CString sItem);
	void SetFile(CString sFile);
	CIniConfig(CString sFile);
	CIniConfig(void);
	~CIniConfig(void);
};
