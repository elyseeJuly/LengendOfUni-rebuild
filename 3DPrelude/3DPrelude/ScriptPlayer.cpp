#include "ScriptPlayer.h"
#include "helper.h"

CScriptPlayer::CScriptPlayer(void)
{
	m_iIndexCounter = 0;
	m_pInfoBoard = NULL;
}

CScriptPlayer::~CScriptPlayer(void)
{
	SAFE_DELETE(m_pInfoBoard)
}

void CScriptPlayer::AddScript(SCRIPT_TYPE scriptType, float arg1, float arg2, float arg3, TCHAR *sArg, BOOL bNext)
{
	// 脚本语法说明：
	// 目前支持5种脚本：
	// 1.ST_SETPOS		设置摄像机位置。arg1, arg2, arg3即位置坐标
	// 2.ST_XMOVE		摄像机沿X轴移动。arg1为移动距离，　arg2为移动的时间
	// 3.ST_ZMOVE		摄像机沿Z轴移动。arg1为移动距离，　arg2为移动的时间
	// 4.ST_YROTATE		摄像机沿自己的Y轴转动。arg1为转动角度，arg2为转动的时间
	// 5.ST_SHOWINFO	显示字幕。sArg为字幕图片路径。
	// 一次可以执行多条脚本，只要它们的m_iIndex相同。
	// 若bNext为true，则标识该条脚本为下一个index。否则仍沿用上一个index

	SScript script;
	script.m_scriptType = scriptType;
	script.m_fArg1 = arg1;
	script.m_fArg2 = arg2;
	script.m_fArg3 = arg3;
	if (sArg)
		_tcsncpy(script.m_sArg, sArg, 255);
	if (bNext)
		m_iIndexCounter ++;
	script.m_iIndex = m_iIndexCounter;
	m_queScript.push(script);
}

BOOL CScriptPlayer::RunScript(LPDIRECT3DDEVICE9 pDev, ZCamera *pCamera)
{
	if (m_lstCurrentScript.empty())
	{
		if (m_queScript.empty())
			return FALSE;

		int iIndex = -1;
		while (1)
		{
			if (m_queScript.empty())
				break;
			SScript script;
			script = m_queScript.front();
			if (iIndex == -1)
				iIndex = script.m_iIndex;
			if (iIndex == script.m_iIndex)
			{
				m_lstCurrentScript.push_back(script);
				m_queScript.pop();
			}
			else
				break;
		}
	}

	static float fLastTime = 0.0f;
	float fTimeDelta = 0.0f;
	float fCurrentTime = ::GetTickCount();
	fTimeDelta = fCurrentTime - fLastTime;
	if (fTimeDelta > 1000.0f)
		fTimeDelta = 0.0f;
	fLastTime = fCurrentTime;

	std::list<SScript>::iterator itr = m_lstCurrentScript.begin();
	while (itr != m_lstCurrentScript.end())
	{
		switch (itr->m_scriptType)
		{
		case ST_SETPOS:
			{
				D3DXVECTOR3 vec(itr->m_fArg1, itr->m_fArg2, itr->m_fArg3);
				pCamera->MoveTo(&vec);
				m_lstCurrentScript.erase(itr ++);
				continue;
			}
			break;
		case ST_XMOVE:
			{
				float fV = itr->m_fArg1 / itr->m_fArg2;
				float fMove = fV * fTimeDelta;
				pCamera->MoveX(fMove);
				itr->m_fArg3 += fMove;
				if (fabs(itr->m_fArg3) >= fabs(itr->m_fArg1))
				{
					m_lstCurrentScript.erase(itr ++);
					continue;
				}
			}
			break;
		case ST_ZMOVE:
			{
				float fV = itr->m_fArg1 / itr->m_fArg2;
				float fMove = fV * fTimeDelta;
				pCamera->MoveZ(fMove);
				itr->m_fArg3 += fMove;
				if (fabs(itr->m_fArg3) >= fabs(itr->m_fArg1))
				{
					m_lstCurrentScript.erase(itr ++);
					continue;
				}
			}
			break;
		case ST_YROTATE:
			{
				if (itr->m_fArg2 <= 0.0001f)	// 或参数2为0，则直接设值(用<=是精度需要)
				{
					pCamera->RotateLocalY(itr->m_fArg1);
					m_lstCurrentScript.erase(itr ++);
					continue;
				}

				float fV = itr->m_fArg1 / itr->m_fArg2;
				float fAngle = fV * fTimeDelta;
				pCamera->RotateLocalY(fAngle);
				itr->m_fArg3 += fAngle;
				if (fabs(itr->m_fArg3) >= fabs(itr->m_fArg1))
				{
					m_lstCurrentScript.erase(itr ++);
					continue;
				}

			}
			break;
		case ST_SHOWINFO:
			{
				if (!m_pInfoBoard)
				{
					m_pInfoBoard = new CInfoBoard;
					if (FAILED(m_pInfoBoard->Create(itr->m_sArg, pDev, itr->m_fArg1, itr->m_fArg2, itr->m_fArg3)))
					{
						m_lstCurrentScript.erase(itr ++);
						continue;
					}
				}
				m_pInfoBoard->Draw(pDev);
				if (m_pInfoBoard->IsDisplayOver())
				{
					SAFE_DELETE(m_pInfoBoard)
					m_lstCurrentScript.erase(itr ++);
					continue;
				}
			}
			break;
		case ST_STARTGAME:
			{
				SendMessage(::GetActiveWindow(), WM_CLOSE, NULL, NULL);
			}
			break;
		}

		itr ++;
	}

	if (m_lstCurrentScript.empty() && m_queScript.empty())
		return FALSE;

	return TRUE;
}