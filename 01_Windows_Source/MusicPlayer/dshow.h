//能播放很多媒体的类,mp3,wav,mid,avi,mpeg,qt,mov,asf,wma,wmv,....
//创建于2003/5/24 by Kylinx
//最后修改于2003/5/24
// modified by zhenghui 2008-11-13
//使用方法举例
//KGDShow*pShow=KGDShow::KGDShowCreate();
//if(!pShow)
//	error();
//pShow->LoadMedia(szMediaFile);//szMediaFile是媒体文件名
//pShow->Play(GetActiveWindow());
//while(pShow->IsPlaying())
// do your things...;
//pShow->Release();
#include<dshow.h>
#include<windows.h>
#include <vector>
#include <string>
using namespace std;

#define String wstring

#define UM_MUSICNOTIFY								WM_USER+0x99
#pragma comment(lib,"strmiids")

class KGDShow
{
public:
	virtual bool LoadMedia(HWND hwnd, const TCHAR* szMediaFile)=0;							//读取一个媒体文件
	virtual bool Pause()=0;												//暂停
	virtual bool Continue()=0;											//从暂停点播放
	virtual bool Reset()=0;												//从新定位到开始点
	virtual bool IsPlaying()=0;											//是否正在播放
	virtual bool StopAndRelease()=0;									//停止并释放
	virtual bool Play(HWND hWnd,BOOL bLoop=false)=0;						//在HWND这个窗口播放
	virtual TCHAR* GetErrorMessage()=0;									//得到错误信息
	virtual bool HandleGraphEvent(HWND hwnd)=0;
	virtual void Release()=0;											//释放
};

class KGDShowInstance : private KGDShow
{
	IGraphBuilder* m_pGraphBuilder;
	IMediaControl* m_pMediaControl;
	IVideoWindow* m_pVideoWindow;
	IMediaEventEx*m_pMediaEvent;
	IMediaSeeking*m_pMediaSeeking;
	TCHAR m_szErrMsg[1024];
	BOOL m_bAudioFile;
	BOOL m_bLoaded;
	BOOL m_bLoop;
	DWORD m_dwState;
	LONGLONG m_llCurrentPos;
	vector<String> m_vecMusicPath;
	int m_iCurMusicIndex;

public:
	BOOL CheckMediaKinds();
	bool InitDirectShow();
	virtual bool LoadMedia(HWND hwnd, const TCHAR* szMediaFile);
	virtual bool Pause();
	virtual bool Continue();
	virtual bool Reset();
	virtual bool IsPlaying();
	virtual bool StopAndRelease();
	virtual bool Play(HWND hWnd,BOOL bLoop=false);
	virtual TCHAR* GetErrorMessage();
	virtual bool HandleGraphEvent(HWND hwnd);
	virtual void Release();

	enum
	{
		MS_NULL=0,
		MS_PLAYING,//MediaState_Playing
		MS_STOP,
		MS_PAUSE
	};
	KGDShowInstance();
	virtual ~KGDShowInstance();
	void AddMusic(String sPath);
	String GetFirstMusicPath();
};