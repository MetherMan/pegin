/**
 * À¯Àú ÀÎÅÍÆäÀÌ½º ¸ðµâ.
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	ÆÄ¶õ~*
 * @date	$Date: 05-02-18 10:23p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /UInterface/W3DUInterface.cpp $
 * 
 * 3     05-02-18 10:23p Umenokoji
 * 
 * 136   05-02-18 6:45p Umenokoji	Line 2932 Ä³¸¯ÅÍ Á¤º¸Ã¢¿¡ ¸¶¹ýÀúÇ×·Â Ç¥½Ã
 * 
 * 135   04-12-08 3:07a Sk8snow2
 * 
 * 134   04-11-16 7:11p Sk8snow2
 * 
 * 133   04-09-24 4:46p Paran
 * 
 * 132   04-09-10 11:28p Paran
 * 
 * 97    04-01-02 2:21p Paran		¸Þ¼¼Áö define Ã³¸®
 * 
 * 96    03-12-31 10:31 Sckyj		Init() - delete Parsign code that file name is *_C.ini when _CHINAVERSION 
 * 
 * 92    03-12-22 5:00p Sckyj		2D, 3D sound ºÐ¸®
 * 
 * 79    03-12-10 2:04p Paran		ÀÎÃ¾Ã÷ Ä«µå Ãß°¡
 * 
 * 78    03-12-09 5:58p Paran		¾ÆÀÌÅÛ µ¥ÀÌÅÍ ¼öÁ¤
 * 
 * 76    03-12-05 11:36a Sckyj		GetUnitNum() : static char szBuff[32]¸¦ char szBuff[32]·Î º¯°æ
 * 
 * 21    03-06-24 3:37p Paran		¼Ò¸® Ãß°¡.
 * 
 * 19    03-06-23 2:25a Paran		SetPlayerPower Ãß°¡.
 * 
 * 17    03-06-18 10:16p Paran		ÀÎº¥Åä¸® ¼Õ ¹Ù²Ù±â, Ä³¸¯ÅÍ ¼±ÅÃ Ã¢ ¼öÁ¤
 * 
 * 11    03-06-04 11:24a Paran		¹Ì´Ï¸Ê »óÁ¡ Ãß°¡
 * 
 * 10    03-05-25 4:10p Paran		¹°¾à ´Ù·® ±¸ÀÔ ÀÎÅÍÆäÀÌ½º Ãß°¡
 * 
 * 7     03-05-23 3:43p Paran		¹ö±×¼öÁ¤ Áß...
 * 
 * 6     03-05-20 9:35p Paran		°è¼Ó ¼öÁ¤ Áß...
 * 
 * 4     03-05-17 4:31p Paran		ÆÄÆ¼¿ø º¸ÀÌ±â ºÎºÐ ¼öÁ¤.
 * 
 * 2     03-05-17 4:20a Paran		°¢ ÄÞÆ÷³ÍÆ® ºÐ¸®.
 *
 * 160   03-05-16 8:09p Paran		NPC ½ºÅ©¸³Æ® ¼öÁ¤.
 *
 * 131   03-05-06 6:18p Paran		»óÁ¡ ±â´É Ãß°¡.
 *
 * 127   03-04-30 7:11p Paran		½ºÅ³ ¼±ÅÃ ¼öÁ¤.
 *
 * 124   03-04-23 8:36p Paran		¸¶¹ýÃ¢.
 *
 * 120   03-04-15 1:27a Paran		NPC Ãß°¡.
 *
 * 119   03-04-12 3:02p Paran		Ä³¸¯ÅÍ ÀåÂø ¾ÆÀÌÅÛ º¯°æ ±â´É Ãß°¡.
 *
 * 118   03-04-11 5:28p Paran		Ä³¸¯ÅÍ ÀåÂø ºÎºÐ°ú ¹Ì´Ï¸Ê ¼öÁ¤.
 *
 * 117   03-04-10 6:24p Paran		¹Ì´Ï¸Ê Ãß°¡.
 *
 * 114   03-04-02 8:11p Paran		ÇÃ·¹ÀÌ¾î ¾ÆÀÌÅÛ Ãß°¡, ¸Þ¼¼Áö ¹Ú½º Å° ÀÔ·Â Ãß°¡.
 *
 * 108   03-03-26 9:19p Paran		ÀÎº¥Åä¸® ¾î»öÇÑ ºÎºÐ ¼öÁ¤ÇÏ°í Ã¼Å©.
 *
 * 107   03-03-26 6:22a Paran		¾ÆÀÌÅÛ ¼­¹ö ¿¬°á.
 *
 * 105   03-03-25 3:36a Paran		ÀÎº¥Åä¸® ¾ÆÀÌÅÛ ¼­¹ö¿Í ¿¬°á.
 *
 * 102   03-03-22 2:37a Paran		¸ÅÁ÷ µî·Ï ºÎºÐ ¼öÁ¤.
 *
 * 86    03-03-06 11:16p Paran		ÀÎº¥Åä¸® ¸¶¿ì½º ±â´É º¯°æ°ú Ä³¸¯ÅÍ ¼±ÅÃ ºÎºÐ Á¤º¸ »èÁ¦ Ã³¸®.
 * <HR>
 *
 * @file	W3DUInterface.cpp
 */


#include "UInterface_Export.h"
#include "W3DUInterface.h"
#include "NotUseWord.h"

IMPLEMENT_SINGLETON_COMPONENT( CW3DUInterface, UInterface );

/**
 * »ý¼ºÀÚ.
 */
CW3DUInterface::CW3DUInterface()
:	m_nLastID			( BEGIN_ID ),
	m_pSound			( NULL ),
	m_pCurrCommopent	( NULL ),
	m_pTopDlg			( NULL ),
	m_pOldFocusCom		( NULL ),
	m_pCurrEditor		( NULL ),
	m_pCursor			( NULL ),
	m_pInput			( NULL ),
	m_pItemInfoMgr		( NULL ),
	m_pNPCScript		( NULL ),
	m_pQuestScript		( NULL ),
	m_pSmithScript		( NULL ),
	m_bLButtonPressed	( FALSE ),
	m_bRButtonPressed	( FALSE ),
	m_bTabKeyPressed	( FALSE ),
	m_bReturnKeyPressed	( FALSE ),
	m_nMouseX			( 0 ),
	m_nMouseY			( 0 ),
	m_nOldMouseX		( 0 ),
	m_nOldMouseY		( 0 ),
	m_nNumPosion		( 0 ),
	m_nMainSkill		( -1 ),
	m_bIME				( FALSE ),
	m_btQuickBarNum		( 0 ),
	m_pHelpImage		( NULL ),
	m_pPartyBoard		( NULL ),
	m_pRectangle		( NULL ),
	m_nGold				( 0 ),
	m_nCash				( 0 ),
	m_pEquipBody		( NULL ),
	m_pConfirmItemData	( NULL ),
	m_bWinMouse			( TRUE ),
	m_btInputSystem		( FALSE ),
	m_SelectIntegration ( FALSE ),
	m_EnchantPricemoney ( 0 ),
	m_EnchantSmithItem	( NULL ),
	m_bNumPosion		( FALSE ),
	m_bCheckSild		( FALSE ),
	m_bCheckWeight		( FALSE ),
	m_ServerNum			( 0 ),
	m_CharPos			( 0 ),
	m_cnt				( 0 ),
	m_CheckState		( FALSE )
	//m_EnchantSmithCard	( NULL )
{
	m_sModeState		= "";
	m_szMegKey[0]		= '\0';
	m_szTipInfo[0]		= '\0';
	m_szRecentWhisperName[0]	= '\0';
	m_szRecentChattText[0]		= '\0';
	m_szUIccompentSmith[0]		= '\0';
	m_szTxtName[0] = '\0';
	ZeroMemory( m_aszWordsSave, MAX_CHATTSTRING * 10 );
//	ZeroMemory( &m_EnchantSmithCard, sizeof( CInchentCard ) );
} //CW3DUInterface::CW3DUInterface

/**
 * ¼Ò¸êÀÚ.
 */
CW3DUInterface::~CW3DUInterface()
{
	LIST_PUIDLG::iterator		listIter = m_listDialog.begin();
	while( listIter != m_listDialog.end() )
	{
		UIDialog*	pDlg = *listIter;
		SAFE_RELEASE( pDlg );
		listIter = m_listDialog.erase( listIter );
	} //while

	MAP_STRSPRITE::iterator		itorCursor = m_mapCursor.begin();
	while( itorCursor != m_mapCursor.end() )
	{
		MAP_STRSPRITE::iterator		itorNext = itorCursor;
		itorNext++;

		IW3DSprite*		pCursor = itorCursor->second;
		SAFE_RELEASE( pCursor );
		m_mapCursor.erase( itorCursor );

		itorCursor = itorNext;
	} //while

	MAP_HCURSOR::iterator		itorWCursor = m_mapWinCursor.begin();
	while( itorWCursor != m_mapWinCursor.end() )
	{
		MAP_HCURSOR::iterator		itorNext = itorWCursor;
		itorNext++;
		
		DestroyCursor( itorWCursor->second );
		m_mapWinCursor.erase( itorWCursor );

		itorWCursor = itorNext;
	} //while

	MAP_STRSPRITE::iterator		itorSprite = m_mapSprite.begin();
	while( itorSprite != m_mapSprite.end() )
	{
		MAP_STRSPRITE::iterator		itorNext = itorSprite;
		itorNext++;

		IW3DSprite*		pSprite = itorSprite->second;
		SAFE_RELEASE( pSprite );
		m_mapSprite.erase( itorSprite );

		itorSprite = itorNext;
	} //while

	MAP_LISTSTRING::iterator	itorModeState = m_mapModeState.begin();
	while( itorModeState != m_mapModeState.end() )
	{
		MAP_LISTSTRING::iterator	itorNext = itorModeState;
		itorNext++;

		LIST_STRING*	plistString = static_cast<LIST_STRING*>( itorModeState->second );
		SAFE_DELETE( plistString );
		m_mapModeState.erase( itorModeState );

		itorModeState = itorNext;
	} //while

	LIST_WHISPERNAME::iterator	iterWhisperName = m_listWhisperName.begin();
	while( iterWhisperName != m_listWhisperName.end() )
	{
		SAFE_FREE( (*iterWhisperName) );
		iterWhisperName = m_listWhisperName.erase( iterWhisperName );
	} //while

	g_ChatIME.Release();
	SAFE_RELEASE( m_pQuestScript );
	SAFE_RELEASE( m_pNPCScript );
	SAFE_RELEASE( m_pSound );
	SAFE_RELEASE( m_pRectangle );
	SAFE_RELEASE( m_pEquipBody );
	SAFE_RELEASE( m_pHelpImage );
	SAFE_RELEASE( g_pUIGraphicMgr );
} //CW3DUInterface::~CW3DUInterface

/**
 * ÀÎÅÍÆäÀÌ½º¸¦ ÃÊ±âÈ­ÇÑ´Ù.
 *
 * @param	hWnd		[IN] À©µµ¿ì ÇÚµé
 * @param	pInput		[IN] Input ÄÄÆ÷³ÍÆ®
 *
 * @return	ÃÊ±âÈ­ ¼º°ø ¿©ºÎ.
 */
BOOL CW3DUInterface::Init( 
	int			nWidth, 
	int			nHeight, 
	BOOL		bWinMouse 
	)
{
	g_nScreenWidth  = nWidth;
	g_nScreenHeight = nHeight;
	m_bWinMouse = bWinMouse;

	HCURSOR		hCursor;
	hCursor = LoadCursorFromFile( "Interface/cursor_nw.wtm" );
	if( hCursor )		m_mapWinCursor.insert( make_pair( (string)"CURSOR_NORMAL", hCursor ) );	
	else				Assert( !"¾ø´Â À©µµ¿ìÄ¿¼­" );	

	hCursor = LoadCursorFromFile( "Interface/cursor_sw.wtm" );
	if( hCursor )		m_mapWinCursor.insert( make_pair( (string)"CURSOR_SELECT", hCursor ) );	
	else				Assert( !"¾ø´Â À©µµ¿ìÄ¿¼­" );

	hCursor = LoadCursorFromFile( "Interface/cursor_ew.wtm" );
	if( hCursor )		m_mapWinCursor.insert( make_pair( (string)"CURSOR_EDIT", hCursor ) );	
	else				Assert( !"¾ø´Â À©µµ¿ìÄ¿¼­" );

	hCursor = LoadCursorFromFile( "Interface/cursor_attackw.wtm" );
	if( hCursor )		m_mapWinCursor.insert( make_pair( (string)"CURSOR_ATTACK", hCursor ) );	
	else				Assert( !"¾ø´Â À©µµ¿ìÄ¿¼­" );

	hCursor = LoadCursorFromFile( "Interface/cursor_hand1w.wtm" );
	if( hCursor )		m_mapWinCursor.insert( make_pair( (string)"CURSOR_HAND", hCursor ) );	
	else				Assert( !"¾ø´Â À©µµ¿ìÄ¿¼­" );

	hCursor = LoadCursorFromFile( "Interface/cursor_hand2w.wtm" );
	if( hCursor )		m_mapWinCursor.insert( make_pair( (string)"CURSOR_HOLD", hCursor ) );	
	else				Assert( !"¾ø´Â À©µµ¿ìÄ¿¼­" );

	hCursor = LoadCursorFromFile( "Interface/cursor_talkw.wtm" );
	if( hCursor )		m_mapWinCursor.insert( make_pair( (string)"CURSOR_TALK", hCursor ) );	
	else				Assert( !"¾ø´Â À©µµ¿ìÄ¿¼­" );

	hCursor = LoadCursorFromFile( "Interface/cursor_nomovew.wtm" );
	if( hCursor )		m_mapWinCursor.insert( make_pair( (string)"CURSOR_NOMOVE", hCursor ) );	
	else				Assert( !"¾ø´Â À©µµ¿ìÄ¿¼­" );

/*	hCursor = LoadCursorFromFile( "Interface/cursor_mun1w.wtm" );
	if( hCursor )		m_mapWinCursor.insert( make_pair( (string)"CURSOR_DOOR1", hCursor ) );	
	else				Assert( !"¾ø´Â À©µµ¿ìÄ¿¼­" );

	hCursor = LoadCursorFromFile( "Interface/cursor_mun2w.wtm" );
	if( hCursor )		m_mapWinCursor.insert( make_pair( (string)"CURSOR_DOOR2", hCursor ) );	
	else				Assert( !"¾ø´Â À©µµ¿ìÄ¿¼­" );*/

	return TRUE;
} //CW3DUInterface::Init

/**
 * °¢ ÀåÄ¡ ¼³Á¤.
 */
BOOL CW3DUInterface::SetWinDevice( HWND	hWnd, IW3DInput* pInput )
{
	Assert( hWnd );
	Assert( pInput );

	g_pUIGraphicMgr = MAKE_COMPONENT( UIGraphic );
	if( !g_pUIGraphicMgr )
	{
		Assert( !"Not Allocate" );
		return FALSE;
	} //if
	g_pUIGraphicMgr->Init();

	m_hWnd = hWnd;
	m_pInput = pInput;

	SAFE_RELEASE( m_pSound );
	m_pSound = MAKE_COMPONENT( SoundObject2D );
	CHECK_PTR_RETURN( m_pSound );

	g_ChatIME.Init( hWnd );

	m_pItemInfoMgr	= MAKE_COMPONENT( ItemInfoMgr );
	if( !m_pItemInfoMgr )
	{
		Assert( !"»ý¼º½ÇÆÐ" );
		return FALSE;
	} //if
	m_ItemMgr.SetItemInfoMgr( m_pItemInfoMgr );
	
	m_pNPCScript = MAKE_COMPONENT( NPCScript );
	if( !m_pNPCScript )
	{
		Assert( !"»ý¼º½ÇÆÐ" );
		return FALSE;
	} //if

	g_pScene		= MAKE_COMPONENT( Scene );
	if( !g_pScene )
	{
		Assert( !"¾ÀÀÌ ¾ø´Ù." );
		return FALSE;
	} //if

	//////////////  ÀÎÅÍÆäÀÌ½º ½ºÅ©¸³Æ®  ////////////
	char		szScripName[64];
	//sprintf( szScripName, "Interface/ui%04d.ini", g_nScreenWidth );
	sprintf( szScripName, "Interface/ui.ini" );
	if( !Load( szScripName, "Interface/guistate.ini", g_nScreenWidth, g_nScreenHeight ) )
	{
		Assert( !"UI ½ºÅ©¸³Æ® ·Îµå ½ÇÆÐ" );
		return FALSE;
	} //if

	SetCharInfoEmptyButton( TRUE );


	//////////////  ÆÁ ÆùÆ®  ////////////

	if( !g_pScene->FindFont( &m_pfntTip, "GULLIM12" ) )
	{
		Assert( !"ÆùÆ®°¡ ¾ø´ç" );
		return FALSE;
	} //if

	m_pRectangle = MAKE_COMPONENT( Rectangle );
	if( !m_pRectangle )
	{
		Assert( !"½ÇÆÐ" );
		return FALSE;
	} //if

	return TRUE;
} //CW3DUInterface::SetWinDevice


/**
 * Ã¢ ±âº» ¼³Á¤. (°ÔÀÓ·Îµù ¹Ù·Î Àü¿¡ È£ÃâÇÏÀÚ. ¾Æ´Ô Clean ÇØ¹ö¸± ¼öµµ...)
 */
