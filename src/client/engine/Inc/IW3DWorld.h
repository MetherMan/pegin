/**
 * 게임 월드 컴포넌트 인터페이스 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-08-27 8:01p $
 * @version	$Revision: 90 $
 *
 * <HR>
 * $Log: /Engine/Inc/IW3DWorld.h $
 * 
 * 90    04-08-27 8:01p Paran
 * 
 * 83    04-03-12 2:11p Paran			Collision
 * 
 * 82    04-03-10 6:17p Paran			기능조물 추가
 * 
 * 81    04-01-13 9:43p Kjmgo			사양 설정 적용
 * 
 * 80    03-12-26 11:51a Kjmgo			AddSkill함수 추가, ( 다른건 없고, AddMagic에서, 몇가지 수정 )
 * 
 * 78    03-12-18 4:12a Kjmgo			GetNearItem , 근처의 아이템을 하나 얻어 온다.
 * 
 * 77    03-12-17 11:21a Kjmgo			맵마다 시간 적용
 * 
 * 76    03-12-12 8:00p Kjmgo			피킹할떄 레이어 저장
 * 
 * 75    03-12-11 4:12a Kjmgo			Settime 함수 수정
 * 
 * 74    03-12-10 8:33p Kjmgo			길찾기 수정, Optimal을 선택적으로 구하도록 수정
 * 
 * 73    03-12-10 2:00p Kjmgo			길찾기및, 피킹 수정
 * 
 * 72    03-12-03 5:26p Kjmgo			시간 변화 추가
 * 
 * 71    03-12-03 2:32p Kjmgo			Fog관련 상태를 변경할수 있도록 수정
 * 
 * 70    03-12-03 9:45a Kjmgo			GetCamera 함수 추가
 * 
 * 68    03-12-01 4:17p Kjmgo			Weather Effect 추가
 * 
 * 67    03-12-01 2:28p Sckyj			GetAreaBGMType 함수 변경
 * 
 * 66    03-11-26 3:50p Sckyj			GetTime() 추가
 * 
 * 65    03-11-21 9:59a Paran			몹 아이템 드롭 수정
 * 
 * 64    03-11-04 9:08p Sckyj			AddNPC를 interface로 뺐음
 * 
 * 63    03-10-17 10:02a Kjmgo			Light를 키고 끄는 함수 추가
 * 
 * 58    03-09-22 9:07p Kjmgo			스킬 쪽 관련 작업( 마법 관리자 )
 * 
 * 48    03-06-19 8:31p Paran			맵 지역 분리
 * 
 * 45    03-06-13 3:02a Kjmgo			마법 체계 뒤엎음
 * 
 * 44    03-06-09 8:18p Kjmgo			크리티컬 수정
 * 
 * 41    03-06-03 11:41p Paran			용병 추가 삭제
 * 
 * 36    03-05-11 10:55a Ahastudio		AddEffect() 수정, RemoveEffect() 추가.
 *
 * 35    03-05-11 2:49a Kjmgo			타격치 추가
 *
 * 34    03-05-10 4:11p Ahastudio		PathFind() 함수 수정.
 *
 * 33    03-05-09 6:01a Ahastudio		게임 시간에 따른 빛 적용.
 *
 * 32    03-05-01 12:03a Ahastudio		SetTime() 추가.
 *
 * 29    03-04-17 5:39p Ahastudio		길찾기 추가.
 *
 * 25    03-04-15 1:26a Paran			NPC 추가.
 *
 * 20    03-03-25 3:36a Paran			인벤토리 아이템 서버와 연결.
 *
 * 16    03-03-17 8:44a Ahastudio		이동 가능 여부 알아내는 함수 - IsMovable - 추가.
 *
 * 14    03-03-14 9:26p Ahastudio		맵 로딩 부분 변경.
 *
 * 13    03-03-13 7:25p Ahastudio		마법 추가 수정.
 *
 * 12    03-03-12 8:16p Ahastudio		Movable 및 World Size 추가.
 *
 * 9     03-03-03 9:25p Kjmgo			마법 추가.
 *
 * 7     03-02-25 11:07a Ahastudio		Draw 내에서 존재하는 객체만 출력. GetTerrain() 추가.
 * <HR>
 *
 * @file	IW3DWorld.h
 */


