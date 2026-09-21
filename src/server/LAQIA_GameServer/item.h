/**
 *	$Workfile: item.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 39 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/item.h $
 * 
 * 39    04-12-15 10:40a Sk8snow
 * 
 * 38    04-09-11 11:20p Sk8snow
 * 
 * 37    04-09-08 3:54p Sk8snow
 * 
 * 36    04-07-13 7:26a Sk8snow
 * 
 * 35    04-05-29 10:12p Sk8snow
 * 
 * 34    04-05-14 10:38p Sk8snow
 * 
 * 33    04-05-11 1:45a Sk8snow
 * 
 * 32    04-04-30 9:46a Sk8snow
 * 
 * 31    04-04-01 5:33p Sk8snow
 * 
 * 30    04-03-31 8:22a Sk8snow
 *        
 */

#ifndef __ITEM_H__
#define __ITEM_H__
#include "stack_limits.h"
int GetItemStackLimit(sPITEM_DATA item);
int GetItemStackCount(sPITEM_DATA item);
void ConsumeOneEnchantCard(sPDESC_DATA player, sPITEM_DATA card);

#define dMAX_ITEMINFO		20480

#define dMAX_ITEM_IDX		999999999

#define dSPELLBOOK_NUM		530					// 마법책 번호

#define dHP_POSION_S				10095			// HP 포션(소)
#define dHP_POSION_M				10096			// HP 포션(중)
#define dHP_POSION_L				10097			// HP 포션(대)

#define dMP_POSION_S				10098			// MP 포션(소)
#define dMP_POSION_M				10099			// MP 포션(중)
#define dMP_POSION_L				10100			// MP 포션(대)

#define dTELEPORT_ZILS			10175			// 질스(40)
#define dTELEPORT_GOURSE			10176			// 고스(22)
#define dTELEPORT_VAIL			10177			// 베일(3)
#define dTELEPORT_MIRAS			10178			// 미라스(4) 
#define dTELEPORT_MOHAS			10179			// 모하스(6) 
#define dTELEPORT_NERIS			10180			// 네리스(8)
#define dTELEPORT_SUHER			10181			// 수헤르(11)
#define dTELEPORT_TAROS			10182			// 타로스(14)
#define dTELEPORT_MAP15			10183			// 로비탄(15)
#define dTELEPORT_MAP17			10184			// 레벤(17)

#define dRIDE_ITEM1				10188			// 말1 ( 갈색말	)
#define dRIDE_ITEM2				10189			// 말2 ( 검정말 )
#define dRIDE_ITEM3				10190			// 말3 ( 하양말 )

#define dRIDE_TYPE1				1			// 갈색말
#define dRIDE_TYPE2				2			// 검정말
#define dRIDE_TYPE3				3			// 하양말

#define dMONEY_NUM				10196			// 돈 아이템 번호 

#define dDENY_ITEM_FRIEND			10148			// 친구차단
#define dDENY_ITEM_CHAT			10149			// 채팅차단
#define dDENY_ITEM_WHISPER			10150			// 귓말차단
#define dDENY_ITEM_PARTY			10152			// 파티차단
#define dDENY_ITEM_DEAL			10151			// 거래차단

#define dENCHANT_CARD_WEAPON		10193			// 웨폰 인첸트 카드
#define dENCHANT_CARD_ARMOR			10194			// 방어구 인첸트 카드
#define dENCHANT_CARD_OPTION		10195			// 옵션 인첸트 카드


#define dEQUIP_ITEM_A_LEVEL			0			//A 등급 0 -24
#define dEQUIP_ITEM_B_LEVEL			25			//B 등급 25 -  49
#define dEQUIP_ITEM_C_LEVEL			50			//C 등급 50 - 74
#define dEQUIP_ITEM_D_LEVEL			75			//D 등급 75 - 99
#define dEQUIP_ITEM_E_LEVEL			100			//E 등급 120 
#define dEQUIP_ITEM_F_LEVEL			125			//F 등급 150 
#define dEQUIP_ITEM_G_LEVEL			150			//G 등급 180 
#define dEQUIP_ITEM_H_LEVEL			210			//H 등급 210 
#define dEQUIP_ITEM_I_LEVEL			240			//I 등급 240 
#define dEQUIP_ITEM_J_LEVEL			270			//J 등급 270 

#define dITEM_A_LEVEL			1
#define dITEM_B_LEVEL			2
#define dITEM_C_LEVEL			3
#define dITEM_D_LEVEL			4
#define dITEM_E_LEVEL			5
#define dITEM_F_LEVEL			6
#define dITEM_G_LEVEL			7
#define dITEM_H_LEVEL			8
#define dITEM_I_LEVEL			9
#define dITEM_J_LEVEL			10

