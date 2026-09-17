/**
 *	$Workfile: message.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 37 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/message.h $
 * 
 * 37    04-12-15 10:40a Sk8snow
 * 
 * 36    04-09-11 11:20p Sk8snow
 * 
 * 35    04-09-08 3:54p Sk8snow
 * 
 * 34    04-08-04 9:27a Sk8snow
 * 
 * 33    04-07-13 7:26a Sk8snow
 * 
 * 32    04-05-29 10:12p Sk8snow
 * 
 * 31    04-05-23 1:45p Sk8snow
 * 
 * 30    04-05-21 11:37a Sk8snow
 * 
 * 29    04-05-18 1:17a Sk8snow
 * 
 * 28    04-05-11 1:45a Sk8snow
 * 
 * 27    04-04-30 9:46a Sk8snow
 * 
 * 26    04-04-03 4:03p Sk8snow
 * 
 * 25    04-04-01 5:45p Sk8snow
 * 
 * 24    04-03-31 8:22a Sk8snow
 *        
 */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define dMAX_PACKET					1024

#define dFIRST_PACKET				1					

#define dPACKET_RESERVED				0

#define dPACKET_LOGINREQUEST			1		// 로그인 요청 
#define dPACKET_LOGINRESULT				2		// 로그인 결과

#define dPACKET_CHARINFO				3		// 케릭터 정보
#define dPACKET_GAMESTART				4		// 게임 시작
#define dPACKET_MAPLOADING				5		// 맵 로딩
#define dPACKET_MAPLOADING_END			6		// 맵 로딩 끝
#define dPACKET_CHARACTER_INSERT			7		// 케릭터 추가
#define dPACKET_CHARACTER_REMOVE			8		// 케릭터 삭제
#define dPACKET_MAGIC_FAILED			9		// 마법 실패
#define dPACKET_CHARACTER_DISCONN			10		// 케릭터 삭제 ( 접속 해제시 )
#define dPACKET_CHARACTER_MOVE			11		// 케릭터 이동
#define dPACKET_CHARACTER_DIRECTION		12		// 케릭터 바라봄

#define dPACKET_MONSTER_INSERT			13		// 몬스터 추가
#define dPACKET_MONSTER_REMOVE			14		// 몬스터 제거
#define dPACKET_MONSTER_MOVE			15		// 몬스터 이동

#define dPACKET_CHAT_MSG				16		// 채팅

#define dPACKET_ATTACK_USERMOB			17     	// 캐릭터 -> 몹 공격
#define dPACKET_ATTACK_MOBUSER			18     	// 몹     -> 캐릭터 공격
#define dPACKET_MOB_HPINFO				19		// 몬스터 체력 정보

#define dPACKET_DAMAGE_USER				20     	// 플레이어 -> 몹 타격
#define dPACKET_DAMAGE_MOB				21     	// 몹 -> 플레이어 타격 

#define dPACKET_DIE_USER				22     	// 유저의 죽음
#define dPACKET_DIE_MOB				23		// 몹의 죽음

#define dPACKET_RESTART				24		// 케릭터 죽었을때 재시작 옵션

#define dPACKET_CHAT_WHISPER			25		// 귓속말
#define dPACKET_CHAT_WHISPER_RES			26		// 귓속말 결과

#define dPACKET_SKILL_UP				27		// 스킬 레벨 업 
#define dPACKET_GET_EXP				28		// 경험치 획득 메시지
#define dPACKET_SKILLUP_EVENT			29		// 레벨업 이벤트

#define dPACKET_HPGAIN_USER				30		// 유져 HP 회복
#define dPACKET_MPGAIN_USER				31		// 유져 MP 회복
	
#define dPACKET_HPGAIN_MOB				32		// 몹 HP 회복

#define dPACKET_GETALLCHAR				33		// 모든 케릭터 기본정보 받기
#define dPACKET_GETALLCHAR_RESULT			34		// 
#define dPACKET_SELECT_CHARACTER			35		// 케릭터 선택
#define dPACKET_NULL_CHARACTER			36		// 케릭터 없음
#define dPACKET_MAKECHAR				37		// 신규 케릭터 생성
#define dPACKET_MAKECHAR_RESULT			38		// 신규 케릭 생성 결과

