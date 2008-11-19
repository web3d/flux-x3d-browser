# Microsoft Developer Studio Project File - Name="Anima" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Anima - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Anima.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Anima.mak" CFG="Anima - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Anima - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Anima - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Anima - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Anima - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Anima - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /I "Animation" /I "Charts" /I "Core" /I "DX7" /I "DX8" /I "ECMAScript" /I "Events" /I "EventUtilities" /I "Graphics" /I "HAnim" /I "ImageIO" /I "Input" /I "Layers" /I "Loaders" /I "Math" /I "Math3D" /I "NURBS" /I "ParticleSystems" /I "SAI" /I "Scene" /I "Scripting" /I "Sound" /I "Threading" /I "Time" /I "UI" /I "Video" /I "Viewer" /I "Windowing" /I "WWW" /D "REGISTER_WRL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 fluxcore\release\fluxcore.lib imageio\zlib.lib Msimg32.lib glu32.lib wininet.lib shlwapi.lib winmm.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Release
TargetPath=.\Release\Anima.ocx
InputPath=.\Release\Anima.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Anima - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /Gm /GX /ZI /Od /I ".." /I "Animation" /I "Charts" /I "Core" /I "DX7" /I "DX8" /I "ECMAScript" /I "Events" /I "EventUtilities" /I "Graphics" /I "HAnim" /I "ImageIO" /I "Input" /I "Layers" /I "Loaders" /I "Math" /I "Math3D" /I "NURBS" /I "ParticleSystems" /I "SAI" /I "Scene" /I "Scripting" /I "Sound" /I "Threading" /I "Time" /I "UI" /I "Video" /I "Viewer" /I "Windowing" /I "WWW" /D "REGISTER_WRL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 fluxcore\debug\fluxcore.lib Msimg32.lib glu32.lib wininet.lib shlwapi.lib winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /libpath:"D:\Data\dev\Flux\iScene\platform\flux\3rdParty\DX81\Lib\Debug"
# SUBTRACT LINK32 /profile
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=.\Debug\Anima.ocx
InputPath=.\Debug\Anima.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Anima - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Ext "ocx"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\DebugU
TargetPath=.\DebugU\Anima.ocx
InputPath=.\DebugU\Anima.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Anima - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseU"
# PROP BASE Intermediate_Dir "ReleaseU"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\ReleaseU
TargetPath=.\ReleaseU\Anima.ocx
InputPath=.\ReleaseU\Anima.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "Anima - Win32 Release"
# Name "Anima - Win32 Debug"
# Name "Anima - Win32 Unicode Debug"
# Name "Anima - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Anima.cpp
# End Source File
# Begin Source File

SOURCE=.\Anima.def
# End Source File
# Begin Source File

SOURCE=.\Anima.odl
# ADD MTL /h "Anima_i.h" /iid "Anima_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\Anima.rc
# End Source File
# Begin Source File

SOURCE=.\AnimaCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimaPpg.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmAboutBox.cpp
# End Source File
# Begin Source File

SOURCE=.\anmocxplugin.cpp
# End Source File
# Begin Source File

SOURCE=.\FluxPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\X3DBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\x3dbrowserbase.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DBrowserListener.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DEventOutObserver.cpp
# End Source File
# Begin Source File

SOURCE=.\x3dexecbase.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DExecutionContext.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DExternalBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DField.cpp
# End Source File
# Begin Source File

SOURCE=.\x3dfieldbase.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DFieldDefinition.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DFieldDefinitionArray.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DFieldEventListener.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DMFColor.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DMFFloat.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DMField.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DMFInt32.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DMFNode.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DMFRotation.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DMFString.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DMFVec2f.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DMFVec3f.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DNode.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DNodeArray.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DProtoDeclaration.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DProtoDeclarationArray.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DRoute.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DScene.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DSFBool.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DSFColor.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DSFDouble.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DSFFloat.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DSFInt32.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DSFNode.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DSFRotation.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DSFString.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DSFTime.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DSFVec2f.cpp
# End Source File
# Begin Source File

SOURCE=.\X3DSFVec3f.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Anima.h
# End Source File
# Begin Source File

