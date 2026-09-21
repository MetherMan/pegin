/**
 * 게임 패킷 처리.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 449 $
 *
 * <HR>
 * $Log: /GameClient/Src/GameProcessPacket.cpp $
 * 
 * 449   05-02-18 7:38p Sk8snow2
 * 
 * 448   04-12-08 3:06a Sk8snow2
 * 
 * 447   04-11-16 7:11p Sk8snow2
 * 
 * 446   04-11-01 2:15a Sk8snow2
 * 
 * 445   04-09-24 4:45p Paran
 * 
 * 443   04-09-18 9:01a Sk8snow2			맵로딩시 혹은 게임 종료시 튕기던 버그 수정 ( 정만 )
 * 
 * 407   04-06-30 2:24a Sk8snow2			OnPacketGarbageItemList 함수 수정
 * 
 * 396   04-06-05 10:29a Sk8snow2			OnPacketMapLoading 에 공성정보 추가 
 * 
 * 375   04-05-07 5:19p Sk8snow2			OnPacketInsertCharacter 던가 -_- ... 직업과 통합레벨 추가
 * 
 * 364   04-04-09 1:38a Sk8snow2			거래시작 패킷관련 수정
 * 
 * 357   04-04-02 9:51a Sk8snow2			OnPacketPetON 수정
 * 
 * 354   04-04-02 9:04a Sk8snow2			OnPacketPetUpdateName 추가
 * 
 * 341   04-03-15 5:43p Sckyj				운영자 작업 1차완료 - 동작막기 안함
 * 
 * 340   04-03-12 6:42p Sckyj				운영자 작업중 - OnPacketRideInfo() 함수 수정
 * 
 * 337   04-02-27 9:43p Kjmgo				펫추가 ^.^/
 * 
 * 326   04-02-03 10:04a Sk8snow2			OnPacketNpcInfo 함수.. 길드관리 NPC 관련 추가
 * 
 * 323   04-01-31 10:37a Sk8snow2			OnPacketGuildAddmemberRequest 추가
 * 
 * 321   04-01-28 12:06a Sk8snow2			친구관련 함수 추가
 * 
 * 316   04-01-12 11:10p Sk8snow2			OnPacketUpdateGuildInfo 추가
 * 
 * 303   04-01-05 5:46p Sk8snow2			OnPacketAddEffect / OnPacketRemoveEffect
 * 
 * 302   04-01-05 10:41 Sckyj				Change UI Message In Chinaversion - Party Invite 
 * 
 * 301   04-01-03 10:59 Sckyj				OnPacketPartyAskInvite() - In Chinese Version Change Message
 * 
 * 300   04-01-03 11:09a Kjmgo				반투명 상태및, 보이지 않는 설정 적용
 * 
 * 298   04-01-03 10:41a Sk8snow2			OnPacketSetVisible 추가
 * 
 * 297   04-01-03 10:29a Sk8snow2			OnPacketCharacterInsert / Visible 패킷 추가
 * 
 * 295   04-01-02 2:20p Paran				메세지 define 처리
 * 
 * 294   04-01-02 7:18a Kjmgo				오른쪽 클릭시 아무것도 피킹되지 않았을떄의 처리를 분리
 * 
 * 292   03-12-31 3:18p Kjmgo				마법쪽 버그 수정 
 * 
 * 291   03-12-31 2:43p Paran				스킬
 * 
 * 288   03-12-26 11:06a Sk8snow2			OnPacketBeginSkill 추가
 * 
 * 285   03-12-21 9:05a Kjmgo				주인공일때 메이가 공격대상에 대해서 설명하도록 수정 ( 종전, 가리지 않았음 )
 * 
 * 284   03-12-19 8:55a Kjmgo				MAY(땡삐) 기능 업그레이드및, 공격시작 패킷을 보낸후, 일정시간동안
											시작하라는 패킷을 받지 못하면 다시 보내는 코드 추가
 * 
 * 282   03-12-19 4:15a Sk8snow2			OnPacketSystemMsg 추가
 * 
 * 277   03-12-18 1:41a Kjmgo				PvP시 데미지 뜨게 수정
 * 
 * 276   03-12-17 9:22p Sckyj				포션 먹는 소리를 마법 이펙트로대체
 * 
 * 275   03-12-17 4:14p Sckyj				OnPacketSoundEffect() 포션 먹는소리 처리, Game 전역 sound 변수 추가
 * 
 * 274   03-12-17 11:21a Kjmgo				맵마다 시간 적용
 * 
 * 273   03-12-17 6:24a Paran				정만: 정혁이형 작업 백업
 * 
 * 272   03-12-17 4:53a Sk8snow2			OnPacketPartyAskInvite 추가
 * 
 * 270   03-12-16 2:46p Sk8snow2			OnPacketEnchantItemResult 추가
 * 
 * 268   03-12-16 2:13p Sk8snow2			OnUpdateCha 추가 ( 성향 변경 )
 * 
 * 267   03-12-16 10:53a Sk8snow2			OnPacketCharacterInsert 수정 ( 길드명, 길드마크 관련 추가작업 )
 * 
 * 266   03-12-14 6:12p Sk8snow2			OnPacketSoundEffect 추가
 * 
 * 264   03-12-11 4:12a Kjmgo				SetTime함수 수정
 * 
 * 263   03-12-11 1:04a Sk8snow2			OnPacketChangeTime 수정
 * 
 * 261   03-12-02 9:11p Kjmgo				날씨 변경 처리 관련 - 클라랑 서버 연결
 * 
 * 260   03-12-02 3:43a Sk8snow2			날씨 변경 처리 관련
 * 
 * 259   03-11-29 2:30p Paran				쳇 처리 작업 중...
 * 
 * 258   03-11-28 5:10p Paran				귓속말 수정
 * 
 * 257   03-11-28 10:15a Sk8snow2			OnPacketMemberChat 추가
 * 
 * 256   03-11-28 9:31a Sk8snow2			void	OnPacketBroadcastChat();	// 전체 채팅
											void	OnPacketSellingChat();	// 매매 채팅
											void	OnPacketGuildChat();	// 길드 채팅
											void	OnPacketPartyChat();	// 파티 채팅 
											추가
 * 
 * 255   03-11-21 10:00a Paran				몹 아이템 드롭 수정
 * 
 * 254   03-11-18 7:18p Sk8snow2			OnPacketInsertMapItem 함수 .. 몹 인덱스 번호 추가
 * 
 * 253   03-11-18 2:19p Sk8snow2			OnPacketMagicInfo 수정
 * 
 * 251   03-11-15 9:41p Paran				PK 부분 1차 수정
 * 
 * 250   03-11-15 11:41a Sk8snow2			OnPacketDamagePvP 수정
 * 
 * 249   03-11-15 11:07a Sk8snow2			OnPacketBeginPlayerAttack 수정
 * 
 * 247   03-11-13 11:39a Sk8snow2			OnPacketSkillUp 수정
 * 
 * 244   03-11-10 4:26a Sk8snow2			OnPacketBeginPlayerAttack() 수정 ( 기술등급이라는 필요없는 데이터 삭제 )
											OnPackerDamagePvP 패킷 관련 부분 추가 
 * 
 * 243   03-10-23 10:04p Paran				공격효과부분 수정.
 * 
 * 242   03-10-20 10:16p Paran				몬스터 데미지 부분 수정.
 * 
 * 241   03-10-15 11:14a Kjmgo				패킷 처리를 함수 포인터 사용
 * 
 * 238   03-10-09 3:45p Sk8snow2			OnPacketPartyBoardList 추가
 * 
 * 237   03-10-08 2:20p Paran
 * 
 * 236   03-10-07 11:59p Kjmgo				다른 사람이 레벨업을 했을때, 자기 한테 이펙트 나오던것 버그 수정
 *	
 * 234   03-10-03 5:50p Kjmgo				몬스터 상태가 반대로 먹던것을 수정
 * 
 * 233   03-10-02 5:48p Kjmgo				몬스터 hp 설정
 * 
 * 232   03-10-02 3:54p Kjmgo		Ride관련 수정 및, 캐릭터 삭제시, m_pPlayer 를 NULL하는 작업
 * 
 * 230   03-09-30 6:07p Kjmgo		이동중 말을 탔을때, 정지 할때까지 말이 보이지 않던 버그 해결
 * 
 * 229   03-09-30 5:29p Kjmgo		AddCharacter에서 기존에 있던 것이면 무시했던것을 지우고 새로 생성하게 수정
 * 
 * 228   03-09-30 4:09p Kjmgo		탈것 정보중, 플레이어( 나 자신 ) 의 정보면 전역 구조체에 저장
 * 
 * 226   03-09-29 10:34p Kjmgo		Damage가 제대로 뜨지 않던것 수정( 서버쪽에서도 수정 요망 - 몬스터 )
 * 
 * 225   03-09-29 5:05p Kjmgo		Picking용 Interface Pointer를 CGame 내부에서 전역으로 수정
 * 
 * 224   03-09-29 4:06p Kjmgo		Picking 수정.( 한프레임당 한개의 Level만 검사하도록. ( 나머지는 Skip ) )
 * 
 * 223   03-09-29 3:28p Kjmgo		m_pWorld->GetCharacter( m_dwPlayer ); 구문의 난무를 m_pPlayer로 대체
 * 
 * 217   03-09-22 11:37a Sk8snow	OnPacketGhostState 추가
 * 
 * 216   03-09-22 11:23a Sk8snow	OnPacketCharacterInsert
 *									OnPacketCharacterRemove 수정
 *									dPACKET_CHARACTER_INSERT
 *									dPACKET_CHARACTER_CONNECT
 *									dPACKET_CHARACTER_REMOVE
 *									dPACKET_CHARACTER_DISCONNECT
 *									통합
 * 
 * 213   03-09-18 10:58a Kjmgo		데미지 처리 관련 수정
 * 
 * 208   03-09-09 5:45a Kjmgo		몬스터 속도 수정
 * 
 * 206   03-09-08 8:20p Kjmgo		동작~ 작업
 * 
 * 205   03-09-07 12:26p Sk8snow	OnPacketMobState 추가
 * 
 * 204   03-09-07 12:13p Sk8snow	OnPacketMonsterInsert 수정
 * 
 * 202   03-09-07 1:32a Sk8snow		OnPacketNpcInfo
 * 
 * 196   03-08-14 10:03a Kjmgo		마법스크립트 두개로 분리
 * 
 * 194   03-08-13 10:54a Sk8snow	OnPacketBeginCastMagic2 추가
 * 
 * 191   03-08-11 12:23p Sk8snow	Character Connect시 탈것과 스피드 정보 추가
 * 
 * 190   03-08-10 6:59p Admin		말 타고내리기 적용
 * 
 * 147   03-06-27 5:30p Kjmgo		Assert처리
 * 
 * 117   03-06-20 11:50a Paran		스킬 바 수정.
 * 
 * 116   03-06-19 8:31p Paran		스킬 바 수치 수정.
 * 
 * 101   03-06-09 7:15p Kjmgo		Critical Hit 추가
 * 
 * 97    03-06-03 11:38p Paran		용병 패킷처리
 * 
 * 94    03-05-29 2:28p Paran		아이템 손바꾸기 패킷 수정.
 * 
 * 93    03-05-28 1:31p Paran		스킬 안맞는 부분, 공격패킷 수정
 * 
 * 92    03-05-27 8:41p Paran		활공격, 폴더 정리
 * 
 * 89    03-05-23 3:43p Paran		인터페이스 관련 수정.
 *
 * 88    03-05-23 4:05a Paran		공격 가능 거리 추가.
 *
 * 87    03-05-21 1:26p Paran		STR, INT, DEX, TOTAL 레벨 관련 패킷 추가.
 *
 * 84    03-05-17 4:31p Paran		파티원 보이기 부분 수정.
 *
 * 81    03-05-16 11:03a Paran		이름 출력 관련 수정.
 *
 * 75    03-05-15 9:04p Paran		파티 관련 이름 출력.
 *
 * 68    03-05-11 10:31a Kjmgo		타격치 출력.
 *
 * 62    03-05-08 1:30a Paran		채팅 텍스트 출력 및 상점 패킷 디버깅.
 *
 * 59    03-05-06 9:27p Ahastudio	공격 패킷 처리 방식 수정.
 *
 * 57    03-05-06 6:18p Paran		상점 기능 추가.
 *
 * 56    03-05-05 1:40p Ahastudio	AddPlayer -> CreatePlayer()
 *
 * 50    03-04-26 9:27p Paran		몬스터 공격 패킷 수정.
 *
 * 49    03-04-26 10:58a Paran		마법 서버 저장 처리.
 *
 * 46    03-04-25 5:54p Paran		INSERT_MONSTER 패킷 처리 부분 수정.
 *
 * 38    03-04-15 1:26a Paran		아이템 장착 부분 수정.
 *
 * 37    03-04-12 3:04p Paran		캐릭터 장착 아이템 변경 기능 추가.
 *
 * 36    03-04-11 5:29p Paran		캐릭터 장착 부분 추가.
 *
 * 34    03-04-10 6:27p Paran		캐릭터 접속, 캐릭터 추가 부분 수정.
 *
 * 29    03-04-03 3:58a Ahastudio	마법 종류에 따라 다른 마법이 보이게 수정.
 *
 * 28    03-04-02 7:21p Paran		플레이어 아이템 추가.
 *
 * 25    03-03-28 1:17a Paran		스킬바 추가, 캐릭터 셀렉트 부분 수정.
 *
 * 19    03-03-26 6:22a Paran		아이템 서버 연결.
 *
 * 16    03-03-25 3:36a Paran		인벤토리 아이템 서버와 연결.
 *
 * 12    03-03-21 5:44a Ahastudio	스킬에 따른 공격 변화.
 *
 * 3     03-03-15 2:34a Ahastudio	사망 메세지 박스 수정.
 *
 * 2     03-03-14 2:30p Ahastudio	ProcessRecvPacket 및 OnPacket을 GameProcessPacket.cpp로 이동.
 * <HR>
 *
 * @file	GameProcessPacket.cpp
 */


#include "Global.h"
#include "Game.h"
#include <string>
#include <vector>

#include <io.h>
#include <errno.h>


using std::string;
using std::vector;

int	test=3;

/**
 * 받은 패킷을 처리한다.
 * 실제로는 패킷 TAG에 따라 처리 함수를 호출한다.
 */
void CGame::ProcessRecvPacket()
{
	WORD wPacket = m_recvPacket.GetTag();
		
	if( wPacket >= dPACKET_MAX_VALUE )
	{
		// -_-;;;
		return ;
	} //if

	(this->*m_apPacketProcess[wPacket])();
	
} //CGame::ProcessRecvPacket

/**
 *
 *
 */
void CGame::OnPacketNull()
{
	//-- NULL --//
	Assert( 0 );
	return ;
} //CGame::OnPacketNull

/**
 * 맵 로딩 패킷을 처리한다. (dPACKET_MAPLOADING)
 * 맵 로딩을 성공적으로 마치면 dPACKET_MAPLOADING_END를 서버로 보낸다.
 * 맵 로딩을 실패하면 서버와의 접속을 끊는다.
 */
void CGame::OnPacketMapLoading()
{
	Assert( m_pWorld );
	Assert( g_pClient );

	WORD	wMap;	// 맵 번호
	int		nX;		// 플레이어 X 좌표
	int		nY;		// 플레이어 Y 좌표
	BYTE	btType = 0;
	BYTE	btLayer;

	m_recvPacket
		>> wMap
		>> nX
		>> nY
		>> btLayer
		>> btType
		>> g_btWarMode
		>> g_nDefenseGuild
		>> g_nAttackGuild;

	if( wMap == 0 || wMap == 1 || wMap == 2)
	{
		wMap = test;
	}

	DEBUG_OUT( "*맵 로딩 - " + wMap );

	m_bPlaying = FALSE;

	m_pPlayer = NULL;
	m_btMapType = btType;

	m_pWorld->LoadLightTable( wMap );

	if( btType < 2 )
	{
		m_bIndoor = FALSE;
		if( !LoadMap( wMap ) )
		{
			Assert( !"맵 로드 실패" );
			g_pClient->Disconnect();
			return;
		} //if
	}
	else
	{
		if( !LoadIndoor( wMap ) )
		{
			Assert( !"Indoor Map Load 실패" );
			g_pClient->Disconnect();
		} //if

		m_bIndoor = TRUE;
	} //if..else..

	if( m_btMapType == 0 && !m_bIndoor )	
	{
		g_pGUI->LoadMiniMap( wMap );
	}
	else
	{
		g_pGUI->LoadMiniMap( 0 );
	} //if..else..

	CreatePlayer( nX, nY, btLayer );

	m_bPlaying		= TRUE;
	g_sTarget.Clean();

	// 맵 로딩 완료 패킷 전송.
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MAPLOADING_END );

	test = wMap;
		
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnPacketMapLoading


int nCharacterCount = 0;


/**
 * 캐릭터 추가 패킷을 처리한다.
 * 플레이어 추가는 이 패킷을 통해서 처리되지 않는다.
 * 새로 게임에 접속한 캐릭터가 아니라
 * 기존에 게임에 존재하던 캐릭터가 주위 영역으로 들어올 때
 * 이 패킷을 받는다.
 * @see		OnPacketCharacterConnect
 */
