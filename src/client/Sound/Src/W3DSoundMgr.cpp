/**
 * 사운드 관리자.
 * CW3DSoundObject를 사용하기 위해 사용되며,
 * 청취자 관련 세팅을 위해서도 사용된다.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-04-17 4:31p $
 * @version	$Revision: 18 $
 *
 * <HR>
 * $Log: /Sound/Src/W3DSoundMgr.cpp $
 * 
 * 18    04-04-17 4:31p Servantes
 * 
 * 17    03-12-17 8:03p Sckyj
 * m_bMultiSpeaker 초기값 false로 설정
 * 
 * 16    03-12-15 5:31p Sckyj
 * MultiSpeaker관련 코드 추가
 * 
 * 15    03-11-28 10:16a Sckyj
 * Map, Area 변경에 따른 특징적인 잔향( Reverb type )을 설정할수 있도록
 * 기능추가
 * 
 * 14    03-10-10 11:39a Kjmgo
 * 소스 정리
 * 
 * 13    03-10-08 4:05p Sckyj
 * 
 * 12    03-09-17 10:39a Sckyj
 * 
 * 11    03-06-01 3:14p Kjmgo
 * 
 * 10    03-03-25 7:15a Ahastudio
 * <HR>
 *
 * @file	W3DSoundMgr.cpp
 */


#include "Sound_Export.h"

CMilesSound* g_pMilesSound = NULL;

/**
 * 사운드 관리자.
 * @ingroup		Sound
 */
class CW3DSoundMgr : public IW3DSoundMgr
{
	SINGLETON_COMPONENT( CW3DSoundMgr );

public:
	UNDER_MEMORY_MGR( W3DSoundMgr );

	CW3DSoundMgr();
	~CW3DSoundMgr();

	BOOL	Init();
	void	SetVolume( float fVolume );
	float	GetVolume();

	void	SetPosition( const Vector3& vPos );
	void	SetPosition( const Vector3& vPos, const Vector3& vDir );
	Vector3	GetPosition();
	
	void	SetReverb( const char* szReverb );
	BYTE	GetReverb();
	
	BOOL	GetOnOff();
	void	SetOnOff( BOOL bOn );
	
	void	SetMultiSpeaker( BOOL bUse ) { m_bMultiSpeaker = bUse; }	
	BOOL	GetMultiSpeaker()			 { return m_bMultiSpeaker; }

private:
	Vector3	m_vPos;
	Vector3 m_vDir;

	float	m_fVolume;

	BOOL	m_bOn;
	BOOL	m_bMultiSpeaker;

	BYTE	m_btReverbType;
}; //class CW3DSoundMgr


IMPLEMENT_SINGLETON_COMPONENT( CW3DSoundMgr, SoundMgr );


/**
 * 생성자.
 */
CW3DSoundMgr::CW3DSoundMgr()
:	m_fVolume		( 1.0f ),
	m_btReverbType	( 0 ),
	m_bOn			( TRUE ),
	m_bMultiSpeaker ( FALSE )
{
	m_vPos = Vector3( 0.0f, 0.0f, 0.0f );
	m_vDir = Vector3( 1.0f, 1.0f, 0.0f );
} //CW3DSoundMgr::CW3DSoundMgr

/**
 * 소멸자.
 *
 * @author	아샬
 * @date	2003-02-06 오후 10:58
 */
CW3DSoundMgr::~CW3DSoundMgr()
{
	if( g_pMilesSound )
	{
		g_pMilesSound->Close();
	} //if

	SAFE_DELETE( g_pMilesSound );
} //CW3DSoundMgr::~CW3DSoundMgr

/**
 * 초기화.
 *
 * @param		hWnd	[IN] Window Handle
 *
 * @return		초기화 성공 여부.
 */
BOOL CW3DSoundMgr::Init()
{
	g_pMilesSound = new CMilesSound;
	CHECK_PTR_RETURN( g_pMilesSound );

	return g_pMilesSound->Init();
} //CW3DSoundMgr::Init

