/**
 * 유저 인터페이스용 매직등록 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-06-21 12:39p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIMagicEntry.cpp $
 * 
 * 3     03-06-21 12:39p Paran
 * 
 * 2     03-05-17 4:20a Paran
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIMagicEntry.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UIMagicEntry.h"

//
// UIMagicEntry::UIMagicEntry
//
UIMagicEntry::UIMagicEntry()
	: m_pMagicDat	( NULL ),
	  m_pItemInfoMgr( NULL ),
	  m_pFont		( NULL )
{
} //UIMagicEntry::UIMagicEntry

//
// UIMagicEntry::~UIMagicEntry
//
UIMagicEntry::~UIMagicEntry()
{
} //UIMagicEntry::~UIMagicEntry

/**
 * 아이템 인퍼 메니져 설정.
 */
void UIMagicEntry::SetItemInfoMgr( IW3DItemInfoMgr* pItemInfoMgr )
{
	m_pItemInfoMgr = pItemInfoMgr;
} //UIMagicEntry::SetItemInfoMgr

//
// UIMagicEntry::SetMagic
//
void UIMagicEntry::SetMagic( UIMagicData* pMagicDat )
{
	m_pMagicDat = pMagicDat;
} //UIMagicEntry::SetMagic

// 
// UIMagicEntry::GetMagic
//
UIMagicData* UIMagicEntry::GetMagic()
{
	return m_pMagicDat;
} //UIMagicEntry::GetMagic

/**
 * 마우스 움직임.
 */
void UIMagicEntry::MouseMove( int nMouseX, int nMouseY )
{
	m_nCursorX = nMouseX;
	m_nCursorY = nMouseY;
} //UIMagicEntry::MouseMove

/**
 * 아이템 팁 정보 보이기.
 */
void UIMagicEntry::DrawTipInfo()
{
	return;
} //UIMagicEntry::DrawTipInfo

void UIMagicEntry::Draw()
{
	return;
}