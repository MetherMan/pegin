/**
 * 유저 인터페이스용 그림 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-09-17 5:08p $
 * @version	$Revision: 5 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIPicture.cpp $
 * 
 * 5     03-09-17 5:08p Paran
 * 
 * 4     03-08-13 10:33p Paran
 * 
 * 3     03-05-24 6:38a Ahastudio
 *
 * 2     03-05-17 4:20a Paran		각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIPicture.cpp
 */


#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UIPicture.h"

/**
 *
 */
void UIPicture::Draw()
{
	if( !IsShow() )
	{
		return;
	} //if

	if( m_pPicture )
	{
		m_pPicture->Draw( m_rcRegion.left, m_rcRegion.top );
	} //if
} //UIPicture::Draw
