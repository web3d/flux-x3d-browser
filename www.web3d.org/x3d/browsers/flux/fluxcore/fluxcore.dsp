# Microsoft Developer Studio Project File - Name="fluxcore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fluxcore - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fluxcore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fluxcore.mak" CFG="fluxcore - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fluxcore - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fluxcore - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fluxcore - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /I "..\Animation" /I "..\Core" /I "..\DX8" /I "..\ECMAScript" /I "..\Events" /I "..\EventUtilities" /I "..\Graphics" /I "..\HAnim" /I "..\ImageIO" /I "..\Input" /I "..\Layers" /I "..\Loaders" /I "..\Math" /I "..\Math3D" /I "..\NURBS" /I "..\ParticleSystems" /I "..\SAI" /I "..\Scene" /I "..\Scripting" /I "..\Shaders" /I "..\Sound" /I "..\Threading" /I "..\Time" /I "..\UI" /I "..\Video" /I "..\Viewer" /I "..\Windowing" /I "..\WWW" /D "ZLIB_DLL" /D "EXPORT_FLUX" /D "DX8" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ..\imageio\zlib.lib C:\DXSDK\samples\Multimedia\DirectShow\BaseClasses\release\strmbase.lib dsound.lib dxguid.lib Msimg32.lib glu32.lib wininet.lib shlwapi.lib winmm.lib version.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../Release/fluxcore.dll"

!ELSEIF  "$(CFG)" == "fluxcore - Win32 Debug"

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
# ADD CPP /nologo /MDd /Gm /GX /ZI /Od /I ".." /I "..\Animation" /I "..\Core" /I "..\DX8" /I "..\ECMAScript" /I "..\Events" /I "..\EventUtilities" /I "..\Graphics" /I "..\HAnim" /I "..\ImageIO" /I "..\Input" /I "..\Layers" /I "..\Loaders" /I "..\Math" /I "..\Math3D" /I "..\NURBS" /I "..\ParticleSystems" /I "..\SAI" /I "..\Scene" /I "..\Scripting" /I "..\Shaders" /I "..\Sound" /I "..\Threading" /I "..\Time" /I "..\UI" /I "..\Video" /I "..\Viewer" /I "..\Windowing" /I "..\WWW" /D "ZLIB_DLL" /D "EXPORT_FLUX" /D "DX8" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\imageio\zlib.lib C:\keith\BaseClasses\Debug/strmbasd.lib dsound.lib dxguid.lib Msimg32.lib glu32.lib wininet.lib shlwapi.lib winmm.lib version.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"C:\Program Files\Media Machines\Flux/fluxcore.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "fluxcore - Win32 Release"
# Name "fluxcore - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\AnimaApp.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\anmdashboard.cpp
# End Source File
# Begin Source File

SOURCE=.\anmdashboardcontrol.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmTooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\AnmView.cpp
# End Source File
# Begin Source File

SOURCE=.\fluxcore.cpp
# End Source File
# Begin Source File

SOURCE=.\fluxcore.def
# End Source File
# Begin Source File

SOURCE=.\fluxcore.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\AnimaApp.h
# End Source File
# Begin Source File

SOURCE=.\AnmConsole.h
# End Source File
# Begin Source File

SOURCE=.\anmdashboard.h
# End Source File
# Begin Source File

SOURCE=.\anmdashboardcontrol.h
# End Source File
# Begin Source File

SOURCE=.\AnmTooltip.h
# End Source File
# Begin Source File

SOURCE=.\AnmView.h
# End Source File
# Begin Source File

SOURCE=.\fluxcore.h
# End Source File
# Begin Source File

SOURCE=..\fluxexports.h
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

SOURCE=.\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
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

SOURCE=.\res\fluxcore.rc2
# End Source File
# Begin Source File

SOURCE=.\forward.bmp
# End Source File
# Begin Source File

SOURCE=".\HAND-M.cur"
# End Source File
# Begin Source File

SOURCE=.\help.bmp
# End Source File
# Begin Source File

SOURCE=.\level.bmp
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

SOURCE=.\touchsen.cur
# End Source File
# End Group
# Begin Group "Layers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Layers\anmlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\Layers\anmlayer.h
# End Source File
# Begin Source File

