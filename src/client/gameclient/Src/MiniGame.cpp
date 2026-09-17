/**
 * 게임 패킷 처리.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	sckyj
 * @date	$Date: 04-06-04 3:53p $
 * @version	$Revision: 16 $
 *
 * <HR>
 * $Log: /GameClient/Src/MiniGame.cpp $
 * 
 * 16    04-06-04 3:53p Paran2
 * 
 * 15    04-04-02 9:51a Sckyj
 * 
 * 14    04-03-30 2:10p Sckyj
 * 
 * 13    04-03-29 6:18p Sckyj
 * 
 * 12    04-03-28 8:29p Sckyj
 * 
 * 11    04-03-27 1:45p Sckyj
 * 낚시 게임 - 찌에니메이션 완료
 * 
 * 10    04-03-27 10:38a Sckyj
 * 
 * 9     04-03-27 10:37a Sckyj
 * 
 * 8     04-03-27 10:29a Sckyj
 * 
 * 7     04-03-26 7:18p Sckyj
 * 
 * 6     04-03-26 4:45p Sckyj
 * 
 * 5     04-03-26 1:53p Sckyj
 * 
 * 4     04-03-24 5:11p Sckyj
 * 
 * 3     04-03-23 9:40p Sckyj
 * 
 * 2     04-03-19 12:27p Sckyj
 * <HR>
 *
 * @file	MiniGame.cpp
 */

#include "Minigame.h"
#include "Client.h"

#define FLOATBOX_WIDTH		172
#define FLOATBOX_HEIGHT		129
#define FLOATWATER_WIDTH	344
#define FLOATWATER_HEIGHT	65
#define FLOAT_WIDTH			10
#define FLOAT_HEIGHT		129
#define OUTERBOX_WIDTH		180
#define OUTERBOX_HEIGHT		137

#define GAP					4		// 테두리와 그림사이의 공간
#define GAP2				81		// 낚시찌가 가운데 오도록 하는 gap

#define UI_BAR_H_800		50		// UI Box의 offset
#define UI_BAR_H_1024		50

#define PATTERN_BASE			0	// 기본 패턴
#define PATTERN_1				1	// 획득 패턴 1
#define PATTERN_2				2
#define PATTERN_3				3
#define PATTERN_4				4
#define PATTERN_5				5
#define PATTERN_6				6	// 페인팅 패턴 1
#define PATTERN_7				7
#define PATTERN_HOOK			8

#define MAX_PATTERNCNT			9
#define MAX_PATTERNNUM			5

#define TICK_MOUSEDRAG			1000

const int PATTERNNUM[MAX_PATTERNCNT][MAX_PATTERNNUM] =
{
	{ 3, 4, 5, 4, 3 },				// 기본패턴
	{ 3, 4, 5, 9, 10 },				// 패턴 1
	{ 3, 6, 4, 6, 8 },
	{ 3, 2, 1, 8, 3 },
	{ 3, 2, 5, 3, 8 },
	{ 3, 1, 2, 1, 9 },
	{ 3, 4, 3, 4, 5 },				// 페인팅 패턴
	{ 3, 4, 5, 6, 7 },
	{ 2, 13, 11, 5, 3 }				// 후킹 패턴
};

#define MAX_CALNUM				10

#define SKILL_SHORT				0
#define SKILL_LONG				1
#define SKILL_WHITE				2
#define SKILL_BLACK				3


/**
 * 생성자
 */ 
CMiniGame::CMiniGame()
:	m_pWorld		( NULL ),
	m_pBox			( NULL ),	
	m_pWater		( NULL ),
	m_pWater2		( NULL ),
	m_pFloat		( NULL ),
//	m_bUse			( FALSE ),
	m_bGameStart	( FALSE ),
	m_bHookTiming	( FALSE ),
	m_btPattern		( 0 ),
	m_btAdvancePattern( 0 ),
	m_nCurrIndex	( 0 ),	
	m_dwTick		( 0 ),	
	m_dwMouseTick	( 0 ),
	m_nWaterX		( 0 ),
	m_nWater2X		( 0 ),	
	m_nFloatY		( 0 ),
	m_nCalCnt		( 0 ),
	m_bMouseLPressed( FALSE ),	
	m_bDragged		( FALSE )
{
	m_vPick			= Vector3( 0.0f, 0.0f, 0.0f );	
	m_vMouseLDown	= Vector2( 0.0f, 0.0f );
	m_vMouseLUp		= Vector2( 0.0f, 0.0f );
	memcpy( m_anPattern, PATTERNNUM[PATTERN_BASE], sizeof( int ) * 5 );	
} // CMiniGame::CMiniGame


/**
 * 소멸자
 */ 
