/**
 *	$Workfile: billing.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 8 $
 *	$Date: 04-11-18 1:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/billing.h $
 * 
 * 8     04-11-18 1:40a Sk8snow
 * 
 * 7     04-09-20 10:52p Sk8snow
 * 
 * 6     04-09-08 3:54p Sk8snow
 * 
 * 5     04-07-13 7:26a Sk8snow
 * 
 * 4     04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __BILLING_H__
#define __BILLING_H__

extern MYSQL *g_BILL_SQL;

#ifdef __CHINA_VER__
extern SOCKET	g_BillSoket;
extern sBILL_INFO g_BILL_INFO;
#endif
extern sDB_INFO g_BILL_DBINFO;



/*enum eBILL_DB_ROW
{
	dBILL_DATA_PURKEY,
	dBILL_DATA_USERUID,
	dBILL_DATA_PRODUCTNUM,
	dBILL_DATA_
};*/

enum
{
	PURROW_PURKEY = 0,
	PURROW_USERUID,
	PURROW_PRODUCTNUM,
	PURROW_PRODUCTPRICE,
	PURROW_PURFLAG,
	PURROW_PURDATE,
	PURROW_PURCHGDATE,
};

enum
{
	BILLITEM_IDX = 0,
	BILLITEM_OWNERID,
	BILLITEM_OWNERPOS,
	BILLITEM_EFFECT1,
	BILLITEM_EFFECT2,
	BILLITEM_EFFECT3,
	BILLITEM_EFFECT4,
	BILLITEM_EFFECT5,
	BILLITEM_EFFECT6,
	BILLITEM_EFFECT7,
	BILLITEM_EFFECT8,
	BILLITEM_EFFECT9,
	BILLITEM_EFFECT10,
	BILLITEM_EFFECT11,
	BILLITEM_EFFECT12,
	BILLITEM_EFFECT13,
	BILLITEM_EFFECT14,
	BILLITEM_EFFECT15,
	BILLITEM_EFFECT16,
	BILLITEM_EFFECT17,
	BILLITEM_EFFECT18,
	BILLITEM_EFFECT19,
	BILLITEM_EFFECT20,
};


#define dMAX_BILLING_ITEM_INFO			512

typedef struct 
{
	int		itemNum;
	int		privateCode;
	int		pcbangCode;
	int		cnt;
} sBILLING_TABLE, *sPBILLING_TABLE;

///////////////////////////////////////////////////////////////
// D E F I N E S
///////////////////////////////////////////////////////////////

// 프리미엄 존
#define dPRIMIUM_ZONE1		31	
#define dPRIMIUM_ZONE2		32
#define dPRIMIUM_ZONE3		33
////////////////////////////////////////////////////////////////////////////
// 유료화 아이템 효과 배열 Arrangement of paid item effects
////////////////////////////////////////////////////////////////////////////
#define dBILL_ITEM_EFFECT1		0		//경험치 1.3배	
#define dBILL_ITEM_EFFECT2		1		//경험치 1.5배	
#define dBILL_ITEM_EFFECT3		2		//사냥시 아이템 드랍률 1.3배	
#define dBILL_ITEM_EFFECT4		3		//사냥시 아이템 드랍률 1.5배	
#define dBILL_ITEM_EFFECT5		4		//프리미엄 존에서 사냥가능	
#define dBILL_ITEM_EFFECT6		5		//경험치와 아이템 드랍률 2배(프리미엄 존 플러스)
#define dBILL_ITEM_EFFECT7		6		//인챈트 카드의 드랍률 증가	
#define dBILL_ITEM_EFFECT8		7		//이동속도 증가.	
#define dBILL_ITEM_EFFECT9		8		//무게에 대한 힘 증가 ( 20% , 1시간 )	
#define dBILL_ITEM_EFFECT10		9		//1시간동안 HP량 증가	
#define dBILL_ITEM_EFFECT11		10		//1시간 동안 MP 증가	
#define dBILL_ITEM_EFFECT12		11		//1시간 동안 파티원의 경험치가 1.3배 증가	
#define dBILL_ITEM_EFFECT13		12		//1시간동안 파티원 HP 20% 증가	
#define dBILL_ITEM_EFFECT14		13		//1시간동안 파티원 MP 20% 증가	
#define dBILL_ITEM_EFFECT15		14		//1시간동안 군단원 HP 20% 증가	
#define dBILL_ITEM_EFFECT16		15		//1시간동안 군단원 MP 20% 증가	
#define dBILL_ITEM_EFFECT17		16		// 마이너스 ( 1시간동안 성향치 증가 2배 )