void CGame::OnPacketCharacterInsert()
{
	Assert( m_pWorld );

	DWORD				dwId;				// 캐릭터 ID
	SW3DCharacterInfo	infoCharacter;		// 캐릭터 정보
	BYTE				btMercType;
	WORD				wSkill;				// 스킬 아이템 번호 
	BYTE				btIsConnect;		
	BYTE				btIsDead;		
	char				guildName[64];
	WORD				guildMark;
	BYTE				btIsVisible;		// 않보이는 상태인지 아닌지
	BYTE				btPetType;			// 팻 종류 ( 0 이면 없음 )	
	BYTE				btIsFishing;		// 낚시 상태인가
	int					nX, nY;				// 낚시중일경우 방향좌표
	string				szPetName;			// Pet Name
	BYTE				btClass;			// 직업 
	int					nTotalLevel;		// 통합레벨
	BYTE				btGuildLevel;		// 길드 등급 0 = 등급 없음 , 1 = 단원 , 2 = 부단장 , 3 = 단장
	BYTE				btIsShopkeeper;		// 개인상점여부 ( 1 = 개인상점 오픈중, 0 = 개인상점 아님 )
	string				szShopName;			// 개인상점 이름
	BYTE				btR, btG, btB;		// 이름색 

	// btIsVisible 값이 1이면 해당 케릭터는 화면에 찍지 않음
	// 0이면 찍음 .. ( 추가된 dPACKET_SET_VISIBLE 에서 해당관련 처리 )

	// 길드 이름이 " " <== 이면 ;; 
	// 길드이름 없는것임 .. guildMark 값이 0 이면
	// 길드마크 없는 것임

	// from sk8snow

	m_recvPacket
		>> dwId
		>> infoCharacter.szName
		>> infoCharacter.bSex
		>> btIsConnect
		>> infoCharacter.nX
		>> infoCharacter.nY
		>> infoCharacter.wHair
		>> infoCharacter.wFace
		>> infoCharacter.wLongWeapon
		>> infoCharacter.wShortWeapon
		>> infoCharacter.wHand
		>> infoCharacter.wHelmet
		>> infoCharacter.wUpper
		>> infoCharacter.wLower
		>> infoCharacter.wShield
		>> infoCharacter.wFoot
		>> btMercType						///< 용병(영웅) 타입
		>> infoCharacter.btLayer
		>> wSkill
		>> infoCharacter.wRide
		>> infoCharacter.btOffset
		>> btIsDead
		>> guildName
		>> guildMark
		>> btIsVisible
		>> btPetType
		>> btIsFishing
		>> nX
		>> nY
		>> szPetName
		>> btClass
		>> nTotalLevel
		>> btGuildLevel
		>> btIsShopkeeper
		>> szShopName
		>> infoCharacter.nGuildId
		>> btR 
		>> btG 
		>> btB;
		// from sk8snow
		
	if( _MASTER == infoCharacter.bSex )
	{
		infoCharacter.wHair			= 10373;
		infoCharacter.wFace			= 10372;
		
		infoCharacter.wUpper		= 10375;
		infoCharacter.wLower		= 10376;
		infoCharacter.wHand			= 10374;
		infoCharacter.wFoot			= 10377;
		infoCharacter.wHelmet		= 0;
		infoCharacter.wShield		= 0;
		infoCharacter.wLongWeapon	= 0;
		infoCharacter.wShortWeapon	= 0;
		
		infoCharacter.wRide			= 0;		
	}

	IW3DCreature*		pCharacter = m_pWorld->GetCharacter( dwId );
	if( pCharacter )
	{	// 이미 존재하는 캐릭터라면 기존의 정보를 제거한다.
		m_pWorld->RemoveCharacter( dwId );
	} //if

	m_pWorld->AddCharacter( dwId, &infoCharacter );

	if( g_pPickCharacter && g_pPickCharacter->GetId() == dwId )
	{
		g_pPickCharacter = m_pWorld->GetCharacter( dwId );
	} //if

	if( btIsConnect )
	{
		Vector3		vPos;
		vPos.x = static_cast<float>( infoCharacter.nX ) + 0.5F;
		vPos.y = static_cast<float>( infoCharacter.nY ) + 0.5F;
		vPos.z = m_pWorld->GetHeight( vPos );
		
		m_pWorld->AddEffect( "loading.wed", vPos );
	} //if

	pCharacter = m_pWorld->GetCharacter( dwId );
	if( !pCharacter )
	{
		Assert( 0 );
		return;
	}//if

	g_pGUI->AddPlayerList( dwId, infoCharacter.szName, btClass, nTotalLevel );

	pCharacter->SetGuildData( guildName, infoCharacter.nGuildId, btGuildLevel, guildMark );
	if( btIsShopkeeper )
	{
		pCharacter->SetShopInfo( szShopName.c_str() );
	} //if

	if( g_pGUI->IsPartyMember( dwId ) )
	{ // 파티원이라면...
		pCharacter->VisibleName( TRUE );
	} //if

	pCharacter->SetAttackMode( wSkill );	

	// 이름 색 from sk8snow
	pCharacter->SetNameColor( COLOR_ARGB( 0xFF, btR, btG, btB ) );


	if( btIsVisible )
	{
		if( dwId == g_dwPlayerId )	pCharacter->Ghost( btIsVisible );
		else						pCharacter->SetVisible( !btIsVisible );		
	}

	if( btIsFishing )
	{
		Vector3 vPos  = pCharacter->GetPosition();
		Vector3 vPick = Vector3( ( float )nX, ( float )nY, vPos.z );
		Vector3 vDir  = vPick - vPos;		
		
		pCharacter->Stop();
		pCharacter->SetDirection( vDir );
		pCharacter->Hook();		
	}

	nCharacterCount++;
	//m_pWorld->AddMercenary( btMercType, dwId );
} //CGame::OnPacketCharacterInsert

/**
 * 캐릭터 삭제 패킷을 처리한다.
 * 캐릭터가 게임을 종료하는 경우가 아니라
 * 처리 영역을 벗어난 캐릭터를 삭제하기 위해 사용된다.
 * @see		OnPacketCharacterDisconnect
 */
void CGame::OnPacketCharacterRemove()
{
	Assert( m_pWorld );

	DWORD	dwId;
	BYTE	btIsDisconnect;

	m_recvPacket	
		>> dwId
		>> btIsDisconnect;

	if( g_pPickCharacter && g_pPickCharacter->GetId() == dwId )
	{
		g_pPickCharacter = NULL;
	} //if

	IW3DCreature*		pCharacter = m_pWorld->GetCharacter( dwId );
	if( pCharacter )
	{
		if( btIsDisconnect )
		{
			Vector3		vPos = pCharacter->GetPosition();
			m_pWorld->AddEffect( "loading.wed",  vPos );
		}

		g_pGUI->RemovePlayerList( dwId, pCharacter->GetName() );
		m_pWorld->RemoveCharacter( dwId );

		nCharacterCount--;
	} //if
} //CGame::OnPacketCharacterRemove



/**
 * 캐릭터 이동 패킷을 처리한다.
 * 플레이어가 직접 조작하는 캐릭터가 아닌,
 * 다른 캐릭터들의 이동에 관한 패킷만 받아서 처리한다.
 */
void CGame::OnPacketCharacterMove()
{
	Assert( m_pWorld );

	DWORD	dwId;		// 캐릭터 ID
	int		nX;			// 캐릭터 X 좌표
	int		nY;			// 캐릭터 Y 좌표
	BYTE	btLayer;

	m_recvPacket
		>> dwId
		>> nX
		>> nY
		>> btLayer;

	Assert( dwId != m_dwPlayer );

	Vector3		vPos;
	vPos.x = static_cast<float>( nX ) + 0.5F;
	vPos.y = static_cast<float>( nY ) + 0.5F;
	vPos.z = m_pWorld->GetHeight( vPos );

	IW3DCreature*		pCharacter = m_pWorld->GetCharacter( dwId );

	if( pCharacter )
	{
		pCharacter->MoveTo( vPos );
		//pCharacter->SetGoalPosition( vPos );
		pCharacter->SetLayer( btLayer );
	} //if
} //CGame::OnPacketCharacterMove

/**
 * 캐릭터 방향 패킷을 처리한다.
 */
void CGame::OnPacketCharacterDirection()
{
	Assert( m_pWorld );

	DWORD	dwId;		// 캐릭터 ID
	BYTE	btDir;		// 캐릭터가 바라보는 방향

	m_recvPacket
		>> dwId
		>> btDir;

	IW3DCreature*		pCharacter = m_pWorld->GetCharacter( dwId );

	Assert( pCharacter );

	if( pCharacter )
	{
		// @todo	방향 처리.
	} //if
} //CGame::OnPacketCharacterDirection

/**
 * 몬스터 추가 패킷을 처리한다.
 */
void CGame::OnPacketMonsterInsert()
{
	Assert( m_pWorld );

	WORD				wMonster;		// 몬스터 ID
	SW3DMonsterInfo		infoMonster;	// 몬스터 정보

	int	nHP, nMaxHP;
	BYTE isCreate;

//	BYTE btOffset2;						// 몬스터 이동속력2 ( 전투모드 )
//	BYTE btState;						// 몬스터 상태( 0 = 일반, 1 = 전투 )

	m_recvPacket
		>> wMonster						// 몬스터 ID
		>> infoMonster.wType			// 몬스터 종류
		>> infoMonster.nX				// 몬스터 X 좌표
		>> infoMonster.nY				// 몬스터 Y 좌표
		>> infoMonster.btOffset			// 몬스터 이동 속도
		>> infoMonster.btOffset2		// 몬스터 이동속력2 ( 전투모드 )
		>> infoMonster.btState			// 몬스터 상태( 0 = 일반, 1 = 전투 )
		>> nHP							// 체력
		>> nMaxHP						// 최대체력 
		>> isCreate;					// 새로 생성되는 몬스터 인가

	IW3DCreature*	pMonster = m_pWorld->GetMonster( wMonster );
	if( pMonster )
	{	// 몬스터 정보가 이미 존재하는데 이 패킷이 왔다면 기존의 데이터를 제거한다.
		m_pWorld->RemoveMonster( wMonster );

		if( g_pPickMonster && g_pPickMonster->GetId() == wMonster )
		{
			g_pPickMonster = NULL;
		} //if
	} //if

	m_pWorld->AddMonster( wMonster, reinterpret_cast<void*>( &infoMonster ) );
	pMonster = m_pWorld->GetMonster( wMonster );

	if( pMonster )
	{
		pMonster->SetHp( nHP, nMaxHP );
	} //if

	Vector3		vPos;
	vPos.x = static_cast<float>( infoMonster.nX ) + 0.5F;
	vPos.y = static_cast<float>( infoMonster.nY ) + 0.5F;
	vPos.z = m_pWorld->GetHeight( vPos );

//	if( isCreate )
//		m_pWorld->AddEffect( "mob_appear.wed", vPos );

	g_dtGame.wMonsterCount++;
} //CGame::OnPacketMonsterInsert

/**
 * 몬스터 삭제 패킷을 처리한다.
 */
void CGame::OnPacketMonsterRemove()
{
	Assert( m_pWorld );

	WORD	wId;

	m_recvPacket	>> wId;

	IW3DCreature*	pMonster = m_pWorld->GetMonster( wId );

	Assert( pMonster );

	if( g_pPickMonster && wId == g_pPickMonster->GetId() )
	{
		g_pPickMonster = NULL;
	} //if

	if( pMonster )
	{
		m_pWorld->RemoveMonster( wId );
	} //if

	g_dtGame.wMonsterCount--;

} //CGame::OnPacketMonsterRemove

/**
 * 몬스터 이동 패킷을 처리한다.
 */
void CGame::OnPacketMonsterMove()
{
	Assert( m_pWorld );

	WORD	wMonster;	// 몬스터 ID
	int		nX;			// 몬스터 X 좌표
	int		nY;			// 몬스터 Y 좌표

	m_recvPacket
		>> wMonster
		>> nX
		>> nY;

	Vector3		vPos;
	vPos.x = static_cast<float>( nX ) + 0.5F;
	vPos.y = static_cast<float>( nY ) + 0.5F;
	vPos.z = m_pWorld->GetHeight( vPos );

	IW3DCreature*	pMonster = m_pWorld->GetMonster( wMonster );

	if( pMonster )
	{
		pMonster->SetPosition( pMonster->GetGoalPosition() );
		pMonster->SetGoalPosition( vPos );
	} //if
} //CGame::OnPacketMonsterMove

/**
 * 일반 채팅 패킷을 처리한다.
 */
void CGame::OnPacketChatMsg()
{
	Assert( g_pGUI );

	DWORD	dwId;			// 캐릭터 ID
	string	sText;			// 대화 텍스트

	m_recvPacket
		>> dwId
		>> sText;

	IW3DCreature*		pCharacter = m_pWorld->GetCharacter( dwId );

	Assert( pCharacter );

	if( pCharacter )
	{
		g_pGUI->ChattingInput( pCharacter->GetName(), sText.c_str() , COLOR_ARGB( 255, 224, 224, 224 ) );

		pCharacter->SetTalkText( const_cast<char*>( sText.c_str() ) );
	}
} //CGame::OnPacketChatMsg

/**
 * 귓속말 패킷을 처리한다.
 */
void CGame::OnPacketChatWhisper()
{
	Assert( g_pGUI );

	string	sName;		// 대화 상대
	string	sText;		// 귓말 내용

	m_recvPacket
		>> sName
		>> sText;

	DWORD	dwColor;
	if( sName == "공지" )		dwColor = 0xFF00C0FF;
	else if( sName == " " )		dwColor = 0xFF00FF00;
	else
	{
		g_pGUI->SaveWhisperName( sName.c_str() );
		dwColor = 0xFFFFA60C;
	} //if..else..

	g_pGUI->ChattingInput( sName.c_str(), sText.c_str(), dwColor, 0 );
} //CGame::OnPacketChatWhisper

/**
 * 귓속말 전송 결과 패킷을 처리한다. 
 */
void CGame::OnPacketChatWhisperResult()
{
	Assert( g_pGUI );

	BYTE	bResult;
	m_recvPacket	>> bResult;

	if( !bResult )
	{
		g_pGUI->ChattingInput( "", STR_CHATT_FAILTRANSMIT, 0xFFCF1818 );
		return;
	} //if

	Assert( m_pPlayer );

	if( m_pPlayer )
	{
		char*	pText = g_pGUI->GetChattingText();
		g_pGUI->ChattingInput( m_pPlayer->GetName(), pText, 0xFFFFA60C );
	} //if
} //CGame::OnPacketChatWhisperResult

/**
 * 몬스터 HP 정보 패킷을 처리한다.
 * 몬스터와 전투를 시작하면 날아오는 패킷이다.
 */
void CGame::OnPacketMobHpInfo()
{
	Assert( m_pWorld );

	WORD		wMonster;		// 몬스터 ID
	int			nHp;			// 몬스터 HP
	int			nTotHp;			// 몬스터 Total HP

	m_recvPacket
		>> wMonster
		>> nHp
		>> nTotHp;

	DEBUG_OUT( "*몬스터 HP - " + wMonster + " : " + nHp + "/" + nTotHp );

	IW3DCreature*	pMonster = m_pWorld->GetMonster( wMonster );

	Assert( pMonster );

	if( pMonster )
	{
		pMonster->SetHp( nHp, nTotHp );
	} //if
} //CGame::OnPacketMobHpInfo

/**
 * 플레이어->몬스터 공격 시작 패킷을 처리한다.
 */
void CGame::OnPacketBeginPlayerAttack()
{
	DEBUG_OUT( "*플레이어->몬스터 공격 시작" );

	int		nTarget;		// 공격대상 고유 번호
	DWORD	dwCharacter;	// 공격하는 캐릭터 고유 번호
	WORD	wSkill;			// 기술 번호 ( 0 = 근거리, 1 = 원거리, 마법은 마법 스크롤 아이템 번호 )
	BYTE	btTargetKind;	// 공격 대상 ( 0 = 플레이어, 1 = 몹 )
	int		nActionTime;

	m_recvPacket
		>> nTarget
		>> dwCharacter
		>> wSkill
		>> btTargetKind
		>> nActionTime;

	IW3DCreature*	pCharacter	= m_pWorld->GetCharacter( dwCharacter );
	IW3DCreature*	pTarget = NULL;
	if( btTargetKind == 0 )
	{
		pTarget = m_pWorld->GetCharacter( nTarget );
	}
	else if( btTargetKind == 1 )
	{
		pTarget = m_pWorld->GetMonster( nTarget );
	}

	if( !pCharacter || !pTarget )
	{
		return;
	} //if

	if( //pCharacter->GetState() == STATE_ATTACK ||
		pCharacter->GetState() == STATE_DEAD )
	{
		return;
	} //if

	Vector3		vDir = pTarget->GetPosition() - pCharacter->GetPosition();

	if( pCharacter->GetPosition() == pTarget->GetPosition() )
	{
		vDir = Vector3( 0.0F, 1.0F, 0.0F );
	} //if

	pCharacter->SetDirection( vDir );
	g_pGUI->SetPlayerMapDir( vDir.x, vDir.y );
	
	//-- To Sk8snow 
	//		Test로는 2초로.. 나머진 알아서 Tick단위로
	//
	pCharacter->SetActionSpeed( nActionTime );//g_dwAttackLimit );
	pCharacter->Attack( pTarget );


	m_bSendAttackPacket = FALSE;

	/*
	
	
	*/
	if( g_pHelper && dwCharacter == m_dwPlayer )
	{
		g_pHelper->AttackMonster( pTarget->GetName() );
	} //if


} //CGame::OnBeginPlayerAttack

/**
 * 유저->몬스터 공격 결과 패킷을 처리한다.
 */
