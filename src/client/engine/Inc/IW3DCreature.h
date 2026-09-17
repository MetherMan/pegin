/**
 * 크리쳐 베이스 인터페이스 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-08-27 8:01p $
 * @version	$Revision: 109 $
 *
 * <HR>
 * $Log: /Engine/Inc/IW3DCreature.h $
 * 
 * 109   04-08-27 8:01p Paran
 * 
 * 108   04-06-22 4:20p Paran
 * 
 * 107   04-06-08 3:57p Paran2
 * 
 * 97    04-03-12 6:40p Sckyj		운영자 작업중 - GetSex() 추가
 *
 * 96    04-03-12 2:11p Paran		Collision
 * 
 * 95    04-02-27 9:43p Kjmgo		펫추가 ^.^/
 * 
 * 94    04-02-03 6:35p Kjmgo		길드 이름 움하하!
 * 
 * 92    04-01-13 8:14p Kjmgo		Creature및IW3DMagic을 포인터레퍼런스로 관리하도록 수정
 * 
 * 91    04-01-06 12:01a Kjmgo		공속 추가
 * 
 * 90    04-01-03 11:10a Kjmgo		SetVisible 추가
 * 
 * 89    03-12-31 3:18p Kjmgo		마법쪽 버그 수정 
 * 
 * 88    03-12-31 2:44p Paran		스킬
 * 
 * 86    03-12-22 4:59p Sckyj		2D, 3D sound 분리
 * 
 * 84    03-12-05 12:49a Kjmgo		속도 관련 추가
 * 
 * 83    03-11-18 9:52p Kjmgo		GetHeight 관련 함수 수정및, 추가
 * 
 * 80    03-10-16 4:41p Sckyj		sound 재생관련 코드 수정
 * 
 * 76    03-10-02 4:33a Kjmgo		aabb추가
 * 
 * 75    03-10-01 6:40p Kjmgo		Skill적용
 * 
 * 73    03-10-01 11:38a Kjmgo		최상위 Creature에 ShowDamageForTarget 함수 추가 및, 하위 클래스 함수 추가
 * 
 * 72    03-09-23 9:54p Kjmgo		State중, Skill추가
 * 
 * 71    03-09-23 11:20a Kjmgo		Resurrect함수 추가
 * 
 * 70    03-09-22 2:01p Kjmgo		Creature 색상 설정및 알파값 설정 작업(완료)
 * 
 * 69    03-09-18 5:15p Kjmgo		Magic에서 Bone을 사용할수 있게 수정
 * 
 * 67    03-09-13 1:25p Kjmgo		마법 동작을 Creature Action에서 처리하도록 몇가지 옵션 추가
 * 
 * 64    03-09-09 5:45a Kjmgo		몬스터 속도 수정
 * 
 * 60    03-08-31 10:19p Kjmgo		Chracter 이동과 공격 수정 
 * 
 * 51    03-08-10 6:59p Admin		말 타고내리기 적용
 * 
 * 36    03-06-15 4:44p Paran		STATE_MAGIC 추가.
 * 
 * 35    03-06-14 11:18a Paran		Skill zzang
 * 
 * 34    03-06-13 3:02a Kjmgo		마법 체계 뒤엎음
 * 
 * 33    03-06-12 2:05p Paran		매직 관련작업
 * 
 * 32    03-06-06 9:58a Kjmgo		Shadow Dummy 추가
 * 
 * 31    03-06-03 11:40p Paran		조력자 맴버 추가
 * 
 * 30    03-05-26 9:06p Paran		사용스킬 별로 모델 사용.
 * 
 * 29    03-05-15 9:04p Paran		파티관련 이름출력.
 * 
 * 26    03-05-10 12:32p Ahastudio	SetLighting() 추가.
 *
 * 25    03-05-08 1:28a Paran		말풍선 추가.
 *
 * 22    03-04-30 7:11p Paran		스킬 선택 수정.
 *
 * 18    03-04-23 7:49a Ahastudio	동작들을 모두 virtual로 변경. 새로운 Attack 추가.
 *
 * 15    03-04-15 1:27a Paran		NPC 추가.
 *
 * 14    03-04-12 3:03p Paran		캐릭터 장착 아이템 변경 기능 추가.
 * <HR>
 *
 * @file	IW3DCreature.h
 */


#ifndef __Wind3D_Engine_Creature_Header__
#define __Wind3D_Engine_Creature_Header__


enum
{
	STATE_STOP = 0,
	STATE_MOVE,
	STATE_ATTACK,
	STATE_CASTINGMAGIC,
	STATE_MAGIC,
	STATE_DEAD,
	STATE_SKILL
}; //enum


enum
{
	NONE = 0,
	CHARACTER,
	MONSTER,
	NPC,
	MERCENARY,
	VEHICLE
}; //enum


