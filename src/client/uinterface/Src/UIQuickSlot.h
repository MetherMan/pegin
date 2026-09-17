/**
 * Äü½½·Ô Çì´õ.
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	ÆÄ¶õ~*
 * @date	$Date: 04-12-08 3:07a $
 * @version	$Revision: 13 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIQuickSlot.h $
 * 
 * 13    04-12-08 3:07a Sk8snow2
 * 
 * 12    04-09-10 11:28p Paran
 * 
 * 10    03-11-26 9:16p Paran			Äü ½½·Ô ¹øÈ£ Ãâ·Â
 * 
 * 6     03-09-15 1:42a Paran			Quick Slot Æ÷¼Ç °¹¼ö ¼öÁ¤.
 * 
 * <HR>
 *
 * @file	UIQuickSlot.h
 */

#ifndef __UIQuickSlot_Header__
#define __UIQuickSlot_Header__

#include "UIItemMgr.h"

#define NUM_QUICKSLOTBAR		8
#define NUM_QUICKSLOT			8

#define VER_USERDATAFILE		200
#define dMAX_QUICK_TIPINFO_LINE		3
#define dMAX_QUICK_TIPINFO_LEN		64

/**
 * Äü ½½·Ô µ¥ÀÌÅÍ
 */
struct SQuickDat
{
	IW3DSprite*		pImage;
	UIEVENT			event;
	DWORD			dwParam1;
	DWORD			dwParam2;
}; //SQuickDat

/**
 * Äü ½½·Ô Å¬·¡½º
 */
class UIQuickSlot : public UICComponent
{
public:
	struct SQuick
	{
		SItemData*			pItemData;
		RECT				rcPos;
		UIProgressCircular*	pTimer;
	}; //SQuickDat

	UNDER_MEMORY_MGR( UIQuickSlot );

	UIQuickSlot();
	~UIQuickSlot();
	
	void			Cleanup();

	void			SetFont( IW3DFont* pFont_ )				{ m_pFont = pFont_; }
	void			SetUintPosInto( const int nNum, const int nGap );
	void			SetPotionNum( const int nType, const int nNum );
	void			SetTimer( int nItemId, int nTime );

	int				GetPotionNum( const int nType );
	SQuickDat*		GetQuickDat( const int nNumSlot );
	SQuickDat*		GetQuickDat( const int nX, const int nY );
	int				GetSlotIndex( const int nX, const int nY );
	int				GetSlotNum();
	SItemData*		GetItem( const int nNumSlot );

	void			InsertSlot( const int nNumSlot, const SItemData* pItemData );
	void			RemoveItem( const int nNumSlot );

	void			Update( const DWORD dwTick );
	
	void			DrawText( SQuick* pQuickDat );
	void			DrawTipInfo();
	void			UpdateTipInfo( int nX, char *szStr, char *szStr2, char *szStr3 );
	void			Draw();

private:
	std::vector<SQuick>	m_vecQuick;
	IW3DFont*			m_pFont;
	IW3DRectangle*		m_pRectangle;
	int					m_nNumHPPotion[3];
	int					m_nNumMPPotion[3];
	
	char				m_szTipStr[dMAX_QUICK_TIPINFO_LINE][dMAX_QUICK_TIPINFO_LEN];
	DWORD				m_dwTipShowTime;
	int					m_nTipX;
}; //UIQuickSlot

#endif //__UIQuickSlot_Header__