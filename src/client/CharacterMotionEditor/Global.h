#ifndef __GLOBAL_HEADER__
#define __GLOBAL_HEADER__

#pragma warning( disable : 4786 )


////////////////////////////////////////////////////////////////

#define SEX_MALE						0
#define SEX_FEMALE						1

////////////////////////////////////////////////////////////////

/*0 맨손
1 맨손	+ 방패
2 한손검
3 한손검 + 방패
4 양손검
5 쌍칼
6 한손도끼
7 한손도끼 + 방패
8 양손도끼
9 쌍도끼
10 한손둔기
11 한손둔기 + 방패
12 양손둔기
13 쌍둔기
14 돈파
15 단궁 
16 장궁
17 연노
18 석궁
19 창
20 봉
21 지팡이
22 도
23 참마도*/
#define WEAPON_NONE						0		//"0.맨손",
#define WEAPON_NONEWITHSHIELD			1		//"0.맨손",
#define WEAPON_ONEHANDSWORD				2		//"1.한손검",
#define WEAPON_ONEHANDSWORDWITHSHIELD	3		//"1.한손검",
#define WEAPON_TWOHANDSWORD				4		//"2.양손검", WITHSHIELD
#define WEAPON_DUALSWORD				5		//"3.쌍칼",
#define WEAPON_ONEHANDAXE				6		//"4.한손도끼",
#define WEAPON_ONEHANDAXEWITHSHIELD		7		//"4.한손도끼",
#define WEAPON_TWOHANDAXE				8		//"5.양손도끼",
#define WEAPON_DUALAXE					9		//"6.쌍도끼",
#define WEAPON_ONEHANDBLUNT				10		//"7.한손둔기",
#define WEAPON_ONEHANDBLUNTWITHSHIELD	11		//"7.한손둔기",
#define WEAPON_TWOHANDBLUNT				12		//"8.양손둔기",
#define WEAPON_DUALBLUNT				13		//"9.쌍둔기",
#define WEAPON_DONPA					14		//"10.돈파",
#define WEAPON_SHORTBOW					15		//"11.단궁",
#define WEAPON_LONGBOW					16		//"12.장궁",
#define WEAPON_DDABALBOW				17		//"13.연노",
#define WEAPON_CROSSBOW					18		//"14.석궁",
#define WEAPON_ARROW					19		//"15.화살",
#define WEAPON_SPHERE					20		//"16.창",
#define WEAPON_BONG						21		//"17.봉",
#define WEAPON_STAFF					22		//"18.지팡이",
#define WEAPON_DO						23		// 
#define WEAPON_SLAYER					24		//

////////////////////////////////////////////////////////////////

/*0 정지
1 걷기
2 뛰기
3 뛰기전동작
4 뛰고난후동작
5 사망
6 걷기전쟁모드변경
7 뛰기전쟁모드변경
8 공격1
9 공격2
10 공격3
11 공격4
12 공격5
13 뛰면서공격1
14 뛰면서공격2
15 뛰면서공격3
16 백마법시전
17 흑마법시전
18 이모티콘(피로)
*/

#define STATE_STOP						0
#define STATE_WALK						1
#define STATE_RUN						2
#define STATE_DIE						3
#define STATE_WALK_MODECHANGE			4
#define STATE_RUN_MODECHANGE			5
#define STATE_ATTACK1					6
#define STATE_ATTACK2					7
#define STATE_ATTACK3					8
#define STATE_ATTACK4					9
#define STATE_ATTACK5					10
#define STATE_RUN_ATTACK1				11
#define STATE_RUN_ATTACK2				12
#define STATE_RUN_ATTACK3				13

// 공격형 마법
#define STATE_MAGIC_ATTACK_UPANDDOWN	14
#define STATE_MAGIC_ATTACK_DOWNANDUP	15
#define STATE_MAGIC_ATTACK_GO			16
#define STATE_MAGIC_ATTACK_AROUND		17

// 축복
#define STATE_MAGIC_BLESS_UPANDDOWN		18
#define STATE_MAGIC_BLESS_DOWNANDUP		19
#define STATE_MAGIC_BLESS_GO			20
#define STATE_MAGIC_BLESS_AROUND		21

// 소환
#define STATE_MAGIC_RECALL_UPANDDOWN	22
#define STATE_MAGIC_RECALL_DOWNANDUP	23
#define STATE_MAGIC_RECALL_GO			24
#define STATE_MAGIC_RECALL_AROUND		25

