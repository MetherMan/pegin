# Microsoft Developer Studio Project File - Name="Render" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Render - Win32 Debug_c
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Render.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Render.mak" CFG="Render - Win32 Debug_c"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Render - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Render - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Render - Win32 Debug_c" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Render - Win32 Release_c" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Render", JCAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Render - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Build/Render___Win32_Release"
# PROP Intermediate_Dir "../Build/Render___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RENDER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "_USRDLL" /D "RENDER_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHINAVERSION" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"C:\Laqia Client\KOR\Render.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Render - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Build/Render___Win32_Debug"
# PROP Intermediate_Dir "../Build/Render___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RENDER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_USRDLL" /D "RENDER_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBCMTD.lib" /out:"D:\Laqia Client\ENG\Render.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Render - Win32 Debug_c"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Render___Win32_Debug_c"
# PROP BASE Intermediate_Dir "Render___Win32_Debug_c"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Build/Render___Win32_Debug_c"
# PROP Intermediate_Dir "../Build/Render___Win32_Debug_c"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RENDER_EXPORTS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RENDER_EXPORTS" /D "_CHINAVERSION" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../Bin/Render.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBCMTD.lib" /out:"C:\DeicideOnline\Render.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Render - Win32 Release_c"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Render___Win32_Release_c"
# PROP BASE Intermediate_Dir "Render___Win32_Release_c"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Build/Render___Win32_Release_c"
# PROP Intermediate_Dir "../Build/Render___Win32_Release_c"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RENDER_EXPORTS" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RENDER_EXPORTS" /D "_CHINAVERSION" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../Bin/Render.dll"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /nodefaultlib:"MSVCRT.LIB" /out:"C:\DeicideOnline\Render.dll"
# SUBTRACT LINK32 /pdb:none /incremental:yes /debug

!ENDIF 

# Begin Target

# Name "Render - Win32 Release"
# Name "Render - Win32 Debug"
# Name "Render - Win32 Debug_c"
# Name "Render - Win32 Release_c"
# Begin Group "Src"

# PROP Default_Filter ""
# Begin Group "Terrain"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Block.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Block.h
# End Source File
# Begin Source File

SOURCE=.\Src\TerrainPick.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DTerrain.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DTerrain.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\Animation.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Animation.h
# End Source File
# Begin Source File

SOURCE=.\Src\GetDXVer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IndexBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Src\LensFlare.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\LensFlare.h
# End Source File
# Begin Source File

SOURCE=.\Src\Mesh.h
# End Source File
# Begin Source File

SOURCE=.\Src\ProjectiveShadow.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ProjectiveShadow.h
# End Source File
# Begin Source File

SOURCE=.\Src\Render_Export.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Render_Export.h
# End Source File
# Begin Source File

SOURCE=.\Src\Sky.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\SpecialEffectMesh.h
# End Source File
# Begin Source File

SOURCE=.\Src\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Texture.h
# End Source File
# Begin Source File

SOURCE=.\Src\VertexBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Src\W3DAnimationMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DBillboard.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DDecal.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DFont.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DLight.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DModel.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DMultiSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DParticleEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DParticles.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DScene.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DShadow.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DSpecialEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DTraceEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DUIGraphic.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\W3DWater.cpp
# End Source File
# End Group
# Begin Group "Inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inc\IW3DAnimationMgr.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DBillboard.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DDecal.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DFont.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DLight.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DModel.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DMultiSprite.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DParticleEx.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DParticles.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DPlane.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DRectangle.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DScene.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DShadow.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DSky.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DSpecialEffect.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DSprite.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DTerrain.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DTraceEffect.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DUIGraphic.h
# End Source File
# Begin Source File

SOURCE=.\Inc\IW3DWater.h
# End Source File
# Begin Source File

SOURCE=.\Inc\Render.h
# End Source File
# End Group
# End Target
# End Project
