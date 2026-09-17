/**
 * ¿¥ºí·±
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	·Î³Ä ( ª«ªÍ ª»ª¤ªÐªó )
 * @date	$Date: 03-12-20 7:55a $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /GameClient/Src/Emblem.h $
 * 
 * 3     03-12-20 7:55a Paran
 * 
 * 2     03-12-15 10:38a Kjmgo
 * ¿¥ºí·±Ãß°¡(  'Emblem' õÚÊ¥(ªÄª¤ª«) )
 * 
 * 1     03-12-15 8:58a Kjmgo
 * 
 * <HR>
 *
 * @file	Emblem.h
 */

#ifndef __Emblem_Header__
#define __Emblem_Header__

struct SEmblem;
void GetEmblemFileName( char* szFile, SEmblem* pEmblem );
/**
 *
 */
struct SEmblem
{
	// ´çºÐ°£ »ç¿ëÇÒ °Í. ( ª·ªÐªéª¯ ÞÅ(ª·)ª¦ª³ªÈ¡£)
	int			nId;			///< ¿¥ºí·± ¾ÆÀÌµð( «¢«¤«Â )
	IW3DSprite* pSprite;		///< ¿¥ºí·±	

	/**
	 *
	 */
	SEmblem()
	{
		pSprite = NULL; 
	} //SEmblem

	/**
	 *
	 */
	~SEmblem()
	{
		SAFE_RELEASE( pSprite );
	} //~SEmblem

	/**
	 *
	 */
	void Load()
	{
		SAFE_RELEASE( pSprite );

		pSprite = MAKE_COMPONENT( Sprite );
		char szName[256];
		GetEmblemFileName( szName, this );

		if( !pSprite->Load( szName, 14, 20 ) )
		{
			Assert( 0 );
			SAFE_RELEASE( pSprite );
		} //if
	} //Load

	// ´çºÐ°£Àº ¿¥ºí·± ¾ÆÀÌµð¸¸ »ç¿ëÇÔ. ( ª·ªÐªéª¯ªÏ 'Emblem Id'ªÀª± ÞÅ(ªÄª«)ª¦. )
	
	 
	// ÇöÀç ¾²Áö ¾ÊÀ» °Íµé ( úÞî¤(ª²ªóª¶ª¤), ÞÅ(ªÄª«)ª¦ªÇªÏ ª¢ªêªÞª»ªó¡£ )

	// BYTE btBasicShape;		///< ±âº»ÀûÀÎ ¹Ù´Ú ¸ð¾ç	(ÐñÜâîÜ(ª­ªÛªóªÞªÈ)ªÊ î¼(ª½ªÈ)ªÎ û¡÷¾(ª±ª¤ª¿ª¤) )
	// BYTE btBasicPattern;		///< ¹Ù´Ú ¹®¾ç	( î¼(ª½ªÈ)ªÎ Ü·(ª¬ªé) )
	// BYTE btPatternColor;		///< ¹Ù´Ú ¹®¾ç »ö (î¼(ª½ªÈ)ªÎ Ü·(ª¬ªé)ªÎ ßä(ª¤ªí) )
	// BYTE btColor;			///< ¹Ù´Ú ¹è°æ»ö (î¼(ª½ªÈ)ªÎ ÛÎÌØ(ªÏª¤ª±ª¤)ªÎ ßä(ª¤ªí))
	// BYTE btPattern;			///< ¹Ù´Ú ( î¼(ª½ªÈ) )
}; //

/**
 *	¿¥ºí·±ÀÇ ÆÄÀÏ ÀÌ¸§À» ¾ò¾î ¿É´Ï´Ù. ( "Emblem"ªÎ«Õ«¡«¤«ëÙ£îñªÏò¥ªÃªÆÕÎªë. )
 */
void GetEmblemFileName( char* szFile, SEmblem* pEmblem )
{
	Assert( szFile );
	Assert( pEmblem );

	// ÀÓ½Ã ÄÚµå. ( ìúãÁ(ªêªóª¸) «³«Å )

	// ª·ªÐªéª¯ªÏª³ªÎªÞªÞ¡£
	sprintf( szFile, "Emblem/em_%d.tag", pEmblem->nId );

	/*

	sprintf( szFile, "em_%d_%d_%d_%d_%d.tga",
			pEmblem->btBasicShape,		//< ÐñÜâîÜ(ª­ªÛªóªÞªÈ)ªÊî¼(ª½ªÈ)ªÎû¡÷¾(ª±ª¤ª¿ª¤)
			pEmblem->btBasicPattern,	//< î¼(ª½ªÈ)ªÎÜ·(ª¬ªé)
			pEmblem->btPatternColor,	//< î¼(ª½ªÈ)ªÎÜ·(ª¬ªé)ªÎßä(ª¤ªí)
			pEmblem->btColor,			//< î¼(ª½ªÈ)ªÎÛÎÌØ(ªÏª¤ª±ª¤)ªÎßä(ª¤ªí)
			pEmblem->btPattern			//< î¼(ª½ªÈ)
			);
	*/

} //GetEmblemFileName

#endif //#ifndef __Emblem_Header__