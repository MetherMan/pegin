/**
 * WTM 파일 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-10 1:08a $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Common/Inc/WTM.H $
 * 
 * 3     03-12-10 1:08a Paran
 * 소스분리
 * 
 * 2     03-12-09 4:00p Paran
 * wtm 파일 포맷 클래스 추가
 * 
 * 1     03-12-09 2:23a Paran
 * 
 * 3     03-12-09 2:22a Paran
 * 
 * 2     03-12-08 10:08p Paran
 * WTM 파일 리더 추가
 * 
 * <HR>
 *
 * @file	Wtm.h
 */

#ifndef __Wind3D_Common_WTM_Header__
#define __Wind3D_Common_WTM_Header__

#pragma comment( lib, "zlib.lib"  )

#include <zlib.h>

class COMMON_API CWtm
{
public:
	enum { SIZE_WTMHEAD	= 16, VERSION_WTM = 110 };

	CWtm()
		:	m_pData		 ( NULL ),
			m_pFile		 ( NULL ),
			m_dwFileSize ( 0 ),
			m_dwDataSize ( 0 ),
			m_dwOffset	 ( 0 )
	{
	} //CWtm

	~CWtm()
	{
		Close();
		SAFE_FREE( m_pData );
	} //~CWtm

	/**
	 * wtm 파일 열기
	 */
	DWORD			Open( const char* szFileName );

	/**
	 * 실제 데이터 얻기
	 */
	BYTE*			Data() const;

	/**
	 * 데이터 얻기
	 */
	DWORD			SGets( BYTE* szDest, DWORD dwSize );

	/**
	 * 실제 데이터 사이즈
	 */
	DWORD			Size() const;

	/**
	 * 디코딩 데이터.
	 */
	void			DecodeData( 
		BYTE* pDest, 
		const BYTE* pSrc, 
		const DWORD dwFileSize, 
		DWORD* pdwOriginSize 
		);

	/**
	 * 옵셋 설정
	 */
	void			SSeek( DWORD dwOffset );
	
	/**
	 * 파일 크기를 얻는다.
	 */
	DWORD			GetFileSize( FILE* pFile );

	/**
	 * 파일확인
	 */
	BOOL			IsWtmFile( BYTE* pData );

	/**
	 * 닫기
	 */
	void			Close();

private:
	FILE*			m_pFile;					///< 파일 핸들
	DWORD			m_dwFileSize;				///< 파일 전체사이즈
	DWORD			m_dwDataSize;				///< 데이터 사이즈
	DWORD			m_dwOffset;					///< 현제 데이터 위치
	BYTE*			m_pData;					///< 실제 데이터
}; //class CWtm

#endif //__Wind3D_Common_WTM_Header__
