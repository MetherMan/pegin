/**
 * 크리쳐 베이스 클래스 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란
 * @date	$Date: 04-09-24 4:45p $
 * @version	$Revision: 154 $
 *
 * <HR>
 * $Log: /GameClient/Src/Creature.h $
 * 
 * 154   04-09-24 4:45p Paran
 * 
 * 137   04-03-12 2:12p Paran		Collision & Picking
 * 
 * 136   04-02-27 9:43p Kjmgo		펫추가 ^.^/
 * 
 * 135   04-02-03 6:35p Kjmgo		길드 이름 움하하!
 * 
 * 133   04-01-14 11:47p Kjmgo		공성전시 최소프레임유지에 필요한 코드 추가
 * 
 * 130   04-01-13 8:14p Kjmgo		Creature및IW3DMagic을 포인터레퍼런스로 관리하도록 수정
 * 
 * 120   04-01-06 12:01a Kjmgo		공속 추가 
 * 
 * 117   04-01-03 11:10a Kjmgo		SetVisible 추가
 * 
 * 116   03-12-31 3:18p Kjmgo		마법쪽 버그 수정 
 * 
 * 115   03-12-31 2:43p Paran		스킬
 * 
 * 112   03-12-24 10:30a Kjmgo		스킬적용
 * 
 * 111   03-12-22 5:00p Sckyj		2D, 3D sound 분리
 * 
 * 109   03-12-22 6:00a Kjmgo		버그 수정
 * 
 * 108   03-12-20 6:18a Kjmgo		메모리 관리
 * 
 * 107   03-12-17 7:50a Kjmgo		선택 유지 구현 ( 選擇[せんたく] 維持[いじ] )
 * 
 * 106   03-12-14 6:38p Kjmgo		길드 이름 그리기 추가
 * 
 * 103   03-12-10 8:32p Kjmgo		PickingEffect를 Character로 옮김
 * 
 * 102   03-12-09 4:01p Paran		아이템 데이터 수정
 * 
 * 101   03-11-18 9:52p Kjmgo		GetHeight 관련 함수 수정및, 추가
 * 
 * 100   03-10-23 10:04p Paran		공격효과부분 수정.
 * 
 * 99    03-10-16 4:41p Sckyj		sound재생관련 코드 수정
 * 
 * 98    03-10-15 4:50p Kjmgo		모델을 로드할때, 따로, 피봇과 반지름을 계산하지 않습니다. 단, 바운드박스는 계산합니다.
 * 
 * 97    03-10-11 12:08a Kjmgo		AddMagic수정 및, CCreature::~CCreature(); 쓰던거 삭제
 * 
 * 93    03-10-02 10:33p Kjmgo		캐릭터 State 권한 이전 작업 : 1차 백업
 * 
 * 90    03-10-02 4:34a Kjmgo		AABB충돌 추가( 아직 미완성 )
 * 
 * 88    03-10-01 6:40p Kjmgo		Skill적용
 * 
 * 86    03-10-01 11:38a Kjmgo		최상위 Creature에 ShowDamageForTarget 함수 추가 및, 하위 클래스 함수 추가
 * 
 * 85    03-09-30 6:07p Kjmgo		이동중 말을 탔을때, 정지 할때까지 말이 보이지 않던 버그 해결
 * 
 * 84    03-09-23 11:20a Kjmgo		Resurrect함수 추가
 * 
 * 83    03-09-22 2:01p Kjmgo		Creature 색상 설정및 알파값 설정 작업(완료)
 * 
 * 82    03-09-18 5:15p Kjmgo		Magic에서 Bone을 사용할수 있게 수정
 * 
 * 81    03-09-18 10:58a Kjmgo		데미지 처리 관련 수정
 * 
 * 79    03-09-13 1:26p Kjmgo		활을 비롯한 몇가지 옵션 추가
 * 
 * 78    03-09-12 6:42p Kjmgo		Vehicle 관련 수정 및 추가
 * 
 * 73    03-09-09 5:45a Kjmgo		몬스터 속도 수정
 * 
 * 71    03-09-08 8:20p Kjmgo		동작~ 작업
 * 
 * 68    03-08-31 6:18p Kjmgo		Character Action 관련 추가
 * 
 * 56    03-08-14 10:03a Kjmgo		마법스크립트 두개로 분리
 * 
 * 54    03-08-13 10:11a Kjmgo		Vehicle 부분 수정
 * 
 * 51    03-08-10 6:59p Admin		말 타고내리기 적용
 * 
 * 46    03-07-09 9:46p Kjmgo		Vehicle 추가
 * 
 * 40    03-06-27 5:30p Kjmgo		Assert처리
 * 
 * 32    03-06-23 5:53p Paran		바운드 박스 처리.
 * 
 * 28    03-06-15 4:44p Paran		STATE_MAGIC 추가.
 * 
 * 27    03-06-14 11:03a Paran		Skill zzang~
 * 
 * 25    03-06-12 2:05p Paran		매직 관련작업
 * 
 * 24    03-06-06 9:58a Kjmgo		Shadow Dummy 추가
 * h
 * 23    03-06-05 8:20p Paran		동작분리중...
 * 
 * 22    03-05-26 9:06p Paran		사용스킬 별로 모델 사용.
 * 
 * 21    03-05-16 11:03a Paran		이름 출력관련 수정.
 * 
 * 20    03-05-15 9:04p Paran		파티관련 이름출력.
 * 
 * 18    03-05-08 1:28a Paran		말풍선 작업.
 * 
 * 16    03-04-24 11:34a Paran		몬스터 이동 옵셋 수정.
 *
 * 14    03-04-23 7:50a Ahastudio	동작들을 모두 virtual로 변경. 새로운 Attack 추가.
 *
 * 13    03-04-18 3:09p Ahastudio	SetState를 virtual 함수로 변경.
 * <HR>
 *
 * @file	Creature.h
 */

 
