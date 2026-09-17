# Microsoft Developer Studio Project File - Name="UInterface" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=UInterface - Win32 Debug_c
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UInterface.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UInterface.mak" CFG="UInterface - Win32 Debug_c"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UInterface - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "UInterface - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "UInterface - Win32 Debug_c" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "UInterface - Win32 Release_c" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/UInterface", RLQAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UInterface - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Build/UInterface___Win32_Release"
# PROP Intermediate_Dir "../Build/UInterface___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UINTERFACE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "_USRDLL" /D "UINTERFACE_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHINAVERSION" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"C:\Laqia Client\KOR\UInterface.dll"

!ELSEIF  "$(CFG)" == "UInterface - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Build/UInterface___Win32_Debug"
# PROP Intermediate_Dir "../Build/UInterface___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UINTERFACE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_USRDLL" /D "UINTERFACE_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\Laqia Client\ENG\UInterface.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "UInterface - Win32 Debug_c"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UInterface___Win32_Debug_c"
# PROP BASE Intermediate_Dir "UInterface___Win32_Debug_c"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Build/UInterface___Win32_Debug_c"
# PROP Intermediate_Dir "../Build/UInterface___Win32_Debug_c"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UINTERFACE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UINTERFACE_EXPORTS" /D "_CHINAVERSION" /D "_UMENOKOJI_MODE_" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../Bin/UInterface.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\DeicideOnline\UInterface.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "UInterface - Win32 Release_c"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "UInterface___Win32_Release_c"
# PROP BASE Intermediate_Dir "UInterface___Win32_Release_c"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Build/UInterface___Win32_Release_c"
# PROP Intermediate_Dir "../Build/UInterface___Win32_Release_c"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UINTERFACE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UINTERFACE_EXPORTS" /D "_CHINAVERSION" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../Bin/UInterface.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"C:\DeicideOnline\UInterface.dll"

!ENDIF 

# Begin Target

# Name "UInterface - Win32 Release"
# Name "UInterface - Win32 Debug"
# Name "UInterface - Win32 Debug_c"
# Name "UInterface - Win32 Release_c"
# Begin Group "Src"

# PROP Default_Filter ""
# Begin Group "IME"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\ChattingIME.cpp

!IF  "$(CFG)" == "UInterface - Win32 Release"

!ELSEIF  "$(CFG)" == "UInterface - Win32 Debug"

!ELSEIF  "$(CFG)" == "UInterface - Win32 Debug_c"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UInterface - Win32 Release_c"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Src\ChattingIME.h

!IF  "$(CFG)" == "UInterface - Win32 Release"

!ELSEIF  "$(CFG)" == "UInterface - Win32 Debug"

!ELSEIF  "$(CFG)" == "UInterface - Win32 Debug_c"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UInterface - Win32 Release_c"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Src\ChattingIME_C.cpp

!IF  "$(CFG)" == "UInterface - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UInterface - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UInterface - Win32 Debug_c"

!ELSEIF  "$(CFG)" == "UInterface - Win32 Release_c"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Src\ChattingIME_C.h

!IF  "$(CFG)" == "UInterface - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UInterface - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UInterface - Win32 Debug_c"

!ELSEIF  "$(CFG)" == "UInterface - Win32 Release_c"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Src\NotUseWord.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\NotUseWord.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\InchentCard.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\InchentCard.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIButton.h
# End Source File
# Begin Source File

SOURCE=.\Src\UICharacterFace.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UICharacterFace.h
# End Source File
# Begin Source File

SOURCE=.\Src\UICharSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UICharSelect.h
# End Source File
# Begin Source File

SOURCE=.\Src\UICheckButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UICheckButton.h
# End Source File
# Begin Source File

SOURCE=.\Src\UICompList.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIComponent.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIComponent.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIControls.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIControls.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIDialog.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIEditText.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIEditText.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIGameReady.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIGameReady.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIImageList.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIInventoryIN.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIInventoryIN.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIInventoryON.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIInventoryON.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIItemMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIItemMgr.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIMagicEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIMagicEntry.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIMagicList.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIMagicList.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIMagicMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIMagicMgr.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIMiniMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIMiniMap.h
# End Source File
# Begin Source File

SOURCE=.\Src\UInterface_Export.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UInterface_Export.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIPicture.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIProgress.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIQuickSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIQuickSlot.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIRectangle.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\Src\UISelectCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UISelectString.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UISelectString.h
# End Source File
# Begin Source File

SOURCE=.\Src\UISheet.h
# End Source File
# Begin Source File

SOURCE=.\Src\UISkillEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UISkillEntry.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIStaticText.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIStaticText.h
# End Source File
# Begin Source File

SOURCE=.\Src\UIStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UIStringList.h
# End Source File
# Begin Source File

SOURCE=.\Src\UITabButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UITabButton.h
# End Source File
# Begin Source File

SOURCE=.\Src\UITimer.h
# End Source File
# Begin Source File

SOURCE=.\Src\W3DGUIFileLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DGUIInput.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DGUIItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DGUISkill.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DNPCScript.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DUInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DUInterface.h
# End Source File
# Begin Source File

SOURCE=.\Src\W3DUIParty.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DUIShop.cpp
# End Source File
# End Group
# Begin Group "Inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inc\IW3DNPCScript.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DUInterface.h
# End Source File
# Begin Source File

SOURCE=.\Inc\UInterface.h
# End Source File
# Begin Source File

SOURCE=.\Inc\W3DEventCode.h
# End Source File
# Begin Source File

SOURCE=.\W3DUInterface.h
# End Source File
# End Group
# End Target
# End Project