void CGame::OnPacketDamageUser()
{
	Assert( m_pWorld );

	WORD	wMonster;		// 공격대상 몬스터 고유 번호
	DWORD	dwCharacter;	// 공격하는 캐릭터 고유 번호
	WORD	wDamage;		// 타격치
	BYTE	btCritical;
	
	m_recvPacket
		>> wMonster
		>> dwCharacter
		>> wDamage
		>> btCritical;

	IW3DCreature*	pCharacter	= m_pWorld->GetCharacter( dwCharacter );
	IW3DCreature*	pMonster	= m_pWorld->GetMonster( wMonster );

	if( pCharacter && pMonster )
	{
		if( m_dwPlayer == dwCharacter )
		{
			DEBUG_OUT( "Damage : " + wDamage );

			pMonster->Damage( wDamage, btCritical );
			pCharacter->AddTarget( pMonster );
			pMonster->AddAttacker( pCharacter );
						
			int		nHp[2];
			pMonster->GetHp( nHp[0], nHp[1] );
			nHp[0] -= wDamage;
			pMonster->SetHp( nHp[0], 0 );
			if( g_pHelper )
			{
				g_pHelper->AttackMonster( pMonster->GetName() );
			} //if
		}
	} //if
} //CGame::OnPacketDamageUser

/**
 * 몬스터->유저 공격 패킷을 처리함.
 */
void CGame::OnPacketDamageMob()
{
	Assert( m_pWorld );

	WORD		wMonster;		// 몬스터 ID
	DWORD		dwCharacter;	// 캐릭터 ID
	WORD		wDamage;		// 데미지 수치
	BYTE		btCritical;		// 크리티컬
	WORD		wMagicType;		// 마법 종류
	WORD		wMagicIndex;	// 마법 인덱스 
	DWORD		dwAttackTime;	// 공격 딜레이


	m_recvPacket
		>> wMonster
		>> dwCharacter
		>> wDamage
		>> btCritical
		>> wMagicType
		>> wMagicIndex
		>> dwAttackTime;

/*	if( dwCharacter == m_dwPlayer )
	{	// 공격 당하는 게 플레이어 본인일 경우, HP를 갱신한다.
		g_infoPlayer.nHp[0] -= wDamage;
		if( g_infoPlayer.nHp[0] < 0 )
		{
			g_infoPlayer.nHp[0] = 0;
		} //if
	} //if*/

	IW3DCreature*	pCharacter	= m_pWorld->GetCharacter( dwCharacter );
	IW3DCreature*	pMonster	= m_pWorld->GetMonster( wMonster );

	if( pCharacter && pMonster )
	{
		if( pMonster->GetState() == STATE_DEAD )
		{
			return;
		} //if

		Vector3		vDir = pCharacter->GetPosition() - pMonster->GetPosition();
		if( pCharacter->GetPosition() == pMonster->GetPosition() )
		{
			vDir = Vector3( 0.0F, 1.0F, 0.0F );
		} //if
		pMonster->SetDirection( vDir );

		pCharacter->Damage( wDamage, btCritical );
		pMonster->Attack( pCharacter );
	//	pMonster->AddTarget( pCharacter );
	//	pCharacter->AddAttacker( pMonster );
		
		pCharacter->Damage( FALSE, pMonster );
		//pMonster->AddTarget( pCharacter );
		
		/*
		 *	몬스터 동작 시간 ( 몇초 안에 끝낼것인가 )
		 */
		pMonster->SetActionSpeed( dwAttackTime );
	} //if

	DEBUG_OUT( "-=-=-=- RecvPacket - OnPacketDamageMob -=-=-=-" );
} //CGame::OnPacketDamageMob

/**
 * 캐릭터 사망.
 */
void CGame::OnPacketDieUser()
{
	Assert( m_pWorld );

	DWORD	dwCharacter;	// 캐릭터 ID

	m_recvPacket	>> dwCharacter;

	IW3DCreature*		pCharacter = m_pWorld->GetCharacter( dwCharacter );
	if( pCharacter )
	{
		pCharacter->Dead();
	} //if

	if( IsPlayer( dwCharacter ) )
	{	// 플레이어가 사망한 경우.
		g_pGUI->PlayerLive( FALSE );
	} //if
} //CGame::OnPacketDieUser

/**
 * 몬스터 사망.
 */
void CGame::OnPacketDieMob()
{
	Assert( m_pWorld );

	WORD	wMonster;		// 몬스터 ID

	m_recvPacket	>> wMonster;

	IW3DCreature*		pMonster = m_pWorld->GetMonster( wMonster );
	if( pMonster )
	{
		pMonster->Dead();
	} //if
} //CGame::OnPacketDieMob

/**
 * 스킬 업.
 */
void CGame::OnPacketSkillUp()
{
	Assert( m_pWorld );
	Assert( g_pGUI );

	BYTE	bSkill;		// 레벨업한 스킬의 종류
	WORD	wSKillLev;	// 스킬 레벨 
	int		nExp;		// 레벨업한 스킬의 다음 레벨까지의 필요 경험치 ( MaxExp )
	int		nExp2;		// 잔여 EXP
	
	m_recvPacket
		>> bSkill
		>> wSKillLev
		>> nExp
		>> nExp2;

	g_infoPlayer.wLevel[bSkill] = wSKillLev;
	g_infoPlayer.nExp[0][1] = nExp;
	g_infoPlayer.nExp[1][1] = nExp;
	g_infoPlayer.nExp[2][1] = nExp;
	g_infoPlayer.nExp[3][1] = nExp;
	g_infoPlayer.nExp[bSkill][0] = nExp2;

	g_pGUI->SetPosSkill( 0, g_infoPlayer.nExp[0][1], g_infoPlayer.nExp[0][0], FALSE );
	g_pGUI->SetPosSkill( 1, g_infoPlayer.nExp[1][1], g_infoPlayer.nExp[1][0], FALSE );
	g_pGUI->SetPosSkill( 2, g_infoPlayer.nExp[2][1], g_infoPlayer.nExp[2][0], FALSE );
	g_pGUI->SetPosSkill( 3, g_infoPlayer.nExp[3][1], g_infoPlayer.nExp[3][0], FALSE );

	if( m_pPlayer )
	{
		switch( bSkill )
		{
		case 0:		m_pWorld->AddMagic( STR_SKILL_LVUP_SHORT, m_pPlayer, m_pPlayer );		break;
		case 1:		m_pWorld->AddMagic( STR_SKILL_LVUP_LONG, m_pPlayer, m_pPlayer );		break;
		case 2:		m_pWorld->AddMagic( STR_SKILL_LVUP_BLACK, m_pPlayer, m_pPlayer );		break;
		case 3:		m_pWorld->AddMagic( STR_SKILL_LVUP_WHITE, m_pPlayer, m_pPlayer );		break;
		} //switch

		if( g_pHelper )
		{
			switch( bSkill )
			{
				case 0:	 g_pHelper->Talk( "_SHORT_SKILL_UP_" );	break;
				case 1:	 g_pHelper->Talk( "_LONG_SKILL_UP_" );	break;
				case 2:	 g_pHelper->Talk( "_BMAGIC_UP_" );	break;
				case 3:	 g_pHelper->Talk( "_WMAGIC_UP_" );	break;
			} //switch
		} //if
	} //if

	g_pGUI->SetText( "TXT_CINFO_SK1", g_infoPlayer.wLevel[0] );
	g_pGUI->SetText( "TXT_CINFO_SK2", g_infoPlayer.wLevel[1] );
	g_pGUI->SetText( "TXT_CINFO_SK3", g_infoPlayer.wLevel[2] );
	g_pGUI->SetText( "TXT_CINFO_SK4", g_infoPlayer.wLevel[3] );
	
	int		nTotal = g_infoPlayer.wLevel[0] + 
					 g_infoPlayer.wLevel[1] + 
					 g_infoPlayer.wLevel[2] + 
					 g_infoPlayer.wLevel[3];
	g_pGUI->SetText( "TXT_CINFO_TOTAL", nTotal );
	g_pGUI->SetText( "TXT_TOT_TOTLEVEL", nTotal );
	g_pGUI->SetProgressPos( "PRO_CINFO_TOTAL", 400, nTotal, FALSE );

} //CGame::OnPacketSkillUp

/**
 * 경험치 얻음.
 */
void CGame::OnPacketGetExp()
{
	Assert( m_pWorld );
	Assert( g_pGUI );

	BYTE	bSkill;		// 경험치를 얻은 스킬 종류
	WORD	wExp;		// 얻음 경험치

	m_recvPacket
		>> bSkill
		>> wExp;

	g_infoPlayer.nExp[bSkill][0] += wExp;
	
	g_pGUI->SetPosSkill( bSkill, g_infoPlayer.nExp[bSkill][1], g_infoPlayer.nExp[bSkill][0] );
} //CGame::OnPacketGetExp

/**
 * 다른 플레이어의 스킬 업.
 * 레벨업 이펙트만 표현한다.
 */
void CGame::OnPacketSkillUpEvent()
{
	Assert( m_pWorld );

	DWORD	dwCharacter;

	m_recvPacket	>> dwCharacter;

	IW3DCreature*		pCharacter = m_pWorld->GetCharacter( dwCharacter );

	if( pCharacter && m_pPlayer->GetId() == g_dwPlayerId )
	{
		m_pWorld->AddMagic( "레벨업", pCharacter, pCharacter );
	} //if
} //CGame::OnPacketSkillUpEvent

/**
 * 플레이어의 체력 회복.
 */
void CGame::OnPacketHpGainUser()
{
	Assert( g_pGUI );

	m_recvPacket	>> g_infoPlayer.nHp[0];

	if( !m_pPlayer )
	{
		Assert( !"내가 없다" );
		return;
	} //if

	m_pPlayer->SetHp( g_infoPlayer.nHp[0], g_infoPlayer.nHp[1] );
} //CGame::OnPacketHpGainUser

/**
 * 플레이어의 마나 회복.
 */
void CGame::OnPacketMpGainUser()
{
	Assert( g_pGUI );

	m_recvPacket	>> g_infoPlayer.nMp[0];

	g_pGUI->SetPosMP( g_infoPlayer.nMp[1], g_infoPlayer.nMp[0] );
} //CGame::OnPacketMpGainUser

/**
 * 몬스터 체력 회복.
 */
void CGame::OnPacketHpGainMob()
{
 	Assert( g_pGUI );
	Assert( m_pWorld );

	IW3DCreature*		pMonster = m_pWorld->GetMonster( g_sTarget.dwId );
	if( pMonster )
	{
		int		nHp[2];
		m_recvPacket	>> nHp[0];
		pMonster->SetHp( nHp[0] );
	} //if
} //CGame::OnPacketHpGainMob

/**
 * 맵에 아이템 나타나기.
 */
void CGame::OnPacketInsertMapItem()
{
	Assert( m_pWorld );

	DWORD	dwId;		// 아이템 고유 번호
	WORD	wType;		// 아이템 종류
	int		nX;			// 맵상 X 좌표
	int		nY;			// 맵상 Y 좌표
	int		nVal;		// 아이템 추가 정보
	WORD	wMobIdx;	// 떨구는 몹의 고유번호(몹이 떨구는게 아니면 0)

	m_recvPacket
		>> dwId
		>> wType
		>> nX
		>> nY
		>> nVal
		>> wMobIdx;

	IW3DItem*	pItem = m_pWorld->GetItem( dwId );
	if( pItem )
	{
		m_pWorld->RemoveItem( dwId );
	} //if

	m_pWorld->AddItem( dwId, wType, nX, nY, nVal, wMobIdx );

	DEBUG_OUT( "*아이템 " + wType + " 추가 " + nX + "," + nY );
} //CGame::OnPacketInsertMapItem

/**
 * 맵에서 아이템 사라지기.
 */
void CGame::OnPacketRemoveMapItem()
{
	Assert( m_pWorld );

	DWORD	dwId;

	m_recvPacket	>> dwId;

	if( g_pPickItem && g_pPickItem->GetId() == dwId )
	{
		g_pPickItem = NULL;
	} //if

	m_pWorld->RemoveItem( dwId );
} //CGame::OnPacketRemoveMapItem

/**
 * 인벤토리에 아이템 추가.
 */
void CGame::OnPacketInsertInventory()
{
	Assert( g_pGUI );

	DWORD	dwId;		// 아이템 고유 번호
	DWORD	dwType;		// 아이템 종류
	BYTE	btPage;		// 페이지
	BYTE	btX;		// X
	BYTE	btY;		// Y 
	WORD	btContainCnt;	// 수량(포션용) from sk8snow
//	BYTE	btRange;
	// from sk8snow
	m_recvPacket
		>> dwId
		>> dwType
		>> btPage
		>> btX
		>> btY
		>> btContainCnt;
		//>> btRange;

	g_pGUI->InsertInventory( dwId, dwType, btPage, btX,	btY, btContainCnt );	
//	m_fAttackRange = btRange;
	DEBUG_OUT( "*인벤토리 아이템 추가!" + "아뒤 : " + dwId + "타입 : " + dwType );
	//DEBUG_OUT( "*인벤토리 아이템 추가!" + "아뒤 : " + dwId + "타입 : " + dwType + "공격 거리 : " + m_fAttackRange );
} //CGame::OnPacketInsertInventory

/**
 * 인벤토리에서 아이템 삭제.
 */
void CGame::OnPacketRemoveInventory()
{
	Assert( g_pGUI );

	DWORD	dwId;

	m_recvPacket	>> dwId;

	g_pGUI->RemoveItemInInventory( dwId );
	DEBUG_OUT( "*인벤토리 아이템 삭제!" + "아뒤 : " + dwId );
} //CGame::OnPacketRemoveInventory

void CGame::OnPacketSerInvenItemRemove()
{
	Assert( g_pGUI );

	int	cnt, nCnt;

	m_recvPacket >> cnt;

	for( nCnt = 0; nCnt < cnt; nCnt++ )
	{
		int dwId;
		m_recvPacket >> dwId;
		g_pGUI->RemoveSerInventory( dwId );
	}
}
/**
 * 아이템 장착.
 */
void CGame::OnPacketInsertEquipItem()
{
	DWORD	dwId;			// 아이템 고유 번호
	BYTE	nPosition;		// 아이템 장착 위치

	m_recvPacket	>> dwId;

	nPosition = 0;	// 임시

	WORD	wType = g_pGUI->InsertEquipItem( dwId, nPosition );
	m_pWorld->UpdateModelCharacter( m_dwPlayer, wType, TRUE );
} //CGame::OnPacketInsertEquipItem

/**
 * 아이템 장착 해제.
 */
void CGame::OnPacketRemoveEquipItem()
{
	DWORD	dwId;			// 아이템 고유 번호
	BYTE	nPosition;		// 아이템 장착 위치

	m_recvPacket	>> dwId;

	nPosition = 0;	// 임시

	WORD	wType = g_pGUI->RemoveEquipItem( dwId, nPosition );
	m_pWorld->UpdateModelCharacter( m_dwPlayer, wType, FALSE );
} //CGame::OnPacketRemoveEquipItem

/**
 * 메인 스킬 교체.
 */
void CGame::OnPacketChangeMainSkill()
{
	DWORD	dwId;		// 캐릭터 고유 번호
	WORD	nSkillItem;	// 스킬아이템 번호 

	m_recvPacket
		>> dwId
		>> nSkillItem;

	m_nSkillItem = nSkillItem;
	
	IW3DCreature*	pCharater = m_pWorld->GetCharacter( dwId );
	if( pCharater )
	{
		pCharater->SetAttackMode( nSkillItem );

		if( m_dwPlayer == dwId )
		{
			g_pGUI->SetMainSkill( nSkillItem );
		} //if
	}
	else
	{
		Assert( pCharater );
		DEBUG_OUT( "*" + dwId + "캐릭터가 없습니다." );
	}
} //CGame::OnPacketChangeMainSkill

/**
 * 아이템 손에서 이동.
 */
void CGame::OnPacketItemFromHand()
{
	Assert( !"삭제처리" );
} //CGame::OnPacketItemFromHand

/**
 * 이미접속 중인 아이디.
 */
void CGame::OnPacketAlreadyConnected()
{
	//g_pGUI->MessageBox( STR_SYSERR_ALREADYCONNECTED, "AlreadyConnected", IW3DUInterface::MSG_OK );
	g_pGUI->MessageBox( STR_SYSERR_ALREADYCONNECTED, "AlreadyConnected", IW3DUInterface::MSG_OK );
	
} //CGame::OnPacketAlreadyConnected

#ifdef _dGUILD_WAR_CHINA_	//중국 길드 대항전관련
/**
 * 허용된 접속시간아님.	
 */
void CGame::OnPacketNotConnected()
{
	g_pGUI->MessageBox( STR_SYSERR_NOTCONNECTED, "AlreadyConnected", IW3DUInterface::MSG_OK ); //이세호
}//CGame::OnPacketNotConnected()


/**
 * 허용된 접속유저 아님.
 */
void CGame::OnPacketErrorUser()
{
	g_pGUI->MessageBox( STR_SYSERR_ERRUSER, "ErrorUser", IW3DUInterface::MSG_OK );
}
#endif

/**
 * 플레이어 아이템 정보 패킷을 저리한다.
 */
