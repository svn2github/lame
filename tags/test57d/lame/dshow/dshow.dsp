# Microsoft Developer Studio Project File - Name="LAME DShow" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=LAME DShow - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dshow.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dshow.mak" CFG="LAME DShow - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LAME DShow - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "LAME DShow - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "."
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LAME DShow - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gz /MD /W3 /GX /O2 /I "../" /I "Vitec" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "INC_OLE2" /D "STRICT" /D WINVER=0x0400 /D _X86_=1 /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT MTL /nologo
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 strmbase.lib msvcrt.lib vfw32.lib winmm.lib kernel32.lib advapi32.lib version.lib largeint.lib Comdlg32.lib user32.lib gdi32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib quartz.lib measure.lib /base:"0x1c400000" /version:0.3 /entry:"DllEntryPoint@12" /subsystem:windows /dll /pdb:none /machine:I386 /nodefaultlib /out:"..\lame_dshow.ax"
# SUBTRACT LINK32 /nologo

!ELSEIF  "$(CFG)" == "LAME DShow - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gz /MDd /W3 /GX /ZI /Od /I "../" /I "Vitec" /D "WIN32" /D "DEBUG" /D "_WINDOWS" /D "INC_OLE2" /D WINVER=0x0400 /D _X86_=1 /D "STRICT" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 strmbasD.lib msvcrt.lib vfw32.lib winmm.lib kernel32.lib advapi32.lib version.lib largeint.lib Comdlg32.lib user32.lib gdi32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib quartz.lib measure.lib /nologo /base:"0x1c400000" /entry:"DllEntryPoint@12" /subsystem:windows /dll /incremental:no /debug /machine:I386 /nodefaultlib /out:"..\lame_dshow.ax"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "LAME DShow - Win32 Release"
# Name "LAME DShow - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Encoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Mpegac.cpp
# End Source File
# Begin Source File

SOURCE=.\Mpegac.def
# End Source File
# Begin Source File

SOURCE=.\Property.rc
# End Source File
# Begin Source File

SOURCE=.\PropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PropPage_adv.cpp
# End Source File
# Begin Source File

SOURCE=.\REG.CPP
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Encoder.h
# End Source File
# Begin Source File

SOURCE=.\iaudioprops.h
# End Source File
# Begin Source File

SOURCE=.\Mpegac.h
# End Source File
# Begin Source File

SOURCE=.\PropPage.h
# End Source File
# Begin Source File

SOURCE=.\PropPage_adv.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
