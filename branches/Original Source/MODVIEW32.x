; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CGameBarD3
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "modview32.h"
LastPage=0

ClassCount=13
Class1=CExplorer
Class2=CGameBarD3
Class3=CGameBarFS
Class4=CMainFrame
Class5=CMODVIEW32App
Class6=CAboutDlg
Class7=CMODVIEW32Doc
Class8=CMODVIEW32View
Class9=COpenDlg
Class10=COptionsDlg

ResourceCount=15
Resource1=IDD_ABOUTBOX
Resource2=IDD_OPEN
Resource3=IDD_FREESPACE
Resource4=IDD_OPTIONS
Resource5=IDR_MAINFRAME
Class11=CDataSafe
Resource6=IDD_DESCENT3
Resource7=IDD_OPTIONS (English (U.S.))
Resource8=IDD_ABOUTBOX (English (U.S.))
Resource9=IDD_SPLASH
Resource10=IDR_SMALL
Resource11=IDD_DESCENT3 (English (U.S.))
Resource12=IDD_DETAILS
Class12=CSplash
Resource13=IDD_OPEN (English (U.S.))
Resource14=IDD_FREESPACE (English (U.S.))
Class13=CDetails
Resource15=IDR_MAINFRAME (English (U.S.))

[CLS:CExplorer]
Type=0
BaseClass=CTreeView
HeaderFile=Explorer.h
ImplementationFile=Explorer.cpp
Filter=C
VirtualFilter=VWC
LastObject=CExplorer

[CLS:CGameBarD3]
Type=0
BaseClass=CDialogBar
HeaderFile=GameBarD3.h
ImplementationFile=GameBarD3.cpp
Filter=C
VirtualFilter=dWC
LastObject=IDC_POSITION_CHECK

[CLS:CGameBarFS]
Type=0
BaseClass=CDialogBar
HeaderFile=GameBarFS.h
ImplementationFile=GameBarFS.cpp
LastObject=IDC_CHECK1

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=W
VirtualFilter=fWC
LastObject=ID_FILE_OPENFILE

[CLS:CMODVIEW32App]
Type=0
BaseClass=CWinApp
HeaderFile=MODVIEW32.h
ImplementationFile=MODVIEW32.cpp
LastObject=CMODVIEW32App

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=MODVIEW32.cpp
ImplementationFile=MODVIEW32.cpp
LastObject=ID_NAVIGATION_RESETZOOMLEVEL

[CLS:CMODVIEW32Doc]
Type=0
BaseClass=CDocument
HeaderFile=MODVIEW32Doc.h
ImplementationFile=MODVIEW32Doc.cpp

[CLS:CMODVIEW32View]
Type=0
BaseClass=CView
HeaderFile=MODVIEW32View.h
ImplementationFile=MODVIEW32View.cpp
Filter=C
VirtualFilter=VWC
LastObject=ID_MODEL_TEXTURELIST

[CLS:COpenDlg]
Type=0
BaseClass=CDialog
HeaderFile=OpenDlg.h
ImplementationFile=OpenDlg.cpp

[CLS:COptionsDlg]
Type=0
BaseClass=CDialog
HeaderFile=OptionsDlg.h
ImplementationFile=OptionsDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_PATH_D3

[DLG:IDD_DESCENT3]
Type=1
Class=CGameBarD3
ControlCount=13
Control1=IDC_STATIC,static,1342308364
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308364
Control4=IDC_STATIC,static,1342308364
Control5=IDC_STATIC,static,1342308364
Control6=IDC_D3_VERTICIES,static,1342308352
Control7=IDC_D3_POLYGONS,static,1342308352
Control8=IDC_D3_TEXTURES,static,1342308352
Control9=IDC_D3_SOBJ,static,1342308352
Control10=IDC_D3_TEXTURELIST,listbox,1352728835
Control11=IDC_STATIC,static,1342308352
Control12=IDC_D3_POSITIONS1,listbox,1352728833
Control13=IDC_POSITION_CHECK,button,1342242851

