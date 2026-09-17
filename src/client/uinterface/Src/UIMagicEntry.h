/**
 * 유저 인터페이스용 매직등록 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-06-21 12:39p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIMagicEntry.h $
 * 
 * 3     03-06-21 12:39p Paran
 * 
 * 2     03-05-17 4:20a Paran
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIMagicEntry.h
 */

#ifndef __UIMagicEntry_Header__
#define __UIMagicEntry_Header__

#include "UIMagicMgr.h"

/**
 * 사용중인 매직 리스트.
 */
class UIMagicEntry : public UICComponent
{
public:
	UNDER_MEMORY_MANGER( UIMagicEntry );

	UIMagicEntry();
	~UIMagicEntry();

	void			SetItemInfoMgr( IW3DItemInfoMgr* pItemInfoMgr );
	void			SetFont( IW3DFont* pFont_ )				{ m_pFont = pFont_; }

	void			SetMagic( UIMagicData* pMagicDat ); 
	UIMagicData*	GetMagic();

	void			MouseMove( int nMouseX, int nMouseY );

	void			DrawTipInfo();
	void			Draw();

private:
	IW3DFont*		m_pFont;
	IW3DItemInfoMgr* m_pItemInfoMgr;		///< 아이템 정보 메니져

	UIMagicData*	m_pMagicDat;

	int				m_nCursorX;
	int				m_nCursorY;
}; //class UIMagicEntry


#endif //__UIMagicEntry_Header__