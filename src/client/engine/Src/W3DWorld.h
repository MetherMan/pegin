/**
 * 게임 월드.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-08-27 8:01p $
 * @version	$Revision: 13 $
 *
 * <HR>
 * $Log: /Engine/Src/W3DWorld.h $
 * 
 * 13    04-08-27 8:01p Paran
 * 
 * 5     04-03-12 2:11p Paran			Collision
 * 
 * 4     04-03-10 6:17p Paran			기능조물 추가
 * 
 * <HR>
 *
 * @file	W3DWorld.cpp
 */

#ifndef __World_Header__
#define __World_Header__

#include "Engine_Export.h"
#include "FlyingObject.h"
#include "Effect.h"
#include "PathFind.h"
#include "HitIndication.h"
#include "Loading.h"
#include "QuadTree.h"
#include "Indoor.h"
#include "Door.h"
#include "Entity.h"
#include "WeatherEffect.h"
#include "TimeMgr.h"
#include "Map.h"
#include <string>
#include <vector>
#include <list>
#include <map>


using std::string;
using std::vector;
using std::list;
using std::map;

#define MAX_LAYER 3
#define TILE_SIZE 4

/**
 * World 클래스.
 * @ingroup		Engine
 */
class CW3DWorld : public IW3DWorld
{
	SINGLETON_COMPONENT( CW3DWorld );

public:
	UNDER_MEMORY_MGR( CW3DWorld );

	CW3DWorld();
	~CW3DWorld();

	BOOL				Init( IW3DScene* pScene, IW3DCamera* pCamera );
	void				SetCamera( IW3DCamera* pCamera );
	void				CleanupObjects();

	BOOL				CreateTerrain( const DWORD dwSize, const char* szTextureName );
	BOOL				LoadMap( const char* szName, BOOL bReload );
	BOOL				LoadIndoor( const char* szName );
	BOOL				LoadLight( const char* szName );

	void				SetMapType( const BYTE btType );

	void				Update( const DWORD dwTick );
	void				Draw();

	void				SetTime( const int nTime, const int nNextTime, const DWORD dwTime );
	int					GetTime();
	void				SetLayer( BYTE btLayer );
	
	void				AddMapArea( const HRGN	hRgn, const int	nBGMType );
	int					GetAreaBGMType( const int nX, const int nY, int* nAreaType );

//	BOOL				IsFishingPlace( Vector3& vPos );
//	void				AddFishingArea( const HRGN hRgn, const int nPlaceNum );

	void				AddCharacter( const DWORD dwId, const void* pData );
	void				RemoveCharacter( DWORD dwId );
	IW3DCreature*		PickCharacter( const Vector3& vPick, const Vector3&	vOrig, const Vector3& vDir );
	void				UpdateModelCharacter( const DWORD dwId, const WORD wItemType, const BOOL bOn );

	void				AddMonster( const DWORD dwId, const void* pData );
	void				RemoveMonster( DWORD dwId );
	IW3DCreature*		PickMonster( const Vector3& vPick, const Vector3& vOrig, const Vector3& vDir );

	void				AddNPC( const DWORD dwId, const void* pData );
	IW3DCreature*		PickNPC( const Vector3& vPick, const Vector3& vOrig, const Vector3& vDir );

	void				AddMercenary( const BYTE btMercType, const DWORD dwOwnerId );
	IW3DCreature*		PickMercenary( const Vector3& vPick, const Vector3& vOrig, const Vector3& vDir );

	BOOL				AddFuncEntity( const string& strModelName, const CEntity* pEntity );
	IW3DFuncEntity*		PickFuncEntity( const Vector3& vPick, const Vector3& vOrig, const Vector3& vDir );

	void				AddItem( const DWORD dwId, const WORD wType, const int nX, const int nY, const int nVal, const WORD	wModId );
	void				AddItemFromTarget( BYTE btTargetType, DWORD dwId );
	void				RemoveItem( DWORD dwId );
	IW3DItem*			PickItem( const Vector3& vPick, const Vector3& vOrig, const Vector3& vDir );
	void				ShowItemName( BOOL bShow );
	
