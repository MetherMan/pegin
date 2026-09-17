/**
 * 게임 프레임 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-09-08 8:40p $
 * @version	$Revision: 202 $
 *
 * <HR>
 * $Log: /GameClient/Src/Game.h $
 * 
 * 202   04-09-08 8:40p Sk8snow2
 * 
 * 201   04-09-08 8:37p Paran
 * 
 * 200   04-08-27 8:01p Paran
 * 
 * 199   04-08-27 12:23p Sk8snow2
 * 
 * 198   04-08-24 6:06p Sk8snow2
 * 
 * 197   04-08-24 3:20p Sk8snow2
 * 
 * 196   04-08-23 8:40p Paran
 * 
 * 170   04-04-09 1:38a Sk8snow2		거래시작 패킷관련 수정
 * 
 * 167   04-04-02 9:01a Sk8snow2		OnPacketPetUpdateName 추가
 * 
 * 155   04-03-10 6:17p Paran			기능조물 추가
 * 
 * 153   04-02-27 6:02p Sk8snow2		Pet 관련 함수 다수 추가
 * 
 * 148   04-01-31 10:37a Sk8snow2		OnPacketGuildAddmemberRequest 추가
 * 
 * 146   04-01-28 12:06a Sk8snow2		친구관련 함수 추가
 * 
 * 143   04-01-15 2:40a Sk8snow2		OnPacketItemEvent 추가 ( 중국 아이템 이벤트 결과 관련 )
 * 
 * 142   04-01-14 11:47p Kjmgo			공성전시 최소프레임유지에 필요한 코드 추가
 * 
 * 139   04-01-12 11:08p Sk8snow2		OnPacketUpdateGuildInfo 추가
 * 
 * 134   04-01-02 7:18a Kjmgo			오른쪽 클릭시 아무것도 피킹되지 않았을떄의 처리를 분리
 *
 * 133   03-12-31 2:43p Paran			스킬
 * 
 * 131   03-12-26 11:01a Sk8snow2		OnPacketBeginSkill 추가
 * 
 * 129   03-12-19 8:55a Kjmgo			MAY(땡삐) 기능 업그레이드및, 공격시작 패킷을 보낸후, 일정시간동안
										시작하라는 패킷을 받지 못하면 다시 보내는 코드 추가
 * 
 * 128   03-12-19 4:13a Sk8snow2		OnPacketSystemMsg 추가
 * 
 * 125   03-12-17 9:22p Sckyj			m_pSound 제거
 * 
 * 124   03-12-17 4:14p Sckyj			OnPacketSoundEffect() 포션 먹는소리 처리, Game 전역 sound 변수 추가
 * 
 * 123   03-12-17 6:24a Paran			정만: 정혁이형 작업 백업
 * 
 * 121   03-12-16 2:46p Sk8snow2		OnPacketEnchantItemResult 추가
 * 
 * 120   03-12-16 2:13p Sk8snow2		OnUpdateCha 추가 ( 성향 변경 )
 * 
 * 118   03-12-14 6:10p Sk8snow2		OnPacketSoundEffect 추가
 * 
 * 117   03-12-14 3:11p Sckyj			BGM관련 함수 수정(작업중)
 * 
 * 116   03-12-12 8:00p Kjmgo			피킹할떄 레이어 저장
 * 
 * 115   03-12-10 8:32p Kjmgo			PickingEffect를 Character로 옮김
 * 
 * 114   03-12-02 3:43a Sk8snow2		날씨 변경 처리 관련
 * 
 * 113   03-12-01 2:43p Sckyj			PlayBGM 함수 변경( 동일 Area에서 복수개의 BGM을 재생토록 변경 ) - 작업중
 * 
 * 112   03-11-28 10:14a Sk8snow2		OnPacketMemberChat 추가 
 * 
 * 111   03-11-28 9:31a Sk8snow2		void	OnPacketBroadcastChat();	// 전체 채팅
										void	OnPacketSellingChat();	// 매매 채팅
										void	OnPacketGuildChat();	// 길드 채팅
										void	OnPacketPartyChat();	// 파티 채팅 추가
 * 
 * 110   03-11-26 3:51p Sckyj			Ambient BGM 시간대별 재생기능 추가
 * 
 * 109   03-11-21 10:00a Paran			몹 아이템 드롭 수정
 * 
 * 108   03-11-15 9:41p Paran			PK 부분 1차 수정
 * 
 * 107   03-11-10 4:18a Sk8snow2		OnPacketDamagePvP() 함수 추가 ( PK용 )
 * 
 * 106   03-10-15 11:14a Kjmgo			패킷 처리를 함수 포인터 사용
 * 
 * 104   03-10-09 3:45p Sk8snow2		OnPacketPartyBoardList 추가
 * 
 * 98    03-09-29 5:05p Kjmgo			Picking용 Interface Pointer를 CGame 내부에서 전역으로 수정
 * 
 * 97    03-09-29 4:06p Kjmgo			Picking 수정.( 한프레임당 한개의 Level만 검사하도록. ( 나머지는 Skip )
 * 
 * 96    03-09-29 3:28p Kjmgo			m_pWorld->GetCharacter( m_dwPlayer ); 구문의 난무를 m_pPlayer로 대체
 * 
 * 92    03-09-22 11:37a Sk8snow		OnPacketGhostState 추가
 * 
 * 91    03-09-22 11:23a Sk8snow		OnPacketCharacterInsert, 
										OnPacketCharacterRemove 수정, 
										dPACKET_CHARACTER_INSERT, 
										dPACKET_CHARACTER_CONNECT, 
										dPACKET_CHARACTER_REMOVE,
										dPACKET_CHARACTER_DISCONNECT 통합
 * 
 * 84    03-09-07 12:24p Sk8snow		OnPacketMobState 추가
 * 
 * 83    03-09-07 1:29a Sk8snow			OnPacketNpcInfo 추가
 * 
 * 77    03-08-13 10:52a Sk8snow		OnPacketBeginCastMagic2 추가
 * 
 * 48    03-06-19 8:31p Paran			맵 지역 분리
 * 
 * 44    03-06-14 11:03a Paran			Skill zzang~
 * 
 * 42    03-06-03 11:38p Paran			용병 패킷처리
 * 
 * 40    03-05-23 4:05a Paran			공격가능거리추가.
 * 
 * 39    03-05-21 1:26p Paran			STR, INT, DEX, TOTAL 레벨 관련 패킷 추가.
 * 
 * 32    03-05-11 11:03a Ahastudio		바닥 피킹 이펙트를 반복하도록 수정.
 *
 * 30    03-05-09 11:31p Paran			아이템 사용 추가.
 *
 * 27    03-05-06 9:27p Ahastudio		공격 패킷 처리 방식 수정.
 *
 * 25    03-05-06 6:18p Paran			상점 기능 추가.
 *
 * 24    03-05-05 1:40p Ahastudio		AddPlayer -> CreatePlayer()
 *
 * 23    03-04-30 7:10p Paran			스킬 선택 부분 수정.
 *
 * 19    03-04-15 1:25a Paran			NPC 추가.
 *
 * 18    03-04-12 3:04p Paran			캐릭터 장착 아이템 변경 기능 추가.
 *
 * 13    03-04-02 7:21p Paran			플레이어 아이템 추가.
 *
 * 11    03-03-26 6:22a Paran			아이템 서버 연결.
 *
 * 9     03-03-25 8:39p Paran			아이템 관련 함수 추가.
 *
 * 6     03-03-21 5:43a Ahastudio		스킬 변수 추가.
 *
 * 3     03-03-15 1:05p Ahastudio		사망 공지 처리 변경.
 *
 * 2     03-03-14 2:30p Ahastudio		ProcessRecvPacket 및 OnPacket을 GameProcessPacket.cpp로 이동.
 * <HR>
 *
 * @file	Game.h
 */


