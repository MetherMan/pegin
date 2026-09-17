/**
 * 텍스처 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	Teammay
 * @date	$Date: 04-02-09 11:02a $
 * @version	$Revision: 32 $
 *
 * <HR>
 * $Log: /Render/Src/Texture.cpp $
 * 
 * 32    04-02-09 11:02a Paran
 * 
 * 31    04-01-08 12:01p Kjmgo
 * 코드 정리
 *
 * <HR>
 *
 * @file	Texture.cpp
 */


#pragma comment( lib, "zlib.lib"  )

#include "Render_Export.h"
#include "Texture.h"
#include <zlib.h>
#include <io.h>

/**
 * 생성자.
 */
CTextureMgr::CTextureMgr( LPDIRECT3DDEVICE8 pd3dDevice )
:	m_pd3dDevice	( pd3dDevice ),
	m_btLevel		( LEV_HIGH )
{
} //CTextureMgr::CTextureMgr

/**
 * 소멸자.
 * DEBUG 모드에서는 릴리즈 되지 않은 텍스처를 검사한다.
 * 특별한 에러 코드는 없고, 내부에서 Assert가 경고한다.
 */
CTextureMgr::~CTextureMgr()
{
#ifdef _DEBUG
	int		numRemainTextures = m_mapTexture.size();

	Assert( 0 == numRemainTextures &&
			"Release 되지 않은 Texture가 있습니다" );

	TextureMap::iterator	itor = m_mapTexture.begin();
	while( itor != m_mapTexture.end() )
	{
		CTexture*		pTexture = itor->second;
		const string&	strName = pTexture->GetName();
		Assert( !"Release 되지 않은 Texture" );
		itor++;
	} //while

#endif // #ifdef _DEBUG
} //CheckRemainTexture

/**
 * 텍스쳐 레벨 설정 함수.
 *
 * @param		btLevel		[IN] 레벨
 */
void CTextureMgr::SetLevel( BYTE btLevel )
{
	m_btLevel = btLevel;
} //CTextureMgr::SetLevel

/**
 * 텍스처를 읽어오는 함수.
 * 반복되는 텍스처가 있을 경우, Reference Count만 올려준다.
 *
 * @param		strName		[IN] 텍스처 이름
 *
 * @return		텍스처
 */
CTexture* CTextureMgr::LoadTexture( 
	const string& strPath, 
	const string& strName, 
	const int nMode )
{
	TextureMap::iterator	itor = m_mapTexture.find( strPath + strName );

	if( itor == m_mapTexture.end() )
	{	// 새로운 텍스처일 경우..
		if( !GetFullPathName( m_strTemp, strPath, strName ) )
		{
			DEBUG_OUT( ( strPath + strName ).c_str() + "텍스쳐가 없다" );
			return NULL;
		} //if

		CTexture*	pTexture = new CTexture;
		if( !pTexture )
		{
			Assert( !"할당실패" );
			return NULL;
		} //if 

		if( !pTexture->Load( m_pd3dDevice, m_strTemp, strPath + strName, nMode ) )
		{
			pTexture->Release();
			DEBUG_OUT( "CTextureMgr::LoadTexture - 텍스처 읽기 실패 - " + strName.c_str() );
			return NULL;
		} //if

		m_mapTexture.insert( std::make_pair( strPath + strName, pTexture ) );

		return pTexture;
	}
	else
	{	// 기존에 있던 텍스처일 경우..
		CTexture* pTexture = itor->second;
		pTexture->AddRef();
		return pTexture;
	} //if..else..

	return NULL;
} //LoadTexture

/**
 * 텍스처를 리스트에서 제거한다.
 * 텍스처 내부의 Release에서 Reference Count가 0이 되면 이 함수를 호출한다.
 *
 * @param	pTexture		[IN] 텍스처 객체.
 */
void CTextureMgr::UnloadTexture( CTexture* pTexture )
{
	Assert( pTexture );

	TextureMap::iterator	itor = m_mapTexture.find( pTexture->GetName() );

	if( itor != m_mapTexture.end() )
	{
		m_mapTexture.erase( itor );
	} //if
} //CTextureMgr::UnloadTexture

/**
 * 설정레벨에 맞는 파일을 얻는다.
 * 레벨에 맞는 파일이 없다면 다른 레벨의 파일을 검색해본다.
 *
 * @param		strDest		[IN] 버퍼스트링
 * @param		strFile		[IN] 구하는 파일 이름
 */