void CGame::OnPacketItemPlayerItemInfo()
{
	Assert( g_pGUI );

	int		nCount;

	WORD	wNumInvenItem;		// 아이템 갯수

	m_recvPacket	>> wNumInvenItem;

	for( nCount = 0 ; nCount < wNumInvenItem ; nCount++ )
	{
		int		nInvenId;		// 아이템 ID
		WORD	wInvenType;		// 아이템 종류
		BYTE	btInvenPage;	// 인벤토리 Page
		BYTE	btInvenCol;		// 인벤토리 Column
		BYTE	btInvenRow;		// 인벤토리 Row
		WORD	btContainCnt;	// 수량 ( 포션만 ) from sk8snow

		m_recvPacket
			>> nInvenId
			>> wInvenType
			>> btInvenPage
			>> btInvenCol
			>> btInvenRow
			>> btContainCnt;
		
		g_pGUI->InsertInventory(
			nInvenId,
			wInvenType,
			btInvenPage,
			btInvenCol,
			btInvenRow,
			btContainCnt,
			FALSE );
	} //for

	//g_pGUI->InitInventory();

	BYTE	wNumEquipItem;		// 장착 아이템 갯수

	m_recvPacket	>> wNumEquipItem;

	for( nCount = 0 ; nCount < wNumEquipItem ; nCount++ )
	{
		int		nEquipId;		// 장착 아이템 ID
		WORD	wEquipType;		// 장착 아이템 종류

		m_recvPacket
			>> nEquipId
			>> wEquipType;

		g_pGUI->InsertEquipItem( nEquipId, wEquipType );
		m_pWorld->UpdateModelCharacter( m_dwPlayer, wEquipType, TRUE );
	} //for
} //CGame::OnPacketItemPlayerItemInfo

/**
 * 캐릭터 장착 아이템을 바꾼다.
 */
void CGame::OnPacketChangeEquip()
{
	DWORD		dwId;		// 아이템 번호
	WORD		wType;		// 아이템 종류
	BYTE		bOn;		// 아이템 장착 여부

	m_recvPacket
		>> dwId
		>> wType
		>> bOn;

	m_pWorld->UpdateModelCharacter( dwId, wType, bOn );
} //CGame::OnPacketChangeEquip

/**
 * 마법창에서 마법을 제거한다.
 */
void CGame::OnPacketRemoveSpellBook()
{
	Assert( !"삭제처리" );
} //CGame::OnPacketRemoveSpellBook

/**
 * 게임 시간 변경 패킷을 처리한다.
 */
void CGame::OnPacketChangeTime()
{
	BYTE	btTime;		// 게임 시간 (0~23)
	BYTE	btNextTime;	// 다음 시간
	DWORD	dwNext;		// 다음 시간까지의 기간( 기본 3분 )
  
	m_recvPacket 
		>> btTime		// 현재 시간 
		>> btNextTime	// 다음 시간 
		>> dwNext;		// 다음시간까지의 시간

	DEBUG_OUT( "*시간 변경 - " + btTime );

	// Indoor일때는 시간의 영향을 받지 아니한다.
	if( !m_bIndoor && m_btMapType == 0 )
	{		
		m_pWorld->SetTime( btTime, btNextTime, dwNext );
	} //if
} //CGame::OnPacketChangeTime

/**
 * 돈 업데이트 패킷을 처리한다.
 */
void CGame::OnPacketUpdateMoney()
{
	Assert( g_pGUI );

	int		nMoney;		// 소지 금액

	m_recvPacket	>> nMoney;

	DEBUG_OUT( "*돈 업데이트 - " + nMoney );

	g_pGUI->UpdateMoney( nMoney );
} //CGame::OnPacketUpdateMoney


void CGame::OnPacketUpdateCash()
{
	Assert( g_pGUI );

	int		nCash;

	m_recvPacket >> nCash;

	DEBUG_OUT( "*캐쉬 업데이트 - " + nCash );

	g_pGUI->UpdateCash( nCash );
}

/**
 * 샵 아이템 리스트 패킷을 처리한다.
 */
void CGame::OnPacketShopItemList()
{
	Assert( g_pGUI );

	BYTE	btNumItem;			// 아이템 개수

	m_recvPacket	>> btNumItem;

	for( int nCnt = 0 ; nCnt < btNumItem ; nCnt++ )
	{
		int		nItemType;		// 아이템 타입
		BYTE	btPage;			// 상점창 페이지
		BYTE	btX;			// 상점창 위치 X
		BYTE	btY;			// 상점창 위치 Y

		m_recvPacket
			>> nItemType
			>> btPage
			>> btX
			>> btY;

		g_pGUI->InsertItemInShop( nItemType, btPage, btX, btY );
	} //for

	g_pGUI->ShowShop( TRUE );
} //CGame::OnPacketShopItemList

/**
 * 거래 결과 패킷을 처리한다.
 */
void CGame::OnPacketShopResult()
{
	BYTE	btResult;	// 거래 결과

	m_recvPacket	>> btResult;

	// @todo
} //CGame::OnPacketShopResult

/**
 * 거래 요청 패킷을 처리한다.
 */
void CGame::OnPacketDealBegin()
{
	int		nRequestId;		// 요청한 캐릭터 고유 아이디
	int		nTargetId;		// 대상 캐릭터 고유 아이디

	m_recvPacket
		>> nRequestId
		>> nTargetId;

	IW3DCreature*	pRequestChar = m_pWorld->GetCharacter( nRequestId );
	IW3DCreature*	pTargetChar  = m_pWorld->GetCharacter( nTargetId );
	if( !pRequestChar || !pTargetChar )
	{
		Assert( !"없는 캐릭터" );
		return;
	} //if

	if( nRequestId == m_dwPlayer )
	{	// 플레이어가 거래 요청한 경우
		g_pGUI->SetText( "TXT_DEAL_NAME1", pRequestChar->GetName() );
		g_pGUI->SetText( "TXT_DEAL_NAME2", pTargetChar->GetName() );
	}
	else if( nTargetId == m_dwPlayer )
	{	// 플레이어가 거래 대상인 경우
		if( g_pGUI->IsShowDlg( "DLG_DEAL" ) ||
			g_pGUI->IsShowDlg( "DLG_SHOPINVENTORY" ) )
		{ //이미 다른 거래 중인 경우
			CPacket		sendPacket;
			sendPacket.Tag( dPACKET_DEAL_CANCEL );
			g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

			sendPacket.Tag( dPACKET_CHAT_WHISPER )
				<< pRequestChar->GetName()
				<< STR_UIMSG_ALREADYDEAL;
			g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

			return;
		} //if

		g_pGUI->SetText( "TXT_DEAL_NAME2", pRequestChar->GetName() );
		g_pGUI->SetText( "TXT_DEAL_NAME1", pTargetChar->GetName() );

		string	strMsg = string( pRequestChar->GetName() ) + STR_DEAL_REQUESTFROM;

		g_pGUI->MessageBox(
			strMsg.c_str(),
			"DEALREQUEST",
			IW3DUInterface::MSG_YESNO );
	}
	else
	{	// 잘못된 거래 패킷이 온 경우
		Assert( !"성립하지 않는 거래 요청이다" );
		return;
	} //if..else if..else..
} //CGame::OnPacketDealRequest

/**
 * 유저 거래창에 아이템 추가 패킷을 처리한다.
 */
void CGame::OnPacketDealInsertItem()
{
	int		nCharacterId;	// 소유 캐릭터 고유 아이디
	int		nItemId;		// 아이템 고유 아이디
	WORD	nItemType;		// 아이템 타입
	BYTE	wDestCol;		// 아이텝 삽입 위치 X
	BYTE	wDestRow;		// 아이텝 삽입 위치 Y

	m_recvPacket
		>> nCharacterId
		>> nItemId
		>> nItemType
		>> wDestCol
		>> wDestRow;

	BOOL	bMe = IsPlayer( nCharacterId );

	g_pGUI->InsertItemInUserDeal(
		nItemId,
		nItemType,
		wDestCol,
		wDestRow,
		bMe );
} //CGame::OnPacketInsertItem

/**
 * 유저 거래창에서 아이템 삭제 패킷을 처리한다.
 */
void CGame::OnPacketDealRemoveItem()
{
	int		nCharacterId;	// 소유 캐릭터 고유 아이디
	int		nItemId;		// 아이템 고유 아이디

	m_recvPacket
		>> nCharacterId
		>> nItemId;

	BOOL	bMe = IsPlayer( nCharacterId );

	g_pGUI->RemoveItemInUserDeal( nItemId, bMe );
} //CGame::OnPacketRemoveItem

/**
 * 유저거래창의 금액 정보 업데이트 패킷을 처리한다.
 */
void CGame::OnPacketDealUpdateMoney()
{
	int		nCharacterId;	// 소유 캐릭터 고유 아이디
	int		nMoney;			// 돈

	m_recvPacket
		>> nCharacterId 
		>> nMoney;

	BOOL	bMe = IsPlayer( nCharacterId );

	g_pGUI->UpdateMoneyInUserDeal( nMoney, bMe );
} //CGame::OnPacketUpdateMoney

/**
 * 유저 거래 완료 확인 패킷을 처리한다.
 */
void CGame::OnPacketDealRequestSubmit()
{
	g_pGUI->MessageBox(
		STR_DEAL_SUBMIT,
		"USERDEAL_REQUESTSUBMIT",
		IW3DUInterface::MSG_YESNO );
} //CGame::OnPacketRequestSubmit

/**
 * 유저간 거래 종료 패킷을 처리한다.
 */
void CGame::OnPacketDealFinish()
{
	if( !g_pGUI->IsShowDlg( "DLG_DEAL" ) )
	{
		return;
	} //if

	g_pGUI->ShowUserDealBox( FALSE );

	g_pGUI->MessageBox( 
		STR_DEAL_FINISH,
		"USERDEAL_FINISH",
		IW3DUInterface::MSG_OK );
} //CGame::OnPacketFinish

/**
 * 파티 생성 성공 패킷을 처리한다.
 */
void CGame::OnPacketPartyCreateOk()
{
	g_pGUI->MessageBox(
		STR_PARTY_CREATE,
		"PARTYCREATEOK",
		IW3DUInterface::MSG_OK );

	if( g_pHelper)
	{
		// 파티 생성 성공 메시지를 알려준다.
		g_pHelper->Party( 0 );
	} //if
} //CGame::OnPacketPartyCreate

/**
 * 파티 종료 성공 패킷을 처리한다.
 */
void CGame::OnPacketPartyDestroyed()
{
	g_pGUI->DeletePartyAll();
	DEBUG_OUT( "*파티해제." );

	g_pGUI->MessageBox(
		STR_PARTY_DESTROY,
		"PARTYCREATEOK",
		IW3DUInterface::MSG_OK );
} //CGame::OnPacketPartyCreate

/**
 * 파티 참여 결과 패킷을 처리한다.
 */
void CGame::OnPacketPartyJoinResult()
{
	BYTE		btResult;

	m_recvPacket	>> btResult;

	switch( btResult )
	{
	case 0:		// 상대가 파티장이 아님
		g_pGUI->MessageBox(
			STR_PARTY_NOCHIEF,
			"PARTYRESULT",
			IW3DUInterface::MSG_OK );
		
		break;

	case 1:		// 파티 정원 초과
		g_pGUI->MessageBox(
			STR_PARTY_MEMBEROVER,
			"PARTYRESULT",
			IW3DUInterface::MSG_OK );

		if( g_pHelper )
		{ // 파티에 참여 하지 못한것을 비꼰다.
			g_pHelper->Party( 2 );
		} //if
		break;

	case 2:		// 파티 참여 성공
		g_pGUI->MessageBox(
			STR_PARTY_SUCCESS,
			"PARTYRESULT",
			IW3DUInterface::MSG_OK );

		if( g_pHelper )
		{
			// 파티에 참여 했다는 메시지를 날린다.
			g_pHelper->Party( 1 );
		} //if
		break;
	} //swtich
} //CGame::OnPacketPartyJoinResult

/**
 * 파티원 추가 패킷을 처리한다.
 */
void CGame::OnPacketPartyAddMember()
{
	int			nId;		// 캐릭터 ID
	string		sName;		// 캐릭터 이름
	int			nHp;		// HP
	int			nMaxHp;		// Max HP
	int			nMp;		// MP
	int			nMaxMp;		// Max MP
	
	m_recvPacket
		>> nId
		>> sName
		>> nHp
		>> nMaxHp
		>> nMp
		>> nMaxMp;

	g_pGUI->AddPartyMember( nId, sName.c_str(), nHp, nMaxHp, nMp, nMaxMp );

	IW3DCreature*	pCharacter = m_pWorld->GetCharacter( nId );

	if( pCharacter )
	{
		pCharacter->VisibleName( TRUE );

		char		szText[64];
		sprintf( szText, "%s%s", sName.c_str(), STR_PARTY_ADDMEMBER );
		g_pGUI->MessageBox( szText );
	}
	else
	{
		DEBUG_OUT( "*파티원 추가 패킷 - 파티원이 월드에 존재하지 않음." );
	} //if..else..
} //CGame::OnPacketPartyAddMember

/**
 * 파티원 삭제 패킷을 처리한다.
 */
void CGame::OnPacketPartyDeleteMember()
{
	int		nId;

	m_recvPacket	>> nId;

	if( nId == m_dwPlayer )
	{
		g_pGUI->DeletePartyAll();

		if( g_pHelper )
		{  // 파티를 해제 한것을 말한다.
			g_pHelper->Party( 3 );
		} //if
		DEBUG_OUT( "*파티 해제." );
	}
	else
	{
		g_pGUI->DeletePartyMember( nId );
		DEBUG_OUT( "*파티 멤버 탈퇴." );
	} //if..else

	IW3DCreature*	pCharacter = m_pWorld->GetCharacter( nId );
	if( pCharacter )
	{
		pCharacter->VisibleName( FALSE );
	}
	else
	{
		DEBUG_OUT( "*파티원 삭제 패킷 - 파티원이 월드에 존재하지 않음." );
	} //if..else..
} //CGame::OnPacketPartyDeleteMember

/**
 * 파티원의 HP & MP 정보 패킷을 처리한다.
 */
void CGame::OnPacketPartyMemberHPMP()
{
	int			nId;
	int			nHp;
	int			nMaxHp;
	int			nMp;
	int			nMaxMp;

	m_recvPacket
		>> nId
		>> nHp
		>> nMaxHp
		>> nMp
		>> nMaxMp;

	g_pGUI->RefreshPartyMember( nId, nHp, nMaxHp, nMp, nMaxMp );
} //CGame::OnPacketPartyMemberHPMP

/**
 * 파티 맵상 위치 정보 패킷을 처리한다.
 */
void CGame::OnPacketPartyMemberMapInfo()
{
	int			nId;
	WORD		wMap;
	int			nX;
	int			nY;

	m_recvPacket
		>> nId				//< 파티 아이디
		>> wMap				//< 맵 번호
		>> nX				//< X
		>> nY;				//< Y
	g_pGUI->RefreshPartyMember( nId, wMap, nX, nY );
} //CGame::OnPacketPartyMemberMapInfo

/**
 * STR/INT/DEX 정보 패킷을 처리한다.
 */
void CGame::OnPacketUpdatePoint()
{
	BYTE	btType;
	WORD	wPoint;

	m_recvPacket	
		>> btType
		>> wPoint;

	Assert( 0 <= btType && btType < 3 );
	g_pGUI->SetPlayerPower( btType, wPoint );
	switch( btType )
	{
			case 0 :	g_infoPlayer.wStr = wPoint;			break;
			case 1 :	g_infoPlayer.wInt = wPoint;			break;
			case 2 :	g_infoPlayer.wDex = wPoint;			break;
	}
} //CGame::OnPacketUpdatePoint

/**
 * 통합 레벨 정보 패킷을 처리한다.
 */
void CGame::OnPacketUpdateLevel()
{
	int		nLevel;
	int		nLevelExp;
	int		nNeedLevelExp;

	m_recvPacket	
		>> nLevel
		>> nLevelExp
		>> nNeedLevelExp;

	g_pGUI->SetProgressPos( "PRO_CINFO_TOTAL", nNeedLevelExp, nLevelExp );
	g_pGUI->SetText( "TXT_CINFO_TOTAL", nLevel );
	g_pGUI->SetText( "TXT_TOT_TOTLEVEL", nLevel );
} //CGame::OnPacketUpdateLevel

/**
 * 공격 가능 거리 패킷을 처리한다.
 */
void CGame::OnPacketAttackRange()
{
	BYTE	btRange;

	m_recvPacket	>> btRange;

	m_fAttackRange = btRange;
} //CGame::OnPacketAttackRange

/**
 * 영웅추가.
 */
void CGame::OnPacketAddHero()
{
	int		nCharId;
	BYTE	btType;

	m_recvPacket	
		>> nCharId
		>> btType;

	m_pWorld->AddMercenary( btType, nCharId );
} //CGame::OnPacketAddHero

/**
 * 영웅제거.
 */
void CGame::OnPacketRemoveHero()
{
	int		nCharId;

	m_recvPacket	
		>> nCharId;

	IW3DCreature*	pCharacter = m_pWorld->GetCharacter( nCharId );
	Assert( pCharacter );

	if( pCharacter )
	{
		pCharacter->RemoveHelper();
	} //if
} //CGame::OnPacketRemoveHero

/**
 * 플레이어의 장착 장비 정보를 다시 받는다.
 */
void CGame::OnPacketEquipInfo()
{
	Assert( g_pGUI );
	
	int		nCount;
	BYTE	wNumEquipItem;		// 장착 아이템 갯수
	
	m_recvPacket	>> wNumEquipItem;
	
	for( nCount = 0 ; nCount < wNumEquipItem ; nCount++ )
	{
		int		nEquipId;		// 장착 아이템 ID
		WORD	wEquipType;		// 장착 아이템 종류
		
		m_recvPacket
			>> nEquipId
			>> wEquipType;
		
		g_pGUI->InsertEquipItem( nEquipId, wEquipType );
		m_pWorld->UpdateModelCharacter( m_dwPlayer, wEquipType, TRUE );
	} //for
} //CGame::OnPacketEquipInfo

