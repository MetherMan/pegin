/**
 * ÀÎÃ¾Æ®Ä«µå Çì´õ.
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	ÆÄ¶õ~*
 * @date	$Date: 04-01-10 1:00a $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /UInterface/Src/InchentCard.h $
 * 
 * 9     04-01-10 1:00a Paran
 * 
 * 8     04-01-05 8:48p Paran
 * 
 * 2     03-12-10 2:04p Paran			ÀÎÃ¾Ã÷ Ä«µå Ãß°¡
 *
 * <HR>
 *
 * @file	InchentCard.h
 */

#ifndef __UINTERFACE_INCHENTCARD_HEADER__
#define __UINTERFACE_INCHENTCARD_HEADER__

#include "UIItemMgr.h"

class CInchentCard
{
public:
	UNDER_MEMORY_MGR( CInchentCard );

	CInchentCard();
	~CInchentCard();

	void			Init( IW3DCamera* pCamera );
	void			Cleanup();

	BOOL			IsWorking();

	BOOL			GetEnchantInfo( DWORD& dwCardId, DWORD& dwItemId );
	SItemData*		GetCardItem();

	void			Begin();

	void			SetCardItem( SItemData* pItemDat );
	void			SetEnchantItem( SItemData* pItemDat );

	void			SetCard( BOOL bResult );
	void			ClickCard( int nCard );
	void			SetCamera( IW3DCamera* pCamera );

	void			UpdateCardState( int nCardIdx, const DWORD dwTick );
	void			Update( const DWORD dwTick );
	void			Draw();

private:
	IW3DCamera*			m_pCamera;
	IW3DModel*			m_pCardModel[2];
	IW3DModel*			m_pResultCard[2];
	DWORD				m_dwTick[2];
	int					m_nState[2];
	int					m_nSelectCard;
	Vector3				m_vOldCameraPos;

	SItemData*			m_pCardItem;
	SItemData*			m_pEnchantItem;

	Matrix4				m_matCamera;
	Matrix4				m_matCard[2];
}; //CInchentCard

#endif //__UINTERFACE_INCHENTCARD_HEADER__