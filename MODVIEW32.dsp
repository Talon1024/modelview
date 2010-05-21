# Microsoft Developer Studio Project File - Name="MODVIEW32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MODVIEW32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MODVIEW32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MODVIEW32.mak" CFG="MODVIEW32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MODVIEW32 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MODVIEW32 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Descent Manager MODELVIEW32", AFAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MODVIEW32 - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 glu32.lib opengl32.lib /nologo /subsystem:windows /machine:I386 /out:"Release\modviw32.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "MODVIEW32 - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug1"
# PROP Intermediate_Dir "Debug1"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 glu32.lib opengl32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/modviw32.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MODVIEW32 - Win32 Release"
# Name "MODVIEW32 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AW_CMultiViewSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\CompilerErrorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolControlsManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DataSafe.cpp
# End Source File
# Begin Source File

SOURCE=.\Details.cpp
# End Source File
# Begin Source File

SOURCE=.\DM_Reg.cpp
# End Source File
# Begin Source File

SOURCE=.\dm_tools.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorFS_FUEL.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorFS_GPNT.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorFS_MODEL.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorFS_SHLD.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorFS_SOBJ.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorFS_SPCL.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorFS_TGUN.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorFSDlg_MomentOfInertia.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorFSDlg_TextureNamesEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorFSDlg_Textures.cpp
# End Source File
# Begin Source File

SOURCE=.\Explorer.cpp
# End Source File
# Begin Source File

SOURCE=.\ExplorerBar.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FSChunk.cpp
# End Source File
# Begin Source File

SOURCE=.\GamePaneD2.cpp
# End Source File
# Begin Source File

SOURCE=.\GamePaneD3.cpp
# End Source File
# Begin Source File

SOURCE=.\GamePaneFS.cpp
# End Source File
# Begin Source File

SOURCE=.\GamePaneRF.cpp
# End Source File
# Begin Source File

SOURCE=.\Help_Using.cpp
# End Source File
# Begin Source File

SOURCE=.\HTMLTaskView.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportCOB.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LVSort.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\MODVIEW32.cpp
# End Source File
# Begin Source File

SOURCE=.\MODVIEW32.rc
# End Source File
# Begin Source File

SOURCE=.\MODVIEW32Doc.cpp
# End Source File
# Begin Source File

SOURCE=.\MODVIEW32View.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SilentSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashNew.cpp
# End Source File
# Begin Source File

SOURCE=.\splitterex.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticWithColor.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\stif32c.cpp
# End Source File
# Begin Source File

SOURCE=.\TableList.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureView.cpp
# End Source File
# Begin Source File

SOURCE=.\tga.cpp
# End Source File
# Begin Source File

SOURCE=.\TXList.cpp
# End Source File
# Begin Source File

SOURCE=.\TXView.cpp
# End Source File
# Begin Source File

SOURCE=.\TXViewModeToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\VPFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\VPFileDialogSubWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Welcome.cpp
# End Source File
# Begin Source File

SOURCE=.\WelcomePane.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Game definition Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\D2-extra.h"
# End Source File
# Begin Source File

SOURCE=".\D3-extra.h"
# End Source File
# Begin Source File

SOURCE=".\Fs-extra.h"
# End Source File
# Begin Source File

SOURCE=".\RF-Extra.h"
# End Source File
# End Group
# Begin Source File

SOURCE=.\AW_CMultiViewSplitter.h
# End Source File
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\CompilerErrorDlg.h
# End Source File
# Begin Source File

SOURCE=.\CoolControlsManager.h
# End Source File
# Begin Source File

SOURCE=.\DataSafe.h
# End Source File
# Begin Source File

SOURCE=.\Details.h
# End Source File
# Begin Source File

SOURCE=.\DM_Reg.h
# End Source File
# Begin Source File

SOURCE=.\dm_tools.h
# End Source File
# Begin Source File

SOURCE=.\EditorFS_FUEL.h
# End Source File
# Begin Source File

SOURCE=.\EditorFS_GPNT.h
# End Source File
# Begin Source File

SOURCE=.\EditorFS_MODEL.h
# End Source File
# Begin Source File