#define dPACKET_DELETE_CHARACTER			39		// 케릭터 삭제 
#define dPACKET_DELETE_CHARACTER_RESULT		40		// 케릭터 삭제 결과

#define dPACKET_INSERT_MAPITEM			41		// 맵에 아이템 나타나기
#define dPACKET_REMOVE_MAPITEM			42		// 맵에서 아이템 사라지기
#define dPACKET_INSERT_INVENTORY			43		// 케릭터한테 아이템 주기
#define dPACKET_REMOVE_INVENTORY			44		// 케릭터한테서 아이템 뺏기
#define dPACKET_MOVE_ITEM				45		// 아이템 이동 ( 인벤토리 / 클라이언트 )
#define dPACKET_DROP_ITEM				46		// 아이템 버리기 ( 클라이언트 )
#define dPACKET_GET_ITEM				47		// 아이템 줍기 ( 클라이언트 )
#define dPACKET_INSERT_EQUIP_ITEM			48		// 아이템 장비에 넣기
#define dPACKET_REMOVE_EQUIP_ITEM			49		// 아이템 장비에서 빼기
#define dPACKET_EQUIP_ITEM				50		// 아이템 장착 ( 클라이언트 )
#define dPACKET_CHANGE_HAND_ITEM			51		// 손에 쥔 아이템 바꾸기 ( 클라이언트 )

#define dPACKET_SET_MAIN_SKILL			52		// 메인 스킬 바꾸기 ( 클라이언트 )
#define dPACKET_CHANGE_MAIN_SKILL			53		// 메인 스킬 바꾸기 

#define dPACKET_ITEM_TO_HAND			54		// 아이템 -> 손 ( 서버 )
#define dPACKET_ITEM_FROM_HAND			55		// 손 -> ....??? (서버)
#define dPACKET_HOLDING_ITEM			56		// 아이템 -> 손 ( 클라이언트 )

#define dPACKET_SET_INVENXY				57		// 아이템 위치 바꾸기

#define dPACKET_ALREADY_CONNECTED			58		// 이미 접속중인 아이디

#define dPACKET_UPDATE_MAXHPMP			59		// MP, HP Up

#define dPACKET_PLAYERS_ITEM_INFO			60		// 아이템 정보 전송 (소지품, 장비)

#define dPACKET_CHANGE_EQUIP			61		// 장비 바뀜

#define dPACKET_REMOVE_SPELLBOOK			62		// 서버->클라이언트 
#define dPACKET_MOVETO_SPELLBOOK			63		// 클라이언트->서버

#define dPACKET_CHANGE_TIME				64		// 게임내의 시간변화

#define dPACKET_UPDATE_MONEY			65		// 돈 업데이트

#define dPACKET_GET_SHOPITEM			66		// 상점의 판매목록 받기 (클->서)
#define dPACKET_SHOPITEM_LIST			67		// 상점 판매 목록 (서->클)
#define dPACKET_SELL_ITEM				68		// 아이템 팔기 (클->서)
#define dPACKET_BUY_ITEM				69		// 아이템 사기 (클->서)
#define dPACKET_SHOP_RESULT				70		// 상점 거래 결과

#define dPACKET_BEGIN_PLAYER_ATTACK		71		// 공격 시작 패킷 (서버->클라)
#define dPACKET_END_PLAYER_ATTACK			72		// 공격 종료 패킷 (클라->서버)

#define dPACKET_CAST_ITEM				73		// 아이템 사용 (클라->서버)

