/**
 * 로그인 프레임 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-12-08 3:06a $
 * @version	$Revision: 159 $
 *
 * <HR>
 * $Log: /GameClient/Src/Login.cpp $
 * 
 * 159   04-12-08 3:06a Sk8snow2
 * 
 * 158   04-02-28 10:19a Paran
 * 
 * 157   04-02-13 9:35a Sk8snow2
 * 
 * 156   04-02-03 4:36p Paran
 * 
 * 152   04-01-02 2:20p Paran			메세지 define 처리
 * 
 * 143   03-12-15 4:33p Sckyj			Login_portal bgm을 Intro에서부터 시작하던것을 Login쪽으로 변경
 * 
 * 142   03-12-14 6:10p Sckyj			BGM 재생관련 코드 수정
 * 
 * 141   03-12-13 3:40p Sckyj			login sound를 LAQIA 로고가 나올때부터 연주하도록 수정
 * 
 * 138   03-10-10 1:20p Kjmgo			Login 에 관련하여, 몇가지 수정및, 로그가 남도록 수정 
 * 
 * 137   03-10-06 7:19p Kjmgo			Send Fail. Bug. 
 * 
 * 120   03-06-24 11:17a Paran			다음 인증처리 추가.
 * 
 * 118   03-05-30 12:43a Paran			로그인 이미지 수정
 * 
 * 117   03-05-11 2:20p Ahastudio		모든 메소드를 클래스 안으로 넣음.
 *
 * 103   03-03-06 11:55p Ahastudio		Disconnect 메세지 버튼을 통한 종료. 받은 패킷을 멤버 변수로 이동.
 * <HR>
 *
 * @file	Login.cpp
 */


#include "Global.h"
#include "Client.h"
#include "Protocol.h"
#include "Packet.h"
#include "Type.h"
#include "DaumGameParameter.h"
#include <string>

#define DAUM_CONNECT
#define SIZE_BG			512

#define MAX_DGIP_LENGTH             32
#define MAX_DGPORT_LENGTH           16
#define MAX_DGPARAMETER_LENGTH      1024


using std::string;


/**
 * 로그인 프레임 구현부.
 * @ingroup		Game
 */
class CLogin : public IFrame
{
public:
	UNDER_MEMORY_MGR( CLogin );

	/**
	 * 생성자.
	 */
	CLogin()
	:	m_dwTick		( 0 ),
		m_bQuit			( FALSE ),
		m_bExitFrame	( FALSE ),
		m_sLoginHost	( g_sLoginServerHost ),
		m_bConnect		( FALSE ),
		m_bLoginRequest	( FALSE )
	{
	} //CLogin

	/**
	 * 소멸자.
	 */
	~CLogin()
	{
	} //~CLogin

	/**
	 * 로그인 장면을 초기화한다.
	 */
	BOOL	Init()
	{
		if( g_asGameServerHost[0] == "" )
		{
			FILE*	pFile = fopen( "accountpass.ini", "rt" );
			if( pFile )
			{
				char	szGameId[64];
				char	szDaumId[64];
				fscanf( pFile, "%s %s", szGameId, szDaumId );
				g_sUserGameId = szGameId;
				g_sUserDaumId = szDaumId;
				m_bExitFrame = TRUE;
				
				fclose( pFile );
				return TRUE;
			} //if
		}

		CDaumGameParameter cDGP; 

		char		szServerIP[MAX_DGIP_LENGTH];
//		char		szServerPort[MAX_DGPORT_LENGTH];
		char		szParameter[MAX_DGPARAMETER_LENGTH];

		// ServerPort 파라미터 사용 않함 ( sk8snow )
		if( cDGP.Get( "ServerIP", szServerIP, MAX_DGIP_LENGTH )			 == FALSE || 
			//cDGP.Get( "ServerPort", szServerPort, MAX_DGPORT_LENGTH )	 == FALSE || 
			cDGP.Get( "Parameter", szParameter, MAX_DGPARAMETER_LENGTH ) == FALSE ) 
		{ 
			MessageBox( NULL, STR_SYSERR_RUNFORWEB, "Deicide", MB_OK ); 
			return FALSE;
		} //if
		cDGP.Empty();

		g_btServer = atoi( szServerIP );
		if( g_btServer > 0 && g_btServer <= NUM_GAMESERVER )
		{
			g_btServer -= 1;
		}
		else
		{
			MessageBox( NULL, STR_SYSERR_IPINDEX, "Deicide", MB_OK );
			return FALSE;
		} //if..else

		ProcessLogin( szParameter );
		DEBUG_OUT( "다음 로긴!!" );
	
		string		strFrameName = "";
		while( 1 )
		{
			if( Update( 1, &strFrameName ) )
			{
				if( strFrameName != "" )
				{
					return TRUE;
				} //if
			}
			else
			{
				return FALSE;
			} //if..else
		} //while

		return TRUE;
	} //Init

