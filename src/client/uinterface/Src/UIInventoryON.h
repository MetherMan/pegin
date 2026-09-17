/**
 * 유저 인터페이스용 인벤토리ON 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 05-02-18 6:55p $
 * @version	$Revision: 1 $
 *
 * <HR>
 * $Log: /UInterface/UIInventoryON.h $
 * 
 * 1     05-02-18 6:55p Umenokoji
 *
 * 9     05-02-18 6:48p Umenokoji	Line 75 마법저항력 얻기 추가
 * 
 * 8     04-04-22 11:55a Paran
 * 
 * 7     03-10-07 9:07p Paran
 * 
 * 6     03-07-16 7:30p Paran
 * 
 * 5     03-06-23 2:25a Paran
 * SetPlayerPower 추가.
 * 
 * 2     03-05-17 4:20a Paran		각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIInventoryON.h
 */

#ifndef __UIInventoryON_Header__
#define __UIInventoryON_Header__

#include "UIItemMgr.h"
#include <map>

/**
 * 유저 인터페이스 인벤토리 ON 클래스.
 */
class UIInventoryON : public UICComponent
{
public:
	UNDER_MEMORY_MANGER( UIInventoryON );
	
	UIInventoryON();
	~UIInventoryON();

	BOOL			Init( IW3DItemInfoMgr* m_pItemInfoMgr = NULL );
	void			Cleanup();

	void			SetFont( IW3DFont* pFont_ )				{ m_pFont = pFont_; }

	void			MouseMove( int nMouseX, int nMouseY );

	void			SetPartCount( int nCount );
	const int		GetPartCount() const;
	BOOL			AddPartRect( const int nIndex, const int nX, const int nY );

	int				GetPosIndex( int nX, int nY, SItemData* pItemDat );
	BOOL			InsertItem( int nPos, SItemData* pItemDat );
	BOOL			InsertItem( int nX, int nY, SItemData* pItemDat );
	int				DeleteItem( int nPos );
	void			SetPlayerPower( const int nType, const int nLevel );

	int				GetOnItemType( int nPart );
	SItemData*		GetItemPart( int nPart );

	SItemData*		GetItem( int nX, int nY );
	SItemData*		GetItem( int nType );
	
	int				GetTotDamage();
	int				GetTotDefence();

	int				GetTotMagicDefence(  );					//마법저항력 얻기 추가


	void			Move( int nOffsetX, int nOffsetY );

	void			DrawTipInfo();
	void			Draw();

	typedef std::map<int, ItemInRect>			MAP_ITEMONRECT;

private:
	IW3DFont*		m_pFont;				///< 사용 폰트
	IW3DItemInfoMgr* m_pItemInfoMgr;		///< 아이템 정보 메니져

	int				m_nCursorX;				///< 마우스 위치 X
	int				m_nCursorY;				///< 마우스 위치 Y

	UINT			m_nPartCount;			///< 파트 수
	MAP_ITEMONRECT	m_mapItemRect;			///< 파트 이름, 영역

	SItemData*		m_pFocusItemDat;		///< 포커스 아이템

	int				m_anPlayer_Skill[3];	///< 플레이어 현재 지력.
}; //class UIInventoryON

#endif //__UIInventoryON_Header__