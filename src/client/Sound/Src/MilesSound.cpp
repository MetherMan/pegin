/**
 * Miles Sound 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-21 2:32a $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /Sound/Src/MilesSound.cpp $
 * 
 * 9     03-12-21 2:32a Sckyj
 * 수정중
 * 
 * 8     03-12-20 11:52a Sckyj
 * Memory Leak 수정
 * 
 * 7     03-11-28 10:16a Sckyj
 * Map, Area 변경에 따른 특징적인 잔향( Reverb type )을 설정할수 있도록
 * 기능추가
 * 
 * 6     03-11-26 3:51p Sckyj
 * Ambient BGM 시간대별 재생기능 추가
 * 
 * 5     03-09-18 10:41a Sckyj
 * 
 * 4     03-09-17 10:39a Sckyj
 * 
 * 3     03-09-06 2:13p Sckyj
 * 
 * 2     03-07-26 11:02a Kjmgo
 * 
 * 1     03-06-01 3:16p Kjmgo
 * 
 * <HR>
 *
 * @file	MilesSound.cpp
 */

#include "Sound_Export.h"
#include "MilesSound.h"

#define MAX_PROVIDERS 64

/**
 * 생성자
 */
CMilesSound::CMilesSound()
{
	m_hDigital		= NULL;	
	m_hProvider		= NULL;
	m_hListener		= NULL;
} //CMilesSound::CMilesSound

/**
 * 소멸자
 */
CMilesSound::~CMilesSound()
{
	Close();
} //CMilesSound::~CMilesSound

/**
 * 초기화 
 */
BOOL CMilesSound::Init()
{ 
	// Initialize the Miles Sound System

	AIL_set_redist_directory( "Miles" );

	// Startup

	AIL_startup();

	// Initialize digital driver

	m_hDigital = AIL_open_digital_driver(	DIGITALRATE,
											DIGITALBITS, 
											DIGITALCHANNELS, 
											0 
										);	

	// Open a 3D digital provider

	char*		szName;
	HPROVIDER	hProvider;

	int			nCount = 0;
	HPROENUM	eFirst = HPROENUM_FIRST;


	// Miles Fast 2D Positional Audio를 사용한다.
	while(AIL_enumerate_3D_providers(	&eFirst,
										&hProvider,
										&szName )
										)
	{ 
		if( !strcmp( szName, "Miles Fast 2D Positional Audio" ) )
	//	if( !strcmp( szName, "Dolby Surround" ) )
		{	
			m_hProvider = hProvider;  
			break;
		} //if 
		nCount++;
	} //while

	if( !m_hProvider )
	{
		Assert( !"사운드 생성 실패! - #1" );
		return FALSE;
	} //if

	// Open 3D Provider
    int nResult = AIL_open_3D_provider( m_hProvider );	

	// Provider를 성공적으로 열었는지를 확인한다.
	if( nResult != M3D_NOERR )
	{ // 웃기게도 오류가 있는지가 아니라 오류가 없는지를 검사한다.
		Assert( !"사운드 생성 실패! - #2" );
		return FALSE;
	} //if

	m_hListener = AIL_open_3D_listener( m_hProvider );
		 
	return TRUE;
} //CMilesSound::Init

/**
 * 해제
 */
void CMilesSound::Close()
{
   // Close the digital driver
   if( m_hDigital )
   {
     AIL_close_digital_driver( m_hDigital );
     m_hDigital = NULL;
   } //if

   // Close the 3D Provider
   if( m_hProvider )
   {
      AIL_close_3D_provider( m_hProvider );
      m_hProvider = NULL;
   } //if

   // Now shutdown Miles completely
   AIL_shutdown();
	
} //CMilesSound::Close

/**
 * Listner의 위치를 지정한다.
 */
void CMilesSound::SetPosition( const Vector3& vPos, const Vector3& vDir )
{
   AIL_set_3D_position(	m_hListener, vPos.x, vPos.y, vPos.z );

   AIL_set_3D_orientation( m_hListener,
                          vDir.x, vDir.y, vDir.z,	// Direction
                          0.0F, 0.0F, 1.0F	// Up Vector
						  );

} //CMilesSound::SetPosition

/**
 * Sound 파일을 읽어 온다.
 */
void* LoadSound( const char* szFile )
{
	void* s;
	void* d;
	S32 type;
	AILSOUNDINFO info;
	S32 size;

	size = AIL_file_size( szFile );
	s = AIL_file_read( szFile, 0 );

	if( s == NULL )
		return NULL;

	type = AIL_file_type( s, size );

	switch( type )
	{
	case AILFILETYPE_PCM_WAV:
		return s;

	case AILFILETYPE_ADPCM_WAV:
		AIL_WAV_info( s, &info );
		AIL_decompress_ADPCM( &info, &d, 0 );
		AIL_mem_free_lock( s );
		return d;

	case AILFILETYPE_MPEG_L3_AUDIO:
		AIL_decompress_ASI( s, size, szFile, &d, 0, 0 );
		AIL_mem_free_lock( s );
		return d;

	default:
		AIL_mem_free_lock( s );
		return NULL;
	}
} //LoadSound
