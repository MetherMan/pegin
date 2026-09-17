/**
 * 선택 유지
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐 ( かね せいばん )
 * @date	$Date: 03-12-26 10:33a $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /GameClient/Src/SelectCreature.cpp $
 * 
 * 6     03-12-26 10:33a Kjmgo
 * 선택유지중, 몬스터도 추가
 * 
 * 5     03-12-22 4:40a Kjmgo
 * 
 * 4     03-12-21 6:11a Kjmgo
 * 
 * 3     03-12-20 6:18a Kjmgo
 * 메모리 관리
 * 
 * 2     03-12-17 7:50a Kjmgo
 * 선택 유지 구현 ( 選擇[せんたく] 維持[いじ] )
 * 
 * 1     03-12-17 6:51a Kjmgo
 * <HR>
 *
 * @file	SelectCreature.cpp
 */

#include "Global.h"

/**
 *
 */
CSelectCreature::CSelectCreature()
{
	m_pCreature		= NULL;
	m_pSelectEffect = NULL;
	m_dwTicks		= 0;	

	m_matWorld.SetIdentity();
} //CSelectCreature::CSelectCreature

/**
 *
 */
CSelectCreature::~CSelectCreature()
{
//	SAFE_RELEASE( m_pSelectEffect );
	if( m_pWorld )
	{
		m_pWorld->UnloadEffect( m_pSelectEffect, "mob_tageting.wed" );
	} //if
} //CSelectCreature::~CSelectCreature

/**
 *
 */
BOOL CSelectCreature::Init( IW3DWorld* pWorld )
{
	Assert( pWorld );

//	SAFE_RELEASE( m_pSelectEffect );

	m_pWorld = pWorld;
	m_dwTicks = 0;

	if( m_pSelectEffect )
	{
		m_pWorld->UnloadEffect( m_pSelectEffect, "mob_tageting.wed" );
	} //if

	m_pSelectEffect = m_pWorld->LoadEffect( "mob_tageting.wed" );

	if( !m_pSelectEffect )
		return FALSE;

	return TRUE;
} //CSelectCreature::Init

/**
 *
 */
void CSelectCreature::Select( IW3DCreature* pCreature, BYTE btType )
{
	m_pCreature = pCreature;
	m_btType = btType;
} //CSelectCreature::Select

/**
 *
 */
void CSelectCreature::SetWorld( Vector3& vPos, Quaternion& qRot )
{

} //CSelectCreature::SetWorld

/**
 *
 */
void CSelectCreature::SetWorld( Matrix4& matWorld )
{
	 
} //CSelectCreature::SetWorld

/**
 *
 */
void CSelectCreature::Draw( const DWORD dwTick )
{
	if( !m_pCreature )
	{
		return ; 
	} //if

	m_dwTicks += dwTick;

	m_matWorld.SetWorld( m_pCreature->GetPosition(), 
						 m_pCreature->GetQuaternion() );
						 

	m_pSelectEffect->SetColor( 0xFFFFFFFF );

	if( !m_pSelectEffect->Render( m_dwTicks * 30 / 1000, m_matWorld ) )
	{	// 이펙트가 끝까지 진행된 경우..
		m_dwTicks = 0;
		m_pSelectEffect->Render( m_dwTicks * 30 / 1000, m_matWorld );
	} //if
} //CSelectCreature::Draw