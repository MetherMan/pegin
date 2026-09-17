  /**
 * 파티클 구현부.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-22 1:08a $
 * @version	$Revision: 70 $
 *
 * <HR>
 * $Log: /Render/Src/W3DParticles.cpp $
 * 
 * 70    03-12-22 1:08a Paran
 * 
 * 69    03-11-13 5:46p Paran
 * 
 * 68    03-10-27 3:37p Kjmgo
 * 많이 쓰이는 변수들을 Static으로 선언
 * 
 * 67    03-08-02 11:27a Paran
 * 
 * 66    03-08-02 10:48a Paran
 * 
 * 65    03-08-02 10:45a Paran
 * 
 * 64    03-07-30 3:42p Kjmgo
 * 
 * 63    03-07-28 6:44p Paran
 * 
 * 62    03-07-25 4:16p Paran
 * 
 * 61    03-07-25 12:08a Paran
 * 
 * 60    03-05-11 9:06p Paran
 * 
 * 59    03-04-16 1:36a Ahastudio
 *
 * 51    03-03-13 4:25p Ahastudio	파티클 로딩 부분 파일 제거에서 Wind3D 아카이브로 변경.
 * <HR>
 *
 * @file	W3DParticles.cpp
 */


#include "Render_Export.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include <string>
#include <vector>
#include <list>
#include <map>
#include <queue>


using namespace std;


const float MIN_SIZE			= 1.0F;
const int	MAX_PARTICLE		= 3000;


typedef map<UINT, string>				MAP_UINTSTR;


struct SParticleInfo
{
	UNDER_MEMORY_MGR( SParticleInfo );

	ParticleHead*	pHeader;			///<
	int				nRemaindLife;		///< 남은 생명줄
	Vector3			vPosStart;			///<
	Vector3			vPosCurrent;		///<
	Vector3			vDirection;			///<
	float			fSpeed;				///<
	DWORD			dwColor;			///<
	DWORD			dwColor_s;			///<
	float			fSize;				///<
	BOOL			bInView;			///<
}; //struct SParticleInfo

/**
 * 파티클 클래스.
 * @ingroup		Render
 */
class CW3DParticles : public IW3DParticles
{
public:
	UNDER_MEMORY_MGR( CW3DParticles )

	CW3DParticles();
	~CW3DParticles();

	BOOL	Init();
	void	Render( const DWORD dwTick );

	BOOL	AddParticle( const char* szType, const Vector3& vPos );
	UINT	GetCount();
	BOOL	LoadParticleTypeFile( const char* szFileName );

	int		AddTexture( const char* szTexName );
	BOOL	AddParticleType( const char* szType, const void* pHead );

private:
	struct VERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
		Vector3			p;
		float			tu, tv;
	}; //struct VERTEX

	typedef map<string, ParticleHead*>		ParticleHeadMap;
	typedef list<SParticleInfo*>			ParticleList;
	typedef vector<CTexture*>				TextureVec;
	typedef queue<SParticleInfo*>			ParticleMem;

	ParticleHeadMap		m_mapParticleHead;		///<
	ParticleList		m_listParticle;			///<
	CVertexBuffer*		m_pVB;					///<
	TextureVec			m_vecTexture;			///<
	DWORD				m_dwTick;				///<
	DWORD				m_dwIntervalTick;		///<
	ParticleMem			m_queMem;

	SParticleInfo		m_aParticle[MAX_PARTICLE];		///< 파티클 여유공간.
	void				InitParticleMemory();
	SParticleInfo*		AllocParticle();
	void				DeleteParticle( SParticleInfo* pParticleinfo );

	void				RemoveAll();
	void				Update( const DWORD dwTick );
	void				SetWorld( const Vector3& vPos, const float fW );
	void				SetRenderState( const BOOL bLight );
}; //class CW3DParticles


IMPLEMENT_COMPONENT( CW3DParticles, Particles );


/**
 * 생성자.
 */
CW3DParticles::CW3DParticles()
:	m_pVB			( NULL ),
	m_dwTick		( 0 ),
	m_dwIntervalTick( 0 )
{
} //CW3DParticles::CW3DParticles

/**
 * 소멸자.
 */
CW3DParticles::~CW3DParticles()
{
	RemoveAll();

	SAFE_DELETE( m_pVB );
} //CW3DParticles::~CW3DParticles

