/**
 *	$Workfile: db_rows.h $
 *
 *	Copyright (c) 2003 ∆¿∏ﬁ¿Ã, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 14 $
 *	$Date: 04-10-29 11:56p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/db_rows.h $
 * 
 * 14    04-10-29 11:56p Sk8snow
 * 
 * 13    04-09-08 3:54p Sk8snow
 * 
 * 12    04-08-04 9:27a Sk8snow
 * 
 * 11    04-07-13 7:26a Sk8snow
 * 
 * 10    04-05-29 10:12p Sk8snow
 * 
 * 9     04-05-21 11:37a Sk8snow
 * 
 * 8     04-04-03 4:03p Sk8snow
 * 
 * 7     04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __DB_ROWS_H__
#define __DB_ROWS_H__

////////////////////////////////////////////////////
// M O B    I N F O 
////////////////////////////////////////////////////
enum
{
	MOBDATA_IDX = 0,
	MOBDATA_LEVEL,
	MOBDATA_MOBNUM,
	MOBDATA_HNAME,
	MOBDATA_ENAME,
	MOBDATA_DP,
	MOBDATA_AP,
	MOBDATA_HP,
	MOBDATA_MP,
	MOBDATA_DEX,
	MOBDATA_STR,
	MOBDATA_INT,
	MOBDATA_MINMONEY,
	MOBDATA_MAXMONEY,
	MOBDATA_EXP,
	MOBDATA_ATTACKRANGE,
	MOBDATA_VISIONRANGE,
	MOBDATA_ATTACKSPEED,
	MOBDATA_MOVESPEED,
	MOBDATA_MOBATTR,
	MOBDATA_DEFATTR,
	MOBDATA_MOVESPEED2
};

////////////////////////////////////////////////////
// N P C    I N F O 
////////////////////////////////////////////////////
#define NPC_IDX					0
#define NPC_NUM					1
#define NPC_HNAME					2
#define NPC_ENAME					3
#define NPC_MAPNUM					4
#define NPC_POSX					5
#define NPC_POSY					6
#define NPC_ISSHOPKEEPER				7
#define NPC_SHOPNUM					8

////////////////////////////////////////////////////
// I T E M    I N F O 
////////////////////////////////////////////////////
enum 
{		
		ITEMDATA_IDXNUM	 = 0,
		ITEMDATA_ITEMNUM,
		ITEMDATA_LEVEL,
		ITEMDATA_HNAME,
		ITEMDATA_ENAME,
		ITEMDATA_PRICE,
		ITEMDATA_SELLPRICE1,
		ITEMDATA_SELLPRICE2,
		ITEMDATA_SELLPRICE3,
		ITEMDATA_MINDAMAGE,
		ITEMDATA_MAXDAMAGE,
		ITEMDATA_DEFENSE,
		ITEMDATA_NEEDSTR,
		ITEMDATA_NEEDINT,
		ITEMDATA_NEEDDEX,
		ITEMDATA_NEEDCHA,
		ITEMDATA_NEEDSATTACK,
		ITEMDATA_NEEDLATTACK,
		ITEMDATA_NEEDBMAGIC,
		ITEMDATA_NEEDWMAGIC,
		ITEMDATA_ITEMTYPE,
		ITEMDATA_ATTACKATTR,
		ITEMDATA_ARMORATTR,
		ITEMDATA_STRPLUSE,
		ITEMDATA_DEXPLUSE,
		ITEMDATA_INTPLUSE,
		ITEMDATA_HPPLUSE,
		ITEMDATA_MPPLUSE,
		ITEMDATA_STRMINUS,
		ITEMDATA_DEXMINUS,
		ITEMDATA_INTMINUS,
		ITEMDATA_HPMINUS,
		ITEMDATA_MPMINUS,
		ITEMDATA_SATTACKPLUSE,
		ITEMDATA_LATTACKPLUSE,
		ITEMDATA_BMAGICPLUSE,
		ITEMDATA_WMAGICPLUSE,
		ITEMDATA_SATTACKMINUS,
		ITEMDATA_LATTACKMINUS,
		ITEMDATA_BMAGICMINUS,
		ITEMDATA_WMAGICMINUS,
		
		ITEMDATA_IMGFILE1,
		ITEMDATA_IMGFILE2,
		ITEMDATA_IMGFILE3,
		ITEMDATA_IMGFILE4,
		ITEMDATA_IMGFILE5,

		ITEMDATA_ISHAVESKIN,
		ITEMDATA_EXATTR,
		ITEMDATA_MAGICKIND,
		ITEMDATA_MAGICCASTTYPE,
		ITEMDATA_MAGICAREATYPE,
		ITEMDATA_MAGICCASTRANGE,
		ITEMDATA_MAGICDELAYTIME,
		ITEMDATA_MAGICMINLEV,
		ITEMDATA_MAGICMAXLEV,
		ITEMDATA_MAGICMINDAMAGE,
		ITEMDATA_MAGICMAXDAMAGE,
		ITEMDATA_MAGICMINEXDAMAGE,
		ITEMDATA_MAGICMAXEXDAMAGE,
		ITEMDATA_MAGICMINNEEDMP,
		ITEMDATA_MAGICMAXNEEDMP,
		ITEMDATA_MAGICCASTTARGET,
		
		ITEMDATA_NEEDSEX,
		ITEMDATA_ATTACKSPEED
};


////////////////////////////////////////
// M O B  I N V E N T O R Y
////////////////////////////////////////
enum
{
	MOBINVEN_IDX	= 0,
	MOBINVEN_MOBNUM,
	MOBINVEN_ITEMNUM,
	MOBINVEN_DICENUM,
	MOBINVEN_DICESIZE,
	MOBINVEN_CHECKNUM,
	MOBINVEN_ISUP
};


////////////////////////////////////////
// M O B    R E S E T 
////////////////////////////////////////
#define MOB_RESET_IDX				0
#define MOB_RESET_MOBNUM				1
#define MOB_RESET_MAPNUM				2
#define MOB_RESET_POSX				3
#define MOB_RESET_POSY				4

////////////////////////////////////////
// P L A Y E R S  I T E M
////////////////////////////////////////
enum
{
	INVEN_IDX	=	0,
	INVEN_ITEMNUM,
	INVEN_PAGE,
	INVEN_X,
	INVEN_Y,
	INVEN_OWNERID,
	INVEN_OWNERPOS,
	INVEN_LIFE,
	INVEN_EXVALUE1,
	INVEN_EXVALUE2,
	INVEN_EXVALUE3,
	INVEN_SERIAL,
};

////////////////////////////////////////
// P L A Y E R S  E Q U I P
////////////////////////////////////////
enum
{
		EQ_IDX			= 0,
		EQ_ITEMNUM		,
		EQ_POS			,
		EQ_LIFE		,
		EQ_OWNERID		,
		EQ_OWNERPOS		,
		EQ_SERIAL,
};

////////////////////////////////////////
// P L A Y E R   S K I L L
////////////////////////////////////////
enum
{
	PLAYERSKILL_IDX		=	0,
	PLAYERSKILL_OWNERID,
	PLAYERSKILL_OWNERPOS,
	PLAYERSKILL_SKILLNUM
};


////////////////////////////////////////
// S H O P   I N F O
////////////////////////////////////////
enum
{
	SHOPDATA_IDX = 0,
	SHOPDATA_SHOPNUM,
	SHOPDATA_ITEMNUM
};


////////////////////////////////////////
// M O B   R E S E T  I N F O
////////////////////////////////////////
enum
{
	MOBRESET_IDX = 0,
	MOBRESET_MAPNUM,
	MOBRESET_MOBNUM,
	MOBRESET_POSX,
	MOBRESET_POSY
};

////////////////////////////////////////
// Q U E S T  I N F O
////////////////////////////////////////
enum
{
	QUESTINFO_IDX = 0,
	QUESTINFO_OWNERID,
	QUESTINFO_OWNERPOS,
	QUESTINFO_QUESTNUM,
	QUESTINFO_QUESTSTATE
};

////////////////////////////////////////
// G A R B A G E    I N F O 
////////////////////////////////////////
enum
{
	GARBAGE_IDX = 0,
	GARBAGE_OWNERID,
	GARBAGE_ITEMNUM,
	GARBAGE_LIFE,
	GARBAGE_EXVALUE1,
	GARBAGE_EXVALUE2,
	GARBAGE_EXVALUE3,
	GARBAGE_SERIAL,
};


////////////////////////////////////////
// N P C  I N F O
////////////////////////////////////////

enum
{
	NPCDATA_IDX = 0,
	NPCDATA_MAPNUM,
	NPCDATA_NPCNUM,
	NPCDATA_ISGARBAGE,
	NPCDATA_SHOPNUM,
	NPCDATA_QUESTNUM
};

///////////////////////////////////////
// Q U E S T   I N F O
///////////////////////////////////////
enum
{
	QUESTDATA_IDX = 0,
	QUESTDATA_NUM,
	QUESTDATA_NAME,
	QUESTDATA_BEGIN_LEVEL,
	QUESTDATA_BEGIN_SATTACK,
	QUESTDATA_BEGIN_LATTACK,
	QUESTDATA_BEGIN_BMAGIC,
	QUESTDATA_BEGIN_WMAGIC,
	QUESTDATA_BEGIN_STR,
	QUESTDATA_BEGIN_INT,
	QUESTDATA_BEGIN_DEX,
	QUESTDATA_BEGIN_CHA,
	QUESTDATA_BEGIN_ITEM1,
	QUESTDATA_BEGIN_ITEM2,
	QUESTDATA_BEGIN_ITEM3,
	QUESTDATA_BEGIN_MONEY,

	QUESTDATA_END_LEVEL,
	QUESTDATA_END_SATTACK,
	QUESTDATA_END_LATTACK,
	QUESTDATA_END_BMAGIC,
	QUESTDATA_END_WMAGIC,
	QUESTDATA_END_STR,
	QUESTDATA_END_INT,
	QUESTDATA_END_DEX,
	QUESTDATA_END_CHA,
	QUESTDATA_END_ITEM1,
	QUESTDATA_END_ITEM2,
	QUESTDATA_END_ITEM3,
	QUESTDATA_END_MONEY,
	QUESTDATA_PREV_QUEST,
	QUESTDATA_NEXT_QUEST
};

enum
{
	GUILDDATA_IDX = 0,
	GUILDDATA_NUM,
	GUILDDATA_MASTERNAME,
	GUILDDATA_MEMBERCNT,
	GUILDDATA_TYPE,
	GUILDDATA_NAME,
	GUILDDATA_MARK,
	GUILDDATA_POINT
};


enum
{
	PETDATA_IDX = 0,
	PETDATA_OWNERID,
	PETDATA_OWNERPOS,
	PETDATA_TYPE,
	PETDATA_LEVEL,
	PETDATA_HP,
	PETDATA_MP,
	PETDATA_EXP,
	PETDATA_STATE,
	PETDATA_ANGERGAGE,
	PETDATA_NAME
};

enum
{
	MAPOWNERDATA_IDX = 0,
	MAPOWNERDATA_MAPNUM,
	MAPOWNERDATA_GUILDNUM,
	MAPOWNERDATA_TAXVALUE,
	MAPOWNERDATA_TAXBANK
};

enum
{
	WARDATA_IDX	=	0,
	WARDATA_OWNERGUILD,
	WARDATA_OWNERPOINT,
	WARDATA_ENEMYGUILD,
	WARDATA_TARGETMAPNUM,
	WARDATA_TYPE
};

enum
{
	GARBAGE_MONEY_IDX = 0,
	GARBAGE_MONEY_OWNERID,
	GARBAGE_MONEY_AMOUNT
};

#endif