[DLG:IDD_FREESPACE]
Type=1
Class=CGameBarFS
ControlCount=6
Control1=IDC_DETAILLEVEL,SysListView32,1350631437
Control2=IDC_CHECK1,button,1342242819
Control3=IDC_SHOWSHIELD,button,1342242819
Control4=IDC_CHECK3,button,1342242819
Control5=IDC_TEXTURES,listbox,1352728835
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_OPEN]
Type=1
Class=COpenDlg
ControlCount=9
Control1=IDC_BUTTON1,button,1342177280
Control2=IDC_BUTTON2,button,1073741824
Control3=IDC_BUTTON3,button,1073741824
Control4=IDC_BUTTON4,button,1073741824
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_FILETREE,SysTreeView32,1350631463
Control8=IDC_EDIT1,edit,1350631552
Control9=IDC_STATIC,static,1342308352

[DLG:IDD_OPTIONS]
Type=1
Class=COptionsDlg
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1073872896
Control6=IDC_PATH_D2,edit,1082196096
Control7=IDC_STATIC,static,1342308352
Control8=IDC_PATH_F1,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_PATH_F2,edit,1350631552
Control11=IDC_STATIC,static,1342179331

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_FILE_OPENFILE
Command3=ID_FILE_OPTIONSCONFIGURATION
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_RENDERMODE_WIREFRAME
Command7=ID_VIEW_RENDERMODE_SURFACE
Command8=ID_VIEW_RENDERMODE_TEXTURED
Command9=ID_VIEW_RENDERSETTINGS_CULLFACE
Command10=ID_VIEW_RENDERSETTINGS_SMOOTH
Command11=ID_VIEW_FASTTEXTURELOAD
Command12=ID_VIEW_RENDERSETTINGS_D3SMOOTH
Command13=ID_VIEW_RENDERSETTINGS_ZBUFFERED
Command14=ID_VIEW_SELECTBACKGROUNDCOLOR
Command15=ID_VIEW_DETAILLEVEL_LEVEL1
Command16=ID_VIEW_DETAILLEVEL_LEVEL2
Command17=ID_VIEW_DETAILLEVEL_LEVEL3
Command18=ID_VIEW_DETAILLEVEL_LEVEL4
Command19=ID_VIEW_DETAILLEVEL_LEVEL5
Command20=ID_VIEW_DETAILLEVEL_LEVEL6
Command21=ID_VIEW_DETAILLEVEL_DEBRIS
Command22=ID_VIEW_SHOWSHIELDS
Command23=ID_VIEW_SHOWTHRUSTER
Command24=ID_VIEW_DOROTATION
Command25=ID_NAVIGATION_RESETZOOMLEVEL
Command26=ID_MODEL_LEFT
Command27=ID_MODEL_RIGHT
Command28=ID_MODEL_UP
Command29=ID_MODEL_DOWN
Command30=ID_MODEL_ZOOMIN
Command31=ID_MODEL_ZOOMOUT
Command32=ID_APP_ABOUT
CommandCount=32

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_VIEW_DETAILLEVEL_LEVEL1
Command3=ID_VIEW_DETAILLEVEL_LEVEL2
Command4=ID_VIEW_DETAILLEVEL_LEVEL3
Command5=ID_VIEW_DETAILLEVEL_LEVEL4
Command6=ID_VIEW_DETAILLEVEL_LEVEL5
Command7=ID_VIEW_DETAILLEVEL_LEVEL6
Command8=ID_VIEW_DETAILLEVEL_DEBRIS
Command9=ID_VIEW_RENDERMODE_WIREFRAME
Command10=ID_VIEW_RENDERMODE_SURFACE
Command11=ID_VIEW_RENDERMODE_TEXTURED
Command12=ID_VIEW_SHOWSHIELDS
Command13=ID_VIEW_SHOWTHRUSTER
Command14=ID_DUMMY
CommandCount=14

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_VIEW_DETAILLEVEL_DEBRIS
Command2=ID_VIEW_DETAILLEVEL_LEVEL1
Command3=ID_VIEW_RENDERMODE_WIREFRAME
Command4=ID_VIEW_DETAILLEVEL_LEVEL2
Command5=ID_VIEW_RENDERMODE_SURFACE
Command6=ID_VIEW_DETAILLEVEL_LEVEL3
Command7=ID_VIEW_RENDERMODE_TEXTURED
Command8=ID_VIEW_DETAILLEVEL_LEVEL4
Command9=ID_VIEW_DETAILLEVEL_LEVEL5
Command10=ID_VIEW_DETAILLEVEL_LEVEL6
Command11=ID_FILE_OPEN
Command12=ID_NAVIGATION_RESETZOOMLEVEL
Command13=ID_MODEL_DOWN
Command14=ID_MODEL_LEFT
Command15=ID_MODEL_ZOOMOUT
Command16=ID_MODEL_ZOOMIN
Command17=ID_MODEL_RIGHT
Command18=ID_MODEL_UP
CommandCount=18

