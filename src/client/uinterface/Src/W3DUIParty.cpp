/**
 * 유저 인터페이스 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-12-08 3:07a $
 * @version	$Revision: 31 $
 *
 * <HR>
 * $Log: /UInterface/Src/W3DUIParty.cpp $
 * 
 * 31    04-12-08 3:07a Sk8snow2
 * 
 * 30    04-08-18 5:34p Paran
 * 
 * 29    04-08-03 10:49p Paran
 * 
 * 28    04-08-03 5:18p Paran
 * 
 * 27    04-08-03 12:42a Paran
 * 
 * 12    04-01-02 2:21p Paran		메세지 define 처리
 * 
 * 5     03-05-28 4:55p Paran		파티 위치 미니맵 표시 수정
 * 
 * 4     03-05-28 4:14p Paran		미니 맵 파티 관련 수정
 * 
 * 2     03-05-20 9:35p Paran		계속 수정 중...
 * 
 * <HR>
 *
 * @file	W3DUIParty.cpp
 */

#include "UInterface_Export.h"
#include "W3DUInterface.h"

/**
 * 파티맴버 정보를 다시 보여준다.
 */
void CW3DUInterface::RefreshPartyInfo()
{
	sPartyMember	*pParty;
	char			szTemp[64];

	LIST_PARTYMEMBER::iterator	itetList_Party = m_listPartyMember.begin();
	for( int nCnt = 0; nCnt < MAX_PARTYNUM; nCnt++ )
	{
		if( itetList_Party != m_listPartyMember.end() )
		{
			pParty = &(*itetList_Party);
		
			pParty->nSlotNum = nCnt;
			sprintf( szTemp, "PRO_PARTY%d_HP",  nCnt );
			SetProgressPos( szTemp, pParty->nMaxHp, pParty->nHp, TRUE );

			sprintf( szTemp, "TXT_PARTY%d",		nCnt );
			SetText( szTemp, pParty->szName  );

			sprintf( szTemp, "PRO_PARTY%d_MP",  nCnt );
			SetProgressPos( szTemp, pParty->nMaxMp, pParty->nMp, TRUE );

			sprintf( szTemp, "DLG_PARTY%d",		nCnt );
			ShowDlg( szTemp, TRUE );

			itetList_Party++;
		}
		else
		{
			sprintf( szTemp, "DLG_PARTY%d",		nCnt );
			ShowDlg( szTemp, FALSE );
		} // if..else
	}
} //CW3DUInterface::RefreshPartyInfo

/**
 * 파티맴버 추가.
 */
void CW3DUInterface::AddPartyMember( int nId, const char* szName, int nHp, int nMaxHp, int nMp,	int nMaxMp )
{
	LIST_PARTYMEMBER::iterator		itor = m_listPartyMember.begin();
	while( itor != m_listPartyMember.end() )
	{
		sPartyMember*	psMemver = &(*itor);
		if( !strcmp( psMemver->szName, szName ) )
		{ // 중복
			return;
		} //if
		itor++;
	} //while

	sPartyMember		Party;
	Party.nId		= nId;
	Party.nHp		= nHp;
	Party.nMaxHp	= nMaxHp;
	Party.nMp		= nMp;
	Party.nMaxMp	= nMaxMp;
	strcpy( Party.szName, szName );
	m_listPartyMember.push_back( Party );

	RefreshPartyInfo();

	SetDamage( NULL );
	SetDefence();
} //CW3DUInterface::AddPartyMember

/**
 * 파티원을 삭제.
 */
void CW3DUInterface::DeletePartyMember( int nId )
{
	int				nCnt = 0;
	sPartyMember	*pParty;
	LIST_PARTYMEMBER::iterator	itetList_Party = m_listPartyMember.begin();
	while( itetList_Party != m_listPartyMember.end() )
	{
		pParty = &(*itetList_Party);
		if( pParty->nId == nId )
		{
			char		szText[64];
			sprintf( szText, "%s%s", pParty->szName, STR_PARTY_DELETEMEMBER );
			MessageBox( szText );
			m_listPartyMember.erase( itetList_Party );
			m_pMiniMap->DeletePoint( nCnt );
			m_pLargeMap->DeletePoint( nCnt );
			break;
		}
		itetList_Party++;
		nCnt++;
	}

	RefreshPartyInfo();

	SetDamage( NULL );
	SetDefence();
} //CW3DUInterface::DeletePartyMember

/** 
 * 파티원 모두 지운다.
 */
