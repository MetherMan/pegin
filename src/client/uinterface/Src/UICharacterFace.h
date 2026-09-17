/**
 * 인터페이스 얼굴 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-09-17 5:08p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /UInterface/Src/UICharacterFace.h $
 * 
 * 2     03-09-17 5:08p Paran
 * 
 * 1     03-05-17 12:00a Paran
 * 
 * 7     03-04-11 4:44a Ahastudio
 * <HR>
 *
 * @file	UICharacterFace.h
 */


#ifndef __UICharacterFace_Header__
#define __UICharacterFace_Header__


#include <string>


using std::string;


/**
 * 인터페이스 얼굴 클래스.
 */
class CUICharacterFace
{
public:
	UNDER_MEMORY_MGR( CUICharacterFace );

	enum
	{
		MAN_FACE = 0,
		GIRL_FACE = 1,
		NUM_FACE = 2
	}; //enum

	CUICharacterFace();
	~CUICharacterFace();

	BOOL			Create( const int nHead, const int nHair, const int nTorso, const int nSex );

	IW3DSprite*		GetImageFace();
	IW3DSprite*		GetImageHair();
	IW3DSprite*		GetImageTorso();
	IW3DSprite*		GetImageDeath();

private:
	IW3DSprite*		m_psprHead;		///<
	IW3DSprite*		m_psprHair;		///<
	IW3DSprite*		m_psprTorso;	///<
	
	IW3DSprite*		m_psprDeath;	///<

	int				m_nWidth;		///<
	int				m_nHeight;		///<

	void			Cleanup();
	BOOL			Load( 
		const char* szPath, 
		const char* szHead, 
		const char* szHair, 
		const char* szTorso 
		);
}; //class CUICharacterFace


#endif //#ifndef __UICharacterFace_Header__