#define dEVENT_MOB_DROP_MONEY_MIN		50000  		//드랍 돈 수정
#define dEVENT_MOB_DROP_MONEY_MAX		125000


#define dMAY_BOX_ITEM			10192			// 메이박스

#define dMAP_ITEM_LIFE			3			// 맵 위의 아이템 수명 ( 분단위 )

#define dMAX_POTION_CNT			10000			// 포션 CNT

#define dMAX_GARBAGE_CNT			90			//창고최대보관		


typedef struct
{
	BYTE page;
	BYTE x, y;
} sINVEN_POS, *sPINVEN_POS;

struct item_command
{
	char command[50];
	void (*item_func)(sPDESC_DATA pPlayer, char *command );
};

/////////////////////////////////////////////////////////////////
// D  E  F  I  N  E  S
/////////////////////////////////////////////////////////////////
#define dMAGIC_KIND_BLACK			1			// 흑마법 
#define dMAGIC_KIND_WHITE			2			// 백마법 

#define IS_VALID_ITEM( num ) ( num < dMAX_ITEMINFO && g_ItemInfo[num] )

#define GET_ITEM_PRICE( item ) (g_ItemInfo[item->itemNum]->Price)
#define GET_ITEM_PRICE2( num ) (g_ItemInfo[num]->Price)
#define GET_MONEY_AMOUNT( item ) (item->exVal[0])
#define GET_ITEM_SELLPRICE( item ) (g_ItemInfo[item->itemNum]->sellPrice )
#define GET_ITEM_TYPE( item ) (g_ItemInfo[item->itemNum]->itemType)
#define GET_ITEM_TYPE2( num ) (g_ItemInfo[num]->itemType)

#define GET_ITEM_LEVEL( item ) (g_ItemInfo[item->itemNum]->itemLevel)

#define GET_ITEM_UNIQ( item ) (g_ItemInfo[item->itemNum]->isUniq)
#define GET_ITEM_UNIQ2( num ) (g_ItemInfo[num]->isUniq)
#define GET_ITEM_NEEDLOG( item ) (g_ItemInfo[item->itemNum]->needItemLog)
#define GET_ITEM_NEEDLOG2( num ) (g_ItemInfo[num]->needItemLog)
#define GET_ITEM_NEEDCLASS( item ) (g_ItemInfo[item->itemNum]->needClass)
#define GET_ITEM_NEEDCLASS2( num ) (g_ItemInfo[num]->needClass)

#define GET_NEED_SEX( item ) (g_ItemInfo[item->itemNum]->m_NeedSex)
#define GET_NEED_STR( item ) (g_ItemInfo[item->itemNum]->needStr)
#define GET_NEED_INT( item ) (g_ItemInfo[item->itemNum]->needInt)
#define GET_NEED_DEX( item ) (g_ItemInfo[item->itemNum]->needDex)

#define GET_ITEM_HNAME( item ) (g_ItemInfo[item->itemNum]->hName )
#define GET_ITEM_HNAME2( num ) (g_ItemInfo[num]->hName )
#define GET_ITEM_ENAME( item ) (g_ItemInfo[item->itemNum]->eName )

#define GET_ITEM_MINMANADMG( item ) (g_ItemInfo[item->itemNum]->minMana)
#define GET_ITEM_MINMANADMG2( num ) (g_ItemInfo[num]->minMana)
#define GET_ITEM_MAXMANADMG( item ) (g_ItemInfo[item->itemNum]->maxMana)
#define GET_ITEM_MAXMANADMG2( num ) (g_ItemInfo[num]->maxMana)

#define GET_ITEM_MINDAMAGE( item ) (g_ItemInfo[item->itemNum]->minDamage )
#define GET_ITEM_MAXDAMAGE( item ) (g_ItemInfo[item->itemNum]->maxDamage )

#define GET_ITEM_ATTACKRANGE( item ) (g_ItemInfo[item->itemNum]->attackRange)

#define GET_ITEM_ATTR( item ) (g_ItemInfo[item->itemNum]->itemAttr)

#define GET_ITEM_PLUSESTR( item ) (g_ItemInfo[item->itemNum]->pluseStr)
#define GET_ITEM_PLUSEINT( item ) (g_ItemInfo[item->itemNum]->pluseInt)
#define GET_ITEM_PLUSEDEX( item ) (g_ItemInfo[item->itemNum]->pluseDex)
#define GET_ITEM_PLUSEHP( item ) (g_ItemInfo[item->itemNum]->pluseHP)
#define GET_ITEM_PLUSEMP( item ) (g_ItemInfo[item->itemNum]->pluseMP)

