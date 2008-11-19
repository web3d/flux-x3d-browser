# Microsoft Developer Studio Project File - Name="fluxdx9" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fluxdx9 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fluxdx9.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fluxdx9.mak" CFG="fluxdx9 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fluxdx9 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fluxdx9 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fluxdx9 - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /I "..\Animation" /I "..\Core" /I "..\DX8" /I "..\DX9" /I "..\ECMAScript" /I "..\Events" /I "..\EventUtilities" /I "..\Graphics" /I "..\HAnim" /I "..\ImageIO" /I "..\Input" /I "..\Layers" /I "..\Loaders" /I "..\Math" /I "..\Math3D" /I "..\NURBS" /I "..\ParticleSystems" /I "..\SAI" /I "..\Scene" /I "..\Scripting" /I "..\Shaders" /I "..\Sound" /I "..\Threading" /I "..\Time" /I "..\UI" /I "..\Video" /I "..\Viewer" /I "..\Windowing" /I "..\WWW" /D "EXPORT_FLUXDX9" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../fluxcore/Release/fluxcore.lib strmbase.lib Msimg32.lib glu32.lib wininet.lib shlwapi.lib winmm.lib dsound.lib dinput8.lib d3d9.lib d3dx9.lib dxerr9.lib dxguid.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../Release/fluxdx9.dll"

!ELSEIF  "$(CFG)" == "fluxdx9 - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\Animation" /I "..\Core" /I "..\DX8" /I "..\DX9" /I "..\ECMAScript" /I "..\Events" /I "..\EventUtilities" /I "..\Graphics" /I "..\HAnim" /I "..\ImageIO" /I "..\Input" /I "..\Layers" /I "..\Loaders" /I "..\Math" /I "..\Math3D" /I "..\NURBS" /I "..\ParticleSystems" /I "..\SAI" /I "..\Scene" /I "..\Scripting" /I "..\Shaders" /I "..\Sound" /I "..\Threading" /I "..\Time" /I "..\UI" /I "..\Video" /I "..\Viewer" /I "..\Windowing" /I "..\WWW" /D "EXPORT_FLUXDX9" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
<<<<<<< .working
# ADD LINK32 ../fluxcore/Debug/fluxcore.lib strmbasd.lib Msimg32.lib glu32.lib wininet.lib shlwapi.lib winmm.lib dsound.lib dinput8.lib d3d9.lib d3dx9.lib dxerr9.lib dxguid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../Debug/fluxdx9.dll" /pdbtype:sept
=======
# ADD LINK32 ../fluxcore/Debug/fluxcore.lib ../3rdParty\DX9\lib\strmbasd.lib Msimg32.lib glu32.lib wininet.lib shlwapi.lib winmm.lib dsound.lib dinput8.lib d3d9.lib d3dx9.lib dxerr9.lib dxguid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../Debug/fluxdx9.dll" /pdbtype:sept
>>>>>>> .merge-right.r1335

!ENDIF 

# Begin Target

# Name "fluxdx9 - Win32 Release"
# Name "fluxdx9 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\DX9\anmd3ddevice9.cpp
# End Source File
# Begin Source File

SOURCE=..\DX9\anmd3ddevice9.h
# End Source File
# Begin Source File

SOURCE=..\DX9\anmd3derrors9.cpp
# End Source File
# Begin Source File

SOURCE=..\DX9\anmd3dgraphics9.cpp
# End Source File
# Begin Source File

SOURCE=..\DX9\anmd3dgraphics9.h
# End Source File
# Begin Source File

SOURCE=..\DX8\anmdikeyboard8.cpp
# End Source File
# Begin Source File

SOURCE=..\Dx8\anmdikeyboard8.h
# End Source File
# Begin Source File

SOURCE=..\DX8\anmdimouse8.cpp
# End Source File
# Begin Source File

SOURCE=..\Dx8\anmdimouse8.h
# End Source File
# Begin Source File

SOURCE=..\DX8\anmdinput8.cpp
# End Source File
# Begin Source File

SOURCE=..\Dx8\anmdinput8.h
# End Source File
# Begin Source File

SOURCE=..\DX9\anmdshowmovietextureblitter9.cpp
# End Source File
# Begin Source File

SOURCE=..\DX9\anmdshowmovietextureblitter9.h
# End Source File
# Begin Source File

SOURCE=.\fluxdx9.cpp
# End Source File
# Begin Source File

SOURCE=.\fluxdx9.def
# End Source File
# Begin Source File

SOURCE=.\fluxdx9.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\DX9\anmdx9message.h
# End Source File
# Begin Source File

SOURCE=.\fluxdx9.h
# End Source File
# Begin Source File

SOURCE=.\fluxdx9exports.h
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

SOURCE=.\res\fluxdx9.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
