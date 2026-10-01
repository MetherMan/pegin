/**
 * 몬스터.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 211 $
 *
 * <HR>
 * $Log: /GameClient/Src/Monster.cpp $
 * 
 * 211   05-02-18 7:38p Sk8snow2
 * 
 * 210   04-11-16 7:11p Sk8snow2
 * 
 * 209   04-09-17 12:10p Paran
 * 
 * 208   04-09-10 10:28p Sk8snow2
 * 
 * 207   04-09-10 6:09p Sk8snow2
 * 
 * 206   04-06-21 3:23p Paran
 * 
 * 205   04-06-08 3:57p Paran2
 * 
 * 204   04-05-25 2:58p Paran2
 * 
 * 203   04-03-12 6:42p Sckyj
 * 운영자 작업중
 * 
 * 202   04-03-12 2:12p Paran
 * Collision & Picking
 * 
 * 201   04-02-02 2:26p Paran
 * 
 * 200   04-01-08 10:08p Paran
 * 
 * 199   04-01-08 6:49p Kjmgo
 * 
 * 198   04-01-07 2:35p Kjmgo
 * 
 * 197   04-01-06 1:58p Kjmgo
 * 공격 매크로 추가
 * 
 * 196   04-01-03 4:10p Paran
 * 
 * 195   03-12-31 3:18p Kjmgo
 * 마법쪽 버그 수정 
 * 
 * 194   03-12-30 9:41p Paran
 * 테스트
 * 
 * 191   03-12-26 10:33a Kjmgo			선택유지중, 몬스터도 추가
 * 
 * 189   03-12-22 5:00p Sckyj			2D, 3D sound 분리
 * 
 * 188   03-12-22 6:00a Kjmgo			버그 수정
 * 
 * 187   03-12-20 6:18a Kjmgo			메모리 관리
 * 
 * 185   03-12-16 5:05p Paran			아이템 데이터 수정
 * 
 * 181   03-11-29 2:30p Paran			데미지 동작 작업 중...
 * 
 * 180   03-11-21 10:00a Paran			몹 아이템 드롭 수정
 * 
 * 179   03-11-18 9:52p Kjmgo			GetHeight 관련 함수 수정및, 추가
 * 
 * 178   03-11-15 9:41p Paran			PK 부분 1차 수정
 * 
 * 177   03-11-12 8:36p Kjmgo			코드 최적화
 * 
 * 176   03-10-27 3:24p Kjmgo			코드 최적화
 * 
 * 175   03-10-23 10:04p Paran			공격효과부분 수정.
 * 
 * 174   03-10-20 10:16p Paran			몬스터 데미지 부분 수정.
 * 
 * 171   03-10-17 10:50a Sckyj			Monster sound 재생작업
 * 
 * 170   03-10-15 9:25p Paran			타격시 AddMagic으로 교체(일반이펙트시는 마지막 변수 FALSE설정)
 * 
 * 168   03-10-11 1:32a Kjmgo			몬스터 모델과, 바운드 박스를 맵 로딩시 모두 릴리즈 하도록 수정
 * 
 * 167   03-10-11 12:08a Kjmgo			AddMagic수정 및, CCreature::~CCreature(); 쓰던거 삭제
 * 
 * 165   03-10-10 11:33a Kjmgo			소스 정리
 * 
 * 161   03-10-03 5:51p Kjmgo			몬스터 상태가 반대로 먹던것 수정
 * 
 * 159   03-10-02 5:48p Kjmgo			생성시 hp를 0으로 세팅
 * 
 * 153   03-10-01 11:08p Kjmgo			크리처들과의 충돌처리 진행
 * 
 * 152   03-10-01 11:38a Kjmgo			최상위 Creature에 ShowDamageForTarget 함수 추가 및, 하위 클래스 함수 추가
 * 
 * 150   03-09-29 5:05p Kjmgo			Picking용 Interface Pointer를 CGame 내부에서 전역으로 수정
 * 
 * 147   03-09-22 2:01p Kjmgo			Creature 색상 설정및 알파값 설정 작업(완료)
 * 
 * 146   03-09-18 10:58a Kjmgo			데미지 처리 관련 수정
 * 
 * 140   03-09-09 5:45a Kjmgo			몬스터 속도 수정
 * 
 * 137   03-09-08 8:20p Kjmgo			동작~ 작업
 * 
 * 134   03-09-02 5:49a Kjmgo			Monster 추가작업
 * 
 * 132   03-08-29 9:19a Kjmgo			Animation 바꿈
 * 
 * 98    03-06-27 5:30p Kjmgo			Assert처리
 * 
 * 92    03-06-23 5:53p Paran			바운드 박스 처리.
 * 
 * 90    03-06-20 11:50a Paran			공격 매직 수정.
 * 
 * 86    03-06-15 5:58p Kjmgo			그림자를 동그란놈으로
 * 
 * 85    03-06-15 5:55p Kjmgo			없는 마법을 Add했을때 억세스 나오는걸 수정
 * 
 * 81    03-06-12 2:05p Paran			매직 관련작업
 * 
 * 76    03-06-03 11:38p Paran			조력자 맴버 추가
 * 
 * 72    03-05-27 12:11a Paran			몬스터 추가.
 *
 * 69    03-05-21 1:17a Paran			몬스터 한글 이름으로 변경.
 *
 * 65    03-05-13 11:00p Kjmgo			사망시 그림자를 출력 안 하게 수정.
 *
 * 60    03-05-10 12:54p Ahastudio		Engine에서 Creature Implement 관련 클래스를 Game으로 이동함.
 *
 * 59    03-05-10 12:32p Ahastudio		SetLighting() 추가.
 *
 * 54    03-04-24 11:34a Paran			몬스터 이동 옵셋 수정.
 *
 * 52    03-04-19 6:51p Kjmgo			그림자 그릴 때 Model의 World를 다시 세팅하도록 수정.
 *
 * 50    03-04-13 8:53p Kjmgo			실시간 그림자 추가.
 *
 * 37    03-03-18 12:01p Ahastudio		SoundSync 관련 부분 수정.
 * <HR>
 *
 * @file	Monster.cpp
 */


