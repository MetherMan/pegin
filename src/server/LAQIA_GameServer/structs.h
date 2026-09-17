/**
 *	$Workfile: structs.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 58 $
 *	$Date: 04-12-31 12:08p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/structs.h $
 * 
 * 58    04-12-31 12:08p Sk8snow
 * 
 * 57    04-12-15 10:40a Sk8snow
 * 
 * 56    04-11-18 1:40a Sk8snow
 * 
 * 55    04-10-19 11:14p Sk8snow
 * 
 * 54    04-09-20 10:53p Sk8snow
 * 
 * 53    04-09-11 11:20p Sk8snow
 * 
 * 52    04-09-08 3:54p Sk8snow
 * 
 * 51    04-08-04 9:27a Sk8snow
 * 
 * 50    04-07-13 7:26a Sk8snow
 * 
 * 49    04-05-29 10:12p Sk8snow
 * 
 * 48    04-05-21 11:37a Sk8snow
 * 
 * 47    04-05-18 1:17a Sk8snow
 * 
 * 46    04-05-11 1:45a Sk8snow
 * 
 * 45    04-04-30 9:46a Sk8snow
 * 
 * 44    04-04-07 10:17p Sk8snow
 * 
 * 43    04-04-03 4:03p Sk8snow
 * 
 * 42    04-04-02 7:03a Sk8snow
 * sUSER_PET, *sPUSER_PET; 추가
 * 
 * 41    04-03-31 9:36a Sk8snow
 * sWAR_CRYSTAL, sPWAR_CRYSTAL 추가
 * 
 * 40    04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __STRUCT_H__
#define __STRUCT_H__ 

#ifdef WIN32
#pragma warning( disable : 4018 )			// signed, unsigned missmatch disable
#endif

#define dUSE_AUTO_SHOP

#define dPLAYER					0
#define dMOB						1
#define dNPC						2

////////////////////////////////////////////////////
// D  E  F  I  N  E  S
////////////////////////////////////////////////////
#define dMAX_SOCK_BUFF				4096
#define dMAX_SEND_SOCKBUFF				(dMAX_SOCK_BUFF*30)
#define dMAX_DATA_SOCK_BUFF				(dMAX_SOCK_BUFF*1000)

#define dMAX_QUERY_SIZE				4096
#define dNAME_LEN					50
#define dID_LEN					50
#define dPW_LEN					50

#define dMAX_SPELL					12

#define dUSE_NONAIMOB					// Non AI 분류 사용
#define dUSE_CHATORDERING					// 채팅 메시지를 통한 명령사용

#define dMAX_MONSTER					5
#define dMAX_MOBINFO					2048	// 최대 몹 정보

#define dMAX_QUEST					64	// 최대 퀘스트 
#define dMAX_QUESTINFO				1024	// 최대 퀘스트 정보

#define dMAX_HAVE_MONEY				2100000000

/////////////////////////////////////////////////////
// WORLD 
/////////////////////////////////////////////////////
#define dMAX_MAP_CNT					1700	// 최대 맵
#define dONE_MAP_SIZE				500

/////////////////////////////////////////////////////
// for name hash
/////////////////////////////////////////////////////
#define dHASHKEY_STR_ETC				512//256
#define dHASHKEY_STR_MAX				(dHASHKEY_STR_ETC+1)

///////////////////////////////////////////////////
// P L A Y E R   S T A T E   D E F I N E S
///////////////////////////////////////////////////
#define dDESC_STATE_LOGIN				0
#define dDESC_STATE_CHARSELECT			1
#define dDESC_STATE_MAKECHARACTER			2
#define dDESC_STATE_LOADING				3
#define dDESC_STATE_PLAYING				4
#define dDESC_STATE_FIGHT				5
#define dDESC_STATE_DISCON				6

#define dMAX_PLAYER					10000

////////////////////////////////////////////////////
// F O R   S K I L L 
////////////////////////////////////////////////////
#define dSKILL_SHORT_ATTACK				0		// 근거리 공격 
#define dSKILL_LONG_ATTACK				1		// 원거리 공격
#define dSKILL_BLACK_MAGIC				2		// 흑마법
#define dSKILL_WHITE_MAGIC				3		// 백마법

#define dMAX_PLAYER_SKILL				4
#define dMAX_CHAR_POINT				5

#define dMAX_EQUIP					12		// 장착 갯수

#define dCH_EQ_HEAD					0
#define dCH_EQ_JACKET				1
#define dCH_EQ_PANTS					2
#define dCH_EQ_FOOT					3
#define dCH_EQ_HAND					4
#define dCH_EQ_MAX					5

#define dMAX_EFFECT					30
#define dMAX_STATUS_DELAY				30

#define dMAX_SAVE_MOB_EXP				5

#define dMOB_DROPITEM_MAX_LINE			14000	//드랍아이템라인 
#define dMOB_DROMITEM_MAX_MOB			500	//드롭아이템몹수

#define dMAX_EXP_PROTECT				400+1

////////////////////////////////////////////////////
//  M  A  G  I  C     D  A  T  A
////////////////////////////////////////////////////
typedef struct
{
	WORD magicNum;					// 마법번호
	WORD minLevel;					// 최소레벨 
	WORD maxLevel;					// 최대레벨
	WORD minDamage;					// 최소데미지
	WORD maxDamage;					// 최대데미지
	int	 minNeedMp;					// 최소필요MP
	int	 maxNeedMp;					// 최대필요MP
	WORD range;						// 시전거리
	BYTE type;						// 마법형식
	BYTE victType;					// 시전대상
	BYTE exType;						// 추가형식 
} sMAGIC_DATA, *sPMAGIC_DATA;

typedef struct 
{
	char id[50];
	char pw[50];
} sIDPW;


////////////////////////////////////////////////////
// for Client Map Data ( class Vector3 converting )
////////////////////////////////////////////////////
struct Vector3
{
	float x, y, z;
};


////////////////////////////////////////////////////
// I T E M    D E F A U L T    I N F O    D A T A
////////////////////////////////////////////////////
typedef struct
{
	WORD itemNum;			// 아이템번호 
	BYTE isUniq;			// 유니크 
	char hName[64];		// 한글이름 
	char eName[64];		// 영문이름
	BYTE itemType;		// 아이템 종류
	WORD itemLevel;		// 아이템 레벨 
	WORD minDamage;		// 최소 공격력
	WORD maxDamage;		// 최대 공격력 
	WORD minMana;			// 최소 마법력
	WORD maxMana;			// 최대 마법력

	BYTE slotCnt;			// 아이템슬롯갯수
	BYTE slotDice;		// 아이템슬롯확률
	int needClass;		// 착용직업 

	BYTE attackRange;		// 공격 범위 
	WORD durability;		// 내구력 
	BYTE attackSpeed;		// 공격 속도 
	int itemAttr;			// 아이템 속성
	int itemDispos;		// 아이템 성향 
	
	WORD weight;			// 아이템 무게 

	WORD needStr;			// 필요힘 
	WORD needInt;			// 필요지혜
	WORD needDex;			// 필요민첩
	WORD needCha;			// 필요카리스마

	int pixPrice;			// 수리 가격 
	int sellPrice;		// 판매 가격 
	int Price;			// 구입 가격 
	WORD pluseStr;		// 힘+
	WORD pluseInt;		// 지혜+
	WORD pluseDex;		// 민첩+
	WORD pluseCha;		// 카리스마+
	WORD pluseHP;			// 체력+
	WORD pluseMP;			// 마나+
	
	WORD pluseAttackDmg;		// 물리데미지+
	WORD pluseMagicDmg;		// 마법데미지+
	WORD pluseDefense;		// 방어력+
	WORD pluseMagicDefense;	// 마법저항+

	WORD pluseSAttack;		// 근거리
	WORD pluseLAttack;		// 원거리
	WORD pluseBMagic;		// 흑마법
	WORD pluseWMagic;		// 백마법 
	WORD minusStr;		// 힘-
	WORD minusInt;		// 지혜-
	WORD minusDex;		// 민첩-
	WORD minusCha;		// 카리스마-
	WORD minusHP;			// 체력-
	WORD minusMP;			// 마나-

	WORD minusAttackDmg;		// 물리데미지
	WORD minusMagicDmg;		// 마법데미지
	WORD minusDefense;		// 방어력
	WORD minusMagicDefense;	// 마법저항

	WORD minusSAttack;		// 근거리-
	WORD minusLAttack;		// 원거리-
	WORD minusBMagic;		// 흑마법-
	WORD minusWMagic;		// 백마법-

	WORD skillIdx;		// skillIdx

	BYTE needItemLog;		// 로그 여부

	BYTE isBillingItem;		// 유료아이템 여부
} sITEM_DEF_INFO, *sPITEM_DEF_INFO;


////////////////////////////////////////////////////
// I T E M     D A T A
////////////////////////////////////////////////////
#define dMAX_ITEM_EXVALUE				10
#define dITEM_SERIAL_LEN				25

struct bill_item_data
{
	int		idxNum;
	int		itemNum;

	char	PurKey[21];
	int		ProductNum;

	int		cnt;

	struct bill_item_data *prev;
	struct bill_item_data *next;
};

typedef struct bill_item_data sBILL_ITEM_DATA, *sPBILL_ITEM_DATA;

struct item_data
{
	int idxNum;					// 아이템 고유번호
	char serial[dITEM_SERIAL_LEN];		// 시리얼 번호	

	WORD	itemNum;

	int		life;
	int		life_in_map;
		
	int mapNum, mapX, mapY;		
	BYTE mapKind;
	int equipWhere;	
	BYTE isMobDrop;

	int exVal[3];

	BYTE invenX, invenY, invenPage;

	char	ownerName[dNAME_LEN];			// 소유권자 이름
	char	masterName[dNAME_LEN];			// 길드 마스터 이름
	DWORD	ownerCheckTick;				// 소유권 시간


	struct item_data *prev;
	struct item_data *next;

	// Inventory 처리용
	struct item_data *i_next;
	struct item_data *i_prev;

	// 맵 리스트 처리용 next
	struct item_data *w_prev;			
	struct item_data *w_next;
};

typedef struct item_data sITEM_DATA, *sPITEM_DATA;


////////////////////////////////////////////////////
// C H A R A C T E R    D A T A
////////////////////////////////////////////////////
struct character_data
{
	char	name[dNAME_LEN];				// 케릭터 이름 
	char	id[dID_LEN];					// ID

	BYTE	charClass;					// 직업

	WORD	headStyle;					// 머리 스타일 
	WORD	faceStyle;					// 얼굴 스타일

	WORD	chEquip[dCH_EQ_MAX];				// 장착 아이템 번호

	BYTE	sex;						// 성별

	WORD	mapNum;					// 맵번호
	int		posX, posY;				// 좌표
	BYTE	posLayer;					// 맵 레이어 번호
	BYTE	posMapKind;					// 맵 종류

	WORD	prevMapNum;
	int		prevPosX, prevPosY;

	int		hp, max_hp;				// 체력
	int		mp, max_mp;				// 마나력

	int		money;					// 소유 금액
	int		cash;					// 소유 캐쉬

	WORD	skill[dMAX_PLAYER_SKILL];			// 4 개의 스킬 레벨 
	int		skill_exp[dMAX_PLAYER_SKILL];	// 4 개의 스킬 경험치
	
	WORD	str;						// 힘
	WORD	str_add;					// 힘 +
	int		str_exp;				// 힘 경험치
	WORD	intelli;					// 지력
	WORD	intelli_add;					// 지력 +
	int		intelli_exp;				// 지력 경험치
	WORD	dex;						// 민첩
	WORD	dex_add;					// 민첩 +
	int		dex_exp;				// 민첩 경험치

	BYTE	charPos;					// 몇번째 케릭터?

	BYTE	mercType;					// 용병 타입
	int		mercLevel;				// 용병 레벨
	int		mercExp;				// 용병 경험치

	int		cha;					// 카리스마
	
	int		guildNo;				// 길드번호 
	int		guildLevel;				// 길드 레벨
	int		tmpGuildNum;				// 용병으로 소속된 길드
	
	WORD	useSkill;					// 사용 기술
	BYTE	isNewCharacter;				// 신규 케릭터 ?
};

typedef struct character_data sCHAR_DATA, *sPCHAR_DATA;


////////////////////////////////////////////////////
// P A R T Y  D A T A
////////////////////////////////////////////////////
#define dMAX_PARTY_MEMBER		   7
typedef struct
{
	struct descriptor_data *master;
	struct descriptor_data *member[dMAX_PARTY_MEMBER];
	BYTE memberCnt;
} sPARTY_DATA, *sPPARTY_DATA;

// 

struct __user_skill_data
{
	WORD	skillNum;
	struct __user_skill_data *prev;
	struct __user_skill_data *next;
};

typedef struct __user_skill_data *sUSER_SKILL, *sPUSER_SKILL;

#define dPET_NAME_LEN		50

typedef struct 
{
	BYTE type;
	BYTE level;
	WORD hp;
	WORD mp;
	int exp;
	BYTE state;
	WORD angerGage;
	char name[dPET_NAME_LEN];
} sUSER_PET, *sPUSER_PET;


// 친구용 데이터
struct __friend_data
{
	char id[dID_LEN];
	char name[dNAME_LEN];

	BYTE charPos;
	BYTE isOnline;
	
	struct __friend_data *prev;
	struct __friend_data *next;
};

typedef struct __friend_data sFRIEND_DATA, *sPFRIEND_DATA;


// 개인상점용
struct __myshop_item
{
	int			idxNum;
	int			itemNum;
	int			price;
	BYTE		cnt;

	struct __myshop_item *prev;
	struct __myshop_item *next;
};

typedef struct __myshop_item sMYSHOP_ITEM, *sPMYSHOP_ITEM; 


#define dMAX_MYSHOP_NAME_LEN		128

typedef struct 
{
	char					myShopName[dMAX_MYSHOP_NAME_LEN];
	BYTE					isMyShopOpen;
	sPMYSHOP_ITEM			myShopSellList;
	int						myShopCashBox;
} sMYSHOP, *sPMYSHOP;

#define dSPESKILL_DELAY_MAX			64					// 특화스킬 딜레이 
#define dMAX_BILLING_FLAG			128					// 빌링 속성 
#define dMAX_COOL_TIME			128					// 쿨타임 갯수

// 이펙트 종류 
#define dPLAYER_EFFECT_TYPE_NONE				0
#define dPLAYER_EFFECT_TYPE_ADDDAMAGE			1			// 데미지 증가 
#define dPLAYER_EFFECT_TYPE_ADDARMOR			2			// 방어력 증가
#define dPLAYER_EFFECT_TYPE_HPADD				3			// HP 증가
#define dPLAYER_EFFECT_TYPE_MPADD				4			// MP 증가
#define dPLAYER_EFFECT_TYPE_STRADD				5			// STR 증가
#define dPLAYER_EFFECT_TYPE_DEXADD				6			// DEX 증가 
#define dPLAYER_EFFECT_TYPE_INTADD				7			// INT 증가 
#define dPLAYER_EFFECT_TYPE_ATTACKSPEED			8			// 공격속도증가
#define dPLAYER_EFFECT_TYPE_MOVESPEED			9			// 이동속도증가
#define dPLAYER_EFFECT_TYPE_ETC				10			// 기타

#define dEFFECT_VALUE_TYPE_NONE				0
#define dEFFECT_VALUE_TYPE_PERCENT				1			// 퍼센트
#define dEFFECT_VALUE_TYPE_POINT				2			// 고정값

#define dMAX_PLAYER_EFFECT					64

typedef struct 
{
	int		skillNum;				// 스킬 아이템 번호
	BYTE	effectType;					// 이펙트 종류 
	DWORD	delay;						// 지속 시간

	BYTE	valueType;					// 적용값 종류
	int		value;					// 적용 값 
} sPLAYER_EFFECT, *sPPLAYER_EFFECT;


typedef struct 
{
	int			skillNum;
	int			targetIdx;
	BYTE		targetType;
} sWAIT_SKILL, sPWAIT_SKILL;

////////////////////////////////////////////////////
// C H A R A C T E R    D A T A II
////////////////////////////////////////////////////
struct character_data_sub
{
	WORD	gotoMapNum;						// 로딩할 맵번호
	int		gotoPosX, gotoPosY;				// 로딩할 좌표 
	BYTE	gotoPosMapKind;					// 로딩할 맵의 종류

	DWORD	actionDelay;						// 행동 관련 통합 딜레이
	WORD	lastCastMagic;					// 마지막 시전한 마법

	BYTE	endMapLoading;

	sPARTY_DATA party;						// 파티

	// For Deal
	struct descriptor_data *dealVict;				// 아이템 교환 대상
	CList dealItems;						// 거래아이템들
	int dealMoney;						// 거래 금액
	BYTE dealCheckIn;						// 거래 아이템 처리 완료
	BYTE dealFinish;						// 거래 종료
	// For Deal End 

	WORD		effectIdx;
	
	DWORD status_delay[dMAX_STATUS_DELAY];			// 상태관련 딜레이
	DWORD speSkillDelay[dSPESKILL_DELAY_MAX];			// 특화스킬 딜레이 ( 추가시 max 값 체크 )

	int	flag;							// 케릭터 플레그
	BYTE					isVisible;	

	struct monster_data		*sMob;
	BYTE					sMobLife;

	// 운영자용
	BYTE					expJump;

#ifdef dUSE_AUTO_SHOP
	WORD					currOpenShop;
#endif

#ifdef dUSE_DEAD_GHOST
	BYTE					isDeadGhost;
#endif
	
	BYTE					questState[dMAX_QUEST];

	WORD					RideNum;			// 타고 있는것의 번호
	BYTE					isRide;			// 무언가를 타고 있다

	DWORD					ghostTimer;

	int						partyExp;		// 파티 EXP

	// ADD
	WORD					addHP;				// HP Add
	WORD					addMP;				// MP Add
	WORD					addStr;			// Str Add
	WORD					addInt;			// Int Add
	WORD					addDex;			// Dex Add
	WORD					addSAttack;			// SAttack Add
	WORD					addLAttack;			// LAttack Add
	WORD					addBMagic;			// BMagic Add
	WORD					addWMagic;			// WMagic Add
	
	// End ADD

	sPFRIEND_DATA			friendList;
	BYTE					friendCnt;

	sPUSER_SKILL			haveSkill;

	BYTE					btGuildKickOut;
	BYTE					btFirstLoad;

	sMYSHOP					myShop;

	DWORD					worldChatDelay;
	DWORD					sellingChatDelay;

	BYTE					isEvilCha;		// 보라돌이
	DWORD					evilChaTime;		// 보라돌이 유지시간

	BYTE					btPkMode;	

#ifndef dNOT_USE_PET
	sUSER_PET				pet;
#endif

	DWORD					billingFlag[dMAX_BILLING_FLAG];
	DWORD					billingAddValue[dMAX_BILLING_FLAG];

	BYTE					haveBillRow;
	DWORD					checkBillItemTime;
	DWORD					lastSaveBillInfoTime;

	DWORD					skillCoolTime[dMAX_COOL_TIME];

	sPLAYER_EFFECT			skillEffect[dMAX_PLAYER_EFFECT];

	sWAIT_SKILL				waitSkill;

	BYTE					EnchantNoFail;
	DWORD					guildSummonTime;

	int						MaxLevel;

#ifdef _dUMENOKOJI_GHOST_GUILD_		//유령 길드 관련 전쟁포인트 테스트
	DWORD					getTime;   //1시간당 캐릭에 전쟁포인트를 넣기 위한 변수 유령길드 관련
#endif

};

typedef struct character_data_sub sCHAR_DATA_SUB, *sPCHAR_DATA_SUB;


////////////////////////////////////////////////////
// M O B   R E S E T   I N F O 
////////////////////////////////////////////////////
struct s_mob_reset
{
	int mobNum;
	int mapNum;
	
	int origPosX;
	int origPosY;

	int posX;
	int posY;
	BYTE moveDir;

	DWORD moveStartTime;

	int	currCnt;

	struct s_mob_reset *next;
	struct s_mob_reset *prev;
};

typedef struct s_mob_reset sMOB_RESET, *sPMOB_RESET;


////////////////////////////////////////////////////////
// 몹 경험치 저장용
////////////////////////////////////////////////////////
typedef struct
{
	int expDamage[4];
	
	struct descriptor_data *pPlayer;
} sSAVE_EXP, *sPSAVE_EXP;

#define dMAX_MOB_EFFECT						12

////////////////////////////////////////////////////
// M O N S T E R    D A T A
////////////////////////////////////////////////////
struct monster_data
{
	int		idxNum;
	int		mobNum;
	int		mapNum;			// 맵 번호

	int		hp;				// 체력, 최대체력
	int		mp;				// 마나력, 최대 마나력
	int		posX, posY;			// x , y 
	int		destPosX, destPosY;		// 목표 좌표

	BYTE	moveDir;				// 이동 방향
	
	BYTE	state;					// 몹 상태

	WORD	move_speed;				// 이동속도 
	WORD	move_speed2;				// 이동속도2
	BYTE	move_range;				// 이동거리
	BYTE	move_range2;				// 이동거리2
	DWORD	lastMoveTime;				// 최후 이동 시간
	DWORD	lastAttackTime;			// 최후 공격 시간
	WORD	attackDelay;				// 공격 딜레이
	DWORD	AICheckTIme;				// AI 체크 시간

	BYTE	blockCnt;					// 

	BYTE	isDead;

	WORD	effectIdx;

#ifdef dUSE_NONAIMOB
	BYTE	isAIMob;				// AI 몬스터 인지
	BYTE	AiTick;				// AI Tick
#endif

	sPMOB_RESET pCreater;			// 몬스터 자동 생성자

	sSAVE_EXP	saveExp[dMAX_SAVE_MOB_EXP];	// 경험치 저장용

	DWORD		AttrEffectTime[dMAX_MOB_EFFECT];

	BYTE		isObject;			// 오브젝트몹 ( 성문, 성물 )
	int			objAngle;		// 표시각

	DWORD		deadTimer;			// 죽을 시간

	int			nHit;

	// 전체 AI 리스트 , 전투 리스트용 ( 공용 )
	struct monster_data *prev;
	struct monster_data *next;

	struct descriptor_data	*enemy;			// 따라갈 적

	// 섹터 리스트 용 
	struct monster_data *w_prev;
	struct monster_data *w_next;
};

typedef struct monster_data sMOB_DATA, *sPMOB_DATA;

struct __mob_inven
{
	int itemNum;
	
	int percent;

	struct __mob_inven *prev;
	struct __mob_inven *next;
};

typedef struct __mob_inven sMOB_INVEN, *sPMOB_INVEN;

////////////////////////////////////////////////////
// M O B   D E F A U L T   I N F O   D A T A
////////////////////////////////////////////////////
typedef struct
{
	WORD mobNum;						// 몹번호 

	char hName[64];					// 한글이름
	char eName[64];					// 영문이름 
	
	WORD level;						// 레벨 
	
	int hp;						// 체력
	int mp;						// 마나
	
	WORD minDamage;					// 최소공격력
	WORD maxDamage;					// 최대공격력
	WORD dp;						// 방어력 

//#ifdef _dUMENOKOJI_					//마방관련
	WORD	minMagicDamage;				// 마법 최소 공격력
	WORD	maxMagicDamage;				// 마법 최대 공격력
	WORD	Magicdp;					// 마법 방어력 
//#endif
	
	WORD exp;						// 경험치 
	WORD bonusExp;					// 보너스 경험치 

	int money;						// 돈 

	WORD dropPercent;					// 전리확률 
	WORD dropLevelLimit;					// 전리Level제한 

	BYTE attackRange;					// 공격거리 
	BYTE defenseRange;					// 방어 범위 
	BYTE helpRange;					// 원호 범위 
	BYTE moveRange;					// 이동 범위 
	
	int attackAttr;					// 공격 성향 
	int helpAttr;						// 원호 성향 
	
	int attackType;					// 공격 방법
	
	int waitDelay;					// 대기 딜레이 
	
	int moveSpeed;					// 이동 속도
	int traceSpeed;					// 추적 속도
	int attackSpeed;					// 공격 속도
	
	int resetTimer;					// 리셋 주기 
	
	WORD minCnt;						// 최소 개체수 
	
	int mobAttr;						// 몬스터 속성
		
	WORD minTurn;						// 최소 턴
	WORD maxTurn;						// 최대 턴 
	
	int speAttackType;					// 특수공격종류 
	int speAttackLevel;					// 특수공격레벨 
	
	WORD createEffect;					// 생성 Effect 종류 
	WORD deadEffect;					// 죽음 Effect 종류 

	WORD mobAttackAttr;					// 몹의 공격 속성

	sPMOB_INVEN	inven;
	int		invenCnt;
	sPMOB_INVEN	currDropItem;

} sMOB_DEF_INFO, *sPMOB_DEF_INFO;

//#ifdef _dUMENOKOJI_QUEST_					//퀘스트관련
////////////////////////////////////////////////////////////////////
// Q U E S T	D E F A U L T   I N F O   D A T A
////////////////////////////////////////////////////////////////////
struct quest_data{
	int		questNum;				//퀘스트 번호
	int		questStartMapNum;			//퀘스트 시작맵번호
	int		questStartNpcNum;			//퀘스트 시작 NPC번호
	int		questAllLvLimit;			//모든 레벨 제한
	int		questShortAttackLimit;		//근거리 제한
	int		questLognAttackLimit;		//원거리 제한
	int		questBMagicLimit;			//흑마법 제한
	int		questWMagicLimit;			//백마법 제한
	int		questTimeLimit;			//시간제한
	int		questNeedPeople;			//필요인원
	int		questNeedObject;			//시작 아이템 번호
	int		questLoop;				//퀘스트 반복회수 0 = 반복없음, 1 = 1번, 2 = 2번 .....
	int		questPrequest;			//선결퀘스트
	int		questOverlap;				//중복가능 퀘스트
	int		questItemNum;				//수집할 아이템 번호
	int		questMobNum;				//잡을 몬스터번호
	int		questNeedItemMobCnt;			//필요한 아이템 / 몬스터 개수
	int		questEndNpcMap;			//종료NPC맵
	int		questEndNpcNum;			//종료NPC번호
	int		questExpCompen;			//exp보상
	int		questMoneyCompen;			//실드 보상
	int		questItemCompen;			//아이템 보상
	
	struct quest_data	*next;
	struct quest_data	*prev;
};

typedef struct quest_data sQUEST_DEF_INFO, *sPQUEST_DEF_INFO;



/////////////////////////////////////////////////
// for Player data flushing to DataServer
/////////////////////////////////////////////////
#define dDATA_UPDATE_SKILLEXP1		0
#define dDATA_UPDATE_SKILLEXP2		1
#define dDATA_UPDATE_SKILLEXP3		2
#define dDATA_UPDATE_SKILLEXP4		3
#define dDATA_UPDATE_STREXP			4
#define dDATA_UPDATE_DEXEXP			5
#define dDATA_UPDATE_INTEXP			6
#define dDATA_UPDATE_CHA			7

#define dMAX_DATA_UPDATE			8

#define dINVEN_PAGE					2  //인벤페이지 
#define dINVEN_X					5
#define dINVEN_Y					4


////////////////////////////////////////////////////
// D E S C R I P T O R    D A T A
////////////////////////////////////////////////////
struct descriptor_data
{
	////////////////////////////////////////////
	// 데이터 전송 처리용 
	////////////////////////////////////////////
	SOCKET		sock;

	char	ip[20];

	//char recvBuff[dMAX_SOCK_BUFF];				// recv 데이터 버퍼 
	char recvBuff[40960];					// recv 데이터 버퍼 
	int recvSize;							// recv 버퍼 상의 총 데이터 량
	int	recvPos;						
	char TemprecvBuff[40960];					// recv 데이터 버퍼 

	char sendBuff[dMAX_SEND_SOCKBUFF];//[40960];[dMAX_SEND_SOCKBUFF];				// send 대기중인 데이터 
	int sendSize;								// send 할 데이터량

	int	status;							// 상태

	struct character_data ch;						// 케릭터 데이터
	struct character_data_sub ch2;					// 저장할 필요 없는 케릭터 데이터들

	struct item_data *inven;						// 소지품 데이터
	struct item_data *garbage;						// 창고

	BYTE   useGarbageMoney;						//
	BYTE   useCashMoney;							//
	int	   garbageMoney;						// 창고에 보관중인 돈
	int	   invenIdx;							// 소지품&장비 인덱스
	int	   garbageCnt;					

	//대장장이 시스템
	struct item_data *smith;						// 대장장이 아이템
	int	   smithCnt;							// 대장장이 아이템 카운터

	struct item_data *equip[dMAX_EQUIP];				// 장비 장착 데이터

	struct bill_item_data *billItems;					// 빌링 아이템들

	WORD quickSlot[2][8];						// 퀵슬롯
	BYTE invenArray[dINVEN_PAGE][dINVEN_Y][dINVEN_X];		// 인벤토리 처리 ( 2페이지, 5*5 ) 
	int	 currItemWeight;						// 현재 아이템 총 무게

	BYTE isUpdate[dMAX_DATA_UPDATE];					// 데이터 저장 체크
	DWORD lastDataUpdateTime;						// 데이터 업데이트 타임
	
	struct monster_data *enemyMob;					// 공격 대상
	struct descriptor_data *victPlayer;				// 공격 대상 (PVP)

	int		nPartyDamage;						// 파티사냥시 몬스터에게 준 데미지

	////////////////////////////////////////////
	// 데이터 위치 확인용
	////////////////////////////////////////////
	BYTE isWaitAllData;
	BYTE isWaitCharData;
	BYTE isWaitDelData;
	BYTE isDead;
	BYTE isNewCharacter;
	BYTE isInNameList;
	BYTE isInIdList;
	BYTE disconnectFlag;

	BYTE isAccepted;

	BYTE chDataLoaded;

#ifdef _DUMMY_MODE_
	BYTE isDummy;
	DWORD dummyMoveTime;
#endif

	////////////////////////////////////////////
	// 연결 상태 확인용
	////////////////////////////////////////////
	DWORD		lastRecvTime;
	DWORD		lastAliveCheck;
	BYTE		isSpeedHack;

	DWORD		aliveCheckTick;
	BYTE		aliveCnt;
	
	BYTE		isAdmin;
	BYTE		adminLevel;
	BYTE		isGodCharacter;

	DWORD		chatEndTime;
	BYTE		badUserType;

	BYTE		selectGuildLevel[3];

	////////////////////////////////////////////
	// Linked List Data
	////////////////////////////////////////////
	// 전체 리스트용 prev, next
	struct descriptor_data *prev;		
	struct descriptor_data *next;

	// Name List용
	struct descriptor_data *name_prev;
	struct descriptor_data *name_next;

	// ID List용 
	struct descriptor_data *id_prev;
	struct descriptor_data *id_next;

	// map 처리용 prev, next
	struct descriptor_data *w_prev;
	struct descriptor_data *w_next;

	// 전체체팅용
	struct descriptor_data *c_prev;
	struct descriptor_data *c_next;
	
	// 길드용 prev, next
	struct descriptor_data *guild_prev;	
	struct descriptor_data *guild_next;

	// 용병용 
	struct descriptor_data *merc_prev;
	struct descriptor_data *merc_next;
	
	// 낚시용
	struct descriptor_data *fishing_prev;
	struct descriptor_data *fishing_next;
};

typedef struct descriptor_data sDESC_DATA, *sPDESC_DATA;


/////////////////////////////////////////
// M O B   R E S E T   I N F O   D A T A
/////////////////////////////////////////
typedef struct
{
	sPMOB_DATA list;
	sPMOB_DATA memory;

	int total;
	int total_memory;
} sMOBS;



///////////////////////////////////////////
// I T E M  G L O V A L  D A T A
///////////////////////////////////////////
typedef struct
{
	sPITEM_DATA list;
	sPITEM_DATA memory;
	int total;
	int total_memory;

	sPMYSHOP_ITEM			myshopitem_memory;

	sPBILL_ITEM_DATA		billItem_Memory;
} sITEMS;


///////////////////////////////////////////
// P L A Y E R  G L O V A L  D A T A
///////////////////////////////////////////
typedef struct
{
	CList	WaitAllData;
	CList	WaitCharData;
	CList	WaitDelData;

	sPDESC_DATA name_list[dHASHKEY_STR_MAX];
	sPDESC_DATA id_list[dHASHKEY_STR_MAX];

	sPDESC_DATA list;				// 전체 접속자 리스트
	sPDESC_DATA memory;				// Garbage List
	int	total;
	int	total_memory;
	
	sPUSER_SKILL userSkill_memory;	
	sPFRIEND_DATA friend_memory;
	
	// 낚시 리스트
	sPDESC_DATA fishingList;
} sPLAYERS;

/////////////////////////////////////////////////////////
// DB CONNECTION INFO
/////////////////////////////////////////////////////////
typedef struct
{
	char	host[128];
	int		port;

	char	id[128];
	char	pw[128];
	char	dbName[128];
} sDB_INFO, *sPDB_INFO;


///////////////////////////////////////////////////////
// BILLING CONNECTION SERVER
///////////////////////////////////////////////////////
typedef struct 
{
	char	host[128];
	int		port;
	int		ServerNum;
}sBILL_INFO, *sPBILL_INFO;


///////////////////////////////////////////////////////
// 일반 스킬 구조체
///////////////////////////////////////////////////////
typedef struct skill_data sSKILL_DATA, *sPSKILL_DATA;

struct skill_data 
{
	int			itemNum;		// 아이템 번호

	char		hName[50];			// 한글이름 

	short		skillType;			// 기술형태
	short		effectIndex;			// 이펙트 인덱스
	BYTE		checkReuse;			// 중복검색
	short		skillAttr;			// 기술속성
	
	WORD		needSAttack;			// 요구 근거리 
	WORD		needLAttack;			// 요구 원거리 
	WORD		needBMagic;			// 요구흑마법
	WORD		needWMagic;			// 요구백마법

	BYTE		needClass;			// 습득가능직업

	WORD		minDamage;			// 최소데미지
	WORD		maxDamage;			// 최대데미지

	int			effectType;		// 이펙트 종류
	int			effectValueType;	// 이펙트 값 종류
	int			effectValue;		// 이펙트 적용 값

	WORD		needHP;			// 시전소모 HP
	WORD		needMP;			// 시전소모 MP
	
	BYTE		targetType;			// 시전대상
	BYTE		range;				// 시전거리
	int			timer;			// 지속시간 

	int			coolTime;		// 쿨타임 
	int			coolTimeIdx;		// 쿨타임 인덱스
	int			areaRange;		// 지역공격 범위

	int			tmp1;

	int			beforSkill;		// 이전 스킬

	int			sattackExp;		// 경험치 분배율
	int			lattackExp;
	int			bmagicExp;
	int			wmagicExp;

	BOOL		(*func)( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
};


//////////////////////////////////////////////////////////////
// 길드 구조체 & defines
//////////////////////////////////////////////////////////////
#define dMAX_LINKED_CASTLE				4			// 인접성 정보 MAX 4
#define dMAX_WAR_CAMP				2			// 맵당 진지수 2

#define dMAX_GUILD					1024			// 1024개
#define dMAX_GUILD_ZONE				10	
#define dGUILD_NAME_LEN				32			// 32자 

#define dWARDATA_TYPE_NONE				0
#define dWARDATA_TYPE_CAMP				1
#define dWARDATA_TYPE_CASTLE			2


// 
struct __war_data
{
	int ownerGuild;
	int ownerPoint;

	int enemyGuild;

	int targetMapNum;

	BYTE type;

	DWORD lastUpdateTime;

	struct __war_data *prev;
	struct __war_data *next;
};

typedef struct __war_data sWAR_DATA, *sPWAR_DATA;

struct __member_info
{
	char		name[dNAME_LEN];

	int			level;

	BYTE		btClass;
	BYTE		memberLevel;

	BYTE		btKickOut;

	struct __member_info *prev;
	struct __member_info *next;
};

typedef struct __member_info sMEMBER_INFO, *sPMEMBER_INFO;

//
#define dMAX_GUILD_NOTICE_LENGTH					512
struct guild_data
{
	WORD guildNum;						// 길드번호 
	char guildName[dGUILD_NAME_LEN];				// 길드이름
	
	char masterName[dNAME_LEN];					// 마스터 이름 
	
	WORD memberCnt;						// 멤버수 

	BYTE guildType;						// 길드타입( 용병단이냐 군단이냐 )
	int	 guildMark;						// 길드마크
	int	 guildPoint;

	DWORD	makeDate;						// 생성 일자	
	sPDESC_DATA memberList;					// 멤버 리스트 
	sPDESC_DATA mercenaryList;					// 용병 리스트

	sPMEMBER_INFO memberInfoList;				// 맴버 리스트

	char		notice[dMAX_GUILD_NOTICE_LENGTH];		// 길드 공지

	struct guild_data *prev;						
	struct guild_data *next;
};

typedef struct guild_data sGUILD_DATA, *sPGUILD_DATA;

////////////////////////////////////////////////////////
// 성물
////////////////////////////////////////////////////////
typedef struct 
{
	int allowGuild[2];

	int posX;
	int posY;
} sWAR_CRYSTAL, *sPWAR_CRYSTAL;

#define dMAX_CASTLE_DOOR	3

// 성문
typedef struct 
{
	int posX;
	int posY;
	int angle;
} sCASTLE_DOOR, *sPCASTLE_DOOR;



///////////////////////////////////////////////////////
// GUILD GLOBAL DATA
///////////////////////////////////////////////////////
typedef struct 
{
	sPGUILD_DATA array[dMAX_GUILD];
	sPGUILD_DATA list;
	sPGUILD_DATA memory;

	sPMEMBER_INFO member_memory;

	sPWAR_DATA	 war_list;
	sPWAR_DATA	 war_memory;
} sGUILD, *sPGUILD;


////////////////////////////////////////////////////////////////
// 특화스킬 구조체 
////////////////////////////////////////////////////////////////
struct spe_skill_info
{
	int		itemNum;
	int		needSkill[4];
	BYTE	classNum;
	BYTE	skillIdx[2];
};

typedef struct spe_skill_info sSPE_SKILL, *sPSPE_SKILL;



typedef struct
{
	int guildNum[6];
} sFIND_WAR_ENEMY, *sPFIND_WAR_ENEMY;



#endif


//
 
