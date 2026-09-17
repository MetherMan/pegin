/**
 * 아이템 관리자.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-05-10 11:24p $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIItemMgr.h $
 * 
 * 6     04-05-10 11:24p Paran
 * 
 * 8     03-05-06 6:18p Paran		상점기능 추가
 * 
 * 6     03-04-23 8:36p Paran		마법창.
 *
 * 4     03-03-26 9:19p Paran		인벤토리 어색한 부분 수정하고 체크.
 * <HR>
 *
 * @file	UIItemMgr.h
 */


#ifndef __UIItemMgr_Header__
#define __UIItemMgr_Header__


#include "UInterface_Export.h"
#include <string>
#include <list>
#include <map>

const int	SIZE_ICON_WIDTH		= 28;
const int	SIZE_ICON_HEIGHT	= 28;

const int	NULL_ITEM			= -999;
const int	ATTR_MAGICSCROLL	= 32;


using std::string;
using std::list;
using std::map;


struct SItemData
{
	UNDER_MEMORY_MGR( SItemData );

	int				nId;					///< 아이템 아이디.
	int				nType;					///< 아이템 타입.
	BYTE			btCount;				///< 아이템 개수.
	int				nSellPrice;				///< 아이템 판매가격
	SItemInfoData*	pItemInfoData;			///< 아이템 데이터.
	IW3DSprite*		pimgItem;				///< 아이템 이미지.

	/**
	 * 생성자.
	 */
	SItemData()
	:	pItemInfoData ( NULL ),
		pimgItem	  ( NULL ),
		btCount		  ( 0 )
	{
	} //SItemData

	/**
	 * 소멸자.
	 */
	~SItemData()
	{
		SAFE_RELEASE( pimgItem );
	} //SItemData
}; //struct SItemData


/**
 * 아이템 관리자 클래스.
 * @ingroup		Engine
 */
class CUIItemMgr
{
public:
	CUIItemMgr();
	~CUIItemMgr();
	
	void	Cleanup();
	
	void	SetItemInfoMgr( IW3DItemInfoMgr* pItemInfoMgr );

	BOOL	InsertItem(
		const int nId, 
		const int nType, 
		SItemInfoData *pItemInfo,
		const BYTE btCount = 0,
		const int  nPrice = -1
		);

	BOOL	InsertItemInTemp(
		const int nId, 
		const int nType, 
		SItemInfoData *pItemInfo,
		const BYTE btCount = 0,
		const int  nPrice = -1
		);

	void	SetImageUnitSize(
		int nUnitWidth, int nUnitHeight, int nListWidth, int nListHeight,
		int nSelectWidth, int nSelectHeight, int nScrollWidth, int nScrollHeight );

	/**
	 * 아이템 찾기
	 *
	 * @param	nId				[IN] 아이템 아이디
	 * @param	bTemp			[IN] 임시 저장에서 찾는다.
	 *
	 * @return	UIItemData 포인터
	 */
	SItemData*			FindItem( int nId, BOOL bTemp = FALSE );

	/**
	 * 손안 아이템
	 *
	 * @return	UIItemData 포인터
	 */
	SItemData*			GetHandInItemDat();				///< 현재 이동중인 매직

	/**
	 * 아이템을 손에 쥔다.
	 */
	void				SetHandInItemDat( SItemData* pItemData, BYTE btTo );

	/**
	 * 손에 쥔 아템 어디서..
	 */
	BYTE				FromHandItem();

	/**
	 * 손안 아이템 설정
	 *
	 * @param	nId				[IN] 아이템 아이디
	 *
	 * @return	성공/실패
	 */
	/** 삭제 ** BOOL				SetHandInItem( int nId, BOOL bTemp = FALSE );	///< 현재 이동중인 매직 설정
	**/

	void				ClearItemInTemp();
	/**
	 * 그리기
	 *
	 * @param	nX				[IN] X 좌표
	 * @param	nY				[IN] Y 좌표
	 */
	void				Draw( int nX, int nY );

	BOOL				m_btSkillInfo;
	void				SetCheckSkillInfo( BOOL btIs ){ m_btSkillInfo=btIs; };
	BOOL				GetCheckSkillInfo(  ){ return m_btSkillInfo; };
	

private:
	typedef map<int, SItemData*>			MAP_UIITEMDATA;

	IW3DItemInfoMgr*	m_pItemInfoMgr;		///<

	MAP_UIITEMDATA		m_mapItemData;		///<
	MAP_UIITEMDATA		m_mapItemDataInTemp;///<
	SItemData*			m_pHoldItemData;	///<

	BYTE				m_btHandFrom;		///<
}; //class CUIItemMgr


#endif //#ifndef __UIItemMgr_Header__
