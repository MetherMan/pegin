/**
 * 메이
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 05-02-19 7:11p $
 * @version	$Revision: 29 $
 *
 * <HR>
 * $Log: /Engine/Src/DynamicNPC.cpp $
 * 
 * 29    05-02-19 7:11p Sk8snow2
 * 
 * 28    04-12-09 6:28p Sk8snow2
 * 
 * 27    04-08-27 8:01p Paran
 * 
 * 26    04-01-12 3:42p Kjmgo
 * 
 * 25    04-01-11 8:26p Kjmgo
 * 
 * 
 * <HR>
 *
 * @file	DynamicNPC.cpp
 */

#include "Engine_Export.h"
#include "DynamicNPC.h"

//-----------------------------
//	Global
//-----------------------------

namespace 
{
	IW3DTerrain* g_pTerrain = NULL;
	IW3DShadow*  g_pShadow = NULL;
} //namespace

void GetSay( char* szRow, char* szSay );

/**
 *
 */
CDynamicNPCMgr::CDynamicNPCMgr()
:	m_pScene		( NULL ),
	m_dwTime		( 0 )	
{	
	g_pShadow	= NULL;
} //CDynamicNPCMgr::CDynamicNPCMgr

/**
 *
 */
CDynamicNPCMgr::~CDynamicNPCMgr()
{
	NPCList::iterator itor = m_listNPC.begin();

	while( itor != m_listNPC.end() )
	{
		SAFE_DELETE( *itor );
		itor = m_listNPC.erase( itor );
	} //while

	SAFE_RELEASE( g_pShadow );
} //CDynamicNPCMgr::~CDynamicNPCMgr

/**
 *
 */
void CDynamicNPCMgr::Init( IW3DTerrain* pTerrain )
{
	Assert( pTerrain );

	m_pScene	= MAKE_COMPONENT( Scene );
	g_pShadow	= MAKE_COMPONENT( Shadow );
	g_pTerrain	= pTerrain;
	
	if( !g_pShadow->Init( 0 ) )
	{
		Assert( !"그림자 초기화 실패" );
		SAFE_RELEASE( g_pShadow );
	} //if	
} //CDynamicNPCMgr::Init

/**
 *
 */
BOOL CDynamicNPCMgr::Load( const char* szFile )
{
	Assert( szFile );

	CDynamicNPC* pNPC = new CDynamicNPC;

	if( !pNPC ||
		!pNPC->Load( szFile ) )
	{
		SAFE_DELETE( pNPC );
		Assert( !"NPC Script 읽기 오류" );
	}
	else
	{
		m_listNPC.push_back( pNPC );
	}//if

	return TRUE;
} //CDynamicNPCMgr::Load

/**
 *
 */
void CDynamicNPCMgr::Update( DWORD dwTick )
{
	static DWORD s_dwTick = 0;

	m_dwTime += dwTick; 
	s_dwTick += dwTick;

	if( s_dwTick < 10 )
	{
		return ;
	} //if

	s_dwTick = 0;

	static Vector3	vPos, vCameraPos;
	Frustum& frustum		= m_pScene->GetFrustum();
	vCameraPos				= frustum.m_vPos;

	CDynamicNPC*	pNPC	= NULL;
	BOOL			bShow	= FALSE;

	NPCList::iterator itor = m_listNPC.begin();
	m_listUpdateNPC.clear();

	while( itor != m_listNPC.end() )
	{
		pNPC = *itor;

		if( pNPC )
		{
			vPos = pNPC->GetPosition();
			vPos.z += 1.0F;

			if( frustum.CheckSphere( vPos, 1.0F ) )
			{
				// 보인다아
				bShow = TRUE;
				pNPC->Update( dwTick );
				m_listUpdateNPC.push_back( pNPC );
			}
			else
			{		
				if( ( vCameraPos - vPos ).Length() <= 200.0F )
				{
					pNPC->Update( dwTick );
				} //if

				// Not Update
				bShow = FALSE;
			} //if..else..

			pNPC->SetShow( bShow );
		} //if
			
		itor++;
	} //while
	
} //CDynamicNPCMgr::Update

/**
 *
 */
void CDynamicNPCMgr::Draw()
{
	if( m_listUpdateNPC.empty() )	return ;

	DWORD dwTick = m_dwTime;

	CDynamicNPC* pNPC = NULL;
	NPCList::iterator itor = m_listUpdateNPC.begin();
	while( itor != m_listUpdateNPC.end() )
	{
		pNPC = *itor;

		if( pNPC )
			pNPC->Draw( dwTick );
		
		itor++;
		dwTick += 800;
	} //while
	
} //CDynamicNPCMgr::Draw


/**
 *
 */
CDynamicNPC::CDynamicNPC()
:	m_vPos		( 0.0F, 0.0F, 0.0F ),
	m_bBack		( FALSE ),
	m_fSayHeight( 1.0F ),
	m_pScene	( NULL ),
	m_pModel	( NULL ),
	m_dwSayTick	( 0 ),
	m_nStart	( 0 )
{
	sprintf( m_szTalk, "" );
	m_qRot = Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion();
} //CDynamicNPC::CDynamicNPC

