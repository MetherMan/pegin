/**
 * ±ÝÄ¢ ´Ü¾î
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	·Î³Ä
 * @date	$Date: 04-09-25 2:02p $
 * @version	$Revision: 10 $
 *
 * <HR>
 * $Log: /UInterface/Src/NotUseWord.cpp $
 * 
 * 10    04-09-25 2:02p Paran2
 * 
 * 9     04-09-20 2:23p Sk8snow2
 * 
 * 8     04-09-18 4:42p Paran
 * 
 * 7     04-06-08 3:56p Paran2
 * 
 * 6     04-06-08 12:12p Kjmgo
 * 
 * 5     04-05-29 10:07p Paran
 * 
 * 4     04-05-14 4:05p Paran
 * 
 * 3     04-05-10 11:13a Paran2
 * 
 * 2     04-02-03 6:07p Paran
 * 
 * <HR>
 *
 * @file	NotUseWord.cpp
 */

#include "NotUseWord.h"
#include <string>

#ifdef _CHINAVERSION
#define MAX_WORD 201
#define MAX_AWORD	137
#define MAX_AID		65
#else
#define MAX_WORD 161
#endif


namespace
{
#ifndef _CHINAVERSION
	char*		aWord[MAX_WORD]	=
	{
		"¶óÅ°¾Æ",		"laqia",		"¿î¿µÀÚ",		"ÆÀ¸ÞÀÌ",			"Á÷¿ø",

		"teammay",		"ÀÌÀçº¹",		"±èÀçÇü",		"Á¶À±±Ù",			"°³¹ßÀÚ",
		"Ä«¹«¿¤",		"¿äÇÇ¿¤",		"ÇÏ½Ã¸»",		"ÀÚµåÅ°¿¤",			"¹Â¸®¿¤",

		"¹ÌÄ«¿¤",		"¶óÆÄ¿¤",		"¹Ùºñ¿¤",		"¿ìÁö¿¤",			"Æä¸®¿¤",
		"°¡ºê¸®¿¤",		"¿¡¸£Åä½Ã",		"»ç¸¶¿¤",		"´ÙÀ½°ÔÀÓ",			"daumgame",			

		"¾Æ³ª¿¤",		"»þÆ¼¿¤",		"°¨ºñ¿¤",		"¹ßÅ°¿¤",			"¸»Å°µð¿¤",
		"ÆÈºêÀ£",		"¿¥ºê¸®¿¤",		"º£¸£Å°¿¤",		"ÇÏ¸¶¸®¿¤",			"ÁÖ¸®¿¤",	
		
		"¾ÆµÎ¾ÆÅ°¿¤",	"ÇÏ³×ºê",		"¸ÞÅ¸Æ®·Ð",		"³ª´Ù´Ï¿¤",			"»çÅº", 
		"¿ÀÆÄ´Ï¿¤",		"¶óÅ©ºñ¿¤",		"¿À¸®ÇÇ¿¤",		"¾ßºêÅ°¿¤",			"ÀÚÇÇ¿¤", 
		
		"ÇÏ¹Ì¿¤",		"Ä«¸¶¿¤",		"·¹Äû¿¤",		"¼Î¸£ºñ¿¤",			"¿ì¸®¿¤",
		"¾¾ÆÈ",			"¾ºÆÈ",			"¾¯ÆÈ",			"¾¾ÆÞ",				"¾¯ÆÞ",
		
		"¾¾¹ß",			"¾º¹ß",			"¾¾¹ú",			"¾º¹ú",				"¾¾¹ë",
		"¶Ç¶óÀÌ",		"¶Ê¾ÆÀÌ",		"¾¾´ó",			"º´½Å",				"º´½®",
		
		"ºù½Å",			"ºù½Ã",			"µî½Å",			"µî½®",				"¸ÛÃ»ÀÌ",
		"¸ÛÃæÀÌ",		"»¶Å¥",			"ºýÅ¥",			"»¶±Ô",				"ºý±Ô",
		
		"»õ³¢",			"½¬³¢",			"¼¼³¢",			"»õ²î",				"½¦¸®",
		"¼½±â",			"½§±â",			"µÚÁ®",			"µØÁ®",				"µðÁö",
		
		"´Ï¹Ì¶ö",		"Áö¶ö",			"Áö´Þ",			"Áã¶ö",				"Áö·²",
		"Áö·ê",			"Áö·Ñ",			"¹ÌÄ£",			"¹¿Ä£",				"¹ÌÃì",
		
		"Á¸³ª°Ô",		"Á½±î",			"ÁÀ±î",			"Á¿±î",				"Ã¢³à",
		"Ã¢³â",			"´Ï¾Ö¹Ì",		"´Ï¾Öºñ",		"¾Ã³â",				"¾Ã³ð",
		
		"½à³â",			"½à³ð",			"Àâ³â",			"Àâ³ð",				"°³³â",
		"°³³ð",			"È£·Î",			"½ÊÀÚ¾³",		"Çã¹úÃ¢",			"ºÎ¶ö",
	
		"¾ÃÅÊ",			"½±»ý±¸¸®",		"´Ï±â¹Ì",		"°³»õ³¢",			"°³ÈÄ¶óÁú",
		"°³ÀÚ½Ä",		"gm",			"°ü¸®ÀÚ",		"´ÙÀ½",				"daum",
		"", "\\", "/",

		/*"game master",		"master", "¸¶½ºÅÍ",		"°ÔÀÓ¸¶½ºÅÍ", "Áö¿¥",
		"µµ¿ì¹Ì",			"µµ¿òÀÌ",		"helper", "angel",	
		"µð¿Â",			"Dion",			"ÀÌ¸®½º",			"Iris",
		"Å°¶ó",			"Kira",			"µ¥¹Ì¿Â",		"Demion",			"Äùµå",
		
		,"¼öÈ£Ãµ»ç",	"¿£Á©",	"daum°ÔÀÓ",		"´ÙÀ½game","Ãµ»ç",
		"Qued","Å×½ºÆ®","Test",*/

	}; //aWord
#else
	char*		aWord[MAX_AWORD]	=
	{  
		"bastard", "wetback", "faggot", "nigger", "nipple", 		
		"vagina", "Nigger", "nIgger", "niGger", "niggeR", 

		"NIgger", "NiGger", "NigGer", "NiggEr", "NiggeR", 		
		"nIGger", "nIgGer", "nIggEr", "nIggeR", "niGGer", 
				
		"NIGger", "NIgGer", "nIGgER", "pussy", "penis", 
		"bitch", "moran", "fucka", "ahole", "nigga", 
		
		"Bitch", "bItch", "biTch", "bitCh", "bitcH",
		"BItch", "BiTch", "BitCh", "BitcH", "bITch", 
		
		"bItCh", "bItcH", "biTCh", "biTcH", "bitCH",
		"BITch", "BItCh", "BItcH", "BITCH", "suck", 
		
		"cunt", "dick", "rape", "snob", "mtfk", 
		"puto", "damn", "stfu", "cock", "nuts", 
		
		"fUck", "fuCk", "fucK", "FUck", "FuCk", 
		"FucK", "fUCk", "fUcK", "fuCK", "FUCk", 
		
		"FUcK", "FuCK", "fUCK", "FUCK", "Damn", 
		"dAmn", "daMn", "damN", "DAmn", "DaMn", 
		
		"DamN", "dAMn", "dAmN", "daMN", "DAMn", 
		"DAmN", "DaMN", "DAMN", "Cock", "cOck", 
		
		"coCk", "cocK", "COck", "CoCk", "CocK", 
		"cOCk", "cOcK", "coCK", "COCk", "COcK", 
		
		"cOCK", "COCK", "Shit", "sHit", "shIt", 
		"shiT", "SHit", "ShIt", "ShiT", "sHIt", 
		
		"sHiT", "shIT", "SHIt", "SHiT", "sHIT", 
		"SHIT", "Suck", "sUck", "suCK", "SUck", 
		
		"SUCk", "SUCK", "fuck", "Fuck", "sex", 
		"Sex", "SEx", "SeX", "sEX", "SEX",
		
		"niGgEr", "niGgeR", "nigGEr", "nigGeR", "niggER", 
		 "sonofbitch", "",
	}; //aWord

