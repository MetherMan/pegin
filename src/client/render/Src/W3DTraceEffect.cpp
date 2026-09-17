/**
 * 흔적 효과.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-08 6:18p $
 * @version	$Revision: 45 $
 *
 * <HR>
 * $Log: /Render/Src/W3DTraceEffect.cpp $
 * 
 * 45    04-01-08 6:18p Kjmgo
 * 
 * 44    03-12-14 2:35p Kjmgo
 * 
 * 43    03-11-13 5:46p Paran
 * 
 * 42    03-10-27 3:37p Kjmgo
 * 많이 쓰이는 변수들을 Static으로 선언
 * 
 * 41    03-10-15 11:16a Kjmgo
 * 
 * 40    03-10-10 3:45a Kjmgo
 * Trace정보를 Load할떄, Texture없을시 바로 종료하던것을, 그냥 Skip으로
 * 수정..또한, 생성 실패시 그냥 종료하여 메모리 릭 발생하던것 수정
 * 
 * 39    03-08-08 2:20p Admin
 * 
 * 38    03-07-26 11:24a Kjmgo
 * 
 * 37    03-05-14 2:07a Kjmgo
 * 
 * 36    03-04-03 5:51a Kjmgo
 * 
 * 35    03-03-26 7:11a Kjmgo
 * 
 * 34    03-03-13 5:24p Ahastudio
 * signed/unsigned 구분.
 * 33    03-03-13 5:09p Kjmgo		변수명 정리
 * 32    03-03-12 3:21p Ahastudio	Texture 부분 수정 및 주석 정리.
 * <HR>
 *
 * @file	W3DTraceEffect.cpp
 */


#include "Render_Export.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include <stdio.h>
#include <string>
#include <map>


using namespace std;


struct	TraceType;
struct	VERTEX;
class	CTraceEffect;


enum
{
	TRACE_VERSION	= 100,
	TRACE_MAX		= 100
};


/**
 * 검흔 효과 클래스 구현부.
 *
 * @author	로냐
 * @date	2003-01-02 오후 7:22
 */
class CW3DTraceEffect : public IW3DTraceEffect
{
public:
	UNDER_MEMORY_MGR( W3DTraceEffect )

	CW3DTraceEffect();
	~CW3DTraceEffect();

	BOOL Init();
	BOOL Load( const char* szFileName );

	void Update( DWORD dwTick );
	void Render();

	BOOL CreateEffect( const char* szTypeName, const DWORD dwKey );
	BOOL ChangeType	( const DWORD dwKey, const char* szTypeName );
	void PauseEffect( const DWORD dwKey );
	void DeleteEffect( const DWORD dwKey );
	void InsertPoint( const DWORD dwKey, const Vector3& vStart, const Vector3& vEnd );

private:
	typedef map< string, TraceType* >		TraceTypeMap;
	typedef TraceTypeMap::iterator			TraceTypeItor;

	typedef map< DWORD, CTraceEffect* >		TraceEffectMap;
	typedef TraceEffectMap::iterator		TraceEffectItor;

	TraceTypeMap		m_mapType;			///<
	TraceEffectMap		m_mapEffect;		///<

	void InitTrace( const DWORD dwKey );
	void RemoveAllEffect();
	void RemoveAllType();
	void AddType( char* szTypeName, TraceType* pType );
};


IMPLEMENT_COMPONENT( CW3DTraceEffect, TraceEffect );


struct TraceType
{
	UNDER_MEMORY_MGR( TraceType )

	CTexture*		pTexture;
	char			szTypeName[256];
	int				nLevel;
	int				nRate;
};


/**
 * 검흔 효과.
 *
 * @author	로냐
 * @date	2003-01-02 오후 7:22
 */
class CTraceEffect
{
public:
	UNDER_MEMORY_MGR( CTraceEffect )

	int				m_nLevel;
	DWORD			m_dwTick;
	int				m_nFrame;
	TraceType*		m_pType;

	int				m_nNumFaces;
	int				m_nNumVertices;


	CTraceEffect();
	~CTraceEffect();

	BOOL Init();
	void InitKey();
	void Cleanup();

	void Pause();
	void Render();
	BOOL Update( DWORD dwTick );

	void InsertPoint( const Vector3& vP0, const Vector3& vP1 );

private:
	struct VERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
		Vector3		p;
		DWORD		color;
		float		tu, tv;
	};

	LPDIRECT3DVERTEXBUFFER8		m_pVB;
	VERTEX*						m_pVertices;

	int			m_nColor;
	int			m_nStartFace;
	bool		m_bRate;
	bool		m_bUse;
};


