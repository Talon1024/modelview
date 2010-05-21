#include "stdafx.h"
#include "dm_tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void DoAbout()
{
	typedef INT (CALLBACK* LPFNDLLFUNC1)();
	HINSTANCE hLib = LoadLibrary("dm_tools.dll");
	if(hLib)
	{
		LPFNDLLFUNC1 lpDllEntryPoint = GetProcAddress(hLib,_T("DoAbout"));
		(lpDllEntryPoint)();
	}
	FreeLibrary(hLib);
}

int DoDMVC()
{
	typedef INT (CALLBACK* LPFNDLLFUNC1)();
	HINSTANCE hLib = LoadLibrary("dm_tools.dll");
	if(hLib)
	{
		LPFNDLLFUNC1 lpDllEntryPoint = GetProcAddress(hLib,_T("DoDMVC"));
		(lpDllEntryPoint)();
	}
	FreeLibrary(hLib);
	return FALSE;
}

void DoGamesConfig()
{
	typedef INT (CALLBACK* LPFNDLLFUNC1)();
	HINSTANCE hLib = LoadLibrary("dm_tools.dll");
	if(hLib)
	{
		LPFNDLLFUNC1 lpDllEntryPoint = GetProcAddress(hLib,_T("DoConfigGames"));
		(lpDllEntryPoint)();
	}
	FreeLibrary(hLib);
}
