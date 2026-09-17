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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHINAVERSION" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"C:\Laqia Client\KOR\DeicideOnline.exe"

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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"C:\Laqia Client\ENG\LaqiaOnline.exe"
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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHINAVERSION" /D "_NOWEBLOGIN" /D "__CHINA_VER__" /D "_UMENOKOJI_MODE_" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"../Bin/LaqiaOnline.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /debug /machine:I386 /nodefaultlib:"libcd.lib" /out:"C:\DeicideOnline\DeicideOnline.exe"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHINAVERSION" /D "_CHINA_BILLING_" /D "__CHINA_VER__" /D "_UMENOKOJI_MODE_" /FR /YX /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../Bin/LaqiaOnline.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"C:\DeicideOnline\DeicideOnline.exe"

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
DEP_CPP_ACTIO=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\ActionMgr.h
# End Source File
# Begin Source File

SOURCE=.\Src\CharacterAction.cpp
DEP_CPP_CHARA=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\CharacterAction.h
# End Source File
# Begin Source File

SOURCE=.\Src\MonsterAction.cpp
DEP_CPP_MONST=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\MonsterAction.h
# End Source File
# Begin Source File

SOURCE=.\Src\VehicleAction.cpp
DEP_CPP_VEHIC=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\VehicleAction.h
# End Source File
# End Group
# Begin Group "Creature"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\BodyMgr.cpp
DEP_CPP_BODYM=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\BodyMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\BodyMgr.h
# End Source File
# Begin Source File

SOURCE=.\Src\Character.cpp
DEP_CPP_CHARAC=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\BodyMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Client.h"\
	".\Src\Creature.h"\
	".\Src\Emblem.h"\
	".\Src\Encrypt.h"\
	".\Src\Game.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\may.h"\
	".\Src\MiniGame.h"\
	".\Src\MonsterAction.h"\
	".\Src\Pet.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\Type.h"\
	".\Src\Vehicle.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Creature.h
# End Source File
# Begin Source File

SOURCE=.\Src\Emblem.h
# End Source File
# Begin Source File

SOURCE=.\Src\Helper.cpp
DEP_CPP_HELPE=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Helper.h
# End Source File
# Begin Source File

SOURCE=.\Src\May.cpp
DEP_CPP_MAY_C=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\may.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\May.h
# End Source File
# Begin Source File

SOURCE=.\Src\Mercenary.cpp
DEP_CPP_MERCE=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Client.h"\
	".\Src\Creature.h"\
	".\Src\Encrypt.h"\
	".\Src\Game.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MiniGame.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\Type.h"\
	".\Src\Vehicle.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Monster.cpp
DEP_CPP_MONSTE=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Client.h"\
	".\Src\Creature.h"\
	".\Src\Encrypt.h"\
	".\Src\Game.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MiniGame.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\Type.h"\
	".\Src\Vehicle.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\NPC.CPP
DEP_CPP_NPC_C=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Client.h"\
	".\Src\Creature.h"\
	".\Src\Encrypt.h"\
	".\Src\Game.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MiniGame.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\Type.h"\
	".\Src\Vehicle.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Pet.cpp
DEP_CPP_PET_C=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\may.h"\
	".\Src\MonsterAction.h"\
	".\Src\Pet.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Pet.h
# End Source File
# Begin Source File

SOURCE=.\Src\SelectCreature.cpp
DEP_CPP_SELEC=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\SelectCreature.h
# End Source File
# Begin Source File

SOURCE=.\Src\Vehicle.cpp
DEP_CPP_VEHICL=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Client.h"\
	".\Src\Encrypt.h"\
	".\Src\Game.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MiniGame.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\Type.h"\
	".\Src\Vehicle.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Vehicle.h
# End Source File
# End Group
# Begin Group "Daum"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\DaumGameParameter.cpp
DEP_CPP_DAUMG=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\DaumGameParameter.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
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
DEP_CPP_MINIG=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Client.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MiniGame.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\MiniGame.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\Game.cpp
DEP_CPP_GAME_=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\BodyMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Client.h"\
	".\Src\Encrypt.h"\
	".\Src\Game.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MiniGame.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\Type.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Game.h
# End Source File
# Begin Source File

SOURCE=.\Src\GameProcessPacket.cpp
DEP_CPP_GAMEP=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Client.h"\
	".\Src\Encrypt.h"\
	".\Src\Game.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MiniGame.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\Type.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Intro.cpp
DEP_CPP_INTRO=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
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
DEP_CPP_LOGIN=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Client.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\Type.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

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
DEP_CPP_SELECT=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Client.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCharProduce.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\Type.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\SelectCharProduce.h
# End Source File
# End Group
# Begin Group "Skill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Skill.cpp
DEP_CPP_SKILL=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Skill.h
# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Client.cpp
DEP_CPP_CLIEN=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Client.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# Begin Source File