///////////////////////////////////////////////////////////////////////////////
// 실질 아이템 번호 Real item number
///////////////////////////////////////////////////////////////////////////////
#define dBILL_ITEM_NUM1		11907		//경험치 1.3배	
#define dBILL_ITEM_NUM2		11908		//경험치 1.3배	
#define dBILL_ITEM_NUM3		11909		//경험치 1.3배	
#define dBILL_ITEM_NUM4		11910		//경험치 1.5배	
#define dBILL_ITEM_NUM5		11911		//경험치 1.5배	
#define dBILL_ITEM_NUM6		11912		//경험치 1.5배	
#define dBILL_ITEM_NUM7		11916		//사망시 경험치 손실없음	
#define dBILL_ITEM_NUM8		11917		//사망시 경험치 손실없음	
#define dBILL_ITEM_NUM9		11918		//사망시 경험치 손실없음	
#define dBILL_ITEM_NUM10		11919		//사망시 아이템 유실 없음	
#define dBILL_ITEM_NUM11		11920		//사망시 아이템 유실 없음	
#define dBILL_ITEM_NUM12		11921		//사망시 아이템 유실 없음	
#define dBILL_ITEM_NUM13		11922		//맵 전체 외침	
#define dBILL_ITEM_NUM14		11923		//맵 전체 외침	
#define dBILL_ITEM_NUM15		11924		//맵 전체 외침	
#define dBILL_ITEM_NUM16		11925		//사냥시 아이템 드랍률 1.3배	
#define dBILL_ITEM_NUM17		11926		//사냥시 아이템 드랍률 1.3배	
#define dBILL_ITEM_NUM18		11927		//사냥시 아이템 드랍률 1.3배	
#define dBILL_ITEM_NUM19		11928		//사냥시 아이템 드랍률 1.5배	
#define dBILL_ITEM_NUM20		11929		//사냥시 아이템 드랍률 1.5배	
#define dBILL_ITEM_NUM21		11930		//사냥시 아이템 드랍률 1.5배	
#define dBILL_ITEM_NUM22		11931		//사망시 부활	
#define dBILL_ITEM_NUM23		11932		//사망시 부활	
#define dBILL_ITEM_NUM24		11933		//사망시 부활	
#define dBILL_ITEM_NUM25		11934		//길드공지	
#define dBILL_ITEM_NUM26		11935		//길드공지	
#define dBILL_ITEM_NUM27		11936		//길드공지	
#define dBILL_ITEM_NUM28		11937		//프리미엄 존에서 사냥가능	
#define dBILL_ITEM_NUM29		11938		//프리미엄 존에서 사냥가능	
#define dBILL_ITEM_NUM30		11939		//프리미엄 존에서 사냥가능	
#define dBILL_ITEM_NUM31		11940		//경험치와 아이템 드랍률 2배(프리미엄 존 플러스)	
#define dBILL_ITEM_NUM32		11941		//경험치와 아이템 드랍률 2배(프리미엄 존 플러스)	
#define dBILL_ITEM_NUM33		11942		//경험치와 아이템 드랍률 2배(프리미엄 존 플러스)	
#define dBILL_ITEM_NUM34		11943		//팻 먹이 
#define dBILL_ITEM_NUM35		11944		//팻 먹이 
#define dBILL_ITEM_NUM36		11945		//팻 먹이 
#define dBILL_ITEM_NUM37		11946		//인챈트 카드의 드랍률 증가
#define dBILL_ITEM_NUM38		11947		//인챈트 카드의 드랍률 증가
#define dBILL_ITEM_NUM39		11948		//인챈트 카드의 드랍률 증가
#define dBILL_ITEM_NUM40		11949		//인첸트 카드 변환 ( 아머 -> 웨폰 , 웨폰 -> 아머 )
#define dBILL_ITEM_NUM41		11950		//인첸트 카드 변환 ( 아머 -> 웨폰 , 웨폰 -> 아머 )
#define dBILL_ITEM_NUM42		11951		//인첸트 카드 변환 ( 아머 -> 웨폰 , 웨폰 -> 아머 )
#define dBILL_ITEM_NUM43		11952		//인첸트된 아이템을 인첸트 카드로 환원
#define dBILL_ITEM_NUM44		11953		//인첸트된 아이템을 인첸트 카드로 환원
#define dBILL_ITEM_NUM45		11954		//인첸트된 아이템을 인첸트 카드로 환원
#define dBILL_ITEM_NUM46		11955		//상대방 유져의 아이템정보를 훔쳐본다
#define dBILL_ITEM_NUM47		11956		//상대방 유져의 아이템정보를 훔쳐본다
#define dBILL_ITEM_NUM48		11957		//상대방 유져의 아이템정보를 훔쳐본다
#define dBILL_ITEM_NUM49		11958		//아이템룩을 사용하지 못하게 한다
#define dBILL_ITEM_NUM50		11959		//아이템룩을 사용하지 못하게 한다
#define dBILL_ITEM_NUM51		11960		//아이템룩을 사용하지 못하게 한다
#define dBILL_ITEM_NUM52		11961		//상대의 능력치를 볼 수 있다.
#define dBILL_ITEM_NUM53		11962		//상대의 능력치를 볼 수 있다.
#define dBILL_ITEM_NUM54		11963		//상대의 능력치를 볼 수 있다.
#define dBILL_ITEM_NUM55		11964		//레벨룩을 사용하지 못하게 한다.
#define dBILL_ITEM_NUM56		11965		//레벨룩을 사용하지 못하게 한다.
#define dBILL_ITEM_NUM57		11966		//레벨룩을 사용하지 못하게 한다.
#define dBILL_ITEM_NUM58		11967		//이동속도 증가.
#define dBILL_ITEM_NUM59		11968		//이동속도 증가.
#define dBILL_ITEM_NUM60		11969		//이동속도 증가.
#define dBILL_ITEM_NUM61		11970		//무게에 대한 힘 증가 ( 20% , 1시간 )
#define dBILL_ITEM_NUM62		11971		//무게에 대한 힘 증가 ( 20% , 1시간 )
#define dBILL_ITEM_NUM63		11972		//무게에 대한 힘 증가 ( 20% , 1시간 )
#define dBILL_ITEM_NUM64		11973		//1시간동안 HP량 20% 증가
#define dBILL_ITEM_NUM65		11974		//1시간동안 HP량 20% 증가
#define dBILL_ITEM_NUM66		11975		//1시간동안 HP량 20% 증가
#define dBILL_ITEM_NUM67		11976		//1시간 동안 MP 20% 증가
#define dBILL_ITEM_NUM68		11977		//1시간 동안 MP 20% 증가
#define dBILL_ITEM_NUM69		11978		//1시간 동안 MP 20% 증가
#define dBILL_ITEM_NUM70		11913		//1시간 동안 파티원의 경험치가 1.3배 증가
#define dBILL_ITEM_NUM71		11914		//1시간 동안 파티원의 경험치가 1.3배 증가
#define dBILL_ITEM_NUM72		11915		//1시간 동안 파티원의 경험치가 1.3배 증가
#define dBILL_ITEM_NUM73		11979		//1시간동안 파티원 HP 20% 증가
#define dBILL_ITEM_NUM74		11980		//1시간동안 파티원 HP 20% 증가
#define dBILL_ITEM_NUM75		11981		//1시간동안 파티원 HP 20% 증가
#define dBILL_ITEM_NUM76		11982		//1시간동안 파티원 MP 20% 증가
#define dBILL_ITEM_NUM77		11983		//1시간동안 파티원 MP 20% 증가
#define dBILL_ITEM_NUM78		11984		//1시간동안 파티원 MP 20% 증가
#define dBILL_ITEM_NUM79		11985		//1시간동안 군단원 HP 20% 증가
#define dBILL_ITEM_NUM80		11986		//1시간동안 군단원 HP 20% 증가
#define dBILL_ITEM_NUM81		11987		//1시간동안 군단원 HP 20% 증가
#define dBILL_ITEM_NUM82		11988		//1시간동안 군단원 MP 20% 증가
#define dBILL_ITEM_NUM83		11989		//1시간동안 군단원 MP 20% 증가
#define dBILL_ITEM_NUM84		11990		//1시간동안 군단원 MP 20% 증가
#define dBILL_ITEM_NUM85		11991		// 웜쓰오브 아라임 1.3배 ( 1달 )
#define dBILL_ITEM_NUM86		11992		// 웜쓰오브 아라임 1.5배 ( 1달 )
#define dBILL_ITEM_NUM87		11993		// 아이템 오브 아라임 1.3배 ( 1달 )
#define dBILL_ITEM_NUM88		11994		// 아이템 오브 아라임 1.5배 ( 1달 )
#define dBILL_ITEM_NUM89		11995		// 프리미엄 존 서비스 ( 1달 )
#define dBILL_ITEM_NUM90		11996		// 프리미엄 존 플러스 ( 1달 )
#define dBILL_ITEM_NUM91		11997		// 신의 축복 ( 1달 )
#define dBILL_ITEM_NUM92		11998		// 단원소환 
#define dBILL_ITEM_NUM93		11999		// 단원소환 
#define dBILL_ITEM_NUM94		12000		// 단원소환 
#define dBILL_ITEM_NUM95		12001		// 단원필승의 외침
#define dBILL_ITEM_NUM96		12002		// 단원필승의 외침
#define dBILL_ITEM_NUM97		12003		// 단원필승의 외침
#define dBILL_ITEM_NUM98		12004		// 단원 축복의 외침
#define dBILL_ITEM_NUM99		12005		// 단원 축복의 외침
#define dBILL_ITEM_NUM100		12006		// 단원 축복의 외침
#define dBILL_ITEM_NUM101		12007		// 단원 기원의 외침
#define dBILL_ITEM_NUM102		12008		// 단원 기원의 외침
#define dBILL_ITEM_NUM103		12009		// 단원 기원의 외침
#define dBILL_ITEM_NUM104		12010		// 단원 저주의 외침
#define dBILL_ITEM_NUM105		12011		// 단원 저주의 외침
#define dBILL_ITEM_NUM106		12012		// 단원 저주의 외침
#define dBILL_ITEM_NUM107		12013		// 단원 도발의 외침
#define dBILL_ITEM_NUM108		12014		// 단원 도발의 외침
#define dBILL_ITEM_NUM109		12015		// 단원 도발의 외침
#define dBILL_ITEM_NUM110		12016		// 마이너스
#define dBILL_ITEM_NUM111		12017		// 마이너스
#define dBILL_ITEM_NUM112		12018		// 마이너스
#define dBILL_ITEM_NUM113		12019		// 하트 이모티콘
#define dBILL_ITEM_NUM114		12020		// 하트 이모티콘
#define dBILL_ITEM_NUM115		12021		// 하트 이모티콘
#define dBILL_ITEM_NUM116		12022		// 기쁨 이모티콘
#define dBILL_ITEM_NUM117		12023		// 기쁨 이모티콘
#define dBILL_ITEM_NUM118		12024		// 기쁨 이모티콘
#define dBILL_ITEM_NUM119		12025		// 슬픔 이모티콘
#define dBILL_ITEM_NUM120		12026		// 슬픔 이모티콘
#define dBILL_ITEM_NUM121		12027		// 슬픔 이모티콘
#define dBILL_ITEM_NUM122		12028		// 분노 이모티콘
#define dBILL_ITEM_NUM123		12029		// 분노 이모티콘
#define dBILL_ITEM_NUM124		12030		// 분노 이모티콘
#define dBILL_ITEM_NUM125		13551		// 웨폰프로텍트
#define dBILL_ITEM_NUM126		13552		// 아머프로텍트 
#define dBILL_ITEM_NUM127		13553		// 
#define dBILL_ITEM_NUM128		13554		// 
#define dBILL_ITEM_NUM129		13555		//  
#define dBILL_ITEM_NUM130		13556		//  
#define dBILL_ITEM_NUM131		13557		//  
#define dBILL_ITEM_NUM132		13558		// 
#define dBILL_ITEM_NUM133		13559		//  
#define dBILL_ITEM_NUM134		13560		//  
#define dBILL_ITEM_NUM135		13561		//  
#define dBILL_ITEM_NUM136		13562		// 
#define dBILL_ITEM_NUM137		13563		//  
#define dBILL_ITEM_NUM138		13564		// 
#define dBILL_ITEM_NUM139		13565		// 
#define dBILL_ITEM_NUM140		13566		//  
#define dBILL_ITEM_NUM141		13567		//  
#define dBILL_ITEM_NUM142		13568		//  
#define dBILL_ITEM_NUM143		13569		// 
#define dBILL_ITEM_NUM144		13570		//  
#define dBILL_ITEM_NUM145		13571		//  
#define dBILL_ITEM_NUM146		13572		//  
#define dBILL_ITEM_NUM147		13573		//  
#define dBILL_ITEM_NUM148		13574		// 
#define dBILL_ITEM_NUM149		13575		//  