#ifndef __Creature_Header__
#define __Creature_Header__

#include "Global.h"
#include "Game.h"
#include "Vehicle.h"
//#include "SoundSync.h"
#include <string>

//#define SHOW_BOUNDBOX
#define DAMAGE_REACTION			0.3F
#define DAMAGE_REACTIONTIME		100


typedef std::map<string, IW3DModel*>		BoundBoxMap;
static BoundBoxMap		gs_mapBoundBoxModel;		///< 모델 데이터.
static DWORD			g_dwPickingEffect;	///< 바닥 피킹 이펙트 ID

class CCreature;

CCreature*	MakeVehicle();

/**
 * 크리쳐 베이스 클래스 헤더 파일.
 * @ingroup		Engine
 */
class CCreature : public IW3DCreature
{
public:
	/**
	 * 생성자.
	 */
	CCreature()
	:	m_pWorld		( NULL ),
		m_pSound		( NULL ),
		m_pShadowDummy	( NULL ),
		m_pVehicle		( NULL ),
		m_pRectangle	( NULL ),
		m_pScene		( NULL ),
		m_pTargetEffect	( NULL ),
		m_bEndMagic		( TRUE ),
		m_bVisible		( TRUE ),
		m_bLighting		( TRUE ),
		m_bFollow		( FALSE ),
		m_bShow			( FALSE ),
		m_bUseVehicle	( FALSE ),
		m_bShowName		( FALSE ),
		m_bSemiMove		( FALSE ),
		m_bAttackMove	( FALSE ),
		m_nSkill		( 0 ),
		m_dwFollowTarget( 0 ),
		m_nLayer		( 0 ),
		m_dwTick		( 0 ),
		m_btDistLevel	( 0 ),
		m_btGhost		( 0 ),
		m_btGreen		( 255 ),
		m_btRed			( 255 ),
		m_btBlue		( 255 ),
		m_btAlpha		( 255 ),
		m_nState		( STATE_STOP ),		
		m_dwColor		( 0xFFFFFFFF ),
		m_bStatic		( FALSE ),
		m_dwColorName	( 0xFFD1D1D1 )
	{
		m_nHp[0] = 0;
		m_nHp[1] = 0;
	} //CCreature

	/**
	 * 소멸자.
	 */
	virtual ~CCreature()
	{ 
		if( m_dwTargetColor == 0xFFFF0000 )
		{
			g_pGUI->ShowTargetInfo( 0 );
		} //if

		if( m_pTargetEffect )
		{
			m_pWorld->UnloadEffect( m_pTargetEffect, "mob_tageting.wed" );
		} //if

		SAFE_DELETE( m_pVehicle );
		SAFE_RELEASE( m_pSound );
		SAFE_RELEASE( m_pRectangle );

		g_listCollisionCreature.remove( this );
			
		ReleaseOwnerMagic();
		ReleaseTargetMagic();
	} //~CCreature

	/**
	 *
	 */
	void	ReleaseTargetMagic()
	{
		if( !m_mapMagic.empty() )
		{
			MagicMap::iterator itor = m_mapMagic.begin();
			MagicMap::iterator itorNext;
			IW3DMagic* pMagic = NULL;

			while( itor != m_mapMagic.end() )
			{
				itorNext = itor;
				itorNext++;

				pMagic = itor->second;

				if( pMagic )
				{
					pMagic->DieTarget();
				} //if

				m_mapMagic.erase( itor );
				itor = itorNext;
			} //while
		} //if
	} //ReleaseTargetMagic