SOURCE=.\Src\Client.h
# End Source File
# Begin Source File

SOURCE=.\Src\Encrypt.cpp
DEP_CPP_ENCRY=\
	".\Src\Encrypt.h"\
	
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
DEP_CPP_GLOBA=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\Type.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
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
DEP_CPP_WINMA=\
	"..\Common\Inc\AABB.h"\
	"..\Common\Inc\Archive.h"\
	"..\Common\Inc\Common.h"\
	"..\Common\Inc\Component.h"\
	"..\Common\Inc\CriticalSection.h"\
	"..\Common\Inc\DebugHelper.h"\
	"..\Common\Inc\Exception.h"\
	"..\Common\Inc\Frustum.h"\
	"..\Common\Inc\Handle.h"\
	"..\Common\Inc\Log.h"\
	"..\Common\Inc\Matrix4.h"\
	"..\Common\Inc\Package.h"\
	"..\Common\Inc\Plane.h"\
	"..\Common\Inc\Quaternion.h"\
	"..\Common\Inc\Singleton.h"\
	"..\Common\Inc\StringTable.h"\
	"..\Common\Inc\StringTable_c.h"\
	"..\Common\Inc\Thread.h"\
	"..\Common\Inc\Triangle.h"\
	"..\Common\Inc\Vector2.h"\
	"..\Common\Inc\Vector3.h"\
	"..\Common\Inc\WTM.H"\
	"..\Engine\Inc\Engine.h"\
	"..\Engine\Inc\IW3DCamera.h"\
	"..\Engine\Inc\IW3DCreature.h"\
	"..\Engine\Inc\IW3DEffectObject.h"\
	"..\Engine\Inc\IW3DFuncEntity.h"\
	"..\Engine\Inc\IW3DInput.h"\
	"..\Engine\Inc\IW3DItem.h"\
	"..\Engine\Inc\IW3DItemInfoMgr.h"\
	"..\Engine\Inc\IW3DMagic.h"\
	"..\Engine\Inc\IW3DMonsterInfo.h"\
	"..\Engine\Inc\IW3DWorld.h"\
	"..\Engine\Inc\KeyCode.h"\
	"..\Media\Inc\IW3DVideoPlayer.h"\
	"..\Media\Inc\Media.h"\
	"..\Render\Inc\IW3DAnimationMgr.h"\
	"..\Render\Inc\IW3DBillboard.h"\
	"..\Render\Inc\IW3DDecal.h"\
	"..\Render\Inc\IW3DFont.h"\
	"..\Render\Inc\IW3DLight.h"\
	"..\Render\Inc\IW3DModel.h"\
	"..\Render\Inc\IW3DMultiSprite.h"\
	"..\Render\Inc\IW3DParticleEx.h"\
	"..\Render\Inc\IW3DParticles.h"\
	"..\Render\Inc\IW3DPlane.h"\
	"..\Render\Inc\IW3DRectangle.h"\
	"..\Render\Inc\IW3DScene.h"\
	"..\Render\Inc\IW3DShadow.h"\
	"..\Render\Inc\IW3DSky.h"\
	"..\Render\Inc\IW3DSpecialEffect.h"\
	"..\Render\Inc\IW3DSprite.h"\
	"..\Render\Inc\IW3DTerrain.h"\
	"..\Render\Inc\IW3DTraceEffect.h"\
	"..\Render\Inc\IW3DUIGraphic.h"\
	"..\Render\Inc\IW3DWater.h"\
	"..\Render\Inc\Render.h"\
	"..\Sound\Inc\IW3DMediaPlayer.h"\
	"..\Sound\Inc\IW3DSoundMgr.h"\
	"..\Sound\Inc\IW3DSoundObject.h"\
	"..\Sound\Inc\Sound.h"\
	"..\UInterface\Inc\IW3DNPCScript.h"\
	"..\UInterface\Inc\IW3DUInterface.h"\
	"..\UInterface\Inc\UInterface.h"\
	"..\UInterface\Inc\W3DEventCode.h"\
	".\Src\ActionMgr.h"\
	".\Src\CharacterAction.h"\
	".\Src\Client.h"\
	".\Src\Encrypt.h"\
	".\Src\Global.h"\
	".\Src\Helper.h"\
	".\Src\MonsterAction.h"\
	".\Src\SelectCreature.h"\
	".\Src\Skill.h"\
	".\Src\Sword.h"\
	".\Src\VehicleAction.h"\
	{$(INCLUDE)}"lauxlib.h"\
	{$(INCLUDE)}"lua.h"\
	{$(INCLUDE)}"lualib.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	
# End Source File
# End Group
# Begin Group "Res"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\Deicide.ico
# End Source File
# Begin Source File

SOURCE=.\Res\icon1.ico
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