/**
 * 생성자.
 */
CW3DTraceEffect::CW3DTraceEffect()
{
} //CW3DTraceEffect::CW3DTraceEffect

/**
 * 소멸자.
 */
CW3DTraceEffect::~CW3DTraceEffect()
{
	RemoveAllEffect();
	RemoveAllType();
} //CW3DTraceEffect::~CW3DTraceEffect

/**
 * 초기화.
 */
BOOL CW3DTraceEffect::Init()
{
	return TRUE;
} //CW3DTraceEffect::Init

/**
 * ...
 *
 * @param	szKey		[IN] ...
 */
void CW3DTraceEffect::InitTrace( const DWORD dwKey )
{
	TraceEffectItor		itor = m_mapEffect.find( dwKey );

	if( itor != m_mapEffect.end() )
	{
		CTraceEffect*	pEffect = itor->second;
		Assert( pEffect );
		pEffect->InitKey();
	} //if
} //CW3DTraceEffect::InitTrace

/**
 * 타입 정보가 담긴 파일을 읽어온다.
 *
 * @param	szFileName		[IN] 타입 정보가 담긴 파일 이름.
 */
BOOL CW3DTraceEffect::Load( const char* szFileName )
{
	FILE*	fp = fopen( szFileName, "rb" );

	if( NULL == fp )
	{
		Assert( !"파일을 열수 없습니다." );
		return FALSE;
	} //if

	RemoveAllType();

	int nVersion = 0;

	fread( &nVersion, 4, 1, fp );

	if( nVersion != TRACE_VERSION )
	{
		Assert( !"Version이 맞지 않습니다." );
		fclose( fp );
		return FALSE;
	} //if

	int nCount = 0;

	fread( &nCount, 4, 1, fp );

	for( int i = 0 ; i < nCount ; i++ )
	{
		int nLen = 0;

		TraceType* pType = new TraceType;


		char* szTypeName;

		// Type Name
		fread( &nLen, 4, 1, fp );
		fread( pType->szTypeName, nLen, 1, fp );

		szTypeName = pType->szTypeName;

		// Texture

		char szTextureName[ 256 ];

		fread( &nLen, 4, 1, fp );
		fread( szTextureName, nLen, 1, fp );

		pType->pTexture = NULL;

		pType->pTexture = g_pTextureMgr->LoadTexture( "Texture/Effect/", szTextureName );

		if( NULL == pType->pTexture )
		{
			Assert( !"텍스처가 없대 된장~" );
			SAFE_DELETE( pType );
			continue;
		} //if

		// Level

		fread( &pType->nLevel, 4, 1, fp );

		if( pType->nLevel >= TRACE_MAX )
		{
			pType->nLevel = TRACE_MAX - 1;
		} //if

		fread( &pType->nRate, 4, 1, fp );

		m_mapType.insert( make_pair( szTypeName, pType ) );
	} //for

	fclose( fp );

	return TRUE;
} //CW3DTraceEffect::Load

/**
 * 흔적 효과를 업데이트한다.
 *
 * @param	dwTick		[IN] 경과 tick.
 */
void CW3DTraceEffect::Update( DWORD dwTick )
{
	if( m_mapEffect.empty() )
	{
		return ;
	} //if

	TraceEffectItor	itor = m_mapEffect.begin(); 
	CTraceEffect*	pEffect = NULL;

	while( itor != m_mapEffect.end() )
	{
		pEffect = itor->second;
		
		if( pEffect )
		{
			pEffect->Update( dwTick );
		} //if

		itor++;
	} //while
} //CW3DTraceEffect::Update

/**
 * 흔적 효과를 그려준다.
 *
 * @author	로냐
 * @date	2003-01-02 오후 7:22
 */
void CW3DTraceEffect::Render()
{
	static Matrix4	matWorld;
	matWorld.SetIdentity();
	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );

	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	// Set Render State

	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

	// Texture 관련 세팅

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );

	// Alpha Blend 관련 세팅

	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// Alpha Test 관련 세팅

	g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );

	// Light 관련 세팅

	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );

	g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

	// Node Render

	TraceEffectItor	itor = m_mapEffect.begin();
	CTraceEffect* pEffect = NULL;

	while( itor != m_mapEffect.end() )
	{
		pEffect = itor->second;
		
		// Level 0 이하는 처리 할 필요가 없음
		if( pEffect && pEffect->m_nLevel > 0 )
		{
			Assert( pEffect->m_pType );
			Assert( pEffect->m_pType->pTexture );

			CTexture*	pTexture = pEffect->m_pType->pTexture;

			if( pTexture )
			{
				pTexture->SetTexture( 0 );
			} //if

			pEffect->Render();
		} //if

		itor++;
	} //while

	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );                 
} //CW3DTraceEffect::Render

