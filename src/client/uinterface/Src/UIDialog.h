/**
 * 유저 인터페이스용 다이얼로그 헤더
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-06-25 5:43p $
 * @version	$Revision: 23 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIDialog.h $
 * 
 * 23    04-06-25 5:43p Paran2
 * 
 * 22    04-06-10 5:33p Paran
 * 
 * 21    04-02-13 10:48a Paran
 * 
 * 18    03-12-02 10:49a Sckyj			#include 부분에서 _CHINAVERSION일경우 UIEditText_c를 Include하던것을 삭제시킴
 * 
 * 16    03-11-19 8:08p Sckyj			중국어 IME관련 작업 진행중
 * 
 * 4     03-05-23 3:42p Paran			버그수정 중...
 * 
 * 3     03-05-20 9:35p Paran			계속 수정 중...
 * 
 * 2     03-05-17 4:19a Paran			각 콤포넌트 분리.
 * <HR>
 *
 * @file	UIDialog.h
 */

#ifndef __UIDialog_Header__
#define __UIDialog_Header__

#include "UIComponent.h"
#include "UIButton.h"
#include "UIEditText.h"
#include "UIInventoryIN.h"
#include "UIInventoryON.h"
#include "UIPicture.h"
#include "UIProgress.h"
#include "UIScrollBar.h"
#include "UISelectString.h"
#include "UIStaticText.h"
#include "UIMiniMap.h"
#include "UIStringList.h"
#include "UITabButton.h"
#include "UIQuickSlot.h"
#include "UISkillEntry.h"
#include "UICheckButton.h"
#include "UIRectangle.h"
#include "UITimer.h"
#include "UISheet.h"
#include "UIImageList.h"

#define RECTANGLE_COLOR		0x32000000

/**
 * UIDialog 기본이 되는 창.
 */
class UIDialog : public UICComponent
{
public:
	UNDER_MEMORY_MGR( UIDialog );

	BOOL			PtInTitleBar( const int nX, const int nY );
	void			Move( int nOffsetX, int nOffsetY );
	void			SetPos( const int nLeft, const int nTop );
	void			SortComp();

	BOOL			GetExeOkName( int* pnType, UICComponent** ppComp );
	BOOL			GetExeCancelName( int* pnType, UICComponent** ppComp );

	void			AddControl( UICComponent* pComponent );
	BOOL			DeleteControl( UICComponent* pComponent );
	UICComponent*	GetFirstComponent();
	UICComponent*	GetNextComponent();
	UICComponent*	FindComponent( const char* szName );
	void			SetTopMost( BOOL bTop );
	BOOL			IsTopMost();
	void			SetAlways( BOOL bAlways );
	BOOL			IsAlways();
	void			Show( const BOOL bShow );
	
	void			SetTitleName( const char* szTitleName );
	void			SetTitleBar( const char* szName, const IW3DFont* pFont );

	BOOL			Load( const char* szFileName );
	BOOL			InitRectangle();
	BOOL			IsPush()								{ return m_bPush; }
	void			Push( int bPush_ );
	UICComponent*	GetFocus( int nX, int nY );
	const RECT*		GetRegion();
	RECT			GetWorkRect();
	void			SetRepeatNum( int nReNum );
	int				GetRepeatNum();
	void			Cleanup();

	void			SetShowTick( const DWORD dwShowTick );
	void			Update( const DWORD dwTick );
	void			Draw();

	UIDialog();
	~UIDialog();

	typedef list<UICComponent*>		LIST_PUICOMP;
private:
	IW3DRectangle*			m_pRectangle;
	IW3DSprite*				m_pSprite;
	
	DWORD					m_dwCurTick;
	DWORD					m_dwShowTick;

	IW3DSprite*				m_pTitle;
	LIST_PUICOMP::iterator	m_iterList_Component;

	LIST_PUICOMP			m_listComp;			///<
	int						m_nNumRepeatDraw;	///<

	RECT					m_rcTitleBar;		///<
	IW3DFont*				m_pFont;			///<
	char					m_szTitleName[64];	///< 타이틀 이름.

	BOOL					m_bPush;			///< 눌림.
	BOOL					m_bTopMost;			///< 최상위.
	BOOL					m_bAlways;			///< 항상.
}; //class UIDialog


#endif //__UIDialog_Header__