// 저
#define STATE_MAGIC_CURSE_UPANDDOWN		26
#define STATE_MAGIC_CURSE_DOWNANDUP		27
#define STATE_MAGIC_CURSE_GO			28
#define STATE_MAGIC_CURSE_AROUND		29

//#define STATE_MODE_CH
#define STATE_EMOTICON					30


////////////////////////////////////////////////////////////////

#define ROW_STOP						0
#define ROW_WALK						1
#define ROW_RUN							2
#define ROW_DIE							3
#define ROW_WALK_MODECHANGE				4
#define ROW_RUN_MODECHANGE				5
#define ROW_ATTACK						6
#define ROW_RUNATTACK					7
#define ROW_MAGIC_1						8
#define ROW_MAGIC_2						9
#define ROW_MAGIC_3						10
#define ROW_MAGIC_4						11
#define ROW_EMOTICON					12


#define MAX_WEAPON		25
#define MAX_STATE 12

enum
{
	_MALE = 0,
	_FEMALE,
	_MASTER
};


////////////////////////////////////////////////////////////////

#define	SET_CHAR( s, d )	sprintf( s, "%s", d ); 
#define SAFE_DELETE_ARRAY( p )	{ if( p ) { delete [] (p);	(p) = NULL; } }
////////////////////////////////////////////////////////////////

extern	int		g_nAttackNumber;
extern	int		g_nRunAttackNumber;
extern  int		g_nMagicNumber;
extern  int		g_nVehicle;
extern	int		g_nEmoticonNumber;


////////////////////////////////////////////////////////////////

#include <map>
#include <string>

class CFileMgr
{
public:
	
	/**
	 *
	 */
	~CFileMgr()
	{
		m_mapString.clear();
	}

	/**
	 *
	 */
	CFileMgr()
	{
		Clear();
	}

	/**
	 *
	 */
	void Clear()
	{
		m_mapString.clear();
		m_mapWord.clear();
		m_dwId = 1;
	}

	/**
	 *
	 */
	WORD Insert( char* szFile )
	{
		if( !strcmp( szFile, "" ) )
		{
			return 0;
		} //if

		StringMap::iterator itor = m_mapString.find( std::string( szFile ) );
		
		if( itor != m_mapString.end() )
		{
			return itor->second;
		} //if

		m_mapString.insert( std::make_pair( std::string( szFile ), m_dwId ) );
		m_mapWord.insert( std::make_pair( m_dwId, std::string( szFile ) ) );
		m_dwId++;
		return m_dwId - 1;
	} 

	/**
	 *
	 */
	const char* GetFile( WORD w )
	{
		DWORDMap::iterator itor = m_mapWord.find( w );

		if( itor != m_mapWord.end() )
		{
			return itor->second.c_str();
		} //if

		return "";
	} //GetFile

	/**
	 *
	 */
	void Save( FILE* fp )
	{
		int nCount = m_mapString.size();
		fwrite( &nCount, 4, 1, fp );

		StringMap::iterator itor = m_mapString.begin();

		while( itor != m_mapString.end() )
		{
			DWORD id = itor->second;
			char szName[80];
			int nSize = 0;

			sprintf( szName, "%s", itor->first.c_str() );
			nSize = strlen( szName ) + 1;

			if( nSize < 4 )
			{
				MessageBox( NULL, "파일 크기가 맛간게 있습니다.", "오류", MB_OK );
			} //if

			fwrite( &id, 4, 1, fp );
			fwrite( &nSize, 4, 1, fp );
			fwrite( szName, nSize, 1, fp );			
			
			itor++;
		} //while
	} //Save

	/**
	 *
	 */
	void Load( FILE* fp )
	{
		Clear();

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

			if( nLen >= 3 )
			{
				m_mapString.insert( std::make_pair( std::string( szName ), dwId ) );
				m_mapWord.insert( std::make_pair( dwId, std::string( szName ) ) );

				if( dwId >= m_dwId )
				{
					m_dwId = dwId + 1;
				} //if
			}
			else
			{
				MessageBox( NULL, "asdf" , "asdf", MB_OK );
			} //if

		} //for
	} //Load

	
private:

	typedef std::map<std::string, DWORD> StringMap;
	typedef std::map<DWORD, std::string> DWORDMap;

	StringMap		m_mapString;
	DWORDMap		m_mapWord;
	DWORD			m_dwId;
};


extern CFileMgr g_FileMgr;



/**
 *
 */
struct  SMotion
{
	DWORD	dwBegin;			///< 시작 프레임
	DWORD	dwEnd;				///< 끝 프레임

