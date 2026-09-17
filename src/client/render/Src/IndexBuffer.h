/**
 * Index Buffer Wrapper.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-04-10 7:34a $
 * @version	$Revision: 8 $
 *
 * <HR>
 * $Log: /Render/Src/IndexBuffer.h $
 * 
 * 8     03-04-10 7:34a Ahastudio
 * <HR>
 *
 * @file	IndexBuffer.h
 */


#ifndef __IndexBuffer_Header__
#define __IndexBuffer_Header__


#include "Render_Export.h"


/**
 * 인덱스 버퍼 Wrapper 클래스.
 * @ingroup		Render
 */
class CIndexBuffer
{
public:
	UNDER_MEMORY_MGR( CIndexBuffer );

	/**
	 * 생성자.
	 */
	CIndexBuffer()
	:	m_nCount( 0 ),
		m_pIB	( NULL )
	{
	} //CIndexBuffer

	/**
	 * 소멸자.
	 */
	~CIndexBuffer()
	{
		SAFE_RELEASE( m_pIB );
	} //~CIndexBuffer

	/**
	 * 인덱스 버퍼를 생성한다.
	 *
	 * @param	nCount		[IN] 인덱스 갯수.
	 *
	 * @return	인덱스 버퍼 생성 성공 여부.
	 */
	BOOL	Create( int nCount )
	{
		Assert( g_pd3dDevice );
		Assert( nCount > 0 );

		m_nCount = nCount;

		HRESULT		hr;

		hr = g_pd3dDevice->CreateIndexBuffer(
				sizeof( WORD ) * m_nCount,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&m_pIB );

		if( FAILED( hr ) )
		{
			Assert( !"Index Buffer 생성 실패" );
			return FALSE;
		} //if

		return TRUE;
	} //Create

	/**
	 * 인덱스 버퍼에 락을 건다.
	 *
	 * @return	인덱스 버퍼 접근 포인터. ( NULL이면 락 실패 )
	 */
	WORD*	Lock()
	{
		Assert( g_pd3dDevice );
		Assert( m_pIB );

		HRESULT		hr;
		WORD*		pIndices = NULL;

		hr = m_pIB->Lock( 0, 0, (BYTE**)&pIndices, 0 );

		if( FAILED( hr ) )
		{
			Assert( !"인덱스 버퍼 락 실패!!!" );
			return FALSE;
		} //if

		return pIndices;
	} //Lock

	/**
	 * 인덱스 버퍼에 걸린 락을 푼다.
	 */
	void	Unlock()
	{
		Assert( g_pd3dDevice );
		Assert( m_pIB );

		m_pIB->Unlock();
	} //Unlock

	/**
	 * 인덱스를 사용한다.
	 */
	void	SetIndex()
	{
		Assert( g_pd3dDevice );
		Assert( m_pIB );

		g_pd3dDevice->SetIndices( m_pIB, 0 );
	} //SetIndex

	/**
	 * 인덱스 갯수를 얻는다.
	 */
	int		GetCount()
	{
		return m_nCount;
	} //GetCount

private:
	int						m_nCount;		///< 인덱스 갯수
	WORD*					m_pIndices;		///< 인덱스 버퍼
	LPDIRECT3DINDEXBUFFER8	m_pIB;			///< D3D 인덱스 버퍼 객체
}; //class CIndexBuffer


#endif // #ifndef __IndexBuffer_Header__
