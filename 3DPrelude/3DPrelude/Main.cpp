#include "game.h"
#include "windows.h"
#include <tchar.h>

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	static CGame game(_T("Áõ´ÈÐÀÈºÐÇ´«"), 800, 600, 30);
	if(game.CreateGame(hInstance,nCmdShow))
	{
		game.MainLoop();
	}
	return 1;
}