#define GET_ITEM_WEIGHT( item ) (g_ItemInfo[item->itemNum]->weight)
#define GET_ITEM_WEIGHT2( num ) (g_ItemInfo[num]->weight)

#define IS_BILLING_ITEM( item ) (g_ItemInfo[item->itemNum]->isBillingItem)
#define IS_BILLING_ITEM2( num ) (g_ItemInfo[num]->isBillingItem)

//umenokoji
#define GET_ITEM_MAGIC_DEFENSE( item ) ( g_ItemInfo[item->itemNum]->pluseMagicDefense )		//마법방어 얻어오기
#define GET_ITEM_NUMBER( item ) ( g_ItemInfo[item->itemNum]->itemNum )
// 
#define INSERT_ITEM_TO_MEMORY( item ) \
{\
INSERT_TO_LIST( gITEMS.memory, item, prev, next ); \
gITEMS.total_memory++;\
}

#define ISSHORTWEAPON( i )						\
	( (i) == dITEMTYPE_ONEHANDSWORD	||			\
	  (i) == dITEMTYPE_TWOHANDSWORD	||			\
	  (i) == dITEMTYPE_DUALSWORD	||			\
	  (i) == dITEMTYPE_ONEHANDAXE	||			\
	  (i) == dITEMTYPE_TWOHANDAXE	||			\
	  (i) == dITEMTYPE_DUALAXE		||			\
	  (i) == dITEMTYPE_ONEHANDBLUNT	||			\
	  (i) == dITEMTYPE_TWOHANDBLUNT	||			\
	  (i) == dITEMTYPE_DUALBLUNT	||			\
	  (i) == dITEMTYPE_DONPA 		||			\
	  (i) == dITEMTYPE_SPHERE )					\
	  ? TRUE : FALSE

#define ISLONGWEAPON( i )						\
	( (i) == dITEMTYPE_SHORTBOW		||			\
	  (i) == dITEMTYPE_LONGBOW		||			\
	  (i) == dITEMTYPE_DDABALBOW	||			\
	  (i) == dITEMTYPE_CROSSBOW		||			\
	  (i) == dITEMTYPE_ARROW )					\
	  ? TRUE : FALSE

#define ISUPPERARMOR( i )						\
	( (i) == dITEMTYPE_WORRIOR_UP_ARMOR ||			\
	  (i) == dITEMTYPE_ARCHER_UP_ARMOR  ||			\
	  (i) == dITEMTYPE_WIZARD_UP_ARMOR	)			\
	  ? TRUE : FALSE

#define ISLOWERARMOR( i )						\
	( (i) == dITEMTYPE_WORRIOR_LO_ARMOR ||			\
	  (i) == dITEMTYPE_ARCHER_LO_ARMOR  ||			\
	  (i) == dITEMTYPE_WIZARD_LO_ARMOR	)			\
	  ? TRUE : FALSE
		
#define ISHELMET( i )						\
	( (i) == dITEMTYPE_WORRIOR_HELMET	||			\
	  (i) == dITEMTYPE_ARCHER_HELMET	||			\
	  (i) == dITEMTYPE_WIZARD_HELMET )				\
	  ? TRUE : FALSE

#define ISBELT( i )							\
	( (i) == dITEMTYPE_WARRIOR_BELT	||			\
	  (i) == dITEMTYPE_ARCHER_BELT	||			\
	  (i) == dITEMTYPE_WIZZARD_BELT )				\
	  ? TRUE : FALSE

#define ISGLOVE( i )							\
	( (i) == dITEMTYPE_WARRIOR_GLOVE	||			\
	  (i) == dITEMTYPE_ARCHER_GLOVE	||			\
	  (i) == dITEMTYPE_WIZZARD_GLOVE )				\
	  ? TRUE : FALSE

#define ISSHOES( i )							\
	( (i) == dITEMTYPE_WARRIOR_SHOES	||			\
	  (i) == dITEMTYPE_ARCHER_SHOES	||			\
	  (i) == dITEMTYPE_WIZZARD_SHOES )				\
	  ? TRUE : FALSE

#define ISWEAPONITEM( i )						\
	( ISSHORTWEAPON( i ) || ISLONGWEAPON( i ) ) ? TRUE : FALSE

#define ISDEFENSEITEM( i )						\
	( (i) >= 19 && (i) <= 37 ) ? TRUE : FALSE

#define ISDUALWEAPON( i )						\
	( (i) == dITEMTYPE_DUALSWORD ||				\
	  (i) == dITEMTYPE_DUALAXE ||				\
	  (i) == dITEMTYPE_DUALBLUNT ) ? TRUE : FALSE

