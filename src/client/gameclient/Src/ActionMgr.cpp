/**
 * Character Action 관리자
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-06-04 4:42p $
 * @version	$Revision: 20 $
 *
 * <HR>
 * $Log: /GameClient/Src/ActionMgr.cpp $
 * 
 * 20    04-06-04 4:42p Sckyj
 * 
 * 19    04-06-04 4:29p Sckyj
 * 
 * 18    04-03-12 6:41p Sckyj
 * 운영자 작업중
 * 
 * 17    04-02-02 3:48p Kjmgo
 * 
 * 16    03-12-23 9:10p Kjmgo
 * 쓰지 않는 무기에 대한 애니메이션은 생성하지 아니하도록 수정
 * 
 * 15    03-12-12 10:25p Paran
 * 
 * 14    03-12-11 4:00p Kjmgo
 * 
 * 13    03-10-17 11:23a Sckyj
 * 
 * 12    03-10-16 4:41p Sckyj
 * sound재생관련 코드 수정
 * 
 * 11    03-10-10 11:28a Kjmgo
 * 소스정리
 * 
 * 10    03-10-10 10:29a Kjmgo
 * wad파일을 Load하였을시, 정상일떄도 FALSE를 리턴하던것을 TRUE로
 * 리턴하도록 수정
 * 
 * 9     03-10-07 6:49p Kjmgo
 * Vehicle에서 Weapon 사용 가능하도록 수정
 * 
 * 8     03-10-07 9:28a Kjmgo
 * Weapon Animation 처리
 * 
 * 7     03-09-12 6:42p Kjmgo
 * Vehicle 관련 수정 및 추가
 * 
 * 6     03-09-08 3:17p Paran
 * 
 * 5     03-09-07 12:57p Kjmgo
 * 
 * 4     03-09-02 8:30p Kjmgo
 * 몬스터 동작 수정
 * 
 * 3     03-09-02 5:49a Kjmgo
 * Monster 추가작업
 * 
 * 2     03-08-31 6:18p Kjmgo
 * Character Action 관련 추가
 * 
 * 1     03-08-30 11:29a Kjmgo
 * <HR>
 *
 * @file	ActionMgr.cpp
 */

#include "ActionMgr.h"

/////////////////////////////////////////////////////////////////////////
/*
	Character Motion Manager
*/
/////////////////////////////////////////////////////////////////////////

/**
 *
 */
void GetAniFileName( const char* szFile, char* szName );

/**
 *
 */
BOOL IsSkipWeapon( int nWeapon )
{
	switch( nWeapon )
	{
	//	case WEAPON_ONEHANDAXE:
	//	case WEAPON_ONEHANDAXEWITHSHIELD:
		case WEAPON_DUALSWORD:
		case WEAPON_TWOHANDAXE:
		case WEAPON_DUALAXE:
	//	case WEAPON_ONEHANDBLUNT:
	//	case WEAPON_ONEHANDBLUNTWITHSHIELD:
		case WEAPON_DONPA:
		case WEAPON_TWOHANDBLUNT:
		case WEAPON_DUALBLUNT:
		case WEAPON_LONGBOW:
			return TRUE;
		default:
			return FALSE;
	}; //switch

	return FALSE;
} //IsSkipWeapon

/**
 *
 */
CCharacterState::CCharacterState()
:	m_nSex			( _MALE ),
	m_pAnimationMgr	( NULL ),
	m_pAction		( NULL )
{
	for( int i = 0 ; i < MAX_WEAPON ; i++ )
	{
		m_aMaleWeapon[i] = NULL;
		m_aFemaleWeapon[i] = NULL;
	} //for

	m_aMasterWeapon = NULL;
} //CCharacterState::CCharacterState

/**
 *
 */
CCharacterState::~CCharacterState()
{
	SAFE_DELETE( m_pAction );
	
	for( int i = 0 ; i < MAX_WEAPON ; i++ )
	{
		SAFE_DELETE( m_aMaleWeapon[i] );
		SAFE_DELETE( m_aFemaleWeapon[i] );
	} //for

	SAFE_DELETE( m_aMasterWeapon );

} //CCharacterState::~CCharacterState

/**
 *
 */
