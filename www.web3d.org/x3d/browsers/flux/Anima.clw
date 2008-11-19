; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAnmView
LastTemplate=generic CWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Anima.h"
CDK=Y

ClassCount=7
Class1=CAnimaCtrl
Class2=CAnimaPropPage

ResourceCount=6
LastPage=0
Resource1=IDD_DIALOG6
Resource2="IDD_DIALOG5"
Class3=CAnmAboutBox
Resource3=IDD_DIALOG5
Class4=CAnmConsole
Resource4=IDD_PROPPAGE_ANIMA
Class5=CDashboard1
Class6=CAnmView
Class7=CAnmTooltip
Resource5=IDD_CONSOLE
Resource6="IDD_DIALOG6"

[CLS:CAnimaCtrl]
Type=0
HeaderFile=AnimaCtl.h
ImplementationFile=AnimaCtl.cpp
Filter=W
BaseClass=COleControl
VirtualFilter=wWC
LastObject=CAnimaCtrl

[CLS:CAnimaPropPage]
Type=0
HeaderFile=AnimaPpg.h
ImplementationFile=AnimaPpg.cpp
Filter=D
LastObject=CAnimaPropPage

[DLG:IDD_PROPPAGE_ANIMA]
Type=1
Class=CAnimaPropPage
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[CLS:CAnmAboutBox]
Type=0
HeaderFile=AnmAboutBox.h
ImplementationFile=AnmAboutBox.cpp
BaseClass=CDialog
Filter=D
LastObject=CAnmAboutBox

[CLS:CAnmConsole]
Type=0
HeaderFile=AnmConsole.h
ImplementationFile=AnmConsole.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CAnmConsole

[CLS:CDashboard1]
Type=0
HeaderFile=AnmDashboard.h
ImplementationFile=AnmDashboard.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=CDashboard1

[CLS:CAnmView]
Type=0
HeaderFile=AnmView.h
ImplementationFile=AnmView.cpp
BaseClass=CWnd
Filter=W
LastObject=CAnmView
VirtualFilter=WC

[CLS:CAnmTooltip]
Type=0
HeaderFile=AnmTooltip.h
ImplementationFile=AnmTooltip.cpp
BaseClass=CWnd
Filter=W
LastObject=CAnmTooltip
VirtualFilter=WC

[DLG:IDD_DIALOG5]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_DIALOG6]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:"IDD_DIALOG5"]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:"IDD_DIALOG6"]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_CONSOLE]
Type=1
Class=?
ControlCount=1
Control1=IDC_CONSOLETEXT,edit,1353779396

