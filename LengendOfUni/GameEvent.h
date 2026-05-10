#pragma once
#include <vector>

class CGameEvent
{
	struct STalk
	{
		CString m_sPic;
		CString m_sTalker;
		CString m_sContect;
	};

	EVENT_TYPE				m_type;
	EVENT_EFFECT			m_effect;
	std::vector<STalk>		m_vecTalk;
	CString					m_sName;		// 随机事件可以不要name
	int						m_iEventValue;
public:
	EVENT_TYPE GetType()		{ return m_type; };
	void AddTalk(CString sPic, CString sTalker, CString sContent);
	void Init(EVENT_TYPE etype, EVENT_EFFECT eeffect, CString sName, int iEventValue);
	void Run();
	CGameEvent(void);
	~CGameEvent(void);
};