BOOL CCharacterState::Load( const char* szFile )
{
	FILE* fp = fopen( szFile, "rb" );

	if( NULL == fp )
	{
		Assert( !"Character Motion Data :  파일이 없습니다" );
		return FALSE;
	} //if

	// Animation Mgr 
	m_pAnimationMgr = MAKE_COMPONENT( AnimationMgr );
	m_pAction		= new SAction;
	
	// Version
	DWORD dwVersion = 0;
	fread( &dwVersion, 4, 1, fp );

	if( dwVersion != 100 )
	{
		Assert( !"Character Motion Data : 버전이 맞지 않습니다." );
		return FALSE;
	} //if

	// Read File List
	LoadFileName( fp );

	////////////////////////////////////////////
	// Male 
	////////////////////////////////////////////

	// Male
	{
		DWORD dwCount = 0;		// 무기 갯수
		fread( &dwCount, 4, 1, fp );

		for( int i = 0 ; i < dwCount ; i++ )
		{
			LoadWeapon( fp, i, _MALE );
		} //for
	} // Male

	////////////////////////////////////////////
	// Female 
	////////////////////////////////////////////

	// Female
	{
		DWORD dwCount = 0;		// 무기 갯수
		fread( &dwCount, 4, 1, fp );

		for( int i = 0 ; i < dwCount ; i++ )
		{
			LoadWeapon( fp, i, _FEMALE );
		} //for
	} // Female

	////////////////////////////////////////////
	// Master
	////////////////////////////////////////////

	// Master
	{
		DWORD dwCount = 0;		// 무기 갯수
		fread( &dwCount, 4, 1, fp );

		LoadWeapon( fp, 0, _MASTER );
	} // Master

	fclose( fp );
	return TRUE;
} //CCharacterState::Load

/**
 *
 */
BOOL CCharacterState::LoadFileName( FILE* fp )
{
	Assert( fp );

	int nCount = 0;
	fread( &nCount, 4, 1, fp );

	for( int i = 0 ; i < nCount ; i++ )
	{
		DWORD dwId = 0;
		int   nLen = 0;
		char  szName[80];

		fread( &dwId, 4, 1, fp );
		fread( &nLen, 4, 1, fp );
		fread( szName, nLen, 1, fp );

		m_mapString.insert( std::make_pair( dwId, std::string( szName ) ) );
	} //for

	return TRUE;
} //CCharacterState::LoadFileName

/**
 *
 */
BOOL CCharacterState::LoadWeapon( FILE* fp, int nWeapon, UINT nSex )
{
	Assert( fp );

	Character::SWeapon* pWeapon = NULL;

	if( !IsSkipWeapon( nWeapon ) )
	{
		if( _MALE == nSex )
		{ // 남자
			m_aMaleWeapon[nWeapon] = new Character::SWeapon;
			pWeapon = m_aMaleWeapon[nWeapon];
		}
		else
		if( _FEMALE == nSex )
		{ // 여자
			m_aFemaleWeapon[nWeapon] = new Character::SWeapon;
			pWeapon = m_aFemaleWeapon[nWeapon];
		}
		else
		if( _MASTER == nSex )
		{ // 운영자
			m_aMasterWeapon = new Character::SWeapon;
			pWeapon = m_aMasterWeapon;
		} //if..else
	}
	else
	{
		pWeapon = new Character::SWeapon;
	}//if


	pWeapon->Clear();
	LoadMode( fp, pWeapon );

	if( IsSkipWeapon( nWeapon ) )
	{
		SAFE_DELETE( pWeapon );
	} //if

	return TRUE;
} //CCharacterState::LoadWeapon

/**
 *
 */
BOOL CCharacterState::LoadMode( FILE* fp, Character::SWeapon* pWeapon )
{
	Assert( fp );
	Assert( pWeapon );

	Character::SMode* pMode = NULL;

	// War Mode
	{
		pMode = &pWeapon->modeWar;

		for( int i = 0 ; i < 50 ; i++ )
		{
			Character::SAct* pAction = &pMode->aAct[i];
		
			for( int j = 0 ; j < 3 ; j++ )
			{
				LoadAct( fp, &pAction->aMotion[j] );
			} //for
		} //for
		
	} // War Mode

	// Peace Mode
	{
		pMode = &pWeapon->modePeace;

		for( int i = 0 ; i < 50 ; i++ )
		{
			Character::SAct* pAction = &pMode->aAct[i];
		
			for( int j = 0 ; j < 3 ; j++ )
			{
				LoadAct( fp, &pAction->aMotion[j] );
			} //for
		} //for
	} // Peace Mode

	// Vehicle Mode
	{
		for( int i = 0 ; i < 10 ; i++ )
		{
			pMode = &pWeapon->amodeVehicle[i];

			for( int i = 0 ; i < 50 ; i++ )
			{
				Character::SAct* pAction = &pMode->aAct[i];

				for( int j = 0 ; j < 3 ; j++ )
				{
					LoadAct( fp, &pAction->aMotion[j] );
				} //for
			} //for
		} //for
	} //Vehicle Mode


	return TRUE;
} //CCharacterState::LoadMode

/**
 *
 */
BOOL CCharacterState::LoadAct( FILE* fp, Character::SMotion* pMotion )
{
	Assert( fp );
	Assert( pMotion );

	struct SMot
	{
		// Tool에서 쓰던것과 구조가 틀리므로, 
		// 읽어 들일때는 이걸 사용한다.
		DWORD	dwBegin;			///< 시작 프레임
		DWORD	dwEnd;				///< 끝 프레임

		WORD	wAniFile;			///< 애니 파일
		WORD	wWeaponFile;		///< 무기 파일
		WORD	wSoundFile;			///< 사운드 파일
	}; // struct SMot

	// Motion 파일을 읽어 들인다.
	SMot mot;
	fread( &mot, sizeof( SMot ), 1, fp );

	pMotion->Clear();
	pMotion->dwBegin	= mot.dwBegin;
	pMotion->dwEnd		= mot.dwEnd;
	pMotion->wSoundFile	= mot.wSoundFile;

	if( mot.wAniFile == 0 || ( mot.dwEnd - mot.dwBegin ) <= 0 )
	{
		// 0 일땐 사용하지 않는 놈이란 뜻이다냥..
		pMotion->bUse = FALSE;
		return TRUE;
	} //if

	///////////////////////////////////////////
	//	Character Animation
	///////////////////////////////////////////

	const char* szFileName = GetFileName( mot.wAniFile );

	char szFile[256];
	GetAniFileName( szFileName, szFile );
	
	SAction* pAction = m_pAnimationMgr->LoadAnimation( "Body/Animation", szFile );

	if( !pAction )
	{
		Assert( !"Animation 파일을 읽어 올수 없습니다." );
		return FALSE;
	} //if

	pMotion->pAnimation = pAction->pAnimation;

	pMotion->bUse = TRUE;

	///////////////////////////////////////////
	//	Weapon Animation
	///////////////////////////////////////////

	if( mot.wWeaponFile )
	{
		const char* szFileName = GetFileName( mot.wWeaponFile );

		GetAniFileName( szFileName, szFile );
		pAction = m_pAnimationMgr->LoadAnimation( "Body/Weapon", szFile );

		if( !pAction )
		{
			Assert( !"Animation 파일을 읽어 올수 없습니다." );
			return FALSE;
		} //if

		pMotion->pWeapon = pAction->pAnimation;
	}
	else
	{
		pMotion->pWeapon = NULL;
	} //if..else..

	return TRUE;
} //CCharacterState::LoadAct
 
/**
 *
 */
const char* CCharacterState::GetFileName( DWORD dwId )
{
	StringMap::iterator itor = m_mapString.find( dwId );

	if( itor != m_mapString.end() )
	{
		string str = itor->second;
		return str.c_str();
	} //if

	Assert( !"원하는 파일 이름이 없습니다." );
	return "";
} //CCharacterState::GetFileName

/**
 *
 */
Character::SWeapon* CCharacterState::GetWeapon( UINT nSex, WORD wWeapon )
{
	Character::SWeapon* pWeapon = NULL;

	if( _FEMALE == nSex )
	{ //여자
		pWeapon = m_aFemaleWeapon[wWeapon];
	}
	else
	if( _MALE == nSex )	
	{
		pWeapon = m_aMaleWeapon[wWeapon];
	}
	else
	if( _MASTER == nSex )
	{
		pWeapon = m_aMasterWeapon;
	}

	return pWeapon;
} //CCharacterState::GetWeapon