/**
 * 플레이어 사용 마법정보 설정.
 */
void CGame::OnPacketMagicInfo()
{
	BYTE	btMagicType = 0;
	BYTE	btPickType;

	m_recvPacket 
		>> btPickType;				// 피킹 형태 ( 0 - 상대, 1 - 자신, 2 - 모두 )

	Assert( m_pPlayer );

	if( m_pPlayer )
	{
		m_pPlayer->SetMagicInfo( btMagicType, btPickType );
		DEBUG_OUT( "*MagicInfo - " + btMagicType + "," + btPickType );
	} //if
} //CGame::OnPacketMagicInfo

/**
 * 마법을 발현한다.( 시전후 )
 */
void CGame::OnPacketMagicCastUser()
{
	WORD	wMagicType;			// 마법 종류 (스크롤번호)
	int		nOwnerId;			// 마법 사용자 고유번호
	int		nTargetId;			// 대상 고유번호
	BYTE	btTargetKind;		// 대상이 뭔지 ( 0 = 플레이어 , 1 = 몬스터 )
	WORD	wMagicIndex;		// 마법 인덱스 
	
	m_recvPacket
		>> wMagicType
		>> nOwnerId
		>> nTargetId
		>> btTargetKind
		>> wMagicIndex;
	
	IW3DCreature*		pTarget;
	IW3DCreature*		pOwner;
	
	if( btTargetKind == 0 )				pTarget = m_pWorld->GetCharacter( nTargetId );
	else if( btTargetKind == 1 )		pTarget = m_pWorld->GetMonster( nTargetId );
	else								Assert( !"없는 대상" );

	if( !pTarget )			return ;

	if( btTargetKind == 1 )
	{
		pOwner = m_pWorld->GetCharacter( nOwnerId );
	}
	else	
	{
		if( nOwnerId == nTargetId )		
		{	
			pOwner = pTarget;								
		}
		else
		{
			pOwner = m_pWorld->GetCharacter( nOwnerId );
		} //if..else..
	} //if..else..

	Assert( pOwner );

	if( !pOwner )
	{
		return ;
	} //if

	Vector3		vDir = pTarget->GetPosition() - pOwner->GetPosition();

	if( pTarget->GetPosition() == pOwner->GetPosition() )
	{
		vDir = Vector3( 0.0F, 1.0F, 0.0F );
	} //if

	pOwner->SetDirection( vDir );
	
	pOwner->CastMagic();
	pOwner->AddMagic( wMagicType, wMagicIndex, pTarget, TRUE );
} //CGame::OnPacketMagicCastUser


/**
 * 마법 이펙트 추가
 */
void CGame::OnPacketAddEffect()
{
	WORD		wSkillNum;			// 스킬 아이템 번호
	int			nTime;				// 유지시간 ( ms )

	m_recvPacket 
		>> wSkillNum
		>> nTime;

	//여기다가 캐릭터가 준것임을 인지 시킨다.
	g_pGUI->SetSkillInfo( TRUE );
	g_pGUI->AddSkillImage( wSkillNum, nTime );
} // CGame::OnPacketAddEffect

/**
 * 마법 이펙트 삭제 
 */
void CGame::OnPacketRemoveEffect()
{
	WORD	wSkillNum;			// 스킬 아이템 번호 
	
	m_recvPacket
		>> wSkillNum;
	//여기서 캐릭터가 준것을 해제시킨다.
	g_pGUI->SetSkillInfo( FALSE );
	
	g_pGUI->RemoveSkillImage( wSkillNum );
} // CGame::OnPacketRemoveEffect

/**
 * 최대 체력, 마법력 업데이트
 */
void CGame::OnPacketUpdateMaxHPMP()
{
	int nMaxHP, nMaxMP;

	m_recvPacket
		>> nMaxHP
		>> nMaxMP;
	
	g_infoPlayer.nHp[1] = nMaxHP;
	g_infoPlayer.nMp[1] = nMaxMP; 

	DEBUG_OUT( "* 최대 HP : " + nMaxHP + " 최대 MP : " + nMaxMP );

	if( g_infoPlayer.nHp[0] > nMaxHP )
	{
		g_infoPlayer.nHp[0] = nMaxHP;
	} //if

	if( g_infoPlayer.nMp[0] > nMaxMP )
	{
		g_infoPlayer.nMp[0] =  nMaxMP;
	} //if
	
	if( !m_pPlayer )
	{
		Assert( !"내가 없다" );
		return;
	} //if

	m_pPlayer->SetHp( g_infoPlayer.nHp[0], g_infoPlayer.nHp[1] );
	g_pGUI->SetPosMP( g_infoPlayer.nMp[1], g_infoPlayer.nMp[0] );
	g_pGUI->SetPosHP( g_infoPlayer.nHp[1], g_infoPlayer.nHp[0] );
} // CGame::OnPacketUpdateMaxHPMP

/**
 * 기술 경험치 값 변환
 */
void CGame::OnPacketSetSkillExp()
{
	Assert( m_pWorld );
	Assert( g_pGUI );
	
	BYTE	bSkill;		// 경험치를 얻은 스킬 종류
	int		nExp;		// 얻음 경험치
	
	m_recvPacket
		>> bSkill
		>> nExp;
	
	g_infoPlayer.nExp[bSkill][0] = nExp;
	
	g_pGUI->SetPosSkill( bSkill, g_infoPlayer.nExp[bSkill][1], g_infoPlayer.nExp[bSkill][0] );
}//CGame::OnPacketSetSkillExp

/**
 * 기술 추가
 */
void CGame::OnPacketAddSkill()
{
	WORD wSkillItemNum;

	m_recvPacket
		>> wSkillItemNum;		// 추가되는 스킬의 아이템 번호
	
	g_pGUI->AddSkill( wSkillItemNum );
}//CGame::OnPacketAddSkill

/**
 * 스킬 리스트
 */
void CGame::OnPacketSkillList()
{
	BYTE btCnt;
	WORD wSkillItemNum;

	m_recvPacket
		>> btCnt;	

	for( int i = 0; i < btCnt; i++ )
	{
		m_recvPacket
			>> wSkillItemNum;

		g_pGUI->AddSkill( wSkillItemNum );
	} //for
	
	if( !g_pGUI->LoadQuickData() )
	{ // 실패하면 다시 한번 읽는다.
		g_pGUI->LoadQuickData();
	} //if
}//CGame::OnPacketSkillList

/**
 * 포션 수량 업데이트 
 */
void CGame::OnPacketUpdatePotionCnt()
{
	int nID;		// 아이템 고유번호 
	WORD btCnt;		// 수량 from sk8snow

	m_recvPacket 
		>> nID
		>> btCnt;

	g_pGUI->UpdateItemData( nID, btCnt );
}//CGame::OnPacketUpdatePotionCnt

/**
 * 인첸트 카드 수량 업데이트 
 */
void CGame::OnPacketUpdateCardCnt()
{
	int nID;		// 아이템 고유번호 
	WORD btCnt;		// 수량 from sk8snow

	m_recvPacket 
		>> nID
		>> btCnt;

	g_pGUI->UpdateItemData( nID, btCnt );
}//CGame::OnPacketUpdateCardCnt()

/**
 * 퀘스트 정보 받기
 */
void CGame::OnPacketQuestList()
{
#ifdef _dUMENOKOJI_QUEST_
	
	int	 wCnt;			
	WORD wQuestNum;
	BYTE btQuestState;		// 퀘스트 상태 
	int	MapNum;
	int NpcNum;
	
#else
	WORD wCnt;				// 퀘스트정보 개수 
	WORD wQuestNum;			// 퀘스트 번호 
	BYTE btQuestState;		// 퀘스트 상태 
	
							/*
							퀘스트 상태 값은 .. 
							0 이면 진행한적 없음 ( 하지만 ;; 0인건 서버에서 보내지 않음 ;; )
							1 이면 진행중 
							2 이면 이미 완료된 퀘스트임
	*/
#endif	
	m_recvPacket 
		>> wCnt;

	for( int i = 0; i < wCnt; i++ )
	{
		m_recvPacket 
			>> wQuestNum
#ifdef _dUMENOKOJI_QUEST_
			>> MapNum
			>> NpcNum
#endif
			>> btQuestState;
		g_mapQuestState.insert( std::make_pair( wQuestNum, btQuestState ) );
		
		g_pGUI->AddQuestInList( wQuestNum, btQuestState );
	} //for

}//CGame::OnPacketQuestList

/**
 * 퀘스트 질의 응답
 */
void CGame::OnPacketQuestResult()
{
	WORD wQuestNum;				// 퀘스트 번호 
	BYTE btQuestState;			// 퀘스트 상태

	/*
	퀘스트 상태 값은 .. 
	1 이면 진행중 
	2 이면 이미 완료된 퀘스트임
	*/

	m_recvPacket
		>> wQuestNum
		>> btQuestState;
}//CGame::OnPacketQuestResult

/**
 * 창고안의 아이템 리스트
 */
void CGame::OnPacketGarbageItemList()
{
	WORD wCnt;
	WORD wItemType;
	int nIdxNum;
	WORD btContainCnt;
	int nContainMoney;

	m_recvPacket 
		>> wCnt;				///< 총 개수

	g_pGUI->RemoveAllItemInStorage();
	for( int i = 0; i < wCnt; i++ )
	{
		m_recvPacket
			>> wItemType		///< 아이템 종류 
			>> nIdxNum			///< 고유번호 
			>> btContainCnt;	///< 수량(포션)

		g_pGUI->InsertItemInStorage( nIdxNum, wItemType, btContainCnt );
	} //for

	m_recvPacket
		>> nContainMoney; // 창고에 맞긴 돈 
	g_pGUI->SetText( "TXT_STORAGE_GOLD", nContainMoney );

	g_pGUI->ShowStorage( TRUE );

}//CGame::OnPacketGarbageItemList

void CGame::OnPacketSerGarbageItemList()
{
	WORD wCnt;
	WORD wItemType;
	int nIdxNum;
	BYTE btContainCnt;
	int nContainMoney;

	m_recvPacket 
		>> wCnt;				///< 총 개수
	
	g_pGUI->RemoveAllSerItemInStorage();
	for( int i = 0; i < wCnt; i++ )
	{
		m_recvPacket
			>> wItemType		///< 아이템 종류 
			>> nIdxNum			///< 고유번호 
			>> btContainCnt;	///< 수량(포션)
		
		g_pGUI->InsertItemInSerGarbage( nIdxNum, wItemType, btContainCnt );
	} //for

	m_recvPacket
		>> nContainMoney;

	g_pGUI->SetText( "TXT_SERVER_STORAGE_GOLD", nContainMoney );
}

void CGame::OnPacketSerInvenItemList()
{
	Assert( g_pGUI );
	
	int		nCount;
	
	WORD	wNumInvenItem;		// 아이템 갯수
	
	m_recvPacket	>> wNumInvenItem;
	
	for( nCount = 0 ; nCount < wNumInvenItem ; nCount++ )
	{
		int		nInvenId;		// 아이템 ID
		WORD	wInvenType;		// 아이템 종류
		BYTE	btInvenPage;	// 인벤토리 Page
		BYTE	btInvenCol;		// 인벤토리 Column
		BYTE	btInvenRow;		// 인벤토리 Row
		BYTE	btContainCnt;	// 수량 ( 포션만 ) from sk8snow
		
		m_recvPacket
			>> nInvenId
			>> wInvenType
			>> btInvenPage
			>> btInvenCol
			>> btInvenRow
			>> btContainCnt;

		g_pGUI->InsertSerIntInventory(			
			nInvenId,
			wInvenType,
			btInvenPage,
			btInvenCol,
			btInvenRow,
			btContainCnt,
			FALSE );
	} //for
}

/**
 * 창고에 아이템 추가
 */
void CGame::OnPacketInsertToGarbage()
{
	WORD wItemType;
	int nIdxNum;
	WORD btContainCnt;

	m_recvPacket 
		>> wItemType
		>> nIdxNum
		>> btContainCnt;

	g_pGUI->InsertItemInStorage( nIdxNum, wItemType, btContainCnt );
}//CGame::OnPacketInsertToGarbage

/**
 * 창고에서 아이템 삭제
 */
void CGame::OnPacketRemoveFromGarbage()
{
	int nIdxNum;

	m_recvPacket 
		>> nIdxNum;

	g_pGUI->RemoveItemInStorage( nIdxNum );
}//CGame::OnPacketRemoveFromGarbage

void CGame::OnPacketSerRemoveFromGarbage()
{
	int nIdxNum;

	m_recvPacket
		>> nIdxNum;

	g_pGUI->RemoveSerItemInStorage( nIdxNum );
	
}

/**
 * 탈것 정보
 */
void CGame::OnPacketRideInfo()
{
	int nIdxNum;		// 케릭터 고유번호
	WORD wRideNum;		// 탈것 번호 ( 없으면 0 )
	BYTE btSpeed;		// 속력

	m_recvPacket
		>> nIdxNum
		>> wRideNum
		>> btSpeed;

	IW3DCreature*		pPlayer = m_pWorld->GetCharacter( nIdxNum );
	Assert( pPlayer );
	
	if( pPlayer )
	{
		if( wRideNum )
		{
			SW3DVehicle vehicle;
			vehicle.wType = wRideNum;		
			vehicle.nSex = pPlayer->GetSex();
			
			pPlayer->AddVehicle( &vehicle );
			g_pGUI->ShowDlg( "DLG_ENCHANT", FALSE );
		}
		else
		{
			pPlayer->RemoveVehicle();
			g_pGUI->ShowDlg( "DLG_ENCHANT", FALSE );
		} //if..else..
		
		pPlayer->ChangeSpeed( btSpeed );
		
		if( m_dwPlayer == nIdxNum )
		{
			g_infoPlayer.wRide = wRideNum;
			g_infoPlayer.btSpeed = btSpeed;
		} //if
	} //if
}//CGame::OnPacketRideInfo


/**
 * 마법 시전 정보 
 */
void CGame::OnPacketBeginCastMagic2()
{
	int nIdx;
	WORD wMagicNum;
	DWORD dwTargetId;
	BYTE  btIsMonster;

	m_recvPacket
		>> nIdx			// 케릭터 고유번호
		>> wMagicNum	// 마법 번호 
		>> dwTargetId
		>> btIsMonster;

	IW3DCreature*		pPlayer = m_pWorld->GetCharacter( nIdx );
	IW3DCreature*		pTarget = NULL;

	if( btIsMonster )
	{
		pTarget = m_pWorld->GetMonster( dwTargetId );
	}
	else
	{
		pTarget = m_pWorld->GetCharacter( dwTargetId );
	} //if..else..

	if( !pTarget )
	{
		return ;
	} //if

	if( pPlayer )
	{

		pPlayer->CastingMagic( dwTargetId, wMagicNum );
		pPlayer->AddMagic( wMagicNum, 0, pTarget );

		Vector3		vDir = pTarget->GetPosition() - pPlayer->GetPosition();

		if( pTarget->GetPosition() == pPlayer->GetPosition() )
		{
			vDir = Vector3( 0.0F, 1.0F, 0.0F );
		} //if

		pPlayer->SetDirection( vDir );
	} //if

	m_bSendAttackPacket = FALSE;

} //CGame::OnPacketBeginCastMagic2

/**
 * NPC 정보
 */
void CGame::OnPacketNpcInfo()
{
	BYTE btCnt;
	
	int npcNum;
	WORD shopNum;
	WORD questNum;
	BYTE isGarbage;
	BYTE isGuildMaster;	// 길드관리인

	m_recvPacket 
		>> btCnt;

	/*
	shopNum 이나 questNum 이 0일경우 
	상점이나 퀘스트 NPC가 아님
	isGarbage가 0이면 창고아님 1이면 창고 NPC
	*/

	for( int i = 0; i < btCnt; i++ )
	{
		m_recvPacket
			>> npcNum
			>> shopNum
			>> questNum
			>> isGarbage
			>> isGuildMaster;

		IW3DCreature*	pNPC = NULL;
		WORD			wNPCCount = 0;
		while( pNPC = m_pWorld->GetNPC( wNPCCount ) )
		{
			if( pNPC->GetId() == npcNum )
			{
				break;
			} //if
			pNPC = NULL;
			wNPCCount++;
		} //do

		Log( "NPC Link %d, %d, %d", npcNum, shopNum, isGarbage );
		if( !pNPC )
		{
			Log( "Link Failed" );
		//	Assert( !"없는 NPC" );
			continue;
		} //if
		pNPC->SetCastContent( shopNum, questNum, isGarbage, isGuildMaster );
	} //for

	PutNPCMinimap();
}//CGame::OnPacketNpcInfo


/**
 * 몬스터 상태변환
 */
void CGame::OnPacketMobState()
{
	WORD wMobIdx;
	BYTE btState;

	m_recvPacket 
		>> wMobIdx				// 몹 고유번호
		>> btState;				// 상태 ( 0 = 일반, 1 = 전투모드 )

	IW3DCreature*	pMonster = m_pWorld->GetMonster( wMobIdx );

	if( pMonster )
	{	
		pMonster->SetMode( !btState );		
	} //if
}//CGame::OnPacketMobState

/**
 * 유령 상태 값
 */
