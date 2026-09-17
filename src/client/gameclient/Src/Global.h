/**
 * 전역 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 85 $
 *
 * <HR>
 * $Log: /GameClient/Src/Global.h $
 * 
 * 85    05-02-18 7:38p Sk8snow2
 * 
 * 84    04-09-11 1:46p Sk8snow2
 * 
 * 83    04-09-08 8:37p Paran
 * 
 * 82    04-08-27 8:00p Paran
 * 
 * 71    04-03-15 5:43p Sckyj			운영자 작업 1차완료 - 동작막기 안함
 * 
 * 69    04-03-10 6:17p Paran			기능조물 추가
 * 
 * 68    04-02-27 9:43p Kjmgo			펫추가 ^.^/
 * 
 * 63    04-01-13 9:43p Kjmgo			사양 설정 적용
 * 
 * 60    04-01-06 1:58p Kjmgo			공격 매크로 추가
 * 
 * 59    04-01-06 12:01a Kjmgo			공속 추가 
 * 
 * 58    03-12-29 5:20p Kjmgo			레벨이 10이하일때만 메이가 나타나도록 수정
 * 
 * 54    03-12-17 10:04p Kjmgo			Alt키를 누를시, 엠블럼 출력
 * 
 * 53    03-12-17 7:50a Kjmgo			선택 유지 구현 ( 選擇[せんたく] 維持[いじ] )
 * 
 * 51    03-12-04 7:16p Kjmgo			주인공 캐릭터 공속 수정 가능하도록 하는 코드 삽입
 * 
 * 50    03-12-04 10:44a Kjmgo			플레이어의 May의 전역포인터 추가
 * 
 * 49    03-11-15 9:41p Paran			PK 부분 1차 수정
 * 
 * 47    03-10-23 10:04p Paran			공격효과부분 수정.
 * 
 * 45    03-10-01 11:08p Kjmgo			크리처들과의 충돌처리 진행
 * 
 * 44    03-09-29 5:05p Kjmgo			Picking용 Interface Pointer를 CGame 내부에서 전역으로 수정
 * 
 * 41    03-08-30 11:07a Kjmgo			Character Animation Data 읽는 부분 추가
 * 
 * 30    03-05-06 11:51p Ahastudio		Lua 추가.
 * <HR>
 *
 * @file	Global.h
 */


#ifndef __Global_Header__
#define __Global_Header__


#include "../../Engine/Inc/Engine.h"
#include "../../UInterface/Inc/UInterface.h"
#include "Helper.h"
#include "Sword.h"
#include "ActionMgr.h"
#include "SelectCreature.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
} //extern "C"

#include <string>
#include <vector>
#include <map>
#include <list>


#include "Encrypt.h"
 
#define NUM_GAMESERVER			20
#define FONTCOLOR_REF			0xFFFFFF00
#define dPATCHVER				2020092801

#define dMAX_SOCK_BUFF					4096
#define dMAX_SEND_SOCKBUFF				(dMAX_SOCK_BUFF*30)
#define dMAX_DATA_SOCK_BUFF				(dMAX_SOCK_BUFF*1000)

#define dCLIENT_AUTO_ATTACK		// 클라이언트의 자동공격

using std::string;
using std::vector;
using std::map;

class	CClient;
struct  SGameData;
struct	SPlayerInfo;
struct	SPetInfo;
struct	SCreatureTarget;

extern lua_State*			g_pLuaVM;

extern int					g_nWidth;
extern int					g_nHeight;
extern int					g_nColorDepth;
extern BOOL					g_bFullscreen;
extern BOOL					g_bFPS_On;
extern int					g_nTextureLevel;
extern BOOL					g_bShadowOn;
extern int					g_nMapQuality;
extern BOOL					g_bBGM_On;
extern BOOL					g_bEffSound_On;
extern BOOL					g_b51ChSound_On;
extern BOOL					g_bShowMay;		
extern float				g_fVolBGM;
extern float				g_fVolEff;
extern float				g_fFPS;
extern BYTE					g_btShowLevel;
extern WORD					g_wSendCheckSum;

extern DWORD				g_dwAttackLimit;


extern string				g_sUserGameId;
extern string				g_sUserDaumId;
#ifdef __CHINA_VER__
extern char					g_IP[32];
#endif

extern HWND					g_hWnd;

extern CClient*				g_pClient;
extern IW3DInput*			g_pInput;
extern IW3DScene*			g_pScene;
extern IW3DSoundMgr*		g_pSoundMgr;
extern IW3DMediaPlayer*		g_pMediaPlayer;
extern IW3DUInterface*		g_pGUI;
extern IW3DItemInfoMgr*		g_pItemInfoMgr;
extern CSwordTrace*			g_pTrace;
extern CSelectCreature*		g_pSelectMgr;

extern string				g_sDaumAccountHost;
extern string				g_sLoginServerHost;
extern string				g_asGameServerHost[NUM_GAMESERVER];
extern BYTE					g_btGameServerNum;			///< 게임 서버 수
extern BYTE					g_btServer;					///< 서버 인덱스