#ifndef __Game_Header__
#define __Game_Header__


#include "Client.h"
#include "Protocol.h"
#include "Packet.h"
#include "Type.h"
#include "MiniGame.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

/**
 * 게임 프레임 클래스.
 * @ingroup		Game
 */
class CGame : public IFrame
{
public:
	enum { ZOOM_SHORT = 1, ZOOM_LONG = 2, ZOOM_LOCK = 3 };

	UNDER_MEMORY_MGR( CGame );

	CGame();
	~CGame();

	BOOL	Init();
	BOOL	Update( const DWORD dwTick, string* psName );

//private:
	BOOL			m_bQuit;			///< 게임 종료 여부
	BOOL			m_bExitFrame;		///< 프레임 종료
	BOOL			m_bPlaying;			///< 게임 플레이 가능 여부
				
	BOOL			m_bNoMove;			///< 
	DWORD			m_dwPShopId;		///< 개인상점 캐릭터 아이뒤~

	IW3DCamera*		m_pCamera;			///< 카메라
	int				m_nZoomMode;		///< 카메라 줌 모드.
	float			m_fCameraOffset;	///< 카메라 줌 옵셋.

	IW3DWorld*		m_pWorld;			///< 게임 월드

	WORD			m_wMap;				///< 현재 맵 번호
	DWORD			m_dwPlayer;			///< 플레이어의 ID
	SCreatureTarget	m_sItemCreature;	///< 아이템 소유 크리쳐
	DWORD			m_dwRequestId;		///< 뭔가 요청을한 사람