void CW3DUInterface::DeletePartyAll()
{
	int							nCnt = 0;
	LIST_PARTYMEMBER::iterator	itetList_Party = m_listPartyMember.begin();
	while( itetList_Party != m_listPartyMember.end() )
	{
		itetList_Party = m_listPartyMember.erase( itetList_Party );
		m_pMiniMap->DeletePoint( nCnt );
		m_pLargeMap->DeletePoint( nCnt );
		nCnt++;
	}

	RefreshPartyInfo();
} //CW3DUInterface::DeletePartyAll

/**
 * 아이디가 파티인지 확인.
 */
BOOL CW3DUInterface::IsPartyMember( const DWORD dwId )
{
	int					nCnt   = 0;
	sPartyMember*		pParty = NULL;
	LIST_PARTYMEMBER::iterator	itetList_Party = m_listPartyMember.begin();
	while( itetList_Party != m_listPartyMember.end() )
	{
		pParty = &(*itetList_Party);
		if( pParty->nId == dwId )
		{
			return TRUE;
		} //if
		itetList_Party++;
	} //while
	return FALSE;
} //CW3DUInterface::IsPartyMember

/** 
 * 파티원 HP, MP 정보 갱신.
 */
void CW3DUInterface::RefreshPartyMember( int nId, int nHp, int nMaxHp, int nMp, int nMaxMp )
{
	char szTemp[128];
	
	sPartyMember	*pParty;
	LIST_PARTYMEMBER::iterator	itetList_Party = m_listPartyMember.begin();
	while( itetList_Party != m_listPartyMember.end() )
	{
		pParty = &(*itetList_Party);
		if( pParty->nId == nId )
		{
			pParty->nId		= nId;
			pParty->nHp		= nHp;
			pParty->nMaxHp	= nMaxHp;
			pParty->nMp		= nMp;
			pParty->nMaxMp	= nMaxMp;		
			
			//
			sprintf( szTemp, "PRO_PARTY%d_HP",  pParty->nSlotNum );
			SetProgressPos( szTemp, pParty->nMaxHp, pParty->nHp, TRUE );
			
			sprintf( szTemp, "PRO_PARTY%d_MP",  pParty->nSlotNum );
			SetProgressPos( szTemp, pParty->nMaxMp, pParty->nMp, TRUE );

			break;
		} //if

		itetList_Party++;
	} //while

//	RefreshPartyInfo();
} //CW3DUInterface::RefreshPartyMember

/**
 * 파티원 맵상 위치 갱신.
 */
void CW3DUInterface::RefreshPartyMember( int nId, WORD wMap, int nX, int nY )
{
	if( m_pMiniMap->GetMapIndex() == wMap )
	{
		sPartyMember*				pParty;
		int							nCnt = 0;
		LIST_PARTYMEMBER::iterator	itetList_Party = m_listPartyMember.begin();
		while( itetList_Party != m_listPartyMember.end() )
		{
			pParty = &(*itetList_Party);
			if( pParty->nId == nId )
			{
				m_pMiniMap->SetPoint( nX, nY, nCnt );
				m_pLargeMap->SetPoint( nX, nY, nCnt );

				break;
			}
			itetList_Party++;
			nCnt++;
		}
	}
} //CW3DUInterface::RefreshPartyMember

/**
 * 파티 힐.
 */
void CW3DUInterface::HealParty( const int nPartyNum )
{
	sPartyMember	*pParty;
	LIST_PARTYMEMBER::iterator	itetList_Party = m_listPartyMember.begin();
	advance( itetList_Party, nPartyNum );
	if( itetList_Party == m_listPartyMember.end() )
	{
		Assert( !"파티 넘이 이상타!" );
		return;
	} //if

	pParty = &(*itetList_Party);

	PushEvent( "Party", EVENT_PARTY_HEAL, pParty->nId, 0 );
} //CW3DUInterface::HealParty

/** 
 * 파티 게시판 리스트 추가.
 */