/**
 *
 */
SAction* CCharacterState::GetAction( UINT nSex, WORD wWeapon, WORD wMode, WORD wAction, WORD wLevel )
{
	Character::SWeapon* pWeapon = GetWeapon( nSex, wWeapon );
	Character::SMode*	pMode = NULL;
	Character::SAct*	pAct = NULL;
	SAction*			pAction = new SAction;

	switch( wMode )
	{
	case	MODE_PEACE:
		pMode = &pWeapon->modePeace;
		break;
	case	MODE_WAR:
		pMode = &pWeapon->modeWar;
		break;
	}; //switch

	pAct = &pMode->aAct[wAction];

	pAction->nStart		= pAct->aMotion[wLevel].dwBegin;
	pAction->nEnd		= pAct->aMotion[wLevel].dwEnd;
	pAction->nFrames	= pAct->aMotion[wLevel].dwEnd - pAct->aMotion[wLevel].dwBegin + 1;
	pAction->pAnimation	= pAct->aMotion[wLevel].pAnimation;

	return pAction;
} //CCharacterState::GetAction


/////////////////////////////////////////////////////////////////////////
/*
	Monster Motion Manager
*/
/////////////////////////////////////////////////////////////////////////

/**
 *
 */
CMonsterState::CMonsterState()
{

	
}//CMonsterState::CMonsterState

/**
 *
 */
CMonsterState::~CMonsterState()
{
	MonsterList::iterator itor = m_listMonster.begin();

	while( itor != m_listMonster.end() )
	{
		Monster::SMonster* pMonster = *itor;

		SAFE_DELETE( pMonster );
		itor++;
	} //while

} //CMonsterState::~CMonsterState

/**
 *
 */
BOOL CMonsterState::Load( const char* szFile )
{
	FILE* fp = fopen( szFile, "rb" );

	if( NULL == fp )
	{
		return FALSE;
	} //if

	// Animation Mgr 
	m_pAnimationMgr = MAKE_COMPONENT( AnimationMgr );
	
	// Version
	DWORD dwVersion = 0;
	fread( &dwVersion, 4, 1, fp );

	if( dwVersion != 100 )
	{
		Assert( !"Monster Motion Data : 버전이 맞지 않습니다." );
		return FALSE;
	} //if

	LoadFileName( fp );
	LoadMonster( fp );

	fclose( fp );
	return TRUE;
} //CMonsterState::Load

/**
 *
 */
BOOL CMonsterState::LoadFileName( FILE* fp )
{
	Assert( fp );

	int nCount = 0;
	fread( &nCount, 4, 1, fp );

	for( int i = 0 ; i < nCount ; i++ )
	{
		DWORD dwId = 0;
		int   nLen = 0;
		char  szName[80];
		
		fread( &dwId, 4, 1, fp );
		fread( &nLen, 4, 1, fp );
		fread( szName, nLen, 1, fp );
	
		m_mapString.insert( std::make_pair( dwId, std::string( szName ) ) );
	} //for

	return TRUE;
} //CMonsterState::LoadFileName

/**
 *
 */
void CMonsterState::LoadMonster( FILE* fp )
{
	Assert( fp );

	int nCount = 0;
	fread( &nCount, 4, 1, fp );

	for( int i = 0 ; i < nCount ; i++ )
	{
		MonsterDummy::SMonster monster;

		int nSize = sizeof( MonsterDummy::SMonster );

		fread( &monster, nSize, 1, fp );

		MakeMonster( monster );
	} //for

} //CMonsterState::LoadFileName

/**
 *
 */
