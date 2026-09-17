/**
 * 미니게임 공용 헤더파일
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	sckyj
 * @date	$Date: 04-04-02 9:51a $
 * @version	$Revision: 12 $
 *
 * <HR>
 * $Log: /GameClient/Src/MiniGame.h $ 
 * 
 * 12    04-04-02 9:51a Sckyj
 * 
 * 11    04-03-29 6:18p Sckyj
 * 
 * 10    04-03-28 8:29p Sckyj
 * 
 * 9     04-03-27 1:45p Sckyj
 * 낚시 게임 - 찌에니메이션 완료
 * 
 * 8     04-03-27 10:37a Sckyj
 * 
 * 7     04-03-27 10:29a Sckyj
 * 
 * 6     04-03-26 7:18p Sckyj
 * 
 * 5     04-03-26 4:45p Sckyj
 * 
 * 4     04-03-26 1:53p Sckyj
 * 
 * 3     04-03-24 5:11p Sckyj
 * 
 * 2     04-03-23 9:40p Sckyj
 * <HR>
 *
 * @file	MiniGame.h
 */

#ifndef __MINIGAME_HEADER__
#define __MINIGAME_HEADER__

#include "Global.h"
#include "Packet.h"
#include "Protocol.h"

#define FISHING_STATUS_START			1	// 낚시 시작	
#define FISHING_STATUS_END				0	// 낚시 끝
#define FISHING_STATUS_HOOKINGSTART		2	// 건지기 타이밍 시작
#define FISHING_STATUS_HOOKINGEND		3	// 건지기 타이밍 끝

#define FISHING_ACTION_TRY				1	// 낚시터에서 낚시대 들고 마우스 우클릭함
#define FISHING_ACTION_HOOKING			2	// 낚시중 마우스 드래그( Hooking )

class CMiniGame
{
public :
	UNDER_MEMORY_MGR( CMiniGame );

	CMiniGame();
	~CMiniGame();

	/**
	 * 미니게임 초기화
	 */
	void					Init( IW3DWorld* pWorld );

	/**
	 * 업데이트
	 */
	BOOL					Update( DWORD dwTick );

	/**
	 * 그림 그리기
	 */
	void					Render();

	/**
	 * 낚시터가 있는지 여부 설정
	 */
//	void					SetUse( BOOL bUse ) { m_bUse = bUse; }

	/**
	 * 낚시터 있는지 얻기
	 */
//	BOOL					GetUse() { return m_bUse; }
	
	/**
	 * 현재 낚시를 시작했는가 여부 얻기
	 */
	BOOL					GetGameStart() { return m_bGameStart; }
	
	/**
	 * 낚시를 시작한다.
	 */
	BOOL					TryFishing( IW3DCreature* pPlayer, Vector3& vPos );

	/**
	 * 낚시 게임 마우스 체크
	 */
	void					MouseCheck( DWORD dwTick );

	/**
	 * 미니게임 처리함수
	 */
	void					OnGameStart( int nId );					///< 낚시 시작
	void					OnGameEnd( int nId );					///< 낚시 끝
	void					OnSetPattern( int nId, BYTE btPattern);	///< 패턴 설정
	void					OnSetHookTiming( BOOL bTrue );			///< 건질 타이밍 설정
	void					OnFishingPos( int nId, int nX, int nY );///< 위치값

private :

	void					LoadSprite();
	void					ChangePattern( BYTE btPattern );		///< 패턴 변경
	void					CalculatePattern();						///< 찌 좌표 계산

	IW3DWorld*				m_pWorld;								///< 월드 객체

	IW3DSprite*				m_pBox;									///< 배경 박스	
	IW3DSprite*				m_pWater;								///< 물 그림
	IW3DSprite*				m_pWater2;								///< 물 그림
	IW3DSprite*				m_pFloat;								///< 찌 그림
	
//	BOOL					m_bUse;									///< 낚시터가 있는 맵인지 여부
	BOOL					m_bGameStart;							///< 게임을 시작하였는가.
	BOOL					m_bHookTiming;							///< 건질 타이밍인가

	BYTE					m_btPattern;							///< 패턴 번호
	BYTE					m_btAdvancePattern;						///< 다음 패턴 번호
	int						m_anPattern[5];							///< 적용될 패턴
	int						m_nCurrIndex;							///< 패턴 배열에서 사용중인 인덱스
	int						m_nCalCnt;								///< 찌가 변하는 보간 횟수	
	
	Vector3					m_vPick;								///< 피킹한 좌표
	Vector2					m_vMouseLDown;							///< 마우스 좌표
	Vector2					m_vMouseLUp;	
	BOOL					m_bMouseLPressed;						///< 마우스 왼쪽 버튼 눌렸는가
	BOOL					m_bDragged;								///< 마우스 드래그 하였는가

	DWORD					m_dwTick;								///< 게임 틱
	DWORD					m_dwMouseTick;							///< 마우스 더블클릭 틱

	int						m_nWaterX;								///< 물 좌표		
	int						m_nWater2X;									
	int						m_nFloatY;								///< 찌 좌표	
};

#endif // #ifndef __MINIGAME_HEADER__