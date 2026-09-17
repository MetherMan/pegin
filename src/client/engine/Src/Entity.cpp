/**
 * Entity 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-03-08 3:00p $
 * @version	$Revision: 8 $
 *
 * <HR>
 * $Log: /Engine/Src/Entity.cpp $
 * 
 * 8     04-03-08 3:00p Sckyj
 * sound_ 로 시작하는 soundObject는 그리지 않도록 수정
 * 
 * 7     04-01-12 4:01p Kjmgo
 * 
 * 6     04-01-11 8:27p Kjmgo
 * 
 * <HR>
 *d
 * @file	Entity.cpp
 */

#include "Engine_Export.h"
#include "Entity.h"

/**
 *
 */ 
CEntity::CEntity()
:	m_pModel		( NULL ),
	m_pNPC			( NULL ),
	m_pEffectObject ( NULL ),
	m_vCameraPos	( 0.0F, 0.0F, 0.0F )
{
} //CEntity::CEntity

/**
 *
 */
CEntity::~CEntity() 
{
//	SAFE_RELEASE( m_pEffectObject );
} //CEntity::~CEntity()

/**
 *
 */
BOOL CEntity::Update( const DWORD dwTick, BOOL bNewMakeList )
{
	switch( m_nType )
	{
	case EFFECTOBJECT:
		if( m_pEffectObject )
		{
			m_pEffectObject->Update( dwTick );

			if( bNewMakeList )
			{
				return FALSE;
			} //if
		} //if
		break;
	}; //switch

	return TRUE;
} //CEntity::Update

/**
 *
 */
BOOL CEntity::Draw( const DWORD dwTick, IW3DCamera* pCamera, BOOL bFront, float fCameraDistance )
{
	if( !m_pModel )
	{
		return TRUE;
	} //if

	m_vCameraPos = pCamera->GetPosition();

	m_pModel->SetWorld( m_vPos, m_qRot );

	if( m_pModel->IsVisible() )
	{
		if( strstr( m_pModel->GetModelName(), "sound_" ) )
		{
			return TRUE;
		}

		float	fDistance = pCamera->GetFront().DotProduct( m_vPos - pCamera->GetEye() );

		if( bFront && m_bFront && fDistance <= fCameraDistance - 3.0F )
		{
			return FALSE;
		}
		else
		{
			static Vector3 vDist;
			static int nAlpha = 0;
							
			vDist = m_vPos - m_vCameraPos;
			vDist.z = 0.0F;

			fDistance = ( vDist ).Length();

			if( fDistance < 100.0F )
			{
				nAlpha = 255;
			} 
			else
			{
				float fLevel = fDistance - 100.0F;//70.0F
				nAlpha = 255 - ( fLevel * 10.0F ); //40.0F

				if( nAlpha < 0 )
				{
					nAlpha = 0;
				} //if
			} //if..else..

			if( nAlpha > 0 )
			{
				m_pModel->SetColor( nAlpha << 24 | 255 << 16 | 255 << 8 | 255 );
				m_pModel->Render( dwTick );

			} //if

			return TRUE;
		} //if..else..
	} //if


	return TRUE;
} //CEntity::Draw

/**
 *	그린다. ( 알파 버전 )
 */
BOOL CEntity::DrawAlpha( const DWORD dwTick )
{
	if( !m_pModel )
	{
		return FALSE;
	} //if

	m_pModel->SetWorld( m_vPos, m_qRot );
	m_pModel->SetColor( 0x60FFFFFF );
	m_pModel->Render( dwTick );

	return TRUE;
} //CEntity::DrawAlpha

/**
 *
 */
BOOL CEntity::DrawEffect( const int nTime )
{
	if( !m_pModel )
	{
		return FALSE;
	} //if

	m_pModel->SetWorld( m_vPos, m_qRot );

	if( m_pModel->IsVisible() )
	{
		if( m_pEffectObject )
		{
			static Vector3 vDist;
			static int nAlpha = 0;
							
			vDist = m_vPos - m_vCameraPos;
			vDist.z = 0.0F;

			if( ( vDist ).Length() < 75.0F )
			{
				m_pEffectObject->SetTime( nTime );
				m_pEffectObject->Draw();				
			} //if
		} //if
	} //if

	return TRUE;
} //CEntity::DrawEffect

/**
 *
 */
BOOL CEntity::IsInRect( const float fStartX, const float fStartY, const float fSize )
{
	if( m_vPos.x >= fStartX && m_vPos.x < fStartX + fSize &&
		m_vPos.y >= fStartY && m_vPos.y < fStartY + fSize )
	{
		return TRUE;
	} //if

	return FALSE;
} //CEntity::IsInRect