/**
 * 모든 흔적 효과를 제거한다.
 *
 * @author	로냐
 * @date	2003-01-02 오후 7:22
 */
void CW3DTraceEffect::RemoveAllEffect()
{
	TraceEffectItor		itor = m_mapEffect.begin();
	TraceEffectItor		itorNext;

	while( itor != m_mapEffect.end() )
	{
		itorNext = itor;
		itorNext++;

		CTraceEffect*	pEffect = itor->second;
		SAFE_DELETE( pEffect );
		m_mapEffect.erase( itor );

		itor = itorNext;
	} //while
} //CW3DTraceEffect::RemoveAllEffect

/**
 * 모든 흔적 효과 타입 정보를 없앤다.
 */
void CW3DTraceEffect::RemoveAllType()
{
	TraceTypeItor	itor = m_mapType.begin();
	TraceTypeItor	itorNext;

	while( itor != m_mapType.end() )
	{
		itorNext = itor;
		itorNext++;

		TraceType*	pType = itor->second;
		SAFE_RELEASE( pType->pTexture );
		SAFE_DELETE( pType );

		m_mapType.erase( itor );

		itor = itorNext;
	} //while
} //CW3DTraceEffect::RemoveAllType

//
// CW3DTraceEffect::CreateEffect
//
BOOL CW3DTraceEffect::CreateEffect( const char* szTypeName_, const DWORD dwKey_ )
{
	//- Find Effect

	TraceEffectItor		itorEffect = m_mapEffect.find( dwKey_ );
	if( itorEffect != m_mapEffect.end() )
	{
		Assert( !"이미 같은 이름의 Trace Effect가 있습니다." );
		return FALSE;
	} //if

	//-
	//- Find Type

	TraceTypeItor	itorType = m_mapType.find( szTypeName_ );
	if( itorType == m_mapType.end() )
	{
		Assert( !"Type이 존재 하지 않습니다." );
		return FALSE;
	} //if

	//-

	// Create Effect

	CTraceEffect*	pEffect = new CTraceEffect;
	if( !pEffect ||
		!pEffect->Init() )
	{
		Assert( !"Trace Effect 쪼가리 생성 실패" );
		return FALSE;
	} //if

	pEffect->m_pType  = itorType->second;
	pEffect->m_nLevel = 0;

	m_mapEffect.insert( make_pair( dwKey_, pEffect ) );

	return TRUE;
} //CW3DTraceEffect::CreateEffect

//
// CW3DTraceEffect::DeleteEffect
//
void CW3DTraceEffect::DeleteEffect( const DWORD dwKey_ )
{
	TraceEffectItor			itor = m_mapEffect.find( dwKey_ );

	if( itor != m_mapEffect.end() )
	{
		TraceEffectItor		itorNext = itor;
		itorNext++;

		CTraceEffect*	pEffect = itor->second;
		SAFE_DELETE( pEffect );
		m_mapEffect.erase( itor );

		itor = itorNext;
	}
	else
	{
		Assert( !"존재 하지 않는 Trace Effect를 삭제 하려 했습니다." );
	}//if	
} //CW3DTraceEffect::DeleteEffect

//
// CW3DTraceEffect::ChangeType
//
BOOL CW3DTraceEffect::ChangeType( const DWORD dwKey_, const char* szTypeName_ )
{
	//- Find Type

	TraceTypeItor	itorType = m_mapType.find( szTypeName_ );

	if( itorType == m_mapType.end() )
	{
	//	Assert( !"Type이 존재 하지 않습니다." );
		return FALSE;
	} //if

	//-

	TraceEffectItor		itorEffect = m_mapEffect.find( dwKey_ );

	if( itorEffect == m_mapEffect.end() )
	{
	//	Assert( !"Effect가 존재 하지 않습니다." );
		return FALSE;
	} //if

	CTraceEffect* pEffect = itorEffect->second;
	Assert( pEffect );

	pEffect->m_pType = itorType->second;

	pEffect->m_nLevel		= 0;
	pEffect->m_nNumVertices	= 0;
	pEffect->m_nNumFaces	= 0;

	Assert( pEffect->m_pType->pTexture );

	return TRUE;
} //CW3DTraceEffect::ChangeType