#define dPACKET_DEAL_REQUEST			74		// 거래 요청 (클라->서버)						
#define dPACKET_DEAL_BEGIN				75		// 거래 시작 (서버->클라)
#define dPACKET_DEAL_FINISH				76		// 거래 종료 (서버->클라)		
#define dPACKET_DEAL_CANCEL				77		// 거래 취소 (클라->서버)
#define dPACKET_DEAL_ADDITEM			78		// 거래 아이템 추가 (클라->서버)
#define dPACKET_DEAL_DELITEM			79		// 거래 아이템 삭제 (클라->서버)
#define dPACKET_DEAL_INSERT_ITEM			80		// 거래 아이템 추가 (서버->클라)
#define dPACKET_DEAL_REMOVE_ITEM			81		// 거래 아이템 삭제 (서버->클라)
#define dPACKET_DEAL_ADDMONEY			82		// 거래창에 돈 추가 (클라->서버)
#define dPACKET_DEAL_UPDATEMONEY			83		// 거래창의 돈 정보 (서버->클라)
#define dPACKET_DEAL_CHECKIN			84		// 거래 수락 (클라->서버)
#define dPACKET_DEAL_REQUEST_SUBMIT		85		// 최종 거래 수락 요청(서버->클라)
#define dPACKET_DEAL_SUBMIT_OK			86		// 최종 거래 수락(클라->서버)

#define dPACKET_PARTY_CREATE			87		// 파티 만들기 (클라->서버)
#define dPACKET_PARTY_CREATE_OK			88		// 파티 만들기 OK(서버->클라)
#define dPACKET_PARTY_DESTROY			89		// 파티 뽀개기 (클라->서버)
#define dPACKET_PARTY_DESTROYED			90		// 파티 뽀개기 (서버->클라)
#define dPACKET_PARTY_JOIN				91		// 파티 조인 ( 클라->서버 )
#define dPACKET_PARTY_JOIN_RESULT			92		// 파티 조인 결과 ( 서버->클라 )
#define dPACKET_PARTY_ADDMEMBER			93		// 파티원 추가 (서버->클라)
#define dPACKET_PARTY_OUT				94		// 파티에서 나감 (클라->서버)
#define dPACKET_PARTY_DELMEMBER			95		// 파티원 삭제 (서버->클라)
#define dPACKET_PARTY_REMOVE_MEMBER		96		// 맴버 강퇴 (파티장용) (클라->서버)
#define dPACKET_PARTY_MEMBER_HPMP			97		// 멤버들의 체력 정보 (서버->클라)
#define dPACKET_PARTY_MEMBER_MAPINFO		98		// 멤버들의 맵상의 위치정보(서버->클라)

#define dPACKET_UPDATE_POINT			99		// STR, INT, DEX 변경
#define dPACKET_UPDATE_LEVEL			100		// 통합 레벨 변경

#define dPACKET_SET_ATTACKRANGE			101		// 공격 가능 거리

#define dPACKET_ADD_HERO				102		// 화면에 영웅 추가 
#define dPACKET_REMOVE_HERO				103		// 화면에서 영웅 빼기

#define dPACKET_EQUIP_INFO				104		// 장비 정보

#define dPACKET_MAGIC_INFO				105		// 마법 정보
#define dPACKET_MAGIC_CAST				106		// 마법 사용 (클라이언트->서버)
#define dPACKET_MAGIC_CAST_USER			107		// 마법 사용 (서버->클라이언트)

#define dPACKET_ADD_EFFECT				108		// 마법 이펙트 추가
#define dPACKET_REMOVE_EFFECT			109		// 마법 이펙트 삭제

#define dPACKET_SET_SKILLEXP			110		// 스킬 경험치 셋팅 

#define dPACKET_ITEM_DBCLICK			111		// 아이템 인터페이스 처리

#define dPACKET_ADD_SKILL				112		// 스킬 추가 ( 서버 ->클라이언트 )
#define dPACKET_SKILL_LIST				113		// 스킬 리스트
#define dPACKET_USE_QUICKITEM			114		// 퀵아이템 사용 

#define dPACKET_UPDATE_POTIONCNT			115		// 포션 수량 업데이트	

#define dPACKET_QUEST_LIST				116		// 퀘스트 리스트 (서버->클라이언트)
#define dPACKET_QUEST_REQUEST			117		// 퀘스트 정보 요청 (클라이언트->서버)
#define dPACKET_QUEST_RESULT			118		// 퀘스트 결과(서버->클라이언트)

