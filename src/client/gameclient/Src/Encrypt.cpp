/**
 * TEA 암호화 알고리즘
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	sk8snow
 * @date	$Date: 04-07-12 9:38p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /GameClient/Encrypt.cpp $
 * 
 * 3     04-07-12 9:38p Sk8snow2
 * 
 * 2     04-07-12 9:09p Sk8snow2
 * 
 *
 * @file	Client.h
 */
#include <windows.h>
#include "Encrypt.h"

/////////////////////////////////////////////////////////////////////////////////////
// TEA Encrypt Function
// Description : 
// The Tiny Encryption Algorithm ( TEA )
//
// The Tiny Encryption Algorithm is one of the fastest and most efficient 
// cryptographic algorithms in existence. It was developed by David Wheeler 
// and Roger Needham at the Computer Laboratory of Cambridge University. 
// It is a Feistel cipher which uses operations from mixed (orthogonal) 
// algebraic groups - XOR, ADD and SHIFT in this case. 
// This is a very clever way of providing Shannon's twin properties of 
// diffusion and confusion which are necessary for a secure block cipher, 
// without the explicit need for P-boxes and S-boxes respectively. 
// It encrypts 64 data bits at a time using a 128-bit key. It seems highly 
// resistant to differential cryptanalysis, and achieves complete diffusion 
// (where a one bit difference in the plaintext will cause approximately 32 bit 
// differences in the ciphertext) after only six rounds. Performance on a 
// modern desktop computer or workstation is very impressive. 
//
// Original Function :
//
// void encipher(unsigned long *const v,unsigned long *const w,
//         const unsigned long *const k)
// {
//    register unsigned long       y=v[0],z=v[1],sum=0,delta=0x9E3779B9,
//				a=k[0],b=k[1],c=k[2],d=k[3],n=32;
//
//   while(n-->0)
//      {
//      sum += delta;
//      y += (z << 4)+a ^ z+sum ^ (z >> 5)+b;
//      z += (y << 4)+c ^ y+sum ^ (y >> 5)+d;
//      }
//
//   w[0]=y; w[1]=z;
// }
//
// void decipher(unsigned long *const v,unsigned long *const w,
//    const unsigned long *const k)
// {
//   register unsigned long       y=v[0],z=v[1],sum=0xC6EF3720,
//				delta=0x9E3779B9,a=k[0],b=k[1],
//				c=k[2],d=k[3],n=32;
//
//   /* sum = delta<<5, in general sum = delta * n */
//
//   while(n-->0)
//      {
//      z -= (y << 4)+c ^ y+sum ^ (y >> 5)+d;
//      y -= (z << 4)+a ^ z+sum ^ (z >> 5)+b;
//      sum -= delta;
//      }
//   
//   w[0]=y; w[1]=z;
// }
//
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

unsigned long  TEA_DecryptData( char *encData, char *plainData, unsigned long size, unsigned long *key )
{
	unsigned long dwPos = 0;

	for( unsigned long i = 0; i < size; i += 8 )
	{
		decipher( (unsigned long*)&encData[i], (unsigned long*)&plainData[dwPos], key );
		dwPos += 8;
	}
	return dwPos;
}