SOURCE=..\Layers\anmlayer2D.cpp
# End Source File
# Begin Source File

SOURCE=..\Layers\anmlayer2D.h
# End Source File
# Begin Source File

SOURCE=..\Layers\anmlayer3D.cpp
# End Source File
# Begin Source File

SOURCE=..\Layers\anmlayer3D.h
# End Source File
# Begin Source File

SOURCE=..\Layers\anmlayout.cpp
# End Source File
# Begin Source File

SOURCE=..\Layers\anmlayout.h
# End Source File
# Begin Source File

SOURCE=..\Layers\anmlayoutgroup.cpp
# End Source File
# Begin Source File

SOURCE=..\Layers\anmlayoutgroup.h
# End Source File
# Begin Source File

SOURCE=..\Layers\anmmainlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\Layers\anmmainlayer.h
# End Source File
# Begin Source File

SOURCE=..\Layers\anmscreenfontstyle.cpp
# End Source File
# Begin Source File

SOURCE=..\Layers\anmscreenfontstyle.h
# End Source File
# Begin Source File

SOURCE=..\Layers\anmscreengroup.cpp
# End Source File
# Begin Source File

SOURCE=..\Layers\anmscreengroup.h
# End Source File
# End Group
# Begin Group "ParticleSystems"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ParticleSystems\anmparticleset.cpp
# End Source File
# Begin Source File

SOURCE=..\ParticleSystems\anmparticleset.h
# End Source File
# End Group
# Begin Group "NURBS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\NURBS\anmknotvector.cpp
# End Source File
# Begin Source File

SOURCE=..\Nurbs\anmknotvector.h
# End Source File
# Begin Source File

SOURCE=..\NURBS\anmnurbspatchsurface.cpp
# End Source File
# Begin Source File

SOURCE=..\Nurbs\anmnurbspatchsurface.h
# End Source File
# Begin Source File

SOURCE=..\NURBS\anmnurbssurfaceevaluator.cpp
# End Source File
# Begin Source File

SOURCE=..\Nurbs\anmnurbssurfaceevaluator.h
# End Source File
# Begin Source File

SOURCE=..\NURBS\anmnurbssurfaceinterpolator.cpp
# End Source File
# Begin Source File

SOURCE=..\Nurbs\anmnurbssurfaceinterpolator.h
# End Source File
# Begin Source File

SOURCE=..\NURBS\anmnurbstexturecoordinate.cpp
# End Source File
# Begin Source File

SOURCE=..\Nurbs\anmnurbstexturecoordinate.h
# End Source File
# End Group
# Begin Group "HAnim"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\HAnim\anmhanimdisplacer.cpp
# End Source File
# Begin Source File

SOURCE=..\HAnim\anmhanimdisplacer.h
# End Source File
# Begin Source File

SOURCE=..\HAnim\anmhanimhumanoid.cpp
# End Source File
# Begin Source File

SOURCE=..\HAnim\anmhanimhumanoid.h
# End Source File
# Begin Source File

SOURCE=..\HAnim\anmhanimjoint.cpp
# End Source File
# Begin Source File

SOURCE=..\HAnim\anmhanimjoint.h
# End Source File
# Begin Source File

SOURCE=..\HAnim\anmhanimsegment.cpp
# End Source File
# Begin Source File

SOURCE=..\HAnim\anmhanimsegment.h
# End Source File
# Begin Source File

SOURCE=..\HAnim\anmhanimsite.cpp
# End Source File
# Begin Source File

SOURCE=..\HAnim\anmhanimsite.h
# End Source File
# End Group
# Begin Group "ImageIO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ImageIO\BitmapReader.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Cderror.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Cdjpeg.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\gifio.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\gifio.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\ImageIO.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jcapimin.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jcapistd.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jccoefct.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jccolor.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jcdctmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jchuff.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jchuff.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jcinit.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jcmainct.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jcmarker.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jcmaster.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JCOMAPI.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jconfig.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jcparam.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jcphuff.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jcprepct.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jcsample.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDAPIMIN.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDAPISTD.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jdatadst.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDATASRC.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDC.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDCOLOR.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jdct.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDDCTMGR.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDHUFF.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jdhuff.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDINPUT.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDMAINCT.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDMARKER.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDMASTER.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDMERGE.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDPHUFF.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDPOSTCT.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JDSAMPLE.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JERROR.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jerror.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jfdctflt.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jfdctfst.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jfdctint.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JIDCTFLT.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JIDCTFST2.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JIDCTINT.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JIDCTRED.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jinclude.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JMEMMGR.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jmemnobs.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jmemsys.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jmorecfg.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jpegint.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jpeglib.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\jpgread.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JQUANT1.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JQUANT2.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\JUTILS.CPP
# End Source File
# Begin Source File

