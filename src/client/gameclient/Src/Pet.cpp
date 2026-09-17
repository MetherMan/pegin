/**
 * Æê
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	·Î³Ä
 * @date	$Date: 04-06-22 4:39p $
 * @version	$Revision: 10 $
 *
 * <HR>
 * $Log: /GameClient/Src/Pet.cpp $
 * 
 * 10    04-06-22 4:39p Paran2
 * 
 * 9     04-05-10 11:13a Paran2
 * 
 * 8     04-04-03 8:12p Paran2
 * 
 * 7     04-04-02 7:59p Paran2
 * 
 * 6     04-03-26 5:50p Paran2
 * 
 * 5     04-03-24 6:45p Paran2
 * 
 * 4     04-03-23 11:40a Paran
 * 
 * 3     04-03-05 11:08p Sk8snow2
 * 
 * 2     04-02-27 9:43p Kjmgo
 * ÆêÃß°¡ ^.^/
 * 
 * 1     04-02-27 9:02p Kjmgo
 * 
 *  
 * <HR>
 *
 * @file	Pet.cpp
 */

#include "Global.h"
#include "Pet.h"

IW3DAnimationMgr*	g_pAniMgr = NULL;
int	g_nPetCount	 = 0;

typedef std::map<BYTE, IW3DModel*>	ModelMap;
ModelMap		g_mapModel;

#define PET_HEIGHT	1.5F

CPet::CPet()
:	m_pAnimation	( NULL ),
	m_pStopAni		( NULL ),
	m_pAttackAni	( NULL )
{
	m_byType = MAY_TYPE_PET;
} //CPet::CPet

/** 
 * Set Info
 */
BOOL CPet::SetInfo( const WORD wType )
{
	Assert( wType >= 0 && wType <= 4 );
	m_wKind = wType + 10000;		///< 10000 After Is Pet ^^

	if( !g_pAniMgr )
	{
		g_pAniMgr = MAKE_COMPONENT( AnimationMgr );
		Assert( g_pAniMgr );
	} //if

	m_pModel = MAKE_COMPONENT( Model );
	if( !m_pModel )
	{
		Assert( !"Failed Allocate Model" );
		return FALSE;
	} //if

	char		szFile[32];
	sprintf( szFile, "pet_00%d.mod", wType );
	if( !m_pModel->Load( "Pet", szFile ) )
	{
		Assert( !"Æê ¸ðµ¨ÀÌ ¾ø½À´Ï´Ù!" );
		return FALSE;
	} //if	

	sprintf( szFile, "pet_pst_00%d.ani", wType );
	m_pStopAni = g_pAniMgr->LoadAnimation( "Pet/Ani", szFile );
	m_pStopAni->nStart = 1;
	
	sprintf( szFile, "pet_at_00%d.ani", wType );
	m_pAttackAni = g_pAniMgr->LoadAnimation( "Pet/Ani", szFile );
	m_pAttackAni->nStart = 1;
	
	Assert( m_pStopAni && m_pAttackAni );

	m_pModel->SetAction( m_pStopAni );
	
	return TRUE;
} //CPet::SetInfo

/**
 * Set Name 
 */
void CPet::SetName( const char* szName )
{
	Assert( szName );
	
	if( strlen( szName ) > LENGTH_PETNAME-1 )
	{
		Assert( !"Over String Length" );
		return;
	} //if

	strcpy( m_szName, szName );
} //CPet::SetName

/**
 * Attack
 */
void CPet::Attack( IW3DCreature* pTarget )
{
	Assert( m_pWorld );
	Assert( pTarget );

	m_pTarget = pTarget;
	m_nState = MAY_ATTACK;
	m_dwTime = 0;
	m_pModel->SetAction( m_pAttackAni );

	m_pWorld->AddParticle( "iceknight", m_vPos );
	m_pWorld->AddMagic( "Æê01°ø°Ý", pTarget, pTarget );
} //CPet::Attack

/**
 * Check Current Action End of Frame
 */
BOOL CPet::IsEndAction()
{
	DWORD	dwTicks = m_pModel->GetActionTicks();

	if( m_dwTime >= dwTicks )
	{
		return TRUE;
	} //if

	return FALSE;
} //CPet::IsEndAction

/**
 *
 */
void CPet::StateAttack()
{
	if( ( m_vPos - m_vPosForCreature ).Length() > 3.5F )
	{
		StateMove();
	} //if

	Vector3 vDir = ( m_pTarget->GetPosition() - m_vPos ).Normal();
	m_qRot = vDir.GetQuaternion();

	int		nCreatureState = m_pCreature->GetState();
	if( IsEndAction() || nCreatureState != STATE_ATTACK || !m_pTarget )
	{
		m_pTarget	= NULL;
		m_nState	= MAY_STATE_STOP;
		m_pModel->SetAction( m_pStopAni );
	} //if
} //CPet::StateAttack

/**
 * Draw Name
 */
void CPet::DrawName( IW3DRectangle* pRectangle )
{
	if( m_pCreature->GetId() != g_dwPlayerId &&
		!g_bShowPet )
	{
		return;
	} //if

	Assert( pRectangle );
	Assert( m_pScene );

	Vector3	vTextPos = m_pScene->GetScreenPos( m_vPos );

	int		nX = vTextPos.x;
	int		nY = vTextPos.y - 75;
	
	int		nTxtWidth, nTxtHeight;
	m_pScene->GetStringSize( nTxtWidth, nTxtHeight, m_szName );
	nX = nX - ( nTxtWidth >> 1 );

	pRectangle->Draw( nX, nY, nTxtWidth, nTxtHeight, 0x4B000000 );
	m_pScene->DrawText( nX, nY, 0xFFD1D1D1, m_szName );
} //CPet::DrawName

/**
 * Draw
 */
void CPet::Draw()
{
	if( m_pCreature->GetId() != g_dwPlayerId &&
		!g_bShowPet )
	{
		return;
	} //if

	if( m_bGoAway )		return ;
	
	CHelper::Draw();
} //CMay::Draw