	char*		aID[MAX_AID]	=			{

		"admin",	"ADMIN",	"Admin",	"aDmin",	"adMin", 
		"admIn",	"admiN",	"ADMIn",	"ADMiN",	"ADmIN", 

		"AdMIN",	"aDMIN",	"AdMiN",	"aDmIn",	"Deicide", 
		"DEICIDE",	"deicide",	"dEicide" ,	"deIcide",	"deiCide",

		"deicIde",	"deiciDe",	"deicidE",	"master" ,	"MASTER",		
		"Master",	"mAster",	"maSter",	"masTer",	"mastEr",

		"masteR",	 "GM",		"gm",		"ongameport","ONGAMEPORT", 
		"Ongameport", "oNgameport",	"onGameport",	"ongAmeport",	"ongaMeport",

		"ongamEport", "ongamePort",	"ongamepOrt",	"ongamepoRt",	"ongameporT",
		"helper",	"HELPER",	"Helper",	"hElper",	"heLper",

		"helPer",	"helpEr",	"helpeR",	"moderator", "MODERATOR", 
		"Moderator", "mOderator", "moDerator", "modErator", "modeRator", 
		
		"moderAtor", "moderaTor", "moderatOr", "moderatoR",	"",
	};
#endif

	/**
	 *
	 */
	void SetStrLwr( const char* szWord, char* szLwr )
	{
		int nLen = strlen( szWord );

		for( int i = 0 ; i < nLen ; i++ )
		{
			char bt = szWord[i];

			if( bt >= 65 && bt <= 90 )
			{
				bt += 32;
			} //if

			szLwr[i] = bt;
		} //for

		szLwr[nLen] = 0;
	} //SetStrLwr
	
	//char	
} //namespace


/**
 * ±ÝÄ¢´Ü¾î¸¦ °Ë»çÇÕ´Ï´Ù.
 */
BOOL CheckWord( const char* szWord )
{
	static char szLwr[256];
	SetStrLwr( szWord, szLwr );

	std::string str( szLwr );
	
	for( int i = 0 ; i < MAX_AWORD ; i++ )
	{
		if( *aWord[i] == '\0' )
		{
			return TRUE;
		} //if

		int n = str.find( aWord[i] );

		if( n != -1 )
		{
			return FALSE;
		} //if
	} //for

	return TRUE;
} //CheckWord

BOOL CheckUseID( const char* szWord )
{
	static char szLwr[256];
	SetStrLwr( szWord, szLwr );

	std::string str( szLwr );
	
	for( int i = 0 ; i < MAX_AID ; i++ )
	{
		if( *aID[i] == '\0' )
		{
			return TRUE;
		} //if

		int n = str.find( aID[i] );

		if( n != -1 )
		{
			return FALSE;
		} //if
	} //for

	return TRUE;
} //CheckWord