//
// CW3DTraceEffect::InsertPoint
//
void CW3DTraceEffect::InsertPoint( const DWORD dwKey_, const Vector3& vStart_, const Vector3& vEnd_ )
{
	TraceEffectItor		itor = m_mapEffect.find( dwKey_ );
	if( itor != m_mapEffect.end() )
	{
		CTraceEffect*	pEffect = itor->second;
		Assert( pEffect );

		if( pEffect )
		{
			pEffect->InsertPoint( vStart_, vEnd_ );
		} //if
	} //if
} //CW3DTraceEffect::InsertPoint

//
// CW3DTraceEffect::PauseEffect
//
void CW3DTraceEffect::PauseEffect( const DWORD dwKey )
{
	TraceEffectItor		itor = m_mapEffect.find( dwKey );

	if( itor != m_mapEffect.end() )
	{
		CTraceEffect*	pEffect = itor->second;
		Assert( pEffect );
		if( pEffect )
		{
			pEffect->InitKey();
		} //if
	} //if
} //CW3DTraceEffect::PauseEffect

//////////////////////////////////////////////////////////////////////////
// CTraceEffect


//
// CTraceEffect::CTraceEffect
//
CTraceEffect::CTraceEffect()
{
	m_pVB			= NULL;
	m_pVertices     = NULL;

	m_nNumFaces		= 0;
	m_nNumVertices	= 0;
	m_nLevel		= 0;
	m_dwTick		= 0;
	m_nFrame		= 0;
	m_nStartFace	= 0;
	m_pType			= NULL;
	m_nColor		= 255;
	m_bRate			= FALSE;
	m_bUse			= FALSE;
} //CTraceEffect::CTraceEffect

//
// CTraceEffect::~CTraceEffect
//
CTraceEffect::~CTraceEffect()
{
	SAFE_FREE( m_pVertices );
	SAFE_RELEASE( m_pVB );
} //CTraceEffect::~CTraceEffect

//
// CTraceEffect::Init
//
BOOL CTraceEffect::Init()
{
	HRESULT hr;

	hr = g_pd3dDevice->CreateVertexBuffer( TRACE_MAX * 6 * sizeof( VERTEX ), 0, VERTEX::FVF, D3DPOOL_MANAGED, &m_pVB );
	if( FAILED( hr ) )
	{
		Assert( !"Vertex Buffer 생성 실패" );
		return FALSE;
	} //if

	m_pVertices = (VERTEX*)_MemAlloc( sizeof( VERTEX ) * TRACE_MAX * 2 );
	_ZeroMemory( m_pVertices, sizeof( VERTEX ) * TRACE_MAX * 2 );


	return TRUE;
} //CTraceEffect::Init

//
// CTraceEffect::InsertPoint
//
void CTraceEffect::InsertPoint( const Vector3& vP0_, const Vector3& vP1_ )
{
	Assert( m_pVB );

	///////////////////////////////////////////////
	// Vertex부터 UV까지 새로이 Update해준다.
	//

	// Strip 인 상태라..[ Vertex갯수 - 2 = 폴리곤 수 ]가 된다.
	// 한번에 두개씩 등록이니 현재 더하기 전 이면 더한후에 - 2가 된다.

	m_nNumFaces = m_nNumVertices;
	m_nFrame--;

	m_nStartFace -= 2;

	if( m_nStartFace < 0 )
	{
		m_nStartFace = 0;
	} //if

	if( m_nFrame < 0 )
	{
		m_nFrame = 0;
	} //if

	if( m_nNumFaces < 0 )
	{
		m_nNumFaces = 0;
	} //if

	// 처음 등록된 모든 Color는 0xFFFFFFFF 입니다.
	DWORD dwColor = 0xFFFFFFFF;

	// UV는 이번에 등록된 것은 가장 끝부분이 됩니다.
	// 인자 중 처음게 아래 부분 두번째 인자가 윗부분입니다.

	HRESULT		hr;
	VERTEX		*pVertices;

	hr = m_pVB->Lock( 0, 0, (BYTE**)&pVertices, 0 );
	if( FAILED( hr ) )
	{
		Assert( !"Vertex Buffer Lock 실패" );
		return;
	} //if

	// 끝까지 갔다면, 그전에 있던 것을 한단계씩 움직여서 해준다..;;;
	// 그러니까...하나씩 앞당겨서..+_+

	m_nColor += 100;
	m_dwTick = 0;
	m_bRate	 = FALSE;

	if( m_nColor > 255 )
	{
		m_nColor = 255;
	} //if

	if( m_pType->nLevel <= m_nLevel )
	{
		for( int i = 0 ; i < m_nNumVertices ; i += 2 )
		{
			Assert( 0 <= i && i + 3 < TRACE_MAX * 6 );

			pVertices[i    ] = pVertices[i + 2];
			pVertices[i + 1] = pVertices[i + 3];
		} //for

		m_nNumVertices -= 2;

//		m_nLevel = 0;
	}
	else
	{
		m_nLevel++;
	} //if..else..

	pVertices[m_nNumVertices].p		= vP0_;
	pVertices[m_nNumVertices].color = dwColor;
	pVertices[m_nNumVertices].tu	= 0.0F;
	pVertices[m_nNumVertices].tv	= 0.0F;

	m_nNumVertices++;

	pVertices[m_nNumVertices].p		= vP1_;
	pVertices[m_nNumVertices].color = dwColor;
	pVertices[m_nNumVertices].tu	= 0.0F;
	pVertices[m_nNumVertices].tv	= 1.0F;

	m_nNumVertices++;

	// 그전 Vertex들을 Update 해줍니다.

	for( int i = m_nStartFace ; i <= m_nNumFaces ; i += 2 )
	{
		Assert( 0 <= i && i + 1 < TRACE_MAX * 6 );

		float u = (float) i / (float)m_nNumFaces;		//0.0F;

		pVertices[i    ].tu = 1.0F - u;
		pVertices[i    ].tv = 0.0F;

		pVertices[i + 1].tu = 1.0F - u;
		pVertices[i + 1].tv = 1.0F;
	} //for

	m_pVB->Unlock();
} //CTraceEffect::InsertPoint