#include "Global.h"
#include "Creature.h"
#include <string>
#include <vector>
#include <map>

typedef std::map<DWORD, IW3DModel*>		ModelMap;

static ModelMap				gs_mapMonsterModel;		///< 모델 데이터.
struct SMobInfo
{
	char		szFileName[32];
	char		szMobName[32];
	char		szAttackMagic[32];
	DWORD		dwAttackTick;
	int			nLevel;
}; //struct MOBINFO
typedef std::map<WORD, SMobInfo>			MAP_MOBINFO;
MAP_MOBINFO					gs_mapMobInfo;
static IW3DShadow*		g_pMonsterShadow = NULL;

/**
 * 몬스터 클래스.
 * @ingroup		Game
 */
class CW3DMonster : public CCreature
{
public:
	UNDER_MEMORY_MGR( CW3DMonster );

	/**
	 * 생성자.
	 */
	CW3DMonster()
	:	m_pModel		( NULL ),
		m_pAction		( NULL ),
		m_bShadow		( FALSE ),
		m_dwTarget		( 0 ),
		m_szMagic		( NULL ),
		m_pActionMgr	( NULL ),
		m_dwFireTick	( 0 ),
		m_bDead			( FALSE ),
		m_bDamage		( FALSE ),
		m_nTotalDamege	( 0 ),
		m_wDamege		( 0 ),
		m_dwDeadTime	( 0 ),
		m_btCritical	( 0 ),
		m_bDamageEffect	( FALSE )
	{
		bFlag = FALSE;

		ZeroMemory( &m_sDamageAct, sizeof( m_sDamageAct ) );

		m_Damage.btCritical	= 0;
		m_Damage.pCreature	= this;
		m_Damage.wDamage	= 0;
	} //CW3DMonster

	BOOL bFlag;

	/**
	 * 소멸자.
	 */
	~CW3DMonster()
	{
		if( g_pPickMonster && g_pPickMonster->GetId() == GetId() )
		{
			g_pPickMonster = NULL;
		} //if

		if( g_sTarget.IsTarget( MONSTER, m_dwId ) )
		{ // 공격중인 캐릭터인 경우
			g_sTarget.Clean();
		} //if
		
		if( g_pSelectMgr && g_pSelectMgr->GetType() == MONSTER &&
			g_pSelectMgr->GetCreature() &&
			g_pSelectMgr->GetCreature()->GetId() == GetId()
			)
		{
			g_pSelectMgr->Select( NULL );
		} //if


		ClearTargets();
		ClearAttackers();

		if( g_pMonsterShadow->GetRefCount() <= 1 )
		{
			SAFE_RELEASE( g_pMonsterShadow );
		}
		else
		{
			SAFE_RELEASE( m_pShadow );
		}//if

		SAFE_DELETE( m_pActionMgr );
	} //~CW3DMonster

	BOOL	Create( IW3DWorld* pWorld, const DWORD dwId, const void* pData );

	BOOL	Update( const DWORD dwTick );
	BOOL	UpdateModel( const WORD wItemType, const BOOL bOn );

	BOOL	Pick( const Vector3& vOrig, const Vector3& vDir );

	void	Draw();
	void	DrawShadow();
	void	DrawName();
	void	ShowMonsterName( BOOL bShow );

	BOOL	CheckPoint( const Vector3& v );

	/**
	 * 크리쳐 종류를 얻는다.
	 */
	int		GetType()
	{
		return MONSTER;
	} //GetType

	/**
	 * 길찾기를 통해 목표점까지 이동한다.
	 */
	BOOL	MoveTo( const Vector3& vPos )
	{
		//--NULL--

		return FALSE;
	} //MoveTo

	/**
	 * 다른 크리쳐를 공격한다.
	 */
	void	Attack(
		IW3DCreature*	pTarget		///< [IN] 공격 대상
		)
	{
		if( !pTarget )
		{
			Assert( pTarget );
			return ;
		} //if

		m_vGoalPos = m_vPos;

		m_pActionMgr->Attack();
		SetState( STATE_ATTACK );
		m_dwTarget	= pTarget->GetId();
	} //Attack

	void	Hitting( 
		BOOL bHitLock				///< [IN] 한대 맞는 중...
		)
	{

	} //Hitting

