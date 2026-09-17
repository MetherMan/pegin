/**
 * 사운드 관리자.
 * IW3DSoundObject를 사용하기 위해 사용되며,
 * 청취자 관련 세팅을 위해서도 사용된다.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-15 5:31p $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /Sound/Inc/IW3DSoundMgr.h $
 * 
 * 11    03-12-15 5:31p Sckyj
 * MultiSpeaker관련 코드 추가
 * 
 * 10    03-11-28 10:16a Sckyj
 * Map, Area 변경에 따른 특징적인 잔향( Reverb type )을 설정할수 있도록
 * 기능추가
 * 
 * 9     03-10-08 4:05p Sckyj
 * 
 * 8     03-09-17 10:39a Sckyj
 * 
 * 7     03-06-01 3:15p Kjmgo
 * 
 * 6     03-03-25 7:15a Ahastudio
 * <HR>
 *
 * @file	IW3DSoundMgr.h
 */


#ifndef __Wind3D_Sound_IW3DSoundMgr_Header__
#define __Wind3D_Sound_IW3DSoundMgr_Header__


/**
 * 사운드 관리자 인터페이스.
 * @see		IW3DSoundObject
 * @ingroup		Sound
 */
class IW3DSoundMgr : public IComponent
{
public:
	/**
	 * 초기화.
	 *
	 * @return		초기화 성공 여부.
	 */
	virtual BOOL		Init() = 0;
	
	/**
	 * Sound Object들의 볼륨을 설정한다.	
	 */
	virtual void		SetVolume( float fVolume ) = 0;

	/**
	 * Sound Object들의 볼륨을 가져온다
	 */
	virtual float		GetVolume() = 0;
	
	/**
	 * map, area등에 설정되어 있는 잔향 type을 설정한다.
	 */
	virtual void		SetReverb( const char* szReverb ) = 0;

	/**
	 * map, area등에서 설정한 잔향 type을 반환한다.
	 */
	virtual BYTE		GetReverb() = 0;

	/**
	 * Listener의 위치를 정한다.
	 *
	 * @param	vPos	[IN] Listener의 위치
	 */
	virtual void		SetPosition( const Vector3& vPos ) = 0;

	/**
	 * Listener의 위치와 방향을 정한다.
	 *
	 */
	virtual void		SetPosition( const Vector3& vPos, const Vector3& vDir ) = 0;

	/**
	 * Listener의 위치를 얻는다.
	 */
	virtual Vector3		GetPosition() = 0;
	
	/**
	 * Interface의 Effect Sound On, Off여부를 반환한다.
	 */
	virtual BOOL		GetOnOff() = 0;

	/**
	 * Interface의 Effect Sound On, Off여부를 설정한다.
	 */
	virtual void		SetOnOff( BOOL bOn ) = 0;
	
	/**
	 * 멀티 스피커 여부를 설정한다.
	 */
	virtual void		SetMultiSpeaker( BOOL bTrue ) = 0;

	/**
	 * 멀티 스피커 여부를 반환한다.
	 */
	virtual BOOL		GetMultiSpeaker() = 0;

}; //class IW3DSoundMgr


#endif // #ifndef __Wind3D_Sound_IW3DSoundMgr_Header__
