/**
 * Render Global Header.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-08-03 10:01p $
 * @version	$Revision: 23 $
 *
 * <HR>
 * $Log: /Render/Inc/Render.h $
 * 
 * 23    04-08-03 10:01p Paran
 * 
 * 22    04-05-29 10:06p Paran
 * 
 * 21    04-04-29 5:30p Paran
 * MultiSprite 추가
 * 
 * 18    03-10-15 6:42p Kjmgo			라이트 관리자 추가 ^-^ 
 * 
 * 17    03-10-15 4:10p Kjmgo			지형 데칼 인터페이스 작업 ( 아직 미작업 )
 * 
 * 16    03-10-12 9:42p Kjmgo			SkyPlane 추가
 * 
 * 15    03-08-29 9:19a Kjmgo			Animation 바꿈
 * 
 * 14    03-08-04 5:15p Paran			W3DRectangle  추가
 * 
 * 13    03-06-12 5:25p Kjmgo			Plane 추가
 * 
 * <HR>
 *
 * @file	Render.h
 */


#ifndef __Wind3D_Render_Header__
#define __Wind3D_Render_Header__


#include "../../Common/Inc/Common.h"


class	IW3DScene;
class   IW3DFont;
class	IW3DLight;
class	IW3DLightMgr;
class	IW3DTerrain;
class	IW3DWater;
class	IW3DModel;
class	IW3DShadow;
class	IW3DParticles;
class	IW3DParticleEx;
class   IW3DSpecialEffect;
class   IW3DTraceEffect;
class	IW3DBillboard;
class	IW3DSprite;
class	IW3DUIGraphic;
class	IW3DMultiSprite;
class	IW3DPlane;
class	IW3DRectangle;
class	IW3DAnimation;
class	IW3DAnimationMgr;
class	IW3DSky;
class	IW3DDecal;


#include "IW3DScene.h"
#include "IW3DFont.h"
#include "IW3DLight.h"
#include "IW3DTerrain.h"
#include "IW3DWater.h"
#include "IW3DAnimationMgr.h"
#include "IW3DModel.h"
#include "IW3DShadow.h"
#include "IW3DParticles.h"
#include "IW3DParticleEx.h"
#include "IW3DSpecialEffect.h"
#include "IW3DTraceEffect.h"
#include "IW3DBillboard.h"
#include "IW3DSprite.h"
#include "IW3DUIGraphic.h"
#include "IW3DMultiSprite.h"
#include "IW3DPlane.h"
#include "IW3DRectangle.h"
#include "IW3DSky.h"
#include "IW3DDecal.h"


#endif // #ifndef __Wind3D_Render_Header__