	void	Dead()
	{
		m_bDead = TRUE;
		m_dwDeadTime = 0;

		///////// 즉시 죽여준다 ////////
		SetState( STATE_DEAD );
		m_pActionMgr->Dead();
	} //Dead

	/**
	 * 공격을 받는다.
	 */
	void	Damage( BOOL bBegin, IW3DCreature*	pAttacker )
	{
		Assert( m_pWorld );
		Assert( m_pSound );

		m_bDamage = bBegin;

		if( bBegin )
		{
			DEBUG_OUT( "-=-=-=- Begin Attack the monster!! -=-=-=-" );
			return ;
		}
		else
		{
			DEBUG_OUT( "-=-=-=- End Attack the monster!! -=-=-=-" );
		//	DamageEffect( pAttacker );
		} //if..else

		DamageEffect( pAttacker );
  } //Damage

	/**
	 * 공격받은 수치설정.
	 */
	void	Damage( const WORD wDamage, const BYTE btCritical )
	{
		m_wDamege	 = wDamage;
		m_btCritical = btCritical;

		m_nTotalDamege += wDamage;

		m_Damage.btCritical = btCritical;
		m_Damage.wDamage	= wDamage;
		m_Damage.pCreature	= this;
	} //Damage

	/**
	 * 데미지 효과.
	 */
	void	DamageEffect( IW3DCreature* pAttacker )
	{
		if( g_sTarget.IsTarget( MONSTER, m_dwId ) )
		{
			char		szEffect[16];
			int			nEffect = ( rand() % 3 )  + 1;
			sprintf( szEffect, "타격%d", nEffect );
			m_pWorld->AddMagic( szEffect, this, this, FALSE );

			if( m_btCritical )
			{
				m_pWorld->AddMagic( "크리티컬", this, this, FALSE );
			} //if
		} //if

		m_pWorld->AddHit( m_nTotalDamege - m_wDamege, m_vPos, 0xFFFFFFFF, m_btCritical );
		m_nTotalDamege = 0;

		m_vGoalPos = m_vPos;

		Vector3		 vDir = pAttacker->GetPosition() - m_vPos;
		vDir.Normalize();
		m_vPos = m_vPos - vDir * DAMAGE_REACTION;
		
		if( m_sDamageAct.pAnimation )
		{
			if( IsGoal() )
			{
				m_bDamageEffect = TRUE;
				m_dwDamageEffectTick = 0;
			}
		} //if
	} //DamageEffect

	/**
	 * 공격 상태.
	 *
	 * @param	dwTick		[IN] 경과 틱
	 * @param	nIndex		[IN] 공격형태3
	 */
	BOOL	StateAttack( DWORD dwTick, int nIndex )
	{
		Assert( m_pWorld );
		Assert( m_pModel );

		DWORD	dwCurTick = m_dwTick + dwTick;

		if( ( m_dwTick  <= m_dwFireTick ) &&
			( dwCurTick >  m_dwFireTick ) )
		{
			IW3DCreature*	pCharacter = m_pWorld->GetCharacter( m_dwTarget );
			if( pCharacter )
			{
				if( m_szMagic[0] != '\0' )
				{ //매직 지정된...
					m_pWorld->AddMagic( m_szMagic, this, pCharacter );
				} //if
			} //if
		} //if

		if( IsEndAction( dwCurTick ) )
		{			
			if( IsGoal() )
			{
				m_vPos.z   = GetHeight();
				m_vGoalPos = m_vPos;

				Stop();
			} 
			else
			{
				m_pActionMgr->Move();
				Move();
			} //if..else..
			
		} //if

		return TRUE;
	} //StateAttack

	/**
	 * 조력자 설정.
	 */
	void SetHelper( const IW3DCreature* pCreature )
	{
		//--NULL--
	} //SetHelper

	/**
	 * 조력자 제거.
	 */
	void RemoveHelper()
	{
		//--NULL--
	} //RemoveHelper

	/**
	 * 성별을 얻는다.
	 */
	UINT GetSex()
	{		
		return 0;
	} // GetSex

	void AddMagic( const WORD wMagic, const WORD wID, IW3DCreature* pTarget, BOOL bCompulsion )
	{
		IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();
		Assert( pItemInfoMgr );

		if( !pItemInfoMgr || !pTarget )
		{
			return ; 
		} //if

		SItemInfoData*	pItemInfo = pItemInfoMgr->GetItemInfo( wMagic );
		if( !pItemInfo )
		{
			Assert( !"없는 마법" );
			return;
		} //if

	 	m_pWorld->AddMagic(	pItemInfo->szEName,	this, pTarget );
	} //

	BOOL IsVisible()
	{
		return m_bShow;
	} //IsVisible

	/**
	 *
	 */
	void AddAttacker( IW3DCreature* pAttacker );

	/**
	 *
	 */
	void RemoveAttacker( IW3DCreature* pAttacker );

	/**
	 *
	 */
	void AddTarget( IW3DCreature* pTarget );

	/**
	 *
	 */
	void RemoveTarget( IW3DCreature* pTarget );

	/**
	 *
	 */
	void ClearAttackers();

	/**
	 *
	 */
	void ClearTargets();

