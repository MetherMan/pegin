/**
 * TEA 암호화 알고리즘
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	sk8snow
 * @date	$Date: 04-07-13 7:42a $
 * @version	$Revision: 5 $
 *
 * <HR>
 * $Log: /GameClient/Src/Encrypt.h $
 * 
 * 5     04-07-13 7:42a Sk8snow2
 * 
 * 4     04-07-12 9:41p Sk8snow2
 * 
 * 3     04-07-12 9:16p Sk8snow2
 * 
 * 2     04-07-12 9:09p Sk8snow2
 * 
 */
#ifndef __TEA_ENCRYPT_H__
#define __TEA_ENCRYPT_H__

// TEA Key 
//#define dAMHO_KEY1			0x102011
//#define dAMHO_KEY2			0x203122
//#define dAMHO_KEY3			0x239321
//#define dAMHO_KEY4			0x301212
#define dAMHO_KEY1			0x780412
#define dAMHO_KEY2			0x760626
#define dAMHO_KEY3			0x790310
#define dAMHO_KEY4			0x791212

#define dENC_FIRST_HEADER		'A'
#define dENC_SECOND_HEADER		'L'

void encipher( unsigned long *const v, unsigned long *const w, const unsigned long *const k );
void decipher( unsigned long *const v, unsigned long *const w, const unsigned long *const k );
unsigned long TEA_EncryptData( char *plainData, char *encData, unsigned long size , unsigned long *key );
unsigned long TEA_DecryptData( char *encData, char *plainData, unsigned long size, unsigned long *key );

#endif

//EOF

