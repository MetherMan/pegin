# Microsoft Developer Studio Project File - Name="GameClient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GAMECLIENT - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GameClient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GameClient.mak" CFG="GAMECLIENT - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GameClient - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GameClient - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameClient - Win32 Debug_c" (based on "Win32 (x86) Application")
!MESSAGE "GameClient - Win32 Release_c" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameClient", QEAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameClient - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Build/GameClient___Win32_Release"
# PROP Intermediate_Dir "../Build/GameClient___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Bin/LaqiaOnline.exe"

!ELSEIF  "$(CFG)" == "GameClient - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Build/GameClient___Win32_Debug"
# PROP Intermediate_Dir "../Build/GameClient___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Bin/LaqiaOnline.exe"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "GameClient - Win32 Debug_c"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameClient___Win32_Debug_c"
# PROP BASE Intermediate_Dir "GameClient___Win32_Debug_c"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Build/GameClient___Win32_Debug_c"
# PROP Intermediate_Dir "../Build/GameClient___Win32_Debug_c"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHINAVERSION" /D "_NOWEBLOGIN" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"../Bin/LaqiaOnline.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"../Bin/DeicideOnline.exe"

!ELSEIF  "$(CFG)" == "GameClient - Win32 Release_c"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GameClient___Win32_Release_c"
# PROP BASE Intermediate_Dir "GameClient___Win32_Release_c"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Build/GameClient___Win32_Release_c"
# PROP Intermediate_Dir "../Build/GameClient___Win32_Release_c"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHINAVERSION" /D "_NOWEBLOGIN" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../Bin/LaqiaOnline.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../Bin/DeicideOnline.exe"

!ENDIF 

# Begin Target

# Name "GameClient - Win32 Release"
# Name "GameClient - Win32 Debug"
# Name "GameClient - Win32 Debug_c"
# Name "GameClient - Win32 Release_c"
# Begin Group "Src"

# PROP Default_Filter ""
# Begin Group "State"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\ActionMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ActionMgr.h
# End Source File
# Begin Source File

SOURCE=.\Src\CharacterAction.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\CharacterAction.h
# End Source File
# Begin Source File

SOURCE=.\Src\MonsterAction.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\MonsterAction.h
# End Source File
# Begin Source File

SOURCE=.\Src\VehicleAction.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\VehicleAction.h
# End Source File
# End Group
# Begin Group "Creature"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\BodyMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\BodyMgr.h
# End Source File
# Begin Source File

SOURCE=.\Src\Character.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Creature.h
# End Source File
# Begin Source File

SOURCE=.\Src\Emblem.h
# End Source File
# Begin Source File

SOURCE=.\Src\Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Helper.h
# End Source File
# Begin Source File

SOURCE=.\Src\May.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\May.h
# End Source File
# Begin Source File

SOURCE=.\Src\Mercenary.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Monster.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\NPC.CPP
# End Source File
# Begin Source File

SOURCE=.\Src\Pet.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Pet.h
# End Source File
# Begin Source File

SOURCE=.\Src\SelectCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\SelectCreature.h
# End Source File
# Begin Source File

SOURCE=.\Src\Vehicle.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Vehicle.h
# End Source File
# End Group
# Begin Group "Daum"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\DaumGameParameter.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\DaumGameParameter.h
# End Source File
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Group "MiniGame"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\MiniGame.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\MiniGame.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Game.h
# End Source File
# Begin Source File

SOURCE=.\Src\GameProcessPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Intro.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Login.cpp

!IF  "$(CFG)" == "GameClient - Win32 Release"

!ELSEIF  "$(CFG)" == "GameClient - Win32 Debug"

!ELSEIF  "$(CFG)" == "GameClient - Win32 Debug_c"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameClient - Win32 Release_c"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Src\Login_c.cpp

!IF  "$(CFG)" == "GameClient - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameClient - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "GameClient - Win32 Debug_c"

!ELSEIF  "$(CFG)" == "GameClient - Win32 Release_c"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Src\SelectCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\SelectCharProduce.h
# End Source File
# End Group
# Begin Group "Skill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Skill.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Skill.h
# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Client.h
# End Source File
# Begin Source File

SOURCE=.\Src\Encrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Encrypt.h
# End Source File
# Begin Source File

SOURCE=.\Src\Packet.h
# End Source File
# Begin Source File

SOURCE=.\Src\Protocol.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Global.h
# End Source File
# Begin Source File

SOURCE=.\Src\Laqia.rc
# End Source File
# Begin Source File

SOURCE=.\Src\resource.h
# End Source File
# Begin Source File

SOURCE=.\Src\Sword.h
# End Source File
# Begin Source File

SOURCE=.\Src\Type.h
# End Source File
# Begin Source File

SOURCE=.\Src\WinMain.cpp
# End Source File
# End Group
# Begin Group "Res"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\Deicide.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Laqia.ico
# End Source File
# Begin Source File

SOURCE=.\Res\testcurs.cur
# End Source File
# End Group
# End Target
# End Project