void CW3DUInterface::InitDlg( IW3DCamera* pCamera )
{
	SetCharInfoEmptyButton( TRUE );

	//////////////  ÀÎÃ¾Æ® ÃÊ±âÈ­ /////

	m_EnchantCard.Init( pCamera );

	//////////////  ½ºÅ³µî·Ï Ã¢ ÃÊ±âÈ­ /////
	InitSkill();

	//////////////  ÄÉ¸¯ÅÍ Á¤º¸Ã¢  ///////////

	SCharInfo*	pCharInfo = m_CharSelInfo.GetCharInfo();
	Assert( pCharInfo );

	SAFE_RELEASE( m_pEquipBody );
	m_pEquipBody = MAKE_COMPONENT( Sprite );
	Assert( m_pEquipBody );

	if( pCharInfo->btSex )	m_pEquipBody->Load( "Interface/inven-b-2.tga", 168, 149 );
	else					m_pEquipBody->Load( "Interface/inven-b-1.tga", 168, 149 );
	SetPicture( "PIC_INVEN_BODY", m_pEquipBody ); 
	SetPicture( "PIC_SERVER_INVEN_BODY", m_pEquipBody );

	//////////////  ÆÄÆ¼ ¹öÆ°  ///////////
	
	m_pPartyBoard = reinterpret_cast<UISheet*>( GetComponent( "SHEET_PARTYBOARD" ) );
	if( !m_pPartyBoard )
	{
		Assert( !"ÆÄÆ¼°Ô½ÃÆÇ ¾ò±â ½ÇÆÐ" );
	} //if

	////////////// Ã¼Å©¹öÆ° //////////////

	CheckGroup( "CHK_SHOP_TAB", "SHOPINVENTORY_", 1 );
	CheckGroup( "CHK_STORAGE_TAB", "INVEN_STORAGE_", 1 );
	CheckGroup( "CHK_INVEN_TAB", "INVENTORY_IN_", 1 );
	CheckGroup( "CHK_CASH_TAB", "CASHITEM_IN_", 1 );
	CheckGroup( "CHK_DEALME_TAB", "INVENTORY_DEALME_", 1 );
	CheckGroup( "CHK_DEALYOU_TAB", "INVENTORY_DEALYOU_", 1 );
	ChatCheckButton( 1 );
	Skill1CheckButton( 1 );
	Skill2CheckButton( 2 );

	////////////// ½ºÅ©·Ñ //////////////

	UIScrollBar*	pChatScroll = reinterpret_cast<UIScrollBar*>( GetComponent( "SCR_CHAT_LIST" ) );
	Assert( pChatScroll );
	pChatScroll->SetScrollPos( 100 );

	////////////// Äü ½º·Ô //////////////
	m_btCharacterNum = m_CharSelInfo.GetIndex();
	ShowQuickBar();
	m_btQuickBarNum = 0;
	for( int nCnt = 0; nCnt < NUM_QUICKSLOTBAR; nCnt++ )
	{
		m_apQuickBar[nCnt] = GetQuickBar( nCnt );
	} //for

} //CW3DUInterface::InitDlg

/**
 * ¸ðµå »óÅÂ¸¦ ¼³Á¤ÇÑ´Ù.
 *
 * @param	szMode		[IN] ¸ðµå »óÅÂ.
 *
 * @return	»óÅÂ ¼³Á¤ ¼º°ø ¿©ºÎ.
 */
BOOL CW3DUInterface::SetModeState( const char* szMode )
{
	MAP_LISTSTRING::iterator		mapIter_mode = m_mapModeState.find( szMode );

	if( mapIter_mode == m_mapModeState.end() )
	{
		Assert( !"±×·±¸ðµå´Â ¾ø´Ù" );
		return FALSE;
	}

	m_sModeState		= szMode;
	m_pCurrCommopent	= NULL;
	m_pCurrEditor		= NULL;
	

	UIDialog*					pDlg;
	string						strDlgName;
	LIST_STRING*				plistString = (LIST_STRING*)mapIter_mode->second;
	LIST_STRING::iterator		listIter_string;
	LIST_PUIDLG::iterator		listIter_dlg;
	MAP_LISTSTRING::iterator	mapIter_state;

	for( listIter_dlg = m_listDialog.begin() ; listIter_dlg != m_listDialog.end() ; listIter_dlg++ )
	{
		pDlg = *listIter_dlg;
		ShowDlg( pDlg, FALSE, FALSE );
	}

	for( listIter_string = plistString->begin() ; listIter_string != plistString->end() ; listIter_string++ )
	{
		strDlgName = *listIter_string;
		pDlg = (UIDialog*)GetComponent( strDlgName.c_str() );
		if( !pDlg )
		{
			Assert( !"ÇØ´ç Dlg°¡ ¾ø½À´Ï´Ù" );
			return FALSE;
		}
		if( !strDlgName.compare( "DLG_LOGIN" ) )
		{
			UICComponent*	pComp = GetComponent( "EDT_LOGIN_ID" );
			Assert( pComp );
			EditBox_LDown( pComp );
			m_pCurrEditor->SetLoginMode( TRUE );
		}
	/*	else
		{
			m_pCurrEditor->SetLoginMode( FALSE );
		}*/
		ShowDlg( pDlg, TRUE, FALSE );
	}
	
	while( !g_queueEvent.empty() )
	{
		g_queueEvent.pop();
	} //while

	return TRUE;
}


/**
 * Clean up.
 */
void CW3DUInterface::Cleanup()
{
	LIST_PUIDLG::iterator	itor;

	for( itor = m_listDialog.begin() ; itor != m_listDialog.end() ; itor++ )
	{
		UIDialog*	pDlg = *itor;
		Assert( pDlg );
		pDlg->Cleanup();
	} //for
	m_ItemMgr.Cleanup();
	m_CharSelInfo.Cleanup();

	SetPicture( "PIC_CHAR_MAINSKILL", (IW3DSprite*)NULL );

	m_GameReady.Cleanup();
} //CW3DUInterface::Cleanup

/**
 * ÄÄÆ÷³ÍÆ®¸¦ cleanupÇÑ´Ù.
 *
 * @param	szID		[IN] ÄÄÆ÷³ÍÆ® ID.
 */
void CW3DUInterface::CleanupComponent( const char* szID )
{
	UICComponent*	pComp = GetComponent( szID );
	Assert( pComp );

	if( pComp )
	{
		pComp->Cleanup();
	} //if
} //CW3DUInterface::CleanupComponent

/**
 * ...
 */
BOOL CW3DUInterface::AddComponent( const char *szKey, UICComponent* pComponent )
{
	MAP_STRCOMP::iterator		mapIter = m_mapComponent.find( szKey );
	if( mapIter != m_mapComponent.end() )
	{
		return FALSE;
	} //if

	m_mapComponent.insert( make_pair( szKey, pComponent ) );

	return TRUE;
} //CW3DUInterface::AddComponent

/**
 * ...
 */
UICComponent* CW3DUInterface::GetComponent( const char *szKey )
{
	MAP_STRCOMP::iterator		mapIter = m_mapComponent.find( szKey );
	if( mapIter == m_mapComponent.end() )
	{
		return NULL;
	} //if

	return mapIter->second;
} //CW3DUInterface::GetComponent

/**
 *
 */
int CW3DUInterface::GenerateID( const char *szIDName )
{
	string						strIdName;
	MAP_INTSTR::iterator		mapIter_id;
	for( mapIter_id = m_mapControlID.begin(); mapIter_id != m_mapControlID.end(); mapIter_id++ )
	{
		if ( strcmp( mapIter_id->second.c_str(), szIDName ) == 0 )
		{
			return FAILED_ID;
		}
	}
	m_nLastID++;
	m_mapControlID.insert( make_pair( m_nLastID, (string)szIDName ) );

	return m_nLastID;
} //CW3DUInterface::GenerateID

/**
 *
 */
const char* CW3DUInterface::FindNameFromID( int nID )
{
	MAP_INTSTR::iterator		mapIter = m_mapControlID.find( nID );
	if( mapIter == m_mapControlID.end() )
	{
		return NULL;
	}
	return mapIter->second.c_str();
} //CW3DUInterface::FindNameFromID

/**
 * Ä¿¼­º¯°æ.
 */
BOOL CW3DUInterface::SetCursorMode( const char* szCursor )
{
	char		szBuff[64];
	if( m_pCurrCommopent )
	{
		switch( m_pCurrCommopent->m_nType )
		{
		case UI_PUSHBUTTON:
		case UI_TABBUTTON:
		case UI_INVENTORYIN:
		case UI_INVENTORYON:
		case UI_PICTURE:
		case UI_SCROLLBAR:		strcpy( szBuff, "CURSOR_SELECT" );		break;
		case UI_EDITTEXT:		strcpy( szBuff, "CURSOR_EDIT" );		break;
		default:				strcpy( szBuff, "CURSOR_NORMAL" );		break;
		}
	}
	else
	{
		strcpy( szBuff, szCursor );
	}
	
	if( !strcmp( m_szPrevCursorName, szBuff ) )
	{
		return TRUE;
	} //if
	strcpy( m_szPrevCursorName, szBuff );

	if( m_bWinMouse )
	{
		MAP_HCURSOR::iterator		itorWCursor = m_mapWinCursor.find( szBuff );
		if( itorWCursor == m_mapWinCursor.end() )
		{
			Assert( !"¾ø´Â À©µµ¿ì Ä¿¼­" );
			return FALSE;
		} //if
		SetCursor( itorWCursor->second );
		m_pCursor = NULL;
	}
	else
	{
		MAP_STRSPRITE::iterator		mapIter_Cursor;
		mapIter_Cursor = m_mapCursor.find( szBuff );

		if( mapIter_Cursor == m_mapCursor.end() )
		{
			Assert( !"¾ø´Â Ä¿¼­" );
			return FALSE;
		}
		m_pCursor = mapIter_Cursor->second;
		Assert( m_pCursor );
	} //if..else

	return TRUE;
} //CW3DUInterface::SetCursorMode

//
// CW3DUInterface::ShopInvenTabSelect
//
BOOL CW3DUInterface::ShopInvenTabSelect( int nIndex )
{
	UIInventoryIN*	pInven1 = (UIInventoryIN*)GetComponent( "SHOPINVENTORY_1" );
	UIInventoryIN*	pInven2 = (UIInventoryIN*)GetComponent( "SHOPINVENTORY_2" );
	UIInventoryIN*	pInven3 = (UIInventoryIN*)GetComponent( "SHOPINVENTORY_3" );

	if( !pInven1 || !pInven2 || !pInven3 )
	{
		Assert( !"INVENTORY_IN Not Found" );
		return FALSE;
	} //if

	switch( nIndex )
	{
	case 0:
		pInven1->Show( TRUE );
		pInven2->Show( FALSE );
		pInven3->Show( FALSE );
		break;

	case 1:
		pInven1->Show( FALSE );
		pInven2->Show( TRUE );
		pInven3->Show( FALSE );
		break;

	case 2:
		pInven1->Show( FALSE );
		pInven2->Show( FALSE );
		pInven3->Show( TRUE );
		break;
	} //siwtch

	return TRUE;
}

//
// CW3DUInterface::GetInventoryIN
//
UIInventoryIN* CW3DUInterface::GetInventoryIN( const char* szKey, const int nIndex )
{
	Assert( nIndex > 0 );

	char		szInvenName[64];
	sprintf( szInvenName, "%s%d", szKey, nIndex );

	UIInventoryIN*		pInven = reinterpret_cast<UIInventoryIN*>( GetComponent( szInvenName ) );

	return pInven;
}

/**
 * Äü¹Ù ¾ò±â.
 */
UIQuickSlot* CW3DUInterface::GetQuickBar( const int nIndex )
{
	char		szQuickBarName[64];
	sprintf( szQuickBarName, "QSLOT_%d", nIndex );

	UIQuickSlot*		pQuickSlot;
	pQuickSlot = reinterpret_cast<UIQuickSlot*>( GetComponent( szQuickBarName ) );
	if( !pQuickSlot )
	{
		Assert( !"¾ø´Â Äü¹Ù" );
		return NULL;
	} //if

	return pQuickSlot;
} //CW3DUInterface::GetQuickBar

/**
 * UIComponent º¸ÀÌ±â ¿©ºÎ.
 */
BOOL CW3DUInterface::ShowComponent( const char* szCompName, BOOL bShow )
{
	UICComponent*	pComp = GetComponent( szCompName );
	if( !pComp )
	{
		Assert( !"¾ø´Â ÀÎÅÍÆäÀÌ½º" );
		return FALSE;
	} //if

	pComp->Show( bShow );

	return TRUE;
} //CW3DUInterface::ShowComponent

/**
 * Dlg¸¦ º¸¿©ÁØ´Ù.
 */
BOOL CW3DUInterface::ShowDlg( const char* szDlgName, BOOL bShow, BOOL bEvent )
{
	UIDialog*				pDlg;
	LIST_PUIDLG::iterator	itor = m_listDialog.begin();
	while( itor != m_listDialog.end() )
	{
		pDlg = (*itor);
		Assert( pDlg );

		if( !pDlg->m_strName.compare( szDlgName ) )
		{
			pDlg->Show( bShow );

/*
			if( !pDlg->IsAlways() )
			{
				if( bShow )		m_pSound->Play( "wopen.wav" );
				else			m_pSound->Play( "wclose.wav" );
			} //if
*/
			if( bShow )
			{
				m_listDialog.erase( itor );
				m_listDialog.push_back( pDlg );
				m_pTopDlg = pDlg;
			}
			else if( m_pCurrCommopent )
			{
				if( pDlg == m_pCurrCommopent || pDlg == m_pCurrCommopent->m_pOwnerComp )
				{
					m_pCurrCommopent->FocusOut();
					m_pCurrCommopent = NULL;
				}
				ResetTopDlg();
			}

			if( bEvent )
			{ // ÀÌº¥Æ® ¹ß»ý
				PushEvent( pDlg->m_strName.c_str(), EVENT_SHOWDLG, 0, bShow );
				Dlg_Exit( pDlg );
			} //if
			
			break;
		}
		itor++;
	}

	return TRUE;
} //CW3DUInterface::ShowDlg

/**
 * Dlg¸¦ º¸¿©ÁØ´Ù.
 */
BOOL CW3DUInterface::ShowDlg( UICComponent* pComp, BOOL bShow, BOOL bEvent  )
{
	Assert( pComp && "UICComponent°¡ ¾ø½À´Ï´Ù" );

	UIDialog*		pDlg;
	if( pComp->m_pOwnerComp )	pDlg = static_cast<UIDialog*>( pComp->m_pOwnerComp );
	else						pDlg = static_cast<UIDialog*>( pComp );
	Assert( pDlg );
/*
	if( !pDlg->IsAlways() )
	{
		if( bShow )		m_pSound->Play( "wopen.wav" );
		else			m_pSound->Play( "wclose.wav" );
	} //if
*/	
	pDlg->Show( bShow );
	if( !bShow && m_pCurrCommopent )
	{
		if( pDlg == m_pCurrCommopent || pDlg == m_pCurrCommopent->m_pOwnerComp )
		{
			m_pCurrCommopent->FocusOut();
			m_pCurrCommopent = NULL;
		} //if
	} //if

	ResetTopDlg();

	if( bEvent )
	{ //ÀÌº¥Æ® ¹ß»ý
		PushEvent( pDlg->m_strName.c_str(), EVENT_SHOWDLG, 0, bShow );
		Dlg_Exit( pDlg );
	} //if
	
	return TRUE;
} //CW3DUInterface::ShowDlg

/**
 * º¸ÀÌ±â.
 */
BOOL CW3DUInterface::Show( const char* szCompName, BOOL bShow )
{
	UICComponent*	pComp = GetComponent( szCompName );
	if( !pComp )
	{
		Assert( "¾ø´Ù" );
		return FALSE;
	} //if
	pComp->Show( bShow );
	return TRUE;
} //CW3DUInterface::Show

/**
 * Dlg º¸ÀÌ±â »óÅÂ.
 */
BOOL CW3DUInterface::IsShowDlg( const char* szDlgName_ )
{
	UIDialog*				pDlg;
	LIST_PUIDLG::iterator	itor = m_listDialog.begin();
	while( itor != m_listDialog.end() )
	{
		pDlg = (*itor);
		Assert( pDlg );

		if( !pDlg->m_strName.compare( szDlgName_ ) )
		{
			return pDlg->IsShow();
		} //if
		itor++;
	} //while
	return FALSE;
} //CW3DUInterface::IsShowDlg

/**
 * ´Ù¸¥ ÀÔ·ÂÀ» °ÅºÎ Dlg
 */
BOOL CW3DUInterface::IsShowLockDlg()
{
	UIDialog*				pDlg;
	LIST_PUIDLG::iterator	itor = m_listDialog.begin();
	while( itor != m_listDialog.end() )
	{
		pDlg = (*itor);
		Assert( pDlg );

		if( !pDlg->m_strName.compare( "DLG_SHOPINVENTORY" ) ||
			!pDlg->m_strName.compare( "DLG_DEAL" ) ||
			!pDlg->m_strName.compare( "DLG_STORAGE" ) )
		{
			if( pDlg->IsShow() )
			{
				return TRUE;
			} //if
		} //if
		itor++;
	} //while
	return FALSE;
} //CW3DUInterface::IsShowLockDlg

/**
 * ÃÖ»óÀ§ Dlg¼³Á¤.
 */
