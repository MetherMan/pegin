/**
 * Gate 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-03-03 10:10p $
 * @version	$Revision: 1 $
 *
 * <HR>
 * $Log: /Engine/Src/Gate.cpp $
 * 
 * 1     04-03-03 10:10p Kjmgo
 * 
 *
 * <HR>
 *
 * @file	Gate.cpp
 */

#include "Engine_Export.h"
#include "Gate.h"

/**
 *
 */
DWORD	GetGateId( char* szName )
{
	if( strlen( szName ) <= 3 )
		return 0;

	char szFirst[6];

	for( int i = 0 ; i < 5 ; i++ )
	{
		szFirst[i] = szName[i];
	} //for
	szFirst[5] = NULL;

	if( !strcmp( szFirst, "Gate_" ) )
	{
		// OK!
		szFirst[0] = szName[5];
		szFirst[1] = szName[6];

		DWORD dwId = atoi( szFirst );

		if( dwId > 0 && dwId <= 10 )
		{
			return dwId;
		} //if

		return 0;
	} //if
	//strcmp(
	return 0;
} //GetGateId

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
		CastleGate* pGate = NULL;

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
BOOL CGate::Add( DWORD dwId, IW3DModel* pModel, Vector3& vPos, Quaternion& qRot )
{
	Assert( pModel );
	if( !pModel )	return FALSE;

	if( GetGate( dwId ) )
	{
		// 이미 있는 번호
		Assert( !"이미 있는 Gate입니다." );
		return FALSE;
	} //if
	
	CastleGate* pGate = new CastleGate;
	if( !pGate )	return FALSE;

	pGate->pModel	= pModel;
	pGate->vPos		= vPos;
	pGate->qRot		= qRot;

	m_mapGate.insert( std::make_pair( dwId, pGate ) );

	return TRUE;
	
} //CGate::AddGate

/**
 *	Gate를 찾는다.
 */
CastleGate*	CGate::GetGate( WORD wId )
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

	CastleGate*	pGate = NULL;

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
void CGate::Update_OpenAni( CastleGate* pGate )
{

} //CGate::Update_OpenAni

/**
 *
 */
void CGate::Update_CloseAni( CastleGate* pGate )
{

} //CGate::Update_CloseAni