void CW3DUInterface::AddPartyBoardList(
	const char*	szName,			///< [IN] 캐릭터 이름.
	const int	nLevel,			///< [IN] 레벨.
	const int	nSSkill,		///< [IN] 근거리 스킬.
	const int	nLSkill,		///< [IN] 원거리 스킬.
	const int	nBSkill,		///< [IN] 흑마법 스킬.
	const int	nWSkill			///< [IN] 백마법 스킬.
	)
{
	if( !m_pPartyBoard )
	{
		return;
	} //if

	char*	pBuff = new char[6 * ONEDATALENGTH];		/// '6' 컬럼 개수
	if( !pBuff )
	{
		Assert( !"할당실패" );
		return;
	} //if

	if( strlen( szName ) > ONEDATALENGTH )
	{
		Assert( !"캐릭터 이름이 죈장!" );
		SAFE_DELETE_ARRAY( pBuff );
		return;
	} //if
	strcpy( pBuff, szName );

	sprintf( pBuff + ONEDATALENGTH,		"%d", nLevel );
	sprintf( pBuff + ONEDATALENGTH * 2, "%d", nSSkill );
	sprintf( pBuff + ONEDATALENGTH * 3, "%d", nLSkill );
	sprintf( pBuff + ONEDATALENGTH * 4, "%d", nBSkill );
	sprintf( pBuff + ONEDATALENGTH * 5, "%d", nWSkill );

	m_pPartyBoard->AddRow( pBuff );
} //CW3DUInterface::AddPartyBoardList

/**
 * Is My Friend
 */
BOOL CW3DUInterface::IsFriend( const char* szName )
{
	Assert( szName );

	UISelectString*		pList = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_CHATID_SAVE" ) );
	if( !pList )
	{
		Assert( !"Not found SelectString Component!!" );
		return FALSE;
	} //if

	if( !pList->FindString( szName ) )
	{
		return FALSE;
	} //if

	return TRUE;
}

/**
 * Add Friend
 */
void CW3DUInterface::AddFriend( const char* szFriendName )
{
	Assert( szFriendName );

	UISelectString*		pList = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_CHATID_SAVE" ) );
	if( !pList )
	{
		Assert( !"Not found SelectString Component!!" );
		return;
	} //if
	pList->AddString( szFriendName );
} //CW3DUInterface::AddFriend

/**
 * Delete Friend
 */
void CW3DUInterface::RemoveFriend( const char* szFriendName )
{
	Assert( szFriendName );

	UISelectString*		pList = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_CHATID_SAVE" ) );
	if( !pList )
	{
		Assert( !"Not found SelectString Component!!" );
		return;
	} //if
	pList->DeleteString( szFriendName );
} //CW3DUInterface::RemoveFriend

void CW3DUInterface::ClearFriendList()
{
	UISelectString*		pList = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_CHATID_SAVE" ) );
	if( !pList )
	{
		Assert( !"Not found SelectString Component!!" );
		return;
	} //if
	pList->Cleanup();	
}

/**
 * Clear Guild Member
 */
void CW3DUInterface::ClearGuildMember()
{
	UISelectString*		pList = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_GUILDMEMBERLIST_LIST" ) );
	if( !pList )
	{
		Assert( !"Not found SelectString Component!!" );
		return;
	} //if

	pList->Cleanup();
} //CW3DUInterface::ClearGuildMember

/**
 * Add Guild
 */
void CW3DUInterface::AddGuildMember( const char* szName, const BYTE btClass, const int nTotLev )
{
	Assert( szName );

	UISelectString*		pList = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_GUILDMEMBERLIST_LIST" ) );
	if( !pList )
	{
		Assert( !"Not found SelectString Component!!" );
		return;
	} //if

	char	szBuff[64];
	sprintf( szBuff, "%s , %d", szName, nTotLev );
	pList->AddString( szName );
} //CW3DUInterface::AddGuild

/**
 * Delete Guild
 */
void CW3DUInterface::RemoveGuildMember( const char* szName )
{
	Assert( szName );

	UISelectString*		pList = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_GUILDMEMBERLIST_LIST" ) );
	if( !pList )
	{
		Assert( !"Not found SelectString Component!!" );
		return;
	} //if
	pList->DeleteString( szName );
} //CW3DUInterface::RemoveGuildMember

/**
 * Friend Login or Logout
 */
void CW3DUInterface::FriendLogInOut( const char* szFriendName, BOOL bLogin )
{
	Assert( szFriendName );

	UISelectString*		pList = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_CHATID_SAVE" ) );
	if( !pList )
	{
		Assert( !"Not found SelectString Component!!" );
		return;
	} //if
	COLORTEXT*	pColorTxt = pList->FindString( szFriendName );
	if( !pColorTxt )
	{
		return;
	} //if
	
	if( bLogin )	pColorTxt->dwColor = 0xFFEBE800;
	else			pColorTxt->dwColor = 0xFFA4A4A4;//0xFFC0C0C0;
} //CW3DUInterface::FriendInOut