void CW3DUInterface::ResetTopDlg()
{
	if( m_listDialog.empty() )		return;

	LIST_PUIDLG::iterator	itor = m_listDialog.end();
	itor--;
	UIDialog*	pDlg = *itor;
	Assert( pDlg );

	if( pDlg->IsTopMost() && pDlg->IsShow() )
	{
		m_pTopDlg = pDlg;
		return;
	} //if
	itor++;

	while( 1 )
	{
		itor--;

		UIDialog*	pDlg = *itor;
		Assert( pDlg );

		if( !pDlg->IsAlways() && pDlg->IsShow() )
		{
			m_pTopDlg = pDlg;
			return;
		} //if

		if( itor == m_listDialog.begin() )
		{
			break;
		} //if
	} //while
	m_pTopDlg = NULL;
} //CW3DUInterface::ResetTopDlg

/**
 * ÇöÀç ÁÂÇ¥ÀÇ Dlg¸¦ ¾ò´Â´Ù.
 */
UIDialog* CW3DUInterface::GetFocusDlg()
{
	if( m_listDialog.empty() )		return NULL;

	LIST_PUIDLG::iterator	itor = m_listDialog.end();
	UIDialog*	pDlg;

	while( 1 )
	{
		itor--;

		pDlg = *itor;
		Assert( pDlg );

		if( pDlg->IsShow() )
		{
			if( pDlg->PointIn( m_nMouseX, m_nMouseY ) )
			{
				return pDlg;
			} //if
		} //if

		if( itor == m_listDialog.begin() )
		{
			break;
		} //if
	} //while

	return NULL;
} //CW3DUInterface::GetFocusDlg

/**
 * Æ÷Ä¿½º °¡Áø ÄÁÆ®·Ñ.
 */
UICComponent* CW3DUInterface::GetFocusUIComponent()
{
	UIDialog*		pDlg = GetFocusDlg();

	if( pDlg )
	{
		UICComponent*	pComponent = pDlg->GetFocus( m_nMouseX, m_nMouseY );

		if( pComponent )	return pComponent;
		else				return pDlg;
	} //if

	return NULL;
} //CW3DUInterface::GetFocusUIComponent

/**
 * Æ÷Ä¿½º È®ÀÎ.
 */
BOOL CW3DUInterface::IsFocus()
{
	if( m_pCurrCommopent	||
		m_pCurrEditor		||
		m_ItemMgr.GetHandInItemDat() )
	{
		return TRUE;
	} //if
	return FALSE;
} //CW3DUInterface::IsFocus

/**
 * ÄÁÆ®·² ÅØ½ºÆ® ¼³Á¤.
 */
BOOL CW3DUInterface::SetText( const char* szID, const char* szText, DWORD dwColor )
{
	UICComponent*	pComp = GetComponent( szID );
	if( !pComp )
	{
		Assert( !"szID¿¡ ´ëÇÑ ³»¿ëÀÌ ¾ø½À´Ï´Ù" ); 
		return FALSE;
	} //if

	switch( pComp->m_nType )
	{
	case UI_STRLIST:
		reinterpret_cast<UIStringList*>( pComp )->SetText( szText, dwColor );
		break;

	case UI_EDITTEXT:
		reinterpret_cast<UIEditText*>( pComp )->SetText( szText );
		break;

	case UI_STATICTEXT:
		reinterpret_cast<UIStaticText*>( pComp )->SetText( szText );
		break;

	case UI_PUSHBUTTON:
		reinterpret_cast<UIButton*>( pComp )->SetText( szText );
		break;
	} //switch

	return TRUE;
} //CW3DUInterface::SetText

/**
 * ÄÁÆ®·² ÅØ½ºÆ® ¼³Á¤.
 */
BOOL CW3DUInterface::SetText( const char* szID, int nDecimal, DWORD dwColor )
{
	char	szText[50];
	sprintf( szText, "%d", nDecimal );

	return SetText( szID, szText, dwColor );
} //CW3DUInterface::SetText

/**
 * ÄÁÆ®·² ÅØ½ºÆ® ¾ò´Â´Ù.
 */
const char* CW3DUInterface::GetText( const char* szID )
{
	UICComponent*	pComp = GetComponent( szID );
	if( !pComp )
	{
		Assert( !"szID¿¡ ´ëÇÑ ³»¿ëÀÌ ¾ø½À´Ï´Ù" ); 
		return FALSE;
	} //if

	switch( pComp->m_nType )
	{
	case UI_EDITTEXT:
		return reinterpret_cast<UIEditText*>( pComp )->GetText();

	case UI_STATICTEXT:
		return reinterpret_cast<UIStaticText*>( pComp )->GetText();

	case UI_PUSHBUTTON:
		return reinterpret_cast<UIButton*>( pComp )->GetText();
		
	case UI_SELECTSTRING:
		return reinterpret_cast<UISelectString*>( pComp )->GetText();
	} //switch

	return NULL;
} //CW3DUInterface::GetText

/**
 * ÄÁÆ®·² »èÁ¦.
 */
BOOL CW3DUInterface::ClearText( const char* szID )
{
	UICComponent*	pComp = GetComponent( szID );
	if( !pComp )
	{
		Assert( !"szID¿¡ ´ëÇÑ ³»¿ëÀÌ ¾ø½À´Ï´Ù" ); 
		return FALSE;
	} //if

	switch( pComp->m_nType )
	{
	case UI_EDITTEXT:
		reinterpret_cast<UIEditText*>( pComp )->Cleanup();
		return TRUE;

	default:
		return FALSE;
	} //switch
} //CW3DUInterface::ClearText

//
// CW3DUInterface::SetPicture
//
BOOL CW3DUInterface::SetPicture( const char* szID, const char* szPicName )
{
	Assert( szID );

	UIPicture*		pPic = (UIPicture*)GetComponent( szID );
	if( !pPic )
	{
		Assert( pPic ); 
		return FALSE;
	} //if

	MAP_STRSPRITE::iterator		itor = m_mapSprite.find( szPicName );
	if( itor != m_mapSprite.end() )
	{
		IW3DSprite*		pSprite = itor->second;
		Assert( pSprite );
		pPic->SetPicture( pSprite );
	}
	else
	{
		if( !pPic->Load( szPicName, SIZE_ICON_WIDTH, SIZE_ICON_HEIGHT ) )
		{
			Assert( !"Open Not The SpriteFile" );
			return FALSE;
		} //if
	} //if..else

	return TRUE;
} //CW3DUInterface::SetPicture

//
// CW3DUInterface::SetPicture
//
BOOL CW3DUInterface::SetPicture( const char* szID, IW3DSprite* pSprite )
{
	Assert( szID );

	UIPicture*		pPic = (UIPicture*)GetComponent( szID );

	if( !pPic )
	{
		return FALSE;
	}  //if

	pPic->SetPicture( pSprite );

	return TRUE;
}

/**
 * ÃÂÆÃ ÀÔ·Â.
 */
void CW3DUInterface::ChattingInput( 
	const string& strId, 
	const string& strContent, 
	DWORD dwColor, 
	BYTE  btFontIndex,
	BYTE  btGroup
	)
{
	char	szCompName[16];
	sprintf( szCompName, "CHK_CHATOFF_%d", btGroup );

	UICheckButton*		pCheckComp = reinterpret_cast<UICheckButton*>( GetComponent( szCompName ) );
	if( pCheckComp && pCheckComp->IsCheck() )
	{ //Cutoff
		return;
	} //if

	UIStringList*	pCharStrList = reinterpret_cast<UIStringList*>( GetComponent( "LIST_CHATTING" ) );
	UIStringList*	pMsgStrList = reinterpret_cast<UIStringList*>( GetComponent( "LIST_MSG" ) );
	if( !pCharStrList || !pMsgStrList )
	{
		Assert( !"Not Found The ChattListComp" );
		return;
	} //if

	string		strMessage;
	if( strId.size() > 0 )
	{
		strMessage = strId;
		strMessage.append( " : " );	
		strMessage.append( strContent );
		if( strcmp( strId.c_str() , STR_MAY_MAY ) != 0 )
			pCharStrList->SetText( strMessage.c_str(), dwColor );
		else
			pMsgStrList->SetText( strMessage.c_str(), dwColor );
	} //if
	else
	{
		strMessage = strContent;
		pMsgStrList->SetText( strMessage.c_str(), dwColor );
	}
	
} //CW3DUInterface::ChattingInput

//
// CW3DUInterface::SetProgressPos
//
BOOL CW3DUInterface::SetProgressPos( const char *szID, int nMax, int nPos, BOOL bProgress, DWORD dwColor )
{
	UICComponent*	pComp = GetComponent( szID );
	if( !pComp )
	{
		Assert( !"szID¿¡ ´ëÇÑ ³»¿ëÀÌ ¾ø½À´Ï´Ù" ); 
		return FALSE;
	} //if

	if( pComp->m_nType != UI_PROGRESS )
	{
		Assert( !"ÇÁ·Î±×·¹½º°¡ ¾Æ´Õ´Ï´Ù" );
		return FALSE;
	} //if

	UIProgress*		pProgress = reinterpret_cast<UIProgress*>( pComp );

	pProgress->SetMinMax( 0, nMax );
	pProgress->SetPos( nPos, bProgress );
	if( dwColor > 0 )
	{
		pProgress->SetColor( dwColor );
	} //if

	return TRUE;
}

//
// CW3DUInterface::SetPosHP
//
void CW3DUInterface::SetPosHP( int nMax, int nPos )
{
	if( nPos < 0 )
	{
		nPos = 0;
	} //if

	char	szHP[32];
	sprintf( szHP, "%d/%d", nPos, nMax );
	SetText( "TXT_CHAR_HP",  szHP );
	SetText( "TXT_CINFO_HP", szHP );

	SetProgressPos( "PRO_CINFO_HP", nMax, nPos, TRUE );
	UIProgress*		pHpComp = reinterpret_cast<UIProgress*>( GetComponent( "PRO_CHAR_HP" ) );
	if( !pHpComp )
	{
		Assert( !"¾ø´Â °ÔÀÌÁö" );
		return;
	} //if

	if( nPos < pHpComp->GetPos() )
	{
		UICComponent*	pRectangle = GetComponent( "RECT_CHAR_DAMAGE" );
		pRectangle->Enable( TRUE );
	} //if

	pHpComp->SetMinMax( 0, nMax );
	pHpComp->SetPos( nPos );
}

//
// CW3DUInterface::SetPosMP
//
void CW3DUInterface::SetPosMP( int nMax, int nPos )
{
	SetProgressPos( "PRO_CINFO_MP", nMax, nPos, TRUE );
	SetProgressPos( "PRO_CHAR_MP", nMax, nPos, TRUE );

	char	szMP[32];
	sprintf( szMP, "%d/%d", nPos, nMax );
	SetText( "TXT_CINFO_MP", szMP );
	SetText( "TXT_CHAR_MP", szMP );
} //CW3DUInterface::SetPosMP

//
// CW3DUInterface::SetPosSkill
//
void CW3DUInterface::SetPosSkill( int nSkill, int nMax, int nPos, BOOL bProgress )
{
	if( nSkill < 0 || nSkill > 3 )
	{
		Assert( !"¸ÂÁö¾Ê´Â ½ºÅ³¿µ¿ª" );
		return;
	} //if

	UIProgress*		pSkill[4];
	pSkill[0] = reinterpret_cast<UIProgress*>( GetComponent( "PRO_CINFO_SK1" ) );
	pSkill[1] = reinterpret_cast<UIProgress*>( GetComponent( "PRO_CINFO_SK2" ) );
	pSkill[2] = reinterpret_cast<UIProgress*>( GetComponent( "PRO_CINFO_SK3" ) );
	pSkill[3] = reinterpret_cast<UIProgress*>( GetComponent( "PRO_CINFO_SK4" ) );

	if( !pSkill[0] || !pSkill[1] || !pSkill[2] || !pSkill[3] )
	{
		Assert( !"¾ø´Â ÄÞÆ÷°¡ ÀÖ´Ù" );
		return;
	} //if

	pSkill[nSkill]->SetMinMax( 0, nMax );	
	pSkill[nSkill]->SetPos( nPos, bProgress );

	if( m_nMainSkill == nSkill )
	{
		UIProgressEx*	pMainGraph = reinterpret_cast<UIProgressEx*>( GetComponent( "PRO_SKILLGRAPH" ) );
		Assert( pMainGraph );
		pMainGraph->SetMinMax( 0, nMax );
		pMainGraph->SetPos( nPos, bProgress );
	} //if
}

/**
 * ÇÃ·¹ÀÌ¾î ·¹º§.
 */
void CW3DUInterface::SetPlayerPower( const int nType, const int nLevel, BOOL btIs )
{
	switch( nType )
	{
		case 0 :	SetText( "TXT_CINFO_STR", nLevel );		break;
			case 1 :	SetText( "TXT_CINFO_INT", nLevel );		break;
			case 2 :	SetText( "TXT_CINFO_DEX", nLevel );		break;
		
	} //switch

	UIInventoryIN*	pInven;
	UIInventoryON*	pEquip;
	UISkillEntry*	pSkillEntry;
	pInven = (UIInventoryIN*)GetComponent( "INVENTORY_IN_1" );
	Assert( pInven );
	pInven->SetPlayerPower( nType, nLevel );
	pInven = (UIInventoryIN*)GetComponent( "INVENTORY_IN_2" );
	Assert( pInven );
	pInven->SetPlayerPower( nType, nLevel );

	pInven = (UIInventoryIN*)GetComponent( "SHOPINVENTORY_1" );
	Assert( pInven );
	pInven->SetPlayerPower( nType, nLevel );
	pInven = (UIInventoryIN*)GetComponent( "SHOPINVENTORY_2" );
	Assert( pInven );
	pInven->SetPlayerPower( nType, nLevel );
	pInven = (UIInventoryIN*)GetComponent( "SHOPINVENTORY_3" );
	Assert( pInven );
	pInven->SetPlayerPower( nType, nLevel );
	
	pInven = (UIInventoryIN*)GetComponent( "INVENTORY_DEALME_1" );
	Assert( pInven );
	pInven->SetPlayerPower( nType, nLevel );
	pInven = (UIInventoryIN*)GetComponent( "INVENTORY_DEALME_2" );
	Assert( pInven );
	pInven->SetPlayerPower( nType, nLevel );

	pInven = (UIInventoryIN*)GetComponent( "INVENTORY_DEALYOU_1" );
	Assert( pInven );
	pInven->SetPlayerPower( nType, nLevel );
	pInven = (UIInventoryIN*)GetComponent( "INVENTORY_DEALYOU_2" );
	Assert( pInven );
	pInven->SetPlayerPower( nType, nLevel );

	pInven = (UIInventoryIN*)GetComponent( "INVEN_STORAGE_1" );
	Assert( pInven );
	pInven->SetPlayerPower( nType, nLevel );
	pInven = (UIInventoryIN*)GetComponent( "INVEN_STORAGE_1" );
	Assert( pInven );
	pInven->SetPlayerPower( nType, nLevel );

	pEquip = (UIInventoryON*)GetComponent( "INVENTORY_ON" );
	Assert( pEquip );
	pEquip->SetPlayerPower( nType, nLevel );

	pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_SHORT" ) );
	Assert( pSkillEntry );
	pSkillEntry->SetPlayerPower( nType, nLevel );
	pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_LONG" ) );
	Assert( pSkillEntry );
	pSkillEntry->SetPlayerPower( nType, nLevel );
	pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_WHITE" ) );
	Assert( pSkillEntry );
	pSkillEntry->SetPlayerPower( nType, nLevel );
	pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_BLACK" ) );
	Assert( pSkillEntry );
	pSkillEntry->SetPlayerPower( nType, nLevel );


} //CW3DUInterface::SetSkillLevel

/**
 * ¸ó½ºÅÍ hp º¸ÀÌ±â.
 */
void CW3DUInterface::ShowTargetInfo( int nId, int nMax, int nPos, const char* szText )
{
	if( nId )
	{
		ShowDlg( "DLG_MONSTERHP", TRUE );
		SetProgressPos( "PRO_MONSTER_HP", nMax, nPos, FALSE );
		SetText( "TXT_MONSTER_NAME", szText );
	}
	else
	{
		ShowDlg( "DLG_MONSTERHP", FALSE );
	} //if..else
} //CW3DUInterface::ShowMonsterHP

/** 
 * ÀÎÅÍÆäÀÌ½º ÆËÀÎº¥Æ®.
 */
BOOL CW3DUInterface::PopEvent( char* szUIName, UIEVENT* pEvent, DWORD* pdwParam1, DWORD* pdwParam2 )
{
	return ::PopEvent( szUIName, pEvent, pdwParam1, pdwParam2 );
} //CW3DUInterface::PopEvent

/**
 *
 */
BOOL PopEvent( char* szUIName, UIEVENT* pEvent, DWORD* pdwParam1, DWORD* pdwParam2 )
{
	if( g_queueEvent.empty() )
	{
		*pEvent = 0;
		return FALSE;
	} //if

	static UIEvent		s_Event;

	s_Event = g_queueEvent.front();
	g_queueEvent.pop();

	if( !s_Event.pCtrlName || strlen( s_Event.pCtrlName ) > 64 )
	{
		szUIName[0] = '\0';
	}
	else
	{
		strcpy( szUIName, s_Event.pCtrlName );
	} //if..else

	*pEvent			= s_Event.nEvent;
	*pdwParam1		= s_Event.dwParam1;
	*pdwParam2		= s_Event.dwParam2;

	return TRUE;
} //PopEvent

