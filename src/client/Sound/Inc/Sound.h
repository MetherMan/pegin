/**
 * 사운드 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 02-12-12 4:15p $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /Sound/Inc/Sound.h $
 * 
 * 9     02-12-12 4:15p Sckyj
 * 2D, 3D를 분리
 * 
 * 8     03-11-05 11:37a Sckyj
 * Ambient BGM관련 코드 추가(1차완료)
 * 
 * 7     03-11-04 10:43a Sckyj
 * SoundPlay 함수 수정( Reverb control을 IW3DSoundObject 안으로 넣음 )
 * SND_RANDOM 추가
 * 
 * 6     03-11-01 3:41p Sckyj
 * soundobject Update추가( 사운드 재생에 방향부여 )
 * 
 * 5     03-09-06 2:13p Sckyj
 * 
 * 4     03-03-25 7:15a Ahastudio
 * <HR>
 *
 * @file	Sound.h
 */


#ifndef __Wind3D_Sound_Header__
#define __Wind3D_Sound_Header__


#include "../../Common/Inc/Common.h"

class	IW3DSoundMgr;
class	IW3DSoundObject2D;
class	IW3DSoundObject3D;
class	IW3DMediaPlayer;

#include "IW3DSoundMgr.h"
#include "IW3DSoundObject.h"
#include "IW3DMediaPlayer.h"

#endif // #ifndef __Wind3D_Sound_Header__