[CLS:CDataSafe]
Type=0
HeaderFile=DataSafe.h
ImplementationFile=DataSafe.cpp
BaseClass=generic CWnd
Filter=W
LastObject=CDataSafe

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_VIEW_DETAILLEVEL_LEVEL1
Command3=ID_VIEW_DETAILLEVEL_LEVEL2
Command4=ID_VIEW_DETAILLEVEL_LEVEL3
Command5=ID_VIEW_DETAILLEVEL_LEVEL4
Command6=ID_VIEW_DETAILLEVEL_LEVEL5
Command7=ID_VIEW_DETAILLEVEL_LEVEL6
Command8=ID_VIEW_DETAILLEVEL_DEBRIS
Command9=ID_VIEW_RENDERMODE_WIREFRAME
Command10=ID_VIEW_RENDERMODE_SURFACE
Command11=ID_VIEW_RENDERMODE_TEXTURED
Command12=ID_VIEW_SHOWSHIELDS
Command13=ID_VIEW_SHOWTHRUSTER
Command14=ID_DUMMY
CommandCount=14

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_FILE_OPENFILE
Command3=ID_FILE_OPTIONSCONFIGURATION
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_RENDERMODE_WIREFRAME
Command7=ID_VIEW_RENDERMODE_SURFACE
Command8=ID_VIEW_RENDERMODE_TEXTURED
Command9=ID_VIEW_RENDERSETTINGS_CULLFACE
Command10=ID_VIEW_RENDERSETTINGS_SMOOTH
Command11=ID_VIEW_RENDERSETTINGS_ZBUFFERED
Command12=ID_VIEW_SELECTBACKGROUNDCOLOR
Command13=ID_VIEW_DOROTATION
Command14=ID_VIEW_RENDERSETTINGS_D3SMOOTH
Command15=ID_VIEW_DETAILLEVEL_LEVEL1
Command16=ID_VIEW_DETAILLEVEL_LEVEL2
Command17=ID_VIEW_DETAILLEVEL_LEVEL3
Command18=ID_VIEW_DETAILLEVEL_LEVEL4
Command19=ID_VIEW_DETAILLEVEL_LEVEL5
Command20=ID_VIEW_DETAILLEVEL_LEVEL6
Command21=ID_VIEW_DETAILLEVEL_DEBRIS
Command22=ID_VIEW_SHOWSHIELDS
Command23=ID_VIEW_SHOWTHRUSTER
Command24=ID_VIEW_FASTTEXTURELOAD
Command25=ID_MODEL_NEXT
Command26=ID_MODEL_PREVIOUS
Command27=ID_MODEL_TEXTURELIST
Command28=ID_NAVIGATION_RESETZOOMLEVEL
Command29=ID_MODEL_LEFT
Command30=ID_MODEL_RIGHT
Command31=ID_MODEL_UP
Command32=ID_MODEL_DOWN
Command33=ID_MODEL_ZOOMIN
Command34=ID_MODEL_ZOOMOUT
Command35=ID_APP_ABOUT
CommandCount=35

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_VIEW_DETAILLEVEL_DEBRIS
Command2=ID_VIEW_DETAILLEVEL_LEVEL1
Command3=ID_VIEW_RENDERMODE_WIREFRAME
Command4=ID_VIEW_DETAILLEVEL_LEVEL2
Command5=ID_VIEW_RENDERMODE_SURFACE
Command6=ID_VIEW_DETAILLEVEL_LEVEL3
Command7=ID_VIEW_RENDERMODE_TEXTURED
Command8=ID_VIEW_DETAILLEVEL_LEVEL4
Command9=ID_VIEW_DETAILLEVEL_LEVEL5
Command10=ID_VIEW_DETAILLEVEL_LEVEL6
Command11=ID_MODEL_NEXT
Command12=ID_FILE_OPEN
Command13=ID_MODEL_PREVIOUS
Command14=ID_NAVIGATION_RESETZOOMLEVEL
Command15=ID_MODEL_DOWN
Command16=ID_MODEL_LEFT
Command17=ID_MODEL_ZOOMOUT
Command18=ID_MODEL_ZOOMIN
Command19=ID_MODEL_TEXTURELIST
Command20=ID_MODEL_RIGHT
Command21=ID_MODEL_UP
CommandCount=21

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_FREESPACE (English (U.S.))]
Type=1
Class=CGameBarFS
ControlCount=6
Control1=IDC_DETAILLEVEL,SysListView32,1350631437
Control2=IDC_CHECK1,button,1342242819
Control3=IDC_SHOWSHIELD,button,1342242819
Control4=IDC_CHECK3,button,1342242819
Control5=IDC_TEXTURES,listbox,1352728835
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_OPTIONS (English (U.S.))]
Type=1
Class=COptionsDlg
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1073872896
Control6=IDC_PATH_D2,edit,1082196096
Control7=IDC_STATIC,static,1342308352
Control8=IDC_PATH_F1,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_PATH_F2,edit,1350631552
Control11=IDC_STATIC,static,1342179331
Control12=IDC_STATIC,static,1342308352
Control13=IDC_PATH_D3,edit,1350631552