/**
 *
 */
BOOL PushEvent( const char* szCtrlName, UIEVENT nEvent, DWORD dwParam1, DWORD dwParam2 )
{
	static UIEvent		s_event;

	if( s_event.pCtrlName == szCtrlName &&
		s_event.nEvent	  == nEvent	    &&
		s_event.dwParam1  == dwParam1	&&
		s_event.dwParam2  == dwParam2 )
	{	// ¹æ±ÝÀü ÀÌº¥Æ®¿Í µ¿ÀÏÇÒ °æ¿ì
		return FALSE;
	}

	if( g_queueEvent.size() > MAX_EVENTNUM )
	{
		g_queueEvent.pop();	
	} //if
	s_event.pCtrlName	= szCtrlName;
	s_event.nEvent		= nEvent;
	s_event.dwParam1	= dwParam1;
	s_event.dwParam2	= dwParam2;

	g_queueEvent.push( s_event );

	return TRUE;
} //PushEvent

/**
 * ¸ÖÆ¼ ¹ÙÀÌÆ® ¹®ÀÚÈ®ÀÎ
 */
BOOL CW3DUInterface::IsDoubleByteCharset( const char* pBuff )
{
	return ::IsDoubleByteCharset( pBuff );
} //CW3DUInterface::IsDoubleByteCharset

/**
 * ¸Þ¼¼Áö ¹Ú½º Ãâ·Â
 */
BOOL CW3DUInterface::MessageBox( const char* szText, char *szKey, int nType )
{
	UIStaticText*	pStaticTxt = NULL;
	UIDialog*		pDlg = NULL;
	UIButton*		pButtonYes;
	UIButton*		pButtonNo;
	UIButton*		pButtonOk;
	char			szBtnName[64];

	if( m_szMegKey[0] == '\0' || !szKey )
	{
		strcpy( m_szMegKey, "BTN_MSG" );
	}

	UIDialog*	pMsgDlg = reinterpret_cast<UIDialog*>( GetComponent( "DLG_MESSAGEBOX" ) );
	if( !pMsgDlg )
	{
		Assert( !"¸Þ¼¼Áö ¹Ú½º ¾îµø¾î" );
		return FALSE;
	} //if

	switch( nType )
	{
		case MSG_YESNO:
			pButtonYes = (UIButton*)GetComponent( "BTN_MSG_YES" );
			pButtonNo  = (UIButton*)GetComponent( "BTN_MSG_NO" );
			pButtonOk  = (UIButton*)GetComponent( "BTN_MSG_OK" );
			if( !pButtonYes || !pButtonNo || !pButtonOk )
			{
				Assert( !"¾ø´ç" );
				return FALSE;
			}
			if( szKey )
			{
				sprintf( m_szMegKey, "%s%s", "BTN_MSG_", szKey );
			}
			sprintf( szBtnName, "%s%s", m_szMegKey, "_YES" );
			pButtonYes->m_strName = szBtnName;
			sprintf( szBtnName, "%s%s", m_szMegKey, "_NO" );
			pButtonNo->m_strName = szBtnName;
			sprintf( szBtnName, "%s%s", m_szMegKey, "_OK" );
			pButtonOk->m_strName = szBtnName;

			SetPicture( "PIC_MSG_ICON", "SPRITE_QUESTION_ICON" );

			pButtonYes->Enable( TRUE );
			pButtonNo->Enable( TRUE );
			pButtonOk->Enable( FALSE );

			pMsgDlg->SetShowTick( 0 );
			break;

		case MSG_OK:
			pButtonYes = (UIButton*)GetComponent( "BTN_MSG_YES" );
			pButtonNo  = (UIButton*)GetComponent( "BTN_MSG_NO" );
			pButtonOk  = (UIButton*)GetComponent( "BTN_MSG_OK" );
			if( !pButtonYes || !pButtonNo || !pButtonOk )
			{
				Assert( !"¾ø´ç" );
				return FALSE;
			}
			if( szKey )
			{
				sprintf( m_szMegKey, "%s%s", "BTN_MSG_", szKey );
			}
			sprintf( szBtnName, "%s%s", m_szMegKey, "_YES" );
			pButtonYes->m_strName = szBtnName;
			sprintf( szBtnName, "%s%s", m_szMegKey, "_NO" );
			pButtonNo->m_strName = szBtnName;
			sprintf( szBtnName, "%s%s", m_szMegKey, "_OK" );
			pButtonOk->m_strName = szBtnName;

			SetPicture( "PIC_MSG_ICON", "SPRITE_EXCLAMATION_ICON" );
			
			pButtonYes->Enable( FALSE );
			pButtonNo->Enable( FALSE );
			pButtonOk->Enable( TRUE );

			pMsgDlg->SetShowTick( 0 );
			break;

		case MSG_NONE:
			pButtonYes = (UIButton*)GetComponent( "BTN_MSG_YES" );
			pButtonNo  = (UIButton*)GetComponent( "BTN_MSG_NO" );
			pButtonOk  = (UIButton*)GetComponent( "BTN_MSG_OK" );
			if( !pButtonYes || !pButtonNo || !pButtonOk )
			{
				Assert( !"¾ø´ç" );
				return FALSE;
			}
			if( szKey )
			{
				sprintf( m_szMegKey, "%s%s", "BTN_MSG_", szKey );
			}
			sprintf( szBtnName, "%s%s", m_szMegKey, "_YES" );
			pButtonYes->m_strName = szBtnName;
			sprintf( szBtnName, "%s%s", m_szMegKey, "_NO" );
			pButtonNo->m_strName = szBtnName;
			sprintf( szBtnName, "%s%s", m_szMegKey, "_OK" );
			pButtonOk->m_strName = szBtnName;

			SetPicture( "PIC_MSG_ICON", "SPRITE_EXCLAMATION_ICON" );

			pButtonYes->Enable( FALSE );
			pButtonNo->Enable( FALSE );
			pButtonOk->Enable( FALSE );

			pMsgDlg->SetShowTick( 1500 );
			break;
	}

	pStaticTxt = (UIStaticText*)GetComponent( "TXT_MSG_TEXT" );
	if( !pStaticTxt )
	{
		Assert( !"TXT_MSG_TEXT Not Found" );
		return FALSE;
	}
	pStaticTxt->SetText( szText );

	ShowDlg( "DLG_MESSAGEBOX", TRUE );

	return TRUE;
}

//
// CW3DUInterface::IsShowMessageBox
//
BOOL CW3DUInterface::IsShowMessageBox()
{
	UIDialog*		pDlg = reinterpret_cast<UIDialog*>( GetComponent( "DLG_MESSAGEBOX" ) );
	Assert( pDlg );

	if( pDlg->IsShow() )
	{
		return TRUE;
	} //if

	return FALSE;
}

//
// CW3DUInterface::BeginNPC
//
BOOL CW3DUInterface::BeginNPC( IW3DNPCScript* pNPCScript, const int nTalkNum )
{
	if( !pNPCScript )
	{
		Assert( pNPCScript );
		return FALSE;
	}

	UIStaticText*		pUISText = (UIStaticText*)GetComponent( "TXT_SAY_TEXT" );
	UISelectString*		pUISStr  = (UISelectString*)GetComponent( "SSTR_NPC_ANSWER" );

	if( !pUISStr || !pUISText )
	{
		Assert( 0 );
		return FALSE;
	} //if

	pUISStr->Cleanup();
	pUISText->Cleanup();

	if( !pNPCScript->FirstTalk( nTalkNum ) )
	{
		return FALSE;
	} //if

	char*		pText = NULL;
	int			nAskNum = pNPCScript->GetAskNum();

	if( nAskNum == -1 )
	{
		return FALSE;
	}

	pText = (char*)pNPCScript->GetText();
	pUISText->SetText( pText );

	for( int nCnt = 0; nCnt < nAskNum; nCnt++ )
	{
		pText = (char*)pNPCScript->GetAskText( nCnt );
		if( pText )
		{
			pUISStr->AddString( pText );
		} //if
	} //for

	if( nAskNum > 0 )
	{
		ShowDlg( "DLG_NPC_ANSWER", TRUE );
	} //if

	ShowDlg( "DLG_NPC_SAY", TRUE );
	UIButton*			pUIBtn		= (UIButton*)GetComponent( "BTN_SAY_OK" );
	if( !pUIBtn )
	{
		Assert( !"¾ø´Â ¹öÆ°" );
		return FALSE;
	} //if

	if( pNPCScript->IsNextTalk() )		pUIBtn->Show( TRUE );
	else								pUIBtn->Show( FALSE );

	return TRUE;
}

/**
 *
 */
BOOL CW3DUInterface::NextNPC( IW3DNPCScript* pNPCScript, int nAnswer )
{
	if( !pNPCScript )
	{
		Assert( pNPCScript );
		return FALSE;
	} //if

	UIStaticText*		pUISText	= (UIStaticText*)GetComponent( "TXT_SAY_TEXT" );
	UISelectString*		pUISStr		= (UISelectString*)GetComponent( "SSTR_NPC_ANSWER" );
	UIButton*			pUIBtn		= (UIButton*)GetComponent( "BTN_SAY_OK" );

	if( !pUISStr || !pUISText || !pUIBtn )
	{
		Assert( !"ÄÄÆ÷³ÍÆ® ¾ò±â ½ÇÆÐ" );
		return FALSE;
	} //if

	pUISStr->Cleanup();
	pUISText->Cleanup();

	if( !pNPCScript->NextTalk( nAnswer ) )
	{
		ShowDlg( "DLG_NPC_ANSWER", FALSE );
		ShowDlg( "DLG_NPC_SAY", FALSE );
		return TRUE;
	} //if

	char*	szText = (char*)pNPCScript->GetText();
	int		nAskNum = pNPCScript->GetAskNum();

	pUISText->SetText( szText );

	for( int nCnt = 0 ; nCnt < nAskNum ; nCnt++ )
	{
		szText = (char*)pNPCScript->GetAskText( nCnt );
		if( szText )
		{
			pUISStr->AddString( szText );
		} //if
	} //for

	if( nAskNum > 0 )
	{
		ShowDlg( "DLG_NPC_ANSWER", TRUE );
		pUIBtn->Show( FALSE );
	}
	else
	{
		ShowDlg( "DLG_NPC_ANSWER", FALSE );
		pUIBtn->Show( TRUE );
	} //if..else..

	ShowDlg( "DLG_NPC_SAY", TRUE );
	
	if( pNPCScript->IsNextTalk() )		pUIBtn->Show( TRUE );
	else								pUIBtn->Show( FALSE );

	return TRUE;
} //CW3DUInterface::NextNPC

/**
 *
 */
void CW3DUInterface::InitCharSelectInfo()
{
	m_CharSelInfo.Cleanup();
	SetText( "EDT_CHARNEW_NAME", "" );
} //CW3DUInterface::InitCharSelectInfo

/**
 * ÄÉ¸¯ÅÍ ¼¿·ºÆ®½Ã ·¹º§Á¤º¸.
 */
void CW3DUInterface::SetCharInfo( SCharInfo* pCharInfo )
{
	if( pCharInfo )
	{
		SetText( "TXT_CHARSELINFO_NAME", pCharInfo->szName );
		SetText( "TXT_CHARSELINFO_LEVEL", pCharInfo->nLevel );
		SetText( "TXT_CHARSELINFO_SHORT", pCharInfo->nSLevel );
		SetText( "TXT_CHARSELINFO_LONG",  pCharInfo->nLLevel );
		SetText( "TXT_CHARSELINFO_BLACK", pCharInfo->nBLevel );
		SetText( "TXT_CHARSELINFO_WHITE", pCharInfo->nWLevel );

		char		szClassName[32];
		GetClassName( szClassName, pCharInfo->nClass );
		SetText( "TXT_CHARSELINFO_JOB", szClassName );
		SetCharacterFace();
		SetCharInfoEmptyButton( pCharInfo->bEmpty );
	}
	else
	{
		SetCharInfoEmptyButton( TRUE );
	} //if
} //CW3DUInterface::SetCharInfo

/**
 *
 */
void CW3DUInterface::SetCharInfoEmptyButton( BOOL bEmpty )
{
	UIButton*	pCreateButton = (UIButton*)GetComponent( "BTN_CHARSEL_CREATE" );
	UIButton*	pDeleteButton = (UIButton*)GetComponent( "BTN_CHARSEL_DELETE" );
	UIButton*	pSelectButton = (UIButton*)GetComponent( "BTN_CHARSEL_SELECT" );

	

	if( !pCreateButton || !pDeleteButton || !pSelectButton )
	{
		Assert( !"¾ø´Ù" );
		return;
	} //if

	if( bEmpty )
	{
		pCreateButton->Enable( TRUE );
		pDeleteButton->Enable( FALSE );
		pSelectButton->Enable( FALSE );
	}
	else
	{
		pCreateButton->Enable( FALSE );
		pDeleteButton->Enable( TRUE );
		pSelectButton->Enable( TRUE );
	} //if..else..
		
	RefreshNewCharacter();
} //CW3DUInterface::SetCharInfoEmptyButton

/**
 *
 */
BOOL CW3DUInterface::GetCharSelInfo(
	char*	szName,
	BYTE*	pSex,
	WORD*	pHair,
	WORD*	pFace,
	WORD*	pCoat, WORD *pPants, WORD *pHand, WORD *pFoot,
	int*	pIndex,
	BYTE*	pSTR, BYTE* pINT, BYTE* pDEX,
	BYTE*	pClass
	)
{
	if( !szName || !pSex || !pHair || !pFace || !pCoat || !pPants ||
		!pSTR || !pINT || !pDEX || !pClass )
	{
		Assert( !"Output ÀÎÀÚ·Î NULL Æ÷ÀÎÅÍ°¡ ³Ñ¾î¿ÔÀ½." );
		return FALSE;
	} //if

	SCharInfo*	pCharInfo = m_CharSelInfo.GetCharInfo();
	Assert( pCharInfo );

	strcpy( szName, pCharInfo->szName );
	*pSex	= pCharInfo->btSex;
	*pHair	= pCharInfo->wHair;
	*pFace	= pCharInfo->wFace;
	*pCoat	= pCharInfo->wCoat;
	*pPants	= pCharInfo->wPants;
	*pHand	= pCharInfo->wHand;
	*pFoot	= pCharInfo->wFoot;
	*pIndex = m_CharSelInfo.GetIndex();
	*pSTR	= pCharInfo->nSTR;
	*pDEX	= pCharInfo->nDEX;
	*pINT	= pCharInfo->nINT;
	*pClass = pCharInfo->nClass;

	return !pCharInfo->bEmpty;
} //CW3DUInterface::GetCharSelInfo

/**
 * Ä³¸¯ÅÍ »ý¼º Á¤º¸¿¡ ¹®Á¦ ¾ø´ÂÁö È®ÀÎ.
 */
BOOL CW3DUInterface::VerifyCharacterSet( const char* szChar )
{
	if( strlen( szChar ) <= 0 )
	{
		MessageBox( STR_UIMSG_INPUTNAME );
		return FALSE;
	} //if

	if( IsExcepChar( szChar ) || !CheckUseID( szChar ) )
	{
		MessageBox( STR_SYSMSG_EXCEPTSTRING );
		return FALSE;
	} //if

	return TRUE;
} //CW3DUInterface::VerifyCharacterSet

/**
 *
 */
BOOL CW3DUInterface::GetCharSelModelName(
	const int nIndex,	///< [IN]
	char*	szName,		///< [OUT]
	BYTE*	pSex,		///< [OUT]
	char*	szHair,		///< [OUT]
	char*	szFace,		///< [OUT]
	char*	szCoat,		///< [OUT]
	char*	szPants,	///< [OUT]
	char*	szHand,		///< [OUT]
	char*	szFoot		///< [OUT]
	)
{
	if( !szName || !pSex || !szHair || !szFace || !szCoat || !szPants )
	{
		Assert( !"Output ÀÎÀÚ·Î NULL Æ÷ÀÎÅÍ°¡ ³Ñ¾î¿ÔÀ½." );
		return FALSE;
	} //if

	int			nPreIndex = m_CharSelInfo.GetIndex();
	m_CharSelInfo.SetIndex( nIndex );
	SCharInfo*	pCharInfo = m_CharSelInfo.GetCharInfo();
	m_CharSelInfo.SetIndex( nPreIndex );
	Assert( pCharInfo );
	
	strcpy( szName, pCharInfo->szName );
	*pSex = pCharInfo->btSex;

	SItemInfoData*		pItemInfo;
	pItemInfo = m_pItemInfoMgr->GetItemInfo( pCharInfo->wFace );
	if( !pItemInfo )
	{
 		Assert( !"¾ø´Â ¾ó±¼" );
		return FALSE;
	} //if
	strcpy( szFace, ITEMFILENAME1( pCharInfo->btSex, pItemInfo ) );

	char	cRaceNum = *(szFace + strlen( szFace ) - 7);

	pItemInfo = m_pItemInfoMgr->GetItemInfo( pCharInfo->wHair );
	if( !pItemInfo )
	{
		Assert( !"¾ø´Â Çì¾î" );
		return FALSE;
	} //if
	m_CharSelInfo.GetModel1FileName( pCharInfo->btSex, szHair, pItemInfo, cRaceNum );

	pItemInfo = m_pItemInfoMgr->GetItemInfo( pCharInfo->wHand );
	if( !pItemInfo )
	{
		Assert( !"¾ø´Â Àå°©" );
		return FALSE;
	} //if
	m_CharSelInfo.GetModel1FileName( pCharInfo->btSex, szHand, pItemInfo, cRaceNum );

	pItemInfo = m_pItemInfoMgr->GetItemInfo( pCharInfo->wCoat );
	if( !pItemInfo )
	{
		Assert( !"¾ø´Â »óÀÇ" );
		return FALSE;
	} //if
	m_CharSelInfo.GetModel1FileName( pCharInfo->btSex, szCoat, pItemInfo, cRaceNum );

	pItemInfo = m_pItemInfoMgr->GetItemInfo( pCharInfo->wPants );
	if( !pItemInfo )
	{
		Assert( !"¾ø´Â ÇÏÀÇ" );
		return FALSE;
	} //if
	m_CharSelInfo.GetModel1FileName( pCharInfo->btSex, szPants, pItemInfo, cRaceNum );

	pItemInfo = m_pItemInfoMgr->GetItemInfo( pCharInfo->wFoot );
	if( !pItemInfo )
	{
		Assert( !"¾ø´Â ½Å¹ß" );
		return FALSE;
	} //if
	m_CharSelInfo.GetModel1FileName( pCharInfo->btSex, szFoot, pItemInfo, cRaceNum );

	return !pCharInfo->bEmpty;
} //CW3DUInterface::GetCharSelModelName

