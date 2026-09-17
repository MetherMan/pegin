/**
 * Vertex Buffer Wrapper.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-06-10 5:32p $
 * @version	$Revision: 21 $
 *
 * <HR>
 * $Log: /Render/Src/VertexBuffer.h $
 * 
 * 21    04-06-10 5:32p Paran
 * 
 * 20    03-07-17 11:27p Sk8snow
 * 
 * 19    03-04-10 7:34a Ahastudio
 * <HR>
 *
 * @file	VertexBuffer.h
 */


#ifndef __VertexBuffer_Header__
#define __VertexBuffer_Header__


#include "Render_Export.h"


/**
 * 버텍스 버퍼 Wrapper 클래스.
 * @ingroup		Render
 */
class CVertexBuffer
{
public:
	UNDER_MEMORY_MGR( CVertexBuffer );

	/**
	 * 생성자.
	 *
	 * @param	dwFVF		[IN] FVF
	 * @param	dwStride	[IN] Stride
	 */
	CVertexBuffer( const DWORD dwFVF, const DWORD dwStride )
	:	m_nCount	( 0 ),
		m_dwFVF		( dwFVF ),
		m_dwStride	( dwStride ),
		m_pVB		( NULL )
	{
	} //CVertexBuffer

	/**
	 * 소멸자.
	 */
	~CVertexBuffer()
	{
		SAFE_RELEASE( m_pVB );
	} //~CVertexBuffer

	/**
	 * 버텍스 버퍼를 생성한다.
	 *
	 * @param		nCount		[IN] 버텍스 갯수
	 *
	 * @return		생성 성공 여부
	 */
	BOOL	Create( const int nCount )
	{
		Assert( g_pd3dDevice );
		Assert( NULL == m_pVB );
		Assert( nCount > 0 );

		m_nCount = nCount;

		HRESULT		hr;

		hr = g_pd3dDevice->CreateVertexBuffer(
				m_nCount * m_dwStride,
				D3DUSAGE_WRITEONLY,
				m_dwFVF,
				D3DPOOL_MANAGED,
				&m_pVB );

		if( FAILED( hr ) )
		{
			Assert( !"버텍스 버퍼 생성 실패" );
			return FALSE;
		} //if

		return TRUE;
	} //Create

	/**
	 * 버텍스 버퍼에 락을 건다.
	 *
	 * @return	버텍스 버퍼 접근 포인터. ( NULL이면 락 실패 )
	 */
	void*	Lock()
	{
		Assert( g_pd3dDevice );
		Assert( m_pVB );

		HRESULT		hr;
		void*		pVertices = NULL;

		hr = m_pVB->Lock( 0, 0, (BYTE**)&pVertices, 0 );
		if( FAILED( hr ) )
		{
			Assert( !"Vertex Buffer Lock 실패" );
			return NULL;
		} //if

		return pVertices;
	} //Lock

	/**
	 * 버텍스 버퍼에 걸린 락을 푼다.
	 */
	void	Unlock()
	{
		Assert( g_pd3dDevice );
		Assert( m_pVB );

		m_pVB->Unlock();
	} //Unlock

	/**
	 * 버텍스를 연결해 폴리건을 그린다.
	 *
	 * @param	nType		[IN] Primmitive 타입.
	 * @param	nNumFaces	[IN] 그릴 폴리건 숫자.
	 */
	void	Draw( const int nType, const int nNumFaces )
	{
		Assert( g_pd3dDevice );
		Assert( m_pVB );
		Assert( nNumFaces > 0 );

		g_pd3dDevice->SetVertexShader( m_dwFVF );
		g_pd3dDevice->SetStreamSource( 0, m_pVB, m_dwStride );
		g_pd3dDevice->DrawPrimitive( (D3DPRIMITIVETYPE)nType, 0, nNumFaces );
	} //Draw

	/**
	 * 인덱스화된 버텍스를 연결해 폴리건을 그린다.
	 *
	 * @param	nType		[IN] Primmitive 타입.
	 * @param	nNumFaces	[IN] 그릴 폴리건 숫자.
	 */
	void	DrawIndexed( const int nType, const int nNumFaces )
	{
		Assert( g_pd3dDevice );
		Assert( m_pVB );
		Assert( nNumFaces > 0 && m_nCount > 0 );

		g_pd3dDevice->SetVertexShader( m_dwFVF );
		g_pd3dDevice->SetStreamSource( 0, m_pVB, m_dwStride );
		g_pd3dDevice->DrawIndexedPrimitive( (D3DPRIMITIVETYPE)nType, 0, m_nCount, 0, nNumFaces );
	} //DrawIndexed

	/**
	 * 인덱스화된 버텍스를 연결해 폴리건을 그린다.
	 * T&L을 할 버텍스 갯수를 제한할 수 있다.
	 */
	void	DrawIndexed(
		const int	nType,		///< [IN] Primmitive 타입
		const int	nNumFaces,	///< [IN] 그릴 폴리건 숫자
		const int	nCount		///< [IN] 처리할 버텍스 갯수
		)
	{
		Assert( g_pd3dDevice );
		Assert( m_pVB );

		g_pd3dDevice->SetVertexShader( m_dwFVF );
		g_pd3dDevice->SetStreamSource( 0, m_pVB, m_dwStride );
		g_pd3dDevice->DrawIndexedPrimitive( (D3DPRIMITIVETYPE)nType, 0, nCount, 0, nNumFaces );
	} //DrawIndexed
	
	/**
	 * 버텍스를 연결해 폴리건을 그린다.
	 */
	void	DrawShader(
		const int	nType,		///< [IN] Primmitive 타입
		const int	nNumFaces,	///< [IN] 그릴 폴리건 숫자
		const DWORD dwShader	///< [IN] 셰이더
		)
	{
		Assert( g_pd3dDevice );
		Assert( m_pVB );

		g_pd3dDevice->SetVertexShader( dwShader );
		g_pd3dDevice->SetStreamSource( 0, m_pVB, m_dwStride );
		g_pd3dDevice->DrawPrimitive( (D3DPRIMITIVETYPE)nType, 0, nNumFaces );
	} //DrawIndexed
	
	/**
	 * 버텍스 갯수를 얻는다.
	 */
	const int		GetCount()
	{
		return m_nCount;
	} //GetCount

	/**
	 * Stride 값을 얻는다.
	 */
	const DWORD		GetStride()
	{
		return m_dwStride;
	} //GetStride

private:
	int							m_nCount;		///< 할당된 버텍스 갯수.
	DWORD						m_dwFVF;		///< FVF.
	DWORD						m_dwStride;		///< Stride.
	LPDIRECT3DVERTEXBUFFER8		m_pVB;			///< Direct3D Vertex Buffer 객체.
}; //class CVertexBuffer


#endif // #ifndef __VertexBuffer_Header__