/**
 * 파티클을 초기화한다.
 */
BOOL CW3DParticles::Init()
{
	VERTEX			vertex[4];

	vertex[0].p.x		= -0.03F;
	vertex[0].p.y		=  0.03F;
	vertex[0].p.z		=  0.0F;
	vertex[0].tu		=  0.0F;
	vertex[0].tv		=  0.0F;

	vertex[1].p.x		=  0.03F;
	vertex[1].p.y		=  0.03F;
	vertex[1].p.z		=  0.0F;
	vertex[1].tu		=  1.0F;
	vertex[1].tv		=  0.0F;

	vertex[2].p.x		= -0.03F;
	vertex[2].p.y		= -0.03F;
	vertex[2].p.z		=  0.0F;
	vertex[2].tu		=  0.0F;
	vertex[2].tv		=  1.0F;

	vertex[3].p.x		=  0.03F;
	vertex[3].p.y		= -0.03F;
	vertex[3].p.z		=  0.0F;
	vertex[3].tu		=  1.0F;
	vertex[3].tv		=  1.0F;

	m_pVB = new CVertexBuffer( VERTEX::FVF, sizeof( VERTEX ) );
	if( !m_pVB )
	{
		Assert( !"Vertex Buffer 인스턴스 생성 실패" );
		return FALSE;
	} //if

	if( !m_pVB->Create( 4 ) )
	{
		Assert( !"Vertex Buffer 생성 실패" );
		return FALSE;
	} //if

	VERTEX*		pVert = (VERTEX*)m_pVB->Lock();
	if( !pVert )
	{
		SAFE_DELETE( m_pVB );
		return FALSE;
	} //if

	_MemCpy( pVert, vertex, sizeof( VERTEX[4] ) );

	m_pVB->Unlock();

	InitParticleMemory();

	return TRUE;
} //CW3DParticles::Init

/**
 * 파티클을 모두 삭제한다.
 */
void CW3DParticles::RemoveAll()
{
	m_listParticle.clear();

	vector<CTexture*>::iterator		iter_Texture = m_vecTexture.begin();

	while( iter_Texture != m_vecTexture.end() )
	{
		SAFE_RELEASE( *iter_Texture );
		iter_Texture = m_vecTexture.erase( iter_Texture );
	} //while

	ParticleHeadMap::iterator	itor = m_mapParticleHead.begin();
	ParticleHeadMap::iterator	itorNext;

	while( itor != m_mapParticleHead.end() )
	{
		itorNext = itor;
		itorNext++;

		ParticleHead*	pParticleHead = itor->second;
		SAFE_DELETE( pParticleHead );
		m_mapParticleHead.erase( itor );

		itor = itorNext;
	} //while
} //CW3DParticles::RemoveAll

void CW3DParticles::InitParticleMemory()
{
	memset( m_aParticle, 0, sizeof( SParticleInfo ) * MAX_PARTICLE );
	for( int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++ )
	{
		m_queMem.push( &m_aParticle[nCnt] );
	}
}

SParticleInfo* CW3DParticles::AllocParticle()
{
	Assert( m_queMem.size() == MAX_PARTICLE );

	SParticleInfo*		pParticle = m_queMem.front();
	m_queMem.pop();
	m_queMem.push( pParticle );
	return pParticle;
}

void CW3DParticles::DeleteParticle( SParticleInfo* pParticleinfo )
{
	if( m_queMem.size() >= MAX_PARTICLE )
	{
		Assert( !"이상타" );
	}
	m_queMem.push( pParticleinfo );
}

/**
 * 파티클 타입을 추가한다.
 */
BOOL CW3DParticles::AddParticleType( const char* szType, const void* pHead )
{
	Assert( szType );
	Assert( pHead );

	ParticleHeadMap::iterator		iter;

	iter = m_mapParticleHead.find( szType );
	if( iter != m_mapParticleHead.end() )
	{
		Assert( !"파티클 타입이 이미 존재함" );
		return FALSE;
	} //if

	m_mapParticleHead.insert( make_pair( szType, (ParticleHead*)pHead ) );

	return TRUE;
} //CW3DParticles::AddParticleType

/**
 * 파티클 데이터 파일에서 파티클 타입을 읽어온다.
 */