	/**
	 *
	 */
	void SetMode( BYTE btMode )
	{
		if( m_pActionMgr )
		{
            if (btMode == 2)
                m_pActionMgr->StopImmediately();
            else
                m_pActionMgr->SetMode(btMode);
		} //if
	} //SetMode

	/**
	 *
	 */
	BOOL IsDead()
	{
		return m_bDead;
	} //IsDead

	/**
	 *
	 */
	IW3DModel* GetModel( int nPart )
	{
		if( m_pModel )
		{
			m_pModel->SetAction( m_pAction );
			return m_pModel;
		} //if

		return NULL;
	} //GetModel

	/**
     *
	 */
	SW3DDamage* GetDamage()
	{
		return &m_Damage;
	} //CW3DCharacter::GetDamage

	/**
	 *
	 */
	void ShowDamageForTarget ()
	{
		if( m_pActionMgr )
		{
			m_pActionMgr->ShowDamageOfTarget();
			m_pActionMgr->ClearTargets();
		} //if
	} //ShowDamageForTarget

	/**
	 *
	 */
	void SetActionSpeed( DWORD dwTicks )
	{
		if( m_pActionMgr )
		{
			m_pActionMgr->SetAttackSpeed( dwTicks );
		} //if
	} //SetActionSpeed

	/**
	 * 몹에 대한 정보 읽기
	 */
	BOOL LoadMobInfo();
	
private:
	IW3DModel*		m_pModel;		///< 모델
	IW3DShadow*		m_pShadow;		///< 그림자
	SAction*		m_pAction;		///< 동작 
	SAction			m_sDamageAct;	///< 맞는 동작 
	char*			m_szMagic;		///< 해당 매직이름
	DWORD			m_dwFireTick;	///< 마법 나가는 틱
	BOOL			m_bDamage;		///< 한대 맞는 중...
	WORD			m_wDamege;		///< 데미지 수치.
	BYTE			m_btCritical;	///< 크리티컬 데미지.
	int				m_nTotalDamege; ///< 카운트어택 체크
	float			m_fOffsetForWar;///<

	BOOL			m_bDead;		///< 죽을 준비.

	BOOL			m_bShadow;		///< 그림자 적용 여부

	DWORD			m_dwTarget;	///< 공격 중인 캐릭터

	CMonsterAction*	m_pActionMgr;	///< 동작 관리자

	DWORD			m_dwDeadTime;	///< 이시간을 넘으면... 무조건 사망 처리

	//-=-=-=-=-=-=-=-=-=- Effect -=-=-=-=-=-=-=-=-=//
	
	BOOL			m_bDamageEffect;
	DWORD			m_dwDamageEffectTick;

	//--=-=-==-=-=-=-=-== Creature List -=-=-=--=-=-==-=-==//
	typedef std::map<DWORD, IW3DCreature*> CreatureMap;
	CreatureMap			m_mapAttackers;
	CreatureMap			m_mapTargets;

	SW3DDamage			m_Damage;		///< 데미지 정보

	IW3DModel*	LoadModel( DWORD dwType, const char* szModelName );

	SMobInfo*	FindMobInfo( DWORD dwType );

	char*		GetActionName();
	BOOL		IsEndAction( DWORD dwTick );

	BOOL		StateDead( DWORD dwTick );
	BOOL		StateMovement( DWORD dwTick );

	void		AddMagic( const WORD wMagic, IW3DCreature* pTarget );
}; //class CW3DMonster


IMPLEMENT_COMPONENT( CW3DMonster, Monster );




/**
 * 몬스터 생성.
 *
 * @return	성공 여부
 */
