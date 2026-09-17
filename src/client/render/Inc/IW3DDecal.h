/**
 * 데칼 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-10-15 4:10p $
 * @version	$Revision: 1 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DDecal.h $
 * 
 * 1     03-10-15 4:10p Kjmgo
 * 
 * <HR>
 *
 * @file	IW3DDecal.h
 */


#ifndef __Wind3D_Render_IW3DDecal_Header__
#define __Wind3D_Render_IW3DDecal_Header__

/**
 *
 */
class IW3DDecal : public IComponent
{
public:
	/**
	 *	데칼 초기화
	 */
	virtual void Init( IW3DTerrain* pTerrain ) = 0;
	
	/**
	 * 데칼 스크립트 로드
	 */
	virtual BOOL Load( const char* szFile, int nRepeat = 1 ) = 0;

	/**
	 * 데칼 초기화, 내부 변수를 초기화 시킨다. 
	 */
	virtual void DecalInit() = 0;

	/**
	 * 그린다.
	 */
	virtual void Draw() = 0;

	/**
	 * 업데이트 
	 * 끝났는지의 여부, 자동적으로 Disappear을 호출하고, 이것마저 끝난 후에
	 * FALSE를 리턴... ( 무한 반복이면, 계속 True를 리턴 )
	 *
	 */
	virtual BOOL Update( const Vector3& vPos, DWORD dwTick ) = 0;

	/**
	 *	 천천히 사라지면서( 알파 블렌딩 ) 끝낸다. ( 자체 소멸?;;; )
	 */
	virtual void Disappear() = 0;
	
}; //class IW3DDecal

#endif //#ifndef __Wind3D_Render_IW3DDecal_Header__