	/**
	 *
	 */
	void	ReleaseOwnerMagic()
	{
		if( !m_mapOwnerMagic.empty() )
		{
			MagicMap::iterator itor = m_mapOwnerMagic.begin();
			MagicMap::iterator itorNext;
			IW3DMagic* pMagic = NULL;
			while( itor != m_mapOwnerMagic.end() )
			{
				itorNext = itor;
				itorNext++;

				pMagic = itor->second;

				if( pMagic )
				{
					pMagic->DieAttacker();
				} //if

				m_mapOwnerMagic.erase( itor );
				itor = itorNext;
			} //while
		} //if
	} //ReleaseOwnerMagic

	/**
	 * 크리쳐 생성.
	 */
	virtual BOOL	Create(
		IW3DWorld*		pWorld,		///< [IN] 월드 객체
		const DWORD		dwId,		///< [IN] 크리쳐 ID
		const void*		pData		///< [IN] 크리쳐 생성 정보
		)
	{
		m_pWorld = pWorld;
		m_pScene = g_pScene;
		Assert( m_pScene );

		m_pRectangle = MAKE_COMPONENT( Rectangle );
		Assert( m_pRectangle );

		m_mapMagic.clear();
		m_mapOwnerMagic.clear();
		
		// Load Target Effect
		m_bViewTargetEffect = FALSE;
		m_dwEffectTime = 0;
		m_pTargetEffect = m_pWorld->LoadEffect( "mob_tageting.wed" );

		return TRUE;
	} //Creates
	
	/**
	 * 이동 완료 확인.
	 *
	 * @return 완료시 TRUE
	 */
	BOOL		IsGoal()
	{
		if( m_vGoalPos != m_vPos )
		{
			return FALSE;
		} //if
		return TRUE;
	} //IsGoal

	/**
	 * 이동 목표 설정.
	 */
	void		SetGoalPosition( const Vector3& vPos )
	{
		m_vLastGoal = vPos;
		m_vGoalPos = vPos;
		Move();
	} //SetGoalPosition

	/**
	 * 위치 설정.
	 */
	void		SetPosition( const Vector3& vPos )
	{
		m_vLastGoal = m_vGoalPos = m_vPos = vPos;
	} //SetPosition

	/**
	 *
	 */
	float		GetHeight()
	{
		SetLayer();

		float fHeight = m_pWorld->GetHeight( m_vPos );

		SetLayer( m_pWorld->GetTerrain()->GetLevel() );

		return fHeight;
	} //GetHeight

	/**
	 *
	 */
	Vector3		GetGoalPosition()
	{
		return m_vGoalPos;
	} //

	/**
	 * 캐릭터의 위치를 얻어옴.
	 */
	Vector3		GetPosition()
	{
		return m_vPos;
	} //GetPosition

	/**
	 * 케릭터 방향을 얻어옴.
	 */
	Vector3		GetDirection()
	{
		return m_qRot.GetDirection();
	} //GetDirection

	/**
	 * 방향 설정.
	 */
	void		SetDirection( const Vector3& vDir )
	{
		if( m_bStatic )
		{
			return;
		} //if

		m_qRot = Vector3( vDir.x, vDir.y, 0.0F ).GetQuaternion();
		if( m_qRot.w > -0.0f || m_qRot.x > -0.0f || m_qRot.y > -0.0f || m_qRot.z > -0.0f )
			int aa=0;
	} //SetDirection

	void		SetDirection( const int nAngle )
	{
		m_bStatic = TRUE;
		m_qRot.SetRotationZ( ANGLE( nAngle ) );
		m_qRot		= Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion() * m_qRot;
	} //SetDirection

	/**
	 * 방향 설정.
	 */
	void		SetQuaternion( const Quaternion& qRot )
	{
		m_qRot = qRot;
	} //SetDirection

	Quaternion	GetQuaternion()
	{
		return m_qRot;
	} //GetQuaternion

	/**
	 * 캐릭터 ID를 얻어옴.
	 */
	DWORD		GetId()
	{
		return m_dwId;
	} //GetId

	/**
	 * 캐릭터 이름을 얻어옴.
	 */
	const char*		GetName()
	{
		return m_sName.c_str();
	} //GetName

	/**
	 * Set Color of Character Name 
	 */
	void			SetNameColor( const DWORD dwColor )
	{
		m_dwColorName = dwColor;
	} //SetNameColor
	
	/**
	 * 이름보이기.
	 *
	 * @param	보이기선택
	 */
	void			VisibleName( BOOL bVisible )
	{
		m_bShowName = bVisible;
	} //VisibleName

	/**
	 * 배역설정.
	 */
	virtual void	SetCastContent( 
		const WORD wShop,					///< [IN] 상점 번호.
		const WORD wQuest,					///< [IN] 퀘스트 번호.
		const BYTE btHaveStorage,			///< [IN] 창고 유무.
		const BOOL bGuildMgr				///< [IN] 단 관리
		)
	{
		//-- NULL --
	} //SetCastContent

