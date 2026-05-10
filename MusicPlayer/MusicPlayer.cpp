#include "StdAfx.h"
#include "MusicPlayer.h"
#include <tchar.h>

#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }

CMusicPlayer::CMusicPlayer(HWND hWnd)
{
	m_pDShow = new KGDShowInstance;
	m_hWndParent = hWnd;
}

CMusicPlayer::~CMusicPlayer(void)
{
	if (m_pDShow)
		m_pDShow->StopAndRelease();
}

void CMusicPlayer::AddMusic(String sName)
{
	m_pDShow->AddMusic(sName);
}

void CMusicPlayer::SetPlayType(PLAY_TYPE playType)
{
	m_playType = playType;
}

void CMusicPlayer::Play(BOOL bLoop)
{
	m_pDShow->LoadMedia(m_hWndParent, m_pDShow->GetFirstMusicPath().c_str());
	m_pDShow->Play(m_hWndParent, bLoop);
}

BOOL CMusicPlayer::HandleEvent()
{
	return m_pDShow->HandleGraphEvent(m_hWndParent);
}

void CMusicPlayer::Pause()
{
	m_pDShow->Pause();
}