void CGame::OnPacketGhostState()
{
	BYTE btState;

	m_recvPacket
		>> btState;
	// 0 = 유령상태 해제
	// 1 = 유령상태 설정

	if( !m_pPlayer )
	{
		Assert( !"없당." );
		return;
	} //if

	if( btState )
	{
		m_pPlayer->Ghost( TRUE );
		g_pGUI->ShowTimer( "TIMER_GHOST", 30000 );
	}
	else
	{
		m_pPlayer->Ghost( FALSE );
		g_pGUI->PlayerLive( TRUE );
	} //if..else
}//CGame::OnPacketGhostState

/**
 * 파티구함게시판 리스트 
 */
void CGame::OnPacketPartyBoardList()
{
	WORD wCnt;

	char	name[128];
	int		level;
	int		skill[4];

	m_recvPacket
		>> wCnt;

	g_pGUI->ShowDlg( "DLG_PARTYBOARD", TRUE );
	for( int i = 0; i < wCnt; i++ )
	{
		m_recvPacket
			>> name				// 이름
			>> level			// 레벨 
			>> skill[0]			// 근거리 
			>> skill[1]			// 원거리 
			>> skill[2]			// 흑마법
			>> skill[3];		// 백마법

		g_pGUI->AddPartyBoardList( name, level, skill[0], skill[1], skill[2], skill[3] );
	} //for
}//CGame::OnPacketPartyBoardList

/**
 * PK 용 데미지 패킷 
 */
void CGame::OnPacketDamagePvP()
{
	int nAttacker;
	int nVict;
	WORD wDamage;
	BYTE isCritical;
	
	m_recvPacket 
		>> nVict			// 공격당하는넘
		>> nAttacker		// 공격 하는넘
		>> wDamage			// 데미지
		>> isCritical;		// 크리티컬 ? ( 나중에 빠질지도 )

	IW3DCreature*	pCharacter	= m_pWorld->GetCharacter( nAttacker );
	IW3DCreature*	pTarget		= m_pWorld->GetCharacter( nVict );

	if( !pCharacter || !pTarget )
	{
		return;
	} //if

	if( //pCharacter->GetState() == STATE_ATTACK ||
		pCharacter->GetState() == STATE_DEAD )
	{
		return;
	} //if

	Vector3		vDir = pTarget->GetPosition() - pCharacter->GetPosition();

	if( pCharacter->GetPosition() == pTarget->GetPosition() )
	{
		vDir = Vector3( 0.0F, 1.0F, 0.0F );
	} //if

	pCharacter->SetDirection( vDir );
	g_pGUI->SetPlayerMapDir( vDir.x, vDir.y );
	
	//pCharacter->Attack( pTarget );

	////////////////////////////////

	DEBUG_OUT( "Damage : " + wDamage );
	pCharacter->ShowDamageForTarget();
	pTarget->Damage( wDamage, isCritical );
//	pMonster->Damage( wDamage, btCritical );
	pCharacter->AddTarget( pTarget );
	pTarget->AddAttacker( pCharacter );
}//CGame::OnPacketDamagePvP


/**
 * 전체채팅
 */
void CGame::OnPacketBroadcastChat()		
{
	Assert( g_pGUI );

	string	sName;
	string	sText;

	m_recvPacket
		>> sName
		>> sText;

	g_pGUI->ChattingInput( 
		sName.c_str(), 
		sText.c_str(), 
		0, 
		0,
		IW3DUInterface::CHATT_BROADCAST
		);
}//CGame::OnPacketBroadcastChat

/**
 * 매매채팅
 */
void CGame::OnPacketSellingChat()			
{
	Assert( g_pGUI );

	string	sName;
	string	sText;
	int		isadmin;

	m_recvPacket
		>> sName
		>> sText
		>> isadmin;

	if( isadmin == 1 )
	{
		g_pGUI->ChattingInput( 
			sName.c_str(), 
			sText.c_str(), 
			COLOR_ARGB( 255, 255, 20, 20 ), 
			0,
			IW3DUInterface::CHATT_SELL 
			);
	}
	else
	{
	g_pGUI->ChattingInput( 
		sName.c_str(), 
		sText.c_str(), 
		COLOR_ARGB( 255, 241, 147, 255 ), 
		0,
		IW3DUInterface::CHATT_SELL 
		);
	}
}//CGame::OnPacketSellingChat

/**
 * 길드채팅 
 */
void CGame::OnPacketGuildChat()			
{
	Assert( g_pGUI );

	string	sName;
	string	sText;

	m_recvPacket
		>> sName
		>> sText;

	g_pGUI->ChattingInput( 
		sName.c_str(), 
		sText.c_str(), 
		0xFF00FFF0, 
		0,
		IW3DUInterface::CHATT_GUILD
		);
}//CGame::OnPacketGuildChat

/**
 * 파티채팅
 */
void CGame::OnPacketPartyChat()	
{
	Assert( g_pGUI );

	string	sName;
	string	sText;

	m_recvPacket
		>> sName
		>> sText;

	g_pGUI->ChattingInput( 
		sName.c_str(), 
		sText.c_str(), 
		0xFFFF6000, 
		0,
		IW3DUInterface::CHATT_PARTY
		);
}//CGame::OnPacketPartyChat

/**
 * 단 채팅
 */
void CGame::OnPacketMemberChat()
{
	Assert( g_pGUI );

	string	sName;
	string	sText;

	m_recvPacket
		>> sName
		>> sText;

	g_pGUI->ChattingInput( 
		sName.c_str(), 
		sText.c_str(), 
		COLOR_ARGB( 255, 221, 254, 107 ) , 
		0,
		IW3DUInterface::CHATT_MEMBER
		);
}//CGame::OnPacketMemberChat

/**
 * 날씨 변경
 */
void CGame::OnPacketChangeWeather()
{
	BYTE btWeather;		// 0 = 기본 , 1 = 눈 , 2 = 비

	m_recvPacket 
		>> btWeather;

	// 일단은 3가지 만 받도록 한다.
	Assert( btWeather >= 0 && btWeather < 3 );

	m_pWorld->SetWeather( btWeather );
}//CGame::OnPacketChangeWeather

/**
 * 사운드 이펙트 
 */
void CGame::OnPacketSoundEffect()
{	
	// 플레이할 사운드의 값
	WORD	wSoundIdx;

	m_recvPacket 
		>> wSoundIdx;	
	
	switch( wSoundIdx )
	{
	case 1 :	m_pWorld->AddMagic( "힐링포션1", m_pPlayer, m_pPlayer );	break;
	case 2 :	m_pWorld->AddMagic( "힐링포션2", m_pPlayer, m_pPlayer );	break;
	case 3 :	m_pWorld->AddMagic( "힐링포션3", m_pPlayer, m_pPlayer );	break;
	case 4 :	m_pWorld->AddMagic( "마나포션1", m_pPlayer, m_pPlayer );	break;
	case 5 :	m_pWorld->AddMagic( "마나포션2", m_pPlayer, m_pPlayer );	break;
	case 6 :	m_pWorld->AddMagic( "마나포션3", m_pPlayer, m_pPlayer );	break;
	}

}//CGame::OnPacketSoundEffect

/**
 * 성향 업데이트 
 */
void CGame::OnPacketUpdateCha()
{
	int nCha;

	m_recvPacket 
		>> nCha;

	g_infoPlayer.nCha = nCha;
	g_pGUI->SetText( "TXT_CINFO_CHA", g_infoPlayer.nCha );
}//CGame::OnPacketUpdateCha

/**
 * 인첸트 아이템 
 */
void CGame::OnPacketEnchantItemResult()
{
	BYTE result;

	// result 값 0 = 실패 1 = 성공

	m_recvPacket
		>> result;

	g_pGUI->EnchantResult( result );
}//CGame::OnPacketEnchantItemResult


/**
 * 파티 초대 
 */
void CGame::OnPacketPartyAskInvite()
{
	int	nIdx;	// 파티에 초대하는 파티장의 고유번호
	
	m_recvPacket
		>> nIdx;

	m_dwRequestId = nIdx;

	IW3DCreature*		pCharacter = m_pWorld->GetCharacter( nIdx );
	if( !pCharacter )
	{
		Assert( !"없는 파티장이다" );
		return;
	} //if

	char		szText[125];

	sprintf( szText, "%s%s", pCharacter->GetName(), STR_PARTY_REQUESTINVIT );

	g_pGUI->MessageBox( szText, "PARTYINVIT", IW3DUInterface::MSG_YESNO );
}//CGame::OnPacketPartyAskInvite

void CGame::OnPacketFriendDOUWANT()
{
	int nIdx;

	m_recvPacket
		>> nIdx;

	IW3DCreature*	pCharacter = m_pWorld->GetCharacter( nIdx );

	m_dwRequestId = nIdx;

	if( !pCharacter )
	{
		return;
	}

	char	szText[125];

	sprintf( szText, "%s%s", pCharacter->GetName(), STR_FRIEND_REQUESTINVIT );

	g_pGUI->MessageBox( szText, "FRIENDINVIT", IW3DUInterface::MSG_YESNO );
}

/**
 * 직업 업데이트
 */
void CGame::OnPacketUpdateClass()
{
	BYTE btClass;

	m_recvPacket 
		>> btClass;

	if( btClass > 16 )
	{
		Assert( !"잘못된 직업 번호" );
		return;
	}

	g_infoPlayer.btClass = btClass;
	g_pGUI->SetJobName( btClass );
}//CGame::OnPacketUpdateClass

/**
 * 시스템 메세지
 */
void CGame::OnPacketSystemMsg()
{
	char szMsg[1024];

	m_recvPacket
		>> szMsg;
	
	g_pGUI->ChattingInput( "", szMsg, 0xFF00FF00, 0 );
}//CGame::OnPacketSustemMsg

/*
 *	
 */
void CGame::OnPacketFriendLoginMsg()
{
	char szMsg[1024];
	
	m_recvPacket
		>> szMsg;
	
	g_pGUI->ChattingInput( "", szMsg, 0xFF0084FF, 0 );
}

/**
 * 스킬 사용 
 **/
void CGame::OnPacketBeginSkill()
{
	WORD	wSkillNum;
	int		nPlayerIdx;
	int		nTargetIdx;
	BYTE	btTargetType; // 0 = 플레이어 , 1 = 몹

	m_recvPacket
		>> wSkillNum
		>> nPlayerIdx
		>> nTargetIdx
		>> btTargetType;

	IW3DCreature*		pPlayer = m_pWorld->GetCharacter( nPlayerIdx );
	IW3DCreature*		pTarget;
	if( btTargetType == 0 )
	{ //캐릭터
		pTarget = m_pWorld->GetCharacter( nTargetIdx );
	}
	else if( btTargetType == 1 )
	{ //몬스터
		pTarget = m_pWorld->GetMonster( nTargetIdx );
	} //if..else

	if( !pPlayer || !pTarget )
	{
//		Assert( !"스킬에 대한 대상이 없다" );
		return;
	} //if

	pPlayer->AddSkill( pTarget, wSkillNum );
}//CGame::OnPacketBeginSkill()

/**
 * 케릭터 안보이게 만들기 
 */
void CGame::OnPacketSetVisible()
{
	int nIdxNum;		// 케릭터 고유번호 
	BYTE btVisible;	

	// nIdxNum 이 본인 케릭터 이면
	// 유령과 같이 반투명 상태로
	// btVisible이 1이면 해당 케릭터는
	// 화면에 찍지 않음
	// 0 이면 찍음

	m_recvPacket 
		>> nIdxNum
		>> btVisible;

	IW3DCreature*	pCreature = m_pWorld->GetCharacter( nIdxNum );

	if( !pCreature )
	{
		return ;
	} //if

	if( nIdxNum == g_dwPlayerId )		pCreature->Ghost( btVisible );
	else								pCreature->SetVisible( !btVisible );
}//CGame;;OnPacketSetVisible


/**
 * 이동 가능 불가능 
 **/
void CGame::OnPacketSetMovable()
{
	BYTE btFlag;		// 0 이면 이동 가능 1이면 이동불가

	m_recvPacket
		>> btFlag;
	
	m_bNoMove = btFlag;
}//CGame::OnPacketSetMovable

/*
 *	길드정보 변경
 */
void CGame::OnPacketUpdateGuildInfo()
{
	int	 nCharacterId;		// 캐릭터 아이디
	char guildName[64];		// 길드이름 ( 없을시 " " ) 
	WORD guildMark;			// 길드마크 ( 0 이면 없음 )
	BYTE btGuildLevel;		// 길드 등급
	int	 nGuildNum;			// 길드번호 

	m_recvPacket
		>> nCharacterId
		>> guildName
		>> guildMark
		>> btGuildLevel
		>> nGuildNum;

	IW3DCreature* pCreature = m_pWorld->GetCharacter( nCharacterId );
	if( pCreature )
	{
		pCreature->SetGuildData( guildName, nGuildNum, btGuildLevel, guildMark );
		if( pCreature->GetId() == m_dwPlayer )
		{
			g_infoPlayer.sGuildName = guildName;
			g_infoPlayer.wGuildMark = guildMark;
			
			g_pGUI->SetText( "TXT_CINFO_GUILD", guildName ); 
		} //if
	} //if
} //CGame::OnPacketUpdateGuildInfo()

/*
 *	중국 아이템 이벤트 결과
 */
void CGame::OnPacketItemEvent()
{
	BYTE btResult;

	m_recvPacket 
		>> btResult;

	// 0 꽝, 1 핸드폰, 2 디카, 3 TV, 4 액정

	g_pGUI->ShowEventMessageBox( btResult );
}//CGame::OnPacketItemEvent()

/*
 *	친구목록
 */
void CGame::OnPacketFriendList()
{
	BYTE btCnt;	// 총 친구 갯수 
	char	name[128];

	m_recvPacket 
		>> btCnt;

	g_pGUI->ClearFriendList();

	for( int i = 0; i < btCnt; i++ )
	{
		m_recvPacket >> name;

		g_pGUI->AddFriend( name );		
	}
}//CGame::OnPacketFriendList()

/*
 * 친구등록 	
 */
void CGame::OnPacketFriendAdded()
{
	char name[128];

	m_recvPacket 
		>> name;

	g_pGUI->AddFriend( name );
	// 등록할 친구이름 ;; 
}//CGame::OnPacketFriendAdded()

/*
 *	친구 삭제 
 */
void CGame::OnPacketFriendRemoved()
{
	char name[128];
	
	m_recvPacket
		>> name;

	g_pGUI->RemoveFriend( name );
}//CGame::OnPacketFriendRemoved()

/*
 *	친구 쪽지
 */
void CGame::OnPacketFriendMemo()
{
	char name[128];
	char memo[1024];

	m_recvPacket
		>> name		// 전송한 사람 이름
		>> memo;	// 내용

	g_pGUI->ChattingFriend( name, memo );
}//CGame::OnPacketFriendMemo()

/*
 *	길드 가입 요청 ( 길드마스터 -> 일반 플레이어 )
 */
void CGame::OnPacketGuildAddmemberRequest()
{
	WORD	wGuildNo;		// 길드번호 
	string	guildName;		// 길드이름
	int		nIdxNum;		// 길드마스터 케릭 고유번호

	m_recvPacket
		>> wGuildNo
		>> guildName
		>> nIdxNum;
	
	g_pGUI->RequestInviteGuild( wGuildNo, nIdxNum, guildName.c_str() );
	
	// 보내야할 데이터
	// dPACKET_GUILD_ADDMEMBER (166번) 패킷
	// int nIdxNum
	// WORD wGuildNo
    // BYTE allowJoin ( YES = 1, NO = 0 )
}//CGame::OnPacketGuildAddmemberRequest()

/*
 *	팻 정보 ( 본인에게만 전송 )
 */
void CGame::OnPacketPetInfo()
{
	BYTE btType;		// 타입
	string		strPetName;	// 펫 이름 
	WORD		wHP;
	WORD		wMaxHP;
	WORD		wMP;
	WORD		wMaxMP;
	WORD		wMinAP;
	WORD		wMaxAP;
	WORD		wDP;

	m_recvPacket
		>> btType
		>> g_infoPet.nLevel			// 팻 레벨 
		>> g_infoPet.nExp			// 현재 경험치
		>> g_infoPet.nNeedExp		// 목표 경험치
		>> g_infoPet.wAngerGage		// 분노 게이지
		>> strPetName
		>> wHP
		>> wMaxHP
		>> wMP
		>> wMaxMP
		>> wMinAP
		>> wMaxAP
		>> wDP;


	g_pGUI->UpdatePet( 
		strPetName.c_str(), 
		btType, 
		g_infoPet.nLevel, 
		g_infoPet.nExp, 
		g_infoPet.nNeedExp, 
		g_infoPet.wAngerGage, 
		wHP, 
		wMaxHP, 
		wMP, 
		wMaxMP,
		wMinAP,
		wMaxAP,
		wDP
		);
}//CGame::OnPacketPetInfo()

/*
 *	팻 ON
 */
void CGame::OnPacketPetON()
{
	int nIdx;				// 케릭터 고유 번호 
	BYTE btPetType;			// 팻 종류 번호 
	string strPetName;		// 팻 이름

	m_recvPacket
		>> nIdx
		>> btPetType
		>> strPetName;

	IW3DCreature* pCreature = m_pWorld->GetCharacter( nIdx );

	if( !pCreature )
		return ;

	pCreature->InsertPet( btPetType, NULL );
	pCreature->UpdatePet( strPetName.c_str(), NULL );

	if( pCreature->GetId() == m_dwPlayer )
	{
		g_pGUI->SetText( "BTN_PET_USE", STR_UIMSG_PETOFF );
	} //if
} //CGame::OnPacketPetON()