#define ISTWOHANDWEAPON( i )					\
	( (i) == dITEMTYPE_TWOHANDSWORD ||				\
	  (i) == dITEMTYPE_TWOHANDAXE ||				\
	  (i) == dITEMTYPE_TWOHANDBLUNT ||				\
	  (i) == dITEMTYPE_SPHERE ||				\
	  (i) == dITEMTYPE_BONG ||					\
	  (i) == dITEMTYPE_STAFF ) ? TRUE : FALSE



#define ISONEHANEWEAPON( i )					\
	( (i) == dITEMTYPE_ONEHANDSWORD ||				\
	  (i) == dITEMTYPE_ONEHANDAXE ||				\
	  (i) == dITEMTYPE_ONEHANDBLUNT ) ? TRUE : FALSE
	  
/*#define IS100LVITEM( i )										   \
	( (i) <= dBLACKNIGHTARMOR && (i) >= dBLACKNIGHTARMOR20		&& \
	  (i) <= dWITHEHANTTUNIC && (i) >= dWITHEHANTTUNIC20		&& \
	  (i) <= dMONOFROAD	&& (i) >= dMONOFROAD20				    && \
	  (i) <= dBLACKNIGHTPANT && (i) >= dBLACKNIGHTPANT20		&& \
	  (i) <= dWITHEHANTPANT	&& (i) >= dWITHEHANTPANT20			&& \
	  (i) <= dMONOFPANT	&& (i) >= dMONOFPANT20					&& \
	  (i) <= dBLACKNIGHTGUN	&& (i) >= dBLACKNIGHTGUN20			&& \
	  (i) <= dWITHEHANTGLAVE && (i) >= dWITHEHANTGLAVE20		&& \
	  (i) <= dMONOFGLAVE && (i) >= dMONOFGLAVE20				&& \
	  (i) <= dBLACKNIGHTBUT	&& (i) >= dBLACKNIGHTBUT20			&& \
	  (i) <= dWITHEHANTBUT && (i) >= dWITHEHANTBUT20			&& \
	  (i) <= dMONOFBUT && (i) >= dMONOFBUT20					&& \
	  (i) <= dBLACKNIGHTSILD && (i) >= dBLACKNIGHTSILD20		&& \
	  (i) <= dBLACKNIGHTBIGKEN	&& (i) >= dBLACKNIGHTBIGKEN20	&& \
	  (i) <= dBLACKNIGHTONEHANDSOLD	&& (i) >= dBLACKNIGHTONEHANDSOLD20 && \
	  (i) <= dBLACKNIGHTTWOHANDSOLD	&& (i) >= dBLACKNIGHTTWOHANDSOLD20 && \
	  (i) <= dBLACKNIGHTAX && (i) >= dBLACKNIGHTAX20			&& \
	  (i) <= dBLACKNIGHTMEICE && (i) >= dBLACKNIGHTMEICE20		&& \
	  (i) <= dBLACKNIGHTSPIER && (i) >= dBLACKNIGHTSPIER20		&& \
	  (i) <= dBLACKNIGHTSHOTBOW && (i) >= dBLACKNIGHTSHOTBOW20	&& \
	  (i) <= dWITHEHANTLONGBOW && (i) >= dWITHEHANTLONGBOW20	&& \
	  (i) <= dWITHEHANTTINBOW && (i) >= dWITHEHANTTINBOW20		&& \
	  (i) <= dMONOFSTAFF && (i) >= dMONOFSTAFF20 ) ? TRUE : FALSE	  
*/


/////////////////////////////////////////////////////////////////
// S E X  
/////////////////////////////////////////////////////////////////
#define dSEX_BOTH					0		// 공용 
#define dSEX_MALE					1		// 남자 
#define dSEX_FEMALE					2		// 여자

