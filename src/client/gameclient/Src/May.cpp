/**
 * 메이
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 30 $
 *
 * <HR>
 * $Log: /GameClient/Src/May.cpp $
 * 
 * 30    05-02-18 7:38p Sk8snow2
 * 
 * 29    04-08-23 8:40p Paran
 * 
 * 28    04-06-22 4:39p Paran2
 * 
 * 27    04-03-25 11:41a Paran2
 * 
 * 26    04-03-24 6:45p Paran2
 * 
 * 22    03-12-29 5:20p Kjmgo			레벨이 10이하일때만 메이가 나타나도록 수정
 * 
 * 21    03-12-18 10:04p Kjmgo			함축 단어, Map 추가
 * 
 * 14    03-12-17 12:01a Kjmgo			Command 추가( Command 追加 )
 * 
 * 13    03-12-11 2:40a Kjmgo			3단계로 분리 추가 
 * 
 * 12    03-12-10 11:52p Kjmgo			레벨을 3단계로 나눔
 * 
 * 11    03-12-06 12:01a Kjmgo			메이에게 말거는 코드 추가
 * 
 * 9     03-11-27 5:28p Kjmgo			임시 백업
 * 
 * 8     03-11-16 1:30a Kjmgo			May 관련 코드 추가
 * 
 * 6     03-11-12 8:36p Kjmgo			이동속도및, 대화 수정
 * 
 * 5     03-10-13 9:33a Kjmgo			Talk관련 함수 추가
 * 
 * 4     03-10-07 9:32p Kjmgo			빌보드 사용 수정
 * 
 * 3     03-10-07 8:08p Kjmgo			May그리기 순서 수정 + 캐릭터 말 탔을떄의 채팅 그리기 높이 수정
 * 
 * 2     03-10-06 9:38p Kjmgo			May(땡삐) 1단계 완성
 * 
 * <HR>
 *
 * @file	May.h
 */

#include "Global.h"
#include "May.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <map>

#define		SET_TALK( s )		sprintf( m_szTalk, "%s : %s", STR_MAY_MAY, s );

Vector3		GetRandomPos( Vector3 vPos );
void		GetSay( char* szRow, char* szSay );

/**
 *
 */
CMay::CMay()
:	m_pBillboard	( NULL ),
	m_pRectangle	( NULL )
{
	m_szTalk[0] = '\0';
	m_byType = MAY_TYPE_MAY;
} //CMay::CMay

/**
 *
 */
CMay::~CMay()
{
	SAFE_RELEASE( m_pBillboard );
	SAFE_RELEASE( m_pRectangle );
} //CMay::~CMay

/**
 * Set Info
 */
BOOL CMay::SetInfo( WORD wType, WORD wLevel, BOOL bPlayer )
{
	m_bPlayer	= bPlayer;
	m_wKind		= wType;
	m_wLevel	= GetLevel( wLevel );

	if( m_wLevel == 9999 )
		return FALSE;

	m_pModel = MAKE_COMPONENT( Model );
	if( !m_pModel->Load( "May", "co_0001.mod" ) 	||
		!m_pModel->LoadAnimation( "May", "co_0001.ani" ) )
	{
		Assert( !" asdf" );
		return FALSE;
	} //if

	LoadTalkScript( "May/Talk.txt" );
	SET_TALK( "" );
	
	m_pBillboard = MAKE_COMPONENT( Billboard );
	m_pBillboard->Load( "may.bbs" );

	m_bStart = FALSE;

	m_pRectangle = MAKE_COMPONENT( Rectangle );

	UpdateSay();

	return TRUE;
} //CMay::Init

/**
 *
 */
BOOL CMay::Update( DWORD dwTick )
{
	if( CHelper::Update( dwTick ) )
	{
		return TRUE;
	} //if

	if( m_bPlayer )
	{
		if( m_dwParticle > 500 )
		{
			m_pWorld->AddParticle( "메이", m_vPos );
			m_dwParticle = 0;
		} //if
		
		if( m_pBillboard )
			m_pBillboard->Update( dwTick );
	} //if

	return TRUE;
} //CMay:Update