/**
 * 캐릭터 정보.
 * @ingroup		Engine
 */
struct SW3DCharacterInfo
{
	char	szName[80];			///<

	BYTE	bSex;				///<

	WORD	wHair;				///<
	WORD	wFace;				///<
	WORD	wHand;				///<
	WORD	wUpper;				///<
	WORD	wLower;				///<
	WORD	wFoot;				///<
	WORD	wLongWeapon;		///<
	WORD	wShortWeapon;		///<
	WORD	wHelmet;			///<
	WORD	wShield;			///<

	int		nX;					///<
	int		nY;					///<
	BYTE	btLayer;			///<
	BYTE	btOffset;			///<	속도

	WORD	wRide;				///<	탈것


	BYTE	btSpeedType;		///<	속도의 종류 ( 0 - 공격속도, 1 - 전체 애니메이션 속도 )
	float	fSpeed;				///<	공속

	int		nGuildId;			///< 길드 아뒤
}; //struct SW3DCharacterInfo


/**
 * 몬스터 정보.
 * @ingroup		Engine
 */
struct SW3DMonsterInfo
{
	WORD	wType;				///<

	int		nX;					///<
	int		nY;					///<
	BYTE	btOffset;			///<	평화
	BYTE	btOffset2;			///<	전투
	BYTE	btState;			///<	상태
}; //struct SW3DMonsterInfo


/**
 * NPC 정보.
 * @ingroup		Engine
 */
struct SW3DNPCInfo
{
	WORD	wType;				///<

	Vector3	vPos;				///<
	float	fYaw;				///<
}; //struct SW3DNPCInfo


/**
 * 용병 정보.
 * @ingroup		Engine
 */
struct SW3DMercenaryInfo
{
	WORD	wType;				///< 
	int		nX;					///<
	int		nY;					///<
}; //struct SW3DMercenaryInfo

/**
 * 탈것에 대한 정보
 * @ingroup		Engine
 */
struct SW3DVehicle
{
	WORD	wType;				///<
	WORD	wId;				///<
	UINT	nSex;				///< 
	int		nX;					///<
	int		nY;					///<
}; //struct SW3DVehicle

/**
 *
 */
struct SW3DDamage
{
	WORD			wDamage;			///< 데미지
	BYTE			btCritical;			///< 크리티컬 여뷰
	IW3DCreature*	pCreature;			///< 데미지가 뜰 대상
}; //struct SW3DDamage


/**
 * 크리쳐 베이스 인터페이스.
 * @ingroup		Engine
 */
class IW3DCreature : public IComponent
{
public:
	/**
	 * 크리쳐를 만든다.
	 *
	 * @return	생성 성공 여부.
	 */
	virtual BOOL	Create(
		IW3DWorld*		pWorld,		///< [IN] 월드 객체
		const DWORD		dwId,		///< [IN] 크리쳐 ID
		const void*		pData		///< [IN] 크리쳐 생성 정보
		) = 0;

	/**
	 * 모델을 변경한다.
	 */
	virtual BOOL	UpdateModel( 
		const WORD	wItemType,		///< [IN] 업데이트 ID
		const BOOL	bOn				///< [IN] 장착/해제
		) = 0;

	/**
	 * 크리쳐를 프레임 갱신한다.
	 *
	 * @param	dwTick		[IN] 경과 tick
	 *
	 * @return	계속 존재하는지 여부
	 */
	virtual BOOL	Update( const DWORD dwTick ) = 0;

	/**
	 * 크리쳐를 출력한다.
	 */
	virtual void	Draw() = 0;

	/**
	 * 크리쳐의 그림자를 출력한다.
	 */
	virtual void	DrawShadow() = 0;

	/**
	 * 크리쳐의 더미 그림자 모델을 설정한다.
	 */
	virtual void	SetShadowDummy( IW3DModel* pDummy ) = 0;

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
	 * 이동 목표를 설정한다.
	 *
	 * @param	vPos		[IN] 이동 목적지
	 */
	virtual void	SetGoalPosition( const Vector3& vPos ) = 0;

	/**
	 * 현재 위치 설정한다.
	 *
	 * @param	vPos		[IN] 위치
	 */
	virtual void	SetPosition( const Vector3& vPos ) = 0;

	/**
	 * 크리쳐의 위치를 얻는다.
	 *
	 * @return	크리쳐의 위치
	 */
	virtual Vector3		GetPosition() = 0;

	/**
	 *
	 *
	 */
	virtual Vector3		GetGoalPosition() = 0;

	/**
	 * 크리쳐의 방향을 설정한다.
	 *
	 * @param	vDir		[IN] 방향
	 */
	virtual void	SetDirection( const Vector3& vDir ) = 0;

