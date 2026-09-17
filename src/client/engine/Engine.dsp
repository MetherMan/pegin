# Microsoft Developer Studio Project File - Name="Engine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Engine - Win32 Debug_c
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Engine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Engine.mak" CFG="Engine - Win32 Debug_c"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Engine - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 Debug_c" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Engine - Win32 Release_c" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Engine", GCAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Build/Engine___Win32_Release"
# PROP Intermediate_Dir "../Build/Engine___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "_USRDLL" /D "ENGINE_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHINAVERSION" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"C:\Laqia Client\KOR\Engine.dll"

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Build/Engine___Win32_Debug"
# PROP Intermediate_Dir "../Build/Engine___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_USRDLL" /D "ENGINE_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"D:\Laqia Client\ENG\Engine.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug_c"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Engine___Win32_Debug_c"
# PROP BASE Intermediate_Dir "Engine___Win32_Debug_c"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Build/Engine___Win32_Debug_c"
# PROP Intermediate_Dir "../Build/Engine___Win32_Debug_c"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /D "_CHINAVERSION" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../Bin/Engine.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\DeicideOnline\Engine.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Engine - Win32 Release_c"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_Release_c"
# PROP BASE Intermediate_Dir "Engine___Win32_Release_c"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Build/Engine___Win32_Release_c"
# PROP Intermediate_Dir "../Build/Engine___Win32_Release_c"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ENGINE_EXPORTS" /D "_CHINAVERSION" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../Bin/Engine.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"C:\DeicideOnline\Engine.dll"

!ENDIF 

# Begin Target

# Name "Engine - Win32 Release"
# Name "Engine - Win32 Debug"
# Name "Engine - Win32 Debug_c"
# Name "Engine - Win32 Release_c"
# Begin Group "Src"

# PROP Default_Filter ""
# Begin Group "QuadTree"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\QuadTree.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\QuadTree.h
# End Source File
# End Group
# Begin Group "World"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\DynamicNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\DynamicNPC.h
# End Source File
# Begin Source File

SOURCE=.\Src\Entity.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Entity.h
# End Source File
# Begin Source File

SOURCE=.\Src\Gate.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Gate.h
# End Source File
# Begin Source File

SOURCE=.\Src\Map.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Map.h
# End Source File
# Begin Source File

SOURCE=.\Src\Potal.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Potal.h
# End Source File
# Begin Source File

SOURCE=.\Src\W3DWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DWorld.h
# End Source File
# End Group
# Begin Group "WeatherEffect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Leaf.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Rain.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Snow.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\WeatherEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\WeatherEffect.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\AStar.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\AStar.h
# End Source File
# Begin Source File

SOURCE=.\Src\DirectInput.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\DirectInput.h
# End Source File
# Begin Source File

SOURCE=.\Src\Door.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Door.h
# End Source File
# Begin Source File

SOURCE=.\Src\Effect.h
# End Source File
# Begin Source File

SOURCE=.\Src\Engine_Export.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Engine_Export.h
# End Source File
# Begin Source File

SOURCE=.\Src\FlyingObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\FlyingObject.h
# End Source File
# Begin Source File

SOURCE=.\Src\HitIndication.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HitIndication.h
# End Source File
# Begin Source File

SOURCE=.\Src\Indoor.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Indoor.h
# End Source File
# Begin Source File

SOURCE=.\Src\Loading.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Loading.h
# End Source File
# Begin Source File

SOURCE=.\Src\Magic.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Magic.h
# End Source File
# Begin Source File

SOURCE=.\Src\MagicEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\PathFind.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\PathFind.h
# End Source File
# Begin Source File

SOURCE=.\Src\TimeMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\TimeMgr.h
# End Source File
# Begin Source File

SOURCE=.\Src\W3DCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DEffectObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DFuncEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DInput.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DItemInfoMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DMagic.cpp
# End Source File
# End Group
# Begin Group "Inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inc\Engine.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DCamera.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DCreature.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DEffectObject.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DFuncEntity.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DInput.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DItem.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DItemInfoMgr.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DMagic.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DWorld.h
# End Source File
# Begin Source File

SOURCE=.\Inc\KeyCode.h
# End Source File
# End Group
# End Target
# End Project
