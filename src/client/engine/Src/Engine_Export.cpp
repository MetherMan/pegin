/**
 * Engine.DLL 익스포트 설정 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-03-10 6:17p $
 * @version	$Revision: 26 $
 *
 * <HR>
 * $Log: /Engine/Src/Engine_Export.cpp $
 * 
 * 26    04-03-10 6:17p Paran
 * 기능조물 추가
 * 
 * 25    03-08-07 5:39p Paran
 * 
 * 24    03-06-13 3:02a Kjmgo
 * 마법 체계 뒤엎음
 * 
 * 23    03-05-17 12:04a Paran
 * GUI 분리.
 * 
 * 22    03-05-10 12:38p Ahastudio
 * Character, Monster, NPC 제거.
 *
 * 18    03-04-15 1:27a Paran			NPC 컴포넌트 추가.
 *
 * 13    03-02-28 11:01a Paran			MonsterInfo 컴포넌트 추가.
 *
 * 12    03-02-22 10:07a Ahastudio		Camera 컴포넌트 추가.
 *
 * 11    03-02-22 8:07a Kjmgo			Magic 컴포넌트 추가.
 * <HR>
 *
 * @file	Engine_Export.cpp
 */


#include "Engine_Export.h"


DECLARE_COMPONENT( Input )
DECLARE_COMPONENT( World )
DECLARE_COMPONENT( Camera )
DECLARE_COMPONENT( Item )
DECLARE_COMPONENT( Magic )
DECLARE_COMPONENT( MagicMgr )
DECLARE_COMPONENT( ItemInfoMgr )
DECLARE_COMPONENT( EffectObject )
DECLARE_COMPONENT( FuncEntity )


BEGIN_PACKAGE_LIST
	COMPONENT( Input )
	COMPONENT( World )
	COMPONENT( Camera )
	COMPONENT( Item )
	COMPONENT( Magic )
	COMPONENT( MagicMgr )
	COMPONENT( ItemInfoMgr )
	COMPONENT( EffectObject )
	COMPONENT( FuncEntity )
END_PACKAGE_LIST