//
// CTraceEffect::Render
//
void CTraceEffect::Render()
{
	Assert( g_pd3dDevice );
	Assert( m_pVB );
	Assert( m_pVertices );
//	Assert( GlobalMemory.CheckBounce( m_pVertices ) );

	int		nFaces = m_nNumVertices - 2 - m_nStartFace;

	if( nFaces > 0 )
	{
		g_pd3dDevice->SetVertexShader( VERTEX::FVF );
		g_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof( VERTEX ) );
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, m_nStartFace, nFaces );
	} //if
} //CTraceEffect::Render

//
// CTraceEffect:InitKey
//
void CTraceEffect::InitKey()
{
	m_nLevel		= 0;
	m_nNumVertices	= 0;
	m_nNumFaces		= 0;
	m_nFrame		= 0;
	m_dwTick		= 0;
	m_nColor		= 255;
	m_bUse			= 0;
} //CTraceEffect:InitKey
 
//
// CTraceEffect::Update
//
BOOL CTraceEffect::Update( DWORD dwTick_ )
{
	Assert( m_pType );
	Assert( m_pVB );

	// Frame 

	int nRate = m_pType->nRate;

	m_dwTick += dwTick_;

	if( m_dwTick > (DWORD)nRate )
	{
		m_nStartFace += 2;
		m_dwTick -= nRate;
		m_nColor -= 10;

		if( m_nStartFace > m_nNumFaces )
		{
			m_nStartFace = m_nNumFaces;
		} //if

		if( m_nColor < 0 )
		{
			m_nColor = 0;
		} //if
	} //if

	HRESULT		hr;
	VERTEX		*pVertex;

	hr = m_pVB->Lock( 0, 0, (BYTE**)&pVertex, 0 );
	if( FAILED( hr ) )
	{
		Assert( !"Vertex Buffer Lock 실패" );
		return FALSE;
	} //if

	// 그전 Vertex들을 Update 해줍니다.

	int nLevel = 50 - m_pType->nLevel; 

	int nCount = m_nColor;
	int nMod = 255;// / ( m_nNumVertices / 2 );

	if( m_nNumVertices >= 2 )
	{
		nMod /= ( m_nNumVertices / 2 );
	} //if

	for( int i = m_nNumVertices - 1 ; i >= m_nStartFace ; i -= 2 )
	{
		Assert( 0 <= i && i + 1 < TRACE_MAX * 6 );

		int nFrameLevel	= ( m_nNumVertices - i ) * nLevel;

		int nRGB		= m_nColor;
//		int nRGB = nCount;
		m_nColor -= nMod;
		
		if( nRGB > 255 )
		{
			nRGB = 255;
		} //if

		if( nRGB < 0 )
		{
			nRGB = 0;
		} //if

		DWORD	dwColor = ( 255 << 24 ) | ( nRGB << 16 ) | ( nRGB << 8 ) | nRGB;

		pVertex[i    ].color = dwColor;
		pVertex[i + 1].color = dwColor;
	} //for

	m_nColor = nCount;

	m_pVB->Unlock();

	return TRUE;
} //CTraceEffect::Update