[DLG:IDD_OPEN (English (U.S.))]
Type=1
Class=COpenDlg
ControlCount=9
Control1=IDC_BUTTON1,button,1342177280
Control2=IDC_BUTTON2,button,1073741824
Control3=IDC_BUTTON3,button,1073741824
Control4=IDC_BUTTON4,button,1073741824
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_FILETREE,SysTreeView32,1350631463
Control8=IDC_EDIT1,edit,1350631552
Control9=IDC_STATIC,static,1342308352

[DLG:IDD_DESCENT3 (English (U.S.))]
Type=1
Class=CGameBarD3
ControlCount=13
Control1=IDC_STATIC,static,1342308364
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308364
Control4=IDC_STATIC,static,1342308364
Control5=IDC_STATIC,static,1342308364
Control6=IDC_D3_VERTICIES,static,1342308352
Control7=IDC_D3_POLYGONS,static,1342308352
Control8=IDC_D3_TEXTURES,static,1342308352
Control9=IDC_D3_SOBJ,static,1342308352
Control10=IDC_D3_TEXTURELIST,listbox,1352728835
Control11=IDC_STATIC,static,1342308352
Control12=IDC_D3_POSITIONS1,listbox,1352728833
Control13=IDC_POSITION_CHECK,button,1342242851

[DLG:IDD_SPLASH]
Type=1
Class=CSplash
ControlCount=1
Control1=IDC_STATIC,static,1342177294

[CLS:CSplash]
Type=0
HeaderFile=Splash.h
ImplementationFile=Splash.cpp
BaseClass=CDialog
Filter=D
LastObject=CSplash

[TB:IDR_SMALL]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_APP_ABOUT
Command3=ID_NAVIGATION_RESETZOOMLEVEL
Command4=ID_FILE_OPTIONSCONFIGURATION
Command5=ID_MODEL_LEFT
Command6=ID_MODEL_RIGHT
Command7=ID_MODEL_UP
Command8=ID_MODEL_DOWN
Command9=ID_MODEL_ZOOMIN
Command10=ID_MODEL_ZOOMOUT
Command11=ID_VIEW_SELECTBACKGROUNDCOLOR
Command12=ID_MODEL_TEXTURELIST
CommandCount=12

[DLG:IDD_DETAILS]
Type=1
Class=CDetails
ControlCount=2
Control1=IDCANCEL,button,1342242816
Control2=IDC_LISTVIEW,SysListView32,1350664221

[CLS:CDetails]
Type=0
HeaderFile=Details.h
ImplementationFile=Details.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDetails
