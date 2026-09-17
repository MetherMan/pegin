/**
 * 탈것에 대한 처리 ( 말이나 기타 등등  )
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-03-12 6:42p $
 * @version	$Revision: 8 $
 *
 * <HR>
 * $Log: /GameClient/Src/Vehicle.h $
 * 
 * 8     04-03-12 6:42p Sckyj
 * 운영자 작업중
 * 
 * 7     03-12-22 5:00p Sckyj
 * 2D, 3D sound 분리
 * 
 * 6     03-11-14 9:04a Kjmgo
 * 
 * 5     03-10-02 10:33p Kjmgo
 * 캐릭터 State 권한 이전 작업 : 1차 백업
 * 
 * 4     03-09-14 2:49a Kjmgo
 * 
 * 3     03-09-12 6:42p Kjmgo
 * Vehicle 관련 수정 및 추가
 * 
 * 2     03-09-12 11:23a Kjmgo
 * 
 * <HR>
 *
 * @file	Vehicle.h
 */

#ifndef __Vehicle_Header__
#define __Vehicle_Header__

#include "Global.h"
#include "Game.h"
#include <string>

//#define SHOW_BOUNDBOX

class CVehicle;

/**
 * 탈것에 대한 처리
 */
class CVehicle
{
public:
	UNDER_MEMORY_MGR( CVehicle );

	/**
	 * 생성자
	 */
	CVehicle();

	/**
	 * 소멸자
	 */
	~CVehicle();

	/**
	 * 
	 */
	BOOL	Create( IW3DWorld* pWorld, const DWORD dwId, const void* pData );

	/**
	 * Update
	 */
	BOOL	Update( const DWORD dwTick );

	/**
	 * Draw
	 */
	void	Draw();

	/**
	 * 그림자
	 */
	void	DrawShadow();

	/**
	 *
	 */
	void	SetLighting( const BOOL bLighting );


	/**
	 *
	 */
	CVehicleAction*	GetActionMgr()
	{
		Assert( m_pActionMgr );
		return m_pActionMgr;
	} //GetActionMgr


	/**
	 * 위치 설정.
	 */
	void		SetPosition( const Vector3& vPos )
	{
		m_vGoalPos = m_vPos = vPos;
	} //SetPosition

	/**
	 * 방향 설정.
	 */
	void		SetQuaternion( const Quaternion& qRot )
	{
		m_qRot = qRot;
	} //SetDirection

	/**
	 *
	 */
	DWORD		GetType()
	{
		if( m_pActionMgr )
		{
			return m_pActionMgr->GetType();
		} //if

		return m_dwType;
	} //GetType

	/**
	 *
	 */
	void		SetTime( DWORD dwTime )
	{
		m_pActionMgr->SetTime( dwTime );
	} //SetTime

	/**
	 *
	 */
	IW3DModel*	GetModel()
	{
		m_pModel->SetWorld( m_vPos, m_qRot );
		m_pModel->SetAction( m_pAction );
		return m_pModel;
	} //GetModel


private:
	IW3DModel*	m_pModel;	///< Model
	SAction*	m_pAction;	///< Action

//	BOOL		m_bSex;		///< 남성용인지, 여성용인지의 여부
							///< 0 =  남성, 1 = 여성 

	UINT		m_nSex;		///< 0 : 남성, 1 : 여성, 2 : 운영자

	CVehicleAction*		m_pActionMgr;

	IW3DWorld*			m_pWorld;			///< 게임 월드
	IW3DModel*			m_pBoundBox;		///< 바운드 박스.
	IW3DSoundObject3D*	m_pSound;			///<
	BOOL				m_bLighting;		///< 빛 적용 여부

	DWORD				m_dwId;				///<
	DWORD				m_dwType;			///<
	std::string			m_sName;			///<
	Vector3				m_vGoalPos;			///<
	Vector3				m_vPos;				///<
	Quaternion			m_qRot;				///<
	float				m_fOffset;			///<

	IW3DScene*			m_pScene;			///< 폰트.
	IW3DRectangle*		m_pRectangle;		///< 사각형.
	Vector3				m_vTextPos;			///< 텍스트 출력 좌표.

	Matrix4				m_matBoundBox;		///< 바운드 박스용 World Matrix

	int					m_nState;			///< 현재 상태.

	DWORD				m_dwTick;			///<
	DWORD				m_dwEvent;			///<

	BOOL				m_bUseVehicle;		///< Vehicle 사용 여부
	BOOL				m_bView;			///< 보이는지의 여부
	BOOL				m_bSemiMove;		///< 공격후 이동 여부 
	BOOL				m_bAttackMove;		///< 공격하기 위해 이동하는가?

	//-=-=-=-=-=-=-=-=-= Load -=-=-=-=-=-=-=-=-=-=//

	IW3DModel*	LoadModel( DWORD dwType );
	char*		GetModelName( DWORD dwType );
	char*		GetActionName();
	void		SetState( int nState );

}; //class CVehicle

#endif // #ifndef __Creature_Header__