	BYTE			m_btMapType;		///< Map Type
	BOOL			m_bIndoor;			///< Indoor
	BOOL			m_bAttack;			///< 공격 여부
	BOOL			m_bCheckLMB;		///< 마우스 L버튼 눌린상태 //0 = 정보, 1 = 공격
	BOOL			m_bToTarget;		///< 몬스터에게 자동 이동 중..
	BOOL			m_bAdvanceMove;		///< 공격중일때 이동하기를 원할때
	BOOL			m_bMoveAttack;
	int				m_nSkillItem;		///< 선택된 스킬
	float			m_fAttackRange;		///< 공격가능거리

	int				m_nPickLevel;		///<

	IW3DCreature*	m_pPlayer;			///< 플레이어 크리쳐

	BOOL			m_bPick;

	int				m_nPickedLayer;		///< 피킹한 Layer

	DWORD			m_dwPickDoor;		///< 피킹된 Door의 ID

	CPacket			m_recvPacket;		///< 받은 패킷		


	BOOL			m_bSendAttackPacket;
	DWORD			m_dwAttackLimit;	///< 공격 대비 제한 시간
										///< 공격 패킷을 보낸후, 일정 시간동안,
										///< 시작 패킷을 보내지 아니하면, 다시 한번 보낸다.

	DWORD			m_dwViewDelay;

//	CMiniGame		m_mgFishHook;		///< 낚시 미니게임 객체	
	BOOL			m_bMiniGaming;		///< 미니게임을 하고 있는중인가

	BOOL			bCheck;
	BOOL			bCheck1;

	void			(CGame::*m_apPacketProcess[dPACKET_MAX_VALUE])();

	void	InitGUI();
	void	InitFuncArray();	
	void	Render( const DWORD dwTick );
	void	UpdateNetwork( const DWORD dwTick );
	void	UpdateInput( const DWORD dwTick );
	
	void	CalculateShowLevel();

	void	PlayMapBGM();
	void	PlayEventBGM( const int nEventNum  );
	void	PlayAreaBGM( const int nX, const int nY );		

	BOOL	CheatKey( const char* szText );
	void	ScreenShot();
	void	SetMapInfo( const DWORD dwMap );
	void	ReadMapInfo( FILE* fp );
//	void	ReadMGInfo( FILE* fp );

	void	InputCamera( const DWORD dwTick );
	void	InputPlayer( const DWORD dwTick );
	void	MovePlayer( const DWORD dwTick );
	void	AttackCreature();
	void	SendAttack( );

	void	ProcessRecvPacket();

	BOOL	LoadMap( const DWORD dwMap );
	BOOL	LoadIndoor( const DWORD dwMap );
	void	CreatePlayer( const int nX, const int nY, const BYTE btLayer );
	void	PutNPCMinimap();
	BOOL	CheckPlayerKill();

	BOOL	CheckPick( const Vector3& vPos, int nMouseX, int nMouseY );
	BOOL	CheckPickCharacter( const Vector3& vPos, Vector3& vOrig, Vector3& vDir, BOOL bSkip );
	BOOL	CheckPickMonster( const Vector3& vPos, Vector3& vOrig, Vector3& vDir, BOOL bSkip );
	BOOL	CheckPickNpc( const Vector3& vPos, Vector3& vOrig, Vector3& vDir, BOOL bSkip );
	BOOL	CheckPickEtc( const Vector3& vPos, Vector3& vOrig, Vector3& vDir, BOOL bSkip );

	void	PickedDoor_L();
	void	PickedItem_L();
	void	PickedCharacter_L();
	void	PickedMonster_L();
	void	PickedNPC_L();
	void	PickedTerrain_L( const Vector3&	vPick, IW3DCreature* pPlayer );
	BOOL	PickedTerrain_R();
	void	PickedDoor_R();
	void	PickedItem_R();
	void	PickedCharacter_R();
	void	PickedMonster_R();
	void	PickedNPC_R();	
	void	PickedFuncEntity_R();
	void	PickedNone_R();