CMiniGame::~CMiniGame()
{
	SAFE_RELEASE( m_pBox );	
	SAFE_RELEASE( m_pWater );
	SAFE_RELEASE( m_pWater2 );
	SAFE_RELEASE( m_pFloat );
} // CMiniGame::~CMiniGame


/**
 * 초기화
 */
void CMiniGame::Init( IW3DWorld* pWorld )
{
	Assert( pWorld );
	m_pWorld = pWorld;

	LoadSprite();
} // CMiniGame::Init

/**
 * 낚시 찌 그림 읽기
 */
void CMiniGame::LoadSprite()
{
	m_pBox = MAKE_COMPONENT( Sprite );
	Assert( m_pBox );

	char		szFile[64];
	strcpy( szFile, "Interface/FishBox.bmp" );	
	if( !m_pBox->Load( szFile, FLOATBOX_WIDTH + GAP * 2, FLOATBOX_HEIGHT + GAP * 2) )
	{
		Assert( !"비트맵 파일 로드 실패" );
		SAFE_RELEASE( m_pBox );
		return ;
	} //if	

	m_pWater = MAKE_COMPONENT( Sprite );
	Assert( m_pWater );

	strcpy( szFile, "Interface/FishWater.tga" );	
	if( !m_pWater->Load( szFile, FLOATWATER_WIDTH, FLOATWATER_HEIGHT ) )
	{
		Assert( !"tga 파일 로드 실패" );
		SAFE_RELEASE( m_pWater );
		return ;
	} //if

	m_pWater2 = MAKE_COMPONENT( Sprite );
	Assert( m_pWater2 );

	strcpy( szFile, "Interface/FishWater2.tga" );	
	if( !m_pWater2->Load( szFile, FLOATWATER_WIDTH, FLOATWATER_HEIGHT ) )
	{
		Assert( !"tga 파일 로드 실패" );
		SAFE_RELEASE( m_pWater2 );
		return ;
	} //if
	
	m_pFloat = MAKE_COMPONENT( Sprite );
	Assert( m_pFloat );

	strcpy( szFile, "Interface/FishFloat.tga" );	
	if( !m_pFloat->Load( szFile, FLOATBOX_WIDTH, FLOATBOX_HEIGHT ) )
	{
		Assert( !"tga 파일 로드 실패" );
		SAFE_RELEASE( m_pFloat );
		return ;
	} //if
} // CMiniGame::LoadFloat()

/**
 * 낚시를 시작하려고 한다.
 *
 * @param	[IN] pPlayer	:	플레이어
 *			[IN] vPick		:	피킹된 좌표
 *
 */
BOOL CMiniGame::TryFishing( IW3DCreature* pPlayer, Vector3& vPick )
{
	Assert( m_pWorld );	

	if( m_bGameStart )											return TRUE;		// 이미 게임중인가

	if( pPlayer->GetState() == STATE_ATTACK || 
		pPlayer->GetState() == STATE_DEAD )						return FALSE;		// 죽었냐

	if( pPlayer->IsVehicleMode() )								return FALSE;		// 말을 탔는가

//	if( !m_pWorld->IsFishingPlace( pPlayer->GetPosition() ) )	return FALSE;		// 낚시터인가

	if( g_pGUI->GetOnItemType( EQUIP_SHORTWEAPON ) != 10385 )	return FALSE;		// 낚시대를 착용하였는가

	if( pPlayer->GetSkill() != SKILL_SHORT )					return FALSE;		// 근거리 스킬중인가( 낚시대를 착용하고 원거리스킬은 경우 방지 )	

	if( ( vPick - pPlayer->GetPosition() ).Length() > 15.0f )	return FALSE;		// 물과 나의 거리측정		
	
	if( vPick.z <= 0 )																// 물을 향하여 피킹을 하였는가
	{
		m_vPick = vPick;
	}
	else
	{
		g_pGUI->ChattingInput( "", STR_FISHHOOK_DROWTOWATER, 0xFF00FF00, 0 );		// 물을 향하여 낚시대를 던져주세요
		return FALSE;
	}	

	CPacket		sendPacket;
	// 낚시 시작 패킷 전송
	sendPacket.Tag( dPACKET_FISHING_ACTION )
		<< ( BYTE )FISHING_ACTION_TRY;				
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	
	// 낚시 위치값 패킷 전송
	sendPacket.Tag( dPACKET_FISHING_POS )
		<< ( int )( vPick.x )
		<< ( int )( vPick.y )
		<< ( int )( vPick.z );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	
	memcpy( m_anPattern, PATTERNNUM[PATTERN_BASE], sizeof( int ) * 5 );	

	return TRUE;
	
} // CMiniGame::TryFishing