/**
 * Chatting with friend
 */
void CW3DUInterface::ChattingFriend( const char* szFriendName, const char* szMemo )
{
	UIStringList*	pChattList = reinterpret_cast<UIStringList*>( GetComponent( "LIST_MESSENGER" ) );
	if( !pChattList )
	{
		Assert( !"Not found StringList Component!!" );
		return;
	} //if

	UIDialog*	pMessengerDlg = reinterpret_cast<UIDialog*>( GetComponent( "DLG_MESSENGER" ) );
	if( !pMessengerDlg )
	{
		Assert( !"Not found MessengerDlg!!" );
		return;
	} //if

	if( !pMessengerDlg->IsShow() )
	{
		pMessengerDlg->SetTitleName( szFriendName );
		pChattList->Cleanup();
		pMessengerDlg->Show( TRUE );
	} //if

	string		strMessage = szFriendName;
	strMessage.append( " : " );	
	strMessage.append( szMemo );
	pChattList->SetText( strMessage.c_str() );
} //CW3DUInterface::ChattingFriend

/**
 * 지정한 귓속말 저장
 */
void CW3DUInterface::SaveWhisperName( const char* szName )
{
	Assert( szName );

	char*		pName = (char*)_MemAlloc( strlen( szName ) + 1 );
	if( !pName )
	{
		Assert( !"할당실패" );
		return;
	} //if
	strcpy( pName, szName );

	LIST_WHISPERNAME::iterator		iterName = m_listWhisperName.begin();
	while( iterName != m_listWhisperName.end() )
	{
		if( !strcmp( pName, (*iterName) ) )
		{
			SAFE_FREE( (*iterName) );
			m_listWhisperName.erase( iterName );
			break;
		} //if
		iterName++;
	} //while

	m_listWhisperName.push_front( pName );
	if( m_listWhisperName.size() > MAX_SAVEWHISPERNAME )
	{
		LIST_WHISPERNAME::iterator		iterName = iterName = m_listWhisperName.end();
		iterName--;
		SAFE_FREE( (*iterName) );
		m_listWhisperName.erase( iterName );

		Assert( m_listWhisperName.size() <= MAX_SAVEWHISPERNAME );
	} //if
} //CW3DUInterface::SaveWhisperName

/**
 * 귓속말 상대 이름 얻기
 */
char* CW3DUInterface::GetWhisperName()
{
	return m_szRecentWhisperName;
} //CW3DUInterface::GetWhisperName

/**
 * 귓속말 상대 설정
 */
void CW3DUInterface::SetWhisperName( const char* szName )
{
	strcpy( m_szRecentWhisperName, szName );
	SaveWhisperName( szName );

	UIEditText*		pedtChatt = reinterpret_cast<UIEditText*>( GetComponent( "EDT_CHATINPUT" ) );
	if( !pedtChatt )
	{
		Assert( !"Not Found The Component" );
		return;
	} //if

	char		szWhisperName[64];
	sprintf( szWhisperName, "%c%s ", CHAR_WHISPER, szName );

	pedtChatt->SetText( szWhisperName );
	EditBox_LDown( (UICComponent*)pedtChatt );
} //CW3DUInterface::SetWhisperName

/**
 * 저장한 문자열을 쳇창에 입력
 */
void CW3DUInterface::InputChattKeySaveString( const int nIndex )
{
	if( m_aszWordsSave[nIndex][0] == '\0' )
	{
		return;
	} //if

	UIEditText*		pedtChatt = reinterpret_cast<UIEditText*>( GetComponent( "EDT_CHATINPUT" ) );
	if( !pedtChatt )
	{
		Assert( !"Not Found The Component" );
		return;
	} //if

	g_ChatIME.PutString( m_aszWordsSave[nIndex] );
	pedtChatt->Reflesh();
} //CW3DUInterface::AddKeySaveString

/**
 * 쳇팅 내용 얻기
 */
char* CW3DUInterface::GetChattingText()
{
	return m_szRecentChattText;
} //CW3DUInterface::GetChattingText

/**
 * Add Player List
 */