SOURCE=.\EditorFS_SHLD.h
# End Source File
# Begin Source File

SOURCE=.\EditorFS_SOBJ.h
# End Source File
# Begin Source File

SOURCE=.\EditorFS_SPCL.h
# End Source File
# Begin Source File

SOURCE=.\EditorFS_TGUN.h
# End Source File
# Begin Source File

SOURCE=.\EditorFSDlg_MomentOfInertia.h
# End Source File
# Begin Source File

SOURCE=.\EditorFSDlg_TextureNamesEdit.h
# End Source File
# Begin Source File

SOURCE=.\EditorFSDlg_Textures.h
# End Source File
# Begin Source File

SOURCE=.\Explorer.h
# End Source File
# Begin Source File

SOURCE=.\ExplorerBar.h
# End Source File
# Begin Source File

SOURCE=.\FolderDlg.h
# End Source File
# Begin Source File

SOURCE=.\FSChunk.h
# End Source File
# Begin Source File

SOURCE=.\GamePaneD2.h
# End Source File
# Begin Source File

SOURCE=.\GamePaneD3.h
# End Source File
# Begin Source File

SOURCE=.\GamePaneFS.h
# End Source File
# Begin Source File

SOURCE=.\GamePaneRF.h
# End Source File
# Begin Source File

SOURCE=.\Help_Using.h
# End Source File
# Begin Source File

SOURCE=.\HTMLTaskView.h
# End Source File
# Begin Source File

SOURCE=.\ImportCOB.h
# End Source File
# Begin Source File

SOURCE=.\ImportDlg.h
# End Source File
# Begin Source File

SOURCE=.\LVSort.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\MODVIEW32.h
# End Source File
# Begin Source File

SOURCE=.\MODVIEW32Doc.h
# End Source File
# Begin Source File

SOURCE=.\MODVIEW32View.h
# End Source File
# Begin Source File

SOURCE=.\OpenDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SilentSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\SplashNew.h
# End Source File
# Begin Source File

SOURCE=.\splitterex.h
# End Source File
# Begin Source File

SOURCE=.\StaticWithColor.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\stif32c.h
# End Source File
# Begin Source File

SOURCE=.\TableList.h
# End Source File
# Begin Source File

SOURCE=.\TextureView.h
# End Source File
# Begin Source File

SOURCE=.\TGA.h
# End Source File
# Begin Source File

SOURCE=.\TXList.h
# End Source File
# Begin Source File

SOURCE=.\TXView.h
# End Source File
# Begin Source File

SOURCE=.\TXViewModeToolbar.h
# End Source File
# Begin Source File

SOURCE=.\VPFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\VPFileDialogSubWnd.h
# End Source File
# Begin Source File

SOURCE=.\Welcome.h
# End Source File
# Begin Source File

SOURCE=.\WelcomePane.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\back5a.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\res\descent2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\descent3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DNet.ico
# End Source File
# Begin Source File

SOURCE=.\res\editor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\editor_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\editorfs.bmp
# End Source File
# Begin Source File

SOURCE=.\res\editorfs.ico
# End Source File
# Begin Source File

SOURCE=.\res\elementa.bmp
# End Source File
# Begin Source File

SOURCE=.\res\elemente.bmp
# End Source File
# Begin Source File

SOURCE=.\res\explorer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\freespac.bmp
# End Source File
# Begin Source File

SOURCE=.\res\freespace.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_des.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon_fre.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon_rec.bmp
# End Source File
# Begin Source File

SOURCE=.\res\menu16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\menu20.bmp
# End Source File
# Begin Source File

SOURCE=.\res\modelview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MODVIEW32.ico
# End Source File
# Begin Source File

SOURCE=.\res\MODVIEW32.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MODVIEW32Doc.ico
# End Source File
# Begin Source File

SOURCE=.\res\new_modelview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nomodel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nomodel_.bmp
# End Source File
# Begin Source File

SOURCE=.\shield.bmp
# End Source File
# Begin Source File

SOURCE=.\res\small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\version.bmp
# End Source File
# Begin Source File

SOURCE=.\res\version_.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\D2Pal.dat
# End Source File
# Begin Source File

SOURCE=.\MODVIEW32.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
