/**
 * 미디어 플레이어.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-06-23 4:50p $
 * @version	$Revision: 23 $
 *
 * <HR>
 * $Log: /Sound/Inc/IW3DMediaPlayer.h $
 * 
 * 23    04-06-23 4:50p Sckyj
 * 
 * 22    04-01-05 3:44p Sckyj
 * BGM_EVENT define 변경
 * 
 * 21    03-12-26 3:18p Sckyj
 * 
 * 20    03-12-17 11:34a Sckyj
 * 
 * 19    03-12-15 12:42p Sckyj
 * 작업중
 * 
 * 18    03-12-15 11:42a Sckyj
 * Reverb 작업중
 * 
 * 17    03-12-15 11:21a Sckyj
 * 3차 도중.-_-
 * 
 * 16    03-12-14 6:00p Sckyj
 * 2차 완료
 * 
 * 15    03-12-14 3:11p Sckyj
 * MediaPlayer 개조 1차작업
 * 
 * 14    03-12-05 10:25a Sckyj
 * 
 * 13    03-12-01 11:08a Sckyj
 * #define MAXBGM 위치변경
 * 
 * 12    03-11-28 10:47p Sckyj
 * 뒤엎어따.. 다 빼고 다넣고, 이제 아무거나 Loopin control 가능하다
 * 
 * 11    03-11-28 10:16a Sckyj
 * Map, Area 변경에 따른 특징적인 잔향( Reverb type )을 설정할수 있도록
 * 기능추가
 * 
 * 10    03-11-26 3:51p Sckyj
 * Ambient BGM 시간대별 재생기능 추가
 * 
 * 9     03-11-08 12:02p Sckyj
 * Ambient BGM을 cross play 되도록 기능 추가
 * 
 * 8     03-11-05 11:37a Sckyj
 * Ambient BGM관련 코드 추가(1차완료)
 * 
 * 7     03-10-04 6:12a Sckyj
 * 
 * 6     03-10-03 9:09p Sckyj
 * 
 * 5     03-10-03 5:52p Sckyj
 * CBGMStream을 새로 만들었음
 * 
 * 4     03-10-02 1:33a Sckyj
 * 
 * 3     03-09-30 4:11p Sckyj
 * 
 * 2     03-09-16 4:53p Sckyj
 * 
 * 1     03-09-06 1:45p Sckyj
 * 
 * 4     03-09-03 6:21p Sckyj
 * 
 * 3     03-08-30 2:52p Sckyj
 * 
 * 2     03-03-25 6:15a Ahastudio
 * <HR>
 *
 * @file	IW3DMediaPlayer.h
 */


#ifndef __Wind3D_Media_IW3DMediaPlayer_Header__
#define __Wind3D_Media_IW3DMediaPlayer_Header__

#define MAXBGM				3

// BGM을 play하는 방식
#define BGM_PLAY_EVENT		0		// event play
#define BGM_PLAY_CROSS		1		// cross play
#define	BGM_PLAY_FADING		2		// just fade out
#define BGM_PLAY_AMBIENT	3		// normal looping play

// BGM Type
#define BGM_TYPE_EVENT		0		
#define BGM_TYPE_MAP		1
#define BGM_TYPE_AREA		2
#define BGM_TYPE_AMBIENT	3

// BGM number
#define BGM0			0
#define BGM1			1
#define BGM2			2

// Ambient BGM - bird or waterfall sound
#define BGMAMBIENT0		0
#define BGMAMBIENT1		1		

// event BGM type
#define	BGM_EVENT_INTRO				1		// Intro로고	Intro.mp3
#define	BGM_EVENT_LOGIN				2		// 로그인		Login.mp3
#define	BGM_EVENT_MAKECHARACTER		3		// 캐릭생성		CharacterMake.mp3
#define	BGM_EVENT_SELECTCHARACTER	4		// 캐릭선택		CharacterSelect.mp3
#define	BGM_EVENT_CASTLEWAR			5		// 공성도중
#define	BGM_EVENT6					6		// 공성성공
#define	BGM_EVENT7					7		// 공성실패

struct sBGM
{	
	char		szBGMName[60];		// sBGM 이름
	char		szBGMType[60];		// sBGM type
}; //sBGM

struct sBGMInfo
{
	char		szInfo[50];			// Type별 Info( Event-EventInfo, 그외-ReverbInfo )
	int			nBGMNo;				// sBGM 번호
	BOOL		bLoop;				// Looping Info
	float		fVolume;			// Event sBGM 고정 volume
	int			nTime[2];			// Ambient sBGM 시간대별 재생
}; //sBGMInfo

/**
 * 미디어 플레이어.
 * @ingroup		Media
 */
class IW3DMediaPlayer : public IComponent
{
public:
	/**
	 * 미디어 플레이어를 초기화한다.
	 */
	virtual BOOL		Init( HWND hWnd ) = 0;
	virtual BOOL		Clear() = 0;

	/**
	 * BGB을 재생한다.
	 */
	virtual BOOL		Play( int nBGMType, int nTypeNum ) = 0;
	virtual BOOL		PlayArea( int nAreaCount, int* nAreaType ) = 0;

	/**
	 * 모든 재생을 중단한다.
	 */
	virtual void		StopAll() = 0;

	/**
	 * 미디어 플레이어 업데이트.	 
	 */
	virtual void		Update( DWORD dwTick, int nCurrTime ) = 0;

	/**
	 * 음량 조절
	 * 0.0f ~ 0.5f
	 */
	virtual BOOL		SetVolume( float fVolume ) = 0;

	/**
	 * 음량 얻기
	 */
	virtual float		GetVolume() = 0;	

	/**
	 * Fade Out하기 전의 마지막 Volume 값을 저장하여 놓는다.
	 */
	virtual void		SetLastVolume( float fVolume ) = 0;

	/**
	 * SetLastVolume에서 설정한 마지막 Volume을 가져온다.
	 */
	virtual float		GetLastVolume() = 0;
	
	/**	 
	 * BGM Name을 반환한다.
	 */
	virtual char*		GetBGMName(	int nBGMNum ) = 0;	
	
	/**
	 * Reverb type을 반환한다.
	 */
	virtual char*		GetAreaBGMReverb( int nArea ) = 0;
	virtual char*		GetMapBGMReverb( int nMap ) = 0;
	
}; //class IW3DMediaPlayer

#endif // #ifndef __Wind3D_Media_IW3DMediaPlayer_Header__