void CW3DUInterface::AddPlayerList( const DWORD dwId, const char* szName, const BYTE btClass, const int nTotLev )
{
	UISelectString*			pSSComp = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_PLAYER_LIST" ) );
	if( !pSSComp )
	{
		Assert( "!Not Found the Component" );
		return;
	} //if

	MAP_PLAYERLIST::iterator	iter = m_mapPlayerList.find( dwId );
	if( iter != m_mapPlayerList.end() )
	{
		Assert( !"Already Exist the Player" );
		return;
	} //if

	m_mapPlayerList.insert( make_pair( dwId, (string)szName ) );

	char	szBuff[64];
	sprintf( szBuff, "%s , %d", szName, nTotLev );
	pSSComp->AddString( szBuff );
} //CW3DUInterface::AddPlayerList

/**
 * Remove Player List
 */
void CW3DUInterface::RemovePlayerList( const DWORD dwId, const char* szName )
{
	UISelectString*			pSSComp = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_PLAYER_LIST" ) );
	if( !pSSComp )
	{
		Assert( "!Not Found the Component" );
		return;
	} //if

	MAP_PLAYERLIST::iterator	iter = m_mapPlayerList.find( dwId );
	if( iter == m_mapPlayerList.end() )
	{
		Assert( !"Not Found Player" );
		return;
	} //if
	m_mapPlayerList.erase( dwId );

	pSSComp->DeleteString( szName );
} //CW3DUInterface::RemovePlayerList

/**
 * Request Party Invite By PlayerList
 */
void CW3DUInterface::PartyInvitByPlayerList()
{
	char*		szPlayer = (char*)GetText( "SSTR_PLAYER_LIST" ); 
	if( szPlayer )
	{
		MAP_PLAYERLIST::iterator	iter = m_mapPlayerList.begin();
		while( iter != m_mapPlayerList.end() )
		{
			string	sName = iter->second;
			if( !sName.compare( szPlayer ) )
			{
				DWORD		dwId = iter->first;
				PushEvent( "PARTY", EVENT_PARTY_INVIT, dwId, 0 );
				break;
			} //if
			iter++;
		} //while
	} //if
} //CW3DUInterface::PartyInvitByPlayerList

/**
 * Receive Request Invite
 */
void CW3DUInterface::RequestInviteGuild( WORD wGuildNo, int nMasterId, const char* szGuildName )
{
	m_nInviteGuildMasterId	= nMasterId;
	m_wInviteGuildNumber	= wGuildNo;	

	char		szText[64];
	sprintf( szText, "%s\n %s", szGuildName, STR_GUILD_INVITE );
	MessageBox( szText, "GUILDINVITE", MSG_YESNO );
}

/**
 * Request Talk To The Friend
 */
void CW3DUInterface::RequestTalkFriend()
{
	UISelectString*		pSSComp = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_CHATID_SAVE" ) );
	if( !pSSComp )
	{
		Assert( !"Not Found The SSComp" );
		return;
	} //if

	char*		pFriendName = pSSComp->GetText();
	if( !pFriendName )
	{
		Assert( !"Do Not Geting The Friend Name" );
		return;
	} //if

	UIDialog*	pMessengerDlg = reinterpret_cast<UIDialog*>( GetComponent( "DLG_MESSENGER" ) );
	if( !pMessengerDlg )
	{
		Assert( !"Not found MessengerDlg!!" );
		return;
	} //if

	UICComponent*	pEditComp = GetComponent( "EDT_MESSENSGER" );
	if( !pEditComp )
	{
		Assert( !"Not found The EditComp!!" );
		return;
	} //if

	pMessengerDlg->SetTitleName( pFriendName );
	pMessengerDlg->Show( TRUE );

	EditBox_LDown( pEditComp );
} //CW3DUInterface::RequestTalkFriend

/**
 *  전쟁정보 추가
 */
