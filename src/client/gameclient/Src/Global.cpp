/**
 * 전역 소스 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-08-27 8:00p $
 * @version	$Revision: 69 $
 *
 * <HR>
 * $Log: /GameClient/Src/Global.cpp $
 * 
 * 69    04-08-27 8:00p Paran
 * 
 * 60    04-03-15 5:43p Sckyj			운영자 작업 1차완료 - 동작막기 안함
 * 
 * 58    04-03-10 6:17p Paran			기능조물 추가
 * 
 * 54    04-01-13 9:43p Kjmgo			사양 설정 적용
 * 
 * 51    04-01-06 1:58p Kjmgo			공격 매크로 추가
 * 
 * 50    04-01-06 12:01a Kjmgo			공속 추가 
 * 
 * 49    03-12-29 5:20p Kjmgo			레벨이 10이하일때만 메이가 나타나도록 수정
 * 
 * 45    03-12-17 10:04p Kjmgo			Alt키를 누를시, 엠블럼 출력
 * 
 * 44    03-12-17 7:50a Kjmgo			선택 유지 구현 ( 選擇[せんたく] 維持[いじ] )
 * 
 * 42    03-12-04 7:16p Kjmgo			주인공 캐릭터 공속 수정 가능하도록 하는 코드 삽입
 * 
 * 41    03-12-04 10:44a Kjmgo			플레이어의 May의 전역포인터 추가
 * 
 * 40    03-11-15 9:41p Paran			PK 부분 1차 수정
 * 
 * 38    03-10-23 10:04p Paran			공격효과부분 수정.
 * 
 * 36    03-10-01 11:08p Kjmgo			크리처들과의 충돌처리 진행
 * 
 * 35    03-09-29 5:05p Kjmgo			Picking용 Interface Pointer를 CGame 내부에서 전역으로 수정
 * 
 * 32    03-09-12 6:42p Kjmgo			Vehicle 관련 수정 및 추가
 * 
 * 31    03-09-02 5:49a Kjmgo			Monster 추가작업
 * 
 * 30    03-08-30 11:07a Kjmgo			Character Animation Data 읽는 부분 추가
 * 
 * 23    03-05-06 11:51p Ahastudio		Lua 추가.
 * <HR>
 *
 * @file	Global.cpp
 */


#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "lua.lib" )
#pragma comment( lib, "lualib.lib" )


#include "Global.h"
#include "Type.h"


lua_State*			g_pLuaVM		= NULL;		///< Lua Virtual Machine

int					g_nWidth		= 1024;		///< 화면 가로 해상도
int					g_nHeight		= 768;		///< 화면 세로 해상도
int					g_nColorDepth	= 32;		///< 화면 Color Depth
BOOL				g_bFullscreen	= FALSE;	///< 전체 화면 여부
int					g_nTextureLevel	= 0;
BOOL				g_bShadowOn		= TRUE;
int					g_nMapQuality	= 0;
BOOL				g_bBGM_On		= TRUE;		///< BGM 연주 여부
BOOL				g_bEffSound_On	= TRUE;		///< 이펙트 사운드 출력 여부.
BOOL				g_b51ChSound_On	= TRUE;		///<
BOOL				g_bFPS_On		= FALSE;
BOOL				g_bShowEmblem	= FALSE;
BOOL				g_bShowMay		= TRUE;		
BOOL				g_bShowPet		= TRUE;
float				g_fVolBGM		= 0.5F;		///< 배경음 볼륨.
float				g_fVolEff		= 0.5F;		///< 효과음 볼륨.
float				g_fFPS			= 0.0F;
BYTE				g_btShowLevel	= 5;
WORD				g_wSendCheckSum = 0;

string				g_sUserGameId	= "";
string				g_sUserDaumId	= "";

#ifdef __CHINA_VER__
char				g_[32];
#endif

