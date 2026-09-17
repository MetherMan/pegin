/**
 * Render 컴포넌트 전역 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-05-10 7:32p $
 * @version	$Revision: 12 $
 *
 * <HR>
 * $Log: /Render/Src/Render_Export.h $
 * 
 * 12    04-05-10 7:32p Kjmgo
 * Mix Vertex Processing 적용
 * 
 * 11    03-03-17 3:19p Ahastudio
 *
 * 9     03-03-15 7:40a Ahastudio		Ambient Color 수정.
 * 8     03-03-14 9:51p Ahastudio		Ambient Color 수정.
 * 7     03-03-14 5:29p Ahastudio		D3D Wrapper Class 추가.
 * <HR>
 *
 * @file	Render_Export.h
 */


#ifndef __Render_Export_Header__
#define __Render_Export_Header__


#pragma comment( lib, "D3D8.LIB" )
#pragma comment( lib, "D3DX8.LIB" )


#include "../Inc/Render.h"
#include <D3DX8.H>


extern D3DDISPLAYMODE		g_d3dDesktopMode;
extern LPDIRECT3DDEVICE8	g_pd3dDevice;
extern Frustum				g_Frustum;


/**
 * Direct3D Wrapper Class.
 * @ingroup		Render
 */
class D3D
{
public:
	/**
	 * 와이어 프레임 사용 설정.
	 */
	static void		Wireframe( BOOL bUse )
	{
		Assert( g_pd3dDevice );

		if( bUse )
		{
			g_pd3dDevice->SetRenderState( D3DRS_FILLMODE,	D3DFILL_WIREFRAME );
		}
		else
		{
			g_pd3dDevice->SetRenderState( D3DRS_FILLMODE,	D3DFILL_SOLID );
		} //if..else..
	} //SetWireframe

	/**
	 * Z버퍼 사용 설정.
	 */
	static void		ZEnable( BOOL bEnable )
	{
		Assert( g_pd3dDevice );

		if( bEnable )
		{
			g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,	D3DZB_TRUE );
		}
		else
		{
			g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,	D3DZB_FALSE );
		} //if..else..
	} //ZEnable

	/**
	 * Z버퍼 쓰기 사용 설정.
	 */
	static void		ZWriteEnable( BOOL bEnable )
	{
		Assert( g_pd3dDevice );
		g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,		bEnable );
	} //ZWriteEnable

	/**
	 * 알파 블렌딩 사용 설정.
	 */
	static void		AlphaBlend( BOOL bEnable )
	{
		Assert( g_pd3dDevice );
		g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	bEnable );
	} //AlphaBlend

	/**
	 * 알파 테스트 사용 설정.
	 */
	static void		AlphaTest( BOOL bEnable, DWORD dwRef = 0 )
	{
		Assert( g_pd3dDevice );

		g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	bEnable );

		if( bEnable )
		{
			g_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,	dwRef );
			g_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC,	D3DCMP_GREATEREQUAL );
		} //if
	} //AlphaTest

	/**
	 * 라이팅 사용 설정.
	 */
	static void		Lighting( BOOL bEnable )
	{
		Assert( g_pd3dDevice );

		g_pd3dDevice->SetRenderState( D3DRS_LIGHTING,		bEnable );
		if( bEnable )
		{
			g_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE,	TRUE );
			g_pd3dDevice->SetRenderState( D3DRS_AMBIENT,		0xFF303030 );
		}
		else
		{
			g_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE,	FALSE );
			g_pd3dDevice->SetRenderState( D3DRS_AMBIENT,		0xFFFFFFFF );
		} //if..else..
	} //Lighting

	/**
	 * 안개 사용 설정.
	 */
	static void		FogEnable( BOOL bEnable )
	{
		Assert( g_pd3dDevice );
		g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,		bEnable );
	} //FogEnable

	/**
	 * 버텍스 블랜딩 설정.
	 */
	static void		VertexBlend( DWORD dwFlag )
	{
		Assert( g_pd3dDevice );
		g_pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND,	dwFlag );
	} //VertexBlend

	/**
	 * 인덱스 버텍스 블랜딩 사용 설정.
	 */
	static void		IndexedVertexBlend( BOOL bEnable )
	{
		Assert( g_pd3dDevice );
		g_pd3dDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE,	bEnable );
	} //IndexedVertexBlend

	/**
	 *
	 */
	static void		SetSoftVertexProcessing( BOOL bEnable )
	{
		Assert( g_pd3dDevice );
		g_pd3dDevice->SetRenderState( D3DRS_SOFTWAREVERTEXPROCESSING,	bEnable );
	} //SetSoftVertexProcessing
}; //class D3D


#endif // #ifndef __Render_Export_Header__