SOURCE=.\AnimaCP.h
# End Source File
# Begin Source File

SOURCE=.\AnimaCtl.h
# End Source File
# Begin Source File

SOURCE=.\animadefs.h
# End Source File
# Begin Source File

SOURCE=.\animaerrors.h
# End Source File
# Begin Source File

SOURCE=.\AnimaPpg.h
# End Source File
# Begin Source File

SOURCE=.\AnmAboutBox.h
# End Source File
# Begin Source File

SOURCE=.\anmdisphelper.h
# End Source File
# Begin Source File

SOURCE=.\anmocxplugin.h
# End Source File
# Begin Source File

SOURCE=.\FluxPlayer.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\X3DBrowser.h
# End Source File
# Begin Source File

SOURCE=.\x3dbrowserbase.h
# End Source File
# Begin Source File

SOURCE=.\X3DBrowserListener.h
# End Source File
# Begin Source File

SOURCE=.\X3DEventOutObserver.h
# End Source File
# Begin Source File

SOURCE=.\x3dexecbase.h
# End Source File
# Begin Source File

SOURCE=.\X3DExecutionContext.h
# End Source File
# Begin Source File

SOURCE=.\X3DExternalBrowser.h
# End Source File
# Begin Source File

SOURCE=.\X3DField.h
# End Source File
# Begin Source File

SOURCE=.\x3dfieldbase.h
# End Source File
# Begin Source File

SOURCE=.\X3DFieldDefinition.h
# End Source File
# Begin Source File

SOURCE=.\X3DFieldDefinitionArray.h
# End Source File
# Begin Source File

SOURCE=.\X3DFieldEventListener.h
# End Source File
# Begin Source File

SOURCE=.\X3DMFColor.h
# End Source File
# Begin Source File

SOURCE=.\X3DMFFloat.h
# End Source File
# Begin Source File

SOURCE=.\X3DMField.h
# End Source File
# Begin Source File

SOURCE=.\X3DMFInt32.h
# End Source File
# Begin Source File

SOURCE=.\X3DMFNode.h
# End Source File
# Begin Source File

SOURCE=.\X3DMFRotation.h
# End Source File
# Begin Source File

SOURCE=.\X3DMFString.h
# End Source File
# Begin Source File

SOURCE=.\X3DMFVec2f.h
# End Source File
# Begin Source File

SOURCE=.\X3DMFVec3f.h
# End Source File
# Begin Source File

SOURCE=.\X3DNode.h
# End Source File
# Begin Source File

SOURCE=.\X3DNodeArray.h
# End Source File
# Begin Source File

SOURCE=.\x3dnodebase.h
# End Source File
# Begin Source File

SOURCE=.\X3DProtoDeclaration.h
# End Source File
# Begin Source File

SOURCE=.\X3DProtoDeclarationArray.h
# End Source File
# Begin Source File

SOURCE=.\X3DRoute.h
# End Source File
# Begin Source File

SOURCE=.\X3DScene.h
# End Source File
# Begin Source File

SOURCE=.\X3DSFBool.h
# End Source File
# Begin Source File

SOURCE=.\X3DSFColor.h
# End Source File
# Begin Source File

SOURCE=.\X3DSFDouble.h
# End Source File
# Begin Source File

SOURCE=.\X3DSFFloat.h
# End Source File
# Begin Source File

SOURCE=.\X3DSFInt32.h
# End Source File
# Begin Source File

SOURCE=.\X3DSFNode.h
# End Source File
# Begin Source File

SOURCE=.\X3DSFRotation.h
# End Source File
# Begin Source File

SOURCE=.\X3DSFString.h
# End Source File
# Begin Source File

SOURCE=.\X3DSFTime.h
# End Source File
# Begin Source File

SOURCE=.\X3DSFVec2f.h
# End Source File
# Begin Source File

SOURCE=.\X3DSFVec3f.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Anima.ico
# End Source File
# Begin Source File

SOURCE=.\Anima.rgs
# End Source File
# Begin Source File

SOURCE=.\AnimaCtl.bmp
# End Source File
# Begin Source File

SOURCE=.\Arrow_m.cur
# End Source File
# Begin Source File