	/**
	 * 배역 얻기.
	 */
	virtual void	GetCastContent( 
		WORD& wShop,					///< [OUT] 상점 번호.
		WORD& wQuest,					///< [OUT] 퀘스트 번호.
		BYTE& btHaveStorage,			///< [OUT] 창고 유무.
		BYTE& btGuildMgr				///< [OUT] 단 관리.
		)
	{
		//-- NULL --
	} //GetCastContent

	/**
	 * 케릭터 대화 내용.
	 */
	virtual void	SetTalkText( char* szText )
	{
		//-- NULL --
	}

	virtual void	SetTalkText( const char* szText )
	{

	}

	void	SetShadowDummy( IW3DModel* pModel )
	{
		Assert( pModel );
		m_pShadowDummy = pModel;
	}
	
	/**
	 * 캐릭터의 상태를 설정함
	 */
	virtual void	SetState( int nState )
	{
		guard( CCreature::SetState );

		m_nState = nState;

		switch( nState )
		{
		case STATE_STOP:
			break;

		case STATE_MOVE:
			break;

		case STATE_ATTACK:
			m_dwTick  = 0;
			m_dwEvent = 0;
			break;
			
		case STATE_MAGIC:
			m_dwTick  = 0;
			m_dwEvent = 0;
			break;

		case STATE_DEAD:
			m_dwTick  = 0;
			m_dwEvent = 0;
			break;

		default:
			break;
		} //switch

		unguard
	} //SetState

	/**
	 * 캐릭터의 상태를 얻어옴.
	 */
	int 	GetState()
	{
		return m_nState;
	} //GetState

	/**
	 * 공격방법설정.
	 */
	virtual void	SetAttackMode(
		const int nSkillItem			///< [IN] 스킬.
		)
	{
		Assert( m_pWorld );
		IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();
		if( !pItemInfoMgr )
		{
			Assert( !"아이템 메니져 실패" );
			return;
		} //if

		SSkillInfoData*		pSkillData = pItemInfoMgr->GetSkillInfo( nSkillItem );
		if( !pSkillData )
		{
			Assert( !"아이템 데이터 실패" );
			return;
		} //if
		
		if( IS_SATTACK( pSkillData) )			{ m_nSkill = SKILL_SHORT; }//g_pGUI->SetSaveQuickSlot( m_nSkill ); }
		else if( IS_LATTACK( pSkillData) )		{ m_nSkill = SKILL_LONG; }//g_pGUI->SetSaveQuickSlot( m_nSkill ); }
		else if( IS_BMAGIC( pSkillData) )		{ m_nSkill = SKILL_BLACK; }//g_pGUI->SetSaveQuickSlot( m_nSkill ); }
		else if( IS_WMAGIC( pSkillData) )		{ m_nSkill = SKILL_WHITE; }//g_pGUI->SetSaveQuickSlot( m_nSkill ); }
		{
//			if( pItemData->m_MagicKind == 1 )			m_nSkill = SKILL_WHITE;
//			else if( pItemData->m_MagicKind == 2 )		m_nSkill = SKILL_BLACK;
		} //if..else..
	} //SetAttackMode
	
	/**
	 * 사용매직정보 설정.
	 */
	virtual void	SetMagicInfo( const BYTE btMagicType, const BYTE btMagicPickType )
	{
		//-- NULL --	
	} //SetMagicInfo

	/**
	 * 사용중인 스킬적용정보.
	 */
	virtual BOOL	GetApplyInfoSkill( BYTE* btSkillType, BYTE* btTaget )
	{
		return TRUE;
	} //GetApplyInfoSkill
	
	/**
	 * 다른 크리쳐를 공격한다.
	 */
	void	Attack(
		IW3DCreature*	pTarget		///< [IN] 공격 대상
		)
	{
		//-- NULL --
	} //Attack	

	void	Ghost( BOOL bGhost )
	{
		m_btGhost = bGhost;
		if( bGhost )
		{
			Resurrect();
			SetAlpha( 100 );
			SetColor( 50, 50, 50 );
		}
		else
		{
			SetAlpha();
			SetColor( 255, 255, 255 );

			if( g_dwPlayerId == m_dwId )
			{
				g_pGUI->PlayerLive( TRUE );
			} //if
		} //if..else
	} //Ghost

	/**
	 *
	 */
	void	Hook()
	{
		//-- NULL --
	} // Hook
	
	/**
	 * 고스트 확인.
	 */
	BOOL	IsGhost()
	{
		return m_btGhost;
	} //IsGhost
	
	/**
	 *
	 */
	virtual BOOL	IsDead()
	{
		if( m_nState == STATE_DEAD )
		{
			return TRUE;
		} //if

		return FALSE;
	} //IsDeads

