# Microsoft Developer Studio Project File - Name="Common" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Common - Win32 Debug_c
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Common.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Common.mak" CFG="Common - Win32 Debug_c"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Common - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Common - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Common - Win32 Debug_c" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Common - Win32 Release_c" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Common", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Common - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Build/Common___Win32_Release"
# PROP Intermediate_Dir "../Build/Common___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMON_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "_USRDLL" /D "COMMON_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHINAVERSION" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"C:\Laqia Client\KOR\Common.dll" /implib:"Lib/Common.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Common - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Build/Common___Win32_Debug"
# PROP Intermediate_Dir "../Build/Common___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMON_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_USRDLL" /D "COMMON_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBCMTD.lib" /out:"C:\Laqia Client\ENG\Common.dll" /implib:"Lib/common.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Common - Win32 Debug_c"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Common___Win32_Debug_c0"
# PROP BASE Intermediate_Dir "Common___Win32_Debug_c0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Build/Common___Win32_Debug_c"
# PROP Intermediate_Dir "../Build/Common___Win32_Debug_c"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMON_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMON_EXPORTS" /D "_CHINAVERSION" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../Bin/Common.dll" /implib:"Lib/common.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBCMTD.lib" /out:"C:\DeicideOnline\Common.dll" /implib:"Lib/common.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Common - Win32 Release_c"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Common___Win32_Release_c"
# PROP BASE Intermediate_Dir "Common___Win32_Release_c"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Build/Common___Win32_Release_c"
# PROP Intermediate_Dir "../Build/Common___Win32_Release_c"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMON_EXPORTS" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMMON_EXPORTS" /D "_CHINAVERSION" /FR /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../Bin/Common.dll" /implib:"Lib/Common.lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /nodefaultlib:"MSVCRT.lib" /out:"C:\DeicideOnline\Common.dll" /implib:"Lib/Common.lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Common - Win32 Release"
# Name "Common - Win32 Debug"
# Name "Common - Win32 Debug_c"
# Name "Common - Win32 Release_c"
# Begin Group "Inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inc\AABB.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Archive.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Common.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Component.h
# End Source File
# Begin Source File

SOURCE=.\Inc\CriticalSection.h
# End Source File
# Begin Source File

SOURCE=.\Inc\DebugHelper.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Exception.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Frustum.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Handle.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Log.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Math.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Matrix4.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Memory.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Package.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Plane.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Quaternion.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\Inc\StringTable.h

!IF  "$(CFG)" == "Common - Win32 Release"

!ELSEIF  "$(CFG)" == "Common - Win32 Debug"

!ELSEIF  "$(CFG)" == "Common - Win32 Debug_c"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Common - Win32 Release_c"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inc\StringTable_c.h

!IF  "$(CFG)" == "Common - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Common - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Common - Win32 Debug_c"

!ELSEIF  "$(CFG)" == "Common - Win32 Release_c"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Inc\Thread.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Triangle.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Vector2.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Vector3.h
# End Source File
# Begin Source File

SOURCE=.\Inc\WTM.H
# End Source File
# End Group
# Begin Group "Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\AABB.cpp
DEP_CPP_AABB_=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Archive.cpp
DEP_CPP_ARCHI=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Component.cpp
DEP_CPP_COMPO=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\DebugHelper.cpp
DEP_CPP_DEBUG=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Exception.cpp
DEP_CPP_EXCEP=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Frustum.cpp
DEP_CPP_FRUST=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Math.cpp
DEP_CPP_MATH_=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Matrix4.cpp
DEP_CPP_MATRI=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Memory.cpp
DEP_CPP_MEMOR=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Plane.cpp
DEP_CPP_PLANE=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Quaternion.cpp
DEP_CPP_QUATE=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Thread.cpp
DEP_CPP_THREA=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Triangle.cpp
DEP_CPP_TRIAN=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Vector3.cpp
DEP_CPP_VECTO=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\WTM.CPP
DEP_CPP_WTM_C=\
	".\Inc\AABB.h"\
	".\Inc\Archive.h"\
	".\Inc\Common.h"\
	".\Inc\Component.h"\
	".\Inc\CriticalSection.h"\
	".\Inc\DebugHelper.h"\
	".\Inc\Exception.h"\
	".\Inc\Frustum.h"\
	".\Inc\Handle.h"\
	".\Inc\Log.h"\
	".\Inc\Matrix4.h"\
	".\Inc\Package.h"\
	".\Inc\Plane.h"\
	".\Inc\Quaternion.h"\
	".\Inc\Singleton.h"\
	".\Inc\StringTable.h"\
	".\Inc\StringTable_c.h"\
	".\Inc\Thread.h"\
	".\Inc\Triangle.h"\
	".\Inc\Vector2.h"\
	".\Inc\Vector3.h"\
	".\Inc\WTM.H"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# End Group
# End Target
# End Project
