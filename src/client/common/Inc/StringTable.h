/**
 * ½ºÅ©¸µ Å×ÀÌºí.
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	ÆÄ¶õ~*
 * @date	$Date: 04-09-20 6:21p $
 * @version	$Revision: 29 $
 *
 * <HR>
 * $Log: /Common/Inc/StringTable.h $
 * 
 * 29    04-09-20 6:21p Paran2
 * 
 * 3     04-01-09 21:06 Sckyj				Add More String Using UI Button Script
 * 
 * 2     04-01-02 2:18p Paran				½ºÅ©¸µ Å×ÀÌºí Ãß°¡
 * 
 * <HR>
 *
 * @file	StringTable.h
 */


#ifndef __Common_StringTable_Header__
#define __Common_StringTable_Header__

#define STR_SYSERR_ERRORMESSAGE				"Error Message"
#define STR_SYSERR_WARNINGMESSAGE			"Warning Message"
#define STR_SYSERR_NOTENOUGHMEMORY1			"A fatal error - not enough memory.\nThere is not enough memory\navailable to run Laqia."
#define STR_SYSERR_NOTENOUGHMEMORY2			"Not enough memory.\nProgram may end abnormally"
#define STR_SYSERR_SERVERDISCONNECT			"Server connection disconnected"
#define STR_SYSERR_ALREADYCONNECTED			"This Id is already\nlogged on to sever."
#define STR_SYSERR_NOTCONNECTED				"·Î±×ÀÎ ÇÒ ¼ö ¾ø´Â ½Ã°£ ÀÔ´Ï´Ù."
#define STR_SYSERR_ERRUSER					"·Î±×ÀÎÇÒ ¼ö ¾ø½À´Ï´Ù."
#define STR_SYSERR_RUNFORWEB				"Please run at the website"
#define STR_SYSERR_IPINDEX					"Server IP index error"
#define STR_SYSERR_NOTCONNECTLOGINSERVER	"Cannot connect to Login sever"
#define STR_SYSERR_NOREPRYLOGIN				"Connection has been failed due to the time out with login sever"
#define STR_SYSERR_NOTCONNECTLAQIA			"[0x00]Can not log in Deicide"
#define STR_SYSERR_NOACCEPTLAQIA			"[0x01]Please agree with Deicide's terms of agreement"
#define STR_SYSERR_NOTFOUNDIDLAQIA			"[0x02]This Id does not exist in Deicide"
#define STR_SYSERR_NOTCONNECTSERVER			"Can't connect to Sever"

#define STR_SYSMSG_RETRYINPUTID				"This Id does not exist"
#define STR_SYSMSG_RETRYINPUTPASSWORD		"Incorrect Password"
#define STR_SYSMSG_INPUTID					"Enter your ID"
#define STR_SYSMSG_INPUTPASSWORD			"Enter your Password"
#define STR_SYSMSG_LOGINING					"Login process..."
#define STR_SYSMSG_EXCEPTSTRING				"You can't use these strings"
#define STR_SYSMSG_SCREENSHOT				"Screenshot"
#define STR_SYSMSG_ACCOUNTBLOCK				"Your account has been blocked"

#define STR_CHARSEL_DELETE					"Delete?"
#define STR_CHARSEL_EXCEPTNAME				"This name cannot be used"
#define STR_CHARSEL_EXISTNAME				"Other user is already using this name"
#define STR_CHARSEL_FAILDELETE				"Fail to delete the character"
#define STR_CHARSEL_LEFTPOINT				"Please use all point available"

#define STR_INVENERR_NOTENOUGHSPACE			"Not enough inventory space!"
#define STR_INVENERR_NOTENOUGHGOLD			"Not enough Shild!"
#define STR_INVENERR_NOMATCHGOLD			"Not enough Shild!"

#define STR_DEAL_REQUESTFROM				" has\nasked you a trade.\nAccept the trade?\n"
#define STR_DEAL_SUBMIT						"Complete trade"
#define STR_DEAL_FINISH						"Trade ended"

#define STR_PARTY_CREATE					"Party created!"
#define STR_PARTY_DESTROY					"Party disbanded!"
#define STR_PARTY_NOCHIEF					"»ó´ë°¡ ÆÄÆ¼ÀåÀÌ ¾Æ´Õ´Ï´Ù!"
#define STR_PARTY_MEMBEROVER				"The Party is full!"
#define STR_PARTY_SUCCESS					"You've joined the party!"
#define STR_PARTY_ADDMEMBER					" has\njoined the party"
#define STR_PARTY_DELETEMEMBER				" has\nleft the party."
#define STR_PARTY_REQUESTINVIT				" has asked\nyou to join the party. Accept?"
#define STR_PARTY_BANISH					"\nremove this user from your party?"

#define STR_MYSHOP_OPEN						"Start selling"
#define STR_MYSHOP_CLOSE					"Stop selling"

#define STR_GUILD_INVITE					"has asked you\nto join the guild. Accept?"

