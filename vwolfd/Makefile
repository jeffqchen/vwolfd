BUILD_TYPE=base
!INCLUDE $(BASEDIR)\inc\master.mk

LOCAL_CLEAN=1

NAME = VWOLFD

#       Definitions for the debug level 

DBG			= 0
DEVL		= 0
WOLFDEBUG	=-DDEBLEVEL=0

!if "$(DDKBUILDENV)"=="checked"
DBG			= 1
DEVL		= 1
WOLFDEBUG	=-DDEBLEVEL=1 -DDEBUG
!endif

#       Definitions for MASM 6 Assembler

ASM			= ml
AFLAGS		= -coff -DBLD_COFF -DIS_32 -W2 -c -Cx -Zm -DMASM6 -I$(INC32) $(WOLFDEBUG)
ASMENV		= ML
CVXDFLAGS	= -Zdp -Gs -c -DIS_32 -Zl $(WOLFDEBUG) -I$(INC32)
LFLAGS		= /VXD /NOD

#       MASM 6 only inference rules

all: obj\i386\$(NAME).VXD

obj\i386\sysctlasm.obj: sysctl.asm
	ml $(AFLAGS) -Fo$@ %s

obj\i386\sysctl.obj: sysctl.c
	cl $(CVXDFLAGS) -Fo$@ %s

obj\i386\vwolfd.obj: vwolfd.c
	cl $(CVXDFLAGS) -Fo$@ %s

OBJS = obj\i386\sysctlasm.obj obj\i386\sysctl.obj obj\i386\vwolfd.obj

obj\i386\$(NAME).vxd: $(OBJS) $(NAME).def
!ifndef PASS0ONLY
		@echo link -OUT:$@
        link @<<obj\i386\$(NAME).lnk
$(LFLAGS) 
-ignore:4078 -ignore:4039
/OUT:obj\i386\$(NAME).vxd
/MAP:obj\i386\$(NAME).map
/DEF:$(NAME).def
$(OBJS)
<<
        mapsym -s -o obj\i386\$(NAME).sym obj\i386\$(NAME).map
!endif

clean:
    -@if exist obj\i386\*.obj del obj\i386\*.obj
    -@if exist obj\i386\*.vxd del obj\i386\*.vxd
    -@if exist obj\i386\*.exp del obj\i386\*.exp
    -@if exist obj\i386\*.lib del obj\i386\*.lib
    -@if exist obj\i386\*.map del obj\i386\*.map
    -@if exist obj\i386\*.sym del obj\i386\*.sym
    -@if exist *.bak del *.bak
