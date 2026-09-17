/**
 *	$Workfile: packet.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 1.3 $
 *	$Date: 2005/04/06 22:58:44 $
 *     
 *	@Build Log :
 *	$Log: packet.cpp,v $
 *	Revision 1.3  2005/04/06 22:58:44  cvs
 *	*** empty log message ***
 *	
 *	Revision 1.2  2005/03/10 14:00:41  cvs
 *	*** empty log message ***
 *	
 *	Revision 1.1  2005/02/24 11:21:32  cvs
 *	처음 업데이트
 *	
 * 
 * 16    04-03-31 8:22a Sk8snow
 *        
 */

#include "def.h"
#include "structs.h"
#include "packet.h"

char			g_Packet[dMAX_DATA_SOCK_BUFF];
int				g_nPos;

//
// Function Name: PutByte, GetByte 
// Date: 2004/04/23
// Description: BYTE 형의 패킷 처리
//
void PutByte( char *pPacket, BYTE value, int &nPos )
{
	*(BYTE*)(pPacket + nPos) = value;
	nPos += sizeof(BYTE);
}

void PutByte( BYTE value )
{
	*(BYTE*)(g_Packet + g_nPos) = value;
	g_nPos += sizeof(BYTE);
}

BYTE GetByte( char *pPacket, int &nPos )
{
	BYTE value = *(BYTE*)(pPacket + nPos );
	nPos += sizeof(BYTE);
	return value;
}



//
// Function Name: PutWord, GetWord
// Date: 2004/04/23
// Description: WORD 형의 패킷 처리
//
void PutWord( char *pPacket, WORD value, int &nPos )
{
	*(WORD*)(pPacket + nPos ) = value;
	nPos += sizeof(WORD);
}

void PutWord( WORD value )
{
	*(WORD*)(g_Packet + g_nPos ) = value;
	g_nPos += sizeof(WORD);
}


WORD GetWord( char *pPacket, int &nPos )
{
	WORD value = *(WORD*)(pPacket + nPos );
	nPos += sizeof(WORD);
	return value;
}





//
// Function Name: PutDword, GetDword
// Date: 2004/04/23
// Description: DWORD 형의 패킷 처리
//
void PutDword( char *pPacket, DWORD value, int &nPos )
{
	*(DWORD*)(pPacket + nPos ) = value;
	nPos += sizeof(DWORD);
}

void PutDword( DWORD value )
{
	*(DWORD*)(g_Packet + g_nPos ) = value;
	g_nPos += sizeof(DWORD);
}


DWORD GetDword( char *pPacket, int &nPos )
{
	DWORD value = *(DWORD*)(pPacket + nPos );
	nPos += sizeof(DWORD);
	return value;
}




//
// Function Name: PutInteger, GetInteger
// Date: 2004/04/23
// Description: int 형의 패킷 처리 함수
//
void PutInteger( char *pPacket, int value, int &nPos )
{
	*(int*)(pPacket + nPos ) = value;
	nPos += sizeof(int);
}

void PutInteger( int value )
{
	*(int*)(g_Packet + g_nPos ) = value;
	g_nPos += sizeof(int);
}


int GetInteger( char *pPacket, int &nPos )
{
	int value = *(int*)(pPacket + nPos );
	nPos += sizeof(int);
	return value;
}




//
// Function Name: PutShort, GetShort
// Date: 2004/04/23
// Description: short 형의 패킷 처리 함수
//
void PutShort( char *pPacket, short value, int &nPos )
{
	*(short*)(pPacket + nPos ) = value;
	nPos += sizeof(short);
}

void PutShort( short value )
{
	*(short*)(g_Packet + g_nPos ) = value;
	g_nPos += sizeof(short);
}


short GetShort( char *pPacket, int &nPos )
{
	short value = *(short*)(pPacket + nPos );
	nPos += sizeof(short);
	return value;
}



//
// Function Name: PutFloat, GetFloat
// Date: 2004/04/23
// Description: short 형의 패킷 처리 함수
//
void PutFloat( char *pPacket, float value, int &nPos )
{
	*(float*)(pPacket + nPos) = value;
	nPos += sizeof(float);
}

void PutFloat( float value )
{
	*(float*)(g_Packet + g_nPos) = value;
	g_nPos += sizeof(float);
}

float GetFloat( char *pPacket, int &nPos )
{
	float value = *(float*)(pPacket + nPos );
	nPos += sizeof( float );
	return value;
}





//
// Function Name: PutString, GetString
// Date: 2004/04/23
// Description: 문자열 데이터 패킷 처리 함수 ( 사이즈가 헤더로 붙는다 )
//
void PutString( char *pPacket, char *str, int &nPos )
{
	*(WORD*)(pPacket + nPos ) = strlen( str );
	nPos += sizeof(WORD);
	memcpy( pPacket + nPos, str, strlen( str ) );
	nPos += strlen(str); 
}

void PutString( char *str )
{
	*(WORD*)(g_Packet + g_nPos ) = strlen( str );
	g_nPos += sizeof(WORD);
	memcpy( g_Packet + g_nPos, str, strlen( str ) );
	g_nPos += strlen(str); 
}

void GetString( char *pPacket, char *pBuffer, int &nPos )
{
	WORD BufferLength;

	BufferLength = *(WORD*)(pPacket + nPos );
	nPos += sizeof(WORD);
	memcpy( pBuffer, pPacket + nPos, BufferLength);
	*( pBuffer + BufferLength ) = NULL_STR;
	nPos += BufferLength;
}




//
// Function Name: PutSize
// Date: 2004/04/23
// Description: 패킷의 총 사이즈 처리 함수
//
void PutHeader( char *pPacket, WORD value, int &nPos )
{
	nPos = 0;
	
	pPacket[0] = dENC_FIRST_HEADER;
	pPacket[1] = dENC_SECOND_HEADER;
	
	nPos += 2;
	
	*(WORD*)(pPacket + nPos ) = value;
	nPos += sizeof(WORD);
}

void PutHeader( WORD value )
{
	g_nPos = 0;
	
	g_Packet[0] = dENC_FIRST_HEADER;
	g_Packet[1] = dENC_SECOND_HEADER;
	
	g_nPos += 2;
	
	*(WORD*)( g_Packet + g_nPos ) = value;
	g_nPos += sizeof(WORD);
}

void PutSize( char *pPacket, WORD nPos )
{
	*(WORD*)pPacket = nPos;
}


//EOF



