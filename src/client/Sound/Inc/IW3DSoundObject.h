/**
 * 사운드 오브젝트.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 02-12-12 4:15p $
 * @version	$Revision: 16 $
 *
 * <HR>
 * $Log: /Sound/Inc/IW3DSoundObject.h $
 * 
 * 16    02-12-12 4:15p Sckyj
 * 2D, 3D를 분리
 * 
 * 15    03-12-16 5:36p Sckyj
 * 3D 사운드 재생시 들리는 최대거리를 입력할수 있도록 수정
 * 
 * 14    03-11-28 10:16a Sckyj
 * Map, Area 변경에 따른 특징적인 잔향( Reverb type )을 설정할수 있도록
 * 기능추가
 * 
 * 13    03-11-05 11:37a Sckyj
 * Ambient BGM관련 코드 추가(1차완료)
 * 
 * 12    03-11-01 3:41p Sckyj
 * soundobject Update추가( 사운드 재생에 방향부여 )
 * 
 * 11    03-10-08 11:20a Sckyj
 * 
 * 10    03-09-17 10:39a Sckyj
 * 
 * 9     03-09-16 10:41p Sckyj
 * 
 * 8     03-09-06 2:57p Sckyj
 * 
 * 7     03-07-23 3:26p Kjmgo
 * 
 * 6     03-06-26 12:42a Paran
 * 2D Loop 추가.
 * 
 * 5     03-06-01 3:15p Kjmgo
 * <HR>
 *
 * @file	IW3DSoundObject.h
 */


#ifndef __Wind3D_Sound_IW3DSoundObject_Header__
#define __Wind3D_Sound_IW3DSoundObject_Header__

// 마법 사운드 타입
#define	SND_NORMAL			0	// SND_CENTER와 같은 효과
#define SND_CIRCLE			1	// Right turn
#define SND_CROSS1			2	// cross from right to left speaker
#define SND_CROSS2			3	// cross from front to rear speaker
#define SND_FR				4	// front right
#define SND_FL				5	// front left
#define SND_RR				6	// rear right
#define SND_RL				7	// rear left
#define SND_RANDOM			100	// random position play

// 잔향 타입
#define REVERB_GENERIC		0	// ENVIRONMENT_GENERIC
#define REVERB_ROOM			2	// ENVIRONMENT_ROOM		
#define REVERB_BATHROOM		3	// ENVIRONMENT_BATHROOM
#define REVERB_CONCERTHALL	7	// ENVIRONMENT_CONCERTHALL
#define REVERB_CAVE			8	// ENVIRONMENT_CAVE
#define REVERB_CITY			16	// ENVIRONMENT_CITY
#define REVERB_MOUNTAINS	17	// ENVIRONMENT_MOUNTAINS
#define REVERB_UNDERWATER	22	// ENVIRONMENT_UNDERWATER

// Reverb Type
#define ENV_GENERIC			0
#define ENV_CONCERTHALL		7
#define ENV_CAVE			8
#define ENV_MOUNTAINS		17

#define MAX_DISTANCE		30
#define MIN_DISTANCE		2.0

/**
 * 사운드 객체 인터페이스.
 * @see		IW3DSoundMgr
 * @ingroup		Sound
 */
class IW3DSoundObject2D : public IComponent
{
public:
	/**
	 * 사운드 출력.
	 *
	 * @param	szName		[IN] 사운드 파일 이름.
	 */
	virtual void		Play( const char* szName, const WORD wLoop = 1 ) = 0;

	/**
	 * 사운드 업데이트
	 * CirclePlay등을 수행시 Update를 통해 음원의 위치를 갱신한다.
	 */
	virtual void		Update( DWORD dwTick ) = 0;

	/**
	 * 사운드 정지.
	 */
	virtual void		Stop() = 0;

	/**
	 * 끝났는가?
	 */
	virtual BOOL		IsEnd() = 0;

}; //class IW3DSoundObject


/**
 * 사운드 객체 인터페이스.
 * @see		IW3DSoundMgr
 * @ingroup		Sound
 */
class IW3DSoundObject3D : public IComponent
{
public:

	/**
	 * 사운드 출력.
	 *
	 * @param	szName		[IN] 사운드 파일 이름.
	 */
	virtual void		Play3D( const char* szName, const WORD wLoop = 1, const int nDis = MAX_DISTANCE ) = 0;
	
	/**
	 * 사운드 업데이트
	 * CirclePlay등을 수행시 Update를 통해 음원의 위치를 갱신한다.
	 */
	virtual void		Update3D( DWORD dwTick ) = 0;

	/**
	 * 사운드 플레이 스타일을 지정한다.
	 */
	virtual void		Set3DStyle( BYTE btStyle ) = 0;

	/**
	 * 반향을 설정한다
	 */ 
	virtual void		SetReverb( BYTE btReverb, float fLevel ) = 0;

	/**
	 * 사운드 정지.
	 */
	virtual void		Stop3D() = 0;

	/**
	 * 사운드 위치 지정.
	 *
	 * @param	vPos		[IN] 사운드 위치.
	 */
	virtual void		SetPosition( const Vector3& vPos ) = 0;

	/**
	 * 사운드 위치를 얻어옴.
	 *
	 * @return	사운드 위치.
	 */
	virtual Vector3		GetPosition() = 0;
	
	/**
	 * 끝났는가?
	 */
	virtual BOOL		IsEnd() = 0;

}; //class IW3DSoundObject



#endif // #ifndef __Wind3D_Sound_IW3DSoundObject_Header__