	/**
	 * 우리 로긴
	 */
	BOOL	ProcessLogin( const char* szParameter )
	{
		if( g_pClient->Connect( m_sLoginHost.c_str(), 8030 ) ) //8999
		{
			m_bConnect				= TRUE;
			m_bLoginRequest			= TRUE;
			m_dwLoginRequestTick	= GetTickCount();

			CPacket		sendPacket;
			sendPacket.Tag( dLOGIN_PACKET_LOGININFO )
				<< szParameter;
			g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
			DEBUG_OUT( "dLOGIN_PACKET_LOGININFO" );
		}
		else
		{	// 서버에 접속 실패.
			MessageBox( NULL, STR_SYSERR_NOTCONNECTLOGINSERVER, "Deicide", MB_OK );
			m_bConnect		= FALSE;
			m_bLoginRequest	= FALSE;
			m_bQuit			= TRUE;

			Assert( !"서버에 접속할 수 없습니다." );
			Log( "Try Connecting, Login Server Address : %s\n", m_sLoginHost.c_str() );

			return FALSE;
		} //if..else

		return TRUE;
	} //ProcessLogin

	/**
	 * 프레임을 갱신한다.
	 *
	 * @return	계속 진행할지 여부
	 */
	BOOL	Update(
		const DWORD		dwTick,		///< [IN] 경과 Tick
		string*			psName		///< [OUT] 다른 게임 상태 프레임으로 넘길 경우,
									///<       넘어갈 프레임 이름
		)
	{
		Assert( psName );
		Assert( g_pClient );

		UpdateNetwork( dwTick );
		UpdateInput( dwTick );

		if( m_bQuit )
		{	// 프로그램 종료.
			return FALSE;
		} //if

		if( m_bExitFrame )
		{	// 캐릭터 선택으로 넘어감.
			*psName = "Intro";
			return TRUE;
		} //if

		//Render( dwTick );

		m_dwTick += dwTick;

		return TRUE;
	} //Update

private:
	/**
	 * 로그인 장면을 그려준다.
	 *
	 * @param	dwTick		[IN] 경과 Tick
	 */
	void	Render( const DWORD dwTick )
	{
		return;

		Assert( g_pScene );
		Assert( g_pGUI );

		if( !g_pScene->BeginFrame() )		return;


		g_pScene->EndFrame();
	} //Render

