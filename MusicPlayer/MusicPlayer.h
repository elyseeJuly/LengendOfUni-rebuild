#pragma once
#include "dshow.h"
#include <vector>
#include <string>
using namespace std;

enum PLAY_TYPE
{
	PT_SEQUENCE,
	PT_RANDOM,
};

class CMusicPlayer
{
	HWND			m_hWndParent;
	KGDShowInstance	*m_pDShow;					//directShow接口，可播放多种媒体
	PLAY_TYPE		m_playType;
public:
	BOOL HandleEvent();
	void Pause();
	void Play(BOOL bLoop = FALSE);
	void SetPlayType(PLAY_TYPE playType);
	void AddMusic(String sName);
	CMusicPlayer(HWND hWnd);
	~CMusicPlayer(void);
};