	/** 
	 * 공격을 받는다.
	 */
	virtual void	Damage( 
		BOOL			bBegin,				///< [IN] 공격 시작, 끝
		IW3DCreature*	pAttacker = NULL	///< [IN] 공격자.
		)
	{
		//-- NULL --
	} //Damage
	
	/*
	 * 공격받은 수치설정.
	 */
	virtual void	Damage( 
		const WORD wDamage,			///< [IN] 데미지 수치.
		const BYTE btCritical		///< [IN] 크리티컬 데미지.
		)
	{
		//-- NULL --
	} //Damage

	/**
	 *
	 */
	virtual SW3DDamage* GetDamage()
	{
		return NULL;
	} //GetDamage

	/**
	 * HP 설정.
	 */
	void		SetHp( const int nHp, const int nTotHP )
	{
		m_nHp[0] = nHp;

		if( nTotHP > 0 )
		{
			m_nHp[1] = nTotHP;
		} //if
	} //SetHp

	/**
	 * HP 얻어오기.
	 */
	void		GetHp( int& nHP, int& nTotHP )
	{
		nHP		= m_nHp[0];
		nTotHP	= m_nHp[1];
	} //GetHP

	/**
	 * 일반마법 사용한다.
	 */
	virtual void		CastMagic()
	{
		//-- NULL --
	} //CastMagic

	/** 
	 * 마법추가.
	 */
	virtual void AddMagic( const WORD wMagic, const WORD wID, IW3DCreature* pTarget, BOOL bCompulsion = FALSE )
	{
		//-- NULL --
	} //AddMagic

	/**
	 * 마법시작.
	 */	
	virtual void StartMagic( DWORD dwID, DWORD dwMagicId, BOOL bTargetMonster, BOOL bHero )
	{
		//-- NULL --
	} //StartMagic

	void SetLayer( BYTE nLayer )
	{
		m_nLayer = nLayer;
	} //SetLayer

	void	SetLayer()
	{
		m_pWorld->SetLayer( m_nLayer );
	} //SetLayer

	BYTE	GetLayer()
	{
		return m_nLayer;
	} //GetLayer

   /**
	* 빛 적용 여부를 설정한다.
	* 빛을 적용하지 않으면 원색 그대로 출력된다.
	* 즉, 주변의 오브젝트에 비해 상대적으로 밝게 보인다.
	* 크리쳐를 선택하거나 하는 경우에 사용하면 효과적이다.
	*
	* @param	bLighting	[IN] 빛 적용 여부.
	*/
	void	SetLighting( const BOOL bLighting )
	{
		if( m_btGhost )
		{
			return;
		}//if
		m_bLighting = bLighting;
	} //SetLighting

	/**
	 *
	 */
	void	SetTarget( BOOL bPlayer, BOOL bTarget = TRUE )
	{
		m_bViewTargetEffect = bTarget;

		if( bPlayer )
		{
			m_dwTargetColor = 0xFF000000 | 255 << 16 | 0 << 8 | 0;
		} 
		else
		{
			m_dwTargetColor = 0xFF000000 | 255 << 16 | 255 << 8 | 0;
		}//if
	} //SetTarget

	/**
	 *
	 */
	void	SetFollowTarget( IW3DCreature* pFollow )
	{
		if( pFollow <= 0 )
		{
			m_dwFollowTarget = 0;
			return;
		} //if
		
		m_dwFollowTarget = pFollow->GetId();
	} //SetFollowTarget


	/**
	 *
	 */
	void	SetMagic( IW3DMagic& pMagic )
	{
		IW3DMagic* p = &pMagic;
		MagicMap::iterator itor = m_mapOwnerMagic.find( p->GetID() );

		if( itor != m_mapOwnerMagic.end() )
		{
			Assert( !"이미 등록되어 있는 마법을 추가하려 합니다." );
		} //if

		m_mapOwnerMagic.insert( std::make_pair( p->GetID(), p ) );
	} //SetMagic

	
	/**
	 *
	 */
	virtual void	EndMagic( DWORD dwID )
	{
		m_bEndMagic = TRUE;

		MagicMap::iterator itor = m_mapOwnerMagic.find( dwID );

		if( itor != m_mapOwnerMagic.end() )
		{
			IW3DMagic* pMagic = itor->second;
		
			if( pMagic )
			{
				pMagic->DieAttacker();
			} //if

			m_mapOwnerMagic.erase( itor );
		}
		else
		{
			Assert( !"등록되어 있지않은 마법을 끝내려 합니다." );
		} //if
	} //EndMagic
	
	/**
	 *
	 */
	void	SetMagicTarget( IW3DMagic& pMagic )
	{
		IW3DMagic* p = &pMagic;
		MagicMap::iterator itor = m_mapMagic.find( p->GetID() );

		if( itor != m_mapMagic.end() )
		{
			Assert( !"이미 있는 마법입니다." );
		} //if

		m_mapMagic.insert( std::make_pair( p->GetID(), p ) );		
	} //SetMagicTarget

