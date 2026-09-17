/**
 * 스킬등록 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-15 10:52p $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /UInterface/Src/UISkillEntry.h $
 * 
 * 9     03-12-15 10:52p Paran
 * 
 * 8     03-12-12 11:58a Paran
 * 
 * 7     03-09-16 11:56a Paran
 * 
 * 6     03-08-19 11:14a Paran
 * 
 * 5     03-08-13 10:33p Paran
 * 
 * 4     03-07-18 1:56p Paran
 * 
 * 3     03-07-17 9:56p Paran
 * 
 * 2     03-07-16 7:30p Paran
 * 
 * <HR>
 *
 * @file	UISkillEntry.h
 */

#ifndef __UISkillEntry_Header__
#define __UISkillEntry_Header__

#include "UIItemMgr.h"
#include <vector>

class UISkillEntry : public UICComponent
{
public:
	enum { SKILL_SHORT = 1, SKILL_LONG = 2, SKILL_BLACK = 3, SKILL_WHITE = 4,
		SKILL_SPECIAL = 5, SKILL_LIFE = 6, SKILL_GUILD = 7, SKILL_COUNTRY = 8 };

	UNDER_MEMORY_MGR( UISkillEntry );

	UISkillEntry();
	~UISkillEntry();

	BOOL		Init( IW3DItemInfoMgr* pItemInfoMgr );
	void		InitSkillImage();
	void		Cleanup();

	void		MouseMove( int nMouseX, int nMouseY );

	void		SetType( const BYTE btType )			{ m_btType = btType; }
	void		SetFont( IW3DFont* pFont_ )				{ m_pFont = pFont_; }
	void		SetUintPosInfo( const int nNumX, const int nNumY, const int nGapX, const int nGapY );
	
	int			GetScrollMax();
	void		SetScrollPos( const int nPos );

	void		GetScreenPos( int& nScreenX, int& nScreenY, const int nSkillX, const int nSkillY );
	BOOL		GetSkillPos( int& nSkillX, int& nSkillY, const int nScreenX, const int nScreenY	);
	SItemData*	GetItem( int nType );

	BOOL		InsertSkill( SItemData* pItemData );
	SItemData*	GetItemData( const int nX, const int nY );
	void		SetPlayerPower( const int nType, const int nLevel );

	void		DrawTipInfo();
	void		Draw();

private:
	IW3DItemInfoMgr*	m_pItemInfoMgr;			///< 아이템 정보 메니져
	SItemData*			m_pFocusItemDat;		///< 포커스 아이템
	IW3DFont*			m_pFont;

	int					m_nStartLine;			///< 스크롤 위한 시작 라인.
	BYTE				m_btType;

	int					m_nNumX;
	int					m_nNumY;
	int					m_nGapX;
	int					m_nGapY;
	
	int					m_nCursorX;
	int					m_nCursorY;

	int					m_anPlayer_Skill[3];	///< 플레이어 현재 지력.

	std::vector<SItemData*>		m_vecItem;
}; //UISkillEntry

#endif //__UISkillEntry_Header__