extern DWORD				g_dwPlayerId;
extern SPlayerInfo			g_infoPlayer;
extern SPetInfo				g_infoPet;
extern BOOL					g_bShowEmblem;
extern BOOL					g_bShowPet;
extern SGameData			g_dtGame;
extern int					g_nDefenseGuild;	// 방어(수성)하는 길드 
extern int					g_nAttackGuild;		// 공격(공성)하는 길드 
extern BYTE					g_btWarMode;		// 전쟁 중..

extern char					g_szUIName[64];		// GUI Event용 컴포넌트 이름.
extern UIEVENT				g_UIevent;			// GUI Event 종류.
extern DWORD				g_dwUIParam1;		// GUI Event 추가 정보1.
extern DWORD				g_dwUIParam2;		// GUI Event 추가 정보2.

#ifdef _DEBUG
extern BOOL					g_bAttackMacro;		///< 주위의 일정 시간동안 매크로 돌림 
extern BOOL					g_bSelfAttack;		///< 자신을 공격하도록 설정( 힐 )
#endif //#ifdef _DEBUG

extern IW3DCreature*		g_pPickCharacter;	///< 피킹된 캐릭터
extern IW3DCreature*		g_pPickMonster;		///< 피킹된 몬스터
extern IW3DCreature*		g_pPickNPC;			///< 피킹된 NPC
extern IW3DCreature*		g_pPlayer;			///< 플레이어 크리쳐
extern IW3DItem*			g_pPickItem;		///< 피킹된 아이템
extern IW3DFuncEntity*		g_pFuncEntity;		///< 가능조물
extern CHelper*				g_pHelper;			///< 플레이어의 May
extern SCreatureTarget		g_sTarget;
extern int					g_anGuildId[10];	///< 길드 아뒤

extern BOOL					g_Checkhorse;

typedef std::map<WORD, BYTE>	MAP_QUESTSTAE;
extern MAP_QUESTSTAE		g_mapQuestState;

typedef std::list<IW3DCreature*> LIST_CREATURE;		///< 충돌용 크리처 리스트
extern LIST_CREATURE			g_listCollisionCreature;

/**
 * Frame Base Class.
 * @ingroup		Game
 */
class IFrame
{
public:
	/**
	 * 소멸자.
	 */
	virtual ~IFrame()	{}

	/**
	 * 초기화 함수.
	 */
	virtual BOOL	Init() = 0;

	/**
	 * 업데이트 함수.
	 *
	 * @param	dwTick		[IN] 경과 tick ( 1000tick = 1sec. )
	 * @param	psName		[OUT] 넘어갈 Frame 이름
	 */
	virtual BOOL	Update( const DWORD dwTick, string* psName ) = 0;
}; //class IFrame


IFrame*		MakeFrame( const string& sName );

/**
 * 현제 작업중인 크리쳐
 */
struct SCreatureTarget
{
	BYTE		btType;					///< 크리쳐 타입
	DWORD		dwId;					///< 크리쳐 아이디

	void		Clean()		{ btType = NONE; dwId = 0; }
	BOOL		IsEmpty()	{ if( btType == NONE )	return TRUE;	return FALSE; }
	void		SetTarget( BYTE btType_, DWORD dwId_ )	{ btType = btType_; dwId = dwId_; }
	BOOL		IsTarget( BYTE btType_, DWORD dwId_ )
	{
		if( btType == btType_ && dwId == dwId_ )
		{
			return TRUE;
		} //if
		return FALSE;
	} //IsTarget
}; //SCreatureTarget

/**
 * 게임을 즐기기위해 필요한 정보들
 */
struct SGameData
{
	WORD	wMonsterCount;		///< 주위 몬스터 수
	WORD	wPlayerCount;		///< 주위 플레이어의 수
	WORD	wAttackerCount;		///< 공격하는자의 수

	BYTE	btTargetType;		///< 타겟의 종류
								/*  타겟과의 상태이기도 한데,
									이길수 있는 상대인지,
									이길수 없는 상대인지에 대해서
									알려주게된다.
								*/

	SGameData()
	{
		wMonsterCount = 0;
		wPlayerCount = 0;
		btTargetType = 0;
	}; //SGameData
}; //



/**
 * Frame Information.
 */
struct FrameInfo
{
	const char*		szName;				///< Frame 이름
	IFrame*			(*Factory)();		///< Factory 함수
}; //struct FrameInfo


#define IMPLEMENT_FRAME( I, C )			\
	static IFrame*	Frame_Factory_##C()	\
	{									\
		return new I;					\
	}									\
	FrameInfo	Frame_Info_##C =		\
	{									\
		#C,								\
		Frame_Factory_##C				\
	};

void	ReleaseMonsterModel();
void	ReleaseCreatureBoundBox();

void	SendPacket_Attack();

#endif	// #ifndef __Global_Header__
