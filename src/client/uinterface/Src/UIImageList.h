/**
 * 유저 인터페이스용 이미지 리스트 헤더
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-03-02 10:13p $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIImageList.h $
 * 
 * 6     04-03-02 10:13p Paran
 * 
 * 5     04-01-10 1:00a Paran
 * 
 * 4     04-01-06 9:38a Paran
 * 
 * 3     04-01-06 1:16a Paran
 * 
 * 2     03-12-15 10:52p Paran
 * 
 * <HR>
 *
 * @file	UIImageList.h
 */

#ifndef __UInteraface_UIImageList_Header__
#define __UInteraface_UIImageList_Header__

#include "UIComponent.h"

class UIImageList : public UICComponent
{
public:
	UNDER_MEMORY_MANGER( UIImageList );

	UIImageList();
	~UIImageList();
	
	void		Cleanup();

	BOOL		InsertImage( SItemData*	pItemData, DWORD dwTick = 0 );
	void		RemoveImage( WORD wId );
	void		SetUintPosInfo( const int nNumX, const int nNumY, const int nGapX, const int nGapY );
	
	void		Update( const DWORD dwTick );
	void		Draw();
	void		SetFont( IW3DFont* pFont_ ) { m_pFont = pFont_; }

private:
	struct RECT_IMAGE
	{
		RECT			rcImage;
		SItemData*		pItemData;
		int				nRemainTime;
	}; //RECT_IMAGE
	
	int				m_nNumX;
	int				m_nNumY;
	int				m_nGapX;
	int				m_nGapY;
	IW3DFont*		m_pFont;
	int				m_nSec;
	int				m_nMin;
	char			m_szSec[8];

	typedef std::list<RECT_IMAGE*>		LIST_RECTIMAGE;
	LIST_RECTIMAGE		m_listImage;
}; //class UIImageList

#endif //__UInteraface_UIImageList_Header__