SOURCE=.\back.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap10.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap11.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap12.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap9.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00003.bmp
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

SOURCE=.\Copy4way.cur
# End Source File
# Begin Source File

SOURCE=.\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\dashboard.bmp
# End Source File
# Begin Source File

SOURCE=.\dashboard2.bmp
# End Source File
# Begin Source File

SOURCE=".\dashboard4-background.bmp"
# End Source File
# Begin Source File

SOURCE=".\examine-new.bmp"
# End Source File
# Begin Source File

SOURCE=".\examine-newer.bmp"
# End Source File
# Begin Source File

SOURCE=.\examine.bmp
# End Source File
# Begin Source File

SOURCE=.\explore.bmp
# End Source File
# Begin Source File

SOURCE=.\FluxPlayer.rgs
# End Source File
# Begin Source File

SOURCE=.\forward.bmp
# End Source File
# Begin Source File

SOURCE=.\gravity.bmp
# End Source File
# Begin Source File

SOURCE=".\Hand-m.cur"
# End Source File
# Begin Source File

SOURCE=.\help.bmp
# End Source File
# Begin Source File

SOURCE=.\Help.cur
# End Source File
# Begin Source File

SOURCE=.\info.bmp
# End Source File
# Begin Source File

SOURCE=.\level.bmp
# End Source File
# Begin Source File

SOURCE=.\move.bmp
# End Source File
# Begin Source File

SOURCE=.\moveH.bmp
# End Source File
# Begin Source File

SOURCE=.\movemode.bmp
# End Source File
# Begin Source File

SOURCE=.\pan.bmp
# End Source File
# Begin Source File

SOURCE=.\pref.bmp
# End Source File
# Begin Source File

SOURCE=.\preferences.bmp
# End Source File
# Begin Source File

SOURCE=.\PropTest.rgs
# End Source File
# Begin Source File

SOURCE=.\redo.bmp
# End Source File
# Begin Source File

SOURCE=.\rotate.bmp
# End Source File
# Begin Source File

SOURCE=.\rotatemode.bmp
# End Source File
# Begin Source File

SOURCE=.\seek.bmp
# End Source File
# Begin Source File

SOURCE=.\seek.cur
# End Source File
# Begin Source File

SOURCE=.\spheresensor.cur
# End Source File
# Begin Source File

SOURCE=.\straighten.bmp
# End Source File
# Begin Source File

SOURCE=.\tilt.bmp
# End Source File
# Begin Source File

SOURCE=.\touchsen.cur
# End Source File
# Begin Source File

SOURCE=.\undo.bmp
# End Source File
# Begin Source File

SOURCE=.\viewArray02.bmp
# End Source File
# Begin Source File

SOURCE=.\viewnames.bmp
# End Source File
# Begin Source File

SOURCE=.\viewnext.bmp
# End Source File
# Begin Source File

SOURCE=.\viewPrev.bmp
# End Source File
# Begin Source File

SOURCE=.\viewprevious.bmp
# End Source File
# Begin Source File

SOURCE=.\X3DBrowser.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DBrowserListener.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DEventIn.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DEventOut.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DEventOutObserver.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DExecutionContext.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DExternalBrowser.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DField.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DFieldAggTest.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DFieldDefinition.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DFieldDefinitionArray.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DFieldEventListener.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DFieldMFColor.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DFieldMFNode.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DMFColor.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DMFFloat.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DMField.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DMFInt32.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DMFNode.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DMFRotation.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DMFString.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DMFVec2f.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DMFVec3f.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DNode.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DNodeArray.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DProtoDeclaration.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DProtoDeclarationArray.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DRoute.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DScene.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DSFBool.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DSFColor.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DSFDouble.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DSFFloat.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DSFInt32.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DSFNode.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DSFRotation.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DSFString.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DSFTime.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DSFVec2f.rgs
# End Source File
# Begin Source File

SOURCE=.\X3DSFVec3f.rgs
# End Source File
# Begin Source File

SOURCE=.\zoom.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Debug\localuserguide.html
# End Source File
# Begin Source File

SOURCE=.\mmgraphics.x3d
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Anima : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