void CW3DUInterface::AddWarGuildInfo( 
	int nIndex,
	const char* szGuildName1, int nType1, int nPoint1,
	const char* szGuildName2, int nType2, int nPoint2
	)
{
	static int			s_nCount;
	char				szCompName1[32];
	char				szCompName2[32];

	UIProgress*			pGraph;
	UIRectangle*		pRectangle;

	if( nIndex == 0 )
	{
		s_nCount = 0;
	} //if

	sprintf( szCompName2, "RECT_WARGUILDINFO_GRAPH%d", nIndex+1 );
	pRectangle = reinterpret_cast<UIRectangle*>( GetComponent( szCompName2 ) );
	if( pRectangle )	pRectangle->Enable( FALSE );

	sprintf( szCompName1, "PRO_WARGUILDINFO_GRAPH%d", nIndex+1 );
	pGraph = reinterpret_cast<UIProgress*>( GetComponent( szCompName1 ) );
	if( !pGraph )
	{
		Assert( !"Not Found" );
		return;
	} //if
	pGraph->SetMinMax( 0, 100 );
	pGraph->SetPos( 0, FALSE );

	UIButton*	pButton1;
	UIButton*	pButton2;

	sprintf( szCompName1, "BTN_WARGUILDINFO_TEAM%d", s_nCount+1 );
	pButton1 = reinterpret_cast<UIButton*>( GetComponent( szCompName1 ) );
	if( !pButton1 )
	{
		Assert( !"Not Found" );
		return;
	} //if

	sprintf( szCompName2, "BTN_WARGUILDINFO_ENEMY%d", s_nCount+1 );
	pButton2 = reinterpret_cast<UIButton*>( GetComponent( szCompName2 ) );
	if( !pButton1 )
	{
		Assert( !"Not Found" );
		return;
	} //if

	pButton1->Load( NULL, NULL, NULL, NULL );
	pButton2->Load( NULL, NULL, NULL, NULL );

	if( !nType1 || !nType2 )
	{
		return;
	} //if

	sprintf( szCompName2, "RECT_WARGUILDINFO_GRAPH%d", s_nCount+1 );
	pRectangle = reinterpret_cast<UIRectangle*>( GetComponent( szCompName2 ) );
	if( pRectangle )	pRectangle->Enable( TRUE );

	sprintf( szCompName1, "BTN_WARGUILDINFO_TEAM%d", s_nCount+1 );
	pButton1 = reinterpret_cast<UIButton*>( GetComponent( szCompName1 ) );
	if( !pButton1 )
	{
		Assert( !"Not Found" );
		return;
	} //if

	sprintf( szCompName2, "BTN_WARGUILDINFO_ENEMY%d", s_nCount+1 );
	pButton2 = reinterpret_cast<UIButton*>( GetComponent( szCompName2 ) );
	if( !pButton1 )
	{
		Assert( !"Not Found" );
		return;
	} //if

	if( nType1 == 1 )			pButton1->Load( NULL, NULL, "Interface/war_bluetent.bmp", NULL );
	else if( nType1 == 2 )		pButton1->Load( NULL, NULL, "Interface/war_bluecastle.bmp", NULL );

	if( nType2 == 1 )			pButton2->Load( NULL, NULL, "Interface/war_redtent.bmp", NULL );
	else if( nType2 == 2 )		pButton2->Load( NULL, NULL, "Interface/war_redcastle.bmp", NULL );

	char		szTipInfo[64];

	sprintf( szTipInfo, "%s(%d)", szGuildName1, nPoint1 );
	pButton1->SetInfoText( szTipInfo );
	sprintf( szTipInfo, "%s(%d)", szGuildName2, nPoint2 );
	pButton2->SetInfoText( szTipInfo );

	sprintf( szCompName1, "PRO_WARGUILDINFO_GRAPH%d", s_nCount+1 );
	pGraph = reinterpret_cast<UIProgress*>( GetComponent( szCompName1 ) );
	if( !pGraph )
	{
		Assert( !"Not Found" );
		return;
	} //if
	
	int		nSum = nPoint1 + nPoint2;
	int		nAv  = nSum >> 1;

	if( nSum == 0 )
	{
		pGraph->SetMinMax( 0, 100 );
		pGraph->SetPos( 50, FALSE );
	}
	else
	{
		pGraph->SetMinMax( 0, nSum );
		pGraph->SetPos( nPoint1, FALSE );
	} //if.else

	s_nCount++;
} //CW3DUInterface::AddWarGuildInfo

/**
 * 성 정보  
 */
void CW3DUInterface::AddCastleList( int nIndex, int nMapNum )
{
	m_nCastlePage = 0;

	if( nIndex == 0 )
	{
		ZeroMemory( m_anCastleNum, sizeof( int ) * 20 );
	} //if

	char		szCompName[32];
	sprintf( szCompName, "BTN_CASTLELIST_MAP%d", nIndex+1 );
	UIButton*	pButton = reinterpret_cast<UIButton*>( GetComponent( szCompName ) );
	if( pButton )
	{
		char		szName1[32];
		char		szName2[32];
		sprintf( szName1, "Interface/war_map%02d.bmp", nMapNum );
		sprintf( szName2, "Interface/war_map%02d_2.bmp", nMapNum );

		pButton->Load( szName2, NULL, szName1, NULL );
		pButton->Enable( TRUE );
	} //if

	m_anCastleNum[nIndex] = nMapNum;
} //CW3DUInterface::AddCastleList