BOOL CW3DParticles::LoadParticleTypeFile( const char* szFileName )
{
	Assert( szFileName );

	FILE*	pFile = fopen( szFileName, "rb" );
	if( !pFile )
	{
		Assert( !"파일 열기 실패" );
		return FALSE;
	} //if

	ArchiveFileReader	ar( pFile );

	// File Info

	int			nVersion;
	char*		szFileinfo = new char[strlen( STR_FILEINFO ) + 1];

	ar.Serialize( szFileinfo, strlen( STR_FILEINFO ) + 1 );		// File Info
	if( strcmp( szFileinfo, STR_FILEINFO ) != 0 )
	{
		SAFE_DELETE( szFileinfo );
		ar.Close();
		Assert( !"모르는 파일" );
		return FALSE;
	} //if

	SAFE_DELETE( szFileinfo );

	ar.Serialize( &nVersion, sizeof( int ) );	// File Version
	if( nVersion != INT_FILEVERSION )
	{
		fclose( pFile );
		Assert( !"지금 파일 아님" );
		return FALSE;
	} //if

	// Particle Head

	ParticleHeadMap::iterator	iterMap_Head;

	int		nNumPaticleHeads;
	int		nIndex;
	char	szTexFileName[256];
	char	szName[125];
	float	fColor[3];

	ar.Serialize( &nNumPaticleHeads, sizeof( int ) );

	for( int i = 0 ; i < nNumPaticleHeads ; i++ )
	{
		ParticleHead*	pHeader = new ParticleHead;
		CHECK_PTR_RETURN( pHeader );

		_ZeroMemory( pHeader, sizeof( ParticleHead ) );

		ar.Serialize( szName, 125 );
		ar.Serialize( &pHeader->nNumber,		sizeof( int ) );	// 필요없는 정보( 그냥 읽어 넘김)
		ar.Serialize( &pHeader->nNumber,		sizeof( int ) );
		ar.Serialize( &pHeader->nTick,			sizeof( int ) );
		ar.Serialize( &pHeader->nLife,			sizeof( int ) );
		ar.Serialize( &pHeader->fSpeed,			sizeof( float ) );
		ar.Serialize( &pHeader->vAccelerate,	sizeof( Vector3 ) );
		ar.Serialize( &pHeader->fForce,			sizeof( float ) );
		ar.Serialize( &pHeader->bInverse,		sizeof( BOOL ) );
		ar.Serialize( &pHeader->nNumLevel,		sizeof( int ) );
		ar.Serialize( szTexFileName,			256 );

		nIndex = AddTexture( szTexFileName );
		pHeader->nTexIndex = nIndex;

		// RGB, size

		for( int j = 0 ; j < pHeader->nNumLevel ; j++ )
		{
			ar.Serialize( &fColor[0], sizeof( float ) );
			ar.Serialize( &fColor[1], sizeof( float ) );
			ar.Serialize( &fColor[2], sizeof( float ) );
			pHeader->btColor[0][j] = (BYTE)fColor[0];
			pHeader->btColor[1][j] = (BYTE)fColor[1];
			pHeader->btColor[2][j] = (BYTE)fColor[2];

			ar.Serialize( &fColor[0], sizeof( float ) );
			ar.Serialize( &fColor[1], sizeof( float ) );
			ar.Serialize( &fColor[2], sizeof( float ) );
			pHeader->btColor_s[0][j] = (BYTE)fColor[0];
			pHeader->btColor_s[1][j] = (BYTE)fColor[1];
			pHeader->btColor_s[2][j] = (BYTE)fColor[2];

			ar.Serialize( &pHeader->fSize[j], sizeof( float ) );		// 사이즈값을 처리
		} //for

		iterMap_Head = m_mapParticleHead.find( szName );
		if( iterMap_Head == m_mapParticleHead.end() )
		{
			m_mapParticleHead.insert( make_pair( (string)szName, (ParticleHead*)pHeader ) );
		}
		else
		{
			Assert( !"이미 있당" );
			SAFE_DELETE( pHeader );
		} //if..else..
	} //for

	ar.Close();

	return TRUE;
} //CW3DParticles::LoadParticleTypeFile

/**
 * 원하는 위치에 파티클을 추가한다.
 */