/////////////////////////////////////////////////////////////////
// I  T  E  M     T  Y  P  E
/////////////////////////////////////////////////////////////////
#define dITEMTYPE_NONE					0		//"0.미설정",
#define dITEMTYPE_ONEHANDSWORD				1		//"1.한손검",
#define dITEMTYPE_TWOHANDSWORD				2		//"2.양손검",
#define dITEMTYPE_DUALSWORD					3		//"3.쌍칼",
#define dITEMTYPE_ONEHANDAXE				4		//"4.한손도끼",
#define dITEMTYPE_TWOHANDAXE				5		//"5.양손도끼",
#define dITEMTYPE_DUALAXE					6		//"6.쌍도끼",
#define dITEMTYPE_ONEHANDBLUNT				7		//"7.한손둔기",
#define dITEMTYPE_TWOHANDBLUNT				8		//"8.양손둔기",
#define dITEMTYPE_DUALBLUNT					9		//"9.쌍둔기",
#define dITEMTYPE_DONPA					10		//"10.돈파",
#define dITEMTYPE_SHORTBOW					11		//"11.단궁",
#define dITEMTYPE_LONGBOW					12		//"12.장궁",
#define dITEMTYPE_DDABALBOW					13		//"13.연노",
#define dITEMTYPE_CROSSBOW					14		//"14.석궁",
#define dITEMTYPE_ARROW					15		//"15.화살",
#define dITEMTYPE_SPHERE					16		//"16.창",
#define dITEMTYPE_BONG					17		//"17.봉",
#define dITEMTYPE_STAFF					18		//"18.지팡이",
#define dITEMTYPE_WORRIOR_UP_ARMOR				19		//"19.전사갑옷(상의)",
#define dITEMTYPE_WORRIOR_LO_ARMOR				20		//"20.전사갑옷(하의)",
#define dITEMTYPE_ARCHER_UP_ARMOR				21		//"21.궁수갑옷(상의)",
#define dITEMTYPE_ARCHER_LO_ARMOR				22		//"22.궁수갑옷(하의)",
#define dITEMTYPE_WIZARD_UP_ARMOR				23		//"23.법사갑옷(상의)",
#define dITEMTYPE_WIZARD_LO_ARMOR				24		//"24.법사갑옷(하의)",
#define dITEMTYPE_WORRIOR_HELMET				25		//"25.전사투구",
#define dITEMTYPE_ARCHER_HELMET				26		//"26.궁수투구",
#define dITEMTYPE_WIZARD_HELMET				27		//"27.법사투구",
#define dITEMTYPE_WARRIOR_BELT				28		//"28.전사벨트",
#define dITEMTYPE_ARCHER_BELT				29		//"29.궁수벨트",
#define dITEMTYPE_WIZZARD_BELT				30		//"30.법사벨트",
#define dITEMTYPE_WARRIOR_GLOVE				31		//"31.전사장갑",
#define dITEMTYPE_ARCHER_GLOVE				32		//"32.궁수장갑",
#define dITEMTYPE_WIZZARD_GLOVE				33		//"33.법사장갑",
#define dITEMTYPE_WARRIOR_SHOES				34		//"34.전사신발",
#define dITEMTYPE_ARCHER_SHOES				35		//"35.궁수신발",
#define dITEMTYPE_WIZZARD_SHOES				36		//"36.법사신발",
#define dITEMTYPE_SHIELD					37		//"37.방패",
#define dITEMTYPE_NECKLACE					38		//"38.목걸이",
#define dITEMTYPE_RING					39		//"39.반지",
#define dITEMTYPE_JEWEL					40		//"40.보석",
#define dITEMTYPE_SCROLL					41		//"41.스크롤",
#define dITEMTYPE_MAGICSCROLL				42		//"42.마법스크롤",
#define dITEMTYPE_CONTAINER					43		//"43.가방(상자)",
#define dITEMTYPE_POTION					44		//"44.포션",
#define dITEMTYPE_OBJECT					45		//"45.오브젝트",
#define dITEMTYPE_FOOD					46		//"46.먹을것",
#define dITEMTYPE_ONETIME					47		//"47.일회용",