	/**
	 *
	 */
	void	EndMagicTarget( DWORD dwID )
	{
		MagicMap::iterator itor = m_mapMagic.find( dwID );

		if( itor != m_mapMagic.end() )
		{
			IW3DMagic* pMagic = itor->second;

			if( pMagic )
			{
				pMagic->DieTarget();
			} //if
		
			m_mapMagic.erase( itor );
		} 
		else
		{
			Assert( !"등록되어 있지 않는 마법입니다." );
		} //if
	} //EndMagicTarget

	/**
	 * 그리기.
	 */
	virtual void	Draw()
	{
	} //Draw

	/**
	 * 정보그리기.
	 */
	void			DrawInfo()
	{
		if( !m_bViewTargetEffect || !m_pTargetEffect )
		{
			return;
		} //if

		if( m_dwTargetColor == 0xFFFF0000 )
		{
			g_pGUI->ShowTargetInfo( m_dwId, m_nHp[1], m_nHp[0], m_sName.c_str() );
		} //if

		static Matrix4		matWorld;
		matWorld.SetWorld( m_vPos, m_qRot );
		m_pTargetEffect->SetColor( m_dwTargetColor );

		if( !m_pTargetEffect->Render( m_dwEffectTime * 30 / 1000, matWorld ) )
		{	// 이펙트가 끝까지 진행된 경우..
			m_dwEffectTime = 0;
			m_pTargetEffect->Render( m_dwEffectTime * 30 / 1000, matWorld );
		} //if
	} //DrawSelectPointer

	/**
	 *
	 */
	virtual void AddVehicle( const void* pData )
	{
		SAFE_DELETE( m_pVehicle );

		CVehicle*	pVehicle = new CVehicle;
		Assert( pVehicle );

		SW3DVehicle&	infoVehicle = *(SW3DVehicle*)pData;
		Assert( infoVehicle.wType );

		infoVehicle.nX = m_vPos.x;
		infoVehicle.nY = m_vPos.y;

		if( !pVehicle || !pVehicle->Create( m_pWorld, 0, &infoVehicle ) )
		{
			Assert( !"pVehicle 생성 실패" );
			SAFE_DELETE( pVehicle );
			return;
		} //if

		m_bUseVehicle = TRUE;
		m_pVehicle = pVehicle;

		Stop();
	} //AddVehicle

	/**
	 *
	 */
	virtual void RemoveVehicle()
	{
		SAFE_DELETE( m_pVehicle );
		m_pVehicle = NULL;
		m_bUseVehicle = FALSE;
	} //RemoveVehicle

	/**
	 *
	 */
	void UpdateVehicle( const DWORD dwTick )
	{
		if( m_bUseVehicle && m_pVehicle  )
		{

			Assert( m_pVehicle );
			m_pVehicle->SetLighting( TRUE );

			if( !m_pVehicle->Update( dwTick ) )
			{
				m_bUseVehicle = FALSE;
				SAFE_DELETE( m_pVehicle );
			} //if
		} //if
	} //UpdateVehicle

	/**
	 *
	 */
	void DrawVehicle()
	{
		if( m_bUseVehicle && m_pVehicle )
		{
			m_pVehicle->SetPosition( m_vPos );
			m_pVehicle->SetQuaternion( m_qRot );

			m_pVehicle->Draw();
			m_pVehicle->DrawShadow();
		} //if
	} //DrawVehicle

	BOOL IsShow()
	{
		//---- NULL ----//
		return m_bShow;
	} //IsVisible

	/**
	 *
	 */
	void	ChangeSpeed( BYTE btSpeed )
	{
		m_fOffset = (float)btSpeed / 10.0f;
	} //ChangeSpeed

	/**
	 *
	 */
	virtual BOOL	CastingMagic( DWORD dwID, DWORD dwMagicId )
	{
		//---- NULL ----//
		return TRUE;
	} //CastingMagic

	/**
	 *
	 */
	int		GetSkill()
	{
		return m_nSkill;
	} //GetSkill

	/**
	 *
	 */
	BOOL	CanAttack()
	{
		if( m_bUseVehicle && ( m_nSkill != SKILL_SHORT && m_nSkill != SKILL_LONG ) )
		{
			return FALSE;
		} //if

		return TRUE;
	} //CanAttack

	/**
	 *
	 */
	void	SetAttackMove( BOOL bFlag )
	{
		m_bAttackMove = bFlag;
	} //SetAttackMove

	/**
	 *
	 */
	virtual void	AddAttacker( IW3DCreature* pAttacker )
	{
		//---- NULL ----//
	} //AddAttacker