BOOL CW3DMonster::Create(
	IW3DWorld*		pWorld,		///< [IN] 월드 객체
	const DWORD		dwId,		///< [IN] 크리쳐 ID
	const void*		pData		///< [IN] 크리쳐 생성 정보
	)
{
	CCreature::Create( pWorld, dwId, pData );
	
	Assert( pData );
	Assert( m_pWorld );
	Assert( NULL == m_pSound );
	Assert( NULL == m_pModel );
	//Assert( NULL == g_pMonsterShadow );

	m_pScene = g_pScene;
	Assert( m_pScene );
	
	m_pRectangle = MAKE_COMPONENT( Rectangle );
	Assert( m_pRectangle );

	if( !g_pMonsterShadow )
	{
		g_pMonsterShadow = MAKE_COMPONENT( Shadow );
		CHECK_PTR_RETURN( g_pMonsterShadow ); 

		g_pMonsterShadow->SetSize( 1 );
		if( !g_pMonsterShadow->Init( 0 ) )
		{
			Assert( !"그림자 초기화 실패" );
			SAFE_RELEASE( g_pMonsterShadow );
			return FALSE;
		} //if
	}
	else
	{
		g_pMonsterShadow->AddRef();
	}//if

	m_pShadow = g_pMonsterShadow;
	m_bShowName = FALSE;

	LoadMobInfo();

	SW3DMonsterInfo&	infoMonster = *(SW3DMonsterInfo*)pData;
	SMobInfo*	pMobInfo = FindMobInfo( infoMonster.wType );
	if( !pMobInfo )
	{
		return FALSE;
	} //if

	m_dwType	= infoMonster.wType;
	m_dwId		= dwId;
	m_sName		= pMobInfo->szMobName;
	m_szMagic	= pMobInfo->szAttackMagic;
	m_dwFireTick = pMobInfo->dwAttackTick;
	m_fOffset	= (float)infoMonster.btOffset / 10.0f;
	m_fOffsetForWar = (float)infoMonster.btOffset2 / 10.0f;
	m_vPos.x	= (float)infoMonster.nX;
	m_vPos.y	= (float)infoMonster.nY;
	m_vPos.z	= GetHeight();
	m_vGoalPos	= m_vPos;
	m_nLevel	= pMobInfo->nLevel;
	m_qRot		= Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion();

	m_pSound = MAKE_COMPONENT( SoundObject3D );
	CHECK_PTR_RETURN( m_pSound );

	// Load Model

	m_pModel = LoadModel( m_dwType, pMobInfo->szFileName );
	if( !m_pModel )
	{
		Assert( !"모델 읽기 실패" );
		return FALSE;
	} //if


	// Create Shadow
	m_bShadow = TRUE;

	m_pActionMgr = new CMonsterAction;
	m_pActionMgr->Init( m_pModel, this );
	if( !m_pActionMgr->SetMonster( m_dwType ) )
	{
		SAFE_DELETE( m_pActionMgr );
		return FALSE;
	} //if
	m_pAction = m_pActionMgr->GetAction();
	m_pActionMgr->SetMode( !infoMonster.btState );

	m_pActionMgr->FindAction( m_sDamageAct, FALSE, MONSTER_STATE_DAMAGE );
	m_pActionMgr->SetSound( m_pSound );

	return TRUE;
} //CW3DMonster::Create

/**
 * 몬스터 모델을 읽어옴.
 *
 * @param	dwType		[IN] 몬스터 종류
 *
 * @return	몬스터 모델
 */
IW3DModel* CW3DMonster::LoadModel( DWORD dwType, const char* szModelName )
{
	ModelMap::iterator	itor = gs_mapMonsterModel.find( dwType );
	if( itor != gs_mapMonsterModel.end() )
	{
		IW3DModel*	pModel = itor->second;
		Assert( pModel );

		if( !pModel )
		{
			return NULL;
		} //if

		return pModel;
	} //if

	IW3DModel*	pModel = MAKE_COMPONENT( Model );
	if( !pModel )
	{
		Assert( !"메모리 할당 실패" );
		return NULL;
	} //if

	char szFile[128];
	sprintf( szFile, "%s.mod", szModelName );
	if( !pModel->Load( "Monster", szFile ) )
	{
		Assert( !"모델 읽기 실패" );
		SAFE_RELEASE( pModel );
		return NULL;
	} //if

	gs_mapMonsterModel.insert( std::make_pair( dwType, pModel ) );

	DEBUG_OUT( "Load MONSTER Model - " + szFile );

	return pModel;
} //CW3DMonster::LoadModel

/**
 * 몹 정보 읽기
 */
BOOL CW3DMonster::LoadMobInfo()
{
	if( !gs_mapMobInfo.empty() )
	{
		return FALSE;
	} //if

	FILE*		pFile = fopen( "Monster/MobInfo.dat", "rb" );
	if( !pFile )
	{
		Assert( !"파일열기실패" );
		return FALSE;
	} //if

	char		szBuff[64];
	char		szFileInfo[] = "LaqiaMonster";
	
	fread( szBuff, strlen( szFileInfo ), 1, pFile );
	szBuff[strlen( szFileInfo )] = '\0';
	if( strcmp( szBuff, szFileInfo ) )
	{
		Assert( !"파일오류" );
		fclose( pFile );
		return FALSE;
	} //if

	int				nId;
	SMobInfo		sInfo;

	char			szName1[16];
	char			szName2[16];
	char			szName3[16];

	while( !feof( pFile ) )
	{
		fscanf( pFile, "%d %d %s %s %s %s %s %d", 
		&nId, 
		&sInfo.nLevel,
		szName1,
		szName2,
		szName3,
		sInfo.szFileName,
		sInfo.szAttackMagic,
		&sInfo.dwAttackTick
		); 

		if( strcmp( szName2, "\\" ) == 0 )
		{
			sprintf( sInfo.szMobName, "%s", szName1 );
		}
		else if( strcmp( szName3, "\\" ) == 0 )
		{
			sprintf( sInfo.szMobName, "%s %s", szName1, szName2 );
		}
		else
		{
			sprintf( sInfo.szMobName, "%s %s %s", szName1, szName2, szName3 );
		}

		if( !strcmp( sInfo.szAttackMagic, "NULL" ) )
		{
			sInfo.szAttackMagic[0] = '\0';
		} //if
		gs_mapMobInfo.insert( std::make_pair( nId, sInfo ) );
	} //while

	fclose( pFile );
	return TRUE;
} //CW3DMonster::LoadMobInfo

/**
 * 몬스터 정보를 얻는다.
 *
 * @param	dwType		[IN] 몬스터 종류
 *
 * @return	몬스터 이름
 */
