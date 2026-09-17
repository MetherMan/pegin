/**
 * 마법 처리 구현부.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 05-02-19 7:11p $
 * @version	$Revision: 70 $
 *
 * <HR>
 * $Log: /Engine/Src/W3DMagic.cpp $
 * 
 * 70    05-02-19 7:11p Sk8snow2
 * 
 * 69    04-12-09 6:28p Sk8snow2
 * 
 * 68    04-01-13 8:14p Kjmgo
 * Creature및IW3DMagic을 포인터레퍼런스로 관리하도록 수정
 * 
 * 67    04-01-10 12:57p Kjmgo
 * 
 * 66    04-01-10 12:50p Kjmgo
 * 
 * 65    04-01-10 10:39a Kjmgo
 * 
 * 64    04-01-06 12:01a Kjmgo
 * 버그 수정및, 코드 정리
 * 
 * 63    03-12-26 11:57a Kjmgo
 * 스킬 관련 코드 추가
 * 
 * 62    03-12-26 11:56a Kjmgo
 * Skill관련 추가 
 * 
 * 61    03-12-26 11:51a Kjmgo
 * AddSkill함수 추가, ( 다른건 없고, AddMagic에서, 몇가지 수정 )
 * 
 * 60    03-12-14 2:34p Kjmgo
 * 
 * 59    03-12-04 4:58p Kjmgo
 * Model 관련 추가
 * 
 * 58    03-12-03 10:27a Kjmgo
 *  카메라 흔들기 추가
 * 
 * 57    03-11-01 3:42p Sckyj
 * 마법 이펙트 사운드 관련 코드 추가( 스크립트에서 사운드 재생 타입을
 * 읽어서 사운드 출력 )
 * 
 * 56    03-10-27 3:24p Kjmgo
 * 코드 최적화
 * 
 * 55    03-10-20 3:04p Kjmgo
 * Bone 추가
 * 
 * 54    03-10-16 12:14a Paran
 * 
 * 53    03-10-15 8:36p Kjmgo
 * 
 * 52    03-10-11 10:21a Kjmgo
 * 
 * 51    03-10-01 11:38a Kjmgo
 * BlowTiming 작업
 * 
 * 50    03-09-22 9:07p Kjmgo
 * 스킬 쪽 관련 작업( 마법 관리자 )
 * 
 * 49    03-09-18 5:15p Kjmgo
 * Magic에서 Bone을 사용할수 있게 수정
 * 
 * 48    03-09-13 4:37p Kjmgo
 * Billboard 로드 실패시 메모리 해제 하지 않았던것을 해제하도록 수정
 * 
 * 47    03-09-13 1:25p Kjmgo
 * 마법 동작을 Creature Action에서 처리하도록 몇가지 옵션 추가
 * 
 * 46    03-08-22 11:11a Kjmgo
 * 
 * 45    03-08-19 3:19a Kjmgo
 * 
 * 44    03-08-13 5:29p Kjmgo
 * 
 * 43    03-08-08 2:34p Admin
 * 
 * 42    03-06-28 10:16a Kjmgo
 * Assert 추가
 * 
 * 41    03-06-26 5:13p Kjmgo
 * 
 * 40    03-06-15 5:51p Kjmgo
 * 
 * 39    03-06-13 3:02a Kjmgo
 * 마법 체계 뒤엎음
 * 
 * 38    03-06-11 9:36p Kjmgo
 * Sound 추가
 * 
 * 37    03-05-11 9:23p Kjmgo
 * 
 * 36    03-05-03 11:07a Ahastudio
 *
 * 30    03-03-21 6:23a Kjmgo		Creature를 포인터로 안가지게 고침.
 *
 * 19    03-02-25 4:05a Kjmgo		베이지어 곡선 추가.
 * <HR>
 *
 * @file	W3DMagic.cpp
 */


#include "Engine_Export.h"
#include "Magic.h"
#include <stdio.h>

//////
/*#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h> */
///////


#define MAXSPEED	400
#define CHECK_KEYWORD( s )	!strcmp( szKeyword, s )
#define SET_CHAR( c, d ) 	sprintf( c, "%s", d )


/**
 *
 */
std::string GetBoneName( int nPart );

/**
 * 마법 컴포넌트 구현부.
 * @ingroup		Engine
 */
class CW3DMagicMgr : public IW3DMagicMgr
{
public:
	UNDER_MEMORY_MGR( MagicMgr );

	CW3DMagicMgr();
	~CW3DMagicMgr();

	// 마법을 쓰는 부분


	//-=-=-=-=-=-=-=-= Init & Add -=-=-=-=-=-=-=//
	BOOL		Init( IW3DWorld* pWorld );
	BOOL		Load( const char* szFileName );
	BOOL		AddMagic( const char* szFile );

	IW3DMagic*	AddMagic( const char* szMagic, IW3DCreature* pAttacker, IW3DCreature* pTarget, BOOL bSendEndAttack = TRUE );
	IW3DMagic*	AddSkill( const char* szMagic, IW3DCreature* pAttacker, IW3DCreature* pTarget );
	

	//-=-=-=-=-=-=-=-= Draw -=-=-=-=-=-=-=//
	void	Update( DWORD dwTick );
	void	Draw();