	WORD	wAniFile;
	WORD	wWeaponFile;
	WORD	wSoundFile;

	
	
	void	Clear()
	{
		dwBegin = 1;
		dwEnd = 0;

		wAniFile	= 0;
		wWeaponFile = 0;
		wSoundFile	= 0;
	} //Clear

}; //struct SMotion

/**
 *
 */
struct SAction
{
	int			nCount;
	int			nSelect;
	SMotion		aMotion[3];

	void		Clear()
	{
		nCount = 0;
		nSelect = 0;

		for( int i = 0; i < 3 ; i++ )
		{
			aMotion[i].Clear();
		} //for
	} //if
}; // struct SAction

/**
 *
 */
struct SMode
{
	int			nCount;
	SAction		aAction[50];

	void		Clear()
	{
		nCount = MAX_STATE;
		for( int i = 0 ; i < 50 ; i++ )
		{
			aAction[i].Clear();
		} //for
	} //if
}; //struct SMode

/**
 *
 */
struct SWeapon
{
	BOOL		bUse;
	int			nCount;
	SMode		modePeace;
	SMode		modeWar;
	SMode		amodeVehicle[10];

	void		Clear()
	{
		nCount = 0;

		modePeace.Clear();
		modeWar.Clear();
		
		for( int i = 0 ; i < 10 ; i++ )
		{
			amodeVehicle[i].Clear();
		} //for
	} //if

}; //struct SWeapon

/**
 *
 */
class CActionMgr
{
public:
	
	/**
	 *
	 */
	CActionMgr()
	{
	//	m_apMaleWeapon = NULL;
	//	m_apFemaleWeapon = NULL;

		Init(); 
	} //CActionMgr

	/**
	 *
	 */
	~CActionMgr()
	{
		Release();
	} //~CActionMgr

	/**
	 *
	 */
	void	Init()
	{
		Release();
		g_nAttackNumber = 0;


		for( int i = 0 ; i < MAX_WEAPON ; i++ )
		{	
			m_aMaleWeapon[i].Clear();
			m_aFemaleWeapon[i].Clear();
		} //for		
		m_aMasterWeapon.Clear();

		m_pSelectSex = m_aMaleWeapon;
		m_pSelectWeapon = &m_aMaleWeapon[0];
	} //Init

	/**
	 *
	 */
	void	Release()
	{
	} //Release

	/**
	 *
	 */
	void	SetSex( UINT nSex )
	{
		m_nSex = nSex;

		if( _FEMALE == m_nSex )
		{
			m_pSelectSex = m_aFemaleWeapon;
			m_pSelectWeapon = &m_aFemaleWeapon[m_dwWeapon];
		}
		else
		if( _MALE == m_nSex )
		{
			m_pSelectWeapon = &m_aMaleWeapon[m_dwWeapon];
			m_pSelectSex = m_aMaleWeapon;
		}
		else
		if( _MASTER == m_nSex )
		{
			m_pSelectWeapon = &m_aMasterWeapon;
			m_pSelectSex = &m_aMasterWeapon;
		}

	} //SetSex

	/**
	 *
	 */
	SWeapon*		GetWeapon( DWORD dwId );

	/**
	 *
	 */
	SWeapon*		GetWeapon()
	{
		return m_pSelectWeapon;
	} //GetWeapon

	/**
	 *
	 */
	SAction*		GetAction( DWORD dwId, DWORD dwMode )
	{
		SMode* pMode = NULL;
		switch( dwMode )
		{
		case 0:	//< 평화 모드 
			pMode = &m_pSelectWeapon->modePeace;
			break;
		case 1: //< 전쟁 모드
			pMode = &m_pSelectWeapon->modeWar;
			break;
		default:
			pMode = &m_pSelectWeapon->amodeVehicle[dwMode-2];
		};

		return &pMode->aAction[dwId];
	} //GetAction

	SMode*			GetMode( DWORD dwId, DWORD dwMode )
	{
		
		switch( dwMode )
		{
		case 0:	//< 평화 모드 
			return &m_pSelectWeapon->modePeace;
			break;
		case 1: //< 전쟁 모드
			return &m_pSelectWeapon->modeWar;
			break;
		};
		
		//m_pSelectWeapon->
		//return m_pSelectWeapon;
	}

	/**
	 * 일반 남, 여 용
	 */
	void	SetWeapon( DWORD dwId )
	{
		m_dwWeapon = dwId;
		m_pSelectWeapon = &m_pSelectSex[dwId];
	} //SetWeapon

