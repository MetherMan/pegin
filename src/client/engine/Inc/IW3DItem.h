/**
 * 아이템 컴포넌트 인터페이스 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-10-04 4:13p $
 * @version	$Revision: 13 $
 *
 * <HR>
 * $Log: /Engine/Inc/IW3DItem.h $
 * 
 * 13    03-10-04 4:13p Paran
 * 
 * 12    03-06-30 5:35p Paran
 * 
 * 11    03-06-23 4:42p Paran
 * 
 * 10    03-05-13 6:03p Paran
 * 
 * 9     03-05-03 11:32a Ahastudio
 *
 * 7     03-03-26 9:19p Paran		GetType 함수 추가.
 * <HR>
 *
 * @file	IW3DItem.h
 */


#ifndef __Wind3D_Engine_Item_Header__
#define __Wind3D_Engine_Item_Header__


/**
 * 캐릭터 컴포넌트 인터페이스.
 * @ingroup		Engine
 */
class IW3DItem : public IComponent
{
public:
	/**
	 * 아이템을 만든다.
	 *
	 * @return	생성 성공 여부.
	 */
	virtual BOOL	Create(
		IW3DWorld*		pWorld,		///< [IN] 월드 객체
		const DWORD		dwId,		///< [IN] 아이템 ID
		const WORD		wType,		///< [IN] 아이템 종류
		const int		nX,			///< [IN] 맵 상의 X 좌표
		const int		nY			///< [IN] 맵 상의 Y 좌표
		) = 0;

	/**
	 * 업데이트.
	 *
	 * @param	dwTick		[IN] 경과 tick
	 *
	 * @return	계속 존재하는지 여부
	 */
	virtual BOOL	Update( DWORD dwTick ) = 0;

	/**
	 * 그리기.
	 */
	virtual void	Draw() = 0;
	
	/**
	 * 그림자그리기.
	 */
	virtual void	DrawShadow() = 0;

	/**
	 * 위치 설정.
	 */
	virtual void	SetPosition(
		const int	nX,		///< [IN] 맵 상의 X 좌표
		const int	nY		///< [IN] 맵 상의 Y 좌표
		) = 0;

	/**
	 * 아이템의 위치를 얻어옴.
	 */
	virtual Vector3	GetPosition() = 0;

	/**
	 * Picking.
	 *
	 * @param	vOrig		[IN] Picking Ray Origin
	 * @param	vDir		[IN] Picking Ray Direction
	 *
	 * @return	Picking
	 */
	virtual BOOL	Pick( const Vector3& vOrig, const Vector3& vDir ) = 0;

	/**
	 * 아이템 이름을 얻어옴.
	 */
	virtual const char*		GetName() = 0;
	
	/**
	 * 아이템 이름설정.
	 */
	virtual void			SetName( const char* szName ) = 0;
	
	/**
	 * 아이템 이름을 얻어옴.
	 */
	virtual DWORD			GetId() = 0;
	
	/**
	 * 아이템 타입을 얻어옴.
	 */
	virtual WORD			GetType() = 0;
	
	/**
	 * 아이템 이름 출력여부.
	 */
	virtual void			ShowName( BOOL bShow ) = 0;
	
	/**
	 * 라이트 적용 여부.
	 */
	virtual void			SetLighting( const BOOL bLighting ) = 0;
}; //class IW3DItem


#endif // #ifndef __Wind3D_Engine_Item_Header__
