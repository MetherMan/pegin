/**
 * Wind3D 아카이브 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-05-24 5:27a $
 * @version	$Revision: 8 $
 *
 * <HR>
 * $Log: /Common/Inc/Archive.h $
 * 
 * 8     03-05-24 5:27a Ahastudio
 * ArchiveFileReader에 GetSize() 추가.
 *
 * 5     03-03-14 8:26p Ahastudio		FileReader/Writer에 GetPosition() 추가.
 *
 * 4     03-03-13 4:17p Ahastudio		File Read/Write 사이즈 측정 부분 변경.
 *
 * 3     03-03-13 4:14p Ahastudio		File Handle 관련 부분을 Export에서 inline으로 변경.
 *
 * 2     03-03-13 3:54p Ahastudio		Archive 작성. ( FileReader 및 FileWriter )
 * <HR>
 *
 * @file	Archive.h
 */


#ifndef __Wind3D_Archive_Header__
#define __Wind3D_Archive_Header__


#include <stdio.h>


/**
 * 아카이브 클래스.
 * 읽고 쓰고 가비지 컬렉션 하는데 사용된다.
 * @ingroup	Common
 */
class COMMON_API Archive
{
public:
	UNDER_MEMORY_MGR( Archive );

	/**
	 * 소멸자.
	 */
	virtual		~Archive()
	{
	} //~Archive

	/**
	 * 직렬화.
	 */
	virtual void	Serialize( void* pData, int iLength )
	{
	} //Serialize
}; //class COMMON_API Archive


/**
 * 아카이브 파일 읽기 클래스.
 * @ingroup	Common
 */
class ArchiveFileReader : public Archive
{
public:
	UNDER_MEMORY_MGR( ArchiveFileReader );

	/**
	 * 생성자.
	 *
	 * @param	pFile		[IN] 파일 핸들.
	 */
	ArchiveFileReader( FILE* pFile )
	:	m_pFile	( pFile ),
		m_nPos	( 0 )
	{
		guard( ArchiveFileReader::ArchiveFileReader );
		
		Assert( m_pFile );
		fseek( m_pFile, 0, SEEK_SET );
		
		unguard;
	} //ArchiveFileReader

	/**
	 * 아카이브 핸들을 닫는다.
	 */
	void	Close()
	{
		guard( ArchiveFileReader::Close );

		Assert( m_pFile );

		fclose( m_pFile );
		m_pFile = NULL;

		unguard;
	} //Close

	/**
	 * 파일에서 데이터를 읽어온다.
	 *
	 * @param	pData		[OUT] 데이터 버퍼
	 * @param	iLength		[IN] 읽어올 데이터 길이
	 */
	void	Serialize( void* pData, int iLength )
	{
		guard( ArchiveFileReader::Serialize );

		Assert( m_pFile );
		Assert( pData );
		Assert( iLength > 0 );

		int		iRead = fread( pData, 1, iLength, m_pFile );
		if( iRead != iLength )
		{
			Assert( !"파일 읽기 실패" );
		} //if

		m_nPos += iRead;

		unguard;
	} //Serialize

	/**
	 * Serialize Position을 얻는다.
	 */
	int		GetPosition()
	{
		return m_nPos;
	} //GetPosition

	/**
	 * Serialize Position 변경.
	 */
	void	Seek( int nPos )
	{
		guard( ArchiveFileReader::Seek );

		Assert( m_pFile );
		Assert( nPos >= 0 );

		fseek( m_pFile, nPos, SEEK_SET );
		m_nPos = nPos;

		unguard;
	} //Seek

	/**
	 * 전체 Size를 얻는다.
	 */
	int		GetSize()
	{
		guard( ArchiveFileReader::Seek );

		Assert( m_pFile );

		int		nPos	= ftell( m_pFile );
		fseek( m_pFile, 0, SEEK_END );
		int		nSize	= ftell( m_pFile );
		fseek( m_pFile, nPos, SEEK_SET );

		return nSize;

		unguard;
	} //Seek

protected:
	FILE*		m_pFile;		///< 파일 핸들.
	int			m_nPos;			///< Serialize Position.
}; //class ArchiveFileReader


/**
 * 아카이브 파일 쓰기 클래스.
 * @ingroup	Common
 */
class ArchiveFileWriter : public Archive
{
public:
	UNDER_MEMORY_MGR( ArchiveFileWriter );

	/**
	 * 생성자.
	 *
	 * @param	pFile		[IN] 파일 핸들.
	 */
	ArchiveFileWriter( FILE* pFile )
	:	m_pFile	( pFile ),
		m_nPos	( 0 )
	{
		guard( ArchiveFileWriter::ArchiveFileWriter );

		Assert( m_pFile );

		unguard;
	} //ArchiveFileWriter

	/**
	 * 아카이브 핸들을 닫는다.
	 */
	void	Close()
	{
		guard( ArchiveFileWrite::Close );

		Assert( m_pFile );

		fclose( m_pFile );
		m_pFile = NULL;

		unguard;
	} //Close

	/**
	 * 파일에 데이터를 쓴다.
	 *
	 * @param	pData		[IN] 데이터 버퍼
	 * @param	iLength		[IN] 기록할 데이터 길이
	 */
	void	Serialize( void* pData, int iLength )
	{
		guard( ArchiveFileWriter::Serialize );

		Assert( m_pFile );
		Assert( pData );
		Assert( iLength > 0 );

		int		iWrite = fwrite( pData, 1, iLength, m_pFile );
		if( iWrite != iLength )
		{
			Assert( !"파일 쓰기 실패" );
		} //if

		m_nPos += iWrite;

		unguard;
	} //Serialize

	/**
	 * Serialize Position을 얻는다.
	 */
	int		GetPosition()
	{
		return m_nPos;
	} //GetPosition

protected:
	FILE*		m_pFile;		///< 파일 핸들.
	int			m_nPos;			///< Serialize Position.
}; //class ArchiveFileWriter


#endif // #ifndef __Wind3D_Archive_Header__