	/**
	 * 입력 상태에 따라 처리를 해준다.
	 *
	 * @param	dwTick		[IN] 경과 Tick
	 */
	void	UpdateInput( const DWORD dwTick )
	{
		return;

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
	 * 네트웍을 처리한다.
	 *
	 * @param	dwTick		[IN] 경과 Tick
	 */
	void	UpdateNetwork( const DWORD dwTick )
	{
		if( !m_bConnect )	return;

		Assert( g_pClient );

		if( !g_pClient->Recv() )
		{
			g_pClient->Disconnect();
		} //if

		if( !g_pClient->IsConnect() )
		{
 			MessageBox( NULL, STR_SYSERR_SERVERDISCONNECT, "Deicide", MB_OK );
			m_bConnect = FALSE;
		} //if

		if( m_bLoginRequest &&
			GetTickCount() - m_dwLoginRequestTick > 15000 )
		{
			MessageBox( NULL, STR_SYSERR_NOREPRYLOGIN, "Deicide", MB_OK );
			g_pClient->Disconnect();
			m_bConnect = FALSE;
			m_bQuit = TRUE;
		} //if

		if( m_bConnect )
		{
			g_pClient->Send();
		} //if

		while( g_pClient->GetRecvSize() > 2 )
		{
			char*		pData = const_cast<char*>( g_pClient->GetRecvData() );
			int			nSize = *reinterpret_cast<WORD*>( pData );

			if( nSize > g_pClient->GetRecvSize() )	break;




			
			m_recvPacket.SetPacket( nSize, pData + 2 );
			g_pClient->RemoveRecvData( nSize );
			ProcessRecvPacket();

			if( m_bExitFrame )	break;	// 쀍 !!!
		} //while
	} //UpdateNetwork

	/**
	 * 받은 패킷을 처리한다.
	 * 실제로는 패킷 TAG에 따라 처리 함수를 호출한다.
	 */
	void	ProcessRecvPacket()
	{
		switch( m_recvPacket.GetTag() )
		{
		case dLOGIN_PACKET_LOGINRESULT:		OnLoginPacketLoginResult();		break;
		} //switch
	} //ProcessRecvPacket

	/**
	 * 로그인을 시도한다.
	 */
	void	LoginRequest(
		const string&	sID,			///< [IN] 아이디
		const string&	sPassword		///< [IN] 패스워드
		)
	{
		Assert( g_pGUI );
		Assert( g_pClient );
		
		if( m_bLoginRequest )
		{	// 이미 로그인 처리 중일 경우엔 아무 것도 하지 않는다.
			return;
		} //if

		if( 0 == sID.size() )
		{
			Assert( !"없는 아이디" );
			return;
		} //if

		m_bConnect = FALSE;

		if( m_bConnect )
		{
			m_bLoginRequest			= TRUE;
			m_dwLoginRequestTick	= GetTickCount();
		}
		else
		{	// 서버에 접속 실패.
			m_bConnect		= FALSE;
			m_bLoginRequest	= FALSE;
		} //if..else


	} //LoginRequest

	/**
	 * 버튼이 눌린 경우를 처리한다.
	 *
	 * @param	sName		[IN] 발생 컴포넌트 이름
	 */
	void	OnEventButton( const char* szUIName )
	{
		if( !strcmp( szUIName, "BTN_LOGIN_YES" ) )
		{	// 로그인 확인 버튼이 눌림.
			char*	szID       = (char*)g_pGUI->GetText( "EDT_LOGIN_ID" );
			char*	szPassword = (char*)g_pGUI->GetText( "EDT_LOGIN_PW" );

			Assert( szID );
			Assert( szPassword );

			LoginRequest( szID, szPassword );
		}
		else if( !strcmp( szUIName, "BTN_LOGIN_NO" ) )
		{	// 취소 버튼이 눌림. (종료)
			Sleep( 100 );

			m_bQuit = TRUE;
		}
		else if( !strcmp( szUIName, "BTN_MSG_DISCONNECT_OK" ) )
		{	// 서버와 접속이 끊겼음. 확인.
			m_bQuit = TRUE;
		} //if.. else if..
	} //OnEventButton

	/**
	 * 엔터가 눌린 경우를 처리한다.
	 *
	 * @param	sName		[IN] 발생 컴포넌트 이름.
	 */
	void	OnEventReturnKey( const char* szUIName )
	{
		char*	szID       = (char*)g_pGUI->GetText( "EDT_LOGIN_ID" );
		char*	szPassword = (char*)g_pGUI->GetText( "EDT_LOGIN_PW" );

		Assert( szID );
		Assert( szPassword );

		if( ( !strcmp( szUIName, "EDT_LOGIN_ID" ) ) ||
			( !strcmp( szUIName, "EDT_LOGIN_PW" ) ) )
		{
			LoginRequest( szID, szPassword );
		} //if
	} //OnEventReturnKey

	/**
	 * 로그인 결과
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
			m_recvPacket 
				>> szUserID
				>> szDaumID;
		} //if

		g_pClient->Disconnect();				//< 로긴 서버에서 연결해제.
		m_bConnect		= FALSE;
		m_bLoginRequest	= FALSE;

		switch( btResult )
		{
		case 0:			// 오류
			MessageBox( NULL, STR_SYSERR_NOTCONNECTLAQIA, "Laqia", MB_OK );
			m_bQuit = TRUE;
			return;

		case 1:			// LAQIA 약관을 동의하지 않은 사용자
			MessageBox( NULL, STR_SYSERR_NOACCEPTLAQIA, "Laqia", MB_OK );
			m_bQuit = TRUE;
			return;

		case 2:			// 존재하지 않는 ID
			MessageBox( NULL, STR_SYSERR_NOTFOUNDIDLAQIA, "Laqia", MB_OK );
			m_bQuit = TRUE;
			return;

		case 3:			// LOGIN OK
			g_sUserGameId = szUserID;
			g_sUserDaumId = szDaumID;
			
			m_bExitFrame = TRUE;
			return;
		} //switch
	} //OnLoginPacketLoginResult

	/**
	 * 다음인증 처리.
	 */
	/*void	ProcessDaumAccount()
	{
		if( ProcessDaumAccountMessage( nResult ) )
		{ // OK
			if( g_pClient->Connect( m_sLoginHost.c_str(), 7000 ) )
			{
				m_bConnect				= TRUE;
				m_bLoginRequest			= TRUE;
				m_dwLoginRequestTick	= GetTickCount();

				g_pGUI->ShowDlg( "DLG_LOGIN", TRUE );
				g_pGUI->MessageBox( "로그인 처리 중..." );

				CPacket		sendPacket;
				sendPacket.Tag( dLOGIN_PACKET_LOGININFO )
					<< m_DaumLogin.GetHMCU()
					<< m_DaumLogin.GetHTS()
					<< m_DaumLogin.GetTS()
					<< m_DaumLogin.GetPROF();
				g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
			}
			else
			{	// 서버에 접속 실패.
				g_pGUI->ShowDlg( "DLG_LOGIN", TRUE );
				g_pGUI->ShowDlg( "DLG_MESSAGEBOX", FALSE );
				g_pGUI->MessageBox(
					"서버에 접속할 수 없습니다.",
					"CANNOT_CONNECT",
					IW3DUInterface::MSG_OK );

				m_bConnect		= FALSE;
				m_bLoginRequest	= FALSE;


				Assert( !"서버에 접속할 수 없습니다." );
				Log( "Try Connecting, Login Server Address : %s\n", m_sLoginHost.c_str() );
			} //if..else
			m_DaumLogin.Close();
		}
		else
		{
			m_DaumLogin.Init();
			m_bConnect		= FALSE;
			m_bLoginRequest	= FALSE;
		} //if..else
	} //ProcessDaumAccount

	/**
	 * 다음 이증에 대한 에러 처리.
	 */
/*	BOOL	ProcessDaumAccountMessage( const int nResult )
	{
		switch( nResult )
		{
		case 200:																return TRUE;
		case 404:
		case 601:	g_pGUI->MessageBox(	"존재하지 않는 아이디입니다." );		return FALSE;
		case 403:
		case 602:	g_pGUI->MessageBox(	"사용중지된 아이디입니다." );			return FALSE;
		case 400:	g_pGUI->MessageBox(	"아이디/패스워드를 입력하세요" );		return FALSE;
		case 203:	g_pGUI->MessageBox(	"아이디/패스워드를 확인하세요." );		return FALSE;
		case 412:	g_pGUI->MessageBox(	"부모님의 동의가 필요합니다" );			return FALSE;
		case 415:	g_pGUI->MessageBox(	"법인확인이 필요합니다" );				return FALSE;
		case 423:	g_pGUI->MessageBox(	"서버 작업중인 아이디입니다." );		return FALSE;
		case 603:	g_pGUI->MessageBox(	"서버 내부 에러." );					return FALSE;
		case 604:	g_pGUI->MessageBox(	"Mail 서버 에러." );					return FALSE;
		case 605:	g_pGUI->MessageBox(	"해당 mail 서버 에러." );				return FALSE;
		case 606:	g_pGUI->MessageBox(	"사용자 타임아웃." );					return FALSE;
		case 500:	g_pGUI->MessageBox(	"서버 내부 에러." );					return FALSE;
		} //switch
		return FALSE;
	} //ProcessDaumAccountMessage
*/
private:
	//CDaumLogin		m_DaumLogin;			///< 다음 계정 받는 클래스.
	BOOL			m_bDaumLogin;			///< 다음을 통한 로긴.

	DWORD			m_dwTick;				///< 누적 tick

	IW3DSprite*		m_pBackground;			///< 배경 그림
	IW3DSprite*		m_pBackAni;				///< 배경 에니

	IW3DModel*		m_pModel;				///< 배경 모델
	IW3DLight*		m_pLight;				///< 빛

	int				m_nMouseX;				///< 마우스 커서 X 좌표
	int				m_nMouseY;				///< 마우스 커서 Y 좌표

	BOOL			m_bQuit;				///< 종료
	BOOL			m_bExitFrame;			///< 프레임 종료

	string			m_sLoginHost;			///< 게임 호스트
	BOOL			m_bConnect;				///< 서버와 접속 여부
	BOOL			m_bLoginRequest;		///< 로그인 시도
	DWORD			m_dwLoginRequestTick;	///< 로그인 시도 tick

	CPacket			m_recvPacket;			///< 받은 패킷
}; //class CLogin


IMPLEMENT_FRAME( CLogin, Login );