SOURCE=..\ImageIO\Jversion.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\png.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngasmrd.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngconf.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngerror.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngget.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngmem.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngpng.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngpread.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngread.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngrio.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngrtran.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngrutil.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngset.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngtrans.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngvcrd.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngwio.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngwrite.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngwtran.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\pngwutil.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageIO\zconf.h
# End Source File
# Begin Source File

SOURCE=..\ImageIO\zlib.h
# End Source File
# End Group
# Begin Group "UI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Ui\anmmenucommands.h
# End Source File
# End Group
# Begin Group "Video"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\video\anmmovietexture.cpp
# End Source File
# Begin Source File

SOURCE=..\Video\anmmovietexture.h
# End Source File
# Begin Source File

SOURCE=..\video\anmmovietexturerenderer.cpp
# End Source File
# Begin Source File

SOURCE=..\Video\anmmovietexturerenderer.h
# End Source File
# End Group
# Begin Group "EventUtilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\EventUtilities\anmbooleanfilter.cpp
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmbooleanfilter.h
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmbooleansequencer.cpp
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmbooleansequencer.h
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmbooleantoggle.cpp
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmbooleantoggle.h
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmbooleantrigger.cpp
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmbooleantrigger.h
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmintegersequencer.cpp
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmintegersequencer.h
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmintegertrigger.cpp
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmintegertrigger.h
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmsequencer.h
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmtimetrigger.cpp
# End Source File
# Begin Source File

SOURCE=..\EventUtilities\anmtimetrigger.h
# End Source File
# End Group
# Begin Group "SAI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SAI\anmbrowser.cpp
# End Source File
# Begin Source File

SOURCE=..\Sai\anmbrowser.h
# End Source File
# Begin Source File

SOURCE=..\SAI\anmexecutioncontext.cpp
# End Source File
# Begin Source File

SOURCE=..\Sai\anmexecutioncontext.h
# End Source File
# Begin Source File

SOURCE=..\SAI\anmexternalbrowser.cpp
# End Source File
# Begin Source File

SOURCE=..\Sai\anmexternalbrowser.h
# End Source File
# Begin Source File

SOURCE=..\SAI\anmsaimatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\Sai\anmsaimatrix.h
# End Source File
# Begin Source File

SOURCE=..\SAI\anmsainode.cpp
# End Source File
# Begin Source File

SOURCE=..\Sai\anmsainode.h
# End Source File
# Begin Source File

SOURCE=..\SAI\anmsainodeventsink.cpp
# End Source File
# Begin Source File

SOURCE=..\Sai\anmsainodeventsink.h
# End Source File
# Begin Source File

SOURCE=..\SAI\anmsairoute.cpp
# End Source File
# Begin Source File

SOURCE=..\Sai\anmsairoute.h
# End Source File
# Begin Source File

SOURCE=..\SAI\anmsaiscene.cpp
# End Source File
# Begin Source File

SOURCE=..\Sai\anmsaiscene.h
# End Source File
# End Group
# Begin Group "WWW"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\WWW\anmanchor.cpp
# End Source File
# Begin Source File

SOURCE=..\Www\anmanchor.h
# End Source File
# Begin Source File

SOURCE=..\Www\anmassetcache.h
# End Source File
# Begin Source File

SOURCE=..\WWW\anminline.cpp
# End Source File
# Begin Source File

SOURCE=..\Www\anminline.h
# End Source File
# Begin Source File

SOURCE=..\WWW\anmloadsensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Www\anmloadsensor.h
# End Source File
# Begin Source File

SOURCE=..\WWW\anmnetworkthread.cpp
# End Source File
# Begin Source File

SOURCE=..\Www\anmnetworkthread.h
# End Source File
# Begin Source File

