# Microsoft Developer Studio Project File - Name="npflux" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=npflux - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "npflux.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "npflux.mak" CFG="npflux - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "npflux - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "npflux - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "npflux - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "c:\dev\gecko-sdk\include" /I ".." /I "..\Animation" /I "..\Core" /I "..\DX8" /I "..\DX9" /I "..\ECMAScript" /I "..\Events" /I "..\EventUtilities" /I "..\Graphics" /I "..\HAnim" /I "..\ImageIO" /I "..\Input" /I "..\Layers" /I "..\Loaders" /I "..\Math" /I "..\Math3D" /I "..\NURBS" /I "..\ParticleSystems" /I "..\SAI" /I "..\Scene" /I "..\Scripting" /I "..\Sound" /I "..\Threading" /I "..\Time" /I "..\UI" /I "..\Video" /I "..\Viewer" /I "..\Windowing" /I "..\WWW" /D "MOZILLA_STRICT_API" /D "XPCOM_GLUE" /D "XP_WIN" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ..\fluxcore\release\fluxcore.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../Release/npflux.dll"

!ELSEIF  "$(CFG)" == "npflux - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "c:\dev\gecko-sdk\include" /I ".." /I "..\Animation" /I "..\Core" /I "..\DX8" /I "..\DX9" /I "..\ECMAScript" /I "..\Events" /I "..\EventUtilities" /I "..\Graphics" /I "..\HAnim" /I "..\ImageIO" /I "..\Input" /I "..\Layers" /I "..\Loaders" /I "..\Math" /I "..\Math3D" /I "..\NURBS" /I "..\ParticleSystems" /I "..\SAI" /I "..\Scene" /I "..\Scripting" /I "..\Sound" /I "..\Threading" /I "..\Time" /I "..\UI" /I "..\Video" /I "..\Viewer" /I "..\Windowing" /I "..\WWW" /D "MOZILLA_STRICT_API" /D "XPCOM_GLUE" /D "XP_WIN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\fluxcore\debug\fluxcore.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"C:\Program Files\Mozilla Firefox\plugins/npflux.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "npflux - Win32 Release"
# Name "npflux - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\anmmozillaplugin.cpp
# End Source File
# Begin Source File

SOURCE=.\anmmozplugin.cpp
# End Source File
# Begin Source File

SOURCE=.\anmmozplugin.h
# End Source File
# Begin Source File

SOURCE=.\np_entry.cpp
# End Source File
# Begin Source File

SOURCE=.\npflux.cpp
# End Source File
# Begin Source File

SOURCE=.\npflux.def
# End Source File
# Begin Source File

SOURCE=.\npflux.rc
# End Source File
# Begin Source File

SOURCE=.\npn_gate.cpp
# End Source File
# Begin Source File

SOURCE=.\npp_gate.cpp
# End Source File
# Begin Source File

SOURCE=.\npx3dbrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\npx3dbrowser.h
# End Source File
# Begin Source File

SOURCE=.\npx3dexecutioncontext.cpp
# End Source File
# Begin Source File

SOURCE=.\npx3dexecutioncontext.h
# End Source File
# Begin Source File

SOURCE=.\npx3dexternalbrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\npx3dexternalbrowser.h
# End Source File
# Begin Source File

SOURCE=.\npx3dfield.cpp
# End Source File
# Begin Source File

SOURCE=.\npx3dfield.h
# End Source File
# Begin Source File

SOURCE=.\npx3dfielddefinition.cpp
# End Source File
# Begin Source File

SOURCE=.\npx3dfielddefinition.h
# End Source File
# Begin Source File

SOURCE=.\npx3dnode.cpp
# End Source File
# Begin Source File

SOURCE=.\npx3dnode.h
# End Source File
# Begin Source File

SOURCE=.\npx3dprotodeclaration.cpp
# End Source File
# Begin Source File

SOURCE=.\npx3dprotodeclaration.h
# End Source File
# Begin Source File

SOURCE=.\npx3droute.cpp
# End Source File
# Begin Source File

SOURCE=.\npx3droute.h
# End Source File
# Begin Source File

SOURCE=.\npx3dscene.cpp
# End Source File
# Begin Source File

SOURCE=.\npx3dscene.h
# End Source File
# Begin Source File

SOURCE=.\plugin.cpp
# End Source File
# Begin Source File

SOURCE=.\plugin.h
# End Source File
# Begin Source File

SOURCE=.\scriptablepluginobject.cpp
# End Source File
# Begin Source File

SOURCE=.\scriptablepluginobject.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\anmmozillaplugin.h
# End Source File
# Begin Source File

SOURCE=.\npflux.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\dashboard.bmp
# End Source File
# Begin Source File

SOURCE=".\examine-new.bmp"
# End Source File
# Begin Source File

SOURCE=.\explore.bmp
# End Source File
# Begin Source File

SOURCE=.\forward.bmp
# End Source File
# Begin Source File

SOURCE=.\help.bmp
# End Source File
# Begin Source File

SOURCE=.\level.bmp
# End Source File
# Begin Source File

SOURCE=.\res\npflux.rc2
# End Source File
# Begin Source File

SOURCE=.\seek.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Debug\localuserguide.html
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