/**
 *
 */
void CW3DUInterface::SetCharSelInfo(
	const char*	szName,		///< [IN]
	const int	nLevel,		///< [IN]
	const int	nLLevel,	///< [IN]
	const int	nSLevel,	///< [IN]
	const int	nBLevel,	///< [IN]
	const int	nWLevel,	///< [IN]
	const BYTE	btSex,		///< [IN]
	const WORD	wHair,		///< [IN]
	const WORD	wFace,		///< [IN]
	const WORD	wCoat,		///< [IN]
	const WORD	wPants,		///< [IN]
	const WORD	wHand,		///< [IN]
	const WORD	wFoot,		///< [IN]
	const BYTE	btClass,	///< [IN]
	const int	nIndex		///< [IN]
	)
{
	if( !m_CharSelInfo.SetIndex( nIndex ) )
	{
		Assert( !"¹üÀ§¿¡ ¸ÂÁö ¾Ê´Âµí..." );
		return;
	} //if

	m_CharSelInfo.SetCharInfo( szName, nLevel, nLLevel, nSLevel, nBLevel, nWLevel,
							   btSex, wHair, wFace, wCoat, wPants, wHand, wFoot, btClass );
} //CW3DUInterface::SetCharSelInfo

/**
 * Ä³¸¯ÅÍ »ý¼º
 */
int	CW3DUInterface::CreateCharacter()
{
	return m_CharSelInfo.New();
} //CW3DUInterface::CreateCharacter

/**
 *
 */
int CW3DUInterface::GetCharSelectNum()
{
	return m_CharSelInfo.GetIndex();
} //CW3DUInterface::GetCharSelectNum

/**
 *
 */
void CW3DUInterface::DeleteCharInfo( const int nIndex )
{ // to here
	m_CharSelInfo.DeleteCharInfo( nIndex );
	SCharInfo*	pCharInfo = m_CharSelInfo.GetCharInfo();
	SetCharInfo( pCharInfo );
} //CW3DUInterface::DeleteCharInfo

/**
 *
 */
void CW3DUInterface::SetCharacterFace()
{
	SCharInfo*		pCharInfo = m_CharSelInfo.GetCharInfo();

	m_CharFace.Create(
		pCharInfo->wFace,
		pCharInfo->wHair,
		pCharInfo->wCoat,
		pCharInfo->btSex );

	IW3DSprite*		psprFace	= m_CharFace.GetImageFace();
	IW3DSprite*		psprHair	= m_CharFace.GetImageHair();
	IW3DSprite*		psprTorso	= m_CharFace.GetImageTorso();

	Assert( psprFace );
	Assert( psprHair );
	Assert( psprTorso );

	SetPicture( "PIC_CHAR_FACE",  psprFace );
	SetPicture( "PIC_CHAR_HAIR",  psprHair );
	SetPicture( "PIC_CHAR_TORSO", psprTorso );
} //CW3DUInterface::SetCharacterFace

/**
 * NPC ½ºÅ©¸³Æ®¸¦ ¼±ÅÃÇÑ´Ù.
 */
void CW3DUInterface::SelectNPC(
	const int nAddr,		///< [IN] NPC ¹øÈ£
	const int nTalkNum		///< [IN] ´ë»ç ¹øÈ£.
	)
{
	Assert( m_pNPCScript );

	if( !m_pNPCScript->OpenNPC( nAddr ) )
	{
		return;
	} //if

	m_bNPCTalkMode = 0;			///< '0' - ÀÏ¹Ý´ëÈ­, '1' - Äù½ºÆ®, '2' - ´ëÀåÀåÀÌ;
	m_NPCNum = nAddr;
	BeginNPC( m_pNPCScript, nTalkNum );
} //CW3DUInterface::SelectNPC

/**
 * SmithNPC ½ºÅ©¸³Æ®¸¦ ¼±ÅÃÇÑ´Ù.
 */
void CW3DUInterface::SelectSmithNPC( const int nAddr, const int nTalkNum )
{
	/*Assert( m_pNPCScript );

	if( !m_pNPCScript->OpenNPC( nAddr ) )
	{
		return;
	}

	m_bNPCTalkMode = 2;			///< '0' - ÀÏ¹Ý´ëÈ­, '1' - Äù½ºÆ®, '2' - ´ëÀåÀåÀÌ;
	BeginNPC( m_pNPCScript, nTalkNum );*/
	m_NPCNum = nAddr;
/*	ShowDlg( "DLG_NPC_SMITH_SAY", TRUE );
	if( m_NPCNum == 3 )
	{
		SetText( "TXT_SMITH_SAY_TEXT", STR_TEST_SAY_WP );
	}
	else
	{
		SetText( "TXT_SMITH_SAY_TEXT", STR_TEST_SAY_AM );
	}*/

}

void CW3DUInterface::SelectCardSumNPC( const int nAddr, const int nTalkNum )
{
	ShowDlg( "DLG_CARDENCHANT", TRUE );
	SetText( "TXT_CARD_SAY_TEXT", STR_TEST_SAY_SUM );
}

void CW3DUInterface::SelectCardUndoNPC( const int nAddr, const int nTalkNum )
{
	ShowDlg( "DLG_CARDENCHANTUNDO", TRUE );
	SetText( "TXT_CARDUNDO_SAY_TEXT", STR_UNDOCARD );
}

void CW3DUInterface::SelectServer( const int nAddr )
{
	ShowDlg( "DLG_YOUANDME", FALSE );
	ShowDlg( "DLG_SERVER", TRUE );
}
	
/**
 * NPC ÀÌ¸§À» ¾ò´Â´Ù.
 */
const char* CW3DUInterface::GetNPCName(
	const int nAddr ///< [IN] NPC ¹øÈ£
	)
{
	Assert( m_pNPCScript );

	char*	pName = NULL;
	if( m_pNPCScript->OpenNPC( nAddr ) )
	{
		pName = const_cast<char*>( m_pNPCScript->GetNPCName() );
	} //if
	return pName;
} //CW3DUInterface::GetNPCName

/**
 * ÇÃ·¹ÀÌ¾î À§Ä¡¸¦ ¼³Á¤ÇÑ´Ù.
 */
void CW3DUInterface::SetPlayerMapPos( const int nX, const int nY )
{
	m_pMiniMap->SetPoint( nX, nY );
	m_pLargeMap->SetPoint( nX, nY );
} //CW3DUInterface::SetPlayerMapPos

/**
 * ÇÃ·¹ÀÌ¾î ¹æÇâÀ» ¼³Á¤.
 */
void CW3DUInterface::SetPlayerMapDir( const float fDirX, const float fDirY )
{
	float		fAngle = atan2( fDirX, fDirY );
	fAngle = fAngle * 180 / MATH_PI;

	m_pMiniMap->SetDirection( fAngle );
	m_pLargeMap->SetDirection( fAngle );
} //CW3DUInterface::SetPlayerMapDir

/**
 * »óÁ¡ À§Ä¡¸¦ ¼³Á¤ÇÑ´Ù.
 */
void CW3DUInterface::AddShopMapPos( const int nX, const int nY )
{
	m_pMiniMap->AddShopPoint( nX, nY );
	m_pLargeMap->AddShopPoint( nX, nY );
} //CW3DUInterface::AddShopNPCMapPos

/**
 * °¡Áø µ·.
 */
void CW3DUInterface::UpdateMoney( int nMoney )
{
	m_nGold = nMoney;

	SetText( "TXT_INVEN_GOLD", GetUnitNum( nMoney ) );
} //CW3DUInterface::UpdateMoney

void CW3DUInterface::UpdateCash( int nCash )
{
	m_nCash = nCash;

	SetText( "TXT_INVEN_CASH", GetUnitNum( nCash ) );
}
/**
 * ´ÜÀ§·Î ³ª´« ¹®ÀÚ¿­
 */
char* CW3DUInterface::GetUnitNum( const DWORD dwNum )
{ 
	static char		s_szBuff[32];

	char			szTemp[32];
	sprintf( szTemp, "%d", dwNum );

	int		nUnit = strlen( szTemp ) / 3;
	if( ( strlen( szTemp ) % 3 ) == 0 )
	{
		nUnit--;
	} //if

	strcpy( s_szBuff, szTemp );
	if( nUnit > 0 )
	{
		char*	pSrcPtr  = szTemp + strlen( szTemp );
		char*	pDestPtr = s_szBuff + strlen( szTemp ) + nUnit;
		*pDestPtr = '\0';

		for( int nCnt = nUnit; nCnt > 0; nCnt-- )
		{
			pSrcPtr		-= 3;
			pDestPtr	-= 3;
			memcpy( pDestPtr, pSrcPtr, 3 );
			pDestPtr--;
			*pDestPtr = ',';
		} //for
	} //if

	return s_szBuff;
} //CW3DUInterface::GetUnitNum

/**
 * °ÔÀÓ ÁØºñ »óÅÂ¸¦ ·ÎµåÇÑ´Ù.
 */
BOOL CW3DUInterface::LoadGameReady( const WORD wMap )
{
	m_GameReady.LoadImage( wMap );

	return TRUE;
} //CW3DUInterface::LoadImage

/**
 * °ÔÀÓ ÁØºñ »óÅÂ¸¦ Ãâ·ÂÇÑ´Ù.
 *
 * @param	dwTick		[IN] °æ°ú tick
 */
void CW3DUInterface::DrawGameReady( const DWORD dwTick )
{
	m_GameReady.Draw();
	return;
} //CW3DUInterface::Draw

/**
 * ³»¿ëÀ» ¾÷µ¥ÀÌÆ®ÇÑ´Ù.
 *
 * @param	dwTick		[IN] °æ°ú tick
 *
 * @return	User Interface¿¡ Æ÷Ä¿½º°¡ ÀÖ´Ù¸é TRUE¸¦ ¸®ÅÏÇÕ´Ï´Ù. ¾Æ´Ï¸é FALSE.
 */
BOOL CW3DUInterface::Update( const DWORD dwTick )
{
	m_dwTick = dwTick;

	MouseMove();
	
	if( !m_pCurrEditor )
	{ // ÇöÀç ¹®ÀÚ ÀÔ·ÂÁßÀÌ ¾Æ´Ï¸é..
		HotKey();
	} //if

	m_GameReady.Update( dwTick );
	
	m_EnchantCard.Update( dwTick );				///< ÀÎÃ¾Æ® Ä«µå

	// Mouse Message¸¦ Ã³¸®.

	if( m_pCurrCommopent )
	{
		if( m_pCurrCommopent->m_nType == UI_QUICKSLOT )
			QuickSlot_HoverMouse( m_pCurrCommopent );
	}

	m_dwAccTick += dwTick;
	if( m_pInput->GetMouseL() && !m_bLButtonPressed )
	{
		m_bLButtonPressed = TRUE;
		if( m_dwAccTick - m_dwDoubleClick_L < TICK_DOUBLECLICK )
		{
			int		nMoveX = abs( m_nMouseX - m_nOldMouseX );
			int		nMoveY = abs( m_nMouseY - m_nOldMouseY );

			if( nMoveX < 3 && nMoveY < 3 )
			{
				if( MouseDBClickL() )
				{ // ´õºí Å¬¸¯ Ã³¸®.
					return TRUE;
				} //if
			} //if
		}
		
		if( MouseButtonL_Down() )
		{
			return TRUE;
		} //if
	}
	else if( !m_pInput->GetMouseL() && m_bLButtonPressed )
	{
		m_bLButtonPressed = FALSE;
		m_dwAccTick = 0;
		m_dwDoubleClick_L = m_dwAccTick;

		BOOL	bProc = MouseButtonL_Up();
		m_ItemMgr.SetHandInItemDat( NULL, FROM_NULL );
		if( bProc )
		{
			return TRUE;
		} //if
	} //if..else..

	if( m_pInput->GetMouseR() && !m_bRButtonPressed )
	{
		m_bRButtonPressed = TRUE;
		if( MouseButtonR_Down() )
		{
			return TRUE;
		} //if
	}
	else if( !m_pInput->GetMouseR() && m_bRButtonPressed )
	{
		m_bRButtonPressed = FALSE;
		if( MouseButtonR_Up() )
		{
			return TRUE;
		} //if
	} //if..else..

	if( m_pTopDlg )
	{
		if( m_pTopDlg->IsTopMost() )
		{
			return TRUE;
		} //if
	} //if

	return FALSE;
} //CW3DUInterface::Update

/**
 * Update QuickSlot 
 */
void CW3DUInterface::UpdateQuickBar( const DWORD dwTick )
{
	for( int nCnt = 0; nCnt < NUM_QUICKSLOTBAR; nCnt++ )
	{
		m_apQuickBar[nCnt]->Update( dwTick );
	} //for
} //CW3DUInterface::UpdateQuickBar

/**
 * ¸ðµç ³»¿ëÀ» Ç¥½ÃÇÕ´Ï´Ù.
 */
void CW3DUInterface::Draw()
{
	g_pUIGraphicMgr->Render();
	LIST_PUIDLG::iterator		itor = m_listDialog.begin();
	while( itor != m_listDialog.end() )
	{
		UIDialog*	pDlg = *itor;
		Assert( pDlg );
		pDlg->Update( m_dwTick );
		pDlg->Draw();
		itor++;
	} //while

	if( m_pCurrEditor && m_pCurrEditor->IsShow() )
	{
		g_ChatIME.Draw( m_dwTick );
	} //if

	//¿©±â¼­ ¾ÆÀÌÅÛÀº ÀüºÎ Ç¥½ÃÇÑ´Ù...
	

	if( m_ItemMgr.GetHandInItemDat() )
	{
		m_ItemMgr.Draw( m_nMouseX - 14, m_nMouseY - 14 );
	} //if

	if( m_pCurrCommopent )
	{ // ÆÁ Á¤º¸.
		m_pCurrCommopent->DrawTipInfo();
	}

	m_EnchantCard.Draw();				///< ÀÎÃ¾Æ® Ä«µå
	
	if( m_pCursor )
	{
		m_pCursor->Draw( m_nMouseX, m_nMouseY );
	} //if

	DrawTip( m_nMouseX, m_nMouseY );

	m_GameReady.Draw();
} //CW3DUInterface::Draw

void CW3DUInterface::SetTipText( const char* szText, DWORD dwColor )
{
	if( !szText )
	{
		m_szTipInfo[0] = '\0';
	}
	else
	{
		strcpy( m_szTipInfo, szText );
		m_dwTipColor = dwColor;
	} //if..else
}
/*
void CW3DUInterface::UpdateTipInfo()
{
	switch( m_pCurrCommopent->m_nType )
	{
	case UI_INVENTORYIN:
		UIInventoryIN*	pInventory = reinterpret_cast<UIInventoryIN*>( m_pCurrCommopent );
		Assert( pInventory );
		
		char*	pParentName = pInventory->m_pOwnerComp->m_strName.c_str();
		int		nId = pInventory->GetItem( m_nMouseX, m_nMouseY );

		SItemData*		pItemData;
		if( !strcmp( pParentName, "DLG_INVENTORY_IN" ) )
		{
			pItemData = m_ItemMgr.FindItem( nId );
		}
		else if( !strcmp( pParentName, "DLG_SHOPINVENTORY" ) )
		{
			pItemData = m_ItemMgr.FindItem( nId, FALSE );
		}
		return TRUE;

	case UI_INVENTORYON:
		InventoryOnUp( m_pOldFocusCom );
		return TRUE;
	}
}

void CW3DUInterface::DrawItemTipInfo()
{

} //CW3DUInterface::DrawItemTipInfo
*/

/**
 * ÆÁÁ¤º¸ ±×¸®±â.
 */