SOURCE=..\WWW\anmurlobject.cpp
# End Source File
# Begin Source File

SOURCE=..\Www\anmurlobject.h
# End Source File
# End Group
# Begin Group "Viewer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Viewer\anmnavigationinfo.cpp
# End Source File
# Begin Source File

SOURCE=..\Viewer\anmnavigationinfo.h
# End Source File
# Begin Source File

SOURCE=..\Viewer\anmviewer.cpp
# End Source File
# Begin Source File

SOURCE=..\Viewer\anmviewer.h
# End Source File
# End Group
# Begin Group "Math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Math\anmfrustum.cpp
# End Source File
# Begin Source File

SOURCE=..\Math\anmfrustum.h
# End Source File
# Begin Source File

SOURCE=..\Math\anmhsvcolor.cpp
# End Source File
# Begin Source File

SOURCE=..\Math\anmhsvcolor.h
# End Source File
# Begin Source File

SOURCE=..\Math\anmintersection.cpp
# End Source File
# Begin Source File

SOURCE=..\Math\anmintersection.h
# End Source File
# Begin Source File

SOURCE=..\Math\anmmath.h
# End Source File
# Begin Source File

SOURCE=..\Math\anmmatrixinvert.cpp
# End Source File
# Begin Source File

SOURCE=..\Math\anmmatrixinvert.h
# End Source File
# Begin Source File

SOURCE=..\Math\anmpolygon.cpp
# End Source File
# Begin Source File

SOURCE=..\Math\anmpolygon.h
# End Source File
# Begin Source File

SOURCE=..\Math\anmquaternion.cpp
# End Source File
# Begin Source File

SOURCE=..\Math\anmquaternion.h
# End Source File
# Begin Source File

SOURCE=..\Math\anmvolumes.h
# End Source File
# Begin Source File

SOURCE=..\Math\MMDMatrixOps.cpp
# ADD CPP /Yu
# End Source File
# Begin Source File

SOURCE=..\Math\MMFMatrixOps.cpp
# End Source File
# Begin Source File

SOURCE=..\Math\MMMatrixOps.h
# End Source File
# End Group
# Begin Group "Events"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Events\anmevent.cpp
# End Source File
# Begin Source File

SOURCE=..\Events\anmevent.h
# End Source File
# Begin Source File

SOURCE=..\Events\anmroute.cpp
# End Source File
# Begin Source File

SOURCE=..\Events\anmroute.h
# End Source File
# End Group
# Begin Group "Animation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Animation\anmcolorinterpolator.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\anmcolorinterpolator.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmcoordinateinterpolator.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\anmcoordinateinterpolator.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmcoordinateinterpolator2D.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\anmcoordinateinterpolator2D.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmeaseineaseout.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\anmeaseineaseout.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anminterpolator.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmnormalinterpolator.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\anmnormalinterpolator.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmorientationinterpolator.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\anmorientationinterpolator.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmpositioninterpolator.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\anmpositioninterpolator.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmpositioninterpolator2D.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\anmpositioninterpolator2D.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmscalarinterpolator.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\anmscalarinterpolator.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmsplineinterpolator.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmSplinePositionInterpolator.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\anmsplinepositioninterpolator.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmSplinePositionInterpolator2D.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\anmsplinepositioninterpolator2D.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmsplinescalarInterpolator.cpp
# End Source File
# Begin Source File

SOURCE=..\Animation\anmsplinescalarinterpolator.h
# End Source File
# Begin Source File

SOURCE=..\Animation\anmtween.h
# End Source File
# End Group
# Begin Group "Time"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Time\anmtime.cpp
# End Source File
# Begin Source File

SOURCE=..\Time\anmtime.h
# End Source File
# Begin Source File

SOURCE=..\Time\anmtimedependentobject.cpp
# End Source File
# Begin Source File

SOURCE=..\Time\anmtimedependentobject.h
# End Source File
# Begin Source File

SOURCE=..\Time\anmtimesensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Time\anmtimesensor.h
# End Source File
# End Group
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Core\anmbindablenode.cpp
# End Source File
# Begin Source File

SOURCE=..\Core\anmbindablenode.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmcallback.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmclass.cpp
# End Source File
# Begin Source File