	//-=-=-=-=-=-=-=-= Load Effect -=-=-=-=-=-=-=//
	BOOL				LoadParticle( const char* szName );
	IW3DSpecialEffect*	LoadEffect( const char* szName );

	/**
	 * 
	 */
	IW3DTraceEffect*	GetTraceEffect()
	{
		Assert( m_pTrace );
		return m_pTrace;
	} //GetTraceEffect

	/**
	 *
	 */
	IW3DParticles*		GetParticles()
	{
		Assert( m_pParticles ); 
		return m_pParticles;
	} //GetParticles

	//-=-=-=-=-=-=-=-= Release -=-=-=-=-=-=-=//
	void	ReleaseAll();	
	
private:
	typedef std::map<std::string, IW3DSpecialEffect*>	EffectMap;
	typedef std::map<std::string, MAGIC*>				MagicMap;
	typedef std::list<IW3DMagic*>						MagicList;
	typedef std::map<std::string, std::string>			MagicNameMap;
	typedef std::map<std::string, IW3DModel*>			ModelMap;


	EffectMap			m_mapEffect;			///< 이펙트들의 List

	//-=-=-=-=-=-=-=-= Effect -=-=-=-=-=-=-=-=//

	IW3DParticles*		m_pParticles;			///< Particles
	IW3DTraceEffect*	m_pTrace;				///< Trace Effect

	//-=-=-=-=-=-=-=-= World -=-=-=-=-=-=-=-=//
	IW3DWorld*			m_pWorld;				///< World

	//-=-=-=-=-=-=-=-= Magic -=-=-=-=-=-=-=-=//
	MagicMap			m_mapMagic;	
	MagicNameMap		m_mapMagicName;			///< 마법들의 이름( 파일명 )
	MagicList			m_listMagic;			///< 화면에 그려줄 마법들
	DWORD				m_dwTick;				///< 보관용 경과 틱


	ModelMap			m_mapModel;				///< Effect용 Model Map

	//-=-=-=-=-=-=-=-= Load Script -=-=-=-=-=-=-=//
	MAGIC*				LoadScript( const char* szName );
	EFFECT*				ReadEffect( FILE* fp );
	PART*				ReadPart( FILE* fp );

	IW3DModel*			LoadModel( const char* szName );
}; //class CW3DMagic


IMPLEMENT_COMPONENT( CW3DMagicMgr, MagicMgr );

/**
 * 생성자
 */
CW3DMagicMgr::CW3DMagicMgr()
:	m_pParticles	( NULL ),
	m_pTrace		( NULL )
{
} //CW3DMagic::CW3DMagic

/**
 * 소멸자
 */
CW3DMagicMgr::~CW3DMagicMgr()
{
	ReleaseAll();
	SAFE_RELEASE( m_pTrace );
} //CW3DMagic::~CW3DMagic

/**
 * 초기화 함수.
 *
 * @param	pParticles		[IN] 파티클 객체
 *
 * @return	초기화 성공여부 
 */
BOOL CW3DMagicMgr::Init( IW3DWorld* pWorld )
{
	Assert( pWorld );
	Assert( NULL == m_pTrace );
	Assert( NULL == m_pParticles );

	m_pWorld = pWorld;
	m_pParticles = pWorld->GetParticles();

	m_pTrace = MAKE_COMPONENT( TraceEffect );
	CHECK_PTR_RETURN( m_pTrace );

	if( !m_pTrace->Init() )
	{
		Assert( !"Trace Effect 생성 실패" );
		return FALSE;
	} //if

	Magic::SetWorld( m_pWorld );

	return TRUE;
} //CW3DMagicMgr::Init

/**
 * 마법을 등록한다.
 *
 * @return  등록 성공 여부 ( 마법의 존재 여부 )
 */
IW3DMagic* CW3DMagicMgr::AddMagic( 
						const char*		szMagic,	///< 마법이름
						IW3DCreature*	pAttacker,  ///< 공격자
						IW3DCreature*	pTarget,		///< 타격자
						BOOL			bSendEndAttack
						)
{
	Assert( pAttacker );
	Assert( pTarget );
	Assert( szMagic );

	if( !pAttacker || !pTarget || !szMagic )
	{
		return NULL;
	} //if

	MAGIC* pData = NULL;
	MagicMap::iterator itor = m_mapMagic.find( std::string( szMagic ) );

	if( itor == m_mapMagic.end() )
	{// 없따아!!!
		return NULL;
	} //if

	pData = itor->second;
	Assert( pData );

	if( !pData )
	{
		return NULL;
	} //if

	IW3DMagic* pMagic = MAKE_COMPONENT( Magic );

	if( !pMagic || !pMagic->Create( *pAttacker, *pTarget, this, pData, bSendEndAttack ) )
	{
		DEBUG_OUT( "Add Magic 실패" );
		SAFE_RELEASE( pMagic );
		return  NULL;
	} //if

	//-=-=-=-=-=-=-=-=-=-=-=-= Insert Magic In the list -=-=-=-=-=-=-=-=-=-=-=-=//
	m_listMagic.push_back( pMagic );
	//pAttacker->StartMagic();
	
	if( bSendEndAttack )
	{
		pAttacker->SetMagic( *pMagic );
		pTarget->SetMagicTarget( *pMagic );
	} //if

	return pMagic;
} //CW3DMagicMgr::AddMagic
/**
 *
 *
 */
