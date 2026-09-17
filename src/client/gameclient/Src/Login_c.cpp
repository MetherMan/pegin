/**
 * ·Î±×ÀÎ(Áß±¹) ÇÁ·¹ÀÓ.
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	ÆÄ¶õ~*
 * @date	$Date: 04-06-23 12:08p $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /GameClient/Src/Login_c.cpp $
 * 
 * 11    04-06-23 12:08p Sckyj
 * 
 * 10    04-02-16 10:12p Paran2
 * 
 * 9     04-02-10 3:19p Paran
 * 
 * 4     04-01-05 3:43p Sckyj			BGM Àç»ý°ü·Ã ÄÚµå ¼öÁ¤
 * 
 * 3     04-01-02 2:20p Paran
 * ¸Þ¼¼Áö define Ã³¸®
 * 
 * 2     03-12-24 8:05p Paran
 * 
 * @file	Login_c.cpp
 */


#include "Global.h"
#include "Client.h"
#include "Protocol.h"
#include "Packet.h"
#include "Type.h"
#include <string>

#define SIZE_BG			512

#define MAX_DGIP_LENGTH             32
#define MAX_DGPORT_LENGTH           16
#define MAX_DGPARAMETER_LENGTH      1024


using std::string;


/**
 * ·Î±×ÀÎ ÇÁ·¹ÀÓ ±¸ÇöºÎ.
 * @ingroup		Game
 */
class CLogin : public IFrame
{
public:
	UNDER_MEMORY_MGR( CLogin );

	/**
	 * »ý¼ºÀÚ.
	 */
	CLogin()
	:	m_dwTick		( 0 ),
		m_bQuit			( FALSE ),
		m_bExitFrame	( FALSE ),
		m_sGameHost		( g_sLoginServerHost ),
		m_bConnect		( FALSE ),
		m_bLoginRequest	( FALSE )
	{
	} //CLogin

	/**
	 * ¼Ò¸êÀÚ.
	 */
	~CLogin()
	{
		SAFE_RELEASE( m_pBackground );
	} //~CLogin

	/**
	 * ·Î±×ÀÎ Àå¸éÀ» ÃÊ±âÈ­ÇÑ´Ù.
	 */
	BOOL	Init()
	{
		m_pBackground = MAKE_COMPONENT( Sprite );
		CHECK_PTR_RETURN( m_pBackground );

		char		szLoginBack[64];
		if( g_nWidth >= 1024 )
		{
			sprintf( szLoginBack, "Interface/loginback%04d.bmp", 1024 );
		}

		else
		{
			sprintf( szLoginBack, "Interface/loginback%04d.bmp", g_nWidth );
		}
		
		if( !m_pBackground->Load( szLoginBack, g_nWidth, g_nHeight ) )
		{

			return FALSE;
		} //if

		if( g_nWidth >= 1024 )
		{
			m_nDrawX = ( g_nWidth / 2 ) - ( 1024 / 2 );
			m_nDrawY = ( g_nHeight / 2 ) - ( 768 / 2 );
		}	
		
		g_pGUI->SetModeState( "[LOGIN_MODE]" );

		g_pMediaPlayer->StopAll();
		g_pMediaPlayer->Play( BGM_TYPE_EVENT, BGM_EVENT_LOGIN );

		return TRUE;
	} //Init

	/**
	 * ÇÁ·¹ÀÓÀ» °»½ÅÇÑ´Ù.
	 *
	 * @return	°è¼Ó ÁøÇàÇÒÁö ¿©ºÎ
	 */
	BOOL	Update(
		const DWORD		dwTick,		///< [IN] °æ°ú Tick
		string*			psName		///< [OUT] ´Ù¸¥ °ÔÀÓ »óÅÂ ÇÁ·¹ÀÓÀ¸·Î ³Ñ±æ °æ¿ì,
									///<       ³Ñ¾î°¥ ÇÁ·¹ÀÓ ÀÌ¸§
		)
	{
		Assert( psName );
		Assert( g_pClient );
		Assert( g_pGUI );

		UpdateNetwork( dwTick );

		UpdateInput( dwTick );

		if( m_bQuit )
		{	// ÇÁ·Î±×·¥ Á¾·á.
			return FALSE;
		} //if

		if( m_bExitFrame )
		{	// Ä³¸¯ÅÍ ¼±ÅÃÀ¸·Î ³Ñ¾î°¨.
			*psName = "SelectCharacter";
			return TRUE;
		} //if

		Render( dwTick );

		m_dwTick += dwTick;

		return TRUE;
	} //Update

private:
	/**
	 * ·Î±×ÀÎ Àå¸éÀ» ±×·ÁÁØ´Ù.
	 *
	 * @param	dwTick		[IN] °æ°ú Tick
	 */
	void	Render( const DWORD dwTick )
	{
		Assert( g_pScene );
		Assert( g_pGUI );

		if( !g_pScene->BeginFrame() )		return;

		m_pBackground->Draw( m_nDrawX, m_nDrawY, 0, 0, 1024, 768 );

		g_pGUI->Draw();

		g_pScene->EndFrame();
	} //Render