/**
 * Draw
 */
void CMay::Draw()
{
	if( m_bGoAway )		return ;
	
	CHelper::Draw();
	
	if( m_pBillboard )
	{
		m_pBillboard->Draw( m_vPos + Vector3( 0.0F, 0.0F, 0.6F ) );
	} //if

	if( m_wKind < 10000 )
		DrawTalkText();
} //CMay::Draw

/**
 *
 */
void CMay::DrawTalkText()
{
/*	if( m_dwTalkTime < 2500 )
	{
		Vector3 m_vTextPos = m_pScene->GetScreenPos( m_vPos + Vector3( 0.0F, 0.0F, 1.2F ) );

		int		nX = m_vTextPos.x;
		int		nY = m_vTextPos.y;
		
		int		nTxtWidth, nTxtHeight;
		m_pScene->GetStringSize( nTxtWidth, nTxtHeight, m_szTalk );
		nX -= ( nTxtWidth >> 1 );

		//m_pScene->DrawText( nX, nY, 0xFF8AA9C9, m_szTalk );
		m_pRectangle->Draw( nX, nY, nTxtWidth, nTxtHeight, 0x4B000000 );
		m_pScene->DrawText( nX, nY, 0xFFFFFFFF, m_szTalk );
	}
	else */
	if( m_dwTalkTime >= 5000 )
	{
		if( m_bLetGoAway )
			m_bGoAway = TRUE;

		m_dwTalkTime = 2600;
		UpdateSay();
	} //if..else..

} //CMay::DrawTalkText

/**
 *
 */
void CMay::Say( char *buf, ... )
{
	m_dwTalkTime = 0;

	char tmp_buf[1024]; 

	va_list args; 
	va_start( args, buf ); 
	vsprintf( tmp_buf, buf, args ); 
	va_end( args ); 

	SET_TALK( tmp_buf );
	
	g_pGUI->ChattingInput( STR_MAY_MAY, tmp_buf, 0xFFA4A4A4 );
} //CMay::Say

/**
 *
 */
void CMay::UpdateSay()
{
	STalkState* pState = NULL;

	if( m_bStart )
	{
		pState = &m_aTalk[0][MAY_START];

		if( pState->nCount <= 0 )
		{
			return ; 
		} //if
	
		Say( pState->aTalk[m_nStage].szSay );
		
		m_nStage++;

		if( pState->nCount < m_nStage )
		{
			m_nStage = 0;
			m_bStart = FALSE;
		}
		else
		{
			m_dwTalkTime = 0;
		} //if..else..
		return ;
	}  //if

	WORD wState = TransformState();

	if( wState == MAY_STOP )
		pState = &m_aTalk[m_wLevel][wState];
	else
		pState = &m_aTalk[0][wState];

	if( pState->nCount <= 0 )
	{
		return ; 
	} //if
	
	int nRandom = random( 100 );

	if( nRandom <= 60 )
	{
		return ;
	} //if

	nRandom = random( pState->nCount + 1 );
	Say( pState->aTalk[nRandom].szSay );

	m_dwTalkTime = 0;

} //CMay::UpdatetSay

/**
 *
 */
void CMay::Talk( WORD wState )
{
	STalkState* pState = &m_aTalk[m_wLevel][wState];

	if( pState->nCount <= 0 )
	{
		return ; 
	} //if
	
	int nRandom = random( pState->nCount + 1 );
	Say( pState->aTalk[nRandom].szSay );

	m_dwTalkTime = 0;	
} //CMay::Talk

/**
 *
 */
