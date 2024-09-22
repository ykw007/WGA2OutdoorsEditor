# Microsoft Developer Studio Project File - Name="WGA2OutdoorsEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WGA2OutdoorsEditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WGA2OutdoorsEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WGA2OutdoorsEditor.mak" CFG="WGA2OutdoorsEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WGA2OutdoorsEditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WGA2OutdoorsEditor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WGA2OutdoorsEditor - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 d3d9.lib d3dx9.lib winmm.lib wga2ode.lib /nologo /subsystem:windows /machine:I386 /libpath:".\wga2ode\Release"

!ELSEIF  "$(CFG)" == "WGA2OutdoorsEditor - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d9.lib d3dx9.lib winmm.lib wga2ode.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:".\wga2ode\Debug"

!ENDIF 

# Begin Target

# Name "WGA2OutdoorsEditor - Win32 Release"
# Name "WGA2OutdoorsEditor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CoolDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\dialog\CreateTerrainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgbarObserver.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgbarTabmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgObjectEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTerrainEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTerrainModification.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTerrainObject.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTerrainTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SubDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\TerrainEditArea.cpp
# End Source File
# Begin Source File

SOURCE=.\TerrainEditingMng.cpp
# End Source File
# Begin Source File

SOURCE=.\TerrainHeightWorker.cpp
# End Source File
# Begin Source File

SOURCE=.\TerrainModEditArea.cpp
# End Source File
# Begin Source File

SOURCE=.\TerrainObjectWorker.cpp
# End Source File
# Begin Source File

SOURCE=.\TerrainTextureWorker.cpp
# End Source File
# Begin Source File

SOURCE=.\TerrainWorker.cpp
# End Source File
# Begin Source File

SOURCE=.\WGA2Button.cpp
# End Source File
# Begin Source File

SOURCE=.\WGA2OutdoorsEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\WGA2OutdoorsEditor.rc
# End Source File
# Begin Source File

SOURCE=.\WGA2OutdoorsEditorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\WGA2OutdoorsEditorView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\dialog\CreateTerrainDlg.h
# End Source File
# Begin Source File

SOURCE=.\DlgbarObserver.h
# End Source File
# Begin Source File

SOURCE=.\DlgbarTabmenu.h
# End Source File
# Begin Source File

SOURCE=.\DlgObjectEditor.h
# End Source File
# Begin Source File

SOURCE=.\DlgTerrainEditor.h
# End Source File
# Begin Source File

SOURCE=.\DlgTerrainModification.h
# End Source File
# Begin Source File

SOURCE=.\DlgTerrainObject.h
# End Source File
# Begin Source File

SOURCE=.\DlgTerrainTexture.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\wga2ode\misc.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SubDialog.h
# End Source File
# Begin Source File

SOURCE=.\TerrainEditArea.h
# End Source File
# Begin Source File

SOURCE=.\TerrainEditingMng.h
# End Source File
# Begin Source File

SOURCE=.\TerrainHeightWorker.h
# End Source File
# Begin Source File

SOURCE=.\TerrainModEditArea.h
# End Source File
# Begin Source File

SOURCE=.\TerrainObjectWorker.h
# End Source File
# Begin Source File

SOURCE=.\TerrainTextureWorker.h
# End Source File
# Begin Source File

SOURCE=.\TerrainWorker.h
# End Source File
# Begin Source File

SOURCE=.\WGA2Button.h
# End Source File
# Begin Source File

SOURCE=.\WGA2OutdoorsEditor.h
# End Source File
# Begin Source File

SOURCE=.\WGA2OutdoorsEditorDoc.h
# End Source File
# Begin Source File

SOURCE=.\WGA2OutdoorsEditorView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WGA2OutdoorsEditor.ico
# End Source File
# Begin Source File

SOURCE=.\res\WGA2OutdoorsEditor.rc2
# End Source File
# Begin Source File

SOURCE=.\res\WGA2OutdoorsEditorDoc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\WGA2OutdoorsEditor.manifest
# End Source File
# End Target
# End Project