SOURCE=..\Core\anmclass.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmexternaleventsource.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmexternalresource.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmmessage.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadatadouble.cpp
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadatadouble.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadatafloat.cpp
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadatafloat.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadatainteger.cpp
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadatainteger.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadatanode.cpp
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadatanode.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadataset.cpp
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadataset.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadatastring.cpp
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadatastring.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmmetadatatable.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmobject.cpp
# End Source File
# Begin Source File

SOURCE=..\Core\anmobject.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmrefobject.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmsensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Core\anmsensor.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmstring.cpp
# End Source File
# Begin Source File

SOURCE=..\Core\anmstring.h
# End Source File
# Begin Source File

SOURCE=..\Core\anmworld.cpp
# End Source File
# Begin Source File

SOURCE=..\Core\anmworld.h
# End Source File
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Sound\anmaudioclip.cpp
# End Source File
# Begin Source File

SOURCE=..\Sound\anmaudioclip.h
# End Source File
# Begin Source File

SOURCE=..\Sound\anmaudioSensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Sound\anmaudioSensor.h
# End Source File
# Begin Source File

SOURCE=..\Sound\anmaudioSensorRenderer.cpp
# End Source File
# Begin Source File

SOURCE=..\Sound\anmaudioSensorRenderer.h
# End Source File
# Begin Source File

SOURCE=..\Sound\anmmediasensor.h
# End Source File
# Begin Source File

SOURCE=..\Sound\anmmediasourceobject.cpp
# End Source File
# Begin Source File

SOURCE=..\Sound\anmmediasourceobject.h
# End Source File
# Begin Source File

SOURCE=..\Sound\anmsound.cpp
# End Source File
# Begin Source File

SOURCE=..\Sound\anmsound.h
# End Source File
# Begin Source File

SOURCE=..\Sound\Fourier.cpp
# End Source File
# Begin Source File

SOURCE=..\Sound\Fourier.h
# End Source File
# Begin Source File

SOURCE=..\Sound\Sound.cpp
# End Source File
# Begin Source File

SOURCE=..\Sound\Sound.h
# End Source File
# Begin Source File

SOURCE=..\Sound\SoundLayer.cpp
# End Source File
# Begin Source File

SOURCE=..\Sound\SoundLayer.h
# End Source File
# Begin Source File

SOURCE=..\Sound\Wavread.cpp
# End Source File
# Begin Source File

SOURCE=..\Sound\Wavread.h
# End Source File
# End Group
# Begin Group "Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Input\anmcylindersensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\anmcylindersensor.h
# End Source File
# Begin Source File

SOURCE=..\Input\anmkeyboardsensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\anmkeyboardsensor.h
# End Source File
# Begin Source File

SOURCE=..\Input\anmkeysensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\anmkeysensor.h
# End Source File
# Begin Source File

SOURCE=..\Input\anmmousesensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\anmmousesensor.h
# End Source File
# Begin Source File

SOURCE=..\Input\anmpickmanager.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\anmpickmanager.h
# End Source File
# Begin Source File

SOURCE=..\Input\anmpicksensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\anmpicksensor.h
# End Source File
# Begin Source File

SOURCE=..\Input\anmplanesensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\anmplanesensor.h
# End Source File
# Begin Source File

SOURCE=..\Input\anmspheresensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\anmspheresensor.h
# End Source File
# Begin Source File

SOURCE=..\Input\anmstringsensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\anmstringsensor.h
# End Source File
# Begin Source File

SOURCE=..\Input\anmtouchsensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\anmtouchsensor.h
# End Source File
# Begin Source File

SOURCE=..\Input\InputLayer.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\InputLayer.h
# End Source File
# Begin Source File

SOURCE=..\Input\Keyboard.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\Keyboard.h
# End Source File
# Begin Source File

SOURCE=..\Input\Mouse.cpp
# End Source File
# Begin Source File

SOURCE=..\Input\Mouse.h
# End Source File
# End Group
# Begin Group "Windowing"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Windowing\anmbindstatuscallback.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmbindstatuscallback.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmbitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmbitmap.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmcontextmenu.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmcontextmenu.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmdecrypt.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmdecrypt.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmfilemanager.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmfilemanager.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmfilledrectangle.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmfilledrectangle.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmfont.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmfont.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmgunzip.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmgunzip.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmkeyboard.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmplatform.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmplatform.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmplugin.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmUpdateCheck.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\anmUpdateCheck.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\application.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\Application.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\FileVersionInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\FileVersionInfo.h
# End Source File
# Begin Source File