	DWORD				AddEffect( const char* szName, const Vector3& vPos, const Quaternion& qRot, const BOOL bLoop );
	void				RemoveEffect( DWORD dwId );
	IW3DSpecialEffect*	LoadEffect( const char* szName );
	void				UnloadEffect( IW3DSpecialEffect* pEffect, const char* szName );

	void				AddParticle( const char* szType, const Vector3& vPos );
	IW3DMagic*			AddMagic( const char* szMagic, IW3DCreature* pAttacker, IW3DCreature* pTarget, BOOL bSendEndAttack = TRUE );
	IW3DMagic*			AddSkill( const char* szMagic, IW3DCreature* pAttacker, IW3DCreature* pTarget );
	void				AddHit( DWORD dwDamage, Vector3 vPos, DWORD dwColor, BYTE bCritical );

	float				GetHeight( const Vector3& vPos );
	float				GetWaterHeight();

	BOOL				IsMovable( int nX, int nY, int nLayer = -1 );
	IW3DCreature*		CollisionWithCreature( const IW3DCreature* pPlayer, const Vector3& vPos, const Vector3& vCurrPos );
	IW3DFuncEntity*		CollisionWithEntity( const Vector3& vPos, const Vector3& vCurrPos );

	BOOL				PickTerrain( const int nLayer, const float fHeight, const int nX, const int nY, Vector3* pvPick );
	DWORD				PickDoor( const Vector3& vPick, const Vector3& vOrig, const Vector3& vDir );

	int					PathFind( const Vector3& vStart, const Vector3& vTarget, Vector3** ppPath, BOOL bOptimal = FALSE );

	IW3DCreature*		BeginCharacter();
	IW3DCreature*		NextCharacter();
	IW3DCreature*		GetCharacter( DWORD dwId );
	IW3DCreature*		GetMonster( DWORD dwId );
	IW3DCreature*		GetNPC( const WORD wCount );
	IW3DItem*			GetItem( DWORD dwId );
	IW3DFuncEntity*		GetFuncEntity( const DWORD dwId );

	IW3DTerrain*		GetTerrain();
	BYTE*				GetMovable();

	IW3DTraceEffect*	GetTraceEffect();
	IW3DParticles*		GetParticles();

	IW3DItemInfoMgr*	GetItemInfoMgr();

	DWORD				GetSize();

	IW3DMagicMgr*		GetMagicMgr()
	{
		return m_pMagic;
	} //GetMagicMgr

	void SetSkyHeight( float fHeight )
	{
		//-- NULL --//
	} //SetSkyHeight

	IW3DLight*	GetLight( int nIndex )
	{
		if( m_pLightMgr )
		{
			return m_pLightMgr->GetLight( nIndex );
		} //if

		return NULL;
	} //GetLight

	void		TurnLight( int nIndex, BOOL bOn )
	{
		if( m_pLightMgr )
		{
			m_pLightMgr->Turn( nIndex, bOn );
		} //if
	} //TurnLight

	void		SetWeather( int nWeather )
	{
		if( m_pWeather )
		{
			m_pWeather->SetWeather( nWeather );
		} //if
	} //SetWeather

	/**
	 *
	 */
	IW3DCamera*			GetCamera()
	{
		return m_pCamera;
	} //GetCamera

	/**
	 *
	 */
	void		SetFogState( DWORD dwFogColor, float fFogStart, float fFogEnd )
	{
		m_dwFogColor = dwFogColor;
		m_fFogStart = fFogStart;
		m_fFogEnd = fFogEnd;
	} //SetFogState

	void		LoadLightTable( int nId );

	/**
	 *
	 */
	IW3DItem*	GetNearItem( const Vector3&	vPos );