/*
 *	팻 OFF
 */
void CGame::OnPacketPetOFF()
{
	int nIdx;				// 케릭터 고유 번호

	m_recvPacket
		>> nIdx;

	IW3DCreature* pCreature = m_pWorld->GetCharacter( nIdx );
	if( !pCreature )
		return ;

	pCreature->RemovePet();
	if( pCreature->GetId() == m_dwPlayer )
	{
		g_pGUI->SetText( "BTN_PET_USE", STR_UIMSG_PETON );
	} //if
}//CGame::OnPacketPetOFF()

/*
 *	팻 분노게이지 업데이트 
 */
void CGame::OnPacketUpdatePetGage()
{
	int nIdx;			// 캐릭터 고유 번호
	WORD wAngerGage;	// 펫 분노 게이지

	m_recvPacket
		>> nIdx			// 고유 번호
		>> wAngerGage;	// 분노 게이지

	if( !IsPlayer( nIdx ) )
	{ // No Player
		return;
	} //if

//	char	szText[16];
//	sprintf( szText, "%d/1000", wAngerGage );
	g_pGUI->SetProgressPos( "PRO_PET_POW", 1000, wAngerGage );
//	g_pGUI->SetText( "TXT_CHAR_PET", szText );
} //CGame::OnPacketUpdatePetGage

/*
 *	팻 정보 업데이트
 */
void CGame::OnPacketUpdatePetInfo()
{
	
} //CGame::OnPacketUpdatePetInfo

/*
 *	팻 -> 몹 공격 
 */
void CGame::OnPacketPetAttack()
{
	/*
		TODO : 임시 작업
				서버쪽에서 패킷 정리 해야 함				
	*/
	int nIdx;				// 케릭터 고유 번호
	int nTargetIdx;		// 몬스터 고유 번호
	BYTE btTargetKind;		// 공격 대상 ( 0 = 플레이어, 1 = 몹 )

	m_recvPacket
		>> nIdx
		>> nTargetIdx
		>> btTargetKind;

	// 이제 공격대상을 찾는다.
	IW3DCreature* pTarget = NULL;

	if( btTargetKind == 0 )
	{
		pTarget = m_pWorld->GetCharacter( nTargetIdx );
	}
	else
	{
		pTarget = m_pWorld->GetMonster( nTargetIdx );
	} //if..else..

	if( pTarget && g_pHelper )
	{	// 공격 대상이 있다면 펫을 공격상태로 설정한다.
		g_pHelper->Attack( pTarget );
	} //if

} //CGame::OnPacketPetAttack

/*
 *	몹 -> 팻 공격
 */
void CGame::OnPacketAttackToPet()
{
	//-- 일단 보류 --//
} //CGame::OnPacketAttackToPet

/**
 * 낚시 상태
 */
void CGame::OnPacketFisingStatus()
{
/*	int		nId;			// 유저 Idx
	BYTE	btStatus;		// 시작, 끝

	m_recvPacket
		>> nId
		>> btStatus;
	
	switch( btStatus )
	{
	case FISHING_STATUS_START :			m_mgFishHook.OnGameStart( nId );		break;
	case FISHING_STATUS_END :			m_mgFishHook.OnGameEnd( nId );			break;	
	case FISHING_STATUS_HOOKINGSTART :	m_mgFishHook.OnSetHookTiming( TRUE );	break;
	case FISHING_STATUS_HOOKINGEND :	m_mgFishHook.OnSetHookTiming( FALSE );	break;
	}*/
} // CGame::OnFisingStatus

/**
 * 낚시 패턴번호
 */

void CGame::OnPacketFishingPattern()
{
/*	int		nId;			// 유저 Idx
	BYTE	btPattern;		// 패턴 번호

	m_recvPacket
		>> nId
		>> btPattern;
	
	m_mgFishHook.OnSetPattern( nId, btPattern );*/
} // CGame::OnFishingPattern


/**
 * 낚시 위치값 
 */
void CGame::OnPacketFishingPos()
{
/*	int		nId;			// 유저 idx
	int		nX;
	int		nY;	

	m_recvPacket
		>> nId
		>> nX
		>> nY;

	m_mgFishHook.OnFishingPos( nId, nX, nY );*/
} // CGame::OnPacketFishingPos

/*
 *	Update Pet Name
 */
void CGame::OnPacketPetUpdateName()
{
	int			nIdx;			// 케릭터 고유번호 
	string		strPetName;		// 팻 이름

	m_recvPacket
		>> nIdx		
		>> strPetName;

	IW3DCreature*	pCharacter = m_pWorld->GetCharacter( nIdx );
	if( !pCharacter )
	{
		Assert( !"Do Not Exist the Character" );
		return;
	} //if

	pCharacter->UpdatePet( strPetName.c_str(), NULL );
	if( pCharacter->GetId() == m_dwPlayer )
	{
		g_pGUI->SetText( "EDT_PET_NAME", strPetName.c_str() );
	} //if
} // CGame::OnPacketUpdateName

/*
 *	팻 경험치 업데이트
 */
void CGame::OnPacketPetUpdateExp()
{
	int nExp;	// 경험치
	
	m_recvPacket
		>> nExp;

	g_infoPet.nExp = nExp;

	char		szBuff[32];
	sprintf( szBuff, "%d/%d", nExp, g_infoPet.nNeedExp );
	g_pGUI->SetText( "TXT_PET_EXP", szBuff );
	g_pGUI->SetProgressPos( "PRO_PET_EXP", g_infoPet.nNeedExp, g_infoPet.nExp );
}

/*
 *	거래 시작
 */
void CGame::OnPacketDealBeginOk()
{
	g_pGUI->ShowUserDealBox( TRUE );
}

/*
 *	친구 로그인 
 */
void CGame::OnPacketFriendLogin()
{
	string szName;

	m_recvPacket 
		>> szName;

	g_pGUI->FriendLogInOut( szName.c_str(), TRUE );
}

/*
 *	친구 로그아웃
 */
void CGame::OnPacketFriendLogout()
{
	string szName;

	m_recvPacket
		>> szName;

	g_pGUI->FriendLogInOut( szName.c_str(), FALSE );
}


/*
 *	몬스터 이팩트 출력 
 */
void CGame::OnPacketMobPopEffect()
{
	int nIdx;				// 몬스터 고유번호
	WORD wEffect;			// 이팩트 번호 

	m_recvPacket
		>> nIdx
		>> wEffect;

	IW3DCreature*		pCreature = m_pWorld->GetMonster( nIdx );
	if( !pCreature )
	{
		Assert( !"Not Found the Creature!" );
		return;
	} //if
	
	switch( wEffect )
	{
	case 1:			// 스턴 이팩트
		m_pWorld->AddMagic( "스턴공격", pCreature, pCreature );
		break;
	case 2:			// 이동 못함 
		m_pWorld->AddMagic( "이동정지", pCreature, pCreature );
		break;
	case 3:			// 공격 못함
		m_pWorld->AddMagic( "공격정지a", pCreature, pCreature );
		break;
	} //switch
} //CGame::OnPacketMobPopEffect

/**
 *	플레이어 이팩트 출력
 */
void CGame::OnPacketPlayerPopEffect()
{
	int nIdx;				// 케릭터 고유번호 
	WORD wEffect;			// 이팩트 번호

	m_recvPacket
		>> nIdx
		>> wEffect;

	IW3DCreature*		pCreature = m_pWorld->GetCharacter( nIdx );
	if( !pCreature )
	{
		Assert( !"Not Found the Creature!" );
		return;
	} //if
	
	switch( wEffect )
	{
	case 1:			// 스턴 이팩트
		m_pWorld->AddMagic( "스턴공격", pCreature, pCreature );
		break;
	case 2:			// 이동불가 
		break;
	case 3:			// 공격 못함
		break;
	case 4:			// 웜스 오브 아라임	1.3배 
		m_pWorld->AddMagic( "캐쉬0032", pCreature, pCreature );
		break;
	case 5:			// 오오라 오브 브릴리언스
		m_pWorld->AddMagic( "캐쉬0002", pCreature, pCreature );
		break;
	case 6:			// 팬텀게더러
		m_pWorld->AddMagic( "캐쉬0003", pCreature, pCreature );
		break;
	case 7:			// 외침용알약
		m_pWorld->AddMagic( "캐쉬0006", pCreature, pCreature );
		break;
	case 8:			// 아이템 오브 아라임	1.3배 
		m_pWorld->AddMagic( "캐쉬0040", pCreature, pCreature );
		break;
	case 9:			// 부활의 두루마리 & 이동포탈문서
		m_pWorld->AddMagic( "캐쉬0008", pCreature, pCreature );
		break;
	case 10:		// 길드공지의 알약 
		m_pWorld->AddMagic( "캐쉬0012", pCreature, pCreature );
		break;
	case 11:		// 프리미엄 존 
		m_pWorld->AddMagic( "캐쉬0028", pCreature, pCreature );
		break;
	case 12:		// 프리미엄 존 플러스 
		m_pWorld->AddMagic( "캐쉬0029", pCreature, pCreature );
		break;
	case 13:		// 펫 푸드 
		break;
	case 14:		// 신의축복 
		m_pWorld->AddMagic( "캐쉬0021", pCreature, pCreature );
		break;
	case 15:		// 인챈트 체인지 
		m_pWorld->AddMagic( "캐쉬0022", pCreature, pCreature );
		break;
	case 16:		// 인첸트 콜렉션 
		m_pWorld->AddMagic( "캐쉬0023", pCreature, pCreature );
		break;
	case 17:		// 아이템룩 
		m_pWorld->AddMagic( "캐쉬0024", pCreature, pCreature );
		break;
	case 18:		// 아이템 프로텍션 
		m_pWorld->AddMagic( "캐쉬0025", pCreature, pCreature );
		break;
	case 19:		// 레벨룩 
		m_pWorld->AddMagic( "캐쉬0026", pCreature, pCreature );
		break;
	case 20:		// 레벨 프로텍션 
		m_pWorld->AddMagic( "캐쉬0027", pCreature, pCreature );
		break;
	case 21:		// 기동성의 물약 
		m_pWorld->AddMagic( "캐쉬0005", pCreature, pCreature );
		break;
	case 22:		// 웨이트 드링크 
		m_pWorld->AddMagic( "캐쉬0010", pCreature, pCreature );
		break;
	case 23:		// HP 드링크 
		m_pWorld->AddMagic( "캐쉬0014", pCreature, pCreature );
		break;
	case 24:		// MP 드링크 
		m_pWorld->AddMagic( "캐쉬0015", pCreature, pCreature );
		break;
	case 25:		// 그룹 웜쓰 오브 아라임 
		m_pWorld->AddMagic( "캐쉬0016", pCreature, pCreature );
		break;
	case 26:		// 그룹 HP 드링크
		m_pWorld->AddMagic( "캐쉬0017", pCreature, pCreature );
		break;
	case 27:		// 그룹 MP 드링크
		m_pWorld->AddMagic( "캐쉬0018", pCreature, pCreature );
		break;
	case 28:		// 군단원 HP 드링크 
		m_pWorld->AddMagic( "캐쉬0019", pCreature, pCreature );
		break;
	case 29:		// 군단원 MP 드링크
		m_pWorld->AddMagic( "캐쉬0020", pCreature, pCreature );
		break;
	case 30:		// 웜쓰 오브 아라임 1.5배 
		m_pWorld->AddMagic( "캐쉬0038", pCreature, pCreature );
		break;
	case 31:		// 아이템 오브 아라임 1.5배 
		m_pWorld->AddMagic( "캐쉬0046", pCreature, pCreature );
		break;
	case 32:		// 단원 소환
		m_pWorld->AddMagic( "캐쉬0060", pCreature, pCreature );
		break;
	case 33:		// 단원 필승의 외침
		m_pWorld->AddMagic( "캐쉬0061", pCreature, pCreature );
		break;
	case 34:		// 단원 축복의 외침
		m_pWorld->AddMagic( "캐쉬0062", pCreature, pCreature );
		break;
	case 35:		// 단원 기원의 외침
		m_pWorld->AddMagic( "캐쉬0063", pCreature, pCreature );
		break;
	case 36:		// 단원 저주의 외침
		m_pWorld->AddMagic( "캐쉬0064", pCreature, pCreature );
		break;
	case 37:		// 단원 도발의 외침 
		m_pWorld->AddMagic( "캐쉬0065", pCreature, pCreature );
		break;
	case 38:		// 마이너스 
		m_pWorld->AddMagic( "캐쉬0066", pCreature, pCreature );
		break;
	case 39:		// 하트 이모티콘
		m_pWorld->AddMagic( "캐쉬0070", pCreature, pCreature );
		break;
	case 40:		// 기쁨 이모티콘
		m_pWorld->AddMagic( "캐쉬0071", pCreature, pCreature );
		break;
	case 41:		// 슬픔 이모티콘
		m_pWorld->AddMagic( "캐쉬0072", pCreature, pCreature );
		break;
	case 42:		// 분노 이모티콘
		m_pWorld->AddMagic( "캐쉬0073", pCreature, pCreature );
		break;
	} //switch
} //CGame::OnPacketPlayerPopEffect

/*
 *	플레이어 상태 변환
 */
void CGame::OnPacketChangeStatus()
{
	BYTE btStatus;

	m_recvPacket
		>> btStatus;

	if( !g_pPlayer )
	{
		Assert( !"Empty Player" );
		return;
	} //if

	switch( btStatus )
	{
	case 1:			// 스턴 
		g_infoPlayer.btStun = TRUE;
		g_pPlayer->SetState( STATE_STOP );
		break;
	case 2:			// 스턴 해제
		g_infoPlayer.btStun = FALSE;
		break;
	}

} //CGame::OnPacketChangeStatus

/**
 *	길드 맴버 리스트
 */
void CGame::OnPacketGuildMemberList()
{
	WORD			wCnt;		// 총 수
	string			sName;		// 케릭터명
	int				nLevel;		// 통합레벨 
	BYTE			btClass;	// 직업 

	m_recvPacket >> wCnt;

	g_pGUI->ClearGuildMember();
	for( int i = 0; i < wCnt; i++ )
	{
		m_recvPacket 
			>> sName
			>> nLevel
			>> btClass;

		g_pGUI->AddGuildMember( sName.c_str(), btClass, nLevel );
	} //for

	g_pGUI->ShowDlg( "DLG_GUILDMEMBERLIST", TRUE );
} //CGame::OnPacketGuildMemberList

/**
 * 개인상점 개점 결과
 */
void CGame::OnPacketMyShopOpenResult()
{
	g_pGUI->ShowOpenMyShop( TRUE );
} //CGame::OnPacketMyShopOpenResult

/**
 * 개인상점 상호 변경
 */
void CGame::OnPacketMyShopChangeTitle()
{
	Assert( g_pPlayer );

	string		sTitle;
	m_recvPacket >> sTitle;

	g_pPlayer->SetShopInfo( sTitle.c_str() );
} //CGame::OnPacketMyShopOpenResult

/**
 * 개인상점 아이템 추가
 */
void CGame::OnPacketMyShopAddItem()
{
	int			nItemId;
	int			nItemIdx;
	
	m_recvPacket 
		>> nItemId
		>> nItemIdx;

	g_pGUI->InsertItemInMyShop( nItemId, nItemIdx );

} //CGame::OnPacketMyShopAddItem

void CGame::OnPacketSmithAddItem( )
{
	int			nItemId;
	int			nItemIdx;
	int			nPriceEnchent;

	m_recvPacket 
		>> nItemId
		>> nItemIdx
		>> nPriceEnchent;

	g_pGUI->InsertSmithItem( nItemId, nItemIdx, nPriceEnchent );
}

void CGame::OnPacketSmithFinish()
{
	g_pGUI->ShowSmithEnchentBox( FALSE );
}

void CGame::OnPacketSmithRemoveItem()
{
	int		nItemId;
	
	m_recvPacket
		>> nItemId;

	g_pGUI->RemoveSmithItem( nItemId );
}

/**
 * 개인상점 아이템 제거
 */
void CGame::OnPacketMyShopRemoveItem()
{
	int			nItemId;

	m_recvPacket 
		>> nItemId;

	g_pGUI->RemoveItemInMyShop( nItemId );
} //CGame::OnPacketMyShopRemoveItem

/**
 * 개인상점 판매시작
 */
void CGame::OnPacketMyShopSellBegin()
{
	g_pGUI->SetText( "BTN_MYSHOP_BEGIN", STR_MYSHOP_CLOSE );
	m_bNoMove = TRUE;
} //CGame::OnPacketMyShopSellBegin

/**
 * 개인상점 판매종료
 */
void CGame::OnPacketMyShopSellFinish()
{
	g_pGUI->SetText( "BTN_MYSHOP_BEGIN", STR_MYSHOP_OPEN );
	g_pGUI->ShowOpenMyShop( FALSE );
	m_bNoMove = FALSE;
} //CGame::OnPacketMyShopSellFinish

/**
 * 개인상점 아이템 리스트
 */
void CGame::OnPacketMyShopItemList()
{
	WORD		wItemCnt;
	int			nItemId;
	int			nItemIdx;
	int			nItemPrice;
	BYTE		btNum;

	m_recvPacket
		>> wItemCnt;

	g_pGUI->ShowPersonalShop( TRUE );
	for( int nCnt = 0; nCnt < wItemCnt; nCnt++ )
	{
		m_recvPacket
			>> nItemId
			>> nItemIdx
			>> nItemPrice
			>> btNum;

		g_pGUI->InsertItemInPersonalShop( nItemId, nItemIdx, nItemPrice, btNum );
	} //for
} //CGame::OnPacketMyShopItemList