BOOL CTextureMgr::GetFullPathName( 
	string&			strDest, 
	const string&	strPath, 
	const string&	strFile 
	)
{
	if( !strFile.compare( "" ) )
	{
		return FALSE;
	} //if

	BYTE		btIndex;
	char		aszLevel[3][8] = { "High/", "Normal/", "Low/" };

	if( m_btLevel == 0 )
	{ //High
		strDest = strPath + strFile;
		if( _access( strDest.c_str(), 0 ) != -1 )
		{
			return TRUE;
		} //if
			
		char	*pExeName = (char*)strDest.c_str() + strDest.length() - 3;
		strcpy( pExeName, "wtm" );
		if( _access( strDest.c_str(), 0 ) != -1 )
		{
			return TRUE;
		} //if
	} //if

	for( int nCnt = 0; nCnt < 3; nCnt++ )
	{
		btIndex = nCnt + m_btLevel;
		if( btIndex >= 3 )
		{
			btIndex -= 3;
		} //if

		strDest = strPath + aszLevel[btIndex] + strFile;
		if( _access( strDest.c_str(), 0 ) != -1 )
		{
			return TRUE;
		} //if

		char	*pExeName = (char*)strDest.c_str() + strDest.length() - 3;
		strcpy( pExeName, "wtm" );
		if( _access( strDest.c_str(), 0 ) != -1 )
		{
			return TRUE;
		} //if
	} //for

	strDest = strPath + strFile;
	if( _access( strDest.c_str(), 0 ) != -1 )
	{
		return TRUE;
	} //if
		
	char	*pExeName = (char*)strDest.c_str() + strDest.length() - 3;
	strcpy( pExeName, "wtm" );
	if( _access( strDest.c_str(), 0 ) != -1 )
	{
		return TRUE;
	} //if

	return FALSE;
} //CTextureMgr::GetFullPathName

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

/**
 * 텍스처 객체를 릴리즈한다.
 * Reference Count를 낮추고, 전혀 사용되지 않을 경우 이 객체를 delete한다.
 */
void CTexture::Release()
{
	if( --m_nRef <= 0 )
	{
		CTextureMgr*	pTextureMgr = CTextureMgr::GetSingletonPtr();
		Assert( pTextureMgr );

		pTextureMgr->UnloadTexture( this );
		delete this;
	} //if
} //CTexture::Release

/**
 * 텍스처를 파일에서 읽어온다.
 *
 * @retrun	텍스처 읽기 성공 여부
 */
BOOL CTexture::Load(
	LPDIRECT3DDEVICE8	pd3dDevice,		///< [IN] Direct3D Device
	const string&		strFileName,	///< [IN] 텍스처 파일 이름
	const string&		strName,		///< [IN] 텍스쳐 키 이름
	int					nMode			///< [IN] 텍스쳐 모드	
	)
{
	Assert( pd3dDevice );

	HRESULT		hr;
	BYTE*		pData;
	int			nSize = ReadFile( strFileName, reinterpret_cast<void**>( &pData ) );

	if( nSize < 0 )
	{
		return FALSE;
	} //if

	if( nMode == CTextureMgr::MODEL )
	{ //모델에 쓰이는 텍스쳐
		hr = D3DXCreateTextureFromFileInMemoryEx(
			pd3dDevice,
			pData,
			nSize,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,			// MipLevels
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
			D3DX_DEFAULT,
			0,
			NULL,
			NULL,
			&m_pTexture );
	}
	else if( nMode == CTextureMgr::SPRITE )
	{ //스프라이트에 쓰이는 텍스쳐
		hr = D3DXCreateTextureFromFileInMemoryEx(
			pd3dDevice,
			pData,
			nSize,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,			// MipLevels
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0,
			NULL,
			NULL,
			&m_pTexture );
	} 
	else
	{ //아무것도 아닌 텍스쳐
		Assert( "없는 모드" );
	}//if..if else..else

	SAFE_DELETE( pData );

	if( FAILED( hr ) )
	{
		DEBUG_OUT( "텍스처 읽기 실패 - " + strFileName.c_str() );
		return FALSE;
	} //if

	m_sName = strName;
	return TRUE;
} //CTexture::Load

/**
 * 파일을 읽어온다.
 */
int CTexture::ReadFile( const string& strFileName, VOID** ppBuffer )
{
	Assert( ppBuffer );

	char	szConvertName[256];
	strcpy( szConvertName, strFileName.c_str() );
	strcpy( szConvertName + strlen( szConvertName ) - 3, "wtm" );
	
	FILE*	fp = fopen( szConvertName, "rb" );
	if( !fp )
	{
		fp = fopen( strFileName.c_str(), "rb" );
		if( !fp )
		{
			*ppBuffer = NULL;
			return -1;
		}
	}

	ArchiveFileReader	ar( fp );

	int		nFileSize	= ar.GetSize();
	BYTE*	pFileData	= new BYTE [nFileSize];
	CHECK_PTR_RETURN( pFileData );
	ar.Serialize( pFileData, nFileSize );

	int		nSize;
	BYTE*	pData;

	if( nFileSize > 9 &&
		'T' == pFileData[0] &&
		'E' == pFileData[1] &&
		'A' == pFileData[2] &&
		'M' == pFileData[3] &&
		'M' == pFileData[4] &&
		'A' == pFileData[5] &&
		'Y' == pFileData[6] &&
		0x0 == pFileData[8]		// Version : 0
		)
	{	// 압축된 텍스처 파일일 경우 압축을 푼다.
		//enum { BUFFER_SIZE = 1024 * 1024 * 10 };
		memcpy( &nSize, &pFileData[9], 4 );
		if( nSize <= 0 )
		{
			SAFE_DELETE_ARRAY( pFileData );
			return -1;
		}
		pData = new BYTE [nSize];
		uncompress( pData, reinterpret_cast<DWORD*>( &nSize ), pFileData + 13, nFileSize - 13 );

		SAFE_DELETE_ARRAY( pFileData );
	}
	else
	{	// 압축되지 않은 텍스처 파일일 경우 바로 사용한다.
		nSize  = nFileSize;
		pData  = pFileData;
	} //if..else..

	*ppBuffer = pData;

	ar.Close();
	return nSize;
} //CTexture::ReadFile