void CMonsterState::MakeMonster( MonsterDummy::SMonster monster )
{
	Monster::SMonster* pMonster = new Monster::SMonster;
	pMonster->Clear();
	pMonster->nId = monster.nId;

	// Peace Mode
	{
		Monster::SMode* pMode = &pMonster->modePeace;
		pMode->nCountPart1 = monster.modePeace.nCountPart1;
		pMode->nCountPart2 = monster.modePeace.nCountPart2;

		for( int i = 0 ; i < 9 ; i++ )
		{
			Monster::SMotion* pMotion = &pMode->aAction[i];
			pMotion->Clear();

			pMotion->bUse			= monster.modePeace.aAction[i].bUse;
			pMotion->dwBegin		= monster.modePeace.aAction[i].dwBegin;
			pMotion->dwEnd			= monster.modePeace.aAction[i].dwEnd;

			if( pMotion->bUse )
			{
				const char*	szFileName = GetFileName( monster.modePeace.aAction[i].wAniFile );
				SAction* pAction = m_pAnimationMgr->LoadAnimation( "Monster/Animation", szFileName );

				Assert( pAction );

				if( !pAction )
				{
					Assert( !"파일이 없다냥" );
				} //if
				
				pMotion->pAnimation		= pAction->pAnimation;
			} //if
			pMotion->wSoundFile		= monster.modePeace.aAction[i].wSoundFile;
			
		} //for
	} //Peace Mode

	// War Mode
	{
		Monster::SMode* pMode = &pMonster->modeWar;
		pMode->nCountPart1 = monster.modeWar.nCountPart1;
		pMode->nCountPart2 = monster.modeWar.nCountPart2;

		for( int i = 0 ; i < 9 ; i++ )
		{
			Monster::SMotion* pMotion = &pMode->aAction[i];
			pMotion->Clear();
			pMotion->bUse			= monster.modeWar.aAction[i].bUse;
			pMotion->dwBegin		= monster.modeWar.aAction[i].dwBegin;
			pMotion->dwEnd			= monster.modeWar.aAction[i].dwEnd;

			pMotion->pAnimation		= NULL;
			if( pMotion->bUse )
			{
				const char* szName = GetFileName( monster.modeWar.aAction[i].wAniFile );
				SAction* pAction = m_pAnimationMgr->LoadAnimation( "Monster/Animation", 
																	szName );

				Assert( pAction );
				pMotion->pAnimation = NULL;
				if( pAction )
				{
					pMotion->pAnimation		= pAction->pAnimation;
				} //if

			} //if

			pMotion->wSoundFile		= monster.modeWar.aAction[i].wSoundFile;
		} //for
	} //War Mode

	m_listMonster.push_back( pMonster );

	m_mapMonster.insert( std::make_pair( pMonster->nId, pMonster ) );
} //CMonsterState::MakeMonster

/**
 *
 */
const char* CMonsterState::GetFileName( DWORD dwId )
{
	StringMap::iterator itor = m_mapString.find( dwId );

	if( itor != m_mapString.end() )
	{
		string str = itor->second;
		return str.c_str();
	} //if

	Assert( !"원하는 파일 이름이 없습니다." );
	return "";
} //CMonsterState::GetFileName

/**
 *
 */
Monster::SMonster*	CMonsterState::GetMonster( DWORD dwId )
{
	if( dwId <= 299 && dwId >= 201 )
	{
		dwId = dwId - 200;
	}
	else if( dwId <= 399 && dwId >= 301 )
	{
		dwId = dwId - 300;
	}
	else if( dwId <= 499 && dwId >= 401 )
	{
		dwId = dwId - 400;
	}
	else if( dwId <= 599 && dwId >= 501 )
	{
		dwId = dwId - 500;
	}
	else if( dwId <= 699 && dwId >= 601 )
	{
		dwId = dwId - 600;
	}
	MonsterMap::iterator itor = m_mapMonster.find( dwId );

	if( itor != m_mapMonster.end() )
	{ // 있다냥..
		return itor->second;
	} //if	
	return NULL;
} //MonsterDummy::SMonster*	CMonsterState::GetMonster

/////////////////////////////////////////////////////////////////////////
/*
	Vehicle Motion Manager
*/
/////////////////////////////////////////////////////////////////////////


/**
 *
 */
CVehicleState::CVehicleState()
:	m_nSex			( _MALE ),
	m_pAnimationMgr	( NULL ),
	m_pAction		( NULL )
{

} //CVehicleState::CVehicleState

/**
 *
 */
CVehicleState::~CVehicleState()
{
	SAFE_DELETE( m_pAction );
} //CVehicleState::~CVehicleState

/**
 *
 */
