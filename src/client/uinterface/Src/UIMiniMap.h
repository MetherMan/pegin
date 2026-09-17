/**
 * 유저 인터페이스 미니맵.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-04 4:39p $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIMiniMap.h $
 * 
 * 11    03-12-04 4:39p Paran
 * 
 * 10    03-09-17 5:08p Paran
 * 
 * 9     03-07-30 3:39p Paran
 * 
 * 8     03-06-29 4:48p Paran
 * 
 * 7     03-06-04 11:24a Paran		미니맵 상점 추가
 * 
 * 6     03-05-28 4:55p Paran		파티 위치 미니맵 표시 수정
 * 
 * 5     03-05-28 4:14p Paran		미니 맵 파티 관련 수정
 * 
 * 2     03-05-17 4:20a Paran		각 콤포넌트 분리.
 * 
 * 1     03-04-10 6:24p Paran		미니맵 추가
 * <HR>
 *
 * @file	UIMiniMap.h
 */


#ifndef __UIMiniMap_Header__
#define __UIMiniMap_Header__

#include "UIComponent.h"
#include <vector>

const int			NUM_FIREND = 7;

/**
 * 미니맵 유저 인터페이스 클래스.
 * @ingroup		Engine
 */
class UIMiniMap : public UICComponent
{
public:
	UNDER_MEMORY_MANGER( UIMiniMap );

	BOOL			Init();
	
	BOOL			LoadMap( WORD wMap,	int nMapWidth, int nMapHeight );
	BOOL			LoadMap( const char* szMapName,	int nMapWidth, int nMapHeight );

	void			SetMapSize( const int nWidth, const int nHeight );

	POINT			GetMiniMapPos( int nSrcX, int nSrcY );
	void			SetDirection( float fAngle );
	void			SetPoint( int nX, int nY, int nIndex = -1 );
	void			AddShopPoint( int nX, int nY );
	void			DeletePoint( int nIndex );		///< 프랜드 포인트 삭제.
	int				GetMapIndex();
	void			Zoom( int nZoom );				///< 확대 축소.
	void			SetTextComp( 
		const UIStaticText* pTextCompX,				///< 위치 표시를 위한 StaticText 컨트럴.
		const UIStaticText* pTextCompY				///< 위치 표시를 위한 StaticText 컨트럴.
		);
	
	void			SetAlways( const BOOL bAlways );
	void			Show( const BOOL bShow );

	void			DrawMap();
	void			Draw();

	void			Cleanup();

	UIMiniMap();
	~UIMiniMap();

private:
	IW3DSprite*		m_psprMap;			///< 미니맵 스프라이트
	IW3DSprite*		m_apsprArrow[8];		///< 화살표
	IW3DSprite*		m_psprPoint;		///< 플레이어 위치 포인트 스프라이트
	
	WORD			m_wMap;				///< 맵 번호

	std::vector<POINT>	m_aShopPoint;	///< 상점 위치 포인트 배열
	int				m_nPointX;			///< 플레이어 위치 X좌표
	int				m_nPointY;			///< 플레이어 위치 Y좌표
	BYTE			m_btDirection;		///< 방향
	int				m_nFriendX[NUM_FIREND];		///< 파티 또는 길드 위치 X좌표
	int				m_nFriendY[NUM_FIREND];		///< 파티 또는 길드 위치 Y좌표

	float			m_fHRatio;
	float			m_fVRatio;

	int				m_nZoom;			///< 줌.

	BOOL			m_bAlways;			///< 항상 사용하는 여부.
}; //class UIMiniMap


#endif //__UIMiniMap_Header__
