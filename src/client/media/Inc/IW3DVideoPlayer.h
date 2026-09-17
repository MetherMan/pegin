/**
 * 비디오 플레이어.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-04-10 5:45a $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /Media/Inc/IW3DVideoPlayer.h $
 * 
 * 4     03-04-10 5:45a Ahastudio
 * <HR>
 *
 * @file	IW3DVideoPlayer.h
 */


#ifndef __Wind3D_Media_IW3DVideoPlayer_Header__
#define __Wind3D_Media_IW3DVideoPlayer_Header__


/**
 * 비디오 플레이어.
 * @ingroup		Media
 */
class IW3DVideoPlayer : public IComponent
{
public:
	/**
	 * 비디오 플레이어를 초기화한다.
	 */
	virtual BOOL	Init( const HWND hWnd ) = 0;

	/**
	 * 비디오를 보여준다.
	 *
	 * @param	szFileName		[IN] 비디오 파일 이름
	 * @param	bLoop			[IN] 반복 여부
	 */
	virtual BOOL	Play( const char* szFileName, const BOOL bLoop = FALSE ) = 0;

	/**
	 * 비디오 보여주기를 멈춘다.
	 */
	virtual void	Stop() = 0;

	/**
	 * 비디오 플레이어 업데이트.
	 * 반복 플레이 등을 처리하기 위해서는 이 함수를 계속해서 호출해야 한다.
	 * 내부적으로 오류가 발생하더라도 특별히 오류값을 돌려주지 않는다.
	 */
	virtual void	Update() = 0;

	/**
	 * 플레이 중인지 얻는다.
	 */
	virtual BOOL	IsPlaying() = 0;
}; //class IW3DVideoPlayer


#endif // #ifndef __Wind3D_Media_IW3DVideoPlayer_Header__
