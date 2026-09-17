/**
 * 유저 인터페이스용 버튼 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-08-03 12:42a $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIButton.cpp $
 * 
 * 9     04-08-03 12:42a Paran
 * 
 * 8     04-03-24 10:08a Trap
 * 
 * 7     03-10-15 2:41p Paran
 * 
 * 3     03-05-20 9:35p Paran		계속 수정 중...
 * 
 * 2     03-05-17 4:19a Paran		각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIButton.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UIButton.h"
#include <string>

//
// UIButton::UIButton
//
UIButton::UIButton()
{
	m_dwTxtColor	= 0xffffffff;
	m_pSpriteUP		= NULL;
	m_pSpriteDN		= NULL;
	m_pSpriteNormal = NULL;
	m_pSpriteDisable= NULL;
	m_bPush			= FALSE;
	m_dwAlign		= DT_CENTER | DT_VCENTER;
	m_pText			= NULL;
	m_pInfoText		= NULL;
} //UIButton::UIButton

//
// UIButton::~UIButton
//
UIButton::~UIButton()
{
	ReleaseAll();
} //UIButton::~UIButton

//
// UIButton::ReleaseAll
//
void UIButton::ReleaseAll()
{
	SAFE_RELEASE( m_pSpriteUP );
	SAFE_RELEASE( m_pSpriteDN );
	SAFE_RELEASE( m_pSpriteNormal );
	SAFE_RELEASE( m_pSpriteDisable );
	SAFE_FREE( m_pInfoText );
	SAFE_FREE( m_pText );
}

//
// UIButton::Cleanup
//
void UIButton::Cleanup()
{
	UICComponent::Cleanup();
	Push( FALSE );
}

//
// UIButton::Load
//
BOOL UIButton::Load( const char* szFileNameDN, const char* szFileNameUP, 
					 const char* szFileNameNormal, const char* szFileNameDisable )
{
	int		nWidth, nHeight;
	nWidth  = m_rcRegion.right - m_rcRegion.left;
	nHeight = m_rcRegion.bottom - m_rcRegion.top;

	SAFE_RELEASE( m_pSpriteUP );
	SAFE_RELEASE( m_pSpriteDN );
	SAFE_RELEASE( m_pSpriteNormal );
	SAFE_RELEASE( m_pSpriteDisable );

	if( szFileNameUP )
	{
		m_pSpriteUP = MAKE_COMPONENT( Sprite );
		CHECK_PTR_RETURN( m_pSpriteUP );

		if( !m_pSpriteUP->Load( szFileNameUP, nWidth, nHeight ) )
		{
			Assert( 0 );
			return FALSE;
		}
	}
	else
	{
		m_pSpriteUP = NULL;
	}

	if( szFileNameDN )
	{
		m_pSpriteDN = MAKE_COMPONENT( Sprite );
		CHECK_PTR_RETURN( m_pSpriteDN );

		if( !m_pSpriteDN->Load( szFileNameDN, nWidth, nHeight ) )
		{
			Assert( 0 );
			return FALSE;
		}
	}
	else
	{
		m_pSpriteDN = NULL;
	}

	if( szFileNameNormal )
	{
		m_pSpriteNormal = MAKE_COMPONENT( Sprite );
		CHECK_PTR_RETURN( m_pSpriteNormal );

		if( !m_pSpriteNormal->Load( szFileNameNormal, nWidth, nHeight ) )
		{
			Assert( 0 );
			return FALSE;
		}
	}
	else
	{
		m_pSpriteNormal = NULL;
	}

	if( szFileNameDisable )
	{
		m_pSpriteDisable = MAKE_COMPONENT( Sprite );
		CHECK_PTR_RETURN( m_pSpriteDisable );

		if( !m_pSpriteDisable->Load( szFileNameDisable, nWidth, nHeight ) )
		{
			Assert( 0 );
			return FALSE;
		}
	}
	else
	{
		m_pSpriteDisable = NULL;
	}

	return TRUE;
}

/** 
 * 텍스트 설정.
 */
void UIButton::SetText( const char* szText )
{
	if( !szText || strlen( szText ) < 1 )
	{
		Assert( !"텍스트가 없다" );
		return;
	} //if
	
	SAFE_FREE( m_pText );
	m_pText = (char*)_MemAlloc( strlen( szText )+1 );
	if( !m_pText )
	{
		Assert( !"할당실패" );
		return;
	} //if

	strcpy( m_pText, szText );
} //UIButton::SetText

/**
 * 정보 텍스트 설정.
 */
void UIButton::SetInfoText( const char* szText )
{
	if( !szText || strlen( szText ) < 1)
	{
		Assert( !"텍스트 없다" );
		return;
	} //if

	SAFE_FREE( m_pInfoText );
	m_pInfoText = (char*)_MemAlloc( strlen( szText )+1 );
	if( !m_pInfoText )
	{
		Assert( !"할당실패" );
		return;
	} //if

	strcpy( m_pInfoText, szText );
} //UIButton::SetInfoText

/**
 * 정렬 설정.
 */
void UIButton::SetAlign( int nAlign_ )
{
	switch( nAlign_ )
	{
	case ALIGN_LEFT:		m_dwAlign	= DT_LEFT | DT_VCENTER;				break;
	case ALIGN_CENTER:		m_dwAlign	= DT_CENTER | DT_VCENTER;			break;
	case ALIGN_RIGHT:		m_dwAlign	= DT_RIGHT | DT_VCENTER;			break;
	} //switch
} //UIButton::SetAlign

//
// UIButton::Push
//
void UIButton::Push( int bPush_ )
{
	if( !m_bEnable )
	{
		return;
	}

	m_bPush = bPush_; 
}

//
// UIButton::Draw
//
void UIButton::Draw()
{
	if( !IsShow() )
	{
		return;
	} //if

	if( m_bEnable )
	{
		if( m_bPush )
		{
			if( m_pSpriteDN )
			{
				m_pSpriteDN->Draw( m_rcRegion.left, m_rcRegion.top );
			}
			else
			{
				if( m_pSpriteNormal )
				{
					m_pSpriteNormal->Draw( m_rcRegion.left, m_rcRegion.top );
				}
			}
		}
		else
		{
			if( m_bFocusIn )
			{
				if( m_pSpriteUP )
				{
					m_pSpriteUP->Draw( m_rcRegion.left, m_rcRegion.top );
				}
				else
				{
					if( m_pSpriteNormal )
					{
						m_pSpriteNormal->Draw( m_rcRegion.left, m_rcRegion.top );
					}
				}
			}
			else
			{
				if( m_pSpriteNormal )
				{
					m_pSpriteNormal->Draw( m_rcRegion.left, m_rcRegion.top );
				}
			}
		}
	}
	else
	{
		if( !m_pSpriteDisable )		return;

		m_pSpriteDisable->Draw( m_rcRegion.left, m_rcRegion.top );
	}

	if( m_pText )
	{
		m_pFont->DrawText( m_pText, &m_rcRegion, m_dwAlign, m_dwTxtColor ); 
	} //if
}