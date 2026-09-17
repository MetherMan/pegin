/**
 *	$Workfile: $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 1.2 $
 *	$Date: 2005/03/10 14:00:41 $
 *     
 *	@Build Log :
 *	$Log: packet.h,v $
 *	Revision 1.2  2005/03/10 14:00:41  cvs
 *	*** empty log message ***
 *	
 *	Revision 1.1  2005/02/24 11:21:32  cvs
 *	처음 업데이트
 *	
 *        
 */
#ifndef __PACKET_H__
#define __PACKET_H__

//#define dENC_FIRST_HEADER		'A'
//#define dENC_SECOND_HEADER		'L'
#define dENC_FIRST_HEADER		'K'
#define dENC_SECOND_HEADER		'S'
////////////////////////////////////////////////////
// G L O B A L   V A R I A B L E S
///////////////////////////////////////////////////
extern char				g_Packet[dMAX_DATA_SOCK_BUFF];
extern int				g_nPos;

//
// Function Name: PutByte, GetByte 
// Date: 2004/04/23
// Description: BYTE 형의 패킷 처리
//
void PutByte( char *pPacket, BYTE value, int &nPos );
void PutByte( BYTE value );
BYTE GetByte( char *pPacket, int &nPos );


//
// Function Name: PutWord, GetWord
// Date: 2004/04/23
// Description: WORD 형의 패킷 처리
//
void PutWord( char *pPacket, WORD value, int &nPos );
void PutWord( WORD value );
WORD GetWord( char *pPacket, int &nPos );

//
// Function Name: PutDword, GetDword
// Date: 2004/04/23
// Description: DWORD 형의 패킷 처리
//
void PutDword( char *pPacket, DWORD value, int &nPos );
void PutDword( DWORD value );
DWORD GetDword( char *pPacket, int &nPos );

//
// Function Name: PutInteger, GetInteger
// Date: 2004/04/23
// Description: int 형의 패킷 처리 함수
//
void PutInteger( char *pPacket, int value, int &nPos );
void PutInteger( int value );
int GetInteger( char *pPacket, int &nPos );

//
// Function Name: PutShort, GetShort
// Date: 2004/04/23
// Description: short 형의 패킷 처리 함수
//
void PutShort( char *pPacket, short value, int &nPos );
void PutShort( short value );
short GetShort( char *pPacket, int &nPos );

//
// Function Name: PutFloat, GetFloat
// Date: 2004/04/23
// Description: short 형의 패킷 처리 함수
//
void PutFloat( char *pPacket, float value, int &nPos );
void PutFloat( float value );
float GetFloat( char *pPacket, int &nPos );

//
// Function Name: PutString, GetString
// Date: 2004/04/23
// Description: 문자열 데이터 패킷 처리 함수 ( 사이즈가 헤더로 붙는다 )
//
void PutString( char *pPacket, char *str, int &nPos );
void PutString( char *str );
void GetString( char *pPacket, char *pBuffer, int &nPos );

//
// Function Name: PutHeader
// Date: 2004/04/23
// Description: 패킷의 총 사이즈 처리 함수
//
void PutHeader( char *pPacket, WORD value, int &nPos );
void PutHeader( WORD value );

void PutSize( char *pPacket, WORD nPos );

#endif

//EOF



