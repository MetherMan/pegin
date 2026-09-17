/**
 * 엔진 통합 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-03-10 6:17p $
 * @version	$Revision: 21 $
 *
 * <HR>
 * $Log: /Engine/Inc/Engine.h $
 * 
 * 21    04-03-10 6:17p Paran
 * 기능조물 추가
 * 
 * 20    03-06-13 3:02a Kjmgo
 * 마법 체계 뒤엎음
 * 
 * 19    03-05-17 12:04a Paran
 * GUI 분리.
 * 
 * 18    03-05-05 7:58p Kjmgo
 * 
 * 17    03-05-03 11:23a Ahastudio
 *
 * 15    03-03-20 10:13p Paran			매직 정보 관리 부분 추가.
 *
 * 12    03-02-22 10:07a Ahastudio		Camera 컴포넌트 추가.
 * <HR>
 *
 * @file	Engine.h
 */


#ifndef __Wind3D_Engine_Header__
#define __Wind3D_Engine_Header__


#include "../../Common/Inc/Common.h"
#include "../../Render/Inc/Render.h"
#include "../../Sound/Inc/Sound.h"
#include "../../Media/Inc/Media.h"


class	IW3DInput;
class	IW3DWorld;
class	IW3DCamera;
class	IW3DCreature;
class	IW3DItem;
class	IW3DMagic;
class	IW3DMagicMgr;
class	IW3DMonsterInfo;
class	IW3DItemInfoMgr;
class   IW3DMagicInfoMgr;
class	IW3DEffectObject;
class	IW3DFuncEntity;


#include "IW3DInput.h"
#include "IW3DWorld.h"
#include "IW3DCamera.h"
#include "IW3DCreature.h"
#include "IW3DItem.h"
#include "IW3DMagic.h"
#include "IW3DMonsterInfo.h"
#include "IW3DItemInfoMgr.h"
#include "IW3DEffectObject.h"
#include "IW3DFuncEntity.h"


#endif // #ifndef __Wind3D_Engine_Header__