/////////////////////////////////////////////////////////////////
// L E V E L 1 0 0 I T E M
/////////////////////////////////////////////////////////////////
#define dBLACKNIGHTARMOR				10521
#define dBLACKNIGHTARMOR20				10541
#define dWITHEHANTTUNIC				10542
#define dWITHEHANTTUNIC20				10562
#define dMONOFROAD					10563
#define dMONOFROAD20					10583
#define dBLACKNIGHTPANT				10710		//블랙나이트팬츠	
#define dBLACKNIGHTPANT20				10730		//블랙나이트팬츠	
#define dWITHEHANTPANT				10731		//화이트헌터팬츠	
#define dWITHEHANTPANT20				10751		//화이트헌터팬츠	
#define dMONOFPANT					10752		//달의정령팬츠	
#define dMONOFPANT20					10772		//달의정령팬츠	
#define dBLACKNIGHTGUN				10899		//블랙나이트건틀렛
#define dBLACKNIGHTGUN20				10919		//블랙나이트건틀렛
#define dWITHEHANTGLAVE				10920		//화이트헌터글러브
#define dWITHEHANTGLAVE20				10940		//화이트헌터글러브
#define dMONOFGLAVE					10941		//달의정령장갑	
#define dMONOFGLAVE20				10961		//달의정령장갑	
#define dBLACKNIGHTBUT				11088		//블랙나이트부츠	
#define dBLACKNIGHTBUT20				11108		//블랙나이트부츠	
#define dWITHEHANTBUT				11109		//화이트헌터부츠	
#define dWITHEHANTBUT20				11129		//화이트헌터부츠	
#define dMONOFBUT					11130		//달의정령부츠	
#define dMONOFBUT20					11150		//달의정령부츠	
#define dBLACKNIGHTSILD				11235		//블랙나이트실드	
#define dBLACKNIGHTSILD20				11255		//블랙나이트실드	
#define dBLACKNIGHTBIGKEN				11298		//블랙나이트대거	
#define dBLACKNIGHTBIGKEN20				11318		//블랙나이트대거	
#define dBLACKNIGHTONEHANDSOLD			11361		//블랙나이트한손검
#define dBLACKNIGHTONEHANDSOLD20			11381		//블랙나이트한손검
#define dBLACKNIGHTTWOHANDSOLD			11424		//블랙나이트양손검
#define dBLACKNIGHTTWOHANDSOLD20			11444		//블랙나이트양손검
#define dBLACKNIGHTAX				11487		//블랙나이트도끼	
#define dBLACKNIGHTAX20				11507		//블랙나이트도끼	
#define dBLACKNIGHTMEICE				11550		//블랙나이트메이스
#define dBLACKNIGHTMEICE20				11570		//블랙나이트메이스
#define dBLACKNIGHTSPIER				11613		//블랙나이트스피어
#define dBLACKNIGHTSPIER20				11633		//블랙나이트스피어
#define dBLACKNIGHTSHOTBOW				11676		//화이트헌터숏보우
#define dBLACKNIGHTSHOTBOW20			11696		//화이트헌터숏보우
#define dWITHEHANTLONGBOW				11739		//화이트헌터롱보우
#define dWITHEHANTLONGBOW20				11759		//화이트헌터롱보우
#define dWITHEHANTTINBOW				11802		//화이트헌터석궁	
#define dWITHEHANTTINBOW20				11822		//화이트헌터석궁	
#define dMONOFSTAFF					11865		//달의정령스태프	
#define dMONOFSTAFF20				11885		//달의정령스태프	



#define IS_WEAPON( num ) \
	( ( num >= dITEMTYPE_ONEHANDSWORD && num <= dITEMTYPE_STAFF && num != dITEMTYPE_ARROW ) )

#define IS_ARMOR( num ) \
	( ( num >= dITEMTYPE_WORRIOR_UP_ARMOR && num <= dITEMTYPE_SHIELD ) )

#define IS_ACCESSORI( num ) \
	( ( num >= dITEMTYPE_NECKLACE && num <= dITEMTYPE_JEWEL ) )

/////////////////////////////////////////////////////////////////
// E Q U I P M E N T    T Y P E
/////////////////////////////////////////////////////////////////
#define dEQUIP_NECK					0				// 목걸이 
#define dEQUIP_BOW					1				// 활
#define dEQUIP_GLOVE					2				// 장갑
#define dEQUIP_WEAPON				3				// 무기 
#define dEQUIP_HELMET				4				// 머리 
#define dEQUIP_JACKET				5				// 상의
#define dEQUIP_BELT					6				// 벨트 
#define dEQUIP_PANTS					7				// 바지 
#define dEQUIP_EARRING				8				// 귀걸이 
#define dEQUIP_RING					9				// 반지
#define dEQUIP_SHIELD				10				// 방패
#define dEQUIP_SHOES					11				// 신발

extern sPITEM_DEF_INFO				g_ItemInfo[dMAX_ITEMINFO];

extern sPITEM_DATA g_itemMemory;


#define dHP_GAIN_L					90			//물약 회복설정 
#define dHP_GAIN_M					50
#define dHP_GAIN_S					30

#define dMP_GAIN_L					90
#define dMP_GAIN_M					50
#define dMP_GAIN_S					30

#define dOWNER_CHECK_TIME_LIMIT			10000			// 소유권 체크 만기시간 MS

////////////////////////////////////////////////////
// Item Attr Bit
////////////////////////////////////////////////////
#define dITEM_ATTR_NODROP			(1<<1)			// 버릴수 없음
#define dITEM_ATTR_NOGET			(1<<2)			// 줏을 수 없음
#define dITEM_ATTR_NODEAL			(1<<3)			// 거래불가
#define dITEM_ATTR_NOSELL			(1<<4)			// 팔 수 없음

////////////////////////////////////////////////////
// ITEM EVENT
////////////////////////////////////////////////////
#define dMAX_ITEM_EVENT				10

extern BOOL g_ItemEvent[dMAX_ITEM_EVENT];

#define dEVENT_ITEM1				10367			// 복주머니
#define dEVENT_SUB_ITEM1			10368			// 영화표
#define dEVENT_SUB_ITEM2			10369			// 10만원 상품권
#define dEVENT_SUB_ITEM3			10370			// 50만원 상품권
#define dEVENT_SUB_ITEM4			10371			// 100만원 상품권