#ifndef __Wind3D_Engine_World_Header__
#define __Wind3D_Engine_World_Header__


/**
 * 게임 월드 컴포넌트 인터페이스.
 * @ingroup		Engine
 */
class IW3DWorld : public IComponent
{
public:
	/**
	 * 초기화 함수.
	 *
	 * @param	pScene		[IN] Scene 객체.
	 * @param	pCamera		[IN] Camera 객체.
	 *
	 * @return	초기화 성공 여부.
	 */
	virtual BOOL	Init( 
		IW3DScene* pScene,
		IW3DCamera* pCamera ) = 0;

	/**
	 *	맵의 퀄리티를 설정한다.
	 *	0이 최상 1이 중간 2, 최하
	 */
	virtual void	SetMapQuality( const int nQuality ) = 0;

	/**
	 * 게임 오브젝트 클린업.
	 */
	virtual void	CleanupObjects() = 0;

	/**
	 * 지형을 생성한다.
	 *
	 * @return		지형 생성 성공 여부.
	 */
	virtual BOOL	CreateTerrain(
		const DWORD		dwSize,			///< [IN] 맵 크기 (2m 단위)
		const char*		szTextureName	///< [IN] 맵 텍스처 이름
		) = 0;

	/**
	 * 맵을 불러온다.
	 *
	 * @param	szName		[IN] 맵 파일 이름
	 *
	 * @return	로딩 성공 여부.
	 */
	virtual BOOL	LoadMap( const char* szName, BOOL bReload = FALSE ) = 0;

	/**
	 *
	 *
	 *
	 */
	virtual BOOL	LoadIndoor( const char* szName ) = 0;

	/**
	 *
	 *
	 *
	 */
	virtual void	SetMapType( const BYTE btType ) = 0;
	 

	/**
	 * 빛 정보를 불러온다.
	 *
	 * @param	szName		[IN] 빛 정보 파일 이름
	 *
	 * @return	로딩 성공 여부.
	 */
	virtual BOOL	LoadLight( const char* szName ) = 0;

	/**
	 * 게임 월드 업데이트.
	 *
	 * @param	dwTick		[IN] 경과 tick
	 */
	virtual void	Update( const DWORD dwTick ) = 0;

	/**
	 * 게임 월드를 그려준다.
	 */
	virtual void	Draw() = 0;

	/**
	 * 시간을 설정한다.
	 */
	virtual void	SetTime( const int nTime, const int nNextTime, const DWORD dwNext ) = 0;

	/**
	 * 현재 시간을 반환한다.
	 */
	virtual int		GetTime() = 0;

	/**
	 * World에 캐릭터를 추가한다.
	 */
	virtual void	AddCharacter(
		const DWORD		dwId,		///< [IN] 캐릭터 ID
		const void*		pData		///< [IN] 캐릭터 정보
		) = 0;

	/**
	 * World에서 캐릭터를 삭제한다.
	 *
	 * @param	dwId		[IN] 캐릭터 ID
	 */
	virtual void	RemoveCharacter( DWORD dwId ) = 0;

	/**
	 * 캐릭터를 Picking한다.
	 *
	 * @return	Picking된 캐릭터
	 */
	virtual IW3DCreature*	PickCharacter(
		const Vector3&	vPick,	///< [IN] 바닥 좌표
		const Vector3&	vOrig,	///< 반직선의 시작점.
		const Vector3&	vDir	///< 반직선의 방향.
		) = 0;

	/**
	 * 캐릭터 모델을 갱신한다.
	 */
	virtual void	UpdateModelCharacter( 
		const DWORD		dwId,		///< [IN] 캐릭터 ID
		const WORD		wItemType,	///< [IN] 업데이트 부분 ID
		const BOOL		bOn			///< [IN] 장착/해제
		) = 0;