void CW3DUInterface::DrawTip( int nX, int nY )
{
	Assert( m_pfntTip );
	Assert( m_pRectangle );

	if( strlen( m_szTipInfo ) < 1 )		return;
	
	int		nWidth, nHeight;
	m_pfntTip->GetStringSize( &nWidth, &nHeight, m_szTipInfo );

	nHeight += 2;
	RECT		rcText;
	rcText.left		= nX - ( nWidth >> 1 );
	rcText.top		= nY - nHeight;
	rcText.right	= rcText.left + nWidth;
	rcText.bottom	= rcText.top + nHeight;

	m_pRectangle->Draw( rcText.left-1, rcText.top-1, nWidth+2, nHeight+2, 0xFF000000 );
	m_pRectangle->Draw( rcText.left, rcText.top, nWidth, nHeight, 0xFFFFFFE1 );
	m_pfntTip->DrawText( m_szTipInfo, &rcText, DT_CENTER | DT_VCENTER, m_dwTipColor );

} //CW3DUInterface::DrawTip

/**
 * »ó´ë¹æ°ú...
 */
void CW3DUInterface::YouAndMe( 
	int				nId,				///< [IN] »ó´ë ¾ÆµÚ.
	BYTE			btYou,				///< [IN] »ó´ë Å¸ÀÔ.
	const DWORD		dwChoiceFlag		///< [IN] ¼±ÅÃ ÇÁ·¹±×.
	)
{
	m_btYou  = btYou;
	m_nYouId = nId;
	
/*	WORD	map=0;
	DWORD	ChoiceFlag=0;
	
	if( dwChoiceFlag > CHAR_PARTY )
	{
		ChoiceFlag  = dwChoiceFlag >> 16;
		map = dwChoiceFlag & 0xFF;
	}*/
	
	ShowDlg( "DLG_YOUANDME", TRUE );
	
	char		szCompName[64];
	UIButton*	pButton[4];
	for( int nCnt = 1; nCnt <= 5; nCnt++ )
	{
		sprintf( szCompName, "BTN_YOUME_%d", nCnt );
		pButton[nCnt-1] = reinterpret_cast<UIButton*>( GetComponent( szCompName ) );
		if( !pButton[nCnt-1] )
		{
			continue;
		} //if

		pButton[nCnt-1]->Show( FALSE );
		pButton[nCnt-1]->Enable( TRUE );
	} //for

	if( btYou == YOU_CHARACTER )
	{
		pButton[0]->SetText( STR_UIMSG_DEAL );
		pButton[1]->SetText( STR_UIMSG_PARTY );

		pButton[0]->Show( TRUE );
		pButton[1]->Show( TRUE );
	}
	else if( btYou == YOU_NPC )
	{
		pButton[0]->Show( TRUE );
		pButton[1]->Show( TRUE );
		pButton[2]->Show( TRUE );
		pButton[3]->Show( TRUE );
		if( pButton[4] ) pButton[4]->Show( TRUE );

		if( dwChoiceFlag & NPC_GUILDMGR )
		{
			pButton[0]->SetText( STR_UIMSG_TALK );
			pButton[1]->SetText( STR_UIMSG_CREATEGUILD );
			pButton[2]->SetText( STR_UIMSG_RELEASEGUILD );
			pButton[3]->SetText( STR_UIMSG_STOREHOUSE );

			pButton[3]->Enable( FALSE );
			if( pButton[4] ) pButton[4]->Enable( FALSE );
		}

		else if( dwChoiceFlag & NPC_SMITH )
		{
			pButton[0]->SetText( STR_UIMSG_TALK );
			pButton[1]->SetText( STR_UIMSG_SHOP );
			pButton[2]->SetText( STR_UIMSG_QUEST );
			pButton[3]->SetText( STR_UIMSG_ENCHANT );
			if( !( dwChoiceFlag & NPC_TALK ) )		pButton[0]->Enable( FALSE );
			if( !( dwChoiceFlag & NPC_SHOP ) )		pButton[1]->Enable( FALSE );
			if( !( dwChoiceFlag & NPC_QUEST ) )		pButton[2]->Enable( FALSE );
			if( pButton[4] ) pButton[4]->Enable( FALSE );
			pButton[3]->Enable(FALSE);
		}

		else if( dwChoiceFlag & NPC_CARD )
		{
			pButton[0]->SetText( STR_UIMSG_TALK );
			pButton[1]->SetText( STR_UIMSG_SHOP );
			pButton[2]->SetText( STR_UIMSG_QUEST );
			pButton[3]->SetText( STR_UIMSG_CARDSUM );
			if( !( dwChoiceFlag & NPC_TALK ) )		pButton[0]->Enable( FALSE );
			if( !( dwChoiceFlag & NPC_SHOP ) )		pButton[1]->Enable( FALSE );
			if( !( dwChoiceFlag & NPC_QUEST ) )		pButton[2]->Enable( FALSE );
			if( pButton[4] ) pButton[4]->Enable( FALSE );
		}

		else if( dwChoiceFlag & NPC_CARD1 )
		{
			pButton[0]->SetText( STR_UIMSG_TALK );
			pButton[1]->SetText( STR_UIMSG_SHOP );
			pButton[2]->SetText( STR_UIMSG_QUEST );
			pButton[3]->SetText( STR_UIMSG_CARDUNDO );
			if( !( dwChoiceFlag & NPC_TALK ) )		pButton[0]->Enable( FALSE );
			if( !( dwChoiceFlag & NPC_SHOP ) )		pButton[1]->Enable( FALSE );
			if( !( dwChoiceFlag & NPC_QUEST ) )		pButton[2]->Enable( FALSE );
			if( pButton[4] ) pButton[4]->Enable( FALSE );
		}

		else
		{
			pButton[0]->SetText( STR_UIMSG_TALK );
			pButton[1]->SetText( STR_UIMSG_SHOP );
			pButton[2]->SetText( STR_UIMSG_QUEST );
			pButton[3]->SetText( STR_UIMSG_STOREHOUSE );
			if( pButton[4] ) pButton[4]->SetText( STR_UIMSG_GOTODEALMAP );

			if( !( dwChoiceFlag & NPC_TALK ) )		pButton[0]->Enable( FALSE );
			if( !( dwChoiceFlag & NPC_SHOP ) )		pButton[1]->Enable( FALSE );
			if( !( dwChoiceFlag & NPC_QUEST ) )		pButton[2]->Enable( FALSE );
			if( !( dwChoiceFlag & NPC_STORAGE ) )
			{
				pButton[3]->Enable( FALSE );
				if( pButton[4] )	pButton[4]->Enable( FALSE );
			} //if
		} //if..else

	/*	if( map == 20 )
		{
			pButton[4]->Enable( FALSE );
		}*/
	} //if..else
} //CW3DUInterface::YouAndMe

/**
 * ÇöÀç Ã¢¿¡ µû¶ó º¸ÀÌ±â ¾Ê±â¸¦ Ã³¸®.
 */
void CW3DUInterface::HideActiveDlg()
{
	if( !m_pTopDlg || m_pTopDlg->IsAlways() )		return;

	UIDialog*	pDlg = m_pTopDlg;
	if( pDlg->IsShow() )
	{
		if( !pDlg->m_strName.compare( "DLG_INVENTORY" ) )
		{
			ShowMyInventoryDlg( FALSE );
		}
		else if( !pDlg->m_strName.compare( "DLG_STORAGE" ) )
		{
			ShowStorage( FALSE );
		}
		else if( !pDlg->m_strName.compare( "DLG_SHOPINVENTORY" ) )
		{
			ShowShop( FALSE );
		}
		else if( !pDlg->m_strName.compare( "DLG_DEAL" ) )
		{
			ShowUserDealBox( FALSE );
		}
		else
		{
			DlgNoExe();
			ShowDlg( pDlg, FALSE );
		}  //if..else..
	} //if
} //CW3DUInterface::HideActiveDlg

/**
 * Ä³¸¯ÅÍ ÀÌ¸§ ¼³Á¤.
 */
void CW3DUInterface::SetCharacterName( const char* szName )
{
	SetText( "TXT_CINFO_NAME", szName );
	SetText( "TXT_CHAR_NAME", szName );
} //CW3DUInterface::SetCharacterName

/**
 * ÇØ´ç Å¬·¡½º ÀÌ¸§À» ¾ò´Â´Ù 
 */
BOOL CW3DUInterface::GetClassName( char* szClassName, const BYTE btClass )
{
	if( !szClassName )
	{
		Assert( !"NULL" );
		return FALSE;
	} //if

	if( btClass > 16 )
	{
		Assert( !"Over Range" );
		return FALSE;
	} //if

	// Á÷¾÷ ÀÌ¸§ Å×ÀÌºí
//#ifndef _CHINAVERSION
	char CLASS_NAME[17][32] =
	{
			"None",//"None",//			
			"Warrior",//"Warrior",//		
			"Bandit",//"Bandit",//			
			"Shadow knight",//"Shadow knight",//	
			"Paladin",//"Paladin",//		
			"Assassin",//"Assassin",//		
			"Archer",//"Archer",//			
			"Poacher",//"Poacher",//		
			"Ranger",//"Ranger",//			
			"Blade Mage",//"Blade mage",//		
			"Wanderer",//"Wanderer",//		
			"Wizard",//"Wizard",//			
			"Shaman",//"Shaman",//			
			"Monk",//"Monk",//			
			"Hawk",//"Hawk",//			
			"Druid",//"Druid",//			
			"Cleric"//"Cleric"//			
	};
/*#else
	char CLASS_NAME[17][32] =
	{
			"ÎÞÖ°Òµ",
			"Õ½Ê¿",
			"Ç¿µÁ",
			"°µÆïÊ¿",
			"Ê¥ÆïÊ¿",
			"´Ì¿Í",
			"¹­¼ýÊÖ",
			"ÍµÁÔÕß",
			"ÁÔÈË",
			"µ¶½³",
			"ÂÃÐÐ¼Ò",
			"Î×Ê¦",
			"ÈøÂú",
			"¼ÀË¾",
			"ÉÌÈË",
			"µÂÂ³ÒÁ",
			"ÄÁÊ¦"
	};
#endif //_CHINAVERSION*/

	strcpy( szClassName, CLASS_NAME[btClass] );
	return TRUE;
} //CW3DUInterface::GetClassName

/**
 * Á÷¾÷ÀÌ¸§ ¼³Á¤
 */
void CW3DUInterface::SetJobName( const BYTE btClass )
{
	char		szClassName[32];
	GetClassName( szClassName, btClass );
	SetText( "TXT_CINFO_JOB", szClassName );
} //CW3DUInterface::SetJobName

/**
 * Ã¢°í º¸ÀÌ±â.
 */
void CW3DUInterface::ShowStorage( const BOOL bShow )
{
	if( IsShowDlg( "DLG_STORAGE" ) == bShow )
	{
		return;
	} //if

	ShowDlg( "DLG_STORAGE", bShow );
	CheckGroup( "CHK_STORAGE_TAB", "INVEN_STORAGE_", 1 );

	if( bShow )
	{
		ShowMyInventoryDlg( TRUE );
	}
	else
	{
		ShowDlg( "DLG_INVENTORY", FALSE );
	} //if
} //CW3DUInterface::ShowStorage

void CW3DUInterface::ShowServerStorage( BOOL bShow )
{
	if( IsShowDlg( "DLG_SERVER_STORAGE" ) == bShow )
	{
		return;
	} //if
	
	if( bShow )
	{
		ShowDlg( "DLG_SERVER_STORAGE", bShow );
		CheckGroup( "CHK_STORAGE_TAB", "INVEN_SERVER_STORAGE_", 1 );
	}
	else
	{
		ShowDlg( "DLG_SERVER_STORAGE", FALSE );
	} //if
}

/**
 * ¼¥À» ±¸¼º.
 */
void CW3DUInterface::ShowShop( BOOL bShow )
{
	if( IsShowDlg( "DLG_SHOPINVENTORY" ) == bShow )
	{
		return;
	} //if

	UICComponent*	pXBtn = GetComponent( "BTN_INVEN_X" );
	if( !pXBtn )
	{
		Assert( pXBtn );
		return ;
	} //if

	if( bShow )
	{
		if( IsShowDlg( "DLG_DEAL" ) )
		{
			return;
		}

		ShowDlg( "DLG_CHARACTERINFO", FALSE );

		pXBtn->Show( FALSE );

		ShowMyInventoryDlg( TRUE );
		ShowDlg( "DLG_SHOPINVENTORY", TRUE );
		ShowDlg( "DLG_CHARACTERINFO", FALSE );
		ShowDlg( "DLG_CHARACTERINFO", FALSE );
		CheckGroup( "CHK_SHOP_TAB", "SHOPINVENTORY_", 1 );
	}
	else
	{
		pXBtn->Show( TRUE );

		ShowDlg( "DLG_INVENTORY", FALSE );
		ShowDlg( "DLG_SHOPINVENTORY", FALSE );

		GetComponent( "SHOPINVENTORY_1" )->Cleanup();
		GetComponent( "SHOPINVENTORY_2" )->Cleanup();
		GetComponent( "SHOPINVENTORY_3" )->Cleanup();

		m_ItemMgr.ClearItemInTemp();
	} //if..else
} //CW3DUInterface::ShowShop

/**
 * À¯Àú°Å·¡Ã¢.
 */
void CW3DUInterface::ShowUserDealBox( BOOL bShow )
{
	if( IsShowDlg( "DLG_DEAL" ) == bShow )
	{
		return;
	} //if

	UICComponent*	pXBtn			= GetComponent( "BTN_INVEN_X" );
	UICComponent*	pDealDlg		= GetComponent( "DLG_DEAL" );

	Assert( pXBtn );
	Assert( pDealDlg );

	if( !pXBtn || !pDealDlg )
	{
		return ;
	} //if

	if( bShow )
	{
		if( pDealDlg->IsShow() )
		{
			return;
		}

		ShowDlg( "DLG_CHARACTERINFO", FALSE );

		pXBtn->Show( FALSE );

		ShowDlg( pDealDlg, TRUE );
		ShowMyInventoryDlg( TRUE );

		int					nIndex = 1;
		UIInventoryIN*		pInven;
		while( pInven = GetInventoryIN( "INVENTORY_DEALME_", nIndex ) )
		{
			pInven->Cleanup();
			if( nIndex == 1 )	pInven->Show( TRUE );

			nIndex++;
		} //while
		CheckGroup( "CHK_DEALME_TAB", "INVENTORY_DEALME_", 1 );
		
		nIndex = 1;
		while( pInven = GetInventoryIN( "INVENTORY_DEALYOU_", nIndex ) )
		{
			pInven->Cleanup();
			if( nIndex == 1 )	pInven->Show( TRUE );

			nIndex++;
		} //while
		CheckGroup( "CHK_DEALYOU_TAB", "INVENTORY_DEALYOU_", 1 );

		SetText( "TXT_DEAL_GOLD1", 0 );
		SetText( "TXT_DEAL_GOLD2", 0 );

		m_ItemMgr.ClearItemInTemp();
	}
	else
	{
		pXBtn->Show( TRUE );
		
		ShowDlg( "DLG_INVENTORY", FALSE );
		ShowDlg( pDealDlg, FALSE );

		int					nIndex = 1;
		UIInventoryIN*		pInven;
		while( pInven = GetInventoryIN( "INVENTORY_DEALME_", nIndex ) )
		{
			pInven->Cleanup();
			nIndex++;
		} //while
		CheckGroup( "CHK_DEALME_TAB", "INVENTORY_DEALME_", 1 );
		
		nIndex = 1;
		while( pInven = GetInventoryIN( "INVENTORY_DEALYOU_", nIndex ) )
		{
			pInven->Cleanup();
			nIndex++;
		} //while
		CheckGroup( "CHK_DEALYOU_TAB", "INVENTORY_DEALYOU_", 1 );
		
		SetText( "TXT_DEAL_GOLD1", 0 );
		SetText( "TXT_DEAL_GOLD2", 0 );

		m_ItemMgr.ClearItemInTemp();
	}
} //CW3DUInterface::ShowUserDealBox

void CW3DUInterface::ShowSmithEnchentBox( BOOL bShow )
{
	char	szDlgName[64];
	
	
	if( m_NPCNum == 3 )
	{
		strcpy( szDlgName, "DLG_SMITH_WP_INTEGRATION" );
		strcpy( m_szTxtName, "TXT_WP_GOLD" );
		strcpy( m_szUIccompentSmith, "SMITH_WP_IN" );
	}
	else if( m_NPCNum == 72 )
	{
		strcpy( szDlgName, "DLG_SMITH_AM_INTEGRATION" );
		strcpy( m_szTxtName, "TXT_AM_GOLD" );
		strcpy( m_szUIccompentSmith, "SMITH_AM_IN" );
	}

	if( IsShowDlg( szDlgName ) == bShow )
	{
		return;
	}

	if( bShow )
	{
		ShowDlg( szDlgName, TRUE );
		ShowMyInventoryDlg( TRUE );
		SetText( m_szTxtName, 0 );
	}
	else
	{
		ShowDlg( szDlgName, FALSE );
		UICComponent*	pSmith = GetComponent( m_szUIccompentSmith );

		if( !pSmith )
		{
			Assert( !"Not Found Component" );
			return;
		}
		pSmith->Cleanup();	
	}
} //CW3DUInterface::ShowSmithEnchentBox

/**
 * ½ºÅ³Ã¢ º¸ÀÌ±â.
 */
void CW3DUInterface::ShowSkillDlg( BOOL bShow )
{
	if( bShow )
	{
		ShowDlg( "DLG_SKILL", TRUE );
		Skill1CheckButton( 1 );
		Skill2CheckButton( 2 );
	}
	else
	{
		ShowDlg( "DLG_SKILL", FALSE );
	} //if..else
} //CW3DUInterface::ShowSkillDlg