	/**
	 *	맵의 퀄리티를 설정한다.
	 *	0이 최상 1이 중간 2, 최하
	 */
	void	SetMapQuality( const int nQuality )
	{
		m_nMapQuality = nQuality;
	} //SetMapQuality
	
private:

	/**
	 * Area BGM 용 영역 구조체
	 */
	struct _AreaRegion
	{
		HRGN		hRgn;				///< 현재 지역 좌표
		int			nBGMType;			///< BGM Type
	}; //struct _AreaRegion	

	/**
	 * 낚시터 영역 구조체
	 */
/*	struct _FishRegion
	{
		HRGN		hRgn;				///< 낚시터 영역
		UINT		nNum;				///< 낚시터 번호
	};*/

	/**
	 * 
	 */
	struct SItemOwner
	{
		BYTE			btOwnerType;		///< [IN] 소유자 타입
		DWORD			dwOwnerId;			///< [IN] 소유자 아이디
		IW3DItem*		pItem;				///< [IN] 소유 아이템
	}; //struct SItemOwner

	typedef map<string, IW3DModel*>			ModelMap;
	typedef map<string, IW3DSpecialEffect*> EffectMap;
	typedef map<DWORD, IW3DCreature*>		CreatureMap;
	typedef map<DWORD, IW3DItem*>			ItemMap;
	typedef list<SItemOwner*>				ItemOwnerList;
	
	typedef list<CEntity*>					EntityList;
	typedef list<IW3DCreature*>				CreatureList;
	typedef list<CEffect*>					EffectList;
	typedef list<IW3DEffectObject*>			EffectObjectList;
	typedef map<DWORD, IW3DFuncEntity*>		FuncEntityMap;

	typedef vector<_AreaRegion>				AreaRgnVector;		///< Area BGM용 영역 벡터
//	typedef vector<_FishRegion>				FishRgnVector;		///< 낚시터 영역 벡터

	IW3DScene*				m_pScene;			///< Scene 객체
	IW3DCamera*				m_pCamera;			///< Camera 객체


	CMap*					m_pMap;				///< Map

	DWORD					m_dwTick;			///< 내부 누적 tick
	DWORD					m_dwOldTick;		///< 저번 누적 tick
	DWORD					m_dwSize;			///< 월드 크기
	DWORD					m_dwFogColor;		///< 안개 컬러

	float					m_fFogStart;		///< 안개 시작
	float					m_fFogEnd;			///< 안개 끝

	int						m_nTime;			///< World상의 시간
	int						m_nLayer;			///< Layer
	int						m_nMapQuality;		///< 맵의 퀄리티

	IW3DItemInfoMgr*		m_pItemInfoMgr;		///< 아이템 정보 관리자
	IW3DParticles*			m_pParticles;		///< 파티클
	IW3DTraceEffect*		m_pTraceEffect;		///< 흔적 효과
	IW3DMagicMgr*			m_pMagic;			///< 마법 구현

	
	IW3DTerrain*			m_pTerrain;			///< 지형
	BYTE*					m_pbMovable[MAX_LAYER];		///< 이동 속성 버퍼
	CPathFind*				m_pPathFind[MAX_LAYER];		///< 길찾기 객체

	EffectObjectList		m_listEffectObject; ///< EffectObject 리스트
	ModelMap				m_mapModel;			///< 맵 모델
	IW3DBillboard*			m_pGrass;			///< 풀

	IW3DModel*				m_pShadowDummy;		///< Shadow Dummy
	CreatureMap				m_mapCharacter;		///< 캐릭터 리스트
	CreatureMap				m_mapMonster;		///< 몬스터 리스트
	CreatureList			m_listNPC;			///< NPC 리스트
	CreatureList			m_listMercenary;
	ItemMap					m_mapItem;			///< 아이템 리스트
	ItemOwnerList			m_listWaitItem;		///< 대기 아이템 리스트
	CFlyingObject*			m_pFlyingObject;	///< 날으는 객체 관리자
	CHitIndication*			m_pHit;				///< 타격치 출력 관리자
	EffectMap				m_mapEffect;		///< 이펙트 Map
	EffectList				m_listEffect;		///< 이펙트 리스트
	FuncEntityMap			m_mapFuncEntity;	///< 기능 모델