/**
 * 개인상점 업뎃
 */
void CGame::OnPacketMyShopUpdateInfo()
{
	int			nCharId;
	BYTE		btEnable;
	string		sTitle;

	m_recvPacket
		>> nCharId
		>> btEnable
		>> sTitle;

	IW3DCreature*		pCharacter = m_pWorld->GetCharacter( nCharId );
	if( !pCharacter )
	{
		Assert( !"Not Found The Character" );
		return;
	} //if

	if( btEnable )	
	{
		pCharacter->SetShopInfo( sTitle.c_str() );
	}
	else
	{
		pCharacter->SetShopInfo( "" );
		if( m_dwPShopId == nCharId )
		{
			g_pGUI->ShowPersonalShop( FALSE );
		} //if
	} //if..else
} //CGame::OnPacketMyShopUpdateInfo

/**
 * 개인상점 아이템 삭제 (개인상점에서 아이템을 사고 있을때 판매목록에서 삭제)
 */
void CGame::OnPacketMyShopRemoveListItem()
{
	int			nItemId;

	m_recvPacket
		>> nItemId;

	g_pGUI->RemoveItemInPersonalShop( nItemId );
} //CGame::OnPacketMyShopRemoveListItem

/*
 *	개인상점 판매대금 업데이트
 */
void CGame::OnPacketMyshopUpdateCash()
{
	int		nMoney;		// 금액

	m_recvPacket
		>> nMoney;

	g_pGUI->SetText( "TXT_MYSHOP_PRICE", nMoney );
} //CGame::OnPacketMyshopUpdateCash

/*
 *	세계채널 채팅
 */
void CGame::OnPacketWorldChat()
{
	string		szName;
	string		szChat;

	m_recvPacket
		>> szName
		>> szChat;
	
	g_pGUI->ChattingInput( 
		szName.c_str(), 
		szChat.c_str(), 
		COLOR_ARGB( 255, 26, 198, 3 ), 
		0,
		IW3DUInterface::CHATT_WORLD
		);	
} // CGame::OnPacketWorldChat

/*
 *	오브젝트 몬스터 Insert 
 */
void CGame::OnPacketInsertObjMob()
{
	SW3DMonsterInfo	infoMonster;	// 몬스터 정보
	WORD			wIdx;			// 몬스터 고유번호 
	int				nAngle;			// 각도
	int				nHP;			// HP
	int				nMaxHP;			// 최대HP

	m_recvPacket
		>> wIdx
		>> infoMonster.wType	// 몹번호 1001 = 성문 , 1002 = 성물
		>> infoMonster.nX
		>> infoMonster.nY
		>> nAngle
		>> nHP
		>> nMaxHP;

	IW3DCreature*	pMonster = m_pWorld->GetMonster( wIdx );
	if( pMonster )
	{	// 몬스터 정보가 이미 존재하는데 이 패킷이 왔다면 기존의 데이터를 제거한다.
		m_pWorld->RemoveMonster( wIdx );
		if( g_pPickMonster && g_pPickMonster->GetId() == wIdx )
		{
			g_pPickMonster = NULL;
		} //if
	} //if

	m_pWorld->AddMonster( wIdx, reinterpret_cast<void*>( &infoMonster ) );
	pMonster = m_pWorld->GetMonster( wIdx );
	if( !pMonster )
	{
		Assert( !"Not Found The Mob" );
		return;
	} //if

	pMonster->SetDirection( nAngle );
	pMonster->SetHp( nHP, nMaxHP );
	
	Vector3		vPos;
	vPos.x = static_cast<float>( infoMonster.nX ) + 0.5F;
	vPos.y = static_cast<float>( infoMonster.nY ) + 0.5F;
	vPos.z = m_pWorld->GetHeight( vPos );

//	if( isCreate )
//		m_pWorld->AddEffect( "mob_appear.wed", vPos );

	g_dtGame.wMonsterCount++;
} //CGame::OnPacketInsertObjMob

/*
 *	인벤토리 무게게이지 업데이트 
 */
void CGame::OnPacketUpdateWeightGage()
{
	int nCurrWeight;		// 현재 무게게이지 
	int nMaxWeight;			// 최대 무게게이지

	m_recvPacket 
		>> nCurrWeight
		>> nMaxWeight;

	if( nMaxWeight < nCurrWeight )
	{
		Assert( nMaxWeight >= nCurrWeight );
		nCurrWeight = 100;
		nMaxWeight  = 100;
	} //if

	DWORD		dwColor  = COLOR_ARGB( 255, 10, 200, 10 );
	int			nPercent = ( (float)nCurrWeight / (float)nMaxWeight ) * 100;
	if( nPercent >= 100 )		dwColor = COLOR_ARGB( 255, 200, 0, 0 );
	else if( nPercent > 89 )	dwColor = COLOR_ARGB( 255, 150, 10, 10 );

	g_pGUI->SetNowWeight( nCurrWeight, nMaxWeight );
	g_pGUI->SetProgressPos( "PRO_INVEN_MG", nMaxWeight, nCurrWeight, TRUE, dwColor );

	char	szText[16];
	sprintf( szText, "%d%%", nPercent );
	g_pGUI->SetText( "TXT_INVEN_MG", szText );
} //CGame::OnPacketUpdateWeightGage

/*
 *	케릭터 이름색 변경 
 */
void CGame::OnPacketChangeNameColor()
{
	int nIdx;					// 케릭터 고유번호 
	BYTE btR, btG,btB;			// r, g, b

	m_recvPacket
		>> nIdx
		>> btR 
		>> btG
		>> btB;

	IW3DCreature*		pCharacter = m_pWorld->GetCharacter( nIdx );
	if( !pCharacter )
	{
		Assert( !"Not Found The Character" );
		return;
	} //if

	pCharacter->SetNameColor( COLOR_ARGB( 0xFF, btR, btG, btB ) );
} //CGame::OnPacketChangeNameColor


/*
 * 진지 정보 	
 */
void CGame::OnPacketUpdateWarCamp()
{
	int			nFirstCampOwnerGuild;	// 첫번째 진지 소유길드번호  0이면 주인없음 
	string		szFirstCampOwnerName;	// 첫번째 진지 소유길드이름

	int			nSecondCampOwnerGuild;	// 두번째 진지 소유길드번호	 0이면 주인없음
	string		szSecondCampOwnerGuild;	// 두번쨰 진지 소유길드이름

	m_recvPacket
		>> nFirstCampOwnerGuild
		>> szFirstCampOwnerName
		>> nSecondCampOwnerGuild
		>> szSecondCampOwnerGuild;

	if( nFirstCampOwnerGuild > 0 )
	{
		IW3DFuncEntity*		pFuncEntity1 = m_pWorld->GetFuncEntity( nFirstCampOwnerGuild );
		if( !pFuncEntity1 )
		{
			Assert( !"Not Found The Entity" );
			return;
		} //if
		pFuncEntity1->SetName( szFirstCampOwnerName.c_str() );
	} //if

	if( nSecondCampOwnerGuild > 0 )
	{
		IW3DFuncEntity*		pFuncEntity2 = m_pWorld->GetFuncEntity( nSecondCampOwnerGuild );
		if( !pFuncEntity2 )
		{
			Assert( !"Not Found The Entity" );
			return;
		} //if
		pFuncEntity2->SetName( szSecondCampOwnerGuild.c_str() );
	} //if
}//CGame::OnPacketUpdateWarCamp

/*
 *	전쟁 정보
 */
void CGame::OnPacketUpdateWarinfo()
{
	int			nCastleOwnerGuild;		// 성의 주인 길드번호
	string		szCastleOwnerName;		// 성의 주인 길드이름

	int			nGuild1;				// 전쟁중인 길드 1 길드번호 
	string		szGuildName1;			// 전쟁중인 길드 1 길드이름

	int			nGuild2;				// 전쟁중인 길드 2 길드번호
	string		szGuildName2;			// 전쟁중인 길드 2 길드이름

	m_recvPacket
		>> nCastleOwnerGuild
		>> szCastleOwnerName
		>> nGuild1
		>> szGuildName1
		>> nGuild2
		>> szGuildName2;

}//CGame::OnPacketUpdateWarinfo

/*
 *	창고 돈 업데이트
 */
void CGame::OnPacketUpdateGarbageMoney()
{
	int nAmount; // 변경 금액

	m_recvPacket
		>> nAmount;

	g_pGUI->SetText( "TXT_STORAGE_GOLD", nAmount );
}//CGame::OnPacketUpdateGarbageMoney


/*
 *	PVP 모드 변환
 */
void CGame::OnPacketUpdatePvPMode()
{
	BYTE btMode;

	m_recvPacket
		>> btMode;

//#ifdef _CHINAVERSION
//	DWORD		dwIdPkSkillItem[4] = { 10407, 10408, 10409, 10410 };	
//#else
	DWORD		dwIdPkSkillItem[4] = { 10452, 10453, 10454, 10455 };
//#endif

	switch( btMode )
	{
	case 0:		g_pGUI->ChattingInput( "", STR_UIMSG_NONEPKMODE );		break;	
	case 1:		g_pGUI->ChattingInput( "", STR_UIMSG_FREEPKMODE );		break;
	case 2:		g_pGUI->ChattingInput( "", STR_UIMSG_GUILDPKMODE );		break;
	case 3:		g_pGUI->ChattingInput( "", STR_UIMSG_PARTYPKMODE );		break;
	default:	Assert( 0 )												return;
	}
	
	g_infoPlayer.btPKMode = btMode;

	SItemInfoData*		pItemInfoDat = g_pItemInfoMgr->GetItemInfo( dwIdPkSkillItem[btMode] );
	if( !pItemInfoDat )
	{
		Assert( !"Not Found The ItemInfo" );
		return;
	} //if

	char		szFileName[32];
	sprintf( szFileName, "Item/%s", pItemInfoDat->szFimageFile5 );

	g_pGUI->SetPicture( "PIC_CHAR_PKMODE", szFileName );
	// #define dPACKET_SET_PVP_MODE					226		// PvP 모드 변환      (클라이언트->서버)
	// 226 번 패킷을 보낼경우에 서버에서 보내주는 응답패킷 입니다.
	// dPACKET_SET_PVP_MODE 패킷을 보낼때에 BYTE형으로 상기 btMode 값을 보내시면 됩니다.
}

/*
 *	성 목록
 */
void CGame::OnPacketSendCastleList()
{
	WORD wCnt;			// 데이터수 
	int	 nMapNum;		// 맵번호

	m_recvPacket
		>> wCnt;

	for( int i = 0; i < wCnt; i++ )
	{
		m_recvPacket
			>> nMapNum;

		g_pGUI->AddCastleList( i, nMapNum );
	} //for
	
	g_pGUI->ShowDlg( "DLG_CASTLELIST", TRUE );
} //CGame::OnPacketSendCastleList

/*
 *	전쟁정보	
 */
void CGame::OnPacketWarState()
{
	string szMapName;
	string szOwnerGuildName;

	int		nTaxRate;			// 세율 
	int		nTaxBalance;		// 쌓인 세금 

	string szGuildName1;
	string szGuildName2;

	int		nWarPoint1;
	int		nWarPoint2;

	BYTE	btType1, btType2;			// 타입 , 0은 데이터 없음 .. 1 = 진지 , 2 = 성

	int		i;
	int		nCount = 0;

	m_recvPacket
		>> szMapName					// 맵 이름 
		>> szOwnerGuildName				// 성 주인 길드이름
		>> nTaxRate						// 세율 
		>> nTaxBalance;					// 쌓인 세금 

	char	szName[64];
	sprintf( szName, "[%s] %s", szMapName.c_str(), szOwnerGuildName.c_str() );
	g_pGUI->SetText( "TXT_WARGUILDINFO_NAME", szMapName.c_str() );
//	g_pGUI->SetText( "TXT_WARINFO_OWNERGUILD", szOwnerGuildName.c_str() );

	// 구축된 진지 ( Max 2개 )
	for( i = 0; i < 2; i++ )
	{
		// 전쟁관계인 두길드간의 길드이름과 전쟁포인트
		// 없을경우 길드이름은 "" 이다
		m_recvPacket
			>> btType1
			>> szGuildName1						
			>> nWarPoint1
			>> btType2
			>> szGuildName2
			>> nWarPoint2;

		g_pGUI->AddWarGuildInfo( 
			nCount,
			szGuildName2.c_str(), btType2, nWarPoint2,
			szGuildName1.c_str(), btType1, nWarPoint1
		);

		nCount++;
	} //for

	// 인접성 정보 ( Max 4개 )
	for( i = 0; i < 4; i++ )
	{
		// 전쟁관계인 두길드간의 길드이름과 전쟁포인트
		// 없을경우 길드이름은 "" 이다
		m_recvPacket
			>> btType1
			>> szGuildName1
			>> nWarPoint1
			>> btType2
			>> szGuildName2
			>> nWarPoint2;

		g_pGUI->AddWarGuildInfo( 
			nCount,
			szGuildName1.c_str(), btType1, nWarPoint1,
			szGuildName2.c_str(), btType2, nWarPoint2
		);

		nCount++;
	} //for

	g_pGUI->ShowDlg( "DLG_WARGUILDINFO", TRUE );
} //CGame::OnPacketWarState

/*
 *	세율 조정 창 열기 
 */
void CGame::OnPacketOpenTaxWindow()
{
	int		nCurrTax;				// 현재 세율

	m_recvPacket
		>> nCurrTax;

	char	szText[8];
	sprintf( szText, "%d %%", nCurrTax );
	
	g_pGUI->SetText( "TXT_TEX_CURRENT", szText );
	g_pGUI->SetText( "TXT_TEX_NEXT", szText );
	g_pGUI->ShowDlg( "DLG_TEX", TRUE );
}//CGame::OnPacketOpenTaxWindow

/*
 *	세율 업데이트 
 */
void CGame::OnPacketUpdateTax()
{
	int		nCurrTax;

	m_recvPacket
		>> nCurrTax;

	char	szText[8];
	sprintf( szText, "%d %%", nCurrTax );

	g_pGUI->SetText( "TXT_TEX_CURRENT", szText );
	g_pGUI->SetText( "TXT_TEX_NEXT", szText );
	g_pGUI->ShowDlg( "DLG_TEX", TRUE );
} //CGame::OnPacketUpdateTax

/**
 * 유료아이템 목록 
 */
void CGame::OnPacketBillingItemInfo()
{
	WORD wCnt;
	
	int nIdx;				// 아이템 인덱스 
	int nItemNum;			// 아이템 번호 
	int nContainCnt;		// 수량

	g_pGUI->CleanupCashInven();

	m_recvPacket
		>> wCnt;

	// 현재 효과가 먹은 유료아이템들 ( 유료인터페이스의 상단 )
	for( int i = 0; i < wCnt; i++ )
	{
		m_recvPacket
			>> nItemNum;

		g_pGUI->InsertItemInCash_Buff( nItemNum );
	} //for

	// 구입해놓은 (현재 쓸 수 있는) 유료아이템 

	m_recvPacket
		>> wCnt; 

	for( i = 0; i < wCnt; i++ )
	{
		m_recvPacket
			>> nIdx
			>> nItemNum
			>> nContainCnt;

		g_pGUI->InsertItemInCash( nIdx, nItemNum, nContainCnt );
	} //for
	
	g_pGUI->ShowDlg( "DLG_CASH", TRUE );	
#ifdef _DEBUG
	g_pGUI->ShowDlg( "DLG_BASIC_NPCTALK",  TRUE );
#endif
	g_pGUI->CheckGroup( "CHK_CASH_TAB", "CASHITEM_IN_", 1 );
} //CGame::OnPacketBillingItemInfo

/*
 * 유료아이템 삭제	
 */
void CGame::OnPacketRemoveBillingItem()
{
	int nIdx;			// 아이템 인덱스

	m_recvPacket
		>> nIdx;

	g_pGUI->DeleteItemInCash( nIdx );
} //CGame::OnPacketRemoveBillingItem

/*
 *	맵 전체 외침
 */
void CGame::OnPacketMapShout()
{
	string szName;
	string szStr;

	m_recvPacket
		>> szName
		>> szStr;

	g_pGUI->ChattingInput( szName, szStr, COLOR_ARGB( 255, 255, 0, 255 ), 0 );
} //CGame::OnPacketMapShout

/*
 *	길드의 적군 정보
 */
void CGame::OnPacketEnemyDivision()
{
	int		nEnemy[6];		

	for( int i = 0; i < 6; i++ )
	{
		m_recvPacket
			>> nEnemy[i];

		g_anGuildId[i] = nEnemy[i];
	} //for

	IW3DCreature*		pCharacter = m_pWorld->BeginCharacter();
	while( pCharacter )
	{
		pCharacter->ResetEnemy();
		pCharacter = m_pWorld->NextCharacter();
	} //while
} //CGame::OnPacketEnemyDivision

/*
 *	퀵슬롯의 아이템 타이머 ( 쿨타임용 )
 */
void CGame::OnPacketShowItemTimer()
{
	int		nItemNum;
	int		nTime;

	m_recvPacket
		>> nItemNum
		>> nTime;

	g_pGUI->SetSkillTimer( nItemNum, nTime );
} //CGame::OnPacketShowItemTimer




void CGame::OnPacketFriendRequest()
{

}