#define STR_FRIEND_NOTFRIEND				"Cannot register to your friend list."

#define	STR_UIMSG_DOOR						"Door"
#define	STR_UIMSG_DEAL						"Trade"
#define STR_UIMSG_PARTY						"Party"
#define STR_UIMSG_TALK						"Chat"
#define STR_UIMSG_SHOP						"Shop"
#define STR_UIMSG_QUEST						"Quest"
#define STR_UIMSG_STOREHOUSE				"Warehouse"
#define STR_UIMSG_GOTODEALMAP				"Trade Map"
#define STR_UIMSG_CREATEGUILD				"Create Guild"
#define STR_UIMSG_RELEASEGUILD				"Disband Guild"
#define STR_UIMSG_INPUTGUILDNAME			"- Type your guild name -"
#define STR_UIMSG_SELECTWEAPONITEM			"- Select your weapon -"
#define STR_UIMSG_SELECTARMORITEM			"- Select your armor -"
#define STR_UIMSG_SELECTOPTIONITEM			"- Select your option item -"
#define STR_UIMSG_AGAINENCHANTWEAPON		"- Try to enchant your weapon again -"
#define STR_UIMSG_AGAINENCHANTARMOR			"- Try to enchant your Armor again -"
#define STR_UIMSG_AGAINENCHANTOPTION		""
#define STR_UIMSG_ENCHANTCONFIRM			"Start a enchantment"
#define STR_UIMSG_ENCHANTSUCCESS			"- Enchant succeeded -"
#define STR_UIMSG_ENCHANTFAIL				"- Enchant failed -"
#define STR_UIMSG_NOTRIDER_HOUSE			"- You can't enchant while riding a horse -"
#define STR_UIMSG_DROPITEM					"Do you want to drop this?"
#define STR_UIMSG_INPUTNAME					"Enter your name!"
#define STR_UIMSG_GUILDNAMELENGTH			"Guild name should be under 32 character"
#define STR_UIMSG_ALREADYDEAL				"This user is already in trade with other user."
#define STR_UIMSG_PETON						"Use"
#define STR_UIMSG_PETOFF					"Hide"
#define STR_UIMSG_GUILDZZANG				"Master"
#define STR_UIMSG_GUILDBUZZANG				"Vice Master"
#define STR_UIMSG_ALREADYPSHOP				"This user is inside usershop."
#define STR_UIMSG_FREEPKMODE				"Free PK"
#define STR_UIMSG_NONEPKMODE				"Non PK"
#define STR_UIMSG_PARTYPKMODE				"Party PK"
#define STR_UIMSG_GUILDPKMODE				"Guild PK"
#define STR_UIMSG_ONCEINPUTPRICE			"1È¸ ÀÔ·Â°¡´É ±Ý¾× 100,000,000(1¾ï)"
#define STR_UIMSG_ENCHANTITEM				"This is a Enchanted Item."

#define STR_UIASK_CREATEGUILD				"Create Guild"
#define STR_UIASK_RELEASEGUILD				"Disband Guild"

#define STR_MAY_HELPER						"Guide 'MAY' will follow you, until you reached total level 10."
#define STR_PET_TYPE1						"Electrical element"
#define STR_PET_TYPE2						"Ice element"
#define STR_PET_TYPE3						"Flame element"

#define STR_SKILL_ALREADY					"This skill has activated."
#define STR_SKILL_CHANGESHORTORLONG			"Change your main skill into Close/Ranger attack skill."
#define STR_SKILL_NOTFOUNDTARGET			"This skill has not targeted"
#define STR_SKILL_NOTUSINGTARGET			"You can't use this skill to this target."
#define STR_SKILL_NOTUSINGWEAPON			"You can't use this skill with your weapon."
#define STR_SKILL_NOTUSINGHORSE				"You can't use magic skill while riding a horse."
#define STR_SKILL_LVUP_SHORT				"±Ù°Å¸®·¹º§¾÷"
#define STR_SKILL_LVUP_LONG					"¿ø°Å¸®·¹º§¾÷"
#define STR_SKILL_LVUP_BLACK				"Èæ¸¶¹ý·¹º§¾÷"
#define STR_SKILL_LVUP_WHITE				"¹é¸¶¹ý·¹º§¾÷"
/*#define STR_SKILL_LVUP_SHORT				"Close Attack level-up"
#define STR_SKILL_LVUP_LONG					"Ranger Attack level-up"
#define STR_SKILL_LVUP_BLACK				"Dark Magic level-up"
#define STR_SKILL_LVUP_WHITE				"White Magic level-up"*/

#define STR_CHATT_NOTICE					"Notice"
#define STR_CHATT_FAILTRANSMIT				"Whisper delivery failure"