BOOL CVehicleState::Load( const char* szFile )
{
	FILE* fp = fopen( szFile, "rb" );

	if( NULL == fp )
	{
		Assert( !"Vehicle Motion Data :  파일이 없습니다" );
		return FALSE;
	} //if

	// Animation Mgr 
	m_pAnimationMgr = MAKE_COMPONENT( AnimationMgr );
	m_pAction		= new SAction;
	
	// Version
	DWORD dwVersion = 0;
	fread( &dwVersion, 4, 1, fp );

	if( dwVersion != 100 )
	{
		Assert( !"Vehicle Motion Data : 버전이 맞지 않습니다." );
		return FALSE;
	} //if

	// Read File List
	LoadFileName( fp );

	////////////////////////////////////////////
	// Male 
	////////////////////////////////////////////

	// Male
	{
		DWORD dwCount = 0;		// 무기 갯수
		fread( &dwCount, 4, 1, fp );

		for( int i = 0 ; i < dwCount ; i++ )
		{
			LoadWeapon( fp, i, _MALE );
		} //for
	} // Male

	////////////////////////////////////////////
	// Female 
	////////////////////////////////////////////

	// Female
	{
		DWORD dwCount = 0;		// 무기 갯수
		fread( &dwCount, 4, 1, fp );

		for( int i = 0 ; i < dwCount ; i++ )
		{
			LoadWeapon( fp, i, _FEMALE );
		} //for
	} // Female

	////////////////////////////////////////////
	// Master
	////////////////////////////////////////////

	// Master
	{
		DWORD dwCount = 0;		// 무기 갯수
		fread( &dwCount, 4, 1, fp );

		LoadWeapon( fp, 0, _MASTER );
		
	} // Female

	fclose( fp );
	return TRUE;
} //CVehicleState::Load

/**
 *
 */
BOOL CVehicleState::LoadFileName( FILE* fp )
{
	Assert( fp );

	int nCount = 0;
	fread( &nCount, 4, 1, fp );

	for( int i = 0 ; i < nCount ; i++ )
	{
		DWORD dwId = 0;
		int   nLen = 0;
		char  szName[80];

		fread( &dwId, 4, 1, fp );
		fread( &nLen, 4, 1, fp );
		fread( szName, nLen, 1, fp );

		m_mapString.insert( std::make_pair( dwId, std::string( szName ) ) );
	} //for

	return TRUE;
} //CVehicleState::LoadFileName

/**
 *
 */
BOOL CVehicleState::LoadWeapon( FILE* fp, int nWeapon, UINT nSex )
{
	Assert( fp );

	Vehicle::SWeapon* pWeapon = NULL;

	if( _MALE == nSex )
	{ // 남자
		pWeapon = &m_aMaleWeapon[nWeapon];
	}
	else
	if( _FEMALE == nSex )
	{ // 여자
		pWeapon = &m_aFemaleWeapon[nWeapon];
	}
	else
	if( _MASTER == nSex )
	{
		pWeapon = &m_aMasterWeapon;
	}

	pWeapon->Clear();
	LoadMode( fp, pWeapon );

	return TRUE;
} //CVehicleState::LoadWeapon

/**
 *
 */
BOOL CVehicleState::LoadMode( FILE* fp, Vehicle::SWeapon* pWeapon )
{
	Assert( fp );
	Assert( pWeapon );

	Vehicle::SMode* pMode = NULL;

	// War Mode
	{
		for( int k = 0 ; k < MAX_VEHICLE ; k++ )
		{
			pMode = &pWeapon->aMode[k];

			for( int i = 0 ; i < 50 ; i++ )
			{
				Vehicle::SAct* pAction = &pMode->aAct[i];
			
				for( int j = 0 ; j < 3 ; j++ )
				{
					LoadAct( fp, &pAction->aMotion[j] );
				} //for
			} //for
		} //for		
	} // War Mode

	return TRUE;
} //CVehicleState::LoadMode

/**
 *
 */
