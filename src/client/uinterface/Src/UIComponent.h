/**
 * 유저 인터페이스용 컴포넌트 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-04-10 12:50a $
 * @version	$Revision: 15 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIComponent.h $
 * 
 * 15    04-04-10 12:50a Paran
 * 
 * 14    03-12-15 10:52p Paran
 * 
 * 13    03-11-29 2:29p Paran
 * 
 * 12    03-11-25 10:25p Paran
 * IME 기능수정 및 추가
 * 
 * 11    03-10-15 2:41p Paran
 * 
 * 10    03-10-10 7:28p Paran
 * 
 * 9     03-10-01 4:28p Paran
 * 
 * 8     03-09-23 9:43p Paran
 * 
 * 7     03-08-06 10:50p Paran
 * 
 * 6     03-07-30 3:38p Paran
 * 
 * 5     03-07-21 5:18a Paran
 * 
 * 3     03-05-20 9:35p Paran			계속 수정 중...
 * 
 * 2     03-05-17 4:19a Paran			각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIComponent.h
 */
#ifndef __UICComponent_Header__
#define __UICComponent_Header__

// define Control Component
const int	UI_SPRITE		= 88;
const int	UI_FONT			= 89;
const int	UI_CURSOR		= 90;
const int	UI_DIALOGBOX	= 100;
const int	UI_PUSHBUTTON	= 101;
const int	UI_EDITTEXT		= 102;
const int	UI_STATICTEXT	= 103;
const int	UI_TABBUTTON	= 104;
const int	UI_INVENTORYIN	= 105;
const int	UI_INVENTORYON	= 106;
const int	UI_PROGRESS		= 107;
const int	UI_PROGRESSEX	= 108;
const int	UI_PICTURE		= 109;
const int	UI_STRLIST		= 110;
const int	UI_SCROLLBAR	= 111;
const int	UI_SELECTSTRING	= 112;
const int	UI_MINIMAP		= 115;
const int	UI_QUICKSLOT	= 116;
const int	UI_SKILLENTRY	= 117;
const int	UI_CHECKBUTTON	= 118;
const int   UI_RECTANGLE	= 119;
const int	UI_TIMER		= 120;
const int	UI_SHEET		= 121;
const int	UI_IMAGELIST	= 122;

#include "UIItemMgr.h"
#include <string>

/**
 * 유저 인터페이스 컴포넌트.
 */
class UICComponent
{
public:
	UNDER_MEMORY_MGR( UICComponent );

	/**
	 * 영역을 얻는다.
	 */
	virtual const RECT*		GetRegion();

	/**
	 * 영역 설정.
	 */
	virtual void			SetRegion
	( 
		int nLeft,				///< [IN] 좌.
		int nTop,				///< [IN] 상.
		int nRight,				///< [IN] 우.
		int nBottom				///< [IN] 하.
	);

	/**
	 * 보이기 여부.
	 */
	virtual void			Show
	( 
		const BOOL bShow		///< [IN] TRUE(보이기).
	);

	/**
	 * 현재보이기 상태.
	 */
	BOOL			IsShow();

	/**
	 * 왼쪽 버튼 상태 설정.
	 */
	void			LButton( BOOL bPress );

	/**
	 * 왼쪽 버튼 상태.
	 */
	BOOL			IsLButton();

	/**
	 * 오른쪽 버튼 상태 설정.
	 */
	void			RButton( BOOL bPress );

	/**
	 * 오른쪽 버튼 상태.
	 */
	BOOL			IsRButton();
	
	/**
	 * 사용여부.
	 */
	void			Enable( BOOL bEnable );

	/**
	 * 현재 사용.
	 */
	BOOL			IsEnable();

	/**
	 * 포커스 시작.
	 */
	virtual void	FocusIn();

	/**
	 * 포커스 잃음.
	 */
	virtual void	FocusOut();

	/**
	 * 해제.
	 */
	void			Release();

	/**
	 * 초기화.
	 */
	virtual void	Cleanup();
	
	/** 
	 * 정보텍스트 설정.
	 */
	virtual void	SetInfoText( const char* szText )
	{
		//-- NULL --
	} //SetInfoText

	/** 
	 * 정보텍스트 얻기.
	 */
	virtual char*	GetInfoText()
	{
		return NULL;
	} //GetInfoText

	/**
	 * 마우스 이동.
	 */
	virtual void	MouseMove( int nMouseX, int nMouseY )
	{
		//-- NULL --
	} //MouseMove

	/** 
	 * 창 이동.
	 */
	virtual void	Move( int nOffsetX, int nOffsetY );

	/** 
	 * 팁정보.
	 */
	virtual void	DrawTipInfo()
	{
		//-- NULL --//
	};

	/**
	 * 업데이트.
	 */
	virtual void	Update( const DWORD dwTick )
	{
		//-- NULL --//
	};

	/**
	 * 그리기
	 */
	virtual	void	Draw();

	/**
	 * 마우스가 위치했는지.
	 */
	virtual BOOL	PointIn( int nX, int nY );

	/**
	 * 생성자.
	 */
	UICComponent();

	/**
	 * 소멸자.
	 */
	virtual ~UICComponent()
	{
		//--NULL--//
	}; //~UICComponent



public:
	UICComponent*	m_pOwnerComp;	///< 부모
	int				m_nType;		///< 타입
	int				m_nID;			///< 아뒤
	std::string		m_strName;		///< 이름
	RECT			m_rcRegion;		///< 영역
	BOOL			m_bFocusIn;		///< 현재 포커스
	BOOL			m_bShow;		///< 활성 상태
	BOOL			m_bLButton;		///< 왼쪽버튼
	BOOL			m_bRButton;		///< 오른쪽 버튼
	BOOL			m_bEnable;		///< 사용여부
}; //class UICComponent


/**
 * 컬러 텍스트.
 */
struct COLORTEXT
{
	std::string		strText;
	DWORD			dwColor;
	BYTE			btFontIndex;
}; //struct COLORTEXT


/**
 * 인벤토리 아이템 사각 위치.
 */
struct ItemInRect
{
	SItemData*		pItemData;
	RECT			rcItem;
}; //struct ItemInRect

typedef std::list<COLORTEXT>		LIST_COLORSTRING;

#endif //__UICComponent_Header__