#define dPACKET_DOOR_EVENT				119		// Indoor 문 처리

#define dPACKET_OPEN_GARBAGE			120		// 창고 열기 (클라->서버)
#define dPACKET_GARBAGE_ITEMLIST			121		// 창고 아이템 리스트(서버->클라)
#define dPACKET_INSERT_TO_GARBAGE			122		// 창고에 아이템 추가(서버->클라)
#define dPACKET_REMOVE_FROM_GARBAGE		123		// 창고에 아이템 빼기(서버->클라)
#define dPACKET_INPUT_GARBAGE_ITEM			124		// 창고에 아이템 넣기(클라->서버)
#define dPACKET_OUTPUT_GARBAGE_ITEM		125		// 창고에 아이템 빼기(클라->서버)

#define dPACKET_RIDE_INFO				126		// 탈것 정보

#define dPAKCET_BEGIN_CASTMAGIC			127		// 마법 시전 시~작~ (클라이언트->서버)
#define dPACKET_BEGIN_CASTMAGIC2			128		// 마법 시전 시작 (서버->클라이언트)

#define dPACKET_NPC_INFO				129		// NPC 정보			(서버->클라이언트)
#define dPACKET_MOB_STATE				130		// NPC 모드 변환	(서버->클라이언트)

#define dPACKET_GHOST_STATE				131		// 유령 상태		(서버->클라이언트)

#define dPACKET_ADD_PARTYBOARD			132		// 파티 게시판 등록 (클라이언트->서버)
#define dPACKET_GET_PARTYBOARD			133		// 파티 게시판 게시물 목록(클라이언트->서버)
#define dPACKET_PARTYBOARD_LIST			134		// 파티 게시판(서버->클라이언트)

#define dPACKET_ATTACK_PLAYER			135		// PK 
#define dPACKET_DAMAGE_PVP				136		// 플레이어 -> 플레이어 공격

#define dPACKET_BROADCAST_CHAT			137		// 전체채팅
#define dPACKET_SELLING_CHAT			138		// 매매채팅
#define dPACKET_GUILD_CHAT				139		// 길드(군단)채팅
#define dPACKET_PARTY_CHAT				140		// 파티채팅
#define dPACKET_MEMBER_CHAT				141		// 단 채팅

#define dPACKET_CHANGE_WEATHER			142		// 날씨 변경
#define dPACKET_SOUND_EFFECT			143		// 사운드 이펙트
#define dPACKET_UPDATE_CHA				144		// 성향 업데이트 
#define dPACKET_ENCHANT_ITEM			145		// 인첸트 아이템 
#define dPACKET_ENCHANT_ITEM_RES			146		// 인첸트 아이템 결과 

#define dPACKET_PARTY_ASKINVITE			147		// 파티 초대
#define dPACKET_PARTY_INVITE_RES			148		// 파티 초대 응답

#define dPACKET_UPDATE_CLASS			149		// 직업 업데이트 

#define dPACKET_SYSTEM_MSG				150		// 시스템 메세지

#define dPACKET_USE_SKILL				151		// 스킬 사용
#define dPACKET_BEGIN_SKILL				152		// 스킬 사용( 서버->클라이언트 )

#define dPACKET_SET_VISIBLE				153		// 케릭터 않보이게 하기

#define dPACKET_SET_MOVABLE				154		// 이동 가능, 불가 

#define dPACKET_CREATE_GUILD			155		// 길드생성
#define dPACKET_DESTROY_GUILD			156		// 길드소멸
#define dPACKET_UPDATE_GUILDINFO			157		// 길드정보

#define dPACKET_ITEM_EVENT				158		// 아이템 이벤트

#define dPACKET_GET_FRIENDLIST			159		// 친구목록
#define dPACKET_FRIEND_ADD				160		// 친구등록
#define dPACKET_FRIEND_ADDED			161		// 친구등록결과
#define dPACKET_FRIEND_REMOVE			162		// 친구삭제
#define dPACKET_FRIEND_REMOVED			163		// 친구삭제결과
#define dPACKET_FRIEND_MEMO				164		// 친구쪽지