IW3DMagic*	CW3DMagicMgr::AddSkill(
	const char*		szMagic,	///< 마법이름
	IW3DCreature*	pAttacker,  ///< 공격자
	IW3DCreature*	pTarget	///< 타격자
	)
{
	Assert( pAttacker );
	Assert( pTarget );

	if( !pAttacker || !pTarget )
	{
		return NULL;
	} //if

	MAGIC* pData = NULL;
	MagicMap::iterator itor = m_mapMagic.find( std::string( szMagic ) );

	if( itor == m_mapMagic.end() )
	{// 없따아!!!
		return NULL;
	} //if

	pData = itor->second;
	Assert( pData );

	IW3DMagic* pMagic = MAKE_COMPONENT( Magic );

	if( !pMagic || !pMagic->Create( *pAttacker, *pTarget, this, pData, FALSE ) )
	{
		DEBUG_OUT( "Add Magic 실패" );
		SAFE_RELEASE( pMagic );
		return  NULL;
	} //if

	pMagic->SetSkill();

	//-=-=-=-=-=-=-=-=-=-=-=-= Insert Magic In the list -=-=-=-=-=-=-=-=-=-=-=-=//
	m_listMagic.push_back( pMagic );
	//pAttacker->StartMagic();
	
	pAttacker->SetMagic( *pMagic );
	pTarget->SetMagicTarget( *pMagic );

	return pMagic;
} //CW3DMagicMgr::AddSkill

/**
 *
 */
BOOL CW3DMagicMgr::AddMagic( const char* szMagic )
{
	MAGIC* pMagic = LoadScript( szMagic );

	if( !pMagic )
	{
		Assert( !"마법 스크립트 오류" );
		return FALSE;
	} //if
	
	m_mapMagic.insert( std::make_pair( std::string( szMagic ), pMagic ) );
	m_mapMagicName.insert( std::make_pair( std::string( szMagic ), std::string( szMagic ) ) );

	return TRUE;
} //CW3DMagicMgr::RegMagic

/**
 * 갱신.
 *
 * @param	dwTick		[IN]	누적되지 않은 경과 틱
 */
void CW3DMagicMgr::Update( DWORD dwTick )
{
	Assert( m_pWorld );

	if( m_listMagic.empty() )
	{
		return ;
	} //if

	m_dwTick = dwTick;

	MagicList::iterator itor = m_listMagic.begin();

	IW3DMagic* pMagic = NULL;
	IW3DCreature* pCreature = NULL;

	while( itor != m_listMagic.end() )
	{
		pMagic = (*itor);

		if( !pMagic )
		{
			itor = m_listMagic.erase( itor );
			continue;
		} //if

		if( !pMagic->Update( dwTick ) )
		{
			// Update에 실패한 이유는 단 하나.
			// 모든 단계가 끝났다는 것이다.
			// 끝난 Magic은 렌더링 리스트에서 지우자!

			if( pMagic->IsSendEndAttack() )
			{
				pCreature = pMagic->GetAttacker();

				if( pCreature )
				{
					pCreature->EndMagic( pMagic->GetID() );
					//pCreature->EndMagicTarget( pMagic->GetID() );
				} //if
		
				pCreature = pMagic->GetTarget();

				if( pCreature )
				{
					pCreature->EndMagicTarget( pMagic->GetID() );
				} //if				
			} //if		


			SAFE_RELEASE( pMagic );
			itor = m_listMagic.erase( itor );
		}
		else
		{
			itor++;
		} //if..else..
	} //while

	if( m_pTrace )
	{
		m_pTrace->Update( dwTick );
	} //if
} //CW3DMagicMgr::Update

/**
 * Draw.
 */
void CW3DMagicMgr::Draw()
{
	MagicList::iterator itor = m_listMagic.begin();

	IW3DMagic*	pMagic = NULL;

	while( itor != m_listMagic.end() )
	{
		pMagic = *itor;

		if( !pMagic || !pMagic->Draw() )
		{
			SAFE_RELEASE( pMagic );
			itor = m_listMagic.erase( itor );
			continue;
		} //if

		itor++;
	} //while

	if( m_pTrace )
	{
		m_pTrace->Render();
	} //if
} //CW3DMagicMgr::Draw

/**
 * Magic List를 읽어 온다.
 *
 * @param	szFileName		[IN] List파일 이름
 */