	CreatureMap::iterator	m_itorCharacter;	///< 케릭터 지시자
	
	AreaRgnVector			m_vecAreaRgn;		///< Area BG< 지역
//	FishRgnVector			m_vecFishRgn;		///< 낚시터 지역

	CQuadTree*				m_pQuadTree;		///< Quad Trees

	IW3DLightMgr*			m_pLightMgr;		///< 라이트 관리자
	IW3DLight*				m_pLight[3];		///< 빛

	CTimeMgr*				m_pTimeMgr;			///< 시간 관리자

	CWeather*				m_pWeather;			///< 날씨 관리자.
	
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=---=-=-=-=-=-=-=-=-=-=-=-=//
	//-=-=-=-=-=-=-=-=-=-=-=-=- Door -=-=-=-=-=-=-=-=-=-=-=-=-//
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=---=-=-=-=-=-=-=-=-=-=-=-=//
	CDoor*					m_pDoor;

	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=---=-=-=-=-=-=-=-=-=-=-=-=//
	//-=-=-=-=-=-=-=-=-=-=-=-= Map Type -=-=-=-=-=-=-=-=-=-=-=//
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=---=-=-=-=-=-=-=-=-=-=-=-=//
	BYTE					m_btMapType;

	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=---=-=-=-=-=-=-=-=-=-=-=-=//
	//-=-=-=-=-=-=-=-=-=-=-=-= Indoor -=-=-=-=-=-=-=-=-=-=-=-=//
	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=---=-=-=-=-=-=-=-=-=-=-=-=//
	CIndoor*				m_pIndoor;
	BOOL					m_bIndoor;

	//-=-=-=-=-=-=-=-=-=-=-=-= Indoor용 List -=-=-=-=-=-=-=-=-=-=-=-=//
	CreatureList			m_listNPCForIndoor;			///< NPC 리스트

	
public:


	void		Cleanup();

	void		CleanupEffect();
	void		CleanupSpecialEffect();
	void		CleanupItem();
	void		CleanupNPC();
	void		CleanupMercenary();
	void		CleanupFuncEntity();
	void		CleanupMonster();
	void		CleanupCharacter();
	void		CleanupModel();
	void		CleanupEntity();
	void		CleanupEffectObject();
	void		CleanupIndoor();

	void		LoadDoors( string& sName );
	void		LoadBlocks( Archive& ar, const CLoading* pLoading );
	void		LoadMapEntities( Archive& ar, const CLoading* pLoading );
	void		LoadMapEntity( const string& sModelName, const Vector3& vPos, const float fYaw );
	IW3DModel*	LoadMapEntityModel( const string& sModelName );
	int			GetNpcByModelName( const string& sModelName );

	void		UpdateEntities( const DWORD dwTick );
	void		UpdateCharacters( const DWORD dwTick );
	void		UpdateMonsters( const DWORD dwTick );
	void		UpdateNPC( const DWORD dwTick );
	void		UpdateMercenary( const DWORD dwTick );
	void		UpdateItems( const DWORD dwTick );
	void		UpdateFuncEntity( const DWORD dwTick );

	void		DrawCharacters( const BOOL bShadow = FALSE );
	void		DrawMonsters( const BOOL bShadow = FALSE );
	void		DrawNPC( const BOOL bShadow = FALSE );
	void		DrawMercenary( const BOOL bShadow = FALSE );
	void		DrawItems( const BOOL bShadow = FALSE );
	void		DrawFuncEntity( const BOOL bShadow = FALSE );
	void		DrawEffects();

	void		AddCharacter( IW3DCreature* pCreature );

	CCriticalSection* m_pCriticalSection;
}; //class CW3DWorld

#endif //#ifndef __World_Header__