	/**
	 *
	 */
	virtual void	RemoveAttacker( IW3DCreature* pAttacker )
	{
		//---- NULL ----//
	} //RemoveAttacker

	/**
	 *
	 */
	virtual void	AddTarget( IW3DCreature* pTarget )
	{
		//---- NULL ----//
	} //AddTarget

	/**
	 *
	 */
	virtual void	RemoveTarget( IW3DCreature* pTarget )
	{
		//---- NULL ----//
	} //RemoveTarget

	/**
	 *
	 */
	virtual void	SetMode( BYTE btMode )
	{
		//---- NULL ----//
	} //SetMode

	virtual IW3DModel* GetModel( int nPart )
	{
		//---- NULL ----//
		return NULL;
	} //GetModel

	virtual DWORD	GetTime()
	{
		return m_dwTick;
	} //GetTime

	/**
	 *
	 */
	void	SetAlpha( BYTE	btAlpha = 255 )
	{
		m_btAlpha = btAlpha;

		m_dwColor	= ( m_btAlpha << 24 ) | ( m_btRed << 16 ) | ( m_btGreen << 8 ) | m_btBlue;
	} //SetAlpha

	/**
	 *
	 */
	void	SetColor( BYTE btRed, BYTE btGreen, BYTE btBlue )
	{
		m_btRed		= btRed;
		m_btGreen	= btGreen;
		m_btBlue	= btBlue;

		m_dwColor	= ( m_btAlpha << 24 ) | ( m_btRed << 16 ) | ( m_btGreen << 8 ) | m_btBlue;
	} //SetColor

	/**
	 *
	 */
	virtual void	Resurrect()
	{
		//---- NULL ----//
	} //Resurrect

	/**
	 *
	 */
	virtual void	ShowDamageForTarget()
	{
		//---- NULL ----//
	} //ShowDamageForTarget
	
	/**
	 *
	 */
	virtual void	UseSkill( WORD wSkillId )
	{
		//---- NULL ----//
	} //UseSkill


	/**
	 *
	 */
	virtual void	AddSkill( IW3DCreature* pTarget, const WORD wSkillId )
	{

	} //AddSkill
	
	void	UpdateDamageAction( DWORD dwTick )
	{
		
	} //UpdateDamageAction

	float	GetMinDist()
	{
		if( m_bUseVehicle )
		{
			return 3.0F;
		} //if

		return 0.5F;
	} //if

	IW3DSoundObject3D* GetSound()
	{
		return m_pSound;
	} //GetSound

	void	SetVisible( BOOL bVisible )
	{
		m_bVisible = bVisible;

		if( m_bVisible )
		{
			m_btGhost = FALSE;
		} 
		else
		{
			if( g_pSelectMgr )
			{
				IW3DCreature* p = g_pSelectMgr->GetCreature();
				if( p && p->GetType() == GetType() )
				{
					if( p->GetId() == m_dwId )
					{
						g_pSelectMgr->Select( NULL );
					} //if
				} //if

				if( g_sTarget.IsTarget( GetType(), m_dwId ) )
				{
					m_bViewTargetEffect = FALSE;
					g_sTarget.Clean();
				} //if
			} //if			

			Resurrect();
			m_btGhost = TRUE;
		} //else..if

	} //SetVisible

	/**
	 *
	 */
	virtual void	SetActionSpeed( DWORD dwTicks )
	{
		//-- NULL --//
	} //SetActionSpeed

	/**
	 *
	 */
	void	CalculateDistLevel()
	{
		if( !g_pPlayer ) return ;

		Vector3 vDist = g_pPlayer->GetPosition() - m_vPos;
		float fDist = vDist.Length();

		if( fDist >= 50.0F )
		{
			m_btDistLevel = 5;
		}
		else if( fDist >= 40.0F )
		{
			m_btDistLevel = 4;
		}
		else if( fDist >= 30.0F )
		{
			m_btDistLevel = 3;
		}
		else if( fDist >= 20.0F )
		{
			m_btDistLevel = 2;
		}
		else if( fDist >= 10.0F )
		{
			m_btDistLevel = 1;
		}
		else 
		{
			m_btDistLevel = 0;
		} //if..else..
	
	} //CalculateDistLevel

	/**
	 * 
	 */
	virtual void SetShopInfo( const char* szTitle )
	{
		//-- NULL --//
	} //SetShopTitle

	/**
	 * 
	 */
	virtual char* GetShopInfo()
	{
		return NULL;
	} //GetShopTitle

	/**
	 *
	 */
	virtual BOOL IsOpenShop()
	{
		return FALSE;
	} //IsOpenShop

	/**
	 *
	 */
	virtual void SetGuildData( const char* szName, int nGuildId, BYTE btGuildLev, int nMark )
	{
		//-- NULL --//
	} //SetGuildData