void CMay::LoadTalkScript( const char* szFile )
{
	Assert( szFile );

	FILE* fp = fopen( szFile, "rt" );

	if( NULL == fp )
	{
		Assert( !"스크립트 파일이 없습니다." );
		return ;
	} //if


	char szBuffer[256];
	char szKeyword[80];

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( szKeyword[0] == NULL )
		{
			continue;
		} //if

		if( !strcmp( szKeyword, "[LEVEL]" ) )
		{
			int nType = 0;
			if( sscanf( szBuffer, "%s %d", szKeyword, &nType ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			while( !feof( fp ) )
			{
				fgets( szBuffer, 256, fp );
				*szKeyword = NULL;
				sscanf( szBuffer, "%s", szKeyword );

				if( szKeyword[0] == NULL )
				{
					continue;
				} //if

				if( !strcmp( szKeyword, "[/LEVEL]" ) )
				{
					break;
				} //if

				int nState = GetState( szKeyword );
				LoadTalkType( fp, nType, nState );

			} //while
		}
		else	if( !strcmp( szKeyword, "[DEFINEWORD]" ) )
		{
			LoadDefineSay( fp );
		}
		else	if( !strcmp( szKeyword, "[TALK]" ) )
		{

			while( !feof( fp ) )
			{
				//LoadTalkSay( fp );

				fgets( szBuffer, 256, fp );
				*szKeyword = NULL;
				sscanf( szBuffer, "%s", szKeyword );

				if( szKeyword[0] == NULL )
				{
					continue;
				} //if

				if( !strcmp( szKeyword, "[/TALK]" ) )
				{
					break;
				} //if

				if( strcmp( szKeyword, "[WORD]" ) )
				{
					continue;
				} //if


				char szWord[128];
				if( sscanf( szBuffer, "%s %s", szKeyword, szWord ) != 2 )
				{
					Assert( !"sscanf 오류" );
				}


				std::string sWord( szWord );

				int nId = m_aSay.size();
				m_mapSay.insert( std::make_pair( sWord, nId ) );
				
				int nCount = 0;

				SayVector	aSay;

				while( !feof( fp ) )
				{
					fgets( szBuffer, 256, fp );
					*szKeyword = NULL;
					sscanf( szBuffer, "%s", szKeyword );

					if( szKeyword[0] == NULL )
					{
						return ;
					} //if

					if( !strcmp( szKeyword, "[/WORD]" ) )
					{
						break;
					} //if	
					
					GetSay( szBuffer, szKeyword );
					aSay.push_back( std::string( szKeyword ) );

					nCount++;
				} //while
				
				m_aSay.push_back( aSay );
						
			} //while
		} //if..else..
	} //while


	fclose( fp );

} //CMay::LoadTalkScript

/**
 *
 */
void CMay::LoadTalkType( FILE* fp, int nLevel, int nType )
{
	Assert( fp );

	if( fp == NULL )
	{
		return ;
	} //if

	char szBuffer[256];
	char szKeyword[80];
	int  nNum = 0;

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( szKeyword[0] == NULL )
		{
			continue;
		} //if

		if( szKeyword[1] == '/' )
		{
			break;
		} //if

		STalkState* pState = NULL;

		if( nType == MAY_STOP )
		{
			pState = &m_aTalk[nLevel][nType];
		} 
		else
		{
			pState = &m_aTalk[0][nType];
		} //if..else..

		pState->nCount = nNum;
		pState->aTalk[nNum].dwTime = 0;
		
		GetSay( szBuffer, pState->aTalk[nNum].szSay );
		
		nNum++;
		
	} //while
			
} //CMay::LoadTalkType

/**
 *
 */
void CMay::LoadTalkSay( FILE* fp )
{
	Assert( fp );

	char szBuffer[256];
	char szKeyword[256];

	fgets( szBuffer, 256, fp );
	*szKeyword = NULL;

	sscanf( szBuffer, "%s", szKeyword );

	if( strcmp( szKeyword, "[WORD]" ) )
	{
		return ;
	} //if


	char szWord[128];
	if( sscanf( szBuffer, "%s %s", szKeyword, szWord ) != 2 )
	{
		Assert( !"sscanf 오류" );
	}


	std::string sWord( szWord );

	int nId = m_aSay.size();
	m_mapSay.insert( std::make_pair( sWord, nId ) );
	
	int nCount = 0;

	SayVector	aSay;

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( szKeyword[0] == NULL )
		{
			return ;
		} //if

		if( !strcmp( szKeyword, "[/WORD]" ) )
		{
			break;
		} //if	
		
		GetSay( szBuffer, szKeyword );
		aSay.push_back( std::string( szKeyword ) );

		nCount++;
	} //while
	
	m_aSay.push_back( aSay );
} //CMay::LoadTalkSay