SOURCE=..\Windowing\getdxver.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\Window.cpp
# End Source File
# Begin Source File

SOURCE=..\Windowing\Window.h
# End Source File
# End Group
# Begin Group "Scene"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Scene\anmappearance.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmappearance.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmbackground.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmbackground.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmbillboard.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmbillboard.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmbox.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmbox.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcollision.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcollision.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcolor.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcolor.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcomposedcubecapcexture.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcomposedcubemaptexture.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcomposedgeometry.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcomposedgeometry.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcone.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcone.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcoordinate.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcoordinate.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcylinder.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmcylinder.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmdirectionallight.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmdirectionallight.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmelevationgrid.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmelevationgrid.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmextrudedfontstyle.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmextrudedfontstyle.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmextrusion.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmextrusion.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmfields.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmfields.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmfog.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmfog.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmfontstyle.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmfontstyle.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmgeometry.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmgeometry.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmgroup.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmgroup.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmimagetexture.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmimagetexture.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmindexedfaceset.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmindexedfaceset.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmindexedlineset.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmindexedlineset.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmindexedtriangleset.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmindexedtriangleset.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmlight.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmlight.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmlineset.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmlineset.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmlod.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmlod.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmmaterial.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmmaterial.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmmultitexture.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmmultitexture.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmmultitexturecoordinate.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmmultitexturecoordinate.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmmultitexturetransform.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmmultitexturetransform.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmnode.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmnode.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmnodedefs.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmnormal.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmnormal.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmoctree.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmoctree.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmpixeltexture.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmpixeltexture.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmpointlight.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmpointlight.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmpointset.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmpointset.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmproto.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmproto.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmproximitysensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmproximitysensor.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmrenderobject.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmrenderobject.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmscene.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmscene.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmshape.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmshape.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmsimvisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmsimvisitor.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmsphere.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmsphere.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmspotlight.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmspotlight.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmswitch.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmswitch.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtext.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtext.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtexture.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtexture.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtexturecoordinate.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtexturecoordinate.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtexturecoordinategenerator.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtexturecoordinategenerator.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtexturetransform.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtexturetransform.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtransform.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtransform.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtrianglefanset.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtrianglefanset.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtriangleset.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtriangleset.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtrianglestripset.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmtrianglestripset.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmupdatevisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmupdatevisitor.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmviewpoint.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmviewpoint.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmvisibilitysensor.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmvisibilitysensor.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmvisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmvisitor.h
# End Source File
# Begin Source File

SOURCE=..\Scene\anmworldinfo.cpp
# End Source File
# Begin Source File

SOURCE=..\Scene\anmworldinfo.h
# End Source File
# End Group
# Begin Group "Graphics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Graphics\anmcamera.cpp
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmcamera.h
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmd3derrors.h
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmdevice.cpp
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmdevice.h
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmgraphics.cpp
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmgraphics.h
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmmesh.cpp
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmmesh.h
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmprimitive.cpp
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmprimitive.h
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmtesselator.cpp
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmtesselator.h
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmtextglyph.cpp
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmtextglyph.h
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmtextmesh.cpp
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmtextmesh.h
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmtexturedata.cpp
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmtexturedata.h
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmviewport.cpp
# End Source File
# Begin Source File

SOURCE=..\Graphics\anmviewport.h
# End Source File
# Begin Source File

SOURCE=..\Graphics\crenderthread.cpp
# End Source File
# Begin Source File

SOURCE=..\Graphics\cRenderThread.h
# End Source File
# End Group
# Begin Group "Loaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Loaders\anmfileloader.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmfileloader.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmfilereader.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmfilereader.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmlexer.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmlexer.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmniynodes.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmniynodes.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmprotobuilder.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmprotobuilder.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmsymbol.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmsymbol.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmvrml97lexer.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmvrml97lexer.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmvrml97reader.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmvrml97reader.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmvrmlgrammar.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmvrmlgrammar.cpp.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmxmldomreader.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmxmldomreader.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmxmlreader.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmxmlreader.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmxmlsaxreader.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\anmxmlsaxreader.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\SAXContentHandlerImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\SAXContentHandlerImpl.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\SaxErrorHandlerImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\SaxErrorHandlerImpl.h
# End Source File
# Begin Source File