/**
 * 게임 시작
 *
 * @param	[IN] nId : 유저 아이디
 *
 */
void CMiniGame::OnGameStart( int nId )
{
	if( nId != g_dwPlayerId )
	{
		return;
	}

	// 방향 수정
	IW3DCreature* pPlayer = m_pWorld->GetCharacter( nId );
	if( !pPlayer )
	{
		Assert( !"캐릭터가 없다" );
		return;
	}

	static Vector3 vDir;
	vDir = m_vPick - pPlayer->GetPosition();

	pPlayer->Stop();
	pPlayer->SetDirection( vDir );

	// 액션 세팅		
	pPlayer->Hook();

	if( nId == g_dwPlayerId )
	{
		m_bGameStart = TRUE;
	}	
} // CMiniGame::OnGameStart()


/**
 * 게임 끝
 *
 * @param	[IN] nId : 유저 아이디
 *
 */
void CMiniGame::OnGameEnd( int nId )
{
	// 게임 종료
	m_bGameStart		= FALSE;
	m_bHookTiming		= FALSE;
	m_bMouseLPressed	= FALSE;
	m_bDragged			= FALSE;
	m_nCurrIndex		= 0;	
	m_nFloatY			= 0;

	// 액션 세팅
	IW3DCreature* pPlayer = m_pWorld->GetCharacter( nId );
	if( !pPlayer )
	{
		Assert( !"캐릭터가 없다" );
		return;
	}

	pPlayer->Stop();
} // CMiniGame::OnGameEnd


/**
 * 패턴 번호 셋팅
 */
void CMiniGame::OnSetPattern( int nId, BYTE btPattern )
{
	Assert( ( PATTERN_1 <= btPattern ) && ( btPattern <= PATTERN_7 ) );	
	if( nId == g_dwPlayerId )
	{
		m_btAdvancePattern = btPattern;
	}	
} // CMiniGame::OnSetPattern


/**
 * 건질 타이밍 설정
 */
void CMiniGame::OnSetHookTiming( BOOL bTrue )
{
	m_bHookTiming = bTrue;
	if( !bTrue )
	{// 낚을 타이밍이 종료되면 기본패턴으로 변경
		ChangePattern( PATTERN_BASE );
	}
} // CMiniGame::OnSetHookTiming

/**
 * 나를 제외한 다른 캐릭터들의 동작을 위한 바라보는 방향값 받기
 */
void CMiniGame::OnFishingPos( int nId, int nX, int nY )
{
	// 나에대한 패킷일 경우는 통과
	if( nId == g_dwPlayerId )
	{
		return;
	}

	IW3DCreature* pPlayer = m_pWorld->GetCharacter( nId );	
	if( !pPlayer )
	{
		Assert( !"캐릭터가 없다" );
		return;
	}

	Vector3 vPos = pPlayer->GetPosition();
	Vector3 vPick = Vector3( ( float )nX, ( float )nY, vPos.z );
	Vector3 vDir = vPick - vPos;

	pPlayer->Stop();
	pPlayer->SetDirection( vDir );
	pPlayer->Hook();

} // CMiniGame::OnFishingPos

/**
 * 마우스 체크
 */ 
void CMiniGame::MouseCheck( DWORD dwTick )
{
	int nX = g_pInput->GetMouseX();
	int nY = g_pInput->GetMouseY();		

	if( m_bMouseLPressed )
	{
		m_dwMouseTick += dwTick;
	}

	if( g_pInput->GetMouseL() && !m_bMouseLPressed )
	{
		m_bMouseLPressed = TRUE;

		m_vMouseLDown.x = nX;
		m_vMouseLDown.y = nY;		
	}
	else if( !g_pInput->GetMouseL() && m_bMouseLPressed )
	{
		m_bMouseLPressed = FALSE;

		m_vMouseLUp.x = nX;
		m_vMouseLUp.y = nY;

		// 마우스 드래그 검사
		if( ( m_vMouseLDown - m_vMouseLUp ).Length()  > 300.0f &&		// 거리
			( m_dwMouseTick + dwTick <= TICK_MOUSEDRAG ) )				// 시간
		{
			g_pGUI->ChattingInput( "", STR_FISHHOOK_HOOKING, 0xFF00FF00, 0 ); // 낚시대를 들어올립니다

			// 후킹 패킷 전송
			CPacket		sendPacket;
			sendPacket.Tag( dPACKET_FISHING_ACTION )
					<< ( BYTE )FISHING_ACTION_HOOKING;
			g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

			// 낚는 패턴으로 변경
			ChangePattern( PATTERN_HOOK );
		}

		m_dwMouseTick = 0;
	}	
} // CMiniGame::MouseCheck

/**
 * 업데이트
 *
 * @paran	[IN]
 *
 */
