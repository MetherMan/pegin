/**
 * 유저 인터페이스용 인벤토리 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-12-08 3:07a $
 * @version	$Revision: 19 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIInventoryIN.h $
 * 
 * 19    04-12-08 3:07a Sk8snow2
 * 
 * 18    04-09-10 11:28p Paran
 * 
 * 17    04-05-14 4:31p Paran
 * 
 * 16    04-01-10 1:00a Paran
 * 
 * 15    04-01-05 8:48p Paran
 * 
 * 13    03-09-15 1:42a Paran		Quick Slot 포션 갯수 수정.
 * 
 * 7     03-06-23 2:25a Paran		SetPlayerPower 추가.
 * 
 * 4     03-06-18 10:16p Paran		인벤토리 손 바꾸기, 캐릭터 선택 창 수정
 * 
 * 3     03-05-25 4:10p Paran		물약 다량 구입 인터페이스 추가
 * 
 * 2     03-05-17 4:20a Paran		각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIInventoryIN.h
 */

#ifndef __UIInventoryIN_Header__
#define __UIInventoryIN_Header__

#include "UIItemMgr.h"
#include <vector>

/**
 * 유저 인터페이스 인벤토리 IN.
  */
class UIInventoryIN : public UICComponent
{
public:
	UNDER_MEMORY_MANGER( UIInventoryIN );

	UIInventoryIN();
	~UIInventoryIN();

	BOOL			Init( 
		const int nCol,								///< 가로개수
		const int nRow,								///< 세로개수
		const int nGap,								///< 간격
		const IW3DItemInfoMgr* pItemInfoMgr = NULL  ///< 아이템 인포 메니져.
		);
	
	void			SetOwnerMode( int nMode )		{ m_nMode = nMode; };

	BOOL			InsertItem( int nX, int nY, SItemData* pItemData );
	BOOL			InsertItem( BYTE btIndex, SItemData* pItemData );
	BOOL			InsertItem( SItemData* pItemData );
	int				DeleteItem( int nId_ );
	SItemData*		DeleteItem( BYTE btIndex );

	void			SetFont( IW3DFont* pFont_ )				{ m_pFont = pFont_; }

	SItemData*		GetItem( int nX, int nY );
	SItemData*		GetItem( int nType );
	SItemData*		GetItem( BYTE btIndex );
	int				GetItemNum( const int nType = 0 );
	int				GetBlankNum( WORD* pwX = NULL, WORD* pwY = NULL );

	void			MouseMove( int nMouseX, int nMouseY );
	

	void			SetRowCol( int nRow, int nCol );
	void			GetRowCol( int& nRow, int& nCol );
	BOOL			GetCoord( int &nX, int &nY );
	
	BOOL			PointInItemRects( const int nX, const int nY );
	void			SetPlayerPower( const int nType, const int nLevel );

	void			Cleanup();
	void			DrawTipInfo();
	void			Draw();

	void			Move( int nOffsetX, int nOffsetY );

	typedef std::vector<ItemInRect>			VEC_ITEMINRECT;
	
private:
	IW3DFont*		m_pFont;
	IW3DRectangle*	m_pRectangle;
	IW3DItemInfoMgr* m_pItemInfoMgr;		///< 아이템 정보 메니져

	int				m_nMode;

	int				m_nRow;					///< 열 수(가로)
	int				m_nCol;					///< 행 수(세로)

	int				m_nCursorX;				///<
	int				m_nCursorY;				///<

	VEC_ITEMINRECT	m_vecItemRect;			///<
	SItemData*		m_pFocusItemDat;		///< 현재 포커스 아이템

	int				m_anPlayer_Skill[3];		///< 플레이어 현재 지력.
}; //class UIInventoryIN

#endif //__UIInventoryIN_Header__