/**
 *
 */
CDynamicNPC::~CDynamicNPC()
{
	m_aSay.clear();	

	SoundList::iterator itor = m_aSound.begin();
	while( itor != m_aSound.end() )
	{
		SSound* sSound = *itor;

		SAFE_DELETE( sSound->pSound );
		SAFE_FREE( sSound );

		itor++;
	} //while

	m_aSound.clear();
	
	SAFE_RELEASE( m_pModel );
} //CDynamicNPC::~CDynamicNPC

/**
 *
 */
BOOL CDynamicNPC::Load( const char* szFile )
{
	Assert( szFile );

	char szPath[256];

	sprintf( szPath, "DynamicNPC/%s", szFile );

	FILE* fp = fopen( szPath, "rb" );

	if( NULL == fp )
	{
		return FALSE;
	} //if

	m_pScene = MAKE_COMPONENT( Scene );
	if( !m_pScene )
	{
		Assert( m_pScene );
		fclose( fp );
		return FALSE;
	} //if

	char szBuffer[256];
	char szKeyword[128];
	m_nStart = 0;

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( szKeyword[0] == NULL )
		{
			continue;
		} //if

		if( !strcmp( szKeyword, "End" ) )
		{
			break;
		}
		else if( !strcmp( szKeyword, "Model:" ) )
		{
			if( sscanf( szBuffer, "%s %s", szKeyword, szKeyword ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			m_pModel = MAKE_COMPONENT( Model );
			
			if( !m_pModel ||
				!m_pModel->Load( "NPC", szKeyword ) )
			{
				SAFE_RELEASE( m_pModel );
				
				fclose( fp );
				return FALSE;
			} //if
			
			char	szAnimation[256];
			strcpy( szAnimation, szKeyword );
			strcpy( szAnimation + strlen( szKeyword ) - 3, "ani" );

			m_pModel->CalculateRadius();
			m_pModel->LoadAnimation( "NPC", szAnimation );
		}
		else if( !strcmp( szKeyword, "Start:" ) )
		{
			if( sscanf( szBuffer, "%s %d", szKeyword, &m_nStart ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}
		}
		else if( !strcmp( szKeyword, "Speed:" ) )
		{
			if( sscanf( szBuffer, "%s %f", szKeyword, &m_fSpeed ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}
		}
		else if( !strcmp( szKeyword, "Height:" ) )
		{
			if( sscanf( szBuffer, "%s %f", szKeyword, &m_fSayHeight ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}
		}
		else if( !strcmp( szKeyword, "Say:" ) )
		{
			char szSay[256];
			GetSay( szBuffer, szSay );

			m_aSay.push_back( std::string( szSay ) );
		}
		else if( !strcmp( szKeyword, "Waypoint:" ) )
		{
			if( sscanf( szBuffer, "%s %s", szKeyword, szKeyword ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			LoadWaypoint( szKeyword );			
		} //if..else..
		else if( !strcmp( szKeyword, "Sound:" ) )
		{
			SSound* sSound = ( SSound* )_MemAlloc( sizeof( SSound ) );
			ZeroMemory( sSound, sizeof( SSound ) );

			sSound->pSound = MAKE_COMPONENT( SoundObject3D );			

			if( LoadSound( szBuffer, sSound ) )
			{
				m_aSound.push_back( sSound );
			}		
			else
			{
				SAFE_DELETE( sSound->pSound );
				SAFE_FREE( sSound );
			} //if..else..
		} //if..else..
	} //while

	fclose( fp );
	
	Assert( m_nStart >= 0 );

	m_nStart = MAX( 0 , m_nStart );
	m_vPos = m_aPath[m_nStart];
	m_vGoal = m_vPos;
	m_nNow = m_nStart;
	m_nTalk = m_aSay.size();

	return TRUE;
} //CDynamicNPC::Load

/**
 *
 */
BOOL CDynamicNPC::LoadWaypoint( const char* szFile )
{
	Assert( szFile );

	char szPath[256];
	sprintf( szPath, "DynamicNPC/Waypoint/%s", szFile );

	FILE* fp = fopen( szPath, "rb" );
	
	if( NULL == fp )
	{
		return FALSE;
	} //if

	char szBuffer[256];
	fgets( szBuffer, 256, fp );

	int nCount = 0;

	if( sscanf( szBuffer, "%d", &nCount ) != 1 )
	{
		Assert( !"sscanf 오류" );
	}

	m_aPath.clear();

	for( int i = 0 ; i < nCount ; i++ )
	{
		int nId = 0;
		Vector3 vPos;
		fgets( szBuffer, 256, fp );
		if( sscanf( szBuffer, "%d %f %f", &nId, &vPos.x, &vPos.y ) != 3 )
		{
			Assert( !"sscanf 오류" );
		}

		vPos.z = g_pTerrain->GetHeight( vPos.x, vPos.y );
		m_aPath.push_back( vPos );
	} //for

	fclose( fp );

	return TRUE;
} //CDynamicNPC::LoadWaypoint

/**
 *
 */
BOOL CDynamicNPC::LoadSound( char* szBuffer, SSound* sSound )
{
	Assert( szBuffer && sSound );	
	
	char szKeyword[40] = "\0";
	char szLimit[10] = "\0";
	char szDistance[15] = "\0";
		
	if( sscanf( szBuffer, "%s %s %s %s", szKeyword, sSound->szWave, szLimit, szDistance ) == 4 )
	{
		sSound->dwLimit = atoi( szLimit );
		sSound->nDistance = atoi( szDistance );
		return TRUE;
	}
	else
	{
		Assert( !"sscanf 오류" );
		return FALSE;
	}	

} // CDynamicNPC::LoadSound

/**
 *
 */
void CDynamicNPC::Update( const DWORD dwTick )
{
	m_dwSayTick += dwTick;
	if( m_vPos == m_vGoal )
	{
		if( m_bBack )
			m_nNow--;
		else
			m_nNow++;

		if( m_nNow >= (int)m_aPath.size() )
		{
			m_bBack = TRUE;
			m_nNow = (int)m_aPath.size() - 1;
		}
		else if( m_nNow < 0 )
		{
			m_bBack = FALSE;
			m_nNow = 1;
		} //if..else..
				
		m_vGoal = m_aPath[m_nNow];
	} //if

	Vector3 vDir = m_vGoal - m_vPos;

	float		fOffset = m_fSpeed * dwTick / 1000;
	float		fLength = vDir.Length();
	vDir.Normalize();

	if( fOffset > fLength )
	{
		fOffset = fLength;
	} //if

	Vector3 vPos = m_vPos + vDir * fOffset;

	
	vPos.z = g_pTerrain->GetHeight( vPos.x, vPos.y );
	float	fZ = vPos.z - m_vPos.z;

	Vector3	vDistance = vPos - m_vPos;
	vDistance.z = 0.0F;

	float		fDistance = vDistance.Length();

	if( fDistance > MATH_EPSILON || fDistance > 3.0F )
	{
		m_vPos = vPos;
		m_qRot = vDir.GetQuaternion();
	} //if

	// 사운드 재생
	SoundList::iterator itor = m_aSound.begin();
	while( itor != m_aSound.end() )
	{
		SSound* sSound = *itor;

		if( sSound )
		{
			if( sSound->dwLimit <= sSound->dwTime )
			{
				if( sSound->pSound )
				{
					sSound->pSound->SetPosition( m_vPos );
					sSound->pSound->Play3D( sSound->szWave, 1, sSound->nDistance );
				} //if

				sSound->dwTime = 0;
			}
			else
			{
				sSound->dwTime += dwTick;
			} //if..else..
		} //if

		itor++;
	} //while	

} //CDynamicNPC::Update

/**
 *
 */
void CDynamicNPC::UpdateSay()
{
	if( m_nTalk <= 0 )
	{
		return ;
	} //if

	// 20%의 확률로 말을 한다.

	int nRandom = random( 100 );

	if( nRandom <= 30 )
	{
		return ;
	} //if

	nRandom = random( m_nTalk );

	Assert( nRandom < m_aSay.size() );

	strcpy( m_szTalk, m_aSay[nRandom].c_str() );

	m_dwSayTick = 0;

} //CDynamicNPC::UpdateSay

/**
 *
*/ 
void CDynamicNPC::Draw( const DWORD dwTime )
{
	if( m_pModel )
	{
		m_pModel->SetWorld( m_vPos, m_qRot );
		m_pModel->Render( dwTime, TRUE );

		if( g_pShadow )
		{
			Matrix4		matWorld;
			matWorld.SetWorld( m_vPos + Vector3( 0.0F, 0.0F, 0.1F ), m_qRot );
			g_pShadow->Render( matWorld );
		} //if
	} //if

	DrawSay();
	//m_pModel->
} //CDynamicNPC::Draw


/**
 *
 */
void CDynamicNPC::DrawSay()
{
/*	if( m_dwSayTick < 3000 && m_pScene )
	{
		if( ( m_pScene->GetFrustum().m_vPos - m_vPos ).Length() >= 40.0F )
		{
			return ;
		} //if

		Vector3 m_vTextPos = m_pScene->GetScreenPos( m_vPos + Vector3( 0.0F, 0.0F, m_fSayHeight ) );

		int		nX = m_vTextPos.x;
		int		nY = m_vTextPos.y;
		
		int		nTxtWidth, nTxtHeight;
		m_pScene->GetStringSize( nTxtWidth, nTxtHeight, m_szTalk );
		nX -= ( nTxtWidth >> 1 );

		m_pScene->DrawText( nX, nY, 0xFF8AA9C9, m_szTalk );
		//m_pRectangle->Draw( nX, nY, nTxtWidth, nTxtHeight, 0x4B000000 );
		m_pScene->DrawText( nX, nY, 0xFFFFFFFF, m_szTalk );
	}
	else */if( m_dwSayTick >= 5000 )
	{
		m_dwSayTick = 2600;
		UpdateSay();
	} //if..else..
} //CDynamicNPC

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