SOURCE=..\Loaders\SaxLexicalHandlerImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\Loaders\SaxLexicalHandlerImpl.h
# End Source File
# End Group
# Begin Group "Threading"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Threading\anmcriticalsection.cpp
# End Source File
# Begin Source File

SOURCE=..\Threading\anmcriticalsection.h
# End Source File
# Begin Source File

SOURCE=..\Threading\cThread.cpp
# End Source File
# Begin Source File

SOURCE=..\Threading\cThread.h
# End Source File
# End Group
# Begin Group "Math3D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\math3D\bSphere3.h
# End Source File
# Begin Source File

SOURCE=..\math3D\BspTree.cpp
# End Source File
# Begin Source File

SOURCE=..\math3D\BspTree.h
# End Source File
# Begin Source File

SOURCE=..\math3D\COLOR3.cpp
# End Source File
# Begin Source File

SOURCE=..\math3D\color3.h
# End Source File
# Begin Source File

SOURCE=..\math3D\COLOR4.cpp
# End Source File
# Begin Source File

SOURCE=..\math3D\color4.h
# End Source File
# Begin Source File

SOURCE=..\math3D\lineSeg3.h
# End Source File
# Begin Source File

SOURCE=..\math3D\mathD3D.h
# End Source File
# Begin Source File

SOURCE=..\math3D\mathGlobal.h
# End Source File
# Begin Source File

SOURCE=..\math3D\Matrix4.cpp
# End Source File
# Begin Source File

SOURCE=..\math3D\matrix4.h
# End Source File
# Begin Source File

SOURCE=..\math3D\Matrix4D.cpp
# End Source File
# Begin Source File

SOURCE=..\math3D\MATRIX4D.h
# End Source File
# Begin Source File

SOURCE=..\math3D\PLANE3.cpp
# End Source File
# Begin Source File

SOURCE=..\math3D\plane3.h
# End Source File
# Begin Source File

SOURCE=..\math3D\POINT3.cpp
# End Source File
# Begin Source File

SOURCE=..\math3D\point3.h
# End Source File
# Begin Source File

SOURCE=..\math3D\POINT3D.cpp
# End Source File
# Begin Source File

SOURCE=..\math3D\Point3D.h
# End Source File
# Begin Source File

SOURCE=..\math3D\point4.h
# End Source File
# Begin Source File

SOURCE=..\math3D\polygon.h
# End Source File
# Begin Source File

SOURCE=..\math3D\tri.h
# End Source File
# End Group
# Begin Group "Shaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Shaders\anmprogramshader.cpp
# End Source File
# Begin Source File

SOURCE=..\Shaders\anmprogramshader.h
# End Source File
# Begin Source File

SOURCE=..\Shaders\anmshader.h
# End Source File
# Begin Source File

SOURCE=..\Shaders\anmshaderprogram.cpp
# End Source File
# Begin Source File

SOURCE=..\Shaders\anmshaderprogram.h
# End Source File
# End Group
# Begin Group "ECMAScript"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ECMAScript\anmecmascript.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesbrowser.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesbrowser.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesclass.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmescompiler.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmescompiler.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmescontext.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmescontext.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesexpression.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesfields.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesfields.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesgrammar.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesgrammar.cpp.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmeslanguagebinding.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmeslanguagebinding.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmeslexer.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmeslexer.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesmath.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesmath.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesmatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesmatrix.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesmessage.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesnode.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesnode.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesobject.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesobject.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesstackmachine.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesstackmachine.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesstatement.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesstring.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesstring.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmessymbol.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmessymbol.h
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesvar.cpp
# End Source File
# Begin Source File

SOURCE=..\ECMAScript\anmesvar.h
# End Source File
# End Group
# Begin Group "Scripting"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Scripting\anmscript.cpp
# End Source File
# Begin Source File

SOURCE=..\Scripting\anmscript.h
# End Source File
# Begin Source File

SOURCE=..\Scripting\anmscriptlanguagebinding.cpp
# End Source File
# Begin Source File

SOURCE=..\Scripting\anmscriptlanguagebinding.h
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