	/**
	 * ÀÔ·Â »óÅÂ¿¡ µû¶ó Ã³¸®¸¦ ÇØÁØ´Ù.
	 *
	 * @param	dwTick		[IN] °æ°ú Tick
	 */
	void	UpdateInput( const DWORD dwTick )
	{
		Assert( g_pGUI );
		Assert( g_pClient );

		g_pInput->Update();
		if( g_pGUI->PopEvent( g_szUIName, &g_UIevent, &g_dwUIParam1, &g_dwUIParam2 ) )
		{
			switch( g_UIevent )
			{
			case EVENT_BUTTON:		OnEventButton( g_szUIName );			break;
			case EVENT_RETURNKEY:	OnEventReturnKey( g_szUIName );		break;
			} //switch
		} //if

		g_pGUI->Update( dwTick );
	} //UpdateInput

	/**
	 * ³×Æ®¿÷À» Ã³¸®ÇÑ´Ù.
	 *
	 * @param	dwTick		[IN] °æ°ú Tick
	 */
	void	UpdateNetwork( const DWORD dwTick )
	{
		if( !m_bConnect )	return;

		Assert( g_pClient );
		Assert( g_pGUI );

		if( !g_pClient->Recv() )
		{
			g_pClient->Disconnect();
		} //if

		if( !g_pClient->IsConnect() )
		{
			g_pGUI->MessageBox(
				STR_SYSERR_SERVERDISCONNECT,
				"DISCONNECT",
				IW3DUInterface::MSG_OK );
			m_bConnect = FALSE;
		} //if

		if( m_bLoginRequest &&
			GetTickCount() - m_dwLoginRequestTick > 10000 )
		{
			g_pGUI->MessageBox(
				STR_SYSERR_NOREPRYLOGIN,
				"DISCONNECT",
				IW3DUInterface::MSG_OK );
			g_pClient->Disconnect();
			m_bConnect = FALSE;
		} //if 

		while( g_pClient->GetRecvSize() > 2 )
		{
			char*		pData = const_cast<char*>( g_pClient->GetRecvData() );
			int			nSize = *reinterpret_cast<WORD*>( pData );

			if( nSize > g_pClient->GetRecvSize() )	break;

			//¿©±â¼­ ¾ÐÃà Ç¬´Ù unzip here
			char	pDecryptData[4096];
			DWORD	key[4] = { dAMHO_KEY1, dAMHO_KEY2, dAMHO_KEY3, dAMHO_KEY4 };
			DWORD	dwEncSize = TEA_DecryptData( pData + 2, pDecryptData, nSize - 2, key );

			char	pDec[4096];
			memcpy( pDec, &pDecryptData[2], dwEncSize );
			dwEncSize -= 2; 



			m_recvPacket.SetPacket( dwEncSize + 2, pDec );
			ProcessRecvPacket();
			g_pClient->RemoveRecvData( nSize ); 

	//		m_recvPacket.SetPacket( nSize, pData + 2 );
	//		ProcessRecvPacket();
	//		g_pClient->RemoveRecvData( nSize );

			if( m_bExitFrame )	break;	// —?!!!
		} //while
	} //UpdateNetwork

	/**
	 * ¹ÞÀº ÆÐÅ¶À» Ã³¸®ÇÑ´Ù.
	 * ½ÇÁ¦·Î´Â ÆÐÅ¶ TAG¿¡ µû¶ó Ã³¸® ÇÔ¼ö¸¦ È£ÃâÇÑ´Ù.
	 */
	void	ProcessRecvPacket()
	{
		switch( m_recvPacket.GetTag() )
		{
		case dLOGIN_PACKET_LOGINRESULT:		OnLoginPacketLoginResult();		break;
		} //switch
	} //ProcessRecvPacket