/*
 * Sound Object 들의 볼륨을 설정한다.
 */
void CW3DSoundMgr::SetVolume( float fVolume )
{
	if( fVolume > 1.0f )		m_fVolume = 1.0f;
	else if( fVolume < 0.0f )	m_fVolume = 0.0f;
	else						m_fVolume = fVolume;
} // CW3DSoundMgr::SetVolume

/*
 * Sound Object 들의 볼륨을 가져온다
 */
float CW3DSoundMgr::GetVolume()
{
	return m_fVolume;
} // CW3DSoundMgr::SetVolume

/**
 * Map, Area의 script에서 설정한 잔향 type을 설정한다.
 */
void CW3DSoundMgr::SetReverb( const char* szReverb )
{
	if( !strcmp( szReverb, "NULL" ) )
	{
		m_btReverbType = REVERB_GENERIC;
	}
	else
	if( !strcmp( szReverb, "ROOM" ) )
	{
		m_btReverbType = REVERB_ROOM;
	}
	else
	if( !strcmp( szReverb, "BATHROOM" ) )
	{
		m_btReverbType = REVERB_BATHROOM;
	}
	else
	if( !strcmp( szReverb, "CONCERTHALL" ) )
	{
		m_btReverbType = REVERB_CONCERTHALL;
	}
	else
	if( !strcmp( szReverb, "CAVE" ) )
	{
		m_btReverbType = REVERB_CAVE;
	}
	else
	if( !strcmp( szReverb, "CITY" ) )
	{
		m_btReverbType = REVERB_CITY;
	}
	else
	if( !strcmp( szReverb, "MOUNTAINS" ) )
	{
		m_btReverbType = REVERB_MOUNTAINS;
	}
	else
	if( !strcmp( szReverb, "UNDERWATER" ) )
	{
		m_btReverbType = REVERB_UNDERWATER;
	}
	else
	{
		m_btReverbType = REVERB_GENERIC;
	}	
} // CW3DSoundMgr::SetReverb

/**
 * Map, Area등에서 설정한 잔향 type을 반환한다.
 */
BYTE CW3DSoundMgr::GetReverb()
{
	return m_btReverbType;
} // CW3DSoundMgr::GetReverb()

/**
 * Listener의 위치를 정한다.
 *
 * @param	vPos	[IN] Listener의 위치
 */
void CW3DSoundMgr::SetPosition( const Vector3& vPos )
{
	Assert( g_pMilesSound );

	m_vPos = vPos;

	if( g_pMilesSound )
	{
		g_pMilesSound->SetPosition( m_vPos, m_vDir );
	} //if
} //CW3DSoundMgr::SetPosition

/**
 * Listener의 위치와 방향을 정한다.
 * @param	vDir	[IN] Listenerd의 방향
 */
void CW3DSoundMgr::SetPosition( const Vector3& vPos, const Vector3& vDir )
{
	Assert( g_pMilesSound );

	m_vPos = vPos;
	m_vDir = vDir;

	
	if( g_pMilesSound )
	{
		g_pMilesSound->SetPosition( m_vPos, m_vDir );
	} //if
} //CW3DSoundMgr::SetDirection

/**
 * Listener의 위치를 얻는다.
 */
Vector3 CW3DSoundMgr::GetPosition()
{
	return m_vPos;
} //CW3DSoundMgr::SetPosition


/**
 * Effect Sound의 On, Off여부를 설정한다.
 */
void CW3DSoundMgr::SetOnOff( BOOL bOn )
{
	m_bOn = bOn;
} // CW3DSoundMgr::SetOnOff


/**
 * Effect Sound의 On, Off여부를 반환한다.
 */
BOOL CW3DSoundMgr::GetOnOff()
{
	return m_bOn;
} // CW3DSoundMgr::GetOnOff
