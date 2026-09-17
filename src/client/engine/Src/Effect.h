/**
 * Effect 처리 구현부.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-20 11:29a $
 * @version	$Revision: 25 $
 *
 * <HR>
 * $Log: /Engine/Src/Effect.h $
 * 
 * 25    03-12-20 11:29a Kjmgo
 * 보이지 않아도, 이펙트가 끝나면, 종료한다.
 * 
 * 24    03-12-20 6:21a Kjmgo
 * 메모리 관리
 * 
 * 23    03-10-27 3:24p Kjmgo
 * 코드 최적화
 * 
 * 22    03-10-15 2:16p Kjmgo
 * 
 * 21    03-05-18 7:54a Kjmgo
 * Effect를 확실하게 Release하도록 수정
 * 
 * 20    03-05-17 12:40p Ahastudio
 *
 * 8     03-02-25 11:15a Kjmgo		Magic이 CEffect를 안쓰는 구조로 바뀌어 관련 함수들 삭제.
 *
 * 5     03-02-22 1:33p Kjmgo		루프를 설정할 수 있도록 수정.
 *
 * 4     03-02-22 7:53a Kjmgo		SetPos(), SetDir() 추가.
 * <HR>
 *
 * @file	Effect.h
 */


#ifndef __Effect_Header__
#define __Effect_Header__


#include <stdio.h>
#include <string>
#include <list>
#include <map>


/**
 * 특수효과 클래스.
 * @ingroup		Engine
 */
class CEffect
{
public:
	UNDER_MEMORY_MGR( CEffect );

	enum	{ FRAME_RATE = 30 };

	/**
	 * 생성자.
	 */
	CEffect()
	{
		m_pEffect = NULL;
	} //CEffect

	/**
	 * 소멸자.
	 */
	~CEffect()
	{
		/*
			m_pEffect는 SPE Manager에서 릴리즈 한다.
		*/
	//	SAFE_RELEASE( m_pEffect );
	} //~CEffect

	/**
	 * 이펙트 생성.
	 *
	 * @return	생성 성공 여부.
	 */
	BOOL	Create(
		const char*			szName,			///< [IN] 이펙트 이름
		const Vector3&		vPos,			///< [IN] 위치
		const Quaternion&	qRot,			///< [IN] 방향
		const BOOL			bLoop,			///< [IN] 반복 여부
		IW3DSpecialEffect*  pEffect = NULL	///< [IN]
		)
	{
		Assert( pEffect );
		if( !pEffect )
		{
			return FALSE;
		} //if

		m_bLoop		= bLoop;
		m_dwTick	= 0;
		m_vPos		= vPos;
		m_qRot		= qRot;

		m_sName = szName;
		m_pEffect = pEffect;

		return TRUE;
	} //Create

	/**
	 * 이펙트를 출력한다.
	 *
	 * @return	이펙트가 계속 존재하는지 여부
	 */
	BOOL	Render(
		IW3DScene* pScene,		///< [IN] Scene 객체.
		DWORD dwTick			///< [IN] 경과 tick
		)
	{
		Assert( pScene );
		Assert( m_pEffect );

		static Frustum		frustum;
		static Vector3		vPos;
		frustum = pScene->GetFrustum();
		vPos	= m_vPos;

		if( !frustum.CheckSphere( vPos, 15.0F ) )
		{
			m_dwTick += dwTick;

			int nFrame = m_dwTick * FRAME_RATE / 1000;

			if( m_pEffect->GetMaxFrame() <= nFrame )
			{
				if( m_bLoop )
				{	// 반복시킴.
					m_dwTick = 0;
				} 
				else
				{
					return FALSE;
				} //if
			} //m_dwTick * FRAME_RATE / 1000 

			return TRUE;
		} //if

		static Matrix4		matWorld;
		matWorld.SetWorld( m_vPos, m_qRot );

		if( !m_pEffect->Render( m_dwTick * FRAME_RATE / 1000, matWorld ) )
		{	// 이펙트가 끝까지 진행된 경우..
			if( m_bLoop )
			{	// 반복시킴.
				m_dwTick = 0;
				m_pEffect->Render( m_dwTick * FRAME_RATE / 1000, matWorld );
			}
			else
			{	// 이펙트 삭제로 리턴.
				return FALSE;
			} //if..else..
		} //if

		m_dwTick += dwTick;

		return TRUE;
	} //Render

	/**
	 *
	 */
	IW3DSpecialEffect* GetEffect()
	{
		Assert( m_pEffect );
		return m_pEffect;
	} //GetEffect

	std::string&	GetName()
	{
		return m_sName;
	} //GetName
private:
	std::string				m_sName;
	BOOL					m_bLoop;			///< 반복할지 여부
	DWORD					m_dwTick;			///< 내부 누적 tick
	Vector3					m_vPos;				///< 위치
	Quaternion				m_qRot;				///< 방향
	IW3DSpecialEffect*		m_pEffect;			///< 이펙트 Render
}; //class CEffect


#endif // #ifndef __Effect_Header__