	/**
	 * World에 몬스터를 추가한다.
	 */
	virtual void	AddMonster(
		const DWORD		dwId,		///< [IN] 몬스터 ID
		const void*		pData		///< [IN] 몬스터 생성 정보
		) = 0;

	/**
	 * World에서 몬스터를 삭제한다.
	 *
	 * @param	dwId		[IN] 캐릭터 ID
	 */
	virtual void	RemoveMonster( DWORD dwId ) = 0;

	/**
	 * World에 NPC를 추가한다.
	 */
	virtual void	AddNPC( const DWORD dwId, const void* pData ) = 0;

	/**
	 * 몬스터를 Picking한다.
	 *
	 * @return	Picking된 몬스터
	 */
	virtual IW3DCreature*	PickMonster(
		const Vector3&	vPick,		///< [IN] 바닥 좌표
		const Vector3&	vOrig,		///< 반직선의 시작점.
		const Vector3&	vDir		///< 반직선의 방향.
		) = 0;
	
	/**
	 * NPC를 Picking한다.
	 *
	 * @return	Picking된 NPC
	 */
	virtual IW3DCreature*	PickNPC(
		const Vector3&	vPick,		///< [IN] 바닥 좌표
		const Vector3&	vOrig,		///< 반직선의 시작점.
		const Vector3&	vDir		///< 반직선의 방향.
		) = 0;

	/**
	 * 용병을 추가한다.
	 */
	virtual void			AddMercenary( 
		const BYTE btMercType,		///< [IN] 용병타입
		const DWORD dwOwnerId		///< [IN] 임자아이디
		) = 0;

	/**
	 * 용병을 Picking한다.
	 *
	 * @return	Picking된 용병
	 */
	virtual IW3DCreature*	PickMercenary( 
		const Vector3&	vPick,		///< [IN] 바닥 좌표
		const Vector3&	vOrig,		///< 반직선의 시작점.
		const Vector3&	vDir		///< 반직선의 방향.

		) = 0;

	/**
	 * 기능조뮬을 Picking한다.
	 *
	 * @return	Picking된 용병
	 */
	virtual IW3DFuncEntity*	PickFuncEntity( 
		const Vector3&	vPick,		///< [IN] 바닥 좌표
		const Vector3&	vOrig,		///< 반직선의 시작점.
		const Vector3&	vDir		///< 반직선의 방향.
		) = 0;

	/**
	 * 아이템을 추가한다.
	 */
	virtual void	AddItem(
		const DWORD	dwId,			///< [IN] 아이템 ID
		const WORD	wType,			///< [IN] 아이템 종류
		const int	nX,				///< [IN] 맵 상의 X 좌표
		const int	nY,				///< [IN] 맵 상의 Y 좌표
		const int	nVal = 0,		///< [IN] 아이텝 추가 정보
		const WORD	wModId = 0		///< [IN] 소유한 몹 Id
		) = 0;
	
	/**
	 * World에 타켓에서 얻은 아이템을 추가한다.
	 */
	virtual void	AddItemFromTarget( 
		BYTE btTargetType,			///< [IN] 타겟 타입
		DWORD dwId					///< [IN] 타겟 아이디
		) = 0;

	/**
	 * World에서 아이템을 삭제한다.
	 *
	 * @param	dwId		[IN] 아이템 ID
	 */
	virtual void	RemoveItem( DWORD dwId ) = 0;

	/**
	 * 아이템을 Picking한다.
	 *
	 * @return	Picking된 아이템
	 */
	virtual IW3DItem*		PickItem(
		const Vector3&	vPick,		///< [IN] 아이템 ID
		const Vector3&	vOrig,		///< 반직선의 시작점.
		const Vector3&	vDir		///< 반직선의 방향.
		) = 0;

	/**
	 * 근처의 아이템을 Get한다.
	 * 
	 * @return Get한 아이템
	 */
	virtual IW3DItem*		GetNearItem( const Vector3&	vPos ) = 0;
	
