;;------------------------------------
;; sysctl.asm
;;------------------------------------
name VWOLFD

.386p

;;------------------------------------
;; Includes
;;------------------------------------
.xlist
	include vmm.inc
	include debug.inc
.list

;;------------------------------------
;; Equates
;;------------------------------------

DEVICE_Major_Ver	EQU	1
DEVICE_Minor_Ver	EQU	0

;;------------------------------------
;; Virtual Device Declaration
;;------------------------------------

DECLARE_VIRTUAL_DEVICE	VWOLFD, \
						DEVICE_Major_Ver, DEVICE_Minor_Ver, \
						vwolfd_Control_Proc, \
						UNDEFINED_DEVICE_ID, \
						UNDEFINED_INIT_ORDER

VxD_LOCKED_CODE_SEG

BeginProc vwolfd_Control_Proc
	Control_Dispatch	DEVICE_INIT,				vwolfd_device_init		,sCall
	Control_Dispatch	SYSTEM_EXIT,				vwolfd_sys_exit			,sCall
	Control_Dispatch	W32_DEVICEIOCONTROL,		vwolfd_DeviceIOControl	,sCall,	<ecx, ebx, edx, esi>
	clc
	ret
EndProc vwolfd_Control_Proc

VxD_LOCKED_CODE_ENDS

;;------------------------------------
;;Real mode initial dog petting and video mode setting
;;------------------------------------

VxD_REAL_INIT_SEG

BeginProc Wolf_Real_Init_Proc

	push	es

	push	0C000h
	pop		es							; set segment register
	mov		BYTE PTR [es:0B201h], 0FFh	; pet dog
	mov		BYTE PTR [es:0B600h], 02h	; set to grid pattern

	pop		es
	xor     bx, bx                  ; no exclusion table
	xor     si, si                  ; no instance data table
	xor     edx, edx                ; no reference data
	mov		ax, Device_Load_Ok		; real mode init success
	ret

EndProc Wolf_Real_Init_Proc

VxD_REAL_INIT_ENDS


	END