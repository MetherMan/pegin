/**
 * Miles Sound 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-21 2:32a $
 * @version	$Revision: 5 $
 *
 * <HR>
 * $Log: /Sound/Src/MilesSound.h $
 * 
 * 5     03-12-21 2:32a Sckyj
 * 수정중
 * 
 * 4     03-11-28 10:16a Sckyj
 * Map, Area 변경에 따른 특징적인 잔향( Reverb type )을 설정할수 있도록
 * 기능추가
 * 
 * 3     03-11-26 3:51p Sckyj
 * Ambient BGM 시간대별 재생기능 추가
 * 
 * 2     03-09-06 2:57p Sckyj
 * 
 * 1     03-06-01 3:16p Kjmgo
 * 
 * <HR>
 *
 * @file	MilesSound.h
 */

#ifndef __Sound_Miles_Header__
#define __Sound_Miles_Header__

#include <mss.h>

#define DIGITALRATE       44100
#define DIGITALBITS       16
#define DIGITALCHANNELS   2

/**
 *
 */
class CMilesSound
{
public:
	UNDER_MEMORY_MGR( CMilesSound );

	CMilesSound();
	~CMilesSound();

	BOOL Init();
	void Close();

	HDIGDRIVER GetDriver()
	{
		return m_hDigital;
	} //GetDriver

	HPROVIDER	GetProvider()
	{
		return m_hProvider;
	} //GetProvider	

	void SetPosition( const Vector3& vPos, const Vector3& vDir );

private:
	// Wav, Voc,
	HDIGDRIVER	m_hDigital;		///< A handle to an Digital audio device
	HPROVIDER	m_hProvider;	///< 3D Sound 장치..
	H3DPOBJECT  m_hListener;	///< A handle to an Listener	
}; //class CMilesSound

void* LoadSound( const char* szFile );


#endif //__Sound_Miles_Header__