#define dPACKET_GUILD_ADDMEMBER_REQUEST		165		// 길드 가입 요청(?)
#define dPACKET_GUILD_ADDMEMBER			166		// 길드원 추가
#define dPACKET_GUILD_DROPMEMBER			167		// 길드원 추방
#define dPACKET_GUILD_QUIT				168		// 길드 탈퇴

#define dPACKET_PET_INFO				169		// 애완동물 정보
#define dPACKET_PET_ON				170		// 애완동물 ON
#define dPACKET_PET_OFF				171		// 애완동물 OFF
#define dPACKET_UPDATE_PET_GAGE			172		// 애완동물 분노게이지
#define dPACKET_UPDATE_PET_INFO			173		// 애완동물 정보 업데이트 
#define dPACKET_PET_ATTACK				174		// 팻 -> 몹 공격 
#define dPACKET_ATTACK_TO_PET			175		// 몹 -> 팻 공격

#define dPACKET_MOVE_DEALMAP			176		// 거래맵으로 이동

#define dPACKET_CAMP_INFO				177		// 진지 정보
#define dPACKET_MAKE_CAMP				178		// 진지 생성

#define dPACKET_FISHING_STATUS          		179		// 낚시 관련 상태	( S -> C )
#define dPACKET_FISHING_PATTERN			180		// 낚시 패턴		( S -> C )
#define dPACKET_FISHING_ACTION			181		// 낚시 관련 행동	( S -> C )
#define dPACKET_FISHING_POS				182		// 낚시에서 케릭터 바라보는 방향

#define dPACKET_PET_ONOFF				183		// 애완동물 켜고 끄기 ( 클라이언트->서버 )
#define dPACKET_PET_CHANGENAME			184		// 애완동물 이름바꾸기 ( 클라이언트->서버 )
#define dPACKET_PET_UPDATENAME			185		// 애완동물 이름 바꾸기 ( 서버->클라이언트 )
#define dPACKET_PET_UPDATEEXP			186		// 애완동물 경험치 바꾸기 (서버->클라)

#define dPACKET_DEAL_OK				187		// 거래 수락 ( 클라이언트->서버 )
#define dPACKET_DEAL_BEGINOK			188		// 거래 시작 ( 서버->클라이엄트 )

#define dPACKET_FRIEND_LOGIN			189		// 친구 로그인
#define dPACKET_FRIEND_LOGOUT			190		// 친구 로그아웃

#define dPACKET_MOB_POPEFFECT			191		// 몬스터 이팩트 출력 
#define dPACKET_PLAYER_POPEFFECT			192		// 플레이어 애픽트 출력

#define dPACKET_CHANGE_STATUS			193		// 케릭터 상태변화	

#define dPACKET_GUILD_CHANGEMASTER			194		// 길드 마스터 권한 위임	
#define dPACKET_GUILD_GETMEMBERLIST		195		// 길드 맴버 리스트 요청(클라이언트->서버)
#define dPACKET_GUILD_MEMBERLIST			196		// 길드 맴버리스트 (서버->클라이언트)

#define dPACKET_GUILD_ADVANCE			197		// 부단장 승진
#define dPACKET_GUILD_DEMOTION			198		// 부단장 강등 