SMobInfo* CW3DMonster::FindMobInfo( DWORD dwType )
{
	MAP_MOBINFO::iterator	iter = gs_mapMobInfo.find( dwType );
	if( iter == gs_mapMobInfo.end() )
	{
		Assert( !"몹 정보가 없다" );
		return NULL;
	} //if

	SMobInfo*	pInfo = &iter->second;
	if( !pInfo )
	{
		Assert( !"없는 몹 정보" );
		return NULL;
	} //if
	return pInfo;
} //CW3DMonster::FindMobInfo

/**
 * 몬스터를 그려준다.
 */
void CW3DMonster::Draw()
{
	Assert( m_pWorld );
	Assert( m_pModel );

	if( !m_bShow )
	{
		return ;
	} //if

	m_pModel->SetWorld( m_matWorld );
	m_pModel->SetAction( m_pAction );
	m_pModel->SetColor( m_dwColor );

	m_pModel->SetApplyWorld( FALSE );

	if( m_bDamageEffect )
	{
		if( !m_pModel->SetMultiActionVertexBlend( m_sDamageAct, m_dwTick, m_dwDamageEffectTick ) )
		{
			m_bDamageEffect = FALSE;
			m_pModel->SetVertexBlend( m_dwTick );
		} //if
	}
	else
	{
		m_pModel->SetVertexBlend( m_dwTick );
	} //if..else
	
	m_pModel->Render( m_dwTick, m_bLighting );
	m_pModel->SetApplyWorld( TRUE );
	
	DrawInfo();
} //CW3DMonster::Draw

/**
 * 몬스터의 그림자를 그려준다.
 */
void CW3DMonster::DrawShadow()
{
	Assert( m_pWorld );
	Assert( g_pMonsterShadow );

	if( !m_bShadow || !m_bShow )
	{
		return;
	} //if

	if( g_pMonsterShadow->IsProjection() )
	{
		m_pWorld->GetTerrain()->InsertShadow( g_pMonsterShadow, m_vPos );

		g_pMonsterShadow->Render( m_pScene->GetFrustum().m_matView );
	}
	else
	{
		static Matrix4		matWorld;
		matWorld.SetWorld( m_vPos + Vector3( 0.0F, 0.0F, 0.1F ), m_qRot );
		g_pMonsterShadow->Render( matWorld );
	} //if..else..

	DrawName();
} //CW3DMonster::DrawShadow

/**
 * 이름 찍기.
 */
void CW3DMonster::DrawName()
{
	if( g_bShowEmblem )//if( m_bShowName || !m_bLighting )
	{
		Vector3		vSize = m_pModel->GetSize();
		float	fHeight = vSize.y + 0.2F;
		m_vTextPos = m_pScene->GetScreenPos( m_vPos + Vector3( 0.0F, 0.0F, fHeight ) );

		int		nTxtWidth, nTxtHeight;
		m_pScene->GetStringSize( nTxtWidth, nTxtHeight, m_sName.c_str() );
		int		nX = m_vTextPos.x - ( nTxtWidth / 2 );
		int		nY = m_vTextPos.y + 1;
		//nX = nX - ( nTxtWidth >> 1 );

		int Userlevel = g_infoPlayer.wLevel[0] + g_infoPlayer.wLevel[1] + g_infoPlayer.wLevel[2] + g_infoPlayer.wLevel[3];
		int sum = 0;
		if( Userlevel == m_nLevel ) // 같으면...
		{
			m_dwColorName = 0xffBBBABA;
		}
		else if( Userlevel < m_nLevel ) // 몬스터가 강하면...
		{
			sum = m_nLevel - Userlevel;
			switch( sum ) {
			case 1:
				m_dwColorName = 0xffBBBABA;
				break;
			case 2:
			case 3:
				m_dwColorName = 0xffD9D685;
				break;
			case 4:
			case 5:
				m_dwColorName = 0xffDCD400;
				break;
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				m_dwColorName = 0xffDC9600;
				break;
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
				m_dwColorName = 0xffDC6800;
				break;
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:
				m_dwColorName = 0xffdc0000;
				break;
			default:
				m_dwColorName = 0xffdc0000;
			}
		}
		else if( Userlevel > m_nLevel )	// 유저가 강하면...
		{
			sum = Userlevel - m_nLevel;
			if( sum <= 2 )
			{
				m_dwColorName = 0xffBBBABA;
			}
			else if( sum <= 6 )
			{
				m_dwColorName = 0xff009BDC;
			}
			else if( sum <=  10 )
			{
				m_dwColorName = 0xff0058DC;
			}
			else if( sum <= 20 )
			{
				m_dwColorName = 0xff0010DC;
			}
			else
			{
				m_dwColorName = 0xff0010DC;
			}
		}

		m_pRectangle->Draw( nX-2, nY-2, nTxtWidth+4, nTxtHeight+4, COLOR_ARGB( 150, 0,0,0 ) );
		m_pScene->DrawText( nX, nY, m_dwColorName, m_sName.c_str() );
	} //if
} //CW3DMonster::DrawName

/**
 * 체크포인트
 */
BOOL CW3DMonster::CheckPoint( const Vector3& v )
{
	if( !m_pModel )
	{
		return FALSE;
	} //if

	m_pModel->SetWorld( m_matWorld );
	return m_pModel->CollisionPoint( v );  //(before Calculate Collision Box)
} //CW3DMonster::CheckPoint