BOOL CVehicleState::LoadAct( FILE* fp, Vehicle::SMotion* pMotion )
{
	Assert( fp );
	Assert( pMotion );

	struct SMot
	{
		// Tool에서 쓰던것과 구조가 틀리므로, 
		// 읽어 들일때는 이걸 사용한다.
		DWORD	dwBegin;			///< 시작 프레임
		DWORD	dwEnd;				///< 끝 프레임

		WORD	wAniFile;			///< 애니 파일
		WORD	wWeaponFile;		///< 무기 파일
		WORD	wSoundFile;			///< 사운드 파일
	}; // struct SMot

	// Motion 파일을 읽어 들인다.
	SMot mot;
	fread( &mot, sizeof( SMot ), 1, fp );

	pMotion->Clear();
	pMotion->dwBegin	= mot.dwBegin;
	pMotion->dwEnd		= mot.dwEnd;
	pMotion->wSoundFile	= mot.wSoundFile;

	if( mot.wAniFile == 0 || ( mot.dwEnd - mot.dwBegin ) <= 0 )
	{
		// 0 일땐 사용하지 않는 놈이란 뜻이다냥..
		pMotion->bUse = FALSE;
		return TRUE;
	} //if

	///////////////////////////////////////////
	//	Character Animation
	///////////////////////////////////////////

	const char* szFileName = GetFileName( mot.wAniFile );
	
	SAction* pAction = m_pAnimationMgr->LoadAnimation( "Vehicle/Animation", szFileName );

	if( !pAction )
	{
		Assert( !"Animation 파일을 읽어 올수 없습니다." );
		return FALSE;
	} //if

	pMotion->pAnimation = pAction->pAnimation;

	pMotion->bUse = TRUE;

	///////////////////////////////////////////
	//	Weapon Animation
	///////////////////////////////////////////

	if( mot.wWeaponFile )
	{
		pAction = m_pAnimationMgr->LoadAnimation( "Vehicle/Animation", GetFileName( mot.wWeaponFile ) );

		if( !pAction )
		{
			Assert( !"Animation 파일을 읽어 올수 없습니다." );
			pMotion->pWeapon = NULL;
			return FALSE;
		} //if

		pMotion->pWeapon = pAction->pAnimation;
	} 
	else
	{
		pMotion->pWeapon = NULL;
	} //if..else..

	return TRUE;
} //CVehicleState::LoadAct

/**
 *
 */
const char* CVehicleState::GetFileName( DWORD dwId )
{
	StringMap::iterator itor = m_mapString.find( dwId );

	if( itor != m_mapString.end() )
	{
		string str = itor->second;
		return str.c_str();
	} //if

	Assert( !"원하는 파일 이름이 없습니다." );
	return "";
} //CVehicleState::GetFileName

/**
 *
 */
Vehicle::SWeapon* CVehicleState::GetWeapon( UINT nSex, WORD wWeapon )
{
	Vehicle::SWeapon* pWeapon = NULL;

	if( _FEMALE == nSex )
	{ //여자
		pWeapon = &m_aFemaleWeapon[wWeapon];
	}
	else
	if( _MALE == nSex )
	{
		pWeapon = &m_aMaleWeapon[wWeapon];
	}
	else
	if( _MASTER == nSex )	
	{
		pWeapon = &m_aMasterWeapon;
	}

	return pWeapon;
} //CVehicleState::GetWeapon

/**
 *
 */
SAction* CVehicleState::GetAction( UINT nSex, WORD wWeapon, WORD wMode, WORD wAction, WORD wLevel )
{
	Vehicle::SWeapon* pWeapon = GetWeapon( nSex, wWeapon );
	Vehicle::SMode*	pMode = NULL;
	Vehicle::SAct*	pAct = NULL;

	SAction*		pAction = new SAction;

	pMode = &pWeapon->aMode[wMode];
	pAct = &pMode->aAct[wAction];

	pAction->nStart			= pAct->aMotion[wLevel].dwBegin;
	pAction->nEnd			= pAct->aMotion[wLevel].dwEnd;
	pAction->nFrames		= pAct->aMotion[wLevel].dwEnd - pAct->aMotion[wLevel].dwBegin + 1;
	pAction->pAnimation		= pAct->aMotion[wLevel].pAnimation;

	return pAction;
} //CVehicleState::GetAction

/**
 *
 */
void GetAniFileName( const char* szFile, char* szName )
{
	int nLen = strlen( szFile );

	if( !strcmp( &szFile[nLen-3], "ani" ) )
	{
		strcpy( szName, szFile );
	} 
	else
	{
		sprintf( szName, "%s.ani", szFile );
	} //if
} //GetAniFileName