// FUNC

BOOL IsBillingItem( int itemNum );
void UpdateBillItemInfo( sPDESC_DATA pPlayer, char *rowName, DWORD value );
void LoadPlayerBillingInfo( sPDESC_DATA pPlayer );
void DeleteBillingItemInfo( sPDESC_DATA pPlayer );

BOOL LoadBillingTable();
sPBILLING_TABLE FindBillingItem( int PurCode );

void LoadNonUsedBillItems( sPDESC_DATA pPlayer );
void DestroyBillItemMemory( );
void LoadReward( sPDESC_DATA pPlayer );//아이템보상시 사용
sPBILL_ITEM_DATA FindBillItemByNum( sPDESC_DATA pPlayer, int itemNum );
sPBILL_ITEM_DATA FindBillItemByIdx( sPDESC_DATA pPlayer, int idxNum );

// 중국버전
#ifdef __CHINA_VER__
#define		dUSER_LOGIN_BILLING		1
#define		dUSER_BILLING_INFOR		2
#define     		dUSER_LOGOUT			3
#define		dUSER_TIME_REPORT		4
#define		dUSER_TIMEOUT			5
#define		dUSER_START_BILLING		6
#define		dUSER_END_BILLING		7
#define		dUSER_GAME_PING		8

extern BOOL		g_bGameStar;

int CHN_ConnectToBillServer( sPDB_INFO pInfo );
void StartBilling( int ServerNum );
void EndBilling( int ServerNum );
void PingBilling( int ServerNum  );

/*
	Send Function
 */
void GetBillingInfo( char *szID, char *szMac, char *szIP, int level, int ServerNum );
void OutBillingInfo( char *szID, int ServerNum );

/*
	Recv Function
*/
int RecvBillingPacket();
int ReadBillingBuffer();

#endif

void SaveUserBillEffect( sPDESC_DATA pPlayer );
 
#endif // __BILLING_H__




//EOF





