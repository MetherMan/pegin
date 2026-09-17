/**
 * 몬스터 정보 관리자 소스 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-12-09 6:28p $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /Engine/Src/W3DMonsterInfo.cpp $
 * 
 * 6     04-12-09 6:28p Sk8snow2
 * 
 * 5     03-03-09 9:04p Ahastudio
 * STL 사용법 변경.
 * 
 * 4     03-02-28 12:56p Ahastudio	주석 수정.
 * 3     03-02-28 11:32a Paran
 * <HR>
 * @file	W3DMonsterInfo.cpp
 */


#include "Engine_Export.h"
#include <stdio.h>
#include <fstream>
#include <string>
#include <map>


using namespace std;


/**
 * 몬스터 정보 구조체.
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
struct MonsterInfo
{
	char			szName[64];
	char			szEngName[64];
	int				nDamage;
};


typedef map<int, MonsterInfo*>	MAP_MONSTERINFO;


/**
 * MonsterInfo 구현부.
 * @ingroup	Engine
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
class CW3DMonsterInfo : public IW3DMonsterInfo
{
public:
	UNDER_MEMORY_MGR( CW3DMonsterInfo )

	CW3DMonsterInfo();
	~CW3DMonsterInfo();

	BOOL			Load( const char* szFileName );
	BOOL			OpenMonsterInfo( int nIndex );
	char*			GetName();
	char*			GetEngName();
	int				GetDamage();

private:
	MAP_MONSTERINFO		m_mapMonsterInfo;		///< 몬스터 정보들.
	MonsterInfo*		m_pCurrentInfo;			///< 현재 선택된 몬스터 정보.

	BOOL			ReadNumber( char** ppData, int* piNumber );
	BOOL			ReadName( char** ppData, char* szName );
	BOOL			ReadEngName( char** ppData, char* szEngName );
	BOOL			ReadDamage( char** ppData, int* pnDamage );

	BOOL			IsSkipChar( char chData );
	BOOL			IsComment( char* pData );

	BOOL			AddInfo( int nIndex, const MonsterInfo* pInfo );
}; //class CW3DMonsterInfo


IMPLEMENT_COMPONENT( CW3DMonsterInfo, MonsterInfo );


/**
 * 생성자.
 */
CW3DMonsterInfo::CW3DMonsterInfo()
{
	m_pCurrentInfo = NULL;
} //CW3DMonsterInfo::CW3DMonsterInfo

/**
 * 소멸자.
 */
CW3DMonsterInfo::~CW3DMonsterInfo()
{
	MAP_MONSTERINFO::iterator	itor = m_mapMonsterInfo.begin();
	MAP_MONSTERINFO::iterator	itorNext;

	while( itor != m_mapMonsterInfo.end() )
	{
		itorNext = itor;
		itorNext++;

		MonsterInfo*	pMonInfo = itor->second;
		_MemFree( pMonInfo );
		m_mapMonsterInfo.erase( itor );

		itor = itorNext;
	}
}

