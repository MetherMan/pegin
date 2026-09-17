/**
 * 패킷 클래스.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	TEAMMAY
 * @date	$Date: 04-07-16 6:04p $
 * @version	$Revision: 38 $
 *
 * <HR>
 * $Log: /GameClient/Src/Packet.h $
 * 
 * 38    04-07-16 6:04p Sk8snow2
 * 
 * 37    04-07-16 3:05p Sk8snow2
 * 
 * 36    04-07-14 8:51a Sk8snow2
 * 
 * 35    04-07-12 9:48p Sk8snow2
 * 
 * 34    04-07-12 9:41p Sk8snow2
 * 
 * 33    04-07-12 9:16p Sk8snow2
 * 
 * 32    04-05-22 11:44p Sk8snow2
 * 
 * 31    04-05-22 10:52p Sk8snow2
 * 
 * 30    04-03-23 10:16a Sk8snow2
 * 
 * 28    04-01-07 8:07a Kjmgo
 * 
 * 27    04-01-06 12:01a Kjmgo
 * 공속 추가 
 * 
 * 26    03-08-22 3:52p Sk8snow
 * 
 * 25    03-06-08 1:43a Sk8snow
 * 
 * 24    03-06-08 1:29a Sk8snow
 * 
 * 23    03-06-07 11:16p Sk8snow
 * 
 * 22    03-06-07 11:04p Sk8snow
 * 
 * 21    03-05-01 10:48a Ahastudio
 * Usage Example 추가.
 * <HR>
 *
 * @file	Packet.h
 */


#ifndef __Packet_Header__
#define __Packet_Header__

#ifdef _CHINAVERSION
#define dUSE_ENCRYPT_PACKET				// 암호화된 패킷을 사용할 경우
#endif

#include <string>


using std::string;


/**
 * 패킷 클래스.
 */
class CPacket
{
public:
	UNDER_MEMORY_MGR( CPacket );

	/**
	 * 생성자.
	 */
	CPacket()
	:	m_pwSize( reinterpret_cast<WORD*>( m_Buffer ) )
	{
	} //CPacket

	/**
	 * TAG를 설정한다.
	 * Packet에 데이터를 넣기 전에 반드시 사용해야 하는 함수.
	 *
	 * @param	wTag	[IN] TAG
	 */
	CPacket&	Tag( WORD wTag )
	{
		*reinterpret_cast<WORD*>( m_Buffer + 2 ) = wTag;
		
		*m_pwSize = 4;

		return *this;
	} //Tag

	/**
	 * TAG를 얻는다.
	 *
	 * @return	TAG
	 */
	WORD	GetTag()
	{
		Assert( *m_pwSize >= 4 );

		return *reinterpret_cast<WORD*>( m_Buffer + 2 );
	} //GetTag

	/**
	 * 버퍼를 얻는다.
	 *
	 * @return	버퍼
	 */
	char*	GetBuffer()
	{
		return m_Buffer;
	} //GetBuffer

	/**
	 * 패킷의 길이를 얻어온다.
	 *
	 * @return	패킷의 길이
	 */
	int		GetSize()
	{
		return *m_pwSize;
	} //GetSize

	/**
	 * 패킷을 설정한다.
	 * 네트웍 클라이언트가 서버에서 받아온 데이터를
	 * 패킷 객체에 넣을 때 사용한다.
	 *
	 * @param	nSize		[IN] (Size를 포함한) 패킷 길이
	 * @param	pBuffer		[IN] (Size를 제외한) 패킷 데이터
	 */
	void	SetPacket( int nSize, char* pBuffer )
	{
		Assert( nSize >= 4 );
		Assert( nSize <= BUFFER_SIZE );

		*m_pwSize = nSize;
		memcpy( m_Buffer + 2, pBuffer, nSize - 2 );
		m_nReadPos = 4;
	} //SetPacket

	/**
	 * BYTE 데이터를 추가한다.
	 *
	 * @param	data	[IN] 데이터
	 *
	 * @return	패킷 데이터
	 */
	CPacket&	operator << ( BYTE data )
	{
		Assert( *m_pwSize >= 4 );
		Assert( *m_pwSize + sizeof( BYTE ) <= BUFFER_SIZE );

		*reinterpret_cast<BYTE*>( m_Buffer + *m_pwSize ) = data;
		*m_pwSize += sizeof( BYTE );

		return *this;
	} //operator <<