BOOL CW3DParticles::AddParticle( const char* szType, const Vector3& vPos )
{
	Assert( szType );

	ParticleHeadMap::iterator		itor;

	itor = m_mapParticleHead.find( szType );

	if( itor == m_mapParticleHead.end() )
	{
		DEBUG_OUT( "존재하지 않는 파티클 타입을 사용하려고 합니다 - " + szType );
		return FALSE;
	} //if

	int				nTps;
	ParticleHead*	pHeadParticle = itor->second;

	nTps = 1;
	if( m_dwIntervalTick < (DWORD)pHeadParticle->nTick )
	{
		nTps = 1;
	}
	else
	{
		nTps = (int)( (float)m_dwIntervalTick / (float)pHeadParticle->nTick ) + 1;
	} //if..else..

	int		nNumParticle    = pHeadParticle->nNumber;
	int		nLife			= pHeadParticle->nLife;
	int		nRemaindLife	= 0;	
//	for( int nCnt = 0 ; nCnt < nTps ; nCnt++ )
	{
		for( int nNum = 0; nNum < nNumParticle; nNum++ )
		{
			if( m_dwIntervalTick <= (DWORD)pHeadParticle->nTick )
			{
				nRemaindLife	= nLife;
			}
			else
			{
				nRemaindLife	= nLife - rand() % ( m_dwIntervalTick - pHeadParticle->nTick );
				if( nRemaindLife <= 0 )
				{
					continue;
				} //if
			} //if..else..

			SParticleInfo*	pParticle = AllocParticle();
			if( !pParticle )
			{
				Assert( !"메모리 할당 실패" );
				continue;
			} //if

			pParticle->nRemaindLife	= nRemaindLife;
			pParticle->pHeader		= pHeadParticle;
			pParticle->fSpeed		= pParticle->pHeader->fSpeed;
			pParticle->vPosStart    = vPos;
			pParticle->vDirection.x = float( ( rand() % 6 ) - 3.0F );
			pParticle->vDirection.y = float( ( rand() % 6 ) - 3.0F );
			pParticle->vDirection.z = float( ( rand() % 6 ) - 3.0F );

			m_listParticle.push_back( pParticle );
		} //for
	} //for

	return TRUE;
} //CW3DParticles::AddParticle

/**
 * 파티클의 갯수를 얻는다.
 */
UINT CW3DParticles::GetCount()
{
	return (UINT)m_listParticle.size();
} //CW3DParticles::GetCount

/**
 * 파티클에 사용될 텍스처를 추가한다.
 *
 * @return	현재 등록된 텍스처의 갯수. ( 에러 = 음수 )
 */
int CW3DParticles::AddTexture( const char *szTexName )
{
	Assert( szTexName );

	if( !strcmp( szTexName ,"" ) )
	{
		return -1;
	} //if
/*
	char		szPath[64];
	char		szFile[64];
	for( char* pCurr = (char*)szTexName + strlen( szTexName ); 
		 pCurr > szTexName;
		 pCurr-- )
	{
		if( *pCurr == '/' || *pCurr == '\\' )
		{
			strncpy( szPath, szTexName, pCurr - szTexName );
			szPath[pCurr-szTexName] = '\0';
			strcpy( szFile, pCurr+1 );
		} //if
	} //for
*/
	CTexture*	pTexture = g_pTextureMgr->LoadTexture( "", szTexName );
	if( !pTexture )
	{
		//Assert( !"텍스처 추가 실패" );
		DEBUG_OUT( "파티클 텍스쳐파일 없다" );
		return -1;
	} //if

	m_vecTexture.push_back( pTexture );

	return m_vecTexture.size() - 1;
} //CW3DParticles::AddTexture

/**
 * 파티클 갱신.
 */