	/**
	 *  
	 */
	virtual void ResetEnemy()
	{
		//-- NULL --//
	} //ResetEnemy

	virtual void InsertPet( const BYTE btType, void* pData )
	{
		//-- NULL --//
	} //InsertPet

	virtual void RemovePet()
	{
		//-- NULL --//
	} //RemovePet
	
	virtual void UpdatePet( const char* szName, void* pData )
	{
		//-- NULL --//
	} //UpdatePet

	BOOL	IsVehicleMode()
	{
		return m_bUseVehicle;
	} // IsVehicleMode

	/**
	 * 피킹.
	 *
	 * @return	vOrig		[IN] Picking Ray Origin
	 * @return	vDir		[IN] Picking Ray Direction
	 *
	 * @return	Picking
	 */
	BOOL	Pick( const Vector3& vOrig, const Vector3& vDir ){ return FALSE; }

	BOOL	CheckPoint( const Vector3& v ){ return FALSE; }

protected:
	IW3DWorld*			m_pWorld;			///< 게임 월드
	IW3DModel*			m_pShadowDummy;	
	CVehicle*			m_pVehicle;			///< 탈것
	IW3DSoundObject3D*	m_pSound;			///<
	BOOL				m_bLighting;		///< 빛 적용 여부
	BYTE				m_btGhost;			///< 고스트 상태.

	DWORD				m_dwId;				///<
	DWORD				m_dwType;			///<
	std::string			m_sName;			///<
	DWORD				m_dwColorName;		///< Color Of CharName
	Vector3				m_vLastGoal;		///<
	Vector3				m_vGoalPos;			///<
	Vector3				m_vPos;				///<
	Quaternion			m_qRot;				///<
	float				m_fOffset;			///<
	BOOL				m_bStatic;			///< 정적인 
	int					m_nLevel;			///< 레벨

	IW3DScene*			m_pScene;			///< 폰트.
	IW3DRectangle*		m_pRectangle;		///< 사각형.
	Vector3				m_vTextPos;			///< 텍스트 출력 좌표.

	Matrix4				m_matWorld;			///< World Matrix

	int					m_nState;			///< 현재 상태.
	int					m_nOldState;		///< 예전 상태.
	int					m_nAdvanceState;	///< 준비된 상태
	int					m_nSkill;			///< 현재 공격 방법
	int					m_nLayer;			///< 레이어

	DWORD				m_dwFollowTarget;	///< 선구자.
	DWORD				m_dwTick;			///<
	DWORD				m_dwEvent;			///<
	DWORD				m_dwTargetColor;	///< Target Color
	DWORD				m_dwColor;			///< 캐릭터 색
	int					m_nHp[2];						///< [0] - HP, [1] - HP 최고치. 
 
	//-=-=-=-=-=-=-=-=-=- Effect -=-=-=-=-=-=-=-=-=//
	
	IW3DSpecialEffect*	m_pTargetEffect; ///< 타겟 이펙트
	DWORD				m_dwEffectTime;

	BYTE				m_btAlpha;			///< Alpha
	BYTE				m_btGreen;			///< 
	BYTE				m_btRed;			///< 
	BYTE				m_btBlue;			///< 

	BYTE				m_btDistLevel;		///< 현재 플레이어와의 거리 단계 
											///< 최소 프레임 유지 컬링에서 사용

	BOOL				m_bFollow;			///< 
	BOOL				m_bUseVehicle;		///< Vehicle 사용 여부
	BOOL				m_bShow;			///< 보이는지의 여부
	BOOL				m_bEndMagic;		///< 마법이 끝난는지의 여부
	BOOL				m_bShowName;		///< 이름을 보일지 여부.
	BOOL				m_bViewTargetEffect;
	BOOL				m_bSemiMove;		///< 공격후 이동 여부 
	BOOL				m_bAttackMove;		///< 공격하기 위해 이동하는가?
	BOOL				m_bVisible;

	/**
	 *
	 */
	struct		SMagicData
	{
		BOOL bHero;				///< 현재 내가 시전하고 있는 것인가?
		BOOL bCast;				///< 시전중인가 아니면 시전이 끝난후인까?
		BOOL bIsTargetMonster;	///< 몬스터에게 쓰는 마법인가?
		DWORD dwId;				///< 어떠한 마법을 쓰는가?
		DWORD dwTargetId;		///< 누구에게 쓰고 있는가?		
	}; //struct SMagicData

	SMagicData			m_MagicData;

	//--=-=-==-=-=-=-=-== Magic -=-=-=--=-=-==-=-==//
	typedef std::map<DWORD, IW3DMagic*> MagicMap;
	MagicMap			m_mapMagic;
	MagicMap			m_mapOwnerMagic;

	

}; //class CCreature

#endif // #ifndef __Creature_Header__