/**
 *
 */
/**void CW3DUInterface::ShowQuest( BOOL bShow )
{
	if( bShow )
	{
		ShowDlg( "DLG_MYQUEST", TRUE );
	}
	else
	{
		ShowDlg( "DLG_MYQUEST", FALSE );
	}
}*/


/**
 * ÀÚ½Å ÀÎº¥Åä¸® º¸ÀÌ±â.
 */
void CW3DUInterface::ShowMyInventoryDlg( BOOL bShow )
{
	if( bShow )
	{
		ShowDlg( "DLG_INVENTORY", TRUE );
		CheckGroup( "CHK_INVEN_TAB", "INVENTORY_IN_", 1 );
	}
	else
	{
		ShowDlg( "DLG_INVENTORY", FALSE );

		ShowShop( FALSE );
		ShowStorage( FALSE );
		ShowUserDealBox( FALSE );
		ShowSmithEnchentBox( FALSE );
	} //if..else
} //CW3DUInterface::ShowMyInventoryDlg

void CW3DUInterface::ShowServerInventoryDlg( BOOL bShow )
{
	if( bShow )
	{
		ShowDlg( "DLG_SERVER_INVENTORY", TRUE );
		CheckGroup( "CHK_SERVER_INVEN_TAB", "INVENTORY_SERVER_IN_", 1 );
	}
	else
	{
		ShowDlg( "DLG_SERVER_INVENTORY", FALSE );
		
		ShowShop( FALSE );
		ShowStorage( FALSE );
		ShowUserDealBox( FALSE );
		ShowSmithEnchentBox( FALSE );
	} //if..else
}

/**
 * Äü ½º·Ô º¸ÀÌ±â.
 */
void CW3DUInterface::ShowQuickBar()
{
	UIQuickSlot*	pQuickComp;
	char			szCompName[64];

	char			szBarNum[4];
	sprintf( szBarNum, "F%d", m_btQuickBarNum+1 );
	SetText( "TXT_QUICK_BARNUM", szBarNum );

	for( int nCnt = 0; nCnt < NUM_QUICKSLOTBAR; nCnt++ )
	{
		sprintf( szCompName, "QSLOT_%d", nCnt );
		pQuickComp = reinterpret_cast<UIQuickSlot*>( GetComponent( szCompName ) );
		if( !pQuickComp )
		{
			Assert( !"¾ø´Â ÄÄÆ÷" );
			continue;
		} //if

		if( m_btQuickBarNum == nCnt )		pQuickComp->Show( TRUE );
		else								pQuickComp->Show( FALSE );
	} //for

} //CW3DUInterface::ShowMyInventoryDlg

/**
 * ÇÃ·¹ÀÌ¾î Á×´Ù.
 */
void CW3DUInterface::PlayerLive( const BOOL bLive )
{
	if( bLive )
	{
		Show( "PIC_CHAR_DEAD", FALSE );
	}
	else
	{
		Show( "PIC_CHAR_DEAD", TRUE );
		ShowDlg( "DLG_DEATH", TRUE );
	} //if..else
	
	UIImageList*	pImageList = reinterpret_cast<UIImageList*>( GetComponent( "IMGLST_SKILLIMAGE" ) );
	if( !pImageList )
	{
		Assert( !"¾ø´Â ÀÌ¹ÌÁö ¸®½ºÆ® ÄÞÇÁ" );
		return;
	} //if
	pImageList->Cleanup();
} //CW3DUInterface::PlayerDie

/**
 * °ø°Ý·Â ¼³Á¤.
 */
void CW3DUInterface::SetDamage( SItemInfoData* pItemData )
{
	SItemData*	pItemDat = NULL;
	
	if( m_nMainSkill == 0 )
	{
		UIInventoryON*		pEquipComp = reinterpret_cast<UIInventoryON*>( GetComponent( "INVENTORY_ON" ) );
		if( !pEquipComp )
		{
			Assert( !"¾ø´Â ÄÞÇÁ" );
			return;
		} //if
		
		pItemDat = pEquipComp->GetItemPart( EQUIP_SHORTWEAPON );
	}
	else if( m_nMainSkill == 1 )
	{
		UIInventoryON*		pEquipComp = reinterpret_cast<UIInventoryON*>( GetComponent( "INVENTORY_ON" ) );
		if( !pEquipComp )
		{
			Assert( !"¾ø´Â ÄÞÇÁ" );
			return;
		} //if
		
		pItemDat = pEquipComp->GetItemPart( EQUIP_LONGWEAPON );
	}
	else if( pItemData && ( m_nMainSkill == 2 || m_nMainSkill == 3 ) )
	{
		UIInventoryON*		pEquipComp = reinterpret_cast<UIInventoryON*>( GetComponent( "INVENTORY_ON" ) );
		if( !pEquipComp )
		{
			Assert( "!¾ø´Â ÄÞÇÁ" );
		}
		//return;
		pItemDat = pEquipComp->GetItemPart( EQUIP_SHORTWEAPON );
	}
	else
	{
		return;
	}//if..else
	
	int		nMinDamage, nMaxDamage, nMinMagicDamage, nMaxMagicDamgage;
	if( !pItemDat || !pItemDat->pItemInfoData )
	{
		nMinDamage = 0;
		nMaxDamage = 0;
		nMinMagicDamage = 0;
		nMaxMagicDamgage = 0;
	}
	else
		
	{
		nMinDamage = pItemDat->pItemInfoData->wMinDamage;
		nMaxDamage = pItemDat->pItemInfoData->wMaxDamage;
		
		nMinMagicDamage  = pItemDat->pItemInfoData->wMinMana;
		nMaxMagicDamgage = pItemDat->pItemInfoData->wMaxMana;
		
	} //if..else
/*#ifdef _CHINAVERSION
	if( !m_listPartyMember.empty() )
	{
		nMinDamage = (WORD)( nMinDamage * ( m_listPartyMember.size() + 2 ) * 0.1 / 8 );
		nMaxDamage = (WORD)( nMaxDamage * ( m_listPartyMember.size() + 2 ) * 0.1 / 8 );

		nMinMagicDamage = (WORD)( nMinMagicDamage * (m_listPartyMember.size() + 2) *0.1 / 8 );
		nMaxMagicDamgage = (WORD)( nMaxMagicDamgage * ( m_listPartyMember.size() + 2 ) * 0.1 / 8 );
	} //if
#endif*/
	
	char	szDamageTxt[32],szDamageTxt2[32];
	sprintf( szDamageTxt, "%d - %d", nMinDamage, nMaxDamage );
	SetText( "TXT_CINFO_ATTACK", szDamageTxt );
	sprintf( szDamageTxt2, "%d - %d", nMinMagicDamage, nMaxMagicDamgage );
	SetText( "TXT_CINFO_MAGIC", szDamageTxt2 );
	
} //CW3DUInterface::SetDamage

/** 
 * ¹æ¾î·Â ¼³Á¤.
 */
void CW3DUInterface::SetDefence()
{
	UIInventoryON*		pEquipComp = reinterpret_cast<UIInventoryON*>( GetComponent( "INVENTORY_ON" ) );
	if( !pEquipComp )
	{
		Assert( !"¾ø´Â ÄÞÇÁ" );
		return;
	} //if

	int				nDefence = pEquipComp->GetTotDefence();
	SItemData*		pItemDat;
	if( m_nMainSkill == 0 )
	{
		pItemDat = pEquipComp->GetItemPart( EQUIP_SHORTWEAPON );	
		if( pItemDat && 
			pItemDat->pItemInfoData &&
			ISTWOHANDWEAPON( pItemDat->pItemInfoData->btItemType ) )
		{
			pItemDat = pEquipComp->GetItemPart( EQUIP_SHIELD );
			if( pItemDat &&	pItemDat->pItemInfoData ) 
			{
				nDefence -= pItemDat->pItemInfoData->wMinDamage;
			} //if
		} //if
	}
	else if( m_nMainSkill == 1 )
	{
		pItemDat = pEquipComp->GetItemPart( EQUIP_SHIELD );
		if( pItemDat &&	pItemDat->pItemInfoData ) 
		{
			nDefence -= pItemDat->pItemInfoData->wMinDamage;
		} //if
	} //if..else

/*#ifdef _CHINAVERSION
	if( !m_listPartyMember.empty() )
	{
		nDefence = (WORD)( nDefence * ( m_listPartyMember.size() + 2 ) * 0.1 / 8 );
	} //if
#endif*/

	if( nDefence < 0 )
	{
		nDefence = 0;
	} //if
	SetText( "TXT_CINFO_DEFENCE", nDefence );
} //CW3DUInterface::SetDefence

/************************************************************************/
/* Name : SetMagicResis(  )                                             */
/* Desc : Ä³¸¯ÅÍ Á¤º¸Ã¢¿¡ ¸¶¹ýÀúÇ×·Â Ç¥½Ã                               */
/* Date : 2005-02-18                Make : Umenokoji                    */
/************************************************************************/
void CW3DUInterface::SetMagicResis(  )
{
	UIInventoryON*		pEquipComp = reinterpret_cast<UIInventoryON*>( GetComponent( "INVENTORY_ON" ) );
	if( !pEquipComp )
	{
		Assert( !"¾ø´Â ÄÞÇÁ" );
		return;
	}
	
	int				nMagicRes = pEquipComp->GetTotMagicDefence();
	SItemData*		pItemDat;
	
	if( m_nMainSkill == 0 )
	{
		pItemDat = pEquipComp->GetItemPart( EQUIP_SHORTWEAPON );
		if( pItemDat &&
			pItemDat->pItemInfoData &&
			ISTWOHANDWEAPON( pItemDat->pItemInfoData->btItemType ) )
		{
			pItemDat = pEquipComp->GetItemPart( EQUIP_SHIELD );
			if( pItemDat && pItemDat->pItemInfoData )
			{
				nMagicRes += pItemDat->pItemInfoData->wPluseMagicDefense;
			}
		}
	}
	else if( m_nMainSkill == 1 )
	{
		pItemDat = pEquipComp->GetItemPart( EQUIP_SHIELD );
		if( pItemDat && pItemDat->pItemInfoData )
		{
			nMagicRes -= pItemDat->pItemInfoData->wPluseMagicDefense;
		}
	}
/*#ifdef _CHINAVERSION
	if( !m_listPartyMember.empty() )
	{
		nMagicRes = (WORD)( nMagicRes * ( m_listPartyMember.size() + 2 ) * 0.1 / 8 );
	} //if
#endif*/
	
	if( nMagicRes < 0 )
	{
		nMagicRes = 0;
	}
	
	SetText( "TXT_CINFO_DEFENCEM", nMagicRes );
}



/**
 * Å¸ÀÓ¸Ó º¸ÀÌ±â.
 */
void CW3DUInterface::ShowTimer( const char* szCompName, DWORD dwShowTick )
{
	UITimer*	pTimer = reinterpret_cast<UITimer*>( GetComponent( szCompName ) );
	if( !pTimer )
	{
		Assert( "¾ø´Â Å¸ÀÓ¸Ó" );
		return;
	} //if

	ShowDlg( pTimer, TRUE );
	pTimer->SetTime( dwShowTick );
} //CW3DUInterface::ShowTimer

/** 
 * ÃÂÆÃ ±â·Ï Ã¢ »çÀÌÁî Á¶Àý
 */
void CW3DUInterface::ChangeChatListSize()
{
	static int		s_nBaseLength = 0;
	static int		s_nBaseLineCnt = 0;

	UICComponent*	pListDlg = GetComponent( "DLG_CHATTINGLIST" );
	UIStringList*	pStrList = reinterpret_cast<UIStringList*>( GetComponent( "LIST_CHATTING" ) );
	UIScrollBar*	pScroll  = reinterpret_cast<UIScrollBar*>( GetComponent( "SCR_CHAT_LIST" ) );

	UICComponent*	pMsgDlg		= GetComponent( "DLG_MSGLIST" );
	UIStringList*	pMsgList	= reinterpret_cast<UIStringList*>( GetComponent( "LIST_MSG" ) );
	UIScrollBar*	pMsgScroll  = reinterpret_cast<UIScrollBar*>( GetComponent( "SCR_MSG_LIST" ) );
	
	if( !pListDlg || !pStrList || !pScroll || !pMsgDlg || !pMsgList || !pMsgScroll )
	{
		Assert( !"ÄÞÆ÷°¡ ¾ø´Ù" );
		return;
	} //if

	if( !s_nBaseLength )
	{
		s_nBaseLength  = pListDlg->m_rcRegion.bottom - pListDlg->m_rcRegion.top;
		s_nBaseLineCnt = pStrList->GetDisplayLineCount();
	} //if

	int		nCurCount = ( pListDlg->m_rcRegion.bottom - pListDlg->m_rcRegion.top ) / s_nBaseLength;
	
	if( !pListDlg->IsShow() )
	{
		pListDlg->Show( TRUE );
		pMsgDlg->m_rcRegion.bottom = ( pMsgDlg->m_rcRegion.bottom - s_nBaseLength );
		pMsgDlg->m_rcRegion.top = ( pMsgDlg->m_rcRegion.top - s_nBaseLength );
		
		pMsgList->m_rcRegion.bottom = ( pMsgList->m_rcRegion.bottom - s_nBaseLength);
		pMsgList->m_rcRegion.top = ( pMsgList->m_rcRegion.top - s_nBaseLength );

		pMsgScroll->SetPosition( pMsgDlg->m_rcRegion.left, pMsgDlg->m_rcRegion.top, s_nBaseLength );
		return;
	} //if

	if( ++nCurCount > 3 )
	{
		pMsgDlg->m_rcRegion.bottom = ( pMsgDlg->m_rcRegion.bottom + ( s_nBaseLength * ( nCurCount ) ) );
		pMsgDlg->m_rcRegion.top = ( pMsgDlg->m_rcRegion.top + ( s_nBaseLength * ( nCurCount ) ) );
		
		pMsgList->m_rcRegion.bottom = ( pMsgList->m_rcRegion.bottom + ( s_nBaseLength * ( nCurCount  ) ) );
		pMsgList->m_rcRegion.top = ( pMsgList->m_rcRegion.top + ( s_nBaseLength * ( nCurCount ) ) );

		pMsgScroll->SetPosition( pMsgDlg->m_rcRegion.left, pMsgDlg->m_rcRegion.top, s_nBaseLength );
		
		nCurCount = 1;
		pListDlg->Show( FALSE );
	}
	int		nHeight = s_nBaseLength * nCurCount;

	pListDlg->m_rcRegion.top = pListDlg->m_rcRegion.bottom - nHeight;
	pStrList->m_rcRegion.top = pStrList->m_rcRegion.bottom - nHeight;
	pStrList->SetCount( nCurCount * s_nBaseLineCnt );

	pMsgDlg->m_rcRegion.bottom = ( pMsgDlg->m_rcRegion.bottom - s_nBaseLength );
	pMsgDlg->m_rcRegion.top = ( pMsgDlg->m_rcRegion.top - s_nBaseLength );

	pMsgList->m_rcRegion.bottom = ( pMsgList->m_rcRegion.bottom - s_nBaseLength );
	pMsgList->m_rcRegion.top = ( pMsgList->m_rcRegion.top - s_nBaseLength );

	pMsgScroll->SetPosition( pMsgDlg->m_rcRegion.left, pMsgDlg->m_rcRegion.top, s_nBaseLength );

	int		nNumString  = pStrList->GetListSize();
	int		nNumDisplay = pStrList->GetDisplayLineCount();
	int		nScrollMax  = 0;
	if( nNumString > nNumDisplay )
	{
		nScrollMax = nNumString - nNumDisplay;
	} //if
	pScroll->SetMax( nScrollMax );
	pStrList->SetPos( nScrollMax );
	pScroll->SetPosition( pListDlg->m_rcRegion.left, pListDlg->m_rcRegion.top, nHeight );
	
} //CW3DUInterface::ChangeChatListSize

/**
 * µµ¿ò¸»
 */
void CW3DUInterface::GameHelp()
{
	UIPicture*		pPicComp = reinterpret_cast<UIPicture*>( GetComponent( "PIC_GAMEHELP" ) );
	if( !pPicComp )
	{
		Assert( !"¾ø´Â ÄÞÇÁ" );
		return;
	} //if

	if( IsShowDlg( "DLG_GAMEHELP" ) )
	{
		SAFE_RELEASE( m_pHelpImage );
		pPicComp->SetPicture( NULL );

		ShowDlg( "DLG_GAMEHELP", FALSE );
	}
	else
	{
		m_pHelpImage = MAKE_COMPONENT( Sprite );
		if( !m_pHelpImage )
		{
			Assert( !"»ý¼º½ÇÆÐ" );
			return;
		} //if

		if( !m_pHelpImage->Load( "Interface/keyhelp.tga", 800, 600 ) )
		{
			Assert( !"·Îµå½ÇÆÐ" );
			return;
		} //if

		pPicComp->SetPicture( m_pHelpImage );

		ShowDlg( "DLG_GAMEHELP", TRUE );
	} //if..else	
} //CW3DUInterface::GameHelp

/**
 * ÀÌÀÌº¥Æ® ¸Þ¼¼Áö ¹Ú½º º¸ÀÌ±â(Áß±¹)
 */