/**
 * 죽은 상태.
 *
 * @param	dwTick		[IN]	경과 tick
 */
BOOL CW3DMonster::StateDead( DWORD dwTick )
{
	Assert( m_pWorld );
	Assert( m_pModel );

	DWORD	dwCurTick = m_dwTick + dwTick;

	m_bShadow = FALSE;

	switch( m_dwEvent )
	{
	case 0:
		if( dwCurTick > 0 )
		{
//			m_pSound->Play( "골렘죽을때.wav" );
			m_dwEvent++;
		} //if
		break;

	case 1:
		if( dwCurTick > 500 )
		{
			m_pWorld->AddEffect( "die.wed", m_vPos, m_qRot );
			m_dwEvent++;
		} //if
		break;
	} //switch

	if( IsEndAction( dwCurTick ) )
	{
		return FALSE;
	} //if

	return TRUE;
} //CW3DMonster::StateDead

/**
 * 이동하는 상태
 *
 * @param	dwTick		[IN]	경과 틱
 */
BOOL CW3DMonster::StateMovement( DWORD dwTick )
{
	Assert( m_pWorld );

	if( !IsGoal() )
	{
		float		fOffset = 0.0F;
			
		if( m_pActionMgr->GetMode() )
		{
			fOffset = m_fOffset * dwTick / 1000;
		}
		else
		{
			fOffset = m_fOffsetForWar * dwTick / 1000;
		} //if

		Vector3		vDir = ( m_vGoalPos - m_vPos ).Normal();

		if( ( m_vGoalPos - m_vPos ).Length() < fOffset )
		{
			fOffset = ( m_vGoalPos - m_vPos ).Length();
		}

		m_vPos += vDir * fOffset;
		m_qRot = vDir.GetQuaternion();

		m_vPos.z = m_pWorld->GetHeight( m_vPos );

		if( fOffset > 5.0F )
		{
			m_vPos = m_vGoalPos;
			m_qRot = vDir.GetQuaternion();
		} //if

		m_pActionMgr->Move();
		Move();
	}
	else
	{
		m_vPos.z = GetHeight();

		m_pActionMgr->Stop();
		Stop();
	} //if..else..

	return TRUE;
} //CW3DMonster::StateMovement

/**
 * 현재 Action의 이름을 알려준다.
 *
 * @return	현재 Action의 이름 / NULL
 */
char* CW3DMonster::GetActionName()
{
	switch( GetState() )
	{
	case STATE_DEAD:
		return "사망";

	case STATE_ATTACK:
		return "공격";

	case STATE_MOVE:
		return "이동";

	case STATE_STOP:
		return "정지";

	default: 
		break;
	} //switch
	return NULL;
} //CW3DMonster::GetActionName

/** 
 * 마법추가.
 */
void CW3DMonster::AddMagic( const WORD wMagic, IW3DCreature* pTarget )
{
	IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();
	Assert( pItemInfoMgr );

	if( !pItemInfoMgr || !pTarget )
	{
		return ;
	} //if

	SItemInfoData*	pItemInfo = pItemInfoMgr->GetItemInfo( wMagic );
	if( !pItemInfo )
	{
		Assert( !"없는 마법" );
		return;
	} //if

	m_pWorld->AddMagic( pItemInfo->szEName,	this, pTarget );
} //CW3DMonster::AddMagic

/**
 * 액션이 끝났는지 알려준다.
 */
BOOL CW3DMonster::IsEndAction( DWORD dwTick )
{
	return m_pActionMgr->IsEnd();
} //CW3DMonster::IsEndAction

/**
 * ...
 */
BOOL CW3DMonster::Update( const DWORD dwTick )
{
	Assert( m_pScene );
	Assert( m_pWorld );
	
	if( !m_pScene || 
		!m_pWorld || 
		!m_pModel || 
		!m_pActionMgr )
	{
		return FALSE;
	} //if

#ifdef _DEBUG
	if( g_bAttackMacro && m_nState != STATE_DEAD )
	{
		if( g_pPickMonster )
		{
			if( ( g_pPickMonster->GetPosition() - g_pPlayer->GetPosition() ).Length() >=
				( GetPosition() - g_pPlayer->GetPosition() ).Length()	)
			{
				g_pPickMonster = this;
			}
		}
		else
			g_pPickMonster = this;
		

	} //if
#endif //#ifdef _DEBUG

	static Frustum	frustum;
	static Vector3	vPos;
	frustum = m_pScene->GetFrustum();
	vPos    = m_vPos + Vector3( 0.0F, 0.0F, 1.0F );

	m_pModel->SetAction( m_pAction );
	m_bShow = FALSE;

	m_matWorld.SetWorld( m_vPos, m_qRot );
	if( frustum.CheckSphere( vPos, 1.0F ) )
	{
		m_bShow = TRUE;
	} //if
	
	m_pSound->SetPosition( m_vPos );
	m_pActionMgr->Update( dwTick );

	BOOL	bLive = TRUE;

	switch( GetState() )
	{
	case STATE_DEAD:
		bLive = StateDead( dwTick ); 
		if( !bLive )
		{
			m_pWorld->AddItemFromTarget( MONSTER, m_dwId );
		} //if
		break; 

	case STATE_ATTACK:
		m_bShadow = TRUE;
		StateAttack( dwTick, 1 );
		break;

	case STATE_MOVE:
		m_bShadow = TRUE;
		m_vGoalPos.z = m_pWorld->GetHeight( m_vGoalPos );
		StateMovement( dwTick );
		break;

	default:
		m_bShadow = TRUE;
		StateMovement( dwTick );
		break;
	} //switch

	if( m_bDead )
	{
		m_dwDeadTime += dwTick;

		if( !m_bDamage || m_dwDeadTime >= 12000 || m_mapAttackers.empty() )
		{
			SetState( STATE_DEAD );
			m_pActionMgr->Dead();
		} //if
	} //if
	
	m_dwTick = m_pActionMgr->GetTime();
	m_dwEffectTime		 += dwTick;
	m_dwDamageEffectTick += dwTick;

	return bLive;
} //CW3DMonster::Update