	/**
	 * 크리쳐의 방향을 설정한다.
	 *
	 * @param	nAngle		[IN] 방향
	 */
	virtual void	SetDirection( const int nAngle ) = 0;
	
	/**
	 * 크리쳐의 방향을 얻는다.
	 */
	virtual Vector3	GetDirection() = 0;

	/**
	 *
	 */
	virtual Quaternion GetQuaternion() = 0;

	/**
	 * 크리쳐의 종류를 얻는다.
	 *
	 * @return	크리쳐 타입
	 */
	virtual int		GetType() = 0;

	/**
	 * 크리쳐 ID를 얻는다.
	 *
	 * @return	크리쳐 ID
	 */
	virtual DWORD	GetId() = 0;

	/**
	 * 크리쳐 이름을 얻는다.
	 *
	 * @return	크리쳐 이름
	 */
	virtual const char*		GetName() = 0;

	/**
	 * Set Color of Character Name 
	 */
	virtual void			SetNameColor( const DWORD dwColor ) = 0;

	/**
	 * 이름보이기.
	 *
	 * @param	보이기선택
	 */
	virtual void	VisibleName( BOOL bVisible ) = 0;

	/**
	 * 케릭터 대화 문자열을 설정한다.
	 *
	 * @param	말풍선 내용
	 */
	virtual void	SetTalkText( char* szText ) = 0;


	/**
	 * 빛 적용 여부를 설정한다.
	 * 빛을 적용하지 않으면 원색 그대로 출력된다.
	 * 즉, 주변의 오브젝트에 비해 상대적으로 밝게 보인다.
	 * 크리쳐를 선택하거나 하는 경우에 사용하면 효과적이다.
	 *
	 * @param	bLighting	[IN] 빛 적용 여부.
	 */
	virtual void	SetLighting( const BOOL bLighting ) = 0;

	/**
	 * 배역설정.
	 */
	virtual void	SetCastContent( 
		const WORD wShop,					///< [IN] 상점 번호.
		const WORD wQuest,					///< [IN] 퀘스트 번호.
		const BYTE btHaveStorage,			///< [IN] 창고 유무.
		const BOOL bGuildMgr				///< [IN] 단 관리
		) = 0;

	/**
	 * 배역 얻기.
	 */
	virtual void	GetCastContent( 
		WORD& wShop,					///< [OUT] 상점 번호.
		WORD& wQuest,					///< [OUT] 퀘스트 번호.
		BYTE& btHaveStorage,			///< [OUT] 창고 유무.
		BYTE& btGuildMgr				///< [OUT] 단 관리.
		) = 0;

	/**
	 * 크리쳐의 상태를 설정한다.
	 *
	 * @param	nState		[IN] 크리쳐 상태
	 */
	virtual void	SetState( int nState ) = 0;

	/**
	 * 크리쳐의 상태를 얻는다.
	 *
	 * @return	크리쳐 상태
	 */
	virtual int 	GetState() = 0;
	
	/**
	 * 공격방법설정.
	 */
	virtual void	SetAttackMode( const int nSkill ) = 0;
	
	/**
	 * 사용매직정보 설정.
	 */
	virtual void	SetMagicInfo( const BYTE btMagicType, const BYTE btMagicPickType ) = 0;

	/**
	 * 사용중인 스킬적용정보.
	 */
	virtual BOOL	GetApplyInfoSkill( BYTE* btSkillType, BYTE* btTaget ) = 0;

	/**
	 * 길찾기를 통해 목표점까지 이동한다.
	 */
	virtual BOOL	MoveTo( const Vector3& vPos ) = 0;

	/**
	 * 다른 크리쳐를 공격한다.
	 */
	virtual void	Attack(
		IW3DCreature*	pTarget			///< [IN] 공격 대상
		) = 0;
	
	/**
	 * 일반마법 사용한다.
	 */
	virtual void		CastMagic() = 0;

	/** 
	 * 마법추가.
	 */
	virtual void		AddMagic( 
		const WORD wMagic,				///< [IN] 마법종류
		const WORD wID,					///< [IN] 마법ID
		IW3DCreature* pTarget,			///< [IN] 마법대상
		BOOL bCompulsion = FALSE		///< 강제 시전후 ...											
		) = 0;

	/**
	 * 데미지를 입는다.
	 */
	virtual void	Damage( 
		BOOL			bBegin,				///< [IN] 공격 시작, 끝
		IW3DCreature*	pAttacker = NULL	///< [IN] 공격자.
		) = 0;
	
	/*
	 * 공격받은 수치설정.
	 */
	virtual void	Damage( 
		const WORD wDamage,				///< [IN] 데미지 수치.
		const BYTE btCritical			///< [IN] 크리티컬 데미지.
		) = 0;

