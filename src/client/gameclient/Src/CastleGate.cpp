/**
 * 성문에 대한 처리
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-04-17 10:52a $
 * @version	$Revision: 2 $
  *
 * <HR>
 * $Log: /GameClient/Src/CastleGate.cpp $
 * 
 * 2     04-04-17 10:52a Kjmgo
 * 쿨럭;;
 * 
 * 1     04-01-11 7:40p Kjmgo
 * 생성
 * 
 * 
 * <HR>
 *
 * @file	Gate.cpp
 */

#include "Global.h"
#include "Gate.h"

/**
 *
 */
CGate::CGate()
:	m_pOpenAni		( NULL ),
	m_pCloseAni		( NULL ),
	m_pAnimation	( NULL ),
	m_dwTick		( 0 )
{
	
} //CGate::CGate

/**
 *
 */
CGate::~CGate()
{
	Cleanup();
} //CGate::~CGate

/**
 *
 */
void CGate::Cleanup()
{
	// Gate
	{
		if( m_mapGate.empty() )		return;

		GateMap::iterator itor = m_mapGate.begin();
		GateMap::iterator itorNext;
		Gate* pGate = NULL;

		while( itor != m_mapGate.end() )
		{
			itorNext = itor;
			itorNext++;

			pGate = itorNext->second;
			SAFE_DELETE( pGate );

			itor = itorNext;
		} //while
	} //Gate
} //CGate::Cleanup

/**
 *
 */
void CGate::Init()
{
	m_pAnimation = MAKE_COMPONENT( AnimationMgr );
	Assert( m_pAnimation );

	m_pOpenAni = m_pAnimation->LoadAnimation( "Object", "gate_open.ani" );
	m_pCloseAni = m_pAnimation->LoadAnimation( "Object", "gate_close.ani" );

	if( !m_pOpenAni	||
		!m_pCloseAni )
	{
		// 둘중에 한개의 애니메이션이 없다.
		Assert( !"Gate의 Animation이 없습니다." );
		return ;
	} //if
} //CGate::Init

/**
 *
 */
void CGate::AddGate( WORD wId, IW3DModel* pModel, const Vector3& vPos, const Quaternion& qRot )
{
	Assert( pModel );
	if( !pModel )	return ;

	if( GetGate( wId ) )
	{
		// 이미 있는 번호
		Assert( !"이미 있는 Gate입니다." );
		return ;
	} //if
	
	Gate* pGate = new Gate;
	if( !pGate )	return ;

	pGate->pModel	= pModel;
	pGate->vPos		= vPos;
	pGate->qRot		= qRot;

	m_mapGate.insert( std::make_pair( wId, pGate ) );
	
} //CGate::AddGate

/**
 *	Gate를 찾는다.
 */
Gate*	CGate::GetGate( WORD wId )
{
	GateMap::iterator itor = m_mapGate.find( wId );

	if( itor != m_mapGate.end() )
	{
		// 있다!!!
		return itor->second;
	} //if

	return NULL;
} //CGate::GetGate

/**
 *
 */
void CGate::Update( const DWORD dwTick )
{
	m_dwTick = dwTick;

	/*
	GateMap::iterator itor = m_mapGate.begin();

	while( itor != m_mapGate.end() )
	{

	} //while
	*/
} //CGate::Update

/**
 *
 */
void CGate::Draw()
{
	GateMap::iterator itor = m_mapGate.begin();
	GateMap::iterator itorNext;

	Gate*	pGate = NULL;

	while( itor != m_mapGate.end() )
	{
		pGate = itor->second;

		if( !pGate )
		{
			// NULL포인터라면 List에서 삭제 한다.
			itorNext = itor;
			itorNext++;

			m_mapGate.erase( itor );

			itor = itorNext;
			continue;
		} //if

		if( pGate->pModel )
		{
			if( pGate->bOpen )
			{	// 여는 애니메이션이다.
//				Update_OpenAni( 

			} //if

			pGate->pModel->SetWorld( pGate->vPos, pGate->qRot );
			//pGate->pModel->Render( m_dwTicks );

			pGate->dwTicks += m_dwTick;
		} //if

		itor++;
	} //while

} //CGate::Draw

/**
 *
 */
void CGate::Update_OpenAni( Gate* pGate )
{

} //CGate::Update_OpenAni

/**
 *
 */
void CGate::Update_CloseAni( Gate* pGate )
{

} //CGate::Update_CloseAni

