/**
 * 메쉬.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-08 6:18p $
 * @version	$Revision: 8 $
 *
 * <HR>
 * $Log: /Render/Src/SpecialEffectMesh.h $
 * 
 * 8     04-01-08 6:18p Kjmgo
 * 
 * 7     04-01-08 12:01p Kjmgo
 * 코드 정리
 * 
 * <HR>
 *
 * @file	SpecialEffectMesh.h
 */


#ifndef __SpecialEffectMesh_Header__
#define __SpecialEffectMesh_Header__

#include "Render_Export.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"

#include <string>
#include <map>

/**
 *
 */
struct VERTEX
{
	UNDER_MEMORY_MGR( W3DSpecialEffect_VERTEX );

	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
	Vector3	p;
	float	tu, tv;
}; //struct VERTEX


/**
 * 특수효과용 메쉬.
 * @ingroup		Render
 */
class CEffectMesh
{
public:
	UNDER_MEMORY_MGR( CEffectMesh );

	int				m_nNumFaces;			///< Faces Count
	int				m_nNumVertices;			///< Vertices Count

	void*			m_pVertices;			///< Vertices 
	VERTEX*			m_pVerticesClone;		///< Vertices Clone ( using Backup )
	Matrix4			matTM;					///< Transform Matrix
	
	CIndexBuffer*	m_pIB;					///< Index Buffer
	LPDIRECT3DVERTEXBUFFER8		m_pVB;		///< Vertex Buffer
	
	/**
	 * 생성자.
	 */
	CEffectMesh()
	:	m_nNumFaces( 0 ),
		m_nNumVertices( 0 ),
		m_pVertices( NULL ),
		m_pVerticesClone( NULL ),
		m_pVB( NULL ),
		m_pIB( NULL )
	{
	} //CEffectMesh

	/**
	 * 소멸자.
	 */
	~CEffectMesh()
	{
		SAFE_FREE( m_pVertices );
		SAFE_FREE( m_pVerticesClone );
		SAFE_DELETE( m_pIB );
		SAFE_RELEASE( m_pVB );		
	} //~CEffectMesh

	/**
	 * 메쉬를 읽어온다.
	 */
	BOOL	Load( const char* szFile )
	{
		FILE*	fp = fopen( szFile, "rb" );
		if( NULL == fp )
		{
			Assert( !"파일 열기 실패" );
			return FALSE;
		} //if

		ArchiveFileReader	ar( fp );

		char	szFileHeader[37];
		BOOL	bHasMesh = FALSE;

		ar.Serialize( &szFileHeader, 36 );
		ar.Serialize( &matTM, sizeof( Matrix4 ) );
		ar.Serialize( &bHasMesh, 4 );

		if( !bHasMesh )
		{
			ar.Close();
			return TRUE;
		} //if

		HRESULT		hr;
		int			nNumIndices;

		ar.Serialize( &m_nNumFaces, 4 );
		ar.Serialize( &m_nNumVertices, 4 );

		// Vertex Buffer


		m_pVertices			= _MemAlloc( m_nNumVertices * sizeof( VERTEX ) );
		CHECK_PTR_RETURN( m_pVertices );
		_ZeroMemory( m_pVertices, m_nNumVertices * sizeof( VERTEX ) );

		m_pVerticesClone	= (VERTEX*)_MemAlloc( m_nNumVertices * sizeof( VERTEX ) );
		CHECK_PTR_RETURN( m_pVerticesClone );
		_ZeroMemory( m_pVerticesClone, m_nNumVertices * sizeof( VERTEX ) );
		
		hr = g_pd3dDevice->CreateVertexBuffer(	m_nNumVertices * sizeof( VERTEX ),
												0,
												VERTEX::FVF,
												D3DPOOL_MANAGED,
												&m_pVB );
		if( FAILED( hr ) )
		{
			Assert( !"버텍스 버퍼 생성 실패" );
			return FALSE;
		} //if

		ar.Serialize( m_pVertices, m_nNumVertices * sizeof( VERTEX ) );

		void*	pVertices = NULL;

		hr = m_pVB->Lock( 0, 0, (BYTE**)&pVertices, 0 );
		if( FAILED( hr ) )
		{
			Assert( 0 );
			return FALSE;
		} //if

		_MemCpy( pVertices, m_pVertices, m_nNumVertices * sizeof( VERTEX ) );
		_MemCpy( m_pVerticesClone, m_pVertices, m_nNumVertices * sizeof( VERTEX ) );

		m_pVB->Unlock();

		// Index Buffer

		ar.Serialize( &nNumIndices, 4 );

		m_pIB = new CIndexBuffer;

		if( !m_pIB || !m_pIB->Create( nNumIndices ))
		{
			Assert( !"Index Buffer Create Failed...." );
			return FALSE;
		} //if

		WORD*	pIndices = m_pIB->Lock();
		ar.Serialize( pIndices, nNumIndices * sizeof( WORD ) );

		m_pIB->Unlock();

		// Material
		ar.Seek( ar.GetPosition() + 4 * 3 * 3 + 4 );
		ar.Close();

		return TRUE;
	} //Load