void CW3DUInterface::ShowEventMessageBox( const BYTE btResult )
{
	char		szBuff[64];
	char		szImageName[64];
	switch( btResult )
	{
	case 0:	// ²Î
		sprintf( szBuff, "±¦ÏäÓÐÁË£¬±¦Îï»¹»áÔ¶Âð£¿\n ¼ÌÐøÅ¬Á¦Å¶¡£" );
		strcpy( szImageName, "Interface/evt_0006.tga" );
		break;
	case 1:	// ÇÚµåÆù 
		sprintf( szBuff, "%s\n%s", STR_EVENTMSG_CONGRATULATION, STR_EVENTMSG_HANDPHONE );
		strcpy( szImageName, "Interface/evt_0005.tga" );
		break;
	case 2: // µðÄ«
		sprintf( szBuff, "%s\n%s", STR_EVENTMSG_CONGRATULATION, STR_EVENTMSG_DIGCAMERA );
		strcpy( szImageName, "Interface/evt_0001.tga" );
		break;
	case 3: // TV
		sprintf( szBuff, "%s\n%s", STR_EVENTMSG_CONGRATULATION, STR_EVENTMSG_TV );
		strcpy( szImageName, "Interface/evt_0002.tga" );
		break;
	case 4: // ¾×Á¤
		sprintf( szBuff, "%s\n%s", STR_EVENTMSG_CONGRATULATION, STR_EVENTMSG_LCD );
		strcpy( szImageName, "Interface/evt_0003.tga" );
		break;
	case 5: // MP3
		sprintf( szBuff, "%s\n%s", STR_EVENTMSG_CONGRATULATION, STR_EVENTMSG_MP3 );
		strcpy( szImageName, "Interface/evt_0004.tga" );
		break;
	default:
		Assert( !"ÀÌº¥Æ® °á°ú ½ÇÆÐ" );
		return;
	} //switch

	UIPicture*		pPic = reinterpret_cast<UIPicture*>( GetComponent( "PIC_EVENTMSG_IMAGE" ) );
	if( !pPic )
	{
		Assert( !"¾ø´Â ÄÞÇÁ" );
		return;
	} //if
	if( !pPic->Load( szImageName, 152, 150 ) )
	{
		Assert( !"ÀÌ¹ÌÁö·Îµå ½ÇÆÐ" );
	} //if

	SetText( "TXT_EVENTMSG_TEXT", szBuff );

	ShowDlg( "DLG_EVENTMESSAGE", TRUE );
} //CW3DUInterface::ShowEventMessageBox

#ifdef _dUMENOKOJI_QUEST_
/**
 *		Äù½ºÆ® ¿äÃ» NPC
 */
BOOL CW3DUInterface::BeginQuestNPC( IW3DNPCScript* pNPCScript, const int nTalkNum )
{
	pNPCScript->InitPreTalkNum(  );
	if( !pNPCScript )
	{
		Assert( pNPCScript );
		return FALSE;
	}
	
	UIStaticText*		pUISText = (UIStaticText*)GetComponent( "TXT_QUEST_SAY_TEXT" );
	UISelectString*		pUISStr  = (UISelectString*)GetComponent( "SSTR_NPC_ANSWER" );

#ifdef _dUMENOKOJI_QUEST_
	UIButton*			pUIBtn		= (UIButton*)GetComponent( "BTN_QUEST_SAY_NEXT" );
	UIButton*			pUIBtp		= (UIButton*)GetComponent( "BTN_QUEST_SAY_PRE" );
#endif
	
	UIButton*			pUIQuestYBtn = (UIButton*)GetComponent( "BTN_QUEST_YES" );
	UIButton*			pUIQuestNBtn = (UIButton*)GetComponent( "BTN_QUEST_NO" );

	if( !pUISStr || !pUISText || !pUIBtn || !pUIBtp )
	{
		Assert( 0 );
		return FALSE;
	} //if
	
	if( !pUIQuestYBtn || !pUIQuestNBtn )
	{
		Assert( !"¾ø´Â ¹öÆ°" );
		return  FALSE;
	}
	
	pUISStr->Cleanup();
	pUISText->Cleanup();


	if( !pNPCScript->FirstTalk( nTalkNum ) )
	{
		return FALSE;
	} //if
	
	char*		pText = NULL;
	int			nAskNum = pNPCScript->GetAskNum();
#ifdef _dUMENOKOJI_QUEST_
	int i = 0;
	SetTextArrStartNum( 0 );
	InitTextArroNowNum(  );
	pText = (char*)pNPCScript->GetText();
	
	SetPrevText( pText, i );
	pUISText->SetText( pText );
	
	for( i=1;i<30; i++ )//Äù½ºÆ® ÆäÀÌÁö Á¦ÇÑ 30ÆäÀÌÁö..
	{
		if( pNPCScript->NextTalk() )
		{
			pText = (char*)pNPCScript->GetText();
			SetPrevText( pText, i );
			SetTextArrEndNum( i );
		}
		else
		{
			SetTextArrEndNum( i-1 );
			break;
		}
	}
#else
	pText = (char*)pNPCScript->GetText();
	pUISText->SetText( pText );
#endif
	for( int nCnt = 0; nCnt < nAskNum; nCnt++ )
	{
		pText = (char*)pNPCScript->GetAskText( nCnt );
		if( pText )
		{
			pUISStr->AddString( pText );
		} //if
	} //for
	
	if( nAskNum > 0 )
	{
		ShowDlg( "DLG_NPC_ANSWER", TRUE );
	} //if
	
	ShowDlg( "DLG_NPC_QUEST_SAY", TRUE );
///´ÙÀ½ ´ë»ç°¡ ÀÖÀ¸¸é... 
#ifdef _dUMENOKOJI_QUEST_
	if( GetTextArrStartNum() < GetTextArrEndNum() )
	{
		pUIBtn->Show( TRUE );
		pUIBtp->Show( TRUE );
		pUIQuestYBtn->Show( TRUE );
		pUIQuestNBtn->Show( TRUE );
		//ÀÌÀü ´ë»ç°¡ ÀÖ´Ù¸é...
		if( GetTextArrNowNum() > GetTextArrStartNum() )
		{
			pUIBtp->Enable( TRUE );
			pUIQuestYBtn->Enable( FALSE );
			pUIQuestNBtn->Enable( FALSE );
		}
		//ÀÌÀü ´ë»ç°¡ ¾ø´Ù¸é...
		else
		{
			pUIBtp->Enable( FALSE );
			pUIQuestYBtn->Enable( FALSE );
			pUIQuestNBtn->Enable( FALSE );
		}		
		pUIBtn->Enable( TRUE );
	}
	//´ÙÀ½ ´ë»ç°¡ ¾ø´Ù¸é.. 
	else// if( !pNPCScript->IsNextTalk() ) 					
	{
		pUIBtn->Show( TRUE );
		pUIBtp->Show( TRUE );
		pUIQuestYBtn->Show( TRUE );
		pUIQuestNBtn->Show( TRUE );
		 //ÀÌÀü ´ë»ç°¡ ÀÖ´Ù¸é..
		 if( GetTextArrNowNum() < GetTextArrStartNum() )
		 {
			pUIBtp->Enable( TRUE );
			pUIQuestYBtn->Enable( TRUE );
			pUIQuestNBtn->Enable( TRUE );
		 }
		 //ÀÌÀü ´ë»ç°¡ ¾ø´Ù¸é.
		 else
		 {
			pUIBtp->Enable( FALSE );
			pUIQuestYBtn->Enable( TRUE );
			pUIQuestNBtn->Enable( TRUE );
		 }
		 pUIBtn->Enable( FALSE );
	}



#else
	if( pNPCScript->IsNextTalk() )		pUIBtn->Show( TRUE );
	else								pUIBtn->Show( FALSE );
#endif
	return TRUE;
}

char* CW3DUInterface::GetPrevText( int arrNum )
{
	return (char*)m_PrevText[arrNum];
}

void CW3DUInterface::SetPrevText( char *Buf, int arrNum )
{
	strcpy( m_PrevText[arrNum], Buf );
}

void CW3DUInterface::SetTextArrStartNum( int StartNum )
{
	m_StartNum = StartNum;
}

int	CW3DUInterface::GetTextArrStartNum(  )
{
	return m_StartNum;
}

void CW3DUInterface::SetTextArrEndNum( int EndNum )
{
	m_EndNum = EndNum;
}

int	CW3DUInterface::GetTextArrEndNum(  )
{
	return m_EndNum;
}

void CW3DUInterface::InitTextArroNowNum(  )
{
	m_NowNum = 0;
}

void CW3DUInterface::SetTextArrNowNumPlus( int plusNum )
{
	m_NowNum += plusNum;
}

void CW3DUInterface::SetTextArrNowNumMinus( int minusNum )
{
	m_NowNum -= minusNum;
}

int	CW3DUInterface::GetTextArrNowNum(  )
{
	return m_NowNum;
}


BOOL CW3DUInterface::PreQuestNPC( IW3DNPCScript* pNPCScript )
{
	UIStaticText*		pUISText	= (UIStaticText*)GetComponent( "TXT_QUEST_SAY_TEXT" );
	UISelectString*		pUISStr		= (UISelectString*)GetComponent( "SSTR_NPC_ANSWER" );

	UIButton*			pUIBtn		= (UIButton*)GetComponent( "BTN_QUEST_SAY_NEXT" );
	UIButton*			pUIBtp		= (UIButton*)GetComponent( "BTN_QUEST_SAY_PRE" );

	UIButton*			pUIQuestYBtn = (UIButton*)GetComponent( "BTN_QUEST_YES" );
	UIButton*			pUIQuestNBtn = (UIButton*)GetComponent( "BTN_QUEST_NO" );
	
	if( !pUISStr || !pUISText || !pUIBtn || !pUIBtp || !pUIQuestYBtn || !pUIQuestNBtn )
	{
		Assert( pNPCScript );
		return FALSE;
	}

	pUISStr->Cleanup();
	pUISText->Cleanup();
	SetTextArrNowNumMinus( 1 );

	char	*szText = (char*)GetPrevText( GetTextArrNowNum() );
	pUISText->SetText( szText );
	
	pUIBtn->Show( TRUE );
	pUIBtp->Show( TRUE );
	//¸¸¾à ´ÙÀ½ ´ë»ç°¡ ÀÖ°í...
	if( GetTextArrNowNum() < GetTextArrEndNum() )
	{
		pUIBtn->Show( TRUE );
		pUIBtp->Show( TRUE );
		pUIQuestYBtn->Show( TRUE );
		pUIQuestNBtn->Show( TRUE );

		//ÀÌÀü ´ë»ç°¡ ÀÖ´Ù¸é...
		if( GetTextArrNowNum( ) > GetTextArrStartNum() )
		{
			pUIBtp->Enable( TRUE );
			pUIQuestYBtn->Enable( FALSE );
			pUIQuestNBtn->Enable( FALSE );
		}
		//ÀÌÀü ´ë»ç°¡ ¾ø´Ù¸é...
		else
		{
			pUIBtp->Enable( FALSE );
			pUIQuestYBtn->Enable( FALSE );
			pUIQuestNBtn->Enable( FALSE );
		}
		
		pUIBtn->Enable( TRUE );
	}
	//´ÙÀ½ ´ë»ç°¡ ¾ø´Ù¸é.. 
	else// if( !pNPCScript->IsNextTalk() ) 					
	{
		pUIBtn->Show( TRUE );
		pUIBtp->Show( TRUE );
		pUIQuestYBtn->Show( TRUE );
		pUIQuestNBtn->Show( TRUE );

		//ÀÌÀü ´ë»ç°¡ ÀÖ´Ù¸é..
		if(  GetTextArrNowNum( ) > GetTextArrStartNum()  )
		{
			pUIBtp->Enable( TRUE );
			pUIQuestYBtn->Enable( FALSE );
			pUIQuestNBtn->Enable( FALSE );
		}
		//ÀÌÀü ´ë»ç°¡ ¾ø´Ù¸é.
		else
		{
			pUIBtp->Enable( FALSE );
			pUIQuestYBtn->Enable( FALSE );
			pUIQuestNBtn->Enable( FALSE );
		}
		pUIBtn->Enable( FALSE );
	}
	
	return TRUE;
}

/**
 *
 */
BOOL CW3DUInterface::NextQuestNPC( IW3DNPCScript* pNPCScript, int nAnswer )
{
	if( !pNPCScript )
	{
		Assert( pNPCScript );
		return FALSE;
	} //if

	UIStaticText*		pUISText	= (UIStaticText*)GetComponent( "TXT_QUEST_SAY_TEXT" );
	UISelectString*		pUISStr		= (UISelectString*)GetComponent( "SSTR_NPC_ANSWER" );

	UIButton*			pUIBtn		= (UIButton*)GetComponent( "BTN_QUEST_SAY_NEXT" );
	UIButton*			pUIBtp		= (UIButton*)GetComponent( "BTN_QUEST_SAY_PRE" );

	UIButton*			pUIQuestYBtn = (UIButton*)GetComponent( "BTN_QUEST_YES" );
	UIButton*			pUIQuestNBtn = (UIButton*)GetComponent( "BTN_QUEST_NO" );

	if( !pUISStr || !pUISText || !pUIBtn || !pUIBtp || !pUIQuestYBtn || !pUIQuestNBtn )
	{
		Assert( !"ÄÄÆ÷³ÍÆ® ¾ò±â ½ÇÆÐ" );
		return FALSE;
	} //if
		
	pUISStr->Cleanup();
	pUISText->Cleanup();
	SetTextArrNowNumPlus( 1 );

#ifdef _dUMENOKOJI_QUEST_
	char	*szText = (char*)GetPrevText( GetTextArrNowNum() );
	pUISText->SetText( szText );

#else
	if( !pNPCScript->NextTalk( nAnswer ) )
	{
		ShowDlg( "DLG_NPC_ANSWER", FALSE );
		ShowDlg( "DLG_NPC_QUEST_SAY", FALSE );
		return TRUE;
	} //if

	char*	szText = (char*)pNPCScript->GetText();
	int		nAskNum = pNPCScript->GetAskNum();
	
	pUISText->SetText( szText );
	pNPCScript->SetPreTalkNum();

	for( int nCnt = 0 ; nCnt < nAskNum ; nCnt++ )
	{
		szText = (char*)pNPCScript->GetAskText( nCnt );
		if( szText )
		{
			pUISStr->AddString( szText );
		} //if
	} //for

	if( nAskNum > 0 )
	{
		ShowDlg( "DLG_NPC_ANSWER", TRUE );
		pUIBtn->Show( FALSE );
	}
	else
		ShowDlg( "DLG_NPC_ANSWER", FALSE );
	{
		pUIBtn->Show( TRUE );
	} //if..else..

	ShowDlg( "DLG_NPC_QUEST_SAY", TRUE );
#endif
	
	//´ÙÀ½ ´ë»ç°¡ ÀÖÀ¸¸é... 
#ifdef _dUMENOKOJI_QUEST_
	if( GetTextArrNowNum() < GetTextArrEndNum() )
	{
		pUIBtn->Show( TRUE );
		pUIBtp->Show( TRUE );
		pUIQuestYBtn->Show( TRUE );
		pUIQuestNBtn->Show( TRUE );
		//ÀÌÀü ´ë»ç°¡ ÀÖ´Ù¸é...
		if( GetTextArrNowNum( ) > GetTextArrStartNum() )
		{
			pUIBtp->Enable( TRUE );
			pUIQuestYBtn->Enable( FALSE );
			pUIQuestNBtn->Enable( FALSE );
		}
		//ÀÌÀü ´ë»ç°¡ ¾ø´Ù¸é...
		else
		{
			pUIBtp->Enable( FALSE );
			pUIQuestYBtn->Enable( FALSE );
			pUIQuestNBtn->Enable( FALSE );
		}
		
		pUIBtn->Enable( TRUE );
	}
	//´ÙÀ½ ´ë»ç°¡ ¾ø´Ù¸é.. 
	else// if( !pNPCScript->IsNextTalk() ) 					
	{
		pUIBtn->Show( TRUE );
		pUIBtp->Show( TRUE );
		pUIQuestYBtn->Show( TRUE );
		pUIQuestNBtn->Show( TRUE );
		//ÀÌÀü ´ë»ç°¡ ÀÖ´Ù¸é..
		if(  GetTextArrNowNum( ) > GetTextArrStartNum()  )
		{
			pUIBtp->Enable( TRUE );
			pUIQuestYBtn->Enable( TRUE );
			pUIQuestNBtn->Enable( TRUE );
		}
		//ÀÌÀü ´ë»ç°¡ ¾ø´Ù¸é.
		else
		{
			pUIBtp->Enable( FALSE );
			pUIQuestYBtn->Enable( TRUE );
			pUIQuestNBtn->Enable( TRUE );
		}
		pUIBtn->Enable( FALSE );
		
	}
	
#else
	if( pNPCScript->IsNextTalk() ) pUIBtn->Show( TRUE );
	else pUIBtn->Show( FALSE );
#endif

	return TRUE;
} //CW3DUInterface::NextNPC
#endif


#ifdef _dUMENOKOJI_RELOAD_
void CW3DUInterface::ReLoadUI()
{
	char		szScripName[64];
	sprintf( szScripName, "Interface/ui%04d.ini", g_nScreenWidth );
	if( !Load( szScripName, "Interface/guistate.ini" ) )
	{
		Assert( !"UI ½ºÅ©¸³Æ® ·Îµå ½ÇÆÐ" );
	} //if
}
#endif



/**
 * UseSkill2	
 */
void CW3DUInterface::UseSkill2( BYTE nIndx )
{
	UseSkill( nIndx );
}