BOOL CMiniGame::Update( DWORD dwTick )
{
//	if( !m_bUse )			return m_bUse;
	if( !m_bGameStart )		return m_bGameStart;

	MouseCheck( dwTick );

	m_dwTick += dwTick;
	if( m_dwTick <= 33 )
	{
		return m_bGameStart;
	}	
	m_dwTick = 0;

	// 찌 에니메이션을 위한 좌표값 계산
	if( m_btPattern != m_btAdvancePattern )
	{
		ChangePattern( m_btAdvancePattern );
	}
	else
	{
		CalculatePattern();
	}

	return m_bGameStart;
} // CMiniGame::Update

/**
 * 찌 스프라이트 그리기
 */
void CMiniGame::Render()
{
//	if( !m_bUse )			return;
	if( !m_bGameStart )		return;

	static int ui_bar;	
	static int nInnerWidth;	
	static int nInnerHeight;	

	if( g_nWidth == 1024 && g_nHeight == 768 )
	{
		ui_bar = UI_BAR_H_1024;
	}
	else
	{
		ui_bar = UI_BAR_H_800;
	}

	nInnerWidth = OUTERBOX_WIDTH - GAP;
	nInnerHeight = OUTERBOX_HEIGHT - GAP + ui_bar;	

	// 박스 그리기
	m_pBox->Draw( g_nWidth - OUTERBOX_WIDTH,
				  g_nHeight - ( OUTERBOX_HEIGHT + ui_bar ),
				  0,
				  0,
				  OUTERBOX_WIDTH,
				  OUTERBOX_HEIGHT );

	// 물 그리기
	m_pWater->Draw( g_nWidth - nInnerWidth,
					g_nHeight - ( FLOATWATER_HEIGHT +  ui_bar ),
					m_nWaterX,
					0,
					FLOATBOX_WIDTH,
					FLOATWATER_HEIGHT );
	
	m_pWater2->Draw( g_nWidth - nInnerWidth,
					g_nHeight - ( FLOATWATER_HEIGHT +  ui_bar ),
					m_nWater2X,
					0,
					FLOATBOX_WIDTH,
					FLOATWATER_HEIGHT );	

	// 찌 그리기
	m_pFloat->Draw( g_nWidth - nInnerWidth + GAP2,
					g_nHeight - nInnerHeight,
					FLOAT_WIDTH,
					FLOAT_HEIGHT,
					0,
					m_nFloatY,
					FLOAT_WIDTH,
					FLOAT_HEIGHT );
} // CMiniGame::Render


/**
 * 패턴 변경
 */
void CMiniGame::ChangePattern( BYTE btPattern )
{
	memcpy( m_anPattern, PATTERNNUM[btPattern], sizeof( int ) * 5 );

	m_btPattern = btPattern;
	m_btAdvancePattern = btPattern;
	
	m_nCalCnt = 0;
	m_nCurrIndex = 0;
	m_nFloatY = 0;
} // CMiniGame::ChangePattern

/**
 * 패턴에 따른 물, 찌의 좌표값 계신
 */
void CMiniGame::CalculatePattern()
{
	// 물 좌표 계산
	m_nWaterX += 1;	
	if( m_nWaterX >= FLOATBOX_WIDTH )
	{
		m_nWaterX = 0;
	}	

	m_nWater2X -= 1;
	if( m_nWater2X <= 0 )
	{
		m_nWater2X = FLOATBOX_WIDTH;
	}	

	// 찌 좌표 계산
	static int snCurrValue;
	static int snAdvanValue;
	static float sfDiff;	

	snCurrValue = m_anPattern[m_nCurrIndex];
	snAdvanValue = m_anPattern[m_nCurrIndex  + 1];
	sfDiff = ( float )( snCurrValue - snAdvanValue ) / MAX_CALNUM;

	if( m_btPattern == PATTERN_BASE )
	{
		if( sfDiff > 0 )
		{
			m_nFloatY++;
		}
		else
		{
			m_nFloatY--;
		}
	}	
	else
	{
		m_nFloatY += sfDiff * MAX_CALNUM;
	}	

	if( m_nCalCnt == MAX_CALNUM - 1 )
	{
		m_nCalCnt = 0;		
		
		if( m_nCurrIndex == MAX_PATTERNNUM - 2 )
		{
			if( m_btPattern == PATTERN_HOOK )
			{// 후킹이 종료되면 기본패턴으로 변경
				ChangePattern( PATTERN_BASE );				
			}
			m_nCurrIndex = 0;
			m_nFloatY = 0;
		}
		else
		{
			m_nCurrIndex++;
		}
	}
	else
	{
		m_nCalCnt++;		
	}	
} // CMiniGame::CalculatePattern