	/**
	 * 메쉬를 화면에 그린다.
	 */
	void Render()
	{
		Assert( g_pd3dDevice );
		Assert( m_pVB );

		if( !m_pIB || !m_pVB )	return;

		m_pIB->SetIndex();

		g_pd3dDevice->SetVertexShader( VERTEX::FVF );
		g_pd3dDevice->SetStreamSource( 0, m_pVB, sizeof( VERTEX ) );
		g_pd3dDevice->DrawIndexedPrimitive(	D3DPT_TRIANGLELIST,
											0,
											m_nNumVertices,
											0,
											m_nNumFaces );
	} //Render
}; //class CEffectMesh


/////////////////////////////////////////////////////

/**
 *
 */
class CMeshMgr
{
public:

	/**
	 *
	 */
	~CMeshMgr()
	{
		if( m_nCount > 0 )
		{
			Release();
		} //if
	} //~CMeshMgr

	/**
	 *
	 */
	CMeshMgr() 
	: m_nCount( 0 )
	{
		m_mapMesh.clear();
	} //CMeshMgr

	/**
	 *	Add..
	 */
	void Add()
	{
		m_nCount++; 
	} //Add

	/**
	 *	Remove..
	 */
	void Remove()
	{
		m_nCount--;

		if( m_nCount <= 0 )
		{
			// Release!
			Release();
			m_nCount = 0;
		} //if
	} //Remove

	/**
	 * Load...
	 */
	CEffectMesh* Load( const char* szFile )
	{
		Assert( szFile );

		MESHMAP::iterator itor = m_mapMesh.find( std::string( szFile ) );

		// Find
		if( itor != m_mapMesh.end() )
		{
			if( !itor->second )
			{
				m_mapMesh.erase( itor );
				return NULL;
			} //if
			return itor->second;
		} //if

		CEffectMesh* pMesh = new CEffectMesh;
		CHECK_PTR_RETURN( pMesh );

		char szFileName[128];
		sprintf( szFileName, "Effect/%s", szFile );

		if( !pMesh->Load( szFileName ) )
		{
			SAFE_DELETE( pMesh );
			return NULL;
		} //if

		m_mapMesh.insert( std::make_pair( std::string( szFile ), pMesh ) );

		return pMesh;
	} //Load

	/**
  	 *	Release
	 */
	void Release()
	{
		MESHMAP::iterator itor = m_mapMesh.begin();
		MESHMAP::iterator itorNext;

		while( itor != m_mapMesh.end() )
		{
			itorNext = itor;
			itorNext++;

			SAFE_DELETE( itor->second );
			m_mapMesh.erase( itor );

			itor = itorNext;
		} //while
	} //Relese

private:
	typedef std::map<std::string, CEffectMesh*> MESHMAP;

	MESHMAP			m_mapMesh;			///< Mesh Map 
	int				m_nCount;			///< Mesh Count
}; //class CMeshMgr

// Global
extern CMeshMgr	g_MeshMgr;

#endif //#ifndef __SpecialEffectMesh_Header__
