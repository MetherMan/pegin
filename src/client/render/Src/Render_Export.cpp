/**
 * 렌더 컴포넌트 익스포트 정의 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-08-03 10:01p $
 * @version	$Revision: 24 $
 *
 * <HR>
 * $Log: /Render/Src/Render_Export.cpp $
 * 
 * 24    04-08-03 10:01p Paran
 * 
 * 23    04-05-29 10:06p Paran
 * 
 * 22    04-04-29 5:30p Paran
 * MultiSprite 추가
 * 
 * 19    03-10-15 6:42p Kjmgo			라이트 관리자 추가 ^-^ 
 * 
 * 18    03-10-15 4:10p Kjmgo			지형 데칼 인터페이스 작업 ( 아직 미작업 )
 * 
 * 17    03-10-12 9:42p Kjmgo			SkyPlane 추가
 * 
 * 16    03-08-29 9:19a Kjmgo			Animation 바꿈
 * 
 * 15    03-08-04 5:15p Paran			W3DRectangle  추가
 * 
 * 13    03-06-12 5:25p Kjmgo			Plane 추가
 * 
 * <HR>
 *
 * @file	Render_Export.cpp
 */


#include "Render_Export.h"


DECLARE_COMPONENT( Scene )
DECLARE_COMPONENT( Font )
DECLARE_COMPONENT( Light )
DECLARE_COMPONENT( LightMgr )
DECLARE_COMPONENT( Terrain )
DECLARE_COMPONENT( Water )
DECLARE_COMPONENT( Model )
DECLARE_COMPONENT( Shadow )
DECLARE_COMPONENT( Particles )
DECLARE_COMPONENT( ParticleEx )
DECLARE_COMPONENT( SpecialEffect )
DECLARE_COMPONENT( TraceEffect )
DECLARE_COMPONENT( Billboard )
DECLARE_COMPONENT( Sprite )
DECLARE_COMPONENT( UIGraphic )
DECLARE_COMPONENT( MultiSprite )
DECLARE_COMPONENT( Plane )
DECLARE_COMPONENT( Block )
DECLARE_COMPONENT( Rectangle )
DECLARE_COMPONENT( Animation )
DECLARE_COMPONENT( AnimationMgr )
DECLARE_COMPONENT( Decal )
DECLARE_COMPONENT( Sky )



BEGIN_PACKAGE_LIST
	COMPONENT( Scene )
	COMPONENT( Font )
	COMPONENT( Light )
	COMPONENT( LightMgr )
	COMPONENT( Terrain )
	COMPONENT( Water )
	COMPONENT( Model )
	COMPONENT( Shadow )
	COMPONENT( Particles )
	COMPONENT( ParticleEx )
	COMPONENT( SpecialEffect )
	COMPONENT( TraceEffect )
	COMPONENT( Billboard )
	COMPONENT( Sprite )
	COMPONENT( UIGraphic )
	COMPONENT( MultiSprite )
	COMPONENT( Plane )
	COMPONENT( Block )
	COMPONENT( Rectangle )
	COMPONENT( Animation )
	COMPONENT( AnimationMgr )
	COMPONENT( Decal )
	COMPONENT( Sky )
END_PACKAGE_LIST