/**
 * 몬스터 모델을 업데이트 한다.
 */
BOOL CW3DMonster::UpdateModel( const WORD wItemType, BOOL bOn )
{
	return TRUE;
} //CW3DMonster::UpdateModel


/**
 *
 */
void CW3DMonster::AddAttacker( IW3DCreature* pAttacker )
{
	Assert( pAttacker );

	if( pAttacker )
	{
		m_mapAttackers.insert( std::make_pair( pAttacker->GetId(), pAttacker ) );
	} //if
} //CW3DMonster::AddAttacker

/**
 *
 */
void CW3DMonster::RemoveAttacker( IW3DCreature* pAttacker )
{
	if( !pAttacker )
	{
		return ;
	} //if

	CreatureMap::iterator itor = m_mapAttackers.find( pAttacker->GetId() );

	if( itor != m_mapAttackers.end() )
	{
		// 있다.!!!
		m_mapAttackers.erase( itor );
	} //if
} //CW3DMonster::RemoveAttacker

/**
 *
 */
void CW3DMonster::AddTarget( IW3DCreature* pTarget )
{
	Assert( pTarget );

	if( pTarget )
	{
		m_mapTargets.insert( std::make_pair( pTarget->GetId(), pTarget ) );
	} //if

	m_pActionMgr->AddTarget( pTarget );
} //CW3DMonster::AddTarget

/**
 *
 */
void CW3DMonster::RemoveTarget( IW3DCreature* pTarget )
{
	if( !pTarget )
	{
		return ;
	} //if

	CreatureMap::iterator itor = m_mapTargets.find( pTarget->GetId() );

	if( itor != m_mapTargets.end() )
	{
		// 있다.!!!
		m_mapTargets.erase( itor );
	} //if

	m_pActionMgr->RemoveTarget( pTarget );
} //CW3DMonster::RemoveTarget

/**
 *
 */
void CW3DMonster::ClearAttackers()
{
	if( m_mapAttackers.empty() )
	{
		return ;
	} //if

	CreatureMap::iterator itor = m_mapAttackers.begin();
	CreatureMap::iterator itorNext;

	while( itor != m_mapAttackers.end() )
	{
		itorNext = itor;
		itorNext++;

		IW3DCreature* pCreature = itor->second;

		if( pCreature )
		{
			pCreature->RemoveTarget( this );
		} //if

		m_mapAttackers.erase( itor );
		itor = itorNext;
	} //while
} //CW3DMonster::ClearAttackers

/**
 *
 */
void CW3DMonster::ClearTargets()
{
	if( m_mapTargets.empty() )
	{
		return ;
	} //if

	CreatureMap::iterator itor = m_mapTargets.begin();
	CreatureMap::iterator itorNext;

	while( itor != m_mapTargets.end() )
	{
		itorNext = itor;
		itorNext++;

		IW3DCreature* pCreature = itor->second;

		if( pCreature )
		{
			pCreature->RemoveAttacker( this );
		} //if

		m_mapTargets.erase( itor );
		itor = itorNext;
	} //while
} //CW3DMonster::ClearTargets

/**
 * 피킹.
 *
 * @return	vOrig		[IN] Picking Ray Origin
 * @return	vDir		[IN] Picking Ray Direction
 *
 * @return	Picking
 */
BOOL CW3DMonster::Pick( const Vector3& vOrig, const Vector3& vDir )
{
	if( !m_bShow || !m_pModel || ( m_btGhost && !m_bVisible ) )
	{
		return FALSE;
	} //if

	m_pModel->SetWorld( m_matWorld );
	if( m_pModel->Pick( vOrig, vDir ) )
	{
		return TRUE;
	} //if
	return FALSE;
} //Pick

/**
 *
 */
void ReleaseMonsterModel()
{
	ModelMap::iterator itor		= gs_mapMonsterModel.begin();
	ModelMap::iterator itorNext;

	while( itor != gs_mapMonsterModel.end() )
	{
		itorNext = itor;
		itorNext++;

		IW3DModel* pModel = itor->second;

		SAFE_RELEASE( pModel );

		gs_mapMonsterModel.erase( itor );
		itor = itorNext;
	} //while
} //