	/**
	 * 아이템 이름 출력 여부.
	 */
	virtual void			ShowItemName( BOOL bShow ) = 0;

	/**
	 * World에 Effect를 추가한다.
	 *
	 * @return	이펙트 ID
	 */
	virtual DWORD	AddEffect(
		const char*			szName,			///< [IN] 이펙트 이름
		const Vector3&		vPos,			///< [IN] 이펙트 위치
		const Quaternion&	qRot = Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion(),
											///< [IN] 이펙트 방향
		const BOOL			bLoop = FALSE	///< [IN] 반복 여부
		) = 0;

	/**
	 * World에서 Effect를 삭제한다.
	 *
	 * @param	pEffect		[IN] 이펙트 ID
	 */
	virtual void	RemoveEffect( DWORD dwId ) = 0;

	virtual IW3DSpecialEffect* LoadEffect( const char*	szName ) = 0;

	virtual void	UnloadEffect( IW3DSpecialEffect* pEffect, const char* szName ) = 0;


	/**
	 * World에 파티클을 추가한다.
	 */
	virtual void	AddParticle(
		const char*		szType,		///< [IN] 파티클 종류
		const Vector3&	vPos		///< [IN] 파티클 위치
		) = 0;

	/**
	 * 마법을 등록합니다.
	 */
	virtual IW3DMagic*	AddMagic(
		const char* szMagic,		///< [IN] 마법 이름.
		IW3DCreature* pAttacker,	///< [IN] 시전자
		IW3DCreature* pTarget,		///< [IN] 피시전자.
		BOOL		bSendEndAttack = TRUE
		) = 0;

	/**
	 * 마법을 등록합니다.
	 */
	virtual IW3DMagic*	AddSkill(
		const char* szMagic,		///< [IN] 마법 이름.
		IW3DCreature* pAttacker,	///< [IN] 시전자
		IW3DCreature* pTarget		///< [IN] 피시전자.
		) = 0;

	virtual void	AddHit(
		DWORD dwDamage,				///< [IN] 데미지
		Vector3 vPos,				///< [IN] 데미지 입은 위치	
		DWORD	dwColor,			///< [IN] 타격치 색
		BYTE	bCritical			///< [IN] 크리티컬 여부
		) = 0;

	virtual IW3DMagicMgr* GetMagicMgr() = 0;

	/**
	 * 바닥의 높이를 구한다.
	 *
	 * @param	vPos			[IN] 구하고자 하는 곳의 좌표
	 *
	 * @return	바닥의 높이
	 */
	virtual float	GetHeight( const Vector3& vPos ) = 0;

	/**
	 * 수면의 높이를 구한다.
	 */
	virtual float	GetWaterHeight() = 0;

	/**
	 * 이동 가능한 부분인지 검사한다.
	 */
	virtual BOOL	IsMovable( int nX, int nY, int nLayer = -1 ) = 0;

	/**
	 * 바닥을 Picking한다.
	 *
	 * @return	Picking 됐는지 여부
	 */
	virtual int	PickTerrain(
		const int		nLayer,		///< [IN] 레이어 값
		const float		fHeight,	///< [IN] 바닥 높이
		const int		nX,			///< [IN] 화면상의 X좌표
		const int		nY,			///< [IN] 화면상의 Y좌표
		Vector3*		pvPick		///< [OUT] Picking된 좌표
		) = 0;

	/**
	 * 문을 Picking한다.
	 * 
	 * @return Picking된 문의 ID
	 */
	virtual DWORD	PickDoor( 
		const Vector3&	vPick,		///< [IN] 바닥 좌표
		const Vector3&	vOrig,		///< 반직선의 시작점.
		const Vector3&	vDir		///< 반직선의 방향.
		) = 0;

