#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#define dUSE_SEND_BUFF

#define dMAX_SOCK_BUFF	40960000

#define dNAME_LEN			50
#define dID_LEN				50
#define dPW_LEN				50

#define dMAX_SKILL			4

///////////////////////////////////////////////
// for name hash
///////////////////////////////////////////////
#define dHASHKEY_STR_ETC	256
#define dHASHKEY_STR_MAX	(dHASHKEY_STR_ETC+1)


#define dCH_EQ_HEAD						0
#define dCH_EQ_JACKET					1
#define dCH_EQ_PANTS					2
#define dCH_EQ_FOOT						3
#define dCH_EQ_HAND						4

#define dCH_EQ_MAX						5

struct db_info
{
	char	host[128];
	int		port;

	char	id[128];
	char	pw[128];
	char	dbName[128];
} ;

typedef struct db_info sDB_INFO, *sPDB_INFO;

struct desc_data
{
	SOCKET sock;
	
	char	ip[20];
	
	char	recvBuff[dMAX_SOCK_BUFF];		// recv 데이터 버퍼 
	int		recvSize;						// recv 버퍼 상의 총 데이터 량
	int		recvPos;						
	
#ifdef dUSE_SEND_BUFF
	char	sendBuff[dMAX_SOCK_BUFF];		// send 대기중인 데이터 
	int		sendSize;						// send 할 데이터량
#endif

	CList	DataList[dHASHKEY_STR_MAX];
} ;

typedef struct desc_data sDESC_DATA, *sPDESC_DATA;

////////////////////////////////////////////////////
// C H A R A C T E R    D A T A
////////////////////////////////////////////////////
struct character_data
{
	char	name[dNAME_LEN];				// 케릭터 이름 
	char	id[dID_LEN];					// ID

	BYTE	charClass;						// 직업

	WORD	headStyle;						// 머리 스타일 
	WORD	faceStyle;						// 얼굴 스타일

	WORD	chEquip[dCH_EQ_MAX];			// 장착 아이템 번호

	BYTE	sex;							// 성별

	WORD	mapNum;							// 맵번호
	int		posX, posY;						// 좌표
	BYTE	posLayer;						// 맵 레이어 번호
	BYTE	posMapKind;						// 맵 종류

	WORD	prevMapNum;
	int		prevPosX, prevPosY;

	int		hp, max_hp;						// 체력
	int		mp, max_mp;						// 마나력

	int		money;							// 소유 금액
	int		nCash;							// 소유 캐쉬

	WORD	skill[dMAX_SKILL];				// 4 개의 스킬 레벨 
	int		skill_exp[dMAX_SKILL];			// 4 개의 스킬 경험치
	
	WORD	str;							// 힘
	WORD	str_add;						// 힘 +
	int		str_exp;						// 힘 경험치
	WORD	intelli;						// 지력
	WORD	intelli_add;					// 지력 +
	int		intelli_exp;					// 지력 경험치
	WORD	dex;							// 민첩
	WORD	dex_add;						// 민첩 +
	int		dex_exp;						// 민첩 경험치

	BYTE	charPos;						// 몇번째 케릭터?

	BYTE	mercType;						// 용병 타입
	int		mercLevel;						// 용병 레벨
	int		mercExp;						// 용병 경험치

	int		cha;							// 카리스마
	
	int		guildNo;						// 길드번호 
	int		guildLevel;						// 길드 레벨
	int		tmpGuildNum;					// 용병으로 소속된 길드
	
	WORD	useSkill;						// 사용 기술
	BYTE	isNewCharacter;					// 신규 케릭터 ?
};



typedef struct character_data sCHAR_DATA, *sPCHAR_DATA;


#endif