	/**
	 * 운영자 용
	 */
	void	SetWeapon()
	{
		m_dwWeapon = 0; // 맨손
		m_pSelectWeapon = m_pSelectSex;
	} //SetWeapon


	SWeapon* GetMaleWeapon()
	{
		return m_aMaleWeapon;
	} //GetMaleWeapon

	
	SWeapon* GetFemaleWeapon()
	{
		return m_aFemaleWeapon;
	} //GetFemaleWeapon

	SWeapon* GetMaleWeapon( DWORD w )
	{
		return &m_aMaleWeapon[w];
	} //GetMaleWeapon

	SWeapon* GetFemaleWeapon( DWORD w )
	{
		return &m_aFemaleWeapon[w];
	} //GetFemaleWeapon

	SWeapon* GetMasterWeapon()
	{
		return &m_aMasterWeapon;
	}

private:
		
	int				m_nCount;			///< 
	DWORD			m_dwWeapon;
//	BOOL			m_bSex;				///< 성별
	UINT			m_nSex;				///< 0 : 남자, 1 : 여자, 2 : 운영자

	/////////////////////////////////
	SWeapon*		m_pSelectWeapon;	///< 선택된 웨폰
	/////////////////////////////////

	SWeapon*		m_pSelectSex;		///< 선택된 성별
	SWeapon			m_aMaleWeapon[MAX_WEAPON];		///< 남자 무기
	SWeapon			m_aFemaleWeapon[MAX_WEAPON];	///< 여자 무기
	SWeapon			m_aMasterWeapon;				///< 운영자 무기
	/////////////////////////////////
}; //class CActionMgr

/**
 *
 */
class CVehicleMgr
{
public:
	CVehicleMgr()
	{

	}

	~CVehicleMgr()
	{

	}

	/**
	 *
	 */
	void	Init()
	{
		g_nAttackNumber = 0;


		for( int i = 0 ; i < MAX_WEAPON ; i++ )
		{	
			m_aMaleWeapon[i].Clear();
			m_aFemaleWeapon[i].Clear();
		} //for
		m_pSelectSex = m_aMaleWeapon;
		m_pSelectWeapon = &m_aMaleWeapon[0];
	} //Init

	/**
	 *
	 */
	void	SetSex( UINT nSex )
	{
		m_nSex = nSex;

		if( _MALE == nSex )
		{
			m_pSelectWeapon = &m_aMaleWeapon[m_dwWeapon];
			m_pSelectSex = m_aMaleWeapon;
		}
		else
		if( _FEMALE == nSex )
		{
			m_pSelectSex = m_aFemaleWeapon;
			m_pSelectWeapon = &m_aFemaleWeapon[m_dwWeapon];
		}
		else
		if( _MASTER == nSex )
		{
			m_pSelectSex = &m_aMasterWeapon;
			m_pSelectWeapon = &m_aMasterWeapon;
		}		
	} //SetSex

	/**
	 * 일반 남, 여 용
	 */
	void	SetWeapon( DWORD dwId )
	{
		m_dwWeapon = dwId;
		m_pSelectWeapon = &m_pSelectSex[dwId];
	} //SetWeapon

	/**
	 *	운영자 용
	 */
	void	SetWeapon()
	{
		m_dwWeapon = 0;	// 맨손
		m_pSelectWeapon = m_pSelectSex;
	} //SetWeapon

	
	/**
	 *
	 */
	SAction*		GetAction( DWORD dwId )
	{
		SMode* pMode = &m_pSelectWeapon->amodeVehicle[g_nVehicle];
		return &pMode->aAction[dwId];
	} //GetAction
private:

	SWeapon			m_aMaleWeapon[MAX_WEAPON];		///< 남자 무기
	SWeapon			m_aFemaleWeapon[MAX_WEAPON];	///< 여자 무기
	SWeapon			m_aMasterWeapon;				///< 운영자 무기
		
	int				m_nCount;			///< 
	DWORD			m_dwWeapon;
//	BOOL			m_bSex;				///< 성별
	UINT			m_nSex;				///< 0 : 남자, 1 : 여자, 2 : 운영자

	/////////////////////////////////
	SWeapon*		m_pSelectWeapon;	///< 선택된 웨폰
	SWeapon*		m_pSelectSex;		///< 선택된 성별
	/////////////////////////////////

	
}; //class CVehicleMgr

extern CActionMgr	g_ActionMgr;
extern CVehicleMgr	g_VehicleMgr;

#endif //#ifndef __GLOBAL_HEADER__

