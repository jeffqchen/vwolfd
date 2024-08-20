//#pragma warning(disable: 4142)
// VWOLFD.c - main services module for VxD VWOLFD
#define BUILD_VXD

#include "vwolfd.h"

#pragma VxD_LOCKED_CODE_SEG
// Watchdog Thread
void _cdecl watchdogThread ()
{
#ifdef VDBG
	Out_Debug_String(">VxD> watchdogThread()\n");
#endif

	if (TRUE == enableWatchdog)
	{
#ifdef VDBG
		Out_Debug_String(">VxDWOLF> Feed dog\n");
#endif

		* dogHouse = DOGFOOD;
	}

	dogTimer = Set_Global_Time_Out (watchdogThread, DOG_INTERVAL, 0);

	if (0 == dogTimer)
	{
#ifdef VDBG
		Out_Debug_String("Failed to renew dog thread\n");
#endif

		return;
	}
	return;
}

#pragma VxD_LOCKED_CODE_SEG
// Pass version number back to caller
void _stdcall vwolfdGetVersion (LPDIOC lpDIOCParms)
{
	lpVerNum	retVerNum;
	PDWORD		BytesReturned;

#ifdef VDBG
	Out_Debug_String("Filling version number\n");
#endif

	retVerNum = (lpVerNum)lpDIOCParms->lpvOutBuffer;
	retVerNum->major = (int) VWOLFD_Major;
	retVerNum->minor = (int) VWOLFD_Minor;

	BytesReturned = (PDWORD)lpDIOCParms->lpcbBytesReturned;
	*BytesReturned = sizeof(VERNUM);

	return;
}

void _stdcall vwolfdPollState (LPDIOC lpDIOCParms)
{
	lpWolfState	retCurrentState;
	PDWORD		BytesReturned;

#ifdef VDBG
	Out_Debug_String("Polling state.\n");
#endif

	retCurrentState = (lpWolfState)lpDIOCParms->lpvOutBuffer;
	retCurrentState->dogState = enableWatchdog;
	retCurrentState->videoMode = (BYTE) *videoSwitch;

	BytesReturned = (PDWORD)lpDIOCParms->lpcbBytesReturned;
	*BytesReturned= sizeof(WOLFSTATE);

	return;
}

#pragma VxD_LOCKED_CODE_SEG
void _stdcall vwolfdSetState (LPDIOC lpDIOCParms )
{
	lpWolfState	retCurrentState;
	PDWORD		BytesReturned;

#ifdef VDBG
	Out_Debug_String("Polling state.\n");
#endif

	retCurrentState = (lpWolfState)lpDIOCParms->lpvInBuffer;
	enableWatchdog = retCurrentState->dogState;
	*videoSwitch = retCurrentState->videoMode;

	BytesReturned = (PDWORD)lpDIOCParms->lpcbBytesReturned;
	*BytesReturned= 0;

	return;
}

#pragma VxD_LOCKED_CODE_SEG
void _stdcall vwolfdDogSpawn ()
{
	enableWatchdog = TRUE;

#ifdef VDBG
	Out_Debug_String(">VxDWOLF> Feed dog\n");
#endif

	* dogHouse = DOGFOOD;

	if (0 == dogTimer)
	{
		dogTimer = Set_Global_Time_Out (watchdogThread, DOG_INTERVAL, 0);
	}

	if (0 == dogTimer)
	{
#ifdef VDBG
		Out_Debug_String("Failed to renew dog thread\n");
#endif
	}

	return;
}

#pragma VxD_LOCKED_CODE_SEG
void _stdcall vwolfdDogEnable ()
{
	enableWatchdog = TRUE;
	return;
}

#pragma VxD_LOCKED_CODE_SEG
void _stdcall vwolfdDogDisable ()
{
	enableWatchdog = FALSE;
	return;
}

#pragma VxD_LOCKED_CODE_SEG
void _stdcall vwolfdDogToggle ()
{
	enableWatchdog = !enableWatchdog;
	return;
}

#pragma VxD_LOCKED_CODE_SEG
void _stdcall vwolfdVideoSplash ()
{
	* videoSwitch = VIDEO_SPLASH;
	return;
}

#pragma VxD_LOCKED_CODE_SEG
void _stdcall vwolfdVideoVoodoo ()
{
	* videoSwitch = VIDEO_MONW_GAME;
	return;
}

#pragma VxD_LOCKED_CODE_SEG
void _stdcall vwolfdVideoGrid ()
{
	* videoSwitch = VIDEO_GRID;
	return;
}