BOOL CW3DMagicMgr::Load( const char *szFileName )
{
	char szScript[256];

	sprintf( szScript, "Magic/%s", szFileName ); 

	FILE* fp = fopen( szScript, "rt" );
	if( NULL == fp )
	{
		// 파일이 없다.
		return FALSE;
	}

	char szBuffer[256];
	char szKeyword[80];
	char szDummy[128];

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( CHECK_KEYWORD( "[PTC]" ) )		// 파티클 Type 파일
		{
			if( sscanf( szBuffer, "%s %s", szKeyword, szDummy ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}


			if( !LoadParticle( szDummy ) )
			{
				Assert( !"파티클 Type생성 실패" );
			} //if
		}
		else if( CHECK_KEYWORD( "[MAGIC]" ) )	// 마법 이름 : 마법 스크립트 파일명
		{
			char szMagic[128];
			if( sscanf( szBuffer, "%s %s %s", szKeyword, szMagic, szDummy ) != 3 )
			{
				Assert( !"sscanf 오류" );
			}


			MAGIC* pMagic = LoadScript( szDummy );

			if( !pMagic )
			{
				Assert( !"마법 스크립트 오류" );
				continue;
			} //if
			
        	m_mapMagic.insert( std::make_pair( std::string( szMagic ), pMagic ) );
			m_mapMagicName.insert( std::make_pair( std::string( szMagic ), std::string( szDummy ) ) );
		}	
		else if( CHECK_KEYWORD( "[TRACE]" ) )	// Trace Effect 종류.
		{
			char szTrace[128];
			if( sscanf( szBuffer, "%s %s", szKeyword, szDummy ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			sprintf( szTrace, "Effect/%s", szDummy );

			if( !m_pTrace->Load( szTrace ) )
			{
				Assert( !"Trace 생성 실패" );
			} //if
		} //if..else if..
	} //while

	return TRUE;
} //CW3DMagicMgr::Load

/**
 *	모든 마법들을 릴리즈 한다.
 */
void CW3DMagicMgr::ReleaseAll()
{
	MagicMap::iterator itor = m_mapMagic.begin();
	MagicMap::iterator itorNext;

	while( itor != m_mapMagic.end() )
	{
		itorNext = itor;
		itorNext++;

		MAGIC* pMagic = itor->second;
		SAFE_DELETE( pMagic );

		m_mapMagic.erase( itor );
		itor = itorNext;
	} //while


	// Magic
	MagicList::iterator itorMagic = m_listMagic.begin();

	while( itorMagic != m_listMagic.end() )
	{
		IW3DMagic*	pMagic = *itorMagic;
		SAFE_RELEASE( pMagic );
		itorMagic = m_listMagic.erase( itorMagic );
	}

	//	SpecialEffect
	EffectMap::iterator		itorEffect = m_mapEffect.begin();

	while( itorEffect != m_mapEffect.end() )
	{
		EffectMap::iterator		itorNext = itorEffect;
		itorNext++;

		SAFE_RELEASE( itorEffect->second );
		m_mapEffect.erase( itorEffect );

		itorEffect = itorNext;
	} //while

	// Naming
	MagicNameMap::iterator	itorName = m_mapMagicName.begin();

	while( itorName != m_mapMagicName.end() )
	{
		MagicNameMap::iterator	itorNext = itorName;
		itorNext++;

		m_mapMagicName.erase( itorName );

		itorName = itorNext;
	} //while

	// Model
	ModelMap::iterator itorModel = m_mapModel.begin();
	
	while( itorModel != m_mapModel.end() )
	{
		ModelMap::iterator itorNext = itorModel;
		itorNext++;

		SAFE_RELEASE( itorModel->second );

		m_mapModel.erase( itorModel );
		itorModel = itorNext;
	} //while

} //CW3DMagicMgr::ReleaseAll

/**
 * 파티클을 읽어 옵니다.
 *
 * @param	szName [IN] 파일이름
 *
 * @author	로냐
 * @date	2003-02-21 오후 2:35
 */ 
BOOL CW3DMagicMgr::LoadParticle( const char* szName )
{
	char szFileName[256];

	sprintf( szFileName, "Effect/%s", szName );

	if( !m_pParticles->LoadParticleTypeFile( szFileName ) )
	{
		return FALSE;
	} //if

	return TRUE;
} //CW3DMagicMgr::LoadParticle

/**
 * Special Effect을 읽어 옵니다.
 *
 * @param	szName [IN] 파일이름
 *
 * @return	성공이면 Effect를 리턴한다.
 *
 * @author	로냐
 * @date	2003-02-21 오후 2:35
 */ 
IW3DSpecialEffect* CW3DMagicMgr::LoadEffect( const char* szName )
{
	EffectMap::iterator itor = m_mapEffect.find( std::string( szName ) );

	if( itor != m_mapEffect.end() )
	{
		// 이펙트가 이미 있다.
		return itor->second;
	} //if

	IW3DSpecialEffect* pEffect = MAKE_COMPONENT( SpecialEffect );

	if( !pEffect )
	{
		Assert( !"메모리 생성 실패" );
		return NULL;
	} //if

	if( !pEffect->Load( szName ) )
	{
		SAFE_RELEASE( pEffect );
		Assert( !"Special Effect 읽기 실패" );
		return NULL;
	} //if

	m_mapEffect.insert( std::make_pair( std::string( szName ), pEffect ) );

	return pEffect;
} //CW3DMagicMgr::ReadPart


///////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * 마법을 등록한다.
 */ 
MAGIC* CW3DMagicMgr::LoadScript( const char* szName )
{

	//-=-=-=-=-=-=-=-=-=-=-= Open Script -=-=-=-=-=-=-=-=-=-=-=//
	char szScript[256];
	sprintf( szScript, "Magic/%s", szName ); 

	FILE* fp = fopen( szScript, "rt" );

	if( NULL == fp )
	{
		Assert( !"파일이 없어요" );
		return NULL;
	} //if

	//-=-=-=-=-=-=-=-=-=-=-= Cheack Script -=-=-=-=-=-=-=-=-=-=-=//
	char szBuffer[256];
	char szKeyword[80];
	char szDummy[128];

	MAGIC*	pMagic = NULL;
	pMagic = new MAGIC;

	if( !pMagic )
	{
		fclose( fp );
		return NULL; 
	} //if
	
	BOOL bPartFlag = FALSE;		// 파트 정보에 관한 Flag.

	fgets( szBuffer, 256, fp );
	*szKeyword = NULL;
	sscanf( szBuffer, "%s", szKeyword );

	if( CHECK_KEYWORD( "[NAME]" ) )	// 마법의 이름
	{		
		if( sscanf( szBuffer, "%s %s", szKeyword, szDummy ) != 2 )
		{
			Assert( !"sscanf 오류" );
		}

		SET_CHAR( pMagic->szMagicName, szDummy );
	}
	else
	{
		fclose( fp );
		Assert( !"정상적인 순서의 스크립트가 아닙니다." );
		return NULL;
	} //if..else..

	pMagic->btKind = 0;
	pMagic->btType = 0;

	//-=-=-=-=-=-=-=-=-=-=-= Load Effect -=-=-=-=-=-=-=-=-=-=-=//

	BOOL bIsMagic = FALSE;

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );
		
		// 각 단계별 정보
		//-=-=-=-=-=-=-=-= Read Effect -=-=-=-=-=-=-=-=//
		if( CHECK_KEYWORD( "[EFFECT]" ) )		// 스페셜 이펙트
		{
			//-=-=-=-=-=-=-=-= Read Effect -=-=-=-=-=-=-=-=//
			EFFECT* pEffect = NULL;
			pEffect = ReadEffect( fp );

			if( pEffect )
			{
				pMagic->listEffect.push_back( pEffect );
			} //if
		//	m_listEffect.push_back( pEffect );
		}	
		else if( CHECK_KEYWORD( "[ATTACKMAGIC]" ) )
		{
			// 공격형 마법인지에 대한 ... 
			// 공격형 마법일경우 시전자가 죽어도 마법이 끝까지 진행되도록한다.
			pMagic->nScriptType = TARGET_DIE_LASTTARGET;
		}
		else if( CHECK_KEYWORD( "[SOUND]" ) )
		{
			//-=-=-=-=-=-=-=-= Read Sound -=-=-=-=-=-=-=-=//
			if( sscanf( szBuffer, "%s %d %s", szKeyword, &pMagic->dwSoundDelay, szDummy ) != 3 )
			{
				Assert( !"sscanf 오류" );
			}


			pMagic->bSound = TRUE;
			SET_CHAR( pMagic->szSound, szDummy );
		}
		else if( CHECK_KEYWORD( "[TYPE]" ) )
		{
			//-=-=-=-=-=-=-=-= Read Kind & Type -=-=-=-=-=-=-=-=//
			if( sscanf( szBuffer, "%s %d %d", szKeyword, &pMagic->btKind, &pMagic->btType ) != 3 )
			{
				Assert( !"sscanf 오류" );
			}

		}
		else if( CHECK_KEYWORD( "[/NAME]" ) )		// 정보 끝. 마법을 등록한다.
		{
			//-=-=-=-=-=-=-=-= End Of Script -=-=-=-=-=-=-=-=//
			bIsMagic = TRUE;
			break;
		} //if..else..
	} // while

	//-=-=-=-=-=-=-=-=-=-=-= Close Script -=-=-=-=-=-=-=-=-=-=-=//

	fclose( fp );

	if( !bIsMagic )
	{
		Assert( !"잘못된 스크립트입니다." );
		return NULL;
	} //if

	return pMagic;
} //CMagic::LoadScript