/**
 *
 */
void CMay::LoadDefineSay( FILE* fp )
{
	Assert( fp );

	char szBuffer[256];
	char szKeyword[80];

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( szKeyword[0] == NULL )
		{
			continue;
		} //if

		if( !strcmp( szKeyword, "[/DEFINEWORD]" ) )
		{
			break;
		} //if

		if( strcmp( szKeyword, "[WORD]" ) )
		{
			continue;
		} //if

		char szWord[128];
		if( sscanf( szBuffer, "%s %s", szKeyword, szWord ) != 2 )
		{
			Assert( !"sscanf 오류" );
		}


		while( !feof( fp ) )
		{
			fgets( szBuffer, 256, fp );
			*szKeyword = NULL;
			sscanf( szBuffer, "%s", szKeyword );

			if( szKeyword[0] == NULL )
			{
				return ;
			} //if

			if( !strcmp( szKeyword, "[/WORD]" ) )
			{
				break;
			} //if	
			
			GetRowDefineSay( szWord, szBuffer );
		} //while
	} //while
} //CMay::LoadDefineSay


/**
 *
 */
int	CMay::GetState( char* szState )
{
	Assert( szState );

	if( !strcmp( szState, "[START]" ) )
	{
		return MAY_START;
	}
	else if( !strcmp( szState, "[ATTACK]" ) )
	{
		return MAY_ATTACK;
	}
	else if( !strcmp( szState, "[STOP]" ) )
	{
		return MAY_STOP;
	}
	else if( !strcmp( szState, "[MOVE]" ) )
	{
		return MAY_MOVE;
	}
	else if( !strcmp( szState, "[FOLLOW]" ) )
	{
		return MAY_FOLLOW;
	}
	else if( !strcmp( szState, "[DEAD]" ) )
	{
		return MAY_DEAD;
	}
	else if( !strcmp( szState, "[LEVELUP]" ) )
	{
		return MAY_LEVELUP;
	} 
	else if( !strcmp( szState, "[PARTY_CREATE]" ) )
	{
		return MAY_PARTY_CREATE;
	} 
	else if( !strcmp( szState, "[PARTY_JOIN]" ) )
	{
		return MAY_PARTY_JOIN_SUCCED;
	}	
	else if( !strcmp( szState, "[PARTY_FAIL]" ) )
	{
		return MAY_PARTY_JOIN_FAIL;
	} 
	else if( !strcmp( szState, "[PARTY_DESTORY]" ) )
	{
		return MAY_PARTY_DESTORY;
	} //if..else..else..else..
	/*#define		MAY_DEAL_BEGIN			8
#define		MAY_PARTY_CREATE		9
#define		MAY_PARTY_JOIN_FAIL		10
#define		MAY_PARTY_JOIN_SUCCED	11
#define		MAY_PARTY_DESTORY		12*/

	Assert( !"존재 하지 않는 상태 입니다." );
	return MAY_STOP;
} //CMay::GetState

/**
 *
 */
int	CMay::TransformState()
{
	if( !m_pCreature )
	{
		return STATE_STOP;
	} //if

	switch( m_pCreature->GetState() )
	{
	case STATE_MOVE:		return MAY_MOVE;	break;
	case STATE_STOP:		return MAY_STOP;	break;
//	case STATE_ATTACK:		return MAY_ATTACK;	break;
	case STATE_DEAD:		return MAY_DEAD;	break;
	}; //switch

	return MAY_STOP;
} //CMay::TransformState

/**
 *
 */