	/**
	 * WORD 데이터를 추가한다.
	 *
	 * @param	data	[IN] 데이터
	 *
	 * @return	패킷 데이터
	 */
	CPacket&	operator << ( WORD data )
	{
		Assert( *m_pwSize >= 4 );
		Assert( *m_pwSize + sizeof( WORD ) <= BUFFER_SIZE );

		*reinterpret_cast<WORD*>( m_Buffer + *m_pwSize ) = data;
		*m_pwSize += sizeof( WORD );

		return *this;
	} //operator <<

	/**
	 * DWORD 데이터를 추가한다.
	 *
	 * @param	data	[IN] 데이터
	 *
	 * @return	패킷 데이터
	 */
	CPacket&	operator << ( DWORD data )
	{
		Assert( *m_pwSize >= 4 );
		Assert( *m_pwSize + sizeof( DWORD ) <= BUFFER_SIZE );

		*reinterpret_cast<DWORD*>( m_Buffer + *m_pwSize ) = data;
		*m_pwSize += sizeof( DWORD );

		return *this;
	} //operator <<

	/**
	 * 문자열을 추가한다.
	 *
	 * @param	szText		[IN] 문자열
	 *
	 * @return	패킷 데이터
	 */
	CPacket&	operator << ( const char* szText )
	{
		int nLen = strlen( szText );

		Assert( *m_pwSize >= 4 );
		Assert( *m_pwSize + sizeof( WORD ) + nLen <= BUFFER_SIZE );

		*reinterpret_cast<WORD*>( m_Buffer + *m_pwSize ) = nLen;
		*m_pwSize += sizeof( WORD );

		memcpy( m_Buffer + *m_pwSize, szText, nLen );
		*m_pwSize += nLen;

		return *this;
	} //operator <<

	/**
	 * 문자열을 추가한다.
	 *
	 * @param	sText		[IN] 문자열
	 *
	 * @return	패킷 데이터
	 */
	CPacket&	operator << ( const string sText )
	{
		return operator << ( sText.c_str() );
	} //operator <<

	/**
	 * 원하는 크기의 데이터를 추가한다.
	 *
	 * @param	pData	[IN] 넣을 데이터
	 * @param	nSize	[IN] 넣을 데이터 크기
	 *
	 * @return	패킷 데이터
	 */
	CPacket&	Insert( const void* pData, int nSize )
	{
		Assert( *m_pwSize >= 4 );
		Assert( *m_pwSize + nSize <= BUFFER_SIZE );

		memcpy( m_Buffer + *m_pwSize, pData, nSize );
		*m_pwSize += nSize;

		return *this;
	} //Insert

	/**
	 * BYTE 데이터를 얻는다.
	 *
	 * @param	data	[OUT] 데이터
	 *
	 * @return	패킷 데이터
	 */
	CPacket&	operator >> ( BYTE& data )
	{
		Assert( m_nReadPos >= 4 );
		Assert( m_nReadPos + sizeof( BYTE ) <= *m_pwSize );

		data = *reinterpret_cast<BYTE*>( m_Buffer + m_nReadPos );
		m_nReadPos += sizeof( BYTE );

		return *this;
	} //operator >>

	/**
	 * WORD 데이터를 얻는다.
	 *
	 * @param	data	[OUT] 데이터
	 *
	 * @return	패킷 데이터
	 */
	CPacket&	operator >> ( WORD& data )
	{
		Assert( m_nReadPos >= 4 );
		Assert( m_nReadPos + sizeof( WORD ) <= *m_pwSize );

		data = *reinterpret_cast<WORD*>( m_Buffer + m_nReadPos );
		m_nReadPos += sizeof( WORD );

		return *this;
	} //operator >>

	/**
	 * DWORD 데이터를 얻는다.
	 *
	 * @param	data	[OUT] 데이터
	 *
	 * @return	패킷 데이터
	 */
	CPacket&	operator >> ( DWORD& data )
	{
		Assert( m_nReadPos >= 4 );
		Assert( m_nReadPos + sizeof( DWORD ) <= *m_pwSize );

		data = *reinterpret_cast<DWORD*>( m_Buffer + m_nReadPos );
		m_nReadPos += sizeof( DWORD );

		return *this;
	} //operator >>

