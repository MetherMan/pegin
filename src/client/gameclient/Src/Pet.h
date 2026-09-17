/**
 * 펫
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-04-03 8:12p $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /GameClient/Src/Pet.h $
 * 
 * 7     04-04-03 8:12p Paran2
 * 
 * 6     04-04-02 7:59p Paran2
 * 
 * 5     04-03-26 5:50p Paran2
 * 
 * 4     04-03-24 6:45p Paran2
 * 
 * 3     04-03-23 11:40a Paran
 * 
 * 2     04-02-27 9:43p Kjmgo
 * 펫추가 ^.^/
 * 
 * 1     04-02-27 9:02p Kjmgo
 * 
 *  
 * <HR>
 *
 * @file	Pet.h
 */

#ifndef __Pet_Header__
#define __Pet_Header__

#include "may.h"

#define PET_MAX_TYPE				4
#define LENGTH_PETNAME				17

/**
 * Class CPet
 */
class CPet : public CHelper
{
public:
	MEMORY_UNDER_MGR( CPet );

	CPet();
	virtual ~CPet(){};

	BOOL		SetInfo( const WORD wType );
	void		SetName( const char* szName );
	
	BOOL		IsEndAction();

	void		Attack( IW3DCreature* pTarget );

	void		StateAttack();
	
	void		DrawName( IW3DRectangle* pRectangle );
	void		Draw();

private:
	SAction*		m_pAnimation;				///< 현재 애니메이션 
	SAction*		m_pStopAni;					///< 정지(이동) 동작
	SAction*		m_pAttackAni;				///< 공격 동작

	char			m_szName[LENGTH_PETNAME];	///< Pet's Name
}; //Helper













/**
 *
 */
/*class CPet
{
public:
	UNDER_MEMORY_MGR( CPet );

	// Constructor And Destructor 
		CPet();
		~CPet();
	// ~Constructor And Destructor

	BOOL Init( IW3DWorld* pWorld, IW3DCreature* pCreature, const BYTE btType, const std::string& sName );

	BOOL Update( const DWORD dwTick );
	BOOL Render();

	void SetCharacterPos( const Vector3& vPos );
	void SetCharacterState( const int nState );

	void Attack( IW3DCreature* pTarget );
	void Move();
	void Stop();

	void SetData( void* pData );
	void GetData( void* pData );

private:

	BOOL MoveTo();

	// 상태
	void StateAttack();
	void StateMove();
	void StateStop();
	// ~상태

	BOOL IsEndAction();

private:
	// 능력치
	int			m_nLevel;		// 팻 레벨 
	int			m_nExp;			// 현재 경험치
	int			m_nNeedExp;		// 목표 경험치
	BYTE		m_btType;		// 펫 타입
	WORD		m_wAngerGage;	// 분노 게이지
	// ~능력치

	// 위치 정보
	BEZIER		m_Bezier;
	Vector3		m_vPos;			// 위치
	Vector3		m_vChaPos;		// 캐릭터 위치
	Quaternion	m_qRot;			// 방향
	// ~위치 정보

	std::string m_sName;			// Pet
	DWORD		m_dwId;

	IW3DWorld*	m_pWorld;		// World
	IW3DModel*	m_pModel;		// Model
	SAction*	m_pAnimation;	// 현재 애니메이션 
	SAction*	m_pStopAni;		// 정지(이동) 동작
	SAction*	m_pAttackAni;	// 공격 동작

	IW3DCreature* m_pCreature;	// 크리쳐
	IW3DCreature* m_pTarget;	// 공격대상
	DWORD		m_dwTicks;		// 누적 틱
	int			m_nState;		// 상태
	int			m_nCharacterState;	// 캐릭터 상태
	float		m_fSpeed;		// 속도
}; //class CPet
*/

#endif //#ifndef __Pet_Header__