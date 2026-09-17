/**
 * TEA 암호화 알고리즘
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	sk8snow
 * @date	$Date: 04-07-13 7:26a $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /GameServer/encrypt.cpp $
 * 
 * 2     04-07-13 7:26a Sk8snow
 * 
 * 2     04-07-12 9:09p Sk8snow2
 * 
 *
 * @file	Client.h
 */
#ifdef WIN32
#include <windows.h>
#else
#include "def.h"
#endif

#include "encrypt.h"

/////////////////////////////////////////////////////////////////////////////////////
// TEA Encrypt Function
// Description : 
// The Tiny Encryption Algorithm ( TEA )
////////////////////////////////////////////////////////////////////////////////////

// Encryption ( 8 Byte )
void encipher( unsigned long *const v, unsigned long *const w, const unsigned long *const k )
{
	register unsigned long y = v[0], z = v[1], sum = 0, delta = 0x9E3779B9, a = k[0], b = k[1], c = k[2], d = k[3], n = 8;
	
	while(n-->0)
	{
		sum += delta;
		y += (z << 4)+a ^ z+sum ^ (z >> 5)+b;
		z += (y << 4)+c ^ y+sum ^ (y >> 5)+d;
	}
	
	w[0] = y; 
	w[1] = z;
}

// Decryption
void decipher( unsigned long *const v, unsigned long *const w, const unsigned long *const k )
{
	register unsigned long y = v[0], z = v[1], sum = 0, delta = 0x9E3779B9, a = k[0], b = k[1], c = k[2], d = k[3], n = 8;
	
	//sum = delta<<5;//, in general sum = delta * n */
	
	sum = delta * n;
	
	while(n-->0)
	{
		z -= (y << 4)+c ^ y+sum ^ (y >> 5)+d;
		y -= (z << 4)+a ^ z+sum ^ (z >> 5)+b;
		sum -= delta;
	}
	
	w[0] = y;
	w[1] = z;
}

unsigned long TEA_EncryptData( char *plainData, char *encData, unsigned long size , unsigned long *key )
{
	unsigned long dwPos = 0;

	for( unsigned long i = 0; i < size; i += 8 )
	{
		encipher( (unsigned long*)&plainData[i], (unsigned long*)&encData[dwPos], key );
		dwPos += 8;
	}

	return dwPos;
}

unsigned long TEA_DecryptData( char *encData, char *plainData, unsigned long size, unsigned long *key )
{
	unsigned long dwPos = 0;

	for( unsigned long i = 0; i < size; i += 8 )
	{
		decipher( (unsigned long*)&encData[i], (unsigned long*)&plainData[dwPos], key );
		dwPos += 8;
	}

	return dwPos;
}



//EOF