	/**
	 * 문자열 데이터를 얻는다.
	 *
	 * @param	szText		[OUT] 문자열 데이터를 받을 공간
	 *
	 * @return	패킷 데이터
	 */
	CPacket&	operator >> ( char* szText )
	{
		Assert( m_nReadPos >= 4 );
		Assert( m_nReadPos + sizeof( WORD ) <= *m_pwSize );

		int		iLen = *reinterpret_cast<WORD*>( m_Buffer + m_nReadPos );
		m_nReadPos += sizeof( WORD );

		Assert( iLen >= 0 );
		Assert( m_nReadPos + iLen <= *m_pwSize );

		memcpy( szText, m_Buffer + m_nReadPos, iLen );
		szText[iLen] = 0;
		m_nReadPos += iLen;

		return *this;
	} //operator >>

	/**
	 * 문자열 데이터를 얻는다.
	 *
	 * @param	sText		[OUT] 문자열 데이터를 받을 공간
	 *
	 * @return	패킷 데이터
	 */
	CPacket&	operator >> ( string& sText )
	{
		Assert( m_nReadPos >= 4 );
		Assert( m_nReadPos + sizeof( WORD ) <= *m_pwSize );

		int		iLen = *reinterpret_cast<WORD*>( m_Buffer + m_nReadPos );
		m_nReadPos += sizeof( WORD );

		Assert( iLen >= 0 );
		Assert( m_nReadPos + iLen <= *m_pwSize );

		char	szText[256];
		memcpy( szText, m_Buffer + m_nReadPos, iLen );
		szText[iLen] = 0;
		sText = szText;

//		sText.copy( m_Buffer + m_nReadPos, iLen );
		m_nReadPos += iLen;

		return *this;
	} //operator >>

	/**
	 * 데이터를 얻는다.
	 *
	 * @param	pData	[OUT] 데이터를 받을 공간
	 * @param	nSize	[IN] 받을 데이터 크기
	 *
	 * @return	패킷 데이터
	 */
	CPacket&	Get( void* pData, int nSize )
	{
		Assert( m_nReadPos >= 4 );
		Assert( m_nReadPos + nSize <= *m_pwSize );

		memcpy( pData, m_Buffer + m_nReadPos, nSize );
		m_nReadPos += nSize;

		return *this;
	} //Get

	inline CPacket&		operator << ( char  data )	{ return operator << ( (BYTE)data ); }
	inline CPacket&		operator << ( short data )	{ return operator << ( (WORD)data ); }
	inline CPacket&		operator << ( long  data )	{ return operator << ( (DWORD)data ); }
	inline CPacket&		operator << ( int   data )	{ return operator << ( (DWORD)data ); }

	inline CPacket&		operator >> ( char&  data )	{ return operator >> ( (BYTE&)data ); }
	inline CPacket&		operator >> ( short& data )	{ return operator >> ( (WORD&)data ); }
	inline CPacket&		operator >> ( long&  data )	{ return operator >> ( (DWORD&)data ); }
	inline CPacket&		operator >> ( int&   data )	{ return operator >> ( (DWORD&)data ); }
	
	enum 
	{
		BUFFER_SIZE = 4096				///< Receive 버퍼 크기
	}; //enum

private:
	char	m_Buffer[BUFFER_SIZE];		///< 데이터 버퍼
	WORD*	m_pwSize;					///< 패킷 크기
	int		m_nReadPos;					///< 데이터 읽기용 포인터

}; //class CPacket



#endif // #ifndef __Packet_Header__


/* Usage Example:

Receive에 사용할 경우

	CPacket		recvPacket;

	char*		pData = GetPacketData();
	int			nSize = *reinterpret_cast<WORD*>( pData );

	recvPacket.SetPacket( nSize, pData + 2 );	// 패킷 크기를 제외한 순수 데이터로 세팅한다는 점에 주의한다.

	switch( m_recvPacket.Tag() )
	{
	case ...:
		{
			DWORD	dwId;
			int		nX;
			int		nY;

			recvPacket
				>> dwId
				>> nX
				>> nY;
		}
		break;
	} //switch

Send에 사용할 경우

	CPacket		sendPacket;

	DWORD	dwId	= 0;
	int		nX		= 0;
	int		nY		= 0;

	sendPacket.Tag( ... )
		<< dwId
		<< nX
		<< nY;

	Send( hSocket, sendPacket.GetBuffer(), sendPacket.GetSize() );

*/