// 각종 반지, 목걸이 번호

#define dAMULET_TRACKING			10398			// 트래킹 ( 위치 탐색 )		트래킹 아뮬렛
#define dAMULET_REINCARNATION		10399			// 리인카네이션 ( 부활 )		리인카네이션 아뮬렛
#define dAMULET_CALLGUILD			10400
#define dAMULET_INVISIBILITY		10401			// 인비저빌리티 ( 몬스터가 플레이어를 찾지 못함 )	인비저빌리티 아뮬렛
#define dAMULET_TRAMPING			10402			// 트램핑 ( 9% 확율로 상대방 스턴 )				트래핑 아뮬렛
#define dAMULET_CHANNELING			10403			// 전투시 33% 확율로 HP 대신 MP 감소				채널링 아뮬렛
#define dAMULET_LIONSENDURANCE		10409			// HP 회복률 11% 증가					라이온 엔듀어런스 아뮬렛(11%)
#define dAMULET_ARCANEMAGIC			10410			// 마나 회복률 11% 증가				아케인 매직 아뮬렛 (11%)
#define dRING_BARTERMASTERY			10415			// 상점에서 물건살때 5% 할인					바터 마스터리 링
#define dRING_HOMEWARD			10416			// 근접 마을로 이동								홈워드 링
#define dRING_WIZARDRY			10419			// 마법공격력 15% 증가						링 오브 위저드리
#define dRING_DISTURBANCE_1			10424			// 3% 확률 , 이동정지 ( 5초 )					
#define dRING_DISTURBANCE_2			10425			// 6% 확률 , 이동정지 ( 5초 )
#define dRING_DISTURBANCE_3			10426			// 9% 확률 , 이동정지 ( 5초 )
#define dRING_FEAR_1				10427			// 3% 확률 , 공격정지 ( 5초 )
#define dRING_FEAR_2				10428			// 6% 확률 , 공격정지 ( 5초 )
#define dRING_FEAR_3				10429			// 9% 확률 , 공격정지 ( 5초 )
#define dRING_BARRIER			10431			// 마법 저항력 11%증가					마나 베리어 링
#define dRING_FEAR_4				10432			// 9% 확률 , 공격정지 ( 5초 )



typedef struct
{
	BYTE page;
	BYTE y;
	BYTE x;
} sCHECK_INVEN, *sPCHECK_INVEN;

///////////////////////////////////////////////
// F U N C T I O N S
///////////////////////////////////////////////
BOOL LoadItemInfo();
void DeleteItemInfo();
sPITEM_DATA NewItem();
sPITEM_DATA CreateItem( int itemNum , char *commant, char *filename, int lineNum );

void MakeItemInPacket( sPITEM_DATA pItem , WORD mobIdx = 0 );
void MakeItemOutPacket( sPITEM_DATA pItem );
void GetItemSector( sPDESC_DATA pPlayer, int x, int y );
void GetItemVSector( int x, int startY, int endY, sPDESC_DATA pPlayer );
void GetItemHSector( int y, int startX, int endX, sPDESC_DATA pPlayer );
void OutItemSector( sPDESC_DATA pPlayer, int x, int y );
void OutItemVSector( int x, int startY, int endY, sPDESC_DATA pPlayer );
void OutItemHSector( int y, int startX, int endX, sPDESC_DATA pPlayer );
void SendToItemArea( sPITEM_DATA pItem, char *data, int size );
void GetCurrAreaItem( sPDESC_DATA pPlayer );
void OutCurrAreaItem( sPDESC_DATA pPlayer );

