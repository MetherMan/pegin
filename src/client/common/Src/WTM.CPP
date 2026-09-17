/**
 * WTM 파일 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-17 2:08a $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /Common/Src/WTM.CPP $
 * 
 * 4     03-12-17 2:08a Paran
 * 
 * 3     03-12-10 1:08a Paran
 * 소스분리
 * 
 * 2     03-12-08 10:08p Paran
 * WTM 파일 리더 추가
 * 
 * <HR>
 *
 * @file	Wtm.cpp
 */


#define COMMON_EXPORT

#include "../Inc/Common.h"


DWORD CWtm::Open( const char* szFileName )
{
	m_pFile = fopen( szFileName, "rb" );
	if( !m_pFile )
	{
		return 0;	
	} //if
	
	m_dwFileSize = GetFileSize( m_pFile );
	if( m_dwFileSize < SIZE_WTMHEAD )			// 'SIZE_WTMHEAD' 기본 정보크기
	{
		fclose( m_pFile );
		return 0;
	} //if

	BYTE*		pSrc = (BYTE*)_MemAlloc( m_dwFileSize );
	if( !pSrc )
	{
		Close();
		Assert( !"할당실패" );
		return FALSE;
	} //if

	DWORD		dwReadCnt = fread( pSrc, 1, m_dwFileSize, m_pFile );
	if( dwReadCnt != m_dwFileSize )
	{
		Assert( !"파일읽기 실패" );
		return FALSE;
	} //if
	Close();

	if( m_pData )
	{
		SAFE_FREE( m_pData );
	} //if
	m_dwOffset = 0;

	if( !IsWtmFile( pSrc ) )
	{
		m_dwDataSize = m_dwFileSize;
		m_pData = (BYTE*)_MemAlloc( m_dwDataSize );
		if( !m_pData )
		{
			Assert( !"할당실패" );
			return FALSE;
		} //if
	
		memcpy( m_pData, pSrc, m_dwFileSize );
		SAFE_FREE( pSrc );
		return TRUE;
	} //if

	memcpy( &m_dwDataSize, &pSrc[SIZE_WTMHEAD], 4 );

	m_pData = (BYTE*)_MemAlloc( m_dwDataSize );
	if( !m_pData )
	{
		Assert( !"할당실패" );
		return FALSE;
	} //if

	int			nHeadSize = SIZE_WTMHEAD + 4;

	DWORD		dwOriginLength = m_dwDataSize;
	DecodeData( m_pData, pSrc + nHeadSize, m_dwFileSize - nHeadSize, &dwOriginLength );
	SAFE_FREE( pSrc );
	if( dwOriginLength != m_dwDataSize )
	{
		SAFE_FREE( m_pData );
		Assert( !"읽기사이즈 실패" );
		return 0;
	} //if
	return m_dwDataSize;
} //Open

/**
 * 실제 데이터 얻기
 */
BYTE* CWtm::Data() const
{
	return m_pData;
} //GetData

/**
 * 데이터 얻기
 */
DWORD CWtm::SGets( BYTE* szDest, DWORD dwSize )
{
	Assert( szDest );

	if( !m_pData ||	
		m_dwOffset >= m_dwDataSize )
	{
		szDest[0] = '\0';
		return 0;
	} //if

	DWORD dwCnt = 0;
	while( dwCnt < dwSize )
	{
		if( m_pData[m_dwOffset] == '\r' ||
			m_pData[m_dwOffset] == '\n' ||
			m_dwOffset > m_dwDataSize
			)
		{
			szDest[dwCnt] = m_pData[m_dwOffset];
			m_dwOffset++;
			break;
		} //if
		szDest[dwCnt] = m_pData[m_dwOffset];
		m_dwOffset++;
		dwCnt++;
	} //for
	szDest[dwCnt] = '\0';
	return m_dwOffset;
} //SGets

/**
 * 실제 데이터 사이즈
 */
DWORD CWtm::Size() const
{
	return m_dwDataSize;
} //Size

/**
 * 디코딩 데이터.
 */
void CWtm::DecodeData( 
	BYTE* pDest, 
	const BYTE* pSrc, 
	const DWORD dwFileSize, 
	DWORD* pdwOriginSize 
	)
{
	uncompress( pDest, pdwOriginSize, pSrc, dwFileSize );
} //DecodeDate

/**
 * 옵셋 설정
 */
void CWtm::SSeek( DWORD dwOffset )
{
	m_dwOffset = dwOffset;
} //SSeek

/**
 * 파일 크기를 얻는다.
 */
DWORD CWtm::GetFileSize( FILE* pFile )
{
	if( !m_pFile )
	{
		Assert( pFile );
		return 0;
	} //if
	DWORD		dwPrevPos = ftell( pFile );
	fseek( pFile, 0, SEEK_END );
	DWORD		dwSize	  = ftell( pFile );
	fseek( pFile, dwPrevPos, SEEK_SET );

	return dwSize;
} //GetFileSize

/**
 * 파일확인
 */
BOOL CWtm::IsWtmFile( BYTE* pData )
{
	if( pData[0] != 'T' ||
		pData[1] != 'E' ||
		pData[2] != 'A' ||
		pData[3] != 'M' ||
		pData[4] != 'M' ||
		pData[5] != 'A' ||
		pData[6] != 'Y' )
	{
		return FALSE;
	} //if

	if( pData[7] != VERSION_WTM )		// 버젼 체크
	{
		Assert( !"파일 버젼이 다르다" );
		return FALSE;
	} //if
	return TRUE;
} //ISWtmFile

/**
 * 닫기
 */
void CWtm::Close()
{
 	if( m_pFile )
	{
		fclose( m_pFile );
		m_pFile = NULL;
	} //if
} //Close