	IW3DCreature* GetTargetCreature();

	BOOL	IsPlayer( DWORD dwCharacterId );

	void	OnEventKeyDown( DWORD dwParam1 );
	void	OnEventKeyUp( DWORD dwParam1 );
	void	OnEventButton( const char* szUIName, DWORD dwParam2 );
	void	OnEventHotKey( const DWORD dwTick );
	void	OnEventReturnKey( const char* szUIName );
	void	OnEventShowDlg( const char* szUIName, DWORD dwParam2 );
	void	OnEventInsertItemInInventory( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventHolding( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventChangeHand( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventItemMove( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventItemDrop( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventInsertEquip( DWORD dwParam2 );
	void	OnEventRemoveEquip( DWORD dwParam2 );
	void	OnEventItemToMap( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventSpellMove( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventSetMainSkill( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventBuyItem( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventSellItem( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventCastItem( DWORD dwParam1 );
	void	OnEventSendChatting( DWORD dwParam2 );
	void	OnEventDealAddGold( DWORD dwParam1 );
	void	OnEventDealAddItem( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventDealDeleteItem( DWORD dwParam1, DWORD dwParam2 );	
	void	OnEventDealCancel();
	void	OnEventDealAddMoney( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventDealCheckIn();
	void	OnEventPartyCreate();
	void	OnEventPartyDestroy();
	void	OnEventDealRequest();
	void	OnEventPartyRequest( DWORD dwParam1 );
	void	OnEventPartyRemoveMember( DWORD dwParam1 );
	void	OnEventTalkRequest( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventShopRequest( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventQuestRequest( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventSmithRequest( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventCardSum( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventCardUndoNPC( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventCardUndo( DWORD dwParam1 );
	void	OnEventSmithAddItem( DWORD dwParam1 );
	void	OnEventSmithRemoveItem( DWORD dwParam1 );
	void    OnEventSmithUpdate( DWORD dwParam1, DWORD dwParam2 );
	void	OnEvnetServer( DWORD dwParam1 );
	void	OnEventStorageRequest();
	void	OnEventSerIntGarbageRequiest( DWORD dwParam1 );
	void	OnEventSerIntGarbageInit();
	void	OnEventSerIntGarbageRemove( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventSerIntInvenRequiest( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventSerIntInvenInit();	
	void	OnEventSerIntInvenRemove( DWORD dwParma1, DWORD dwParam2 );
	void	OnEventStorageAddItem( DWORD dwParam1 );
	void	OnEventStorageRemoveItem( DWORD dwParam1 );	
	void	OnEventStorageGetGold( DWORD dwParam2 );
	void	OnEventStorageSaveGold( DWORD dwParam2 );
	void	OnEventQuestAccept( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventSoundOption();
	void	OnEventBGMOnOff( DWORD dwParam2 );
	void	OnEventEffOnOff( DWORD dwParam2 );
	void	OnEvent51ChOnOff( DWORD dwParam2 );
	void	OnEventSoundVol( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventDBClickItem( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventQuickSlot( DWORD dwParam1 );
	void	OnEventPartyInvit( DWORD dwParam1 );
	void	OnEventPartyOut();
	void	OnEventPartyBanish();
	void	OnEventPartyBoard();
	void	OnEventPartyBoardRegist();
	void	OnEventPartyHeal( DWORD dwParam1 );
	void	OnEventCamera( DWORD dwParam1 );
	void	OnEventPlayerDie( DWORD dwParam1 );
	void	OnEventTimer( const char* szUIName, DWORD dwParam1 );
	void	OnEventAutoItemPickUp();
	void	OnEventEnchant( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventEndEnchant();
	void	OnEventInfoPosition();
	void	OnEventSkillSpecial( DWORD dwParam );
	void	OnEventFriendCut();
	void	OnEventChattingCut();
	void	OnEventWhisperCut();
	void	OnEventDealCut();
	void	OnEventFriendInvit( DWORD dwParam );
	void	OnEventFriendRequest( DWORD dwParam );
	void	OnEventFriendRequestNo( DWORD dwParam );
	void	OnEventFriendRegist();
	void	OnEventFriendList();
	void	OnEventFriendDelete();
	void	OnEventGuildTakeMember();
	void	OnEventAid( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventGuildRemoveMember();
	void	OnEventGuildQuit();
	void	OnEventGuildCreate();
	void	OnEventGuildRelease();
	void	OnEventGuildPromote();
	void	OnEventGuildDemote();
	void	OnEvnetGuildWarInfo( DWORD dwParam1 );
	void	OnEvnetGuildGetCastleList();
	void	OnEventGotoDealMap();
	void	OnEventPetOnOff();
	void	OnEventChangePetName();
	void	OnEventPKMode( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventRequestGuildList();
	void	OnEventMyShopOpen();
	void	OnEventMyShopChangeTitle();
	void	OnEventMyShopAddItem( DWORD dwParam1, DWORD dwParam2 );
	void	OnEventMyShopRemoveItem( DWORD dwParam1 );
	void	OnEventMyShopSellBegin();
	void	OnEventMyShopSellFinish();
	void	OnEventMyShopGetItemList( DWORD dwParam1 );
	void	OnEventMyShopBuyItem( DWORD dwParam1 );
	void	OnEventGetTex();
	void	OnEventChangeTex( DWORD dwParam1 );
	void	OnEventEnchantChange( DWORD dwParam1 );
	void	OnEventEnchantCollection( DWORD dwParam1 );
	void	OnEventItemRook();
	void	OnEventLevelRook();
	void	OnEventCashItemUse( DWORD dwParam1 );
	void	OnEventGetBillingItem();
	void	OnEventSmithOpen();
	
	void	OnMouseLClick( const BOOL bPick, const Vector3& vPick );
	void	OnMouseRClick( const BOOL bPick, const Vector3& vPick );

	void	SendPacket_CharacterMove();

	void	OnPacketNull();
	void	OnPacketMapLoading();
	void	OnPacketCharacterInsert();
	void	OnPacketCharacterRemove();
	void	OnPacketCharacterMove();
	void	OnPacketCharacterDirection();
	void	OnPacketMonsterInsert();
	void	OnPacketMonsterRemove();
	void	OnPacketMonsterMove();
	void	OnPacketChatMsg();
	void	OnPacketMobHpInfo();
	void	OnPacketDamageUser();
	void	OnPacketDamageMob();
	void	OnPacketDieUser();
	void	OnPacketDieMob();
	void	OnPacketChatWhisper();
	void	OnPacketChatWhisperResult();
	void	OnPacketSkillUp();
	void	OnPacketGetExp();
	void	OnPacketSkillUpEvent();
	void	OnPacketHpGainUser();
	void	OnPacketMpGainUser();
	void	OnPacketHpGainMob();
	void	OnPacketInsertMapItem();
	void	OnPacketRemoveMapItem();
	void	OnPacketInsertInventory();
	void	OnPacketRemoveInventory();
	void	OnPacketItemFromHand();
	void	OnPacketInsertEquipItem();
	void	OnPacketRemoveEquipItem();
	void	OnPacketChangeMainSkill();
	void	OnPacketAlreadyConnected();

	void	OnPacketItemPlayerItemInfo();
	void	OnPacketChangeEquip();
	void	OnPacketRemoveSpellBook();
	void	OnPacketChangeTime();
	void	OnPacketUpdateMoney();
	void	OnPacketUpdateCash();
	void	OnPacketShopItemList();
	void	OnPacketShopResult();
	void	OnPacketBeginPlayerAttack();
	void	OnPacketDealBegin();
	void	OnPacketDealInsertItem();
	void	OnPacketDealRemoveItem();
	void	OnPacketDealUpdateMoney();
	void	OnPacketDealRequestSubmit();
	void	OnPacketDealFinish();
	void	OnPacketPartyCreateOk();
	void	OnPacketPartyDestroyed();
	void	OnPacketPartyJoinResult();
	void	OnPacketPartyAddMember();
	void	OnPacketPartyDeleteMember();
	void	OnPacketPartyMemberHPMP();
	void	OnPacketPartyMemberMapInfo();
	void	OnPacketUpdatePoint();
	void	OnPacketUpdateLevel();
	void	OnPacketAttackRange();
	void	OnPacketAddHero();
	void	OnPacketRemoveHero();
	void	OnPacketEquipInfo();
	void	OnPacketMagicInfo();
	void	OnPacketMagicCastUser();
	void	OnPacketAddEffect();
	void	OnPacketRemoveEffect();
	void	OnPacketUpdateMaxHPMP();
	void	OnPacketSetSkillExp();
	void	OnPacketAddSkill();
	void	OnPacketSkillList();
	void	OnPacketUpdatePotionCnt();
	void	OnPacketUpdateCardCnt();
	void	OnPacketQuestList();
	void	OnPacketQuestResult();
	void	OnPacketGarbageItemList();
	void	OnPacketSerGarbageItemList();
	void	OnPacketSerRemoveFromGarbage();
	void	OnPacketSerInvenItemList();
	void	OnPacketSerInvenItemRemove();
	void	OnPacketInsertToGarbage();
	void	OnPacketRemoveFromGarbage();
	void	OnPacketRideInfo();
	void	OnPacketBeginCastMagic2();
	void	OnPacketNpcInfo();
	void	OnPacketMobState();
	void	OnPacketGhostState();
	void	OnPacketPartyBoardList();
	void	OnPacketDamagePvP();
	void	OnPacketBroadcastChat();		// 전체 채팅
	void	OnPacketSellingChat();			// 매매 채팅
	void	OnPacketGuildChat();			// 길드 채팅
	void	OnPacketPartyChat();			// 파티 채팅 
	void	OnPacketMemberChat();			// 단 채팅
	void	OnPacketChangeWeather();		// 날씨변경
	void	OnPacketSoundEffect();			// 사운드 이펙트
	void	OnPacketUpdateCha();	
	void	OnPacketEnchantItemResult();
	void	OnPacketPartyAskInvite();
	void	OnPacketUpdateClass();
	void	OnPacketSystemMsg();
	void	OnPacketFriendLoginMsg();
	void	OnPacketBeginSkill();
	void	OnPacketSetVisible();
	void	OnPacketSetMovable();
	void	OnPacketUpdateGuildInfo();
	void	OnPacketItemEvent();
	void	OnPacketFriendList();
	void	OnPacketFriendAdded();
	void	OnPacketFriendRemoved();
	void	OnPacketFriendMemo();
	void	OnPacketGuildAddmemberRequest();
	void	OnPacketPetInfo();
	void	OnPacketPetON();
	void	OnPacketPetOFF();
	void	OnPacketUpdatePetGage();
	void	OnPacketUpdatePetInfo();
	void	OnPacketPetAttack();
	void	OnPacketAttackToPet();		
	void	OnPacketFisingStatus();
	void	OnPacketFishingPattern();
	void	OnPacketFishingPos();
	void	OnPacketPetUpdateName();
	void	OnPacketPetUpdateExp();
	void	OnPacketDealBeginOk();
	void	OnPacketFriendLogin();	
	void	OnPacketFriendLogout();
	void	OnPacketMobPopEffect();	
	void	OnPacketPlayerPopEffect();	
	void	OnPacketChangeStatus();
	void	OnPacketGuildMemberList();
	void	OnPacketMyShopOpenResult();
	void	OnPacketMyShopChangeTitle();
	void	OnPacketMyShopAddItem();
	void	OnPacketMyShopRemoveItem();
	void	OnPacketMyShopSellBegin();
	void	OnPacketMyShopSellFinish();
	void	OnPacketMyShopItemList();
	void	OnPacketMyShopUpdateInfo();
	void	OnPacketMyShopRemoveListItem();
	void	OnPacketMyshopUpdateCash();
	void	OnPacketWorldChat();
	void	OnPacketInsertObjMob();
	void	OnPacketUpdateWeightGage();
	void	OnPacketChangeNameColor();			
	void	OnPacketUpdateWarCamp();	
	void	OnPacketUpdateWarinfo();
	void	OnPacketUpdateGarbageMoney();
	void	OnPacketUpdatePvPMode();	
	void	OnPacketSendCastleList();
	void	OnPacketWarState();
	void	OnPacketOpenTaxWindow();
	void	OnPacketUpdateTax();
	void	OnPacketBillingItemInfo();	
	void	OnPacketRemoveBillingItem();
	void	OnPacketMapShout();
	void	OnPacketEnemyDivision();
	void	OnPacketShowItemTimer();

	void	OnPacketSmithAddItem();
	void	OnPacketSmithFinish();
	void	OnPacketSmithRemoveItem();
//	void	OnPacketSmithUpdate();
//	void	OnPacketFriend();
	void	OnPacketFriendDOUWANT();
	void	OnPacketFriendRequest();

	BOOL	m_btIsTest;
	DWORD	m_dwTickss;

	BOOL	m_btIsFriendList;   //친구등록창

}; //class CGame


#endif // #ifndef __Game_Header__
