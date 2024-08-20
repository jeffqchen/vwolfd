//#pragma warning(disable: 4142)
// sysctl.c - system control messages module for VxD VWOLFD
//#define VDBG
#define BUILD_VXD	//for full version of vwolfd.h

#include "sysctl.h"
#include "vwolfd.h"

#pragma VxD_LOCKED_DATA_SEG
// Initialize device global variables
BYTE * const dogHouse		= (BYTE *) CPLD_WATCHDOG;
BYTE * const videoSwitch	= (BYTE *) CPLD_VIDEOSWITCH;

//dogHouse	= _MapPhysToLinear(CPLD_WATCHDOG,	2,	0L);	//Address to feed the watchdog
//videoSwitch	= _MapPhysToLinear(CPLD_VIDEOSWITCH,	2,	0L);//Address to change the video output on JAMMA edge

BOOL	enableWatchdog		= TRUE;
int		videoMode			= 1;	// Video mode
									// 0: Splash
									// 1: Voodoo
									// 2: Test grid

HTIMEOUT dogTimer = 0;

//System boot initilization
BOOL _stdcall vwolfd_device_init()
{
#ifdef VDBG
	Out_Debug_String(">>> vwolfd_device_init()\n");
#endif
	/*
	_asm push 0
	_asm push 0
	_asm push 10
	_asm push 0CBh
	VMMCall( _Assign_Device_V86_Pages )
	*/
	
	// Initial dog feeding and video setting
	* dogHouse		= DOGFOOD;
	
	dogTimer = Set_Global_Time_Out (watchdogThread, DOG_INTERVAL, 0);

	if (0 == dogTimer)
	{
#ifdef VDBG
		Out_Debug_String("Failed to create dog thread\n");
#endif

		return FALSE;
	}

	return TRUE;
}

#pragma VxD_LOCKED_CODE_SEG
// DeviceIOControl
DWORD _stdcall vwolfd_DeviceIOControl(DWORD dwService,
									  DWORD dwDDB,
									  DWORD hDevice,
									  LPDIOC lpDIOCParms)
{
	DWORD dwRetVal = 0;

#ifdef VDBG
	Out_Debug_String(">VxD> vwolfd_DeviceIOControl\n");
#endif

	switch(dwService)
	{
	case DIOC_OPEN:
#ifdef VDBG
		Out_Debug_String("DIOC>> Device open for business!\n");
#endif
		// do nothing
		dwRetVal = 0;
		break;

	case DIOC_CLOSEHANDLE:
#ifdef VDBG
		Out_Debug_String("DIOC>> Device closing down.\n");
#endif
		// do nothing
		dwRetVal = 0;
		break;

	case VWOLFD_Dog_Spawn:
#ifdef VDBG
		Out_Debug_String("DIOC>> VWOLFD_Dog_Spawn.\n");
#endif
		vwolfdDogSpawn();
		dwRetVal = 0;
		break;

	case VWOLFD_Dog_Toggle:
#ifdef VDBG
		Out_Debug_String("DIOC>> VWOLFD_Dog_Toggle.\n");
#endif
		vwolfdDogToggle();
		dwRetVal = 0;
		break;

	case VWOLFD_Dog_Enable:
#ifdef VDBG
		Out_Debug_String("DIOC>> VWOLFD_Dog_Enable.\n");
#endif
		vwolfdDogEnable();
		dwRetVal = 0;
		break;

	case VWOLFD_Dog_Disable:
#ifdef VDBG
		Out_Debug_String("DIOC>> VWOLFD_Dog_Disable.\n");
#endif
		vwolfdDogDisable();
		dwRetVal = 0;
		break;

	case VWOLFD_Video_Splash:
#ifdef VDBG
		Out_Debug_String("DIOC>> VWOLFD_Video_Splash.\n");
#endif
		vwolfdVideoSplash();
		dwRetVal = 0;
		break;

	case VWOLFD_Video_Voodoo:
#ifdef VDBG
		Out_Debug_String("DIOC>> VWOLFD_Video_Voodoo.\n");
#endif
		vwolfdVideoVoodoo();
		dwRetVal = 0;
		break;

	case VWOLFD_Video_Grid:
#ifdef VDBG
		Out_Debug_String("DIOC>> VWOLFD_Video_Grid.\n");
#endif
		vwolfdVideoGrid();
		dwRetVal = 0;
		break;

	case VWOLFD_Get_Version:
#ifdef VDBG
		Out_Debug_String("DIOC>> VWOLFD_Get_Version.\n");
#endif
		vwolfdGetVersion(lpDIOCParms);
		dwRetVal = 0;
		break;

	case VWOLFD_Poll_State:
#ifdef VDBG
		Out_Debug_String("DIOC>> VWOLFD_Pool_State.\n");
#endif
		vwolfdPollState(lpDIOCParms);
		dwRetVal = 0;
		break;

	case VWOLFD_Set_State:
#ifdef VDBG
		Out_Debug_String("DIOC>> VWOLFD_Set_State.\n");
#endif
		vwolfdSetState(lpDIOCParms);
		dwRetVal = 0;
		break;

	default:
		dwRetVal = ERROR_CALL_NOT_IMPLEMENTED;	//Unsupported code, return error
	}

	return dwRetVal;
}

// System shutdown cleanup
BOOL _stdcall vwolfd_sys_exit()
{
//	Out_Debug_String(">>> vwolfd_sys_exit()\n");

	* videoSwitch	= VIDEO_GRID;
	enableWatchdog = FALSE;
	Cancel_Time_Out (dogTimer);

	return TRUE;
}