/**
 * 인포파일을 로드합니다.
 *
 * @param	szFileName		[IN] 인포 파일(e.g. monsters.dat)
 * @return	로드 성공/실패
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
BOOL CW3DMonsterInfo::Load( const char* szFileName )
{
	Assert( szFileName );

	ifstream		scrfile( szFileName );
	string			strLine;
	char*			pCurData;
	char*			pData;
	char			aBuff[64];
	int				nIndex, nDamage;
	MonsterInfo*	pMonInfo = NULL;

	if( scrfile.is_open() )
	{
		while( 1 )
		{
			getline( scrfile, strLine );
			if( scrfile.fail() )
			{
				break;
			}

			pData = (char*)strLine.c_str();
			for( pCurData = pData ; pCurData < pData + strLine.length() ; pCurData++ )
			{
				if( IsSkipChar( *pCurData ) )
				{
					continue;
				}
				if( IsComment( pCurData ) )
				{
					break;
				}

				if( ReadNumber( &pCurData, &nIndex ) )
				{
					pMonInfo = (MonsterInfo*)_MemAlloc( sizeof( MonsterInfo ) );
					if( !pMonInfo )
					{
						return FALSE;
					}
					if( !AddInfo( nIndex, pMonInfo ) )
					{
						_MemFree( pMonInfo );
						return FALSE;
					}
					if( ReadName( &pCurData, aBuff ) )
					{
						strcpy( pMonInfo->szName, aBuff );
						break;
					}
				}
			
				if( ReadEngName( &pCurData, aBuff ) )
				{
					strcpy( pMonInfo->szEngName, aBuff );
					break;
				}

				if( ReadDamage( &pCurData, &nDamage ) )
				{
					pMonInfo->nDamage = nDamage;
					break;
				}
			}
		}
	}
	return TRUE;
}

/**
 * ...
 *
 * @param	ppData		[OUT]
 * @param	piNumber	[OUT]
 * @return	...
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
BOOL CW3DMonsterInfo::ReadNumber( char** ppData, int* piNumber )
{
	char	aBuff[8];
	
	if( sscanf( *ppData, "%s", aBuff ) != 1 )
	{
		Assert( !"sscanf 오류" );
	}

	if( aBuff[0] == '[' && aBuff[strlen( aBuff ) - 1] == ']' )
	{
		aBuff[strlen( aBuff ) - 1] = '\0';
		*piNumber = atoi( aBuff+1 );
		*ppData += strlen( aBuff );
		return TRUE;
	}
	return FALSE;
}

/**
 * ...
 *
 * @param	ppData		[IN/OUT]
 * @param	szName		[OUT]
 * @return	...
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
BOOL CW3DMonsterInfo::ReadName( char** ppData, char* szName )
{
	BOOL		bBegin = FALSE;
	char*		pBuff;
	char		szMonName[64];
	int			nLength = strlen( *ppData );
	int			nCnt = 0;

	for( pBuff = *ppData ; pBuff < ( (*ppData) + nLength ) ; pBuff++ )
	{
		if( *pBuff == '"' )
		{
			bBegin = !bBegin;
			if( !bBegin )
			{
				szMonName[nCnt] = '\0';
				strcpy( szName, szMonName );
				*ppData = pBuff;
				return TRUE;
			}
			continue;
		}

		if( bBegin )
		{
			szMonName[nCnt] = *pBuff;
			nCnt++;
		}
	}

	return FALSE;
}

/**
 * ...
 *
 * @param	ppData			[IN/OUT]
 * @param	szEngName		[OUT]
 * @return	...
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
BOOL CW3DMonsterInfo::ReadEngName( char** ppData, char* szEngName )
{
	BOOL		bBegin = FALSE;
	char		szName[64];
	int			nLength = strlen( *ppData );
	int			nCnt = 0;
	char*		pBuff = *ppData;

	if( *(pBuff++) == 'e' && 
		*(pBuff++) == 'n' && 
		*(pBuff++) == 'a' &&
		*(pBuff++) == 'm' &&
		*(pBuff++) == 'e' )
	{

		nCnt = 0;
		for( ; pBuff < ( (*ppData) + nLength ) ; pBuff++ )
		{
			if( *pBuff == '"' )
			{
				bBegin = !bBegin;
				if( !bBegin )
				{
					szName[nCnt] = '\0';
					strcpy( szEngName, szName );
					*ppData = pBuff;
					return TRUE;
				}
				continue;
			}

			if( bBegin )
			{
				szName[nCnt] = *pBuff;
				nCnt++;
			}
		}
	}
	return FALSE;
}

/**
 * ...
 *
 * @param	ppData		[IN/OUT]
 * @param	pnDamage	[OUT]
 * @return	...
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
BOOL CW3DMonsterInfo::ReadDamage( char** ppData, int* pnDamage )
{
	BOOL	bBegin = FALSE;
	char*	pBuff = *ppData;
	int		nDamage;
	int		nLength = strlen( *ppData );

	if( *(pBuff++) == 'd' && 
		*(pBuff++) == 'a' && 
		*(pBuff++) == 'm' &&
		*(pBuff++) == 'a' &&
		*(pBuff++) == 'g' &&
		*(pBuff++) == 'e' )
	{
		for( ; pBuff < ( (*ppData) + nLength ) ; pBuff++ )
		{
			if( *pBuff == ':' )
			{
				pBuff++;
				if( sscanf( pBuff, "%d", &nDamage ) != 1 )
				{
					Assert( !"sscanf 오류" );
				}

				*pnDamage = nDamage;
				*ppData = pBuff;
				return TRUE;
			}
		}
	}

	return FALSE;
}

/**
 * White Space인지 검사한다.
 *
 * @param	chData		[IN] 검사할 문자.
 * @return	White Space일 경우 TRUE.
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
BOOL CW3DMonsterInfo::IsSkipChar( char chData )
{
	if( chData == 0x20 || chData == 0x09 )		// Space, Tab
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * 주석인지 검사한다. ( // )
 *
 * @param	pData		[IN] 검사힐 문자열.
 * @return	주석일 경우 TRUE.
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
BOOL CW3DMonsterInfo::IsComment( char* pData )
{
	if( pData[0] == '/' && pData[1] == '/' )
	{
		return TRUE;
	}
	return FALSE;
}

/**
 * ...
 *
 * @param	nIndex		[IN]
 * @param	pInfo		[IN]
 * @return	...
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
BOOL CW3DMonsterInfo::AddInfo( int nIndex, const MonsterInfo* pInfo )
{
	MAP_MONSTERINFO::iterator	iterMap_MonInfo;

	iterMap_MonInfo = m_mapMonsterInfo.find( nIndex );

	if( iterMap_MonInfo != m_mapMonsterInfo.end() )
	{
		return FALSE;
	}
	
	m_mapMonsterInfo.insert( make_pair( nIndex, const_cast<MonsterInfo*>(pInfo) ) );

	return TRUE;
}

/**
 * 원하는 몬스터 정보를 열어줍니다.
 *
 * @param	nIndex			[IN] 몬스터 인덱스
 * @return	오픈 성공/실패
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
BOOL CW3DMonsterInfo::OpenMonsterInfo( int nIndex )
{
	MAP_MONSTERINFO::iterator	iterMap_MonInfo;

	iterMap_MonInfo = m_mapMonsterInfo.find( nIndex );

	if( iterMap_MonInfo != m_mapMonsterInfo.end() )
	{
		m_pCurrentInfo = iterMap_MonInfo->second;
		return TRUE;
	}

	return FALSE;
}

/**
 * 현재 열려 있는 몬스터 이름을 얻는다.
 *
 * @return	몬스터이름/NULL
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
char* CW3DMonsterInfo::GetName()
{
	if( !m_pCurrentInfo )
	{
		return NULL;
	}
	return m_pCurrentInfo->szName;
}

/**
 * 현재 열려 있는 몬스터 영어 이름을 얻는다.
 *
 * @return	몬스터영어이름/NULL
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
char* CW3DMonsterInfo::GetEngName()
{
	if( !m_pCurrentInfo )
	{
		return NULL;
	}
	return m_pCurrentInfo->szEngName;
}

/**
 * 현재 열려 있는 몬스터의 데미지를 얻는다.
 *
 * @return	Damage/-1
 *
 * @author	파란~*
 * @date	2003-02-28 오전 10:28
 */
int CW3DMonsterInfo::GetDamage()
{
	if( !m_pCurrentInfo )
	{
		return -1;
	}
	return m_pCurrentInfo->nDamage;
}