void CW3DParticles::Update( const DWORD dwTick )
{
	ParticleList::iterator		iterCur;

	static SParticleInfo*	pParticle;
	static int		nLevelLife;
	static int		nCurTime;
	static int		nCurLevel;
	static int		nColorSubtract;
	static float	fSizeSubtract;
	static float	fRatio;
	static float	fSpeed;
	static float	fCurrentSize;
	static float	fNextSize;
	static BYTE		btColor[3];
	static BYTE		btColor_s[3];
	static Vector3	vDirection;
	static BOOL		bAttribChange;
	bAttribChange = FALSE;

	m_dwIntervalTick = dwTick - m_dwTick;

	iterCur = m_listParticle.begin();
	while( iterCur != m_listParticle.end() )
	{
		pParticle = *iterCur;
		pParticle->nRemaindLife -= m_dwIntervalTick;

		// Dead Particle
		if( pParticle->nRemaindLife <= 0 )
		{
			//DeleteParticle( pParticle );
			iterCur = m_listParticle.erase( iterCur );
			continue;
		} //if
		iterCur++;

		nCurTime   = pParticle->pHeader->nLife - pParticle->nRemaindLife;
		if( nCurTime < 0 )
		{
			nCurTime = 0;
		} //if
		nLevelLife = pParticle->pHeader->nLife /  ( pParticle->pHeader->nNumLevel - 1 );

		if( nLevelLife <= 0 )
		{
			nCurLevel  = 0;
		}
		else
		{
			nCurLevel  = nCurTime / nLevelLife;
		} //if..else..

		if( pParticle->pHeader->bInverse )
		{
			nCurTime   = pParticle->pHeader->nLife - nCurTime;
		} //if

		fSpeed = ( pParticle->fSpeed / 1000 ) * ( nCurTime );
		fSpeed += pParticle->pHeader->fForce * nCurTime;
		vDirection.x = pParticle->vDirection.x + ( pParticle->pHeader->vAccelerate.x * nCurTime );
		vDirection.y = pParticle->vDirection.y + ( pParticle->pHeader->vAccelerate.y * nCurTime );
		vDirection.z = pParticle->vDirection.z + ( pParticle->pHeader->vAccelerate.z * nCurTime );

		pParticle->vPosCurrent.x = pParticle->vPosStart.x + ( vDirection.x * fSpeed );
		pParticle->vPosCurrent.y = pParticle->vPosStart.y + ( vDirection.z * fSpeed );
		pParticle->vPosCurrent.z = pParticle->vPosStart.z + ( vDirection.y * fSpeed );

		pParticle->bInView = g_Frustum.CheckPoint( pParticle->vPosCurrent );

		//  Red
		fRatio = (float)( nCurTime % nLevelLife );
		nColorSubtract = ( pParticle->pHeader->btColor[0][nCurLevel+1] - 
						   pParticle->pHeader->btColor[0][nCurLevel] );
		btColor[0] = pParticle->pHeader->btColor[0][nCurLevel];
		if( nColorSubtract != 0 )
		{
			btColor[0] += (BYTE)( (float)nColorSubtract / (float)nLevelLife * fRatio );
		} //if

		nColorSubtract = ( pParticle->pHeader->btColor_s[0][nCurLevel+1] - 
						   pParticle->pHeader->btColor_s[0][nCurLevel] );
		btColor_s[0] = pParticle->pHeader->btColor_s[0][nCurLevel];
		if( nColorSubtract != 0 )
		{
			btColor_s[0] += (BYTE)( (float)nColorSubtract / (float)nLevelLife * fRatio );
		} //if

		//  Green
		nColorSubtract = ( pParticle->pHeader->btColor[1][nCurLevel+1] - 
						   pParticle->pHeader->btColor[1][nCurLevel] );
		btColor[1] = pParticle->pHeader->btColor[1][nCurLevel];
		if( nColorSubtract != 0 )
		{
			btColor[1] += (BYTE)( (float)nColorSubtract / (float)nLevelLife * fRatio );
		} //if

		nColorSubtract = ( pParticle->pHeader->btColor_s[1][nCurLevel+1] - 
						   pParticle->pHeader->btColor_s[1][nCurLevel] );
		btColor_s[1] = pParticle->pHeader->btColor_s[1][nCurLevel];
		if( nColorSubtract != 0 )
		{
			btColor_s[1] += (BYTE)( (float)nColorSubtract / (float)nLevelLife * fRatio );
		} //if

		//  Blue
		nColorSubtract = ( pParticle->pHeader->btColor[2][nCurLevel+1] - 
						   pParticle->pHeader->btColor[2][nCurLevel] );
		btColor[2] = pParticle->pHeader->btColor[2][nCurLevel];
		if( nColorSubtract != 0 )
		{
			btColor[2] += (BYTE)( (float)nColorSubtract / (float)nLevelLife * fRatio );
		} //if

		nColorSubtract = ( pParticle->pHeader->btColor_s[2][nCurLevel+1] - 
						   pParticle->pHeader->btColor_s[2][nCurLevel] );
		btColor_s[2] = pParticle->pHeader->btColor_s[2][nCurLevel];
		if( nColorSubtract != 0 )
		{
			btColor_s[2] += (BYTE)( (float)nColorSubtract / (float)nLevelLife * fRatio );
		} //if

		//  size
		fCurrentSize	 = pParticle->pHeader->fSize[nCurLevel];
		fNextSize		 = pParticle->pHeader->fSize[nCurLevel+1];
		fSizeSubtract	 = fNextSize - fCurrentSize;
		pParticle->fSize = fCurrentSize;
		if( fSizeSubtract != 0.0F )
		{
			pParticle->fSize += ( fSizeSubtract / (float)nLevelLife ) * fRatio;
		} //if
		if( pParticle->fSize > MIN_SIZE )
		{
			pParticle->fSize = 0.0F;
		} //if

		pParticle->dwColor	 = 0xFF000000 | ( btColor[0]   << 16 ) | ( btColor[1]   << 8 ) | btColor[2];
		pParticle->dwColor_s = 0xFF000000 | ( btColor_s[0] << 16 ) | ( btColor_s[1] << 8 ) | btColor_s[2];
	} //while

	m_dwTick = dwTick;
} //CW3DParticles::Update

