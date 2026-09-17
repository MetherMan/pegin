/**
 * 매직아이템 정보 관리
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-04-23 7:47a $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /Engine/Src/W3DMagicInfoMgr.cpp $
 * 
 * 6     03-04-23 7:47a Ahastudio
 *
 * 4     03-03-22 2:37a Paran		매직 등록 부분 수정.
 * <HR>
 *
 * @file	W3DMagicInfoMgr.cpp
 */


#include "Engine_Export.h"
#include <stdio.h>
#include <map>


using namespace std;


/**
 * 마법 정보 구조체.
 * @ingroup		Engine
 */
struct MagicInfoData
{
	int			nNum;					///< 마법 번호
	int			nType;					///< 마법 종류
	char		szName[50];				///< 마법 이름
	char		szText[128];			///< 마법 텍스트
	char		szImgFileName1[50];		///< 리스트용 이미지 파일 이름
	char		szImgFileName2[50];		///< 작은 아이콘 파일 이름
	char		szImgFileName3[50];		///< 큰 아이콘 파일 이름
}; //struct MagicInfoData


typedef map<int, MagicInfoData>		MAP_MAGICINFODATA;


/**
 * 마법 정보 관리자.
 * @ingroup		Engine
 */
class CW3DMagicInfoMgr : public IW3DMagicInfoMgr, public Singleton<CW3DMagicInfoMgr>
{
	SINGLETON_COMPONENT( CW3DMagicInfoMgr );

public:
	UNDER_MEMORY_MGR( CW3DItemInfoMgr );

	CW3DMagicInfoMgr();
	~CW3DMagicInfoMgr();

	BOOL			Load( const char* szFileName );

	BOOL			OpenMagic( int nIndex );
	char*			GetName()					const;
	int				GetType()					const;
	char*			GetText()					const;
	char*			ImgFileName1()				const;
	char*			ImgFileName2()				const;
	char*			ImgFileName3()				const;

private:
	MAP_MAGICINFODATA	m_mapItemInfoData;				///< 아이템 맵
	MagicInfoData*		m_pMagicInfo;					///< 현재 작업 중인 마법 정보
}; //class CW3DMagicInfoMgr


IMPLEMENT_SINGLETON_COMPONENT( CW3DMagicInfoMgr, MagicInfoMgr );


/**
 * 생성자.
 */
CW3DMagicInfoMgr::CW3DMagicInfoMgr()
{
	m_pMagicInfo	= NULL;
} //CW3DMagicInfoMgr::CW3DMagicInfoMgr

/**
 * 소멸자.
 */
CW3DMagicInfoMgr::~CW3DMagicInfoMgr()
{
	m_mapItemInfoData.clear();
} //CW3DMagicInfoMgr::~CW3DMagicInfoMgr

/**
 * 마법 정보 파일을 읽는다.
 */
BOOL CW3DMagicInfoMgr::Load( const char* szFileName )
{
	Assert( szFileName );

	DEBUG_OUT( "마법 정보 읽기 - " + szFileName );

	FILE*		pFile = fopen( szFileName, "rt" );
	if( !pFile )
	{
		Assert( !"파일 읽기 실패" );
		return FALSE;
	} //if

	MagicInfoData		magicInfo;
	char				szType[50];

	while( 1 )
	{
		if( fscanf( pFile, "%d", &magicInfo.nNum ) == EOF )
		{
			break;		// 파일의 끝이면 쀍
		} //if
		if( fscanf( pFile, "%s", magicInfo.szName ) == EOF )
		{
			break;		// 파일의 끝이면 쀍
		} //if
		if( fscanf( pFile, "%s", szType ) == EOF )
		{
			break;		// 파일의 끝이면 쀍
		} //if
		if( fscanf( pFile, "%s", magicInfo.szText ) == EOF )
		{
			break;		// 파일의 끝이면 쀍
		} //if
		if( fscanf( pFile, "%s", magicInfo.szImgFileName1 ) == EOF )
		{
			break;		// 파일의 끝이면 쀍
		} //if
		if( fscanf( pFile, "%s", magicInfo.szImgFileName2 ) == EOF )
		{
			break;		// 파일의 끝이면 쀍
		} //if
		if( fscanf( pFile, "%s", magicInfo.szImgFileName3 ) == EOF )
		{
			break;		// 파일의 끝이면 쀍
		} //if

		if( !strcmp( szType, "BLACK" ) )
		{
			magicInfo.nType = IW3DMagicInfoMgr::BLACK_MAGIC;
		}
		else if( !strcmp( szType, "WHITE" ) )
		{
			magicInfo.nType = IW3DMagicInfoMgr::WHITE_MAGIC;
		}
		else
		{
			magicInfo.nType = -1;
		} //if..else if..else..

		MAP_MAGICINFODATA::iterator		iterMap_MagicInfo =	m_mapItemInfoData.find( magicInfo.nNum );
		if( iterMap_MagicInfo != m_mapItemInfoData.end() )
		{	// 이미 존재하는 마법 정보.
			Assert( !"이미 있는 듯" );
			return FALSE;
		} //if

		m_mapItemInfoData.insert( make_pair( magicInfo.nNum, magicInfo ) );
	} //while

	fclose( pFile );

	return TRUE;
} //CW3DMagicInfoMgr::Load

/**
 *
 */
BOOL CW3DMagicInfoMgr::OpenMagic( int nIndex )
{
	MAP_MAGICINFODATA::iterator	iterMap_MagicData = m_mapItemInfoData.find( nIndex );
	if( iterMap_MagicData == m_mapItemInfoData.end() )
	{
		Assert( !"nIndex에 해당하는 마법이 존재하지 않습니다." );
		m_pMagicInfo = NULL;
		return FALSE;
	} //if

	m_pMagicInfo = &iterMap_MagicData->second;

	return TRUE;
} //CW3DMagicInfoMgr::OpenMagic

/**
 *
 */
char* CW3DMagicInfoMgr::GetName() const
{
	Assert( m_pMagicInfo );
	return m_pMagicInfo->szName;
} //CW3DMagicInfoMgr::GetName

/**
 *
 */
int	CW3DMagicInfoMgr::GetType()	const
{
	Assert( m_pMagicInfo );
	return m_pMagicInfo->nType;
} //CW3DMagicInfoMgr::GetType

/**
 *
 */
char* CW3DMagicInfoMgr::GetText() const
{
	Assert( m_pMagicInfo );
	return m_pMagicInfo->szText;
} //CW3DMagicInfoMgr::GetText

/**
 * 리스트용 이미지 파일 이름을 얻는다.
 */
char* CW3DMagicInfoMgr::ImgFileName1() const
{
	Assert( m_pMagicInfo );
	return m_pMagicInfo->szImgFileName1;
} //CW3DMagicInfoMgr::ImgFileName1

/**
 * 작은 아이콘 파일 이름을 얻는다.
 */
char* CW3DMagicInfoMgr::ImgFileName2() const
{
	Assert( m_pMagicInfo );
	return m_pMagicInfo->szImgFileName2;
} //CW3DMagicInfoMgr::ImgFileName2

/**
 * 큰 아이콘 파일 이름을 얻는다.
 */
char* CW3DMagicInfoMgr::ImgFileName3() const
{
	Assert( m_pMagicInfo );
	return m_pMagicInfo->szImgFileName3;
} //CW3DMagicInfoMgr::ImgFileName3