BOOL ItemToInventory( sPDESC_DATA pPlayer, sPITEM_DATA pItem , sPCHECK_INVEN pCheckInven );
BOOL ItemToInventory( sPDESC_DATA pPlayer, sPITEM_DATA pItem , BYTE invenPage, BYTE invenX, BYTE invenY );
void ItemFromInventory( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void ItemToMap( sPITEM_DATA pItem, int mapNum, int mapX, int mapY , WORD mobIdx = 0 );
void ItemFromMap( sPITEM_DATA pItem );
BOOL ItemToEquip( sPDESC_DATA pPlayer, sPITEM_DATA pItem , BYTE equipWhere );
BOOL ItemFromEquip( sPDESC_DATA pPlayer, sPITEM_DATA pItem );

sPITEM_DATA FindItemFromInvenByNum( sPDESC_DATA pPlayer, int itemNum );


sPITEM_DATA FindItemFromInven( sPDESC_DATA pPlayer, int idxNum );
sPITEM_DATA FindItemFromEquip( sPDESC_DATA pPlayer, int idxNum );
sPITEM_DATA FindItemMyArea( sPDESC_DATA pPlayer, int idxNum );
sPITEM_DATA FindItemFromGarbage( sPDESC_DATA pPlayer, int idxNum );
sPITEM_DATA FindItemFromInvenByPos( sPDESC_DATA pPlayer, BYTE page, BYTE invenX, BYTE invenY );
sPITEM_DATA FindItemFromSmith( sPDESC_DATA pPlayer, int idxNum );

BYTE GetEquipPosition( sPITEM_DATA pItem );
BYTE GetEquipPositionByNum( WORD itemNum );

void ClearPlayerItem( sPDESC_DATA pPlayer );

void SendPlayerItemInfo( sPDESC_DATA pPlayer );
void SendChangeEquip( sPDESC_DATA pPlayer, sPITEM_DATA pItem, BYTE io );

int GetMonsterItem( sPMOB_DATA pMob, char *ownerName , char *masterName, sPDESC_DATA pKiller );

BOOL CastItem( sPDESC_DATA pPlayer, sPITEM_DATA pItem );

sPITEM_DATA CreateMoney( int amount );

sPITEM_DATA FindItemFromDeal( sPDESC_DATA pPlayer, int idxNum );


void CheckItemLifeInMap();


BOOL CheckEmptyInven( sPDESC_DATA pPlayer , sPCHECK_INVEN pInven );
void FillInvenArray( sPDESC_DATA pPlayer, BYTE page, BYTE invenX, BYTE invenY );
void UnFillInvenArray( sPDESC_DATA pPlayer, BYTE page, BYTE invenX, BYTE invenY );
int CountEmptyInven( sPDESC_DATA pPlayer );

void UseHPPotion( sPDESC_DATA pPlayer, int itemNum );
void UseMPPotion( sPDESC_DATA pPlayer, int itemNum );
void UseEventPotion( sPDESC_DATA pPlayer , int itemNum );
void UseEnchantCardBox( sPDESC_DATA pPlayer, sPITEM_DATA pItem , int useCnt );

void SendUpdatePotionCnt( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void SendUpdateEnchantCnt( sPDESC_DATA pPlayer, sPITEM_DATA pItem );

BOOL CheckInvalidDealItem( sPDESC_DATA pPlayer ) ;

sPITEM_DATA FindPotionItem( sPDESC_DATA pPlayer, sPITEM_DATA pItem );

void OutCurrHouseItem( sPDESC_DATA pPlayer );
void GetCurrHouseItem( sPDESC_DATA pPlayer );

void ItemFromHouse( sPITEM_DATA pItem );
void ItemToHouse( sPITEM_DATA pItem, int mapNum, int mapX, int mapY );

sPITEM_DATA FindItemMyHouse( sPDESC_DATA pPlayer, int idxNum );

void ItemFromGarbage( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void ItemToGarbage( sPDESC_DATA pPlayer, sPITEM_DATA pItem );

void CheckItemValue( sPDESC_DATA pPlayer , sPITEM_DATA pItem );

void EventItemProcess( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void SendItemEventResult( sPDESC_DATA pPlayer, BYTE res );

void ChangeEquipItem( sPDESC_DATA pPlayer, sPITEM_DATA pItem, BYTE equipWhere );

BYTE LoadNeedLogTable();

BOOL CheckItemCommand( sPDESC_DATA pPlayer, char *command );
BOOL CheckEquipItem( sPDESC_DATA pPlayer, BYTE equipWhere, int itemNum );

void ITEMMGR_ItemToGarbage( char *ip, int itemNum, int cnt );

void SendPlayerGarbageMoney( sPDESC_DATA pPlayer );

void SendPlayerCashMoney( sPDESC_DATA pPlayer );

BOOL UseOtherPotion( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
BOOL UseOtherInvenItem( sPDESC_DATA pPlayer, int itemNum );
BOOL UseOtherInvenItemCnt( sPDESC_DATA pPlayer, int itemNum, int cnt );
BOOL UseOtherOneTimeItem( sPDESC_DATA pPlayer, sPITEM_DATA pItem );

void SendItemTimer( sPDESC_DATA pPlayer, int itemNum, int timer );

void ItemToSmith( sPDESC_DATA pPlayer, sPITEM_DATA pItem, int PlayerMoney );
int CheckEnchentItemtoSmith( sPITEM_DATA pItem );
int CheckEnchentItem( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
int	CheckEnchentCard( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void ItemFromSmith( sPDESC_DATA pPlayer, sPITEM_DATA pItem );


char *GetItemSerial();
void ClearSmith( sPDESC_DATA pPlayer );
BOOL	CheckItemLevel( sPDESC_DATA pPlayer, sPITEM_DATA pItem );

#endif





// EOF


