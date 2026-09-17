/**
 * 컴포넌트 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-05-10 5:52p $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /Common/Src/Component.cpp $
 * 
 * 11    03-05-10 5:52p Ahastudio
 *
 * 10    03-05-10 12:45p Ahastudio		AddComponent() 추가.
 * <HR>
 *
 * @file	Component.cpp
 */


#define COMMON_EXPORT

#include "../Inc/Common.h"
#include <string>
#include <map>


using std::string;
using std::map;


typedef map<string, HMODULE>			ComponentPackageMap;
typedef map<string, ComponentInfo*>		ComponentInfoMap;


ComponentPackageMap		g_mapComponentPackage;	///< 컴포넌트 패키지 목록.
ComponentInfoMap		g_mapComponentInfo;		///< 컴포넌트 정보 목록.


/**
 * Component를 추가한다.
 * @ingroup		Common
 *
 * @param		apComponentInfo		[IN] 컴포넌트 정보 배열
 *
 * @return		추가 성공 여부.
 */
BOOL AddComponent( ComponentInfo** apComponentInfo )
{
	Assert( apComponentInfo );

	for( int i = 0 ; apComponentInfo[i] ; i++ )
	{
		Assert( apComponentInfo[i] );

		ComponentInfoMap::iterator	itor = g_mapComponentInfo.find( apComponentInfo[i]->szName );
		if( itor != g_mapComponentInfo.end() )
		{
			Assert( !"이미 존재하는 컴포넌트를 추가하려고 했습니다." );
			throw "엔진 컴포넌트 초기화 오류";
			return FALSE;
		} //if

		g_mapComponentInfo.insert(
			std::make_pair( apComponentInfo[i]->szName, apComponentInfo[i] ) );
	} //for

	return TRUE;
} //AddComponent

/**
 * Component 패키지를 로드한다.
 * @ingroup		Common
 *
 * @param		szFileName		[IN] 패키지 파일 이름.
 *
 * @return		로드 성공 여부.
 *
 * @exception	실패할 경우 문자열 Exception
 */
BOOL LoadComponentPackage( const char* szFileName )
{
	Assert( szFileName );

	HMODULE hLibrary;

	hLibrary = LoadLibrary( szFileName );
	if( NULL == hLibrary )
	{
		char	szMsg[256];
		sprintf( szMsg, "%s 파일을 찾을 수 없습니다.", szFileName );
		throw szMsg;
		return FALSE;
	} //if

	g_mapComponentPackage.insert( std::make_pair( szFileName, hLibrary ) );

	typedef ComponentInfo**		(*PACKAGE)();

	PACKAGE		package = (PACKAGE)GetProcAddress( hLibrary, "Package" );

	AddComponent( package() );

	return TRUE;
} //LoadComponentPackage

/**
 * Component 패키지를 언로드한다.
 * @ingroup	Common
 *
 * @param	szFileName		[IN] 패키지 파일 이름.
 */
void UnloadComponentPackage( const char* szFileName )
{
	Assert( szFileName );

	ComponentPackageMap::iterator	itor = g_mapComponentPackage.find( szFileName );

	if( itor != g_mapComponentPackage.end() )
	{
		HMODULE		hLibrary = itor->second;
		FreeLibrary( hLibrary );
		g_mapComponentPackage.erase( itor );
	} //if
} //UnloadComponentPackage

/**
 * Component를 생성한다.
 * @ingroup	Common
 *
 * @param	szName			[IN] Component 이름.
 *
 * @return	컴포넌트
 */
IComponent* MakeComponent( const char* szName )
{
	Assert( szName );

	ComponentInfoMap::iterator	itor = g_mapComponentInfo.find( szName );

	if( itor == g_mapComponentInfo.end() )
	{
		Assert( !"Make Component Failed : Not Found Component.." );
		return NULL;
	} //if

	ComponentInfo* pComponentInfo = itor->second;

	return pComponentInfo->Factory();
} //MakeComponent

/**
 * 레퍼런스 카운트를 증가시킨다.
 * @ingroup	Common
 *
 * @param	pCom			[IN] Component 객체
 */
void _ComponentReference( IComponent* pCom )
{
	Assert( pCom );

	++pCom->m_nReferenceCount;
} //_ComponentReference

/**
 * Release.
 * @ingroup	Common
 *
 * @param	pCom			[IN] Component 객체
 */
void _ComponentRelease( IComponent* pCom )
{
	Assert( pCom );

	--pCom->m_nReferenceCount;

	if( pCom->m_nReferenceCount <= 0 )		delete pCom;
} //_ComponentRelease