	/**
	 * 길찾기를 한다.
	 */
	virtual int		PathFind(
		const Vector3&		vStart,		///< [IN] 시작점
		const Vector3&		vTarget,	///< [IN] 목적지
		Vector3**			ppPath,		///< [OUT] 경로
		BOOL				bOptimal = FALSE
		) = 0;

	virtual IW3DCreature*		BeginCharacter() = 0;
	virtual	IW3DCreature*		NextCharacter() = 0;

	/**
	 * 캐릭터 ID로 캐릭터 오브젝트를 얻는다.
	 */
	virtual IW3DCreature*		GetCharacter( DWORD dwId ) = 0;

	/**
	 * 몬스터 ID로 몬스터 오브젝트를 얻는다.
	 */
	virtual IW3DCreature*		GetMonster( DWORD dwId ) = 0;
	
	/**
	 * NPC 오브젝트를 얻는다.
	 *
	 * @param		[IN] NPC 리스트카운트.
	 * @return		NPC
	 */
	virtual IW3DCreature*		GetNPC( const WORD wCount ) = 0;
	
	/**
	 * 아이템 ID를 이용해서 아이템 오브젝트를 얻는다.
	 */
	virtual IW3DItem*			GetItem( DWORD dwId ) = 0;
	
	/**
	 * 기능 옵젝을 얻는다. 
	 */
	virtual IW3DFuncEntity*		GetFuncEntity( const DWORD dwId ) = 0;

	/**
	 * Terrain을 얻는다.
	 */
	virtual IW3DTerrain*		GetTerrain() = 0;

	/**
	 * Movable을 얻는다.
	 */
	virtual BYTE*				GetMovable() = 0;

	/**
	 * Trace Effect를 얻는다.
	 */
	virtual IW3DTraceEffect*	GetTraceEffect() = 0;

	/**
	 * Particles을 얻는다.
	 */
	virtual IW3DParticles*		GetParticles() = 0;

	/**
	 * 아이템 정보 관리자를 얻는다.
	 */
	virtual IW3DItemInfoMgr*	GetItemInfoMgr() = 0;

	/**
	 *
	 */
	virtual IW3DCamera*			GetCamera() = 0;

	/**
	 * 월드 크기를 얻는다. ( 미터 단위 )
	 */
	virtual DWORD			GetSize() = 0;
	
	virtual void			AddMapArea( 		
		const	HRGN	hRgn,
		const	int		nBGMType
		) = 0;	

	virtual int	GetAreaBGMType( const int nX, const int nY, int* nAreaType ) = 0;	

//	virtual BOOL IsFishingPlace( Vector3& vPos ) = 0;
	/**
	 * 낚시터 영역 정보를 입력한다
	 *//*
	virtual void			AddFishingArea(
		const	HRGN	hRgn,
		const	int		nPlaceNum
		) = 0;*/

	/**
	 * Layer를 설정한다.
	 */
	virtual void SetLayer( BYTE btLayer ) = 0;

	/**
	 *
	 */
	virtual void SetSkyHeight( float fHeight ) = 0;

	/**
	 *
	 */
	virtual IW3DLight*	GetLight( int nIndex ) = 0;

	/**
	 *
	 */
	virtual	void		TurnLight( int nIndex, BOOL bOn ) = 0;	

	/**
	 *
	 */
	virtual void		SetWeather( int nWeather ) = 0;

	/**
	 *
	 */
	virtual void		SetFogState( DWORD dwFogColor, float fFogStart, float fFogEnd ) = 0;

	/**
	 *
	 */
	virtual void		LoadLightTable( int nId ) = 0;
	
	/**
	 * Collision with a Creature
	 */
	virtual IW3DCreature*		CollisionWithCreature( const IW3DCreature* pPlayer, const Vector3& vPos, const Vector3& vCurrPos ) = 0;
	
	/**
	 * Collision with the Entity
	 */
	virtual IW3DFuncEntity*		CollisionWithEntity( const Vector3& vPos, const Vector3& vCurrPos ) = 0;
}; //class IW3DWorld


#endif // #ifndef __Wind3D_Engine_World_Header__