	/**
	 * ·Î±×ÀÎÀ» ½ÃµµÇÑ´Ù.
	 */
	void	LoginRequest(
		const string&	sID,			///< [IN] ¾ÆÀÌµð
		const string&	sPassword		///< [IN] ÆÐ½º¿öµå
		)
	{
		Assert( g_pGUI );
		Assert( g_pClient );

		if( m_bLoginRequest )
		{	// ÀÌ¹Ì ·Î±×ÀÎ Ã³¸® ÁßÀÏ °æ¿ì¿£ ¾Æ¹« °Íµµ ÇÏÁö ¾Ê´Â´Ù.
			return;
		} //if

		if( 0 == sID.size() )
		{
			g_pGUI->ShowDlg( "DLG_LOGIN", TRUE );
			g_pGUI->MessageBox(
				STR_SYSMSG_INPUTID,
				"ID",
				IW3DUInterface::MSG_OK );
			return;
		} //if
		if( 0 == sPassword.size() )
		{
			g_pGUI->ShowDlg( "DLG_LOGIN", TRUE );
			g_pGUI->MessageBox(
				STR_SYSMSG_INPUTPASSWORD,
				"PASSWORD",
				IW3DUInterface::MSG_OK );
			return;
		} //if

		int	nPatchVer = 0;
		nPatchVer = dPATCHVER;

		g_sUserGameId = g_sUserDaumId = sID;
		
#ifdef _SAMVERSION
		g_sUserGameId = sID;
		g_sUserDaumId = sPassword;
		m_bExitFrame = TRUE;
		return;
#endif
		if( !m_bConnect )
		{
			//if( !g_pClient->Connect( m_sGameHost.c_str(), 8030 ) )
			if( !g_pClient->Connect( m_sGameHost.c_str(), 8030 ) ) //8205
			{	// ¼­¹ö¿¡ Á¢¼Ó ½ÇÆÐ.
				g_pGUI->ShowDlg( "DLG_LOGIN", TRUE );
				g_pGUI->ShowDlg( "DLG_MESSAGEBOX", FALSE );
				g_pGUI->MessageBox(
					STR_SYSERR_NOTCONNECTSERVER,
					"CANNOT_CONNECT",
					IW3DUInterface::MSG_OK );

				m_bConnect		= FALSE;
				m_bLoginRequest	= FALSE;

				return;
			} //if
		} //if

		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_LOGINREQUEST )
			<< sID
			<< sPassword
			<< nPatchVer;
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

		g_pGUI->ShowDlg( "DLG_LOGIN", TRUE );
		g_pGUI->MessageBox( STR_SYSMSG_LOGINING );