#define dPACKET_MYSHOP_OPEN				199		// 개인상점 오픈 ( 클라이언트 -> 서버 )
#define dPACKET_MYSHOP_OPENRESULT			200		// 개인상점 오픈 결과 ( 서버 -> 클라이언트 )
#define dPACKET_MYSHOP_CHANGETITLE			201		// 개인상점 이름 바꾸기 ( 클라이언트 -> 서버 )
#define dPACKET_MYSHOP_TITLECHANGED		202		// 개인상점 이름 바꾸기 ( 서버 -> 클라이언트 )
#define dPACKET_MYSHOP_ADDITEM			203		// 개인상점 아이템 추가 ( 클라이언트 -> 서버 )
#define dPACKET_MYSHOP_ITEMADDED			204		// 개인상점 아이템 추가 ( 서버->클라이언트 )
#define dPACKET_MYSHOP_REMOVEITEM			205		// 개인상점 아이템 삭제 ( 클라이언트 -> 서버 )
#define dPACKET_MYSHOP_ITEMREMOVED			206		// 개인상점 아이템 삭제 ( 서버 -> 클라이언트 )
#define dPACKET_MYSHOP_SELLBEGIN			207		// 개인상점 판매시작 ( 클라이언트 -> 서버 )
#define dPACKET_MYSHOP_SELLBEGIN_RESULT		208		// 개인상점 판매시작 ( 서버 -> 클라이언트 )
#define dPACKET_MYSHOP_SELLFINISH			209		// 개인상점 판매종료(회수) (클라이언트->서버)
#define dPACKET_MYSHOP_SELLFINISH_OK		210		// 개인상점 판매종료(회수) (서버->클라이언트)
#define dPACKET_MYSHOP_GETITEMLIST			211		// 개인상점 판매목록 ( 클라이언트 -> 서버 )
#define dPACKET_MYSHOP_SENDITEMLIST		212		// 개인상점 판매목록 ( 서버 -> 클라이언트 )
#define dPACKET_MYSHOP_BUYITEM			213		// 개인상점 아이템구입 ( 클라이언트 -> 서버 )
#define dPACKET_MYSHOP_UPDATEINFO			214		// 개인상점 정보 업데이트 ( 서버->클라이언트 )
#define dPACKET_MYSHOP_REMOVELISTITEM		215		// 개인상점 아이템 삭제(구입하는 사람입장 ( 서버->클라이언트 ) 
#define dPACKET_MYSHOP_UPDATECASH			216		// 개인상점 판매대금 업데이트( 서버->클라이언트 )

#define dPACKET_WORLD_CHAT				217		// 세계채널 채팅
#define dPACKET_INSERT_OBJMOB			218		// 오브젝트 몬스터 추가 ( 성문, 성물 )
#define dPACKET_UPDATE_WEIGHTGAGE			219		// 무게게이지
#define dPACKET_CHANGE_NAMECOLOR			220		// 이름색 변경

#define dPACKET_UPDATE_WARCAMP			221		// 진지정보 업데이트
#define dPACKET_UPDATE_WARINFO			222		// 공성관련 정보 업데이트

#define dPACKET_UPDATE_GARBAGEMONEY		223		// 창고돈 업데이트
#define dPACKET_RECEIPT_MONEY			224		// 창고에 돈 넣기
#define dPACKET_WITHDRAW_MONEY			225		// 창고에서 돈 찾기

#define dPACKET_SET_PVP_MODE			226		// PvP 모드 변환      (클라이언트->서버)
#define dPACKET_UPDATE_PVP_MODE			227		// PvP 모드 업데이트  (서버->클라이언트)

#define dPACKET_GET_CASTLELIST			228		// 성 목록 
#define dAPCKET_SEND_CASTLELIST			229		// 성 목록 보내기
#define dPACKET_GET_WARSTATE			230		// 전쟁상황도의 전쟁정보요청 (클라이언트->서버)
#define dPACKET_SEND_WARSTATE			231		// 전쟁상황도 정보 (서버->클라이언트 )

#define dPACKET_GET_TAXINFO				232		// 현재 세율 얻기 (클라이언트->서버)
#define dPACKET_OPEN_TAXWINDOW			233		// 세율 조정 인터페이스 열기 (서버->클라이언트) 
#define dPACKET_CHANGE_TAX				234		// 세율 조정(클라이언트->서버)
#define dPACKET_UPDATE_TAX				235		// 현재 세율 업데이트(서버->클라이언트)

// 유료 
#define dPACKET_ENCHANT_CHANGE			236		// 인챈트 채인지( 유료화 아이템 사용 ) (클라->서버)
#define dPACKET_EHCNAHT_COLLECTION			237		// 인첸트 콜렉션( 유료화 아이템 사용 ) (클라->서버)
#define dPACKET_ITEMLOOK				238		// 아이템 룩 ( 유료화 아이템 사용 ) ( 클라->서버 )
#define dPACKET_LEVELLOOK				239		// 레벨 룩 ( 유료화 아이템 사용 ) ( 클라->서버 )

