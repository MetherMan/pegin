/**
 *  기능조물 인터페이스 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-05-25 3:00a $
 * @version	$Revision: 5 $
 *
 * <HR>
 * $Log: /Engine/Inc/IW3DFuncEntity.h $
 * 
 * 5     04-05-25 3:00a Paran2
 * 
 * 4     04-05-25 12:59a Paran2
 * 
 * 3     04-03-17 10:48p Paran
 * 
 * 2     04-03-10 6:17p Paran			기능조물 추가
 * 
 * <HR>
 *
 * @file	IW3DFuncEntity.h
 */


#ifndef __Wind3D_Engine_FuncEntity_Header__
#define __Wind3D_Engine_FuncEntity_Header__

class IW3DFuncEntity : public IComponent
{
public:
	enum { FUNCENTITY_NONE = 0, FUNCENTITY_ENCAMP = 1, FUNCENTITY_GATE = 2 };

	virtual void		SetEntity( DWORD dwId, BYTE byType, IW3DModel* pModel, const Vector3& vPos ) = 0;
	virtual BOOL		LoadAnimation( const char* szPath, const char* szAniName ) = 0;

	virtual DWORD		GetId() = 0;
	virtual BYTE		GetType() = 0;
	virtual DWORD		GetParam() = 0;
	virtual Vector3*	GetPosition() = 0;

	virtual void		SetName( const char* szName ) = 0;
	virtual void		SetLighting( const BOOL bLighting ) = 0;

	virtual BOOL		Pick( const Vector3& vOrg, const Vector3& vDir ) = 0;
	virtual void		SetCollisionEntity( BOOL bCollision ) = 0;
	virtual BOOL		CollisionPoint( const Vector3& vPos ) = 0;

	virtual BOOL		Update( DWORD dwTick ) = 0;
	virtual void		DrawShadow() = 0;
}; //class IW3DFuncEntity

#endif //__Wind3D_Engine_FuncEntity_Header__