/**
 * 월드 트랜스폼을 설정한다.
 */
void CW3DParticles::SetWorld( const Vector3& vPos, const float fW )
{
	static Matrix4	matWorld;
	matWorld = g_Frustum.m_matView.GetBillboard();

	matWorld._41 = vPos.x * fW;
	matWorld._42 = vPos.y * fW;
	matWorld._43 = vPos.z * fW;
	matWorld._44 *= fW;

	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );
} //CW3DParticles::SetWorld

/**
 * 모든 파티클을 움직이고 렌더링한다.
 *
 * @param	dwTick		[IN] 경과시간 (1000 Ticks = 1 Second)
 */
void CW3DParticles::Render( const DWORD dwTick )
{
	if( m_vecTexture.empty() )
	{
		return;
	} //if

	Update( dwTick );

	D3D::ZEnable( TRUE );
	D3D::ZWriteEnable( FALSE );
	D3D::Lighting( FALSE );
	D3D::AlphaBlend( TRUE );

	SetRenderState( FALSE );

	ParticleList::iterator		itor;
	SParticleInfo*	pParticle = NULL;

	for( itor = m_listParticle.begin(); itor != m_listParticle.end(); itor++ )
	{
		pParticle = *itor;
		if( !pParticle->bInView )
		{
			continue;
		} //if

		int nTexIndex = pParticle->pHeader->nTexIndex; 
		if( m_vecTexture[nTexIndex] )
		{
			m_vecTexture[nTexIndex]->SetTexture( 0 );
		} //if

		SetWorld( pParticle->vPosCurrent, pParticle->fSize );

		g_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, pParticle->dwColor_s );

		m_pVB->Draw( D3DPT_TRIANGLESTRIP, 2 );
	} //for

	SetRenderState( TRUE );

	for( itor = m_listParticle.begin(); itor != m_listParticle.end(); itor++ )
	{
		pParticle = *itor;

		if( !pParticle->bInView )
		{
			continue;
		} //if

		int		nTexIndex = pParticle->pHeader->nTexIndex; 

		if( m_vecTexture[nTexIndex] )
		{
			m_vecTexture[nTexIndex]->SetTexture( 0 );
		} //if

		SetWorld( pParticle->vPosCurrent, pParticle->fSize );

		g_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, pParticle->dwColor );

		m_pVB->Draw( D3DPT_TRIANGLESTRIP, 2 );
	} //for
} //CW3DParticles::Render

/**
 * 렌더링 상태 설정.
 *
 * @param	bLight		[IN] 밝아지는 파티클인지 여부. ( FALSE면 어두워진다 )
 */
void CW3DParticles::SetRenderState( const BOOL bLight )
{
	if( bLight )
	{
		g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
		g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		D3D::AlphaTest( FALSE );
	}
	else
	{
		g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
		g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		D3D::AlphaTest( TRUE );
	} //if..else..

	D3D::FogEnable( TRUE );

	// Texture 관련 세팅
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
} //CW3DParticles::SetRenderState