		m_bConnect				= TRUE;
		m_bLoginRequest			= TRUE;
		m_dwLoginRequestTick	= GetTickCount();
	} //LoginRequest

	/**
	 * ¹öÆ°ÀÌ ´­¸° °æ¿ì¸¦ Ã³¸®ÇÑ´Ù.
	 *
	 * @param	sName		[IN] ¹ß»ý ÄÄÆ÷³ÍÆ® ÀÌ¸§
	 */
	void	OnEventButton( const char* szUIName )\
	{
		if( !strcmp( szUIName, "BTN_LOGIN_YES" ) )
		{	// ·Î±×ÀÎ È®ÀÎ ¹öÆ°ÀÌ ´­¸².
			char*	szID       = strlwr( (char*)g_pGUI->GetText( "EDT_LOGIN_ID" ) );
			char*	szPassword = (char*)g_pGUI->GetText( "EDT_LOGIN_PW" );

			Assert( szID );
			Assert( szPassword );

			LoginRequest( szID, szPassword );
		}
		else if( !strcmp( szUIName, "BTN_LOGIN_NO" ) )
		{	// Ãë¼Ò ¹öÆ°ÀÌ ´­¸². (Á¾·á)
			Sleep( 100 );

			m_bQuit = TRUE;
		}
		else if( !strcmp( szUIName, "BTN_MSG_DISCONNECT_OK" ) )
		{	// ¼­¹ö¿Í Á¢¼ÓÀÌ ²÷°åÀ½. È®ÀÎ.
			m_bQuit = TRUE;
		} //if.. else if..
	} //OnEventButton

	/**
	 * ¿£ÅÍ°¡ ´­¸° °æ¿ì¸¦ Ã³¸®ÇÑ´Ù.
	 *
	 * @param	sName		[IN] ¹ß»ý ÄÄÆ÷³ÍÆ® ÀÌ¸§.
	 */
	void	OnEventReturnKey( const char* szUIName )
	{
		char*	szID       = strlwr( (char*)g_pGUI->GetText( "EDT_LOGIN_ID" ) );
		char*	szPassword = (char*)g_pGUI->GetText( "EDT_LOGIN_PW" );

		Assert( szID );
		Assert( szPassword );

		if( !strcmp( szUIName, "EDT_LOGIN_ID" ) ||
			!strcmp( szUIName, "EDT_LOGIN_PW" ) )
		{
			LoginRequest( szID, szPassword );
		} //if
	} //OnEventReturnKey

	/**
	 * ·Î±×ÀÎ °á°ú
	 */
	void	OnLoginPacketLoginResult()
	{
		char szUserID[256];
		char szDaumID[256];
		BYTE btResult;

		m_recvPacket 
			>> btResult;

		if( btResult == 3 )
		{
		/*	m_recvPacket 
				>> szUserID
				>> szDaumID;*/
		} //if

		g_pClient->Disconnect();				//< ·Î±ä ¼­¹ö¿¡¼­ ¿¬°áÇØÁ¦.
		m_bConnect		= FALSE;
		m_bLoginRequest	= FALSE;

		switch( btResult )
		{
		case 0:			// ID¿À·ù
			g_pGUI->MessageBox( STR_SYSMSG_RETRYINPUTID );
			return;

		case 1:			// PW¿À·ù
			g_pGUI->MessageBox( STR_SYSMSG_RETRYINPUTPASSWORD );
			return;

		case 3:			// Á¢¼Ó ¿¡·¯
			g_pGUI->MessageBox( STR_SYSERR_NOTCONNECTLAQIA );
			return;

		case 2:			// LOGIN OK
			/*g_sUserGameId = szUserID;
			g_sUserDaumId = szDaumID;*/
		
			m_bExitFrame = TRUE;
			return;

		case 4:
			g_pGUI->MessageBox( STR_SYSERR_RETRYIDPW );
			return;
			
		} //switch
	} //OnLoginPacketLoginResult

	/**
	 * ·Î±×ÀÎ °á°ú ÆÐÅ¶À» Ã³¸®ÇÑ´Ù.
	 */
	void	OnPacketLoginResult()
	{
		Assert( g_pClient );
		Assert( g_pGUI );


		BYTE	bResult;
		m_recvPacket	>> bResult;

		switch( bResult )
		{
		case 0:		// ¾ÆÀÌµð ¾øÀ½.
			g_pGUI->ShowDlg( "DLG_LOGIN", TRUE );
			g_pGUI->MessageBox(
				STR_SYSMSG_RETRYINPUTID,
				"LOGIN_RESULT",
				IW3DUInterface::MSG_OK );
			break;

		case 1:		// ÆÐ½º¿öµå Æ²¸².
			g_pGUI->ShowDlg( "DLG_LOGIN", TRUE );
			g_pGUI->MessageBox(
				STR_SYSMSG_RETRYINPUTPASSWORD,
				"LOGIN_RESULT",
				IW3DUInterface::MSG_OK );
			break;

		case 2:		// ·Î±×ÀÎ ¼º°ø.
			g_pGUI->MessageBox( STR_SYSMSG_LOGINING );
			m_bExitFrame = TRUE;
			break;
		} //switch

		m_bLoginRequest = FALSE;
	} // OnPacketLoginResult

private:
	DWORD			m_dwTick;				///< ´©Àû tick

	IW3DSprite*		m_pBackground;			///< ¹è°æ ±×¸²

	int				m_nMouseX;				///< ¸¶¿ì½º Ä¿¼­ X ÁÂÇ¥
	int				m_nMouseY;				///< ¸¶¿ì½º Ä¿¼­ Y ÁÂÇ¥

	BOOL			m_bQuit;				///< Á¾·á
	BOOL			m_bExitFrame;			///< ÇÁ·¹ÀÓ Á¾·á

	string			m_sGameHost;			///< °ÔÀÓ È£½ºÆ®
	BOOL			m_bConnect;				///< ¼­¹ö¿Í Á¢¼Ó ¿©ºÎ
	BOOL			m_bLoginRequest;		///< ·Î±×ÀÎ ½Ãµµ
	DWORD			m_dwLoginRequestTick;	///< ·Î±×ÀÎ ½Ãµµ tick

	CPacket			m_recvPacket;			///< ¹ÞÀº ÆÐÅ¶
	
	int				m_nDrawX;
	int				m_nDrawY;
}; //class CLogin

IMPLEMENT_FRAME( CLogin, Login );