#define STR_ITEMINFO_TWOHANDWEAPON			"Two-handed weapon"
#define STR_ITEMINFO_COMMONCLASS			"Normal"
#define STR_ITEMINFO_WARRIORCLASS			"Close Attack Class"
#define STR_ITEMINFO_ARCHERCLASS			"Range Attack Class"
#define STR_ITEMINFO_MAGICCLASS				"Magical Class"
#define STR_ITEMINFO_ATTACKSPEED			"Attack speed"
#define STR_ITEMINFO_TOOSLOWSPEED			"Very slow"
#define STR_ITEMINFO_SLOWSPEED				"Slow"
#define STR_ITEMINFO_NORMALSPEED			"Normal"
#define STR_ITEMINFO_FASTSPEED				"Fast"
#define STR_ITEMINFO_VERYFASTSPEED			"Very fast"
#define STR_ITEMINFO_ATTACKPOWER			"Attack :"
#define STR_ITEMINFO_ARMORPOWER				"Defense"
#define STR_ITEMINFO_ATTACKDIST				"Attack Range"
#define STR_ITEMINFO_MAGICPOWER				"Magic power"
#define STR_ITEMINFO_WEIGHT					"Weight"
#define STR_ITEMINFO_SELLPRICE				"Selling price"
#define STR_ITEMINFO_BUYPRICE				"Purchase price"
#define STR_ITEMINFO_SHILD					"Shild"
#define STR_ITEMINFO_NEEDMP					"Mana drain"
#define STR_ITEMINFO_NEEDTOTLEV				"Req. level"

#define STR_ITEMINFO_SEARCHUSER				"Search user"
#define STR_ITEMINFO_RESURRECTUSER			"esurrect Dead user"
#define STR_ITEMINFO_TRANSPANRENTEFFECT		"Invisible effect"
#define STR_ITEMINFO_STUNEFFECT9PERCENT		"9% Chance Stun effect"
#define STR_ITEMINFO_CHANGEDAMAGE			"Damage Change"
#define STR_ITEMINFO_STRENGTH2PLUS			"Á¦Á¿+2"
#define STR_ITEMINFO_STRENGTH5PLUS			"Á¦Á¿+5"
#define STR_ITEMINFO_HPGAIN11PERCENTPLUS	"Hp Recovery +11%"
#define STR_ITEMINFO_MPGAIN11PERCENTPLUS	"MP Recovery +11%"
#define STR_ITEMINFO_SHOPITEMDC5PERCER		"5% discount shop-items"
#define STR_ITEMINFO_COMBACKVILLAGE			"Town Portal"
#define STR_ITEMINFO_MAGICDAMAGE15PERCENT	"Magic damage +15%"
#define STR_ITEMINFO_MOVEOFF6PERCENT		"6% Chance stop movement"
#define STR_ITEMINFO_MOVEOFF9PERCENT		"9% Chance stop movement"
#define STR_ITEMINFO_ATTACKOFF6PERCENT		"6% Chance stop attack"
#define STR_ITEMINFO_ATTACKOFF9PERCENT		"9% Chance stop attack"
#define STR_ITEMINFO_MAGICRESIST11PERCENT	"+11% Magic resistance"

#define STR_EVENTMSG_CONGRATULATION			"¹§Ï²»ñ½±"
#define STR_EVENTMSG_NOTHING				""
#define STR_EVENTMSG_HANDPHONE				"²ÊÆÁÊÖ»ú"
#define STR_EVENTMSG_DIGCAMERA				"ÊýÂëÏà»ú"
#define STR_EVENTMSG_TV						"²ÊÉ«µçÊÓ»ú"
#define STR_EVENTMSG_LCD					"Òº¾§ÏÔÊ¾Æ÷"
#define STR_EVENTMSG_MP3					"MP3"

// May String
#define STR_MAY_MAY							"MAY"

#define STR_SKILL_ERROR						"It not correct skill. Please close this program."
#define STR_FRIEND_REQUESTINVIT				" has asked\nYou to Join the Friend. Accpet?"


// Go Away Message
#define STR_MAY_GOAWAY1						"Leave"
#define STR_MAY_GOAWAY2						"Be gone"
#define STR_MAY_GOAWAY3						"Go away"
#define STR_MAY_GOAWAY4						"Leave me alone"
#define STR_MAY_GOAWAY5						"Just leave"
#define STR_MAY_GOAWAY6						"Good bye"
#define STR_MAY_GOODBYE						"See you soon~"
// ~Go Away Message

// Show Message
#define STR_MAY_SHOW1						"Come out"
#define STR_MAY_SHOW2						"Be here !!"
#define STR_MAY_SHOW3						"Help me out"
#define STR_MAY_SHOW						"Yahoo~"
// ~Show Message
// ~May String

// Fishing Hook
#define STR_FISHHOOK_DROWTOWATER			"Throw your fishing rod to the water."
#define STR_FISHHOOK_HOOKING				"Pull your fishig rod!"
// ~Fishing Hook

#define STR_PRIMIEM_ZONE					"Premium zone"

#endif //__Common_StringTable_Header__