#define dPACKET_GET_BILLING_ITEMS			240		// 유료아이템 목록 요청 (클라->서버)
#define dPACKET_BILLING_ITEM_INFO			241		// 유료아이템 목록 (서버->클라)
#define dPACKET_REMOVE_BILLING_ITEM		242		// 유료아이템 유료인터페이스에서 삭제 (서버->클라)

#define dPACKET_USE_BILLITEM			243		// 유료 아이템 사용 ( 클라이언트 -> 서버 )

#define dPACKET_MAP_SHOUT				244		// 맵 전체 외침

#define dPACKET_ENEMY_DIVISION			245		// 피아구분

#define dPACKET_SHOW_ITEMTIMER			246		// 아이템 타이머 돌기
#define dPACKET_SELCHAR_SYSTEMMSG			247		// 시스템 메시지 (캐릭터 셀렉트용)

#define dPACKET_SMITH_REQUEST			248		// 대장장이 요청
#define dPACKET_SMITH_UPDATE			249		// 대장장이 업데이트
#define dPACKET_SMITH_QUIT				250		// 대장장이 종료
#define dPACKET_SMITH_CANCEL			251		// 대장장이 취소
#define dPACKET_SMITH_ADDITEM			252		// 대장장이 아이템  추가 ( 클라이언트 -> 서버 )
#define dPACKET_SMITH_DELITEM			253		// 대장장이 아이템 삭제  ( 클라이언트 -> 서버 )
#define dPACKET_SMITH_ITEMADDED			254		// 대장장이에게 맡길 아이템 추가 ( 서버 -> 클라이언트 )255//
#define dPACKET_SMITH_ITEMDELED			255		// 대장장이 아이템 삭제 ( 서버 -> 클라이언트 )
#define dPACKET_UPDATE_CARDCNT			256
#define dPACKET_CARDUNDO				257

#define dPACKET_FRIEND_LOGIN_REQUEST		258		// 친구로그인메시지
#define dPACKET_FRIEND_LOGOUT_REQUEST		259		// 친구 로그아웃 리퀘스트
#define dPACKET_FRIEND_MSG				260		// 친구 로그인 리퀘스트//

#define dPACKET_UPDATE_CASH_MONEY			261		// 캐쉬금액 업데이트

#define dPACKET_FRIEND_ASKYESNO			262
#define dPACKET_FRIEND_JOIN_YES			263
#define dPACKET_FRIEND_JOIN_NO			264
//#define dPACKET_IP_CHECK				265

#define dMAX_TAG					265

#ifdef __CHINA_VER__
#define dUSE_SALE_MONEY				2000		// 매매채널 사용시 필요한 금액(월드로 변경)
#endif

//
BOOL RecvFromClient( sPDESC_DATA pClient );
BOOL ReadRecvBuff( sPDESC_DATA pClient );
BOOL LoadWarPlayer( sPDESC_DATA pPlayer );

void SendWhisperMsg( sPDESC_DATA pPlayer,  char *name, char *msg );
void UserIPCheck( sPDESC_DATA pPlayer );
BOOL LoadIPTable();

//
void StrSendSysMsg( sPDESC_DATA pPlayer, int line, char *sz1 );
void StrSendSysMsg( sPDESC_DATA pPlayer, int line, char *sz1, int n1 );
void StrSendSysMsg( sPDESC_DATA pPlayer, int line, char *sz1, char *sz2 );
void StrSendSysMsg( sPDESC_DATA pPlayer, int line, char *sz1, char *sz2, int n1, int n2 );
void StrSendSysMsg( sPDESC_DATA pPlayer, int line, char *sz1, char *sz2, char *sz3s, ... );
void StrSendSysMsg( sPDESC_DATA pPlayer, int line, char *sz1, char *sz2, char *sz3s, int n1, int n2, int n3 );
#endif 


//