CClient*			g_pClient		= NULL;		///< 클라이언트 객체
IW3DInput*			g_pInput		= NULL;		///< 입력장치 객체
IW3DScene*			g_pScene		= NULL;		///< 그래픽 Scene 객체
IW3DSoundMgr*		g_pSoundMgr		= NULL;		///< 사운드 관리자
IW3DMediaPlayer*	g_pMediaPlayer	= NULL;		///< 미디어 플레이어 객체
IW3DUInterface*		g_pGUI			= NULL;		///< GUI
IW3DItemInfoMgr*	g_pItemInfoMgr	= NULL;		///< 아이템 정보 관리자
CSwordTrace*		g_pTrace		= NULL;
CCharacterState*	g_pCharacterState = NULL;
CMonsterState*		g_pMonsterState	= NULL;
CVehicleState*		g_pVehicleState = NULL;

string				g_sDaumAccountHost = "";	///< 다음 인증 사이트
string				g_sLoginServerHost = "";	///< 로그인 서버 호스트
string				g_asGameServerHost[NUM_GAMESERVER];		///< 게임 서버 호스트
BYTE				g_btGameServerNum = 0;		///< 게임 서버 수
BYTE				g_btServer = 0;				///< 서버 인덱스


DWORD				g_dwPlayerId	= 0;		///< 플레이어 ID
SPlayerInfo			g_infoPlayer;				///< 플레이어 정보	
SPetInfo			g_infoPet;
MAP_QUESTSTAE		g_mapQuestState;			///< 퀘스트들 상태
int					g_anGuildId[10];			///< 길드 아뒤

SCreatureTarget		g_sTarget;					///< 작업 중인 타겟.

IW3DCreature*		g_pPickCharacter	= NULL;	///< 피킹된 캐릭터
IW3DCreature*		g_pPickMonster		= NULL;	///< 피킹된 몬스터
IW3DCreature*		g_pPickNPC			= NULL;	///< 피킹된 NPC
IW3DItem*			g_pPickItem			= NULL;	///< 피킹된 아이템
IW3DFuncEntity*		g_pFuncEntity		= NULL;	///< 가능조물
IW3DCreature*		g_pPlayer			= NULL;
CHelper*			g_pHelper			= NULL;	///< 플레이어의 May
CSelectCreature*	g_pSelectMgr		= NULL;

DWORD				g_dwAttackLimit		= 2000;
SGameData			g_dtGame;
int					g_nDefenseGuild		= 0;	// 방어(수성)하는 길드 
int					g_nAttackGuild		= 0;	// 공격(공성)하는 길드 
BYTE				g_btWarMode			= 0;

char				g_szUIName[64]		= "";	// GUI Event용 컴포넌트 이름.
UIEVENT				g_UIevent			= 0;	// GUI Event 종류.
DWORD				g_dwUIParam1		= 0;	// GUI Event 추가 정보1.     
DWORD				g_dwUIParam2		= 0;	// GUI Event 추가 정보2.

LIST_CREATURE		g_listCollisionCreature;

#ifdef _DEBUG
BOOL				g_bAttackMacro = FALSE;		///< 주위의 일정 시간동안 매크로 돌림
BOOL				g_bSelfAttack = FALSE;
#endif //#ifdef _DEBUG

extern FrameInfo	Frame_Info_Intro;
extern FrameInfo	Frame_Info_Login;
extern FrameInfo	Frame_Info_SelectCharacter;
extern FrameInfo	Frame_Info_Game;


/**
 * Frame 생성.
 *
 * @param	sName		[IN] 상태 이름
 *
 * @return	생성된 Frame
 */
IFrame* MakeFrame( const string& sName )
{
	if( sName == "Intro" )
	{
		return Frame_Info_Intro.Factory();
	}
	else if( sName == "Login" )
	{
		return Frame_Info_Login.Factory();
	}
	else if( sName == "SelectCharacter" )
	{
		return Frame_Info_SelectCharacter.Factory();
	}
	else if( sName == "Game" )
	{
		return Frame_Info_Game.Factory();
	} //if..else if..

	return NULL;
} //MakeFrame
