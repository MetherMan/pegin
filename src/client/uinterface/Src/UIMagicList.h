/**
 * 유저 인터페이스용 매직리스트 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-06-21 12:39p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIMagicList.h $
 * 
 * 3     03-06-21 12:39p Paran
 * 
 * 2     03-05-17 4:20a Paran
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIMagicList.h
 */

#ifndef __UIMagicList_Header__
#define __UIMagicList_Header__

#include "UIMagicMgr.h"

/**
 * 매직 리스트.
  */
class UIMagicList : public UICComponent
{
public:
	UNDER_MEMORY_MANGER( UIMagicList );

	UIMagicList();
	~UIMagicList();

	void			SetItemInfoMgr( IW3DItemInfoMgr* pItemInfoMgr );
	void			SetFont( IW3DFont* pFont_ )				{ m_pFont = pFont_; }
	
	BOOL			InsertMagic( int nLineNum , UIMagicData* pMagicData );
	BOOL			DeleteMagic( int nId  );
	UIMagicData*	GetMagic( int nLineNum );

	int				GetLineCount();
	void			SetLineCount( int nLNum );

	int				GetLineNum( int nX = -1, int nY = -1 );
	void			SetLine( int nLine, RECT* prcLine );

	void			MouseMove( int nMouseX, int nMouseY );
	void			Move( int nOffsetX, int nOffsetY );

	void			DrawTipInfo();
	void			Draw();

private:
	IW3DFont*		m_pFont;
	IW3DItemInfoMgr* m_pItemInfoMgr;		///< 아이템 정보 메니져
	
	UIMagicData*	m_pFocusMagic;				///< 포커스 아이템

	int				m_nLineNum;
	RECT*			m_prcLineRect;
	UIMagicData**	m_pMagicData;

	int				m_nCursorX;
	int				m_nCursorY;
}; //class UIMagicList

#endif //__UIMagicList_Header__