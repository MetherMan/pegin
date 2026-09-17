/**
* TEA 암호화 알고리즘
*
* Copyright (c) 2002-2003 팀메이, All rights reserved.
*
* @author	sk8snow
* @date	$Date: 04-08-04 9:27a $
* @version	$Revision: 3 $
*
* <HR>
* $Log: /GameServer/encrypt.h $
* 
* 3     04-08-04 9:27a Sk8snow
* 
* 2     04-07-13 7:26a Sk8snow
* 
* 2     04-07-12 9:09p Sk8snow2
* 
*/
#ifndef __TEA_ENCRYPT_H__
#define __TEA_ENCRYPT_H__

// Encryption Packet Keys (This needs to match the client source)
#define dAMHO_KEY1			0x780412
#define dAMHO_KEY2			0x760626
#define dAMHO_KEY3			0x790310
#define dAMHO_KEY4			0x791212
void encipher( unsigned long *const v, unsigned long *const w, const unsigned long *const k );
void decipher( unsigned long *const v, unsigned long *const w, const unsigned long *const k );
unsigned long TEA_EncryptData( char *plainData, char *encData, unsigned long size , unsigned long *key );
unsigned long TEA_DecryptData( char *encData, char *plainData, unsigned long size, unsigned long *key );

#endif

//EOF