void CMay::Talk( const char* szTalk )
{
	if( szTalk[0] == NULL )
	{
		return ;
	} //if

	if( !CheckCommand( szTalk ) )
	{
		return ;
	} //if

	std::string str( GetDefineSay( szTalk ) );

	SayMap::iterator itor = m_mapSay.find( str );
	
	if( itor == m_mapSay.end() )
	{
		Say( "무슨 말인지 모르겠는데..." );
		return ;
	} //if

	DWORD dwId = itor->second;
	SayVector aSay = m_aSay[dwId];

	int nCount = aSay.size();

	if( nCount <= 0 )
	{
		return ;
	} //if

	int nRandom = random( nCount );
	Say( "%s", aSay[nRandom].c_str() );

} //CMay::Talk

/**
 *
 */
WORD CMay::GetLevel( WORD wLevel )
{
	if( wLevel >= 0 && wLevel <= 2 )			//if( wLevel >= 0 && wLevel <= 6 )	
		return 0;
	else if( wLevel >= 3 && wLevel <= 5 )		//else if( wLevel >= 7 && wLevel <= 14 )
		return 1;
	else if( wLevel >= 6 && wLevel <= 8 )		//else if( wLevel >= 15 && wLevel <= 19 )
		return 2;
	else
		return 9999;
} //CMay::GetLevel

/**
 *
 */
BOOL CMay::CheckCommand( const char* szSay )
{
	Assert( szSay );

	if( !strcmp( szSay, STR_MAY_GOAWAY1 ) ||
		!strcmp( szSay, STR_MAY_GOAWAY2 ) ||
		!strcmp( szSay, STR_MAY_GOAWAY3 ) ||
		!strcmp( szSay, STR_MAY_GOAWAY4 ) ||
		!strcmp( szSay, STR_MAY_GOAWAY5 ) ||
		!strcmp( szSay, STR_MAY_GOAWAY6 ) )
	{
		m_bLetGoAway = TRUE;

		Say( STR_MAY_GOODBYE );
		g_bShowMay = FALSE;
		//m_bGoAway = TRUE;
		return FALSE;
	} //if

	if( !strcmp( szSay, STR_MAY_SHOW1 ) ||
		!strcmp( szSay, STR_MAY_SHOW2 ) ||
		!strcmp( szSay, STR_MAY_SHOW3 ) )
	{
		m_bGoAway = FALSE;
		m_bLetGoAway = FALSE;
		Say( STR_MAY_SHOW );
		g_bShowMay = TRUE;
		return FALSE;
	} //if
	return TRUE; 
} //CMay::CheckCommand

/**
 *
 */
const char*	CMay::GetDefineSay( const char* szTalk )
{
	Assert( szTalk );

	DefineSay::iterator itor = m_mapDefineSay.find( std::string( szTalk ) );

	if( itor != m_mapDefineSay.end() )
	{
		return itor->second.c_str();
	} //if

	return szTalk;
						
} //CMay::GetDefineSay

/**
 *
 */
void CMay::GetRowDefineSay( const char* szKey, const char* szRow )
{
	Assert( szKey );
	Assert( szRow );

	char szSay[80];
	int nLen = strlen( szRow );

	for( int i = 0 ; i < nLen ; i++ )
	{
		if( szRow[i] == '\"' )
		{
			i++;
			for( int j = i ; j < nLen ; j++ )
			{				
				if( szRow[j] != '\"' )
				{
					szSay[j - i] = szRow[j];
					szSay[j - i + 1] = NULL;
				}
				else
				{
					break;
				} //if..else..
			} //for

			i = j;

			m_mapDefineSay.insert( std::make_pair(
								   std::string( szSay ),
								   std::string( szKey ) )
								   );			

		} //if
	} //for
} //CMay::GetRowDefineSay
/**
 *
 */
void GetSay( char* szRow, char* szSay )
{
	int nLen = strlen( szRow );

	for( int i = 0 ; i < nLen ; i++ )
	{
		if( szRow[i] == '\"' )
		{
			i++;
			for( int j = i ; j < nLen ; j++ )
			{				
				if( szRow[j] != '\"' )
				{
					szSay[j - i] = szRow[j];
					szSay[j - i + 1] = NULL;
				}
				else
				{
					break;
				} //if..else..
			} //for
			break;
		} //if
	} //for
} //GetSay