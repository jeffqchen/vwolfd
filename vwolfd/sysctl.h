// SYSCTL.h - include file for VxD VWOLFD
#include <basedef.h>
#include <vmm.h>
#include <vwin32.h>

#include <debug.h>
#include <winerror.h>

#define VXD_SUCCESS 1
#define VXD_FAILURE 0

typedef DIOCPARAMETERS *LPDIOC;

//#define TAITO_WOLF_WATCHDOG	0x28C0

// System control message handlers
BOOL _stdcall vwolfd_device_init ();
BOOL _stdcall vwolfd_sys_exit ();
DWORD _stdcall vwolfd_DeviceIOControl (DWORD, DWORD, DWORD, LPDIOC);
