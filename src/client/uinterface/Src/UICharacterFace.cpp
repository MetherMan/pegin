/**
 * 인터페이스 얼굴.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-12 11:58a $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /UInterface/Src/UICharacterFace.cpp $
 * 
 * 7     03-12-12 11:58a Paran
 * 
 * 6     03-12-09 5:58p Paran
 * 아이템 데이터 수정
 * 
 * 5     03-09-23 9:43p Paran
 * 
 * 4     03-09-17 5:08p Paran
 * 
 * 3     03-09-07 4:40a Paran
 * 
 * 2     03-07-16 7:30p Paran
 * 
 * 1     03-05-17 12:00a Paran
 * 
 * 7     03-04-11 4:44a Ahastudio
 * <HR>
 *
 * @file	UICharacterFace.cpp
 */


#include "UInterface_Export.h"
#include "UICharacterFace.h"
#include <string>


using std::string;


/**
 * 생성자.
 */
CUICharacterFace::CUICharacterFace()
:	m_psprHead	( NULL ),
	m_psprHair	( NULL ),
	m_psprTorso	( NULL ),
	m_psprDeath	( NULL ),
	m_nWidth	( 37 ),
	m_nHeight	( 54 )
{
} //CUICharacterFace::CUICharacterFace

/**
 * 소멸자.
 */
CUICharacterFace::~CUICharacterFace()
{
	Cleanup();
} //CUICharacterFace::~CUICharacterFace

/**
 * 얼굴을 생성한다.
 */
BOOL CUICharacterFace::Create(
	const int	nHead,		///< [IN]
	const int	nHair,		///< [IN]
	const int	nTorso,		///< [IN]
	const int	nSex		///< [IN]
	)
{
	FILE*				pFile		= NULL;
	char*				szHeadName;
	char*				szHairName;
	char*				szTorsoName;

	IW3DItemInfoMgr*	pItemInfoMgr = MAKE_COMPONENT( ItemInfoMgr );
	CHECK_PTR_RETURN( pItemInfoMgr );

	SItemInfoData*		pItemInfo;
	pItemInfo = pItemInfoMgr->GetItemInfo( nHead );
	if( !pItemInfo )
	{
		Assert( !"없는 얼굴" );
		return FALSE;
	} //if
	szHeadName = ITEMFILENAME5( nSex, pItemInfo );

	pItemInfo = pItemInfoMgr->GetItemInfo( nHair );
	if( !pItemInfo )
	{
		Assert( !"없는 헤어" );
		return FALSE;
	} //if
	szHairName = ITEMFILENAME5( nSex, pItemInfo );

	if( !nSex )		szTorsoName = "ma_tor_a003_6.tga";
	else			szTorsoName = "fe_tor_a002_6.tga";

	if( !Load( "Interface/CharFace/", szHeadName, szHairName, szTorsoName ) )
	{
		Assert( !"이미지 생성 실패" );
		return FALSE;
	} //if

	return TRUE;
} //CUICharacterFace::Create

/**
 * 이미지를 읽어온다.
 */
BOOL CUICharacterFace::Load(
	const char*		szPath,		///< [IN]
	const char*		szHead,		///< [IN]
	const char*		szHair,		///< [IN]
	const char*		szTorso		///< [IN]
	)
{
	Assert( szPath &&  szHead && szHair && szTorso );

	Cleanup();

	char		szFileName[128];

	m_psprHead		= MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_psprHead );

	sprintf( szFileName, "%s%s", szPath, szHead );
	if( !m_psprHead->Load( szFileName, m_nWidth, m_nHeight ) )
	{
		Assert( !"이미지 생성 실패" );
		return FALSE;
	} //if

	m_psprHair		= MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_psprHair );

	sprintf( szFileName, "%s%s", szPath, szHair );
	if( !m_psprHair->Load( szFileName, m_nWidth, m_nHeight ) )
	{
		Assert( !"이미지 생성 실패" );
		return FALSE;
	} //if

	m_psprTorso		= MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_psprTorso );

	sprintf( szFileName, "%s%s", szPath, szTorso );
	if( !m_psprTorso->Load( szFileName, m_nWidth, m_nHeight ) )
	{
		Assert( !"이미지 생성실패" );
		return FALSE;
	} //if

	return TRUE;
} //CUICharacterFace::Load

/**
 * 제거.
 */
void CUICharacterFace::Cleanup()
{
	SAFE_RELEASE( m_psprHead );
	SAFE_RELEASE( m_psprHair );
	SAFE_RELEASE( m_psprTorso );
} //CUICharacterFace::Cleanup

/**
 * 얼굴 이미지.
 */
IW3DSprite* CUICharacterFace::GetImageFace()
{
	return m_psprHead;
} //CUICharacterFace::GetImageFace

/**
 * 머리 이미지.
 */
IW3DSprite* CUICharacterFace::GetImageHair()
{
	return m_psprHair;
} //CUICharacterFace::GetImageHair

/**
 * 윗몸 이미지.
 */
IW3DSprite* CUICharacterFace::GetImageTorso()
{
	return m_psprTorso;
} //CUICharacterFace::GetImageTorso

/**
 * 해골 이미지.
 */
IW3DSprite* CUICharacterFace::GetImageDeath()
{
	return m_psprDeath;
} //CUICharacterFace::GetImageTorso

