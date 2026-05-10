//--------------------------------------------------------------------------------------------------------
//                        游戏媒体播放模块
//KLDShow.cpp
//游戏引擎中的媒体播放部分
//作者:吴振华(kylinx)
//E-mail:game-diy@163.com
//创建于：2003/5/24 by Kylinx
//--------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include"dshow.h"
#include <stdlib.h>
#include <time.h>
#include <tchar.h>

KGDShowInstance::KGDShowInstance()
{
	m_pGraphBuilder=NULL;
	m_pMediaControl=NULL;
	m_pVideoWindow=NULL;
	m_pMediaSeeking=NULL;
	m_pMediaEvent=NULL;
	m_bLoaded=false;
	m_bLoop=false;
	m_dwState=MS_NULL;
	memset(m_szErrMsg,0,1024);
	m_iCurMusicIndex = 0;
}
KGDShowInstance::~KGDShowInstance()
{
	Release();
}
bool KGDShowInstance::InitDirectShow()
{
	HRESULT hr=::CoInitialize(NULL);
	if(FAILED(hr))
	{
		wsprintf(m_szErrMsg, _T("初始化COM失败!"));
		return false;
	}
	hr=CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC,IID_IGraphBuilder,(void**)&m_pGraphBuilder);
	if(FAILED(hr))
	{
		wsprintf(m_szErrMsg, _T("创建DirectGraph对象失败!"));
		return false;
	}
	
	return true;
}
bool KGDShowInstance::LoadMedia(HWND hwnd, const TCHAR* szMediaFile)
{
	Release();
	if(!InitDirectShow())
		return false;
	
	HRESULT hr;
	
//	WCHAR wstrFile[MAX_PATH];
//    MultiByteToWideChar(CP_ACP,0,szMediaFile,-1,wstrFile,MAX_PATH);
	hr=m_pGraphBuilder->RenderFile(szMediaFile,NULL);
	if(FAILED(hr))
	{
		wsprintf(m_szErrMsg, _T("RenderFile Failed!"));
		return false;
	}
	hr=m_pGraphBuilder->QueryInterface(IID_IMediaControl,(void**)&m_pMediaControl);
	if(FAILED(hr))
	{
		wsprintf(m_szErrMsg, _T("获得DirectGraphMediaControl接口失败!"));
		return false;
	}
	hr=m_pGraphBuilder->QueryInterface(IID_IMediaEventEx,(void**)&m_pMediaEvent);
	if(FAILED(hr))
	{
		wsprintf(m_szErrMsg, _T("获得DirectGraphMediaEventEx接口失败!"));
		return false;
	}
	hr=m_pGraphBuilder->QueryInterface(IID_IVideoWindow,(void**)&m_pVideoWindow);
	if(FAILED(hr))
	{
		wsprintf(m_szErrMsg, _T("获得DirectGraphVideoWindow接口失败!"));
		return false;
	}
	hr=m_pGraphBuilder->QueryInterface(IID_IMediaSeeking,(void**)&m_pMediaSeeking);
	if(FAILED(hr))
	{
		wsprintf(m_szErrMsg, _T("获得DirectGraphMediaSeeking接口失败!"));
		return false;
	}
	if(!CheckMediaKinds())
		return false;
	m_pMediaEvent->SetNotifyWindow((OAHWND)hwnd,UM_MUSICNOTIFY,0);
	m_bLoaded=true;

	return true;
}
bool KGDShowInstance::HandleGraphEvent(HWND hwnd)
{
    LONG evCode, evParam1, evParam2;
	if(!m_pMediaEvent || !m_pMediaSeeking)
		return false;
	while(SUCCEEDED(m_pMediaEvent->GetEvent(&evCode,&evParam1,&evParam2,0)))
    {
        m_pMediaEvent->FreeEventParams(evCode,evParam1,evParam2);
        if(EC_COMPLETE==evCode)
        {
			if(m_bLoop==false)
			{
				int iRand = m_iCurMusicIndex;
				if (m_vecMusicPath.size() > 1)
				{
					int iSeed = 0;
					while (1)
					{
						srand(time(NULL) + iSeed);
						iRand = rand() % m_vecMusicPath.size();
						if (iRand != m_iCurMusicIndex)
						{
							m_iCurMusicIndex = iRand;
							break;
						}
						iSeed += 10;
					}
				}
				LoadMedia(hwnd, (TCHAR*)m_vecMusicPath[m_iCurMusicIndex].c_str());
				Play(hwnd);
				return true;
			}
            LONGLONG pos=0;
			if(FAILED(m_pMediaSeeking->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
												   NULL, AM_SEEKING_NoPositioning)))
			{
				if(!m_pMediaControl)
					return false;
				if(FAILED(m_pMediaControl->Stop()))
                {
                    return false;
                }
                if (FAILED(m_pMediaControl->Run()))
                {
                    return false;
                }
            }
        }
    }
	return true;
}
BOOL KGDShowInstance::CheckMediaKinds()
{
	if(NULL==m_pVideoWindow)
	{
		m_bAudioFile=true;
		return true;
	}
	long visible;
	HRESULT hr=m_pVideoWindow->get_Visible(&visible);
	m_bAudioFile=false;
	if(FAILED(hr))
	{
		if(E_NOINTERFACE==hr)
		{
			m_bAudioFile=true;
			return true;
		}
		else
		{
			wsprintf(m_szErrMsg, _T("Get Visible State Failed!"));
			return false;
		}
	}
	return true;
}
TCHAR* KGDShowInstance::GetErrorMessage()
{
	return m_szErrMsg;
}
bool KGDShowInstance::Play(HWND hWnd,BOOL bLoop)
{
	if(!m_bLoaded)
	{
		wsprintf(m_szErrMsg, _T("Media haven't loaded!"));
		return false;
	}
	HRESULT hr;

	if(!m_bAudioFile)
	{
		m_pVideoWindow->HideCursor(OATRUE);
		hr=m_pVideoWindow->put_Owner((OAHWND)hWnd);
		if(FAILED(hr))
		{
			wsprintf(m_szErrMsg, _T("Put Owner Failed!"));
			return false;
		}
		RECT rect;
		::GetClientRect(hWnd,&rect);
		 
		hr=m_pVideoWindow->SetWindowPosition(rect.left,
											 rect.top,
											 rect.right,
											 rect.bottom);
 	 	m_pVideoWindow->put_WindowStyle(WS_CHILD);
		hr=m_pVideoWindow->put_Visible(OATRUE);
		if (FAILED(hr))
		{
			//MessageBox(hWnd,"set video visible failed.",NULL,MB_OK);
			return false;
		}
	}
	LONGLONG llPos=0;
	m_pMediaSeeking->SetPositions(&llPos,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);

	hr=m_pMediaControl->Run();
	if(FAILED(hr))
	{
		wsprintf(m_szErrMsg, _T("Run Failed!"));
		return false;
	}

	m_dwState=MS_PLAYING;
	m_bLoop=bLoop;
	return true;
}
bool KGDShowInstance::Pause()
{
	if(!m_pMediaControl)
	{
		wsprintf(m_szErrMsg, _T("No Media!"));
		return false;
	}
	if(m_dwState!=MS_PLAYING)
		return false;
	if(FAILED(m_pMediaControl->Pause()))
		return false;
	if(m_pMediaSeeking)
		m_pMediaSeeking->GetCurrentPosition(&m_llCurrentPos);
	m_dwState=MS_PAUSE;
	return true;
}
bool KGDShowInstance::StopAndRelease()
{
	if(!m_pMediaControl)
	{
		wsprintf(m_szErrMsg, _T("No Media!"));
		return false;
	}
	if(FAILED(m_pMediaControl->Stop()))
		return false;
	LONGLONG llPos=0;
	if(m_pMediaSeeking)
		m_pMediaSeeking->SetPositions(&llPos,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
	Release();
	return true;
}
bool KGDShowInstance::Reset()
{
	if(!m_pMediaControl)
	{
		wsprintf(m_szErrMsg, _T("No Media!"));
		return false;
	}
	LONGLONG llPos=0;
	if(m_pMediaSeeking)
		m_pMediaSeeking->SetPositions(&llPos,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
	m_pMediaControl->Pause();
	m_dwState=MS_STOP;
	return true;
}
bool KGDShowInstance::IsPlaying()
{
	if(m_dwState==MS_PLAYING)
		return true;
	return false;
}
bool KGDShowInstance::Continue()
{
	if(m_pMediaControl)
	{
		if(m_dwState==MS_PAUSE)
		{
			if(m_pMediaSeeking)
				m_pMediaSeeking->SetPositions(&m_llCurrentPos,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
			m_pMediaControl->Run();
			m_dwState=MS_PLAYING;
			return true;
		}
	}
	wsprintf(m_szErrMsg, _T("m_pMediaControl==null"));
	return false;
}
void KGDShowInstance::Release()
{
	if(m_pMediaControl)
		m_pMediaControl->Stop();
	if(m_pVideoWindow)
	{
		m_pVideoWindow->put_Visible(OAFALSE);
		m_pVideoWindow->put_Owner(NULL);
	}
	if(m_pMediaControl)
	{
		m_pMediaControl->Release();
		m_pMediaControl=NULL;
	}
	if(m_pMediaSeeking)
	{
		m_pMediaSeeking->Release();
		m_pMediaSeeking=NULL;
	}
	if(m_pMediaEvent)
	{
		m_pMediaEvent->Release();
		m_pMediaEvent=NULL;
	}
	if(m_pVideoWindow)
	{
		m_pVideoWindow->Release();
		m_pVideoWindow=NULL;
	}
	if(m_pGraphBuilder)
	{
		m_pGraphBuilder->Release();
		m_pGraphBuilder=NULL;
	}
	m_bLoaded=false;
	m_bLoop=false;
	m_dwState=MS_NULL;
	::CoUninitialize();
}

void KGDShowInstance::AddMusic(String sPath)
{
	m_vecMusicPath.push_back(sPath);
}

String KGDShowInstance::GetFirstMusicPath()
{
	if (m_vecMusicPath.empty())
		return _T("");

	return m_vecMusicPath[0];
}