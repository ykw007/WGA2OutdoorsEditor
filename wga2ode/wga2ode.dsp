# Microsoft Developer Studio Project File - Name="wga2ode" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=wga2ode - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wga2ode.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wga2ode.mak" CFG="wga2ode - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wga2ode - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wga2ode - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wga2ode - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 d3d9.lib d3dx9.lib winmm.lib /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "wga2ode - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d9.lib d3dx9.lib winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "wga2ode - Win32 Release"
# Name "wga2ode - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3dsParser.cpp
# End Source File
# Begin Source File

SOURCE=.\AABB.cpp
# End Source File
# Begin Source File

SOURCE=.\AseParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterManager.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DGraphics.cpp
# End Source File
# Begin Source File

SOURCE=.\Dib.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\Frustum.cpp
# End Source File
# Begin Source File

SOURCE=.\GameWorldManager.cpp
# End Source File
# Begin Source File

SOURCE=.\IntersectionTester.cpp
# End Source File
# Begin Source File

SOURCE=.\LandscapeSceneManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshObj.cpp
# End Source File
# Begin Source File

SOURCE=.\Model.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Observer.cpp
# End Source File
# Begin Source File

SOURCE=.\Occlusion.cpp
# End Source File
# Begin Source File

SOURCE=.\Octree.cpp
# End Source File
# Begin Source File

SOURCE=.\OctreePicker.cpp
# End Source File
# Begin Source File

SOURCE=.\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\ParserSelector.cpp
# End Source File
# Begin Source File

SOURCE=.\Patch.cpp
# End Source File
# Begin Source File

SOURCE=.\Picker.cpp
# End Source File
# Begin Source File

SOURCE=.\Skybox.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\Text.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureManager.cpp
# End Source File
# Begin Source File

SOURCE=.\wga2ode.cpp
# End Source File
# Begin Source File

SOURCE=.\wga2ode.def
# End Source File
# Begin Source File

SOURCE=.\wga2ode.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\3dsParser.h
# End Source File
# Begin Source File

SOURCE=.\AABB.h
# End Source File
# Begin Source File

SOURCE=.\Base\Array.h
# End Source File
# Begin Source File

SOURCE=.\AseParser.h
# End Source File
# Begin Source File

SOURCE=.\Camera.h
# End Source File
# Begin Source File

SOURCE=.\CharacterManager.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\D3DGraphics.h
# End Source File
# Begin Source File

SOURCE=.\Dib.h
# End Source File
# Begin Source File

SOURCE=.\DrawMesh.h
# End Source File
# Begin Source File

SOURCE=.\File\FileUtil.h
# End Source File
# Begin Source File

SOURCE=.\Frustum.h
# End Source File
# Begin Source File

SOURCE=.\GameWorldManager.h
# End Source File
# Begin Source File

SOURCE=.\IntersectionTester.h
# End Source File
# Begin Source File

SOURCE=.\LandscapeSceneManager.h
# End Source File
# Begin Source File

SOURCE=.\Math\MathUtil.h
# End Source File
# Begin Source File

SOURCE=.\MeshObj.h
# End Source File
# Begin Source File

SOURCE=.\misc.h
# End Source File
# Begin Source File

SOURCE=.\ModelManager.h
# End Source File
# Begin Source File

SOURCE=.\Observer.h
# End Source File
# Begin Source File

SOURCE=.\Octree.h
# End Source File
# Begin Source File

SOURCE=.\OctreePicker.h
# End Source File
# Begin Source File

SOURCE=.\ParserSelector.h
# End Source File
# Begin Source File

SOURCE=.\Patch.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\Skybox.h
# End Source File
# Begin Source File

SOURCE=.\Base\sorts.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Terrain.h
# End Source File
# Begin Source File

SOURCE=.\Text.h
# End Source File
# Begin Source File

SOURCE=.\Base\Utility.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\wga2ode.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