/**
 * 스크립트에서 Effect관련 된 부분을 읽어 옵니다.
 *
 * @param	fp		[IN] File Pointer
 */
EFFECT* CW3DMagicMgr::ReadEffect( FILE* fp )
{
	//-=-=-=-=-=-=-=-=-=-= Create Magic Effect -=-=-=-=-=-=-=-=-=-=//
	EFFECT* pEffect = NULL;
	pEffect = new EFFECT;

	if( !pEffect )
		return NULL;

	//-=-=-=-=-=-=-=-=-=-= Init Magic Effect -=-=-=-=-=-=-=-=-=-=//
	BOOL bIsEffect = FALSE;

	pEffect->bLoop			= FALSE;
	pEffect->bTrace			= FALSE;
	pEffect->bSpecialEffect	= FALSE;

	//-=-=-=-=-=-=-=-=-=-= Read Magic Effect -=-=-=-=-=-=-=-=-=-=//
	char szBuffer[256];
	char szKeyword[80];
	char szDummy[128];

	while( !feof( fp ) )
	{
		//-=-=-=-=-=-=-= Read Row -=-=-=-=-=-=-=//
		fgets( szBuffer, 256, fp );
		sscanf( szBuffer, "%s", szKeyword );

		if( CHECK_KEYWORD( "[TRACE]" ) )
		{
			//-=-=-=-=-=-=-= Create Trace Effect -=-=-=-=-=-=-=//
			if( sscanf( szBuffer, "%s %s", szKeyword, szDummy ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			
			SET_CHAR( pEffect->szTrace, szDummy );
			pEffect->bTrace = TRUE;
		}
		else if( CHECK_KEYWORD( "[BLOWFILE]" ) )
		{
			//-=-=-=-=-=-=-= Create Blow Effect -=-=-=-=-=-=-=//
			if( sscanf( szBuffer, "%s %s", szKeyword, szDummy ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			pEffect->pBlowEffect = LoadEffect( szDummy );
		}
		else if( CHECK_KEYWORD( "[LOOP]" ) )
		{
			pEffect->bLoop = TRUE;
		}
		else if( CHECK_KEYWORD( "[SPE]" ) )	// Special Effect
		{
			//-=-=-=-=-=-=-= Create SPE Effect -=-=-=-=-=-=-=//
			if( sscanf( szBuffer, "%s %s", szKeyword, szDummy ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			pEffect->pEffect = LoadEffect( szDummy );
			pEffect->bSpecialEffect = TRUE;
		}
		else if( CHECK_KEYWORD( "[MODEL]" ) )
		{
			if( sscanf( szBuffer, "%s %s", szKeyword, szDummy ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			pEffect->pModel = LoadModel( szDummy );
		}
		else if( CHECK_KEYWORD( "[PART]" ) )	// 파트
		{
			//-=-=-=-=-=-=-= Create Part -=-=-=-=-=-=-=//
			
			PART* pMagicData = NULL;
			pMagicData = ReadPart( fp );

			//-=-=-=-=-=-=-= Add Part -=-=-=-=-=-=-=//
			if( pMagicData )
			{
				pEffect->listMagic.push_back( pMagicData );
			} //if
		}		
		else if( CHECK_KEYWORD( "[/EFFECT]" ) )
		{
			//-=-=-=-=-=-=-= End Of Effect -=-=-=-=-=-=-=//
			// 등록
			bIsEffect = TRUE;
			break;
		} //if..else if..
	} //while

	//-=-=-=-=-=-=-=-=-=-= Return Effect -=-=-=-=-=-=-=-=-=-=//
	if( !bIsEffect )
	{
		Assert( !"Magic Effect 오류" );
		SAFE_DELETE( pEffect );
		return NULL;
	} //if

	return pEffect;
} //CMagic::ReadEffect

/**
 * 스크립트에서 Magic에 관련된 부분( 각 Effect의 Part )을 읽어 옵니다.
 *
 * @param	fp		[IN] File Pointer
 */
PART* CW3DMagicMgr::ReadPart( FILE* fp )
{	
	//-=-=-=-=-=-=-=-=-=-= Create Part -=-=-=-=-=-=-=-=-=-=//
	PART* pMagicData = new PART;

	if( !pMagicData )
	{
		Assert( !"메모리 생성 실패" );
		return NULL;
	} //if
	
	//-=-=-=-=-=-=-=-=-=-= Create Init -=-=-=-=-=-=-=-=-=-=//
	pMagicData->Init();
	
	//-=-=-=-=-=-=-=-=-=-= Read Part -=-=-=-=-=-=-=-=-=-=//
	char szBuffer[256];
	char szKeyword[80];

	// 이 Flag이 켜지지 않는다면, 그것은 바로, 제대로 스크립트를 짠것이아니므로 NULL을 반환한다.
	BOOL bFlag = FALSE;
						
	while( !feof( fp ) )
	{
		//-=-=-=-=-=-=-=-=-=-= Read Row -=-=-=-=-=-=-=-=-=-=//
		fgets( szBuffer, 256, fp );

		if( szBuffer[0] == NULL )
		{
			continue;
		} //if

		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		//-=-=-=-=-=-=-=-= Read Time -=-=-=-=-=-=-=-=//
		if( CHECK_KEYWORD( "[STARTTIME]" ) )	
		{	
			// 시작 시간+_+
			if( sscanf( szBuffer, "%s %d", szKeyword, &pMagicData->dwStartTime ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

		}
		else if( CHECK_KEYWORD( "[TIMELIMIT]" ) )	
		{	
			// 시간 제한( Play될 시간 )+_+
			if( sscanf( szBuffer, "%s %d", szKeyword, &pMagicData->dwLimit ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}
		}
		//-=-=-=-=-=-=-=-=-=-= Read Flag -=-=-=-=-=-=-=-=-=-=//
		else if( CHECK_KEYWORD( "[BLOW]" ) )
		{
			pMagicData->bBlow = TRUE;
		}
		else if( CHECK_KEYWORD( "[BONE]" ) )
		{
			pMagicData->bBone = TRUE;
			int nBone = 0;
			if( sscanf( szBuffer, "%s %d", szKeyword, &nBone ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}


			std::string str( GetBoneName( nBone ) );
			sprintf( pMagicData->szBone, "%s", str.c_str() );
			
		}
		else if( CHECK_KEYWORD( "[ENEMY]" ) )
		{
			pMagicData->bEnemyPos = TRUE;
		}
		else if( CHECK_KEYWORD( "[ATTACK]" ) )
		{
			pMagicData->bAttack = TRUE;
		}
		else if( CHECK_KEYWORD( "[PAUSE]" ) )
		{
			pMagicData->bPause = TRUE;
		}
		else if( CHECK_KEYWORD( "[NOSHOW]" ) )
		{
			pMagicData->bShow = FALSE;
		}
		else if( CHECK_KEYWORD( "[LOOP]" ) )
		{
			pMagicData->bLoop = TRUE;
		}
		//-=-=-=-=-=-=-=-=-=-= Read Position -=-=-=-=-=-=-=-=-=-=//
		else if( CHECK_KEYWORD( "[POS]" ) )
		{
			// 시작 위치
			if( sscanf( szBuffer, "%s %f %f %f", szKeyword, &pMagicData->vPos.x, 
											 &pMagicData->vPos.y, &pMagicData->vPos.z ) != 4 )
			{
				Assert( !"sscanf 오류" );
			}

		}
		else if( CHECK_KEYWORD( "[TARGET]" ) )
		{
			if( sscanf( szBuffer, "%s %f %f %f", szKeyword, &pMagicData->vTarget.x,
											 &pMagicData->vTarget.y, 
										     &pMagicData->vTarget.z ) != 4 )
			{
				Assert( !"sscanf 오류" );
			}

		}
		else if( CHECK_KEYWORD( "[BEZIER]" ) )
		{
			Vector3 vPos,vPos2;
			if( sscanf( szBuffer, "%s %f %f %f %f %f %f", szKeyword, &vPos.x, &vPos.y, &vPos.z,
													  &vPos2.x, &vPos2.y, &vPos2.z ) != 7 )
			{
				Assert( !"sscanf 오류" );
			}


			pMagicData->bBezier = TRUE;
			pMagicData->vBezier2 = vPos;
			pMagicData->vBezier3 = vPos2;
		}
		//-=-=-=-=-=-=-=-=-=-= Read Effect Option -=-=-=-=-=-=-=-=-=-=//
		else if( CHECK_KEYWORD( "[TAIL]" ) )
		{
			char szTail[256];
			if( sscanf( szBuffer, "%s %s", szKeyword, szTail ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}


			pMagicData->pTailEffect = LoadEffect( szTail );

			if( pMagicData->pTailEffect )
			{
				pMagicData->bTail = TRUE;
			}
		}
		else if( CHECK_KEYWORD( "[BILLBOARD]" ) )
		{
			char szBillboard[256];
			if( sscanf( szBuffer, "%s %s", szKeyword, szBillboard ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			pMagicData->pBillboard = MAKE_COMPONENT( Billboard );

			if( !pMagicData->pBillboard->Load( szBillboard ) )
			{
				SAFE_RELEASE( pMagicData->pBillboard );
				pMagicData->pBillboard = NULL;
			} //if
		}
		else if( CHECK_KEYWORD( "[TRACEDIST]" ) )
		{
			if( sscanf( szBuffer, "%s %f", szKeyword, &pMagicData->fTraceDist ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

		}
		else if( CHECK_KEYWORD( "[PARTICLE]" ) )	// 파티클
		{	
			char szDummy[128];
			if( sscanf( szBuffer, "%s %s", szKeyword, szDummy ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			SET_CHAR( pMagicData->szParticle, szDummy );
		
			pMagicData->bParticle = TRUE;
		}
		else if( CHECK_KEYWORD( "[PARTICLEDELAY]" ) )
		{
			if( sscanf( szBuffer, "%s %d", szKeyword, &pMagicData->dwParticleDelay ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

		}
		else if( CHECK_KEYWORD( "[SPEED]" ) )
		{
			if( sscanf( szBuffer, "%s %d", szKeyword, &pMagicData->dwSpeed ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

		}
		else if( CHECK_KEYWORD( "[MOVETYPE]" ) )
		{
			if( sscanf( szBuffer, "%s %d", szKeyword, &pMagicData->btType ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

		}
		else if( CHECK_KEYWORD( "[SOUND]" ) )
		{
			pMagicData->bSound = TRUE;
			
			char szDummy[128];			
			strcpy( szDummy, "" );
			if( sscanf( szBuffer, "%s %d %s %d", szKeyword, &pMagicData->dwSoundDelay, szDummy, &pMagicData->btType ) != 4 )
			{
				Assert( !"sscanf 오류" );
			}

			sprintf( pMagicData->szSound, "%s", szDummy );
//			Assert( _CrtCheckMemory() );
		}
		else if( CHECK_KEYWORD( "[SHAKE]" ) )
		{
			DWORD dwTime = 0;
			if( sscanf( szBuffer, "%s %d", szKeyword, &dwTime ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			// 카메라 흔들기

			pMagicData->dwShake = dwTime;
		}
		else if( CHECK_KEYWORD( "[BLOWTIMING]" ) )
		{
			pMagicData->bBlowTiming = TRUE;

			while( !feof( fp ) )
			{
				//-=-=-=-=-=-=-=-=-=-= Read Row -=-=-=-=-=-=-=-=-=-=//
				fgets( szBuffer, 256, fp );

				if( szBuffer[0] == NULL )
				{
					continue;
				} //if

				*szKeyword = NULL;
				sscanf( szBuffer, "%s", szKeyword );

				//-=-=-=-=-=-=-=-= Read Time -=-=-=-=-=-=-=-=//
				if( CHECK_KEYWORD( "[STARTTIME]" ) )	
				{	
					if( sscanf( szBuffer, "%s %d", szKeyword, &pMagicData->blowTiming.dwTime ) != 2 )
					{
						Assert( !"sscanf 오류" );
					}

				}
				else if( CHECK_KEYWORD( "[TYPE]" ) )
				{
					if( sscanf( szBuffer, "%s %d", szKeyword, &pMagicData->blowTiming.btType ) != 2 )
					{
						Assert( !"sscanf 오류" );
					}

				}
				else if( CHECK_KEYWORD( "[SOUND]" ) )
				{
					if( sscanf( szBuffer, "%s %s", szKeyword, pMagicData->blowTiming.szSound ) != 2 )
					{
						Assert( !"sscanf 오류" );
					}

				}
				else if( CHECK_KEYWORD( "[/BLOWTIMING]" ) )
				{
					break;
				} //if..else..else..else..
			} //while		
		}
		//-=-=-=-=-=-=-=-=-=-= End Of Part -=-=-=-=-=-=-=-=-=-=//
		else if( CHECK_KEYWORD( "[/PART]" ) )
		{
			bFlag = TRUE;
			break;
		} // if..else..else..else..else..else..
	} //while

	//-=-=-=-=-=-=-=-=-=-= Return Magic Part -=-=-=-=-=-=-=-=-=-=//
	if( !bFlag )
	{
		Assert( !"Magic Part 오류" );
		SAFE_DELETE( pMagicData );
		return NULL;
	} //if

	return pMagicData;
} //CMagic::ReadPart

/**
 *
 */
std::string GetBoneName( int nPart )
{
///////////////////////////////////////////
#define SETCHAR( s ) \
		sprintf( szBuffer, "%s", s );	\
		break;
///////////////////////////////////////////

	char szBuffer[64];
	sprintf( szBuffer, "Bip01" );

	switch( nPart )
	{
	case 0:	SETCHAR( "Bip01" );
	case 1:	SETCHAR( "Bip01 Pelvis" );
	case 2:	SETCHAR( "Bip01 Spine" );
	case 3:	SETCHAR( "Bip01 Spine1" );
	case 4: SETCHAR( "Bip01 Neck" );
	case 5:	SETCHAR( "Bip01 Head" );
	case 6:	SETCHAR( "Bip01 L Clavicle" );
	case 7:	SETCHAR( "Bip01 L UpperArm" );
	case 8: SETCHAR( "Bip01 L Forearm" );
	case 9: SETCHAR( "Bip01 L Hand" );
	case 10: SETCHAR( "Bip01 L Finger0" );
	case 11: SETCHAR( "Bip01 R Clavicle" );
	case 12: SETCHAR( "Bip01 R UpperArm" );
	case 13: SETCHAR( "Bip01 R Forearm" );
	case 14: SETCHAR( "Bip01 R Hand" );
	case 15: SETCHAR( "Bip01 R Finger0" );
	case 16: SETCHAR( "Bip01 L Thingh" );
	case 17: SETCHAR( "Bip01 L Calf" );
	case 18: SETCHAR( "Bip01 L Foot" );
	case 19: SETCHAR( "Bip01 L Toe0" );
	case 20: SETCHAR( "Bip01 R Thingh" );
	case 21: SETCHAR( "Bip01 R Calf" );
	case 22: SETCHAR( "Bip01 R Foot" );
	case 23: SETCHAR( "Bip01 R Toe0" );
	case 30: SETCHAR( "Bone L Hand0" );
	case 31: SETCHAR( "Bone R Hand0" );
	default:
		SETCHAR( "Bip01" );
	}; //switch

#undef SETCHAR
	 
	std::string str( szBuffer );
	return str;
} //GetBoneName

/**
 *
 */
IW3DModel*	CW3DMagicMgr::LoadModel( const char* szName )
{
	Assert( szName );
	
	ModelMap::iterator itor = m_mapModel.find( std::string( szName ) );

	if( itor != m_mapModel.end() )
	{
		return itor->second;
	} //if

	IW3DModel* p = MAKE_COMPONENT( Model );

	if( !p ||
		!p->Load( "Effect/Model", szName ) )
	{
		SAFE_RELEASE( p );
		return NULL;
	} //if

	char	szAnimation[256];
	strcpy( szAnimation, szName );
	strcpy( szAnimation + strlen( szName ) - 3, "ani" );

	p->CalculateRadius();
	p->LoadAnimation( "Effect/Model", szAnimation );

	//p->Load(
	m_mapModel.insert( std::make_pair( szName, p ) );

	return p;
} //CW3DMagicMgr::LoadModel