	/**
	 * 데미지 정보를 얻어온다.
	 */
	virtual SW3DDamage* GetDamage() = 0;
	
	/**
	 * HP 설정.
	 */
	virtual void	SetHp(
		const int nHp,					///< [IN] HP
		const int nTotHP = 0			///< [IN] Total HP
		) = 0;

	/**
	 * HP 얻어오기.
	 */
	virtual void	GetHp( 
		int& nHP,						///< [IN] HP
		int& nTotHP						///< [IN] Total HP
		) = 0;

	virtual IW3DSoundObject3D* GetSound() = 0;///< sound object 반환
 
	virtual BOOL	IsShow() = 0;
	virtual BOOL	CanAttack() = 0;
	virtual BOOL	IsDead()	= 0;
	virtual void	SetFollowTarget( IW3DCreature* pFollow ) = 0;
	virtual void	SetTarget( BOOL bPlayer, BOOL bTarget = TRUE ) = 0 ;
	virtual void	StartMagic( DWORD dwID, DWORD dwMagicId, BOOL bTargetMonster, BOOL bHero = TRUE ) = 0;
	virtual void	SetMagic( IW3DMagic& pMagic ) = 0;
	virtual void	EndMagic( DWORD dwID ) = 0;
	virtual void	SetMagicTarget( IW3DMagic& pMagic ) = 0;
	virtual void	EndMagicTarget( DWORD dwID ) = 0;
	virtual void	Stop()			{ SetState( STATE_STOP ); }
	virtual void	Move()			{ SetState( STATE_MOVE ); }
	virtual void	Attack()		{ SetState( STATE_ATTACK ); }
	virtual void	Dead()			{ SetState( STATE_DEAD ); }
	virtual void	Ghost( BOOL bGhost ) = 0;
	virtual void	Hook() = 0;
	virtual BOOL	IsGhost() = 0;
	virtual BOOL	IsVehicleMode() = 0;							// 말을 사용하는지 여부 반환

	virtual void	SetHelper( const IW3DCreature* pCreature ) = 0;
	virtual void	RemoveHelper() = 0;
	virtual void	SetLayer() = 0;
	virtual void	SetLayer( BYTE btLayer ) = 0;
	virtual BYTE	GetLayer() = 0;

	virtual	void	AddVehicle( const void* pData ) = 0;
	virtual void    RemoveVehicle() = 0;
	virtual void	ChangeSpeed( BYTE btSpeed ) = 0;

	virtual BOOL	CastingMagic( DWORD dwID, DWORD dwMagicId ) = 0;
	virtual int		GetSkill() = 0;
	virtual UINT	GetSex() = 0;
	virtual void	SetAttackMove( BOOL bFlag ) = 0;

	////////////////////////////////////////////////////////////////

	virtual void	AddAttacker( IW3DCreature* pAttacker ) = 0;
	virtual void	RemoveAttacker( IW3DCreature* pAttacker ) = 0;

	virtual void	AddTarget( IW3DCreature* pTarget ) = 0;
	virtual void	RemoveTarget( IW3DCreature* pTarget ) = 0;

	virtual void	SetMode( BYTE btMode ) = 0;

	virtual IW3DModel* GetModel( int nPart ) = 0;
	virtual DWORD	GetTime() = 0;

	virtual void	SetAlpha( BYTE	btAlpha = 255 ) = 0;
	virtual void	SetColor( BYTE btRed, BYTE btGreen, BYTE btBlue ) = 0;
	virtual void	Resurrect() = 0;
	virtual void	ShowDamageForTarget() = 0;

	virtual void	UseSkill( WORD wSkillId ) = 0;
	virtual void	AddSkill( IW3DCreature* pTarget, const WORD wSkillId ) = 0;

	virtual BOOL	CheckPoint( const Vector3& v ) = 0;
	virtual float	GetMinDist()	 = 0;
	virtual float	GetHeight() = 0;
	virtual void	SetVisible( BOOL bVisible = TRUE ) = 0;
	virtual void	SetActionSpeed( DWORD dwTicks ) = 0;
	virtual void	SetShopInfo( const char* szTitle ) = 0;
	virtual char*	GetShopInfo() = 0;
	virtual BOOL	IsOpenShop() = 0;
	virtual void	SetGuildData( const char* szName, int nGuildId, BYTE btGuildLev, int nMark ) = 0;
	virtual void	ResetEnemy() = 0;
	virtual void	InsertPet( const BYTE btType, void* pData ) = 0;	
	virtual void	RemovePet() = 0;
	virtual void	UpdatePet( const char* szName, void* pData ) = 0;
}; //class IW3DCreature


#endif // #ifndef __Wind3D_Engine_Creature_Header__
