/**
 * 사운드 오브젝트.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-05-06 9:43a $
 * @version	$Revision: 57 $
 *
 * <HR>
 * $Log: /Sound/Src/W3DSoundObject.cpp $
 * 
 * 57    04-05-06 9:43a Sckyj
 * 
 * 56    04-03-08 5:53p Sckyj
 * Min Distance 값 변경 MaxDis/36
 * 
 * 55    04-03-06 11:27a Sckyj
 * 
 * 54    03-12-23 4:44a Sckyj
 * 
 * 53    03-12-23 3:32a Sckyj
 * 음원재생시 동일한 파일일 경우 새로 Load하지 않고 그대로 재생
 * 
 * 52    02-12-12 4:15p Sckyj
 * 2D, 3D를 분리
 * 
 * 51    02-12-12 3:41p Sckyj
 * 2D, 3D를 나누기 직전 check in
 * 
 * 50    03-12-21 3:57a Sckyj
 * 메모리 릭 코드 수정
 * 
 * 49    03-12-21 2:32a Sckyj
 * 수정중
 * 
 * 48    03-12-20 8:10p Kjmgo
 * 
 * 47    03-12-20 7:00p Sckyj
 * 
 * 46    03-12-20 2:55p Sckyj
 * 
 * 45    03-12-20 2:33p Kjmgo
 * 
 * 44    03-12-20 11:52a Sckyj
 * Memory Leak 수정
 * 
 * 43    03-12-20 11:27a Sckyj
 * 메모리 누수 수정, play(), play3D()함수 시작시 Stop()을 호출
 * 
 * 42    03-12-20 12:52a Sckyj
 * Play3D() Min Distance를 Max Distance /  7로 변경
 * 
 * 41    03-12-16 5:36p Sckyj
 * 3D 사운드 재생시 들리는 최대거리를 입력할수 있도록 수정
 * 
 * 40    03-12-15 5:31p Sckyj
 * MultiSpeaker관련 코드 추가
 * 
 * 39    03-11-28 11:17a Sckyj
 * reverb 효과 폭 감소( 덜 울리게 조정 1 -> 0.5 )
 * 
 * 38    03-11-28 10:16a Sckyj
 * Map, Area 변경에 따른 특징적인 잔향( Reverb type )을 설정할수 있도록
 * 기능추가
 * 
 * 37    03-11-26 3:51p Sckyj
 * Ambient BGM 시간대별 재생기능 추가
 * 
 * 36    03-11-04 10:43a Sckyj
 * SoundPlay 함수 수정( Reverb control을 IW3DSoundObject 안으로 넣음 )
 * SND_RANDOM 추가
 * 
 * 35    03-11-01 3:41p Sckyj
 * soundobject Update추가( 사운드 재생에 방향부여 )
 * 
 * 34    03-10-18 2:26p Sckyj
 * 확장자가 없는 사운드파일을 wav를 붙여서 로딩하게 변경
 * 
 * 33    03-10-10 11:37a Kjmgo
 * 소스 정리및, Assert 삭제
 * 
 * 
 * 31    03-10-08 4:05p Sckyj
 * 
 * 30    03-10-08 11:20a Sckyj
 * 
 * 29    03-09-18 10:41a Sckyj
 * 
 * 28    03-09-17 10:39a Sckyj
 * 
 * 27    03-09-16 10:41p Sckyj
 * 
 * 26    03-09-16 5:57p Sckyj
 * 
 * 25    03-09-11 10:47p Sckyj
 * 
 * 24    03-09-06 2:57p Sckyj
 * 
 * 23    03-09-06 2:13p Sckyj
 * 
 * 22    03-08-12 10:23p Kjmgo
 * 
 * 21    03-07-26 11:02a Kjmgo
 * 
 * 20    03-07-24 8:27p Kjmgo
 * 
 * 19    03-07-23 3:26p Kjmgo
 * 
 * 18    03-06-26 12:42a Paran
 * 2D Loop 추가.
 * 
 * 17    03-06-01 4:20p Kjmgo
 * 
 * 16    03-06-01 3:14p Kjmgo
 * <HR>
 *
 * @file	W3DSoundObject.cpp
 */


#include "Sound_Export.h"
#include <string>

#define	PHI			3.141591
#define RADIUS		0.5
#define SOUNDANGLE	15
#define SOUNDTICK	400
#define SOUNDOFFSET	0.001

/**
 * 사운드 오브젝트 클래스.
 * @see		CW3DSoundMgr
 * @ingroup		Sound
 */
class CW3DSoundObject2D : public IW3DSoundObject2D
{
public:
	UNDER_MEMORY_MGR( W3DSoundObject2D );

	CW3DSoundObject2D();
	~CW3DSoundObject2D();

	//-=-=-=-=-=-=-=-=-= 2D -=-=-=-=-=-=-=-=-=-//

	void		Play( const char* szName, const WORD wLoop );
	void		Stop();
	void		Update( DWORD dwTick );

	BOOL		IsEnd();

private:

	HSAMPLE		m_hSample;
	IW3DSoundMgr* m_pSoundMgr;		// 싱글톤
	
	DWORD		m_dwTime;			// 누적 틱

	U32*		m_pMem2D;			// 2D 메모리

	char		m_szFile[128];		// file name
}; //class CW3DSoundObject


IMPLEMENT_COMPONENT( CW3DSoundObject2D, SoundObject2D );



/**
 * 사운드 오브젝트 클래스.
 * @see		CW3DSoundMgr
 * @ingroup		Sound
 */
class CW3DSoundObject3D : public IW3DSoundObject3D
{
public:
	UNDER_MEMORY_MGR( W3DSoundObject3D );

	CW3DSoundObject3D();
	~CW3DSoundObject3D();

	void		Play3D( const char* szName, const WORD wLoop, const int nDis = MAX_DISTANCE );
	void		Stop3D();
	void		Update3D( DWORD dwTick );
	
	void		Set3DStyle( BYTE btStyle );
	void		SetReverb( BYTE btReverb, float fLevel );

	void		SetPosition( const Vector3& vPos );
	Vector3		GetPosition();
	
	BOOL		IsEnd();

private:

	void		UpdateRightTurn( DWORD dwTick );	// Turn to the right direction	
	void		UpdateCross1( DWORD dwTick );		// Diagonal Cross play from right to the left Speaker
	void		UpdateCross2( DWORD dwTick );		// Diagonal Cross play from front to the rear Speaker
	void		UpdateFrontRight( DWORD dwTick );	// Front Right Speaker Only
	void		UpdateFrontLeft( DWORD dwTick );	// Front Left Speaker Only
	void		UpdateRearRight( DWORD dwTick );	// Rear Right Speaker Only
	void		UpdateRearLeft( DWORD dwTick );		// Rear left Speaker Only
	void		UpdateRandom( DWORD dwTick );		// Random position play

	Vector3		m_vPos;
	Vector3		m_vDir;

	H3DSAMPLE	m_h3DSample;
	IW3DSoundMgr* m_pSoundMgr;		// 싱글톤
	
	BYTE		m_btStyle;			// Sound Play Style
	DWORD		m_dwTime;			// 누적 틱
	float		m_fAngle;			// 회전 각도

	void FAR*	m_pMem3D;			// 3D 메모리 	

	char		m_szFile[128];		// file name
}; //class CW3DSoundObject

IMPLEMENT_COMPONENT( CW3DSoundObject3D, SoundObject3D );


/**
 * 생성자.
 */
CW3DSoundObject2D::CW3DSoundObject2D()
:	m_pMem2D	( NULL ),
	m_dwTime	( 0	)
{	
	m_pSoundMgr = MAKE_COMPONENT( SoundMgr );
	ZeroMemory( m_szFile, 128 );

	m_hSample	= m_hSample = AIL_allocate_sample_handle( g_pMilesSound->GetDriver() );
} //CW3DSoundObject::CW3DSoundObject

/**
 * 소멸자.
 */
CW3DSoundObject2D::~CW3DSoundObject2D()
{
	if( !m_hSample )	return;

	if( AIL_sample_status( m_hSample ) != SMP_DONE )
	{
		AIL_end_sample( m_hSample );
	}

	if( m_pMem2D )	AIL_mem_free_lock( m_pMem2D );
	
	AIL_release_sample_handle( m_hSample );
} //CW3DSoundObject2D::~CW3DSoundObject2D


/**
 * 사운드 출력.
 */
void CW3DSoundObject2D::Play( const char* szName, const WORD wLoop )
{
	if( !m_hSample ) return;

	if( strcmp( m_szFile, szName ) )
	{
		strcpy( m_szFile, szName );

		char szFileName[128];
		sprintf( szFileName, ".\\Sound\\%s", szName );

		U32* ptr;
		ptr = (U32*)AIL_file_read( szFileName, FILE_READ_WITH_SIZE );
		if( ptr == NULL )
		{
			return;
		}

		if( AIL_sample_status( m_hSample ) != SMP_DONE )
		{
			AIL_end_sample( m_hSample );
		}
		AIL_mem_free_lock( m_pMem2D );
		m_pMem2D = ptr;

		// initialize the sample
		AIL_init_sample( m_hSample );
		// point the sample at the loaded file image (the first dword is the size)
		if( AIL_set_named_sample_file( m_hSample, szName, m_pMem2D+1, m_pMem2D[0], -1 ) == 0 )  
		{
			return;
		} //if	
	}

	AIL_set_sample_loop_count( m_hSample, wLoop );
	AIL_set_sample_volume_levels( m_hSample, m_pSoundMgr->GetVolume(), m_pSoundMgr->GetVolume() );

    // begin playing the sample
    AIL_start_sample( m_hSample );
} //CW3DSoundObject2D::Play


/**
 * 사운드 정지.
 */
void CW3DSoundObject2D::Stop()
{
	if( m_hSample )
	{
		AIL_end_sample( m_hSample );
	} //if

	if( m_pMem2D )
	{
		if( AIL_sample_status( m_hSample ) == SMP_DONE )
		{
			AIL_mem_free_lock( m_pMem2D );
		}		
		else
		{
			Assert( !"2D Sample이 해제되지 않았습니다" );
		}	
	}
} //CW3DSoundObject2D::Stop


/**
 * Update 2D Sound
 */
void CW3DSoundObject2D::Update( DWORD dwTick )
{
} // CW3DSoundObject2D::Update()

/**
 * 끝났는가?
 */
BOOL CW3DSoundObject2D::IsEnd()
{
	WORD result = AIL_sample_status( m_hSample );

	if( result == SMP_STOPPED || result == SMP_DONE )
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	} //if..else..

} // CW3DSoundObject2D::IsEnd


//==================================//==================================//==================================

/**
 * 생성자.
 */
CW3DSoundObject3D::CW3DSoundObject3D()
:	m_pMem3D	( NULL ),
	m_h3DSample ( NULL ),
	m_btStyle	( SND_NORMAL ),
	m_dwTime	( 0 ),
	m_fAngle	( 0 )
{
	m_vPos		= Vector3( 0.0F, 0.0F, 0.0F );
	m_vDir		= Vector3( 0.0F, 0.0F, 1.0F );

	m_pSoundMgr = MAKE_COMPONENT( SoundMgr );

	ZeroMemory( m_szFile, 128 );

	// Seed the random-number generator with currnet time
	srand( ( unsigned )time( NULL ) );	
} //CW3DSoundObject3D::CW3DSoundObject3D


/**
 * 소멸자.
 */
CW3DSoundObject3D::~CW3DSoundObject3D()
{	
	if( !m_h3DSample )	return;

	if( AIL_3D_sample_status( m_h3DSample ) != SMP_DONE )
	{
		AIL_end_3D_sample( m_h3DSample );
	}
	
	if( m_pMem3D )	AIL_mem_free_lock( m_pMem3D );
	
	AIL_release_3D_sample_handle( m_h3DSample );	
} //CW3DSoundObject3D::~CW3DSoundObject3D


/**
 * 사운드 출력.
 */
void CW3DSoundObject3D::Play3D( const char* szName, const WORD wLoop, const int nDis )
{
 	if( !m_pSoundMgr || !m_pSoundMgr->GetOnOff() )		// UI에서 Effect Sound를 Off했을경우 리턴.
	{
		return ;
	} //if

	if( strcmp( m_szFile, szName ) )
	{
		strcpy( m_szFile, szName );

		char szFileName[128] = "\0";
		sprintf( szFileName, ".\\Sound\\%s", szName );
		void* ptr = LoadSound( szFileName );
		if( ptr == NULL )
		{
			return;
		}		

		if( AIL_3D_sample_status( m_h3DSample ) != SMP_DONE )
		{
			AIL_end_3D_sample( m_h3DSample );
		}
		AIL_mem_free_lock( m_pMem3D );
		m_pMem3D = ptr;	
	}
	
	if( m_h3DSample )
	{
		AIL_release_3D_sample_handle( m_h3DSample );
	}
	m_h3DSample = AIL_allocate_3D_sample_handle( g_pMilesSound->GetProvider() );
	if( !m_h3DSample )
	{
		return;
	}
	
	AIL_set_3D_sample_file( m_h3DSample, m_pMem3D );
	
	// Set Distances
	AIL_set_3D_sample_distances( m_h3DSample, nDis, ( int )( nDis / 7 ) );

	// Set Position
	AIL_set_3D_position( m_h3DSample, m_vPos.x, m_vPos.y, m_vPos.z );

	// Set Direction
	AIL_set_3D_orientation(	m_h3DSample,
							m_vDir.x, m_vDir.y, m_vDir.z,
							0.0F, 0.0F, 1.0F);

	// Loop Count
	AIL_set_3D_sample_loop_count( m_h3DSample, wLoop );
	AIL_set_3D_sample_volume( m_h3DSample, m_pSoundMgr->GetVolume() );

	// Set Reverb Type
	SetReverb( m_pSoundMgr->GetReverb(), 0.5 );

	AIL_start_3D_sample( m_h3DSample );

	m_fAngle = 0;
} //CW3DSoundObject3D::Play3D

/**
 * 사운드 정지.
 */
void CW3DSoundObject3D::Stop3D()
{
	if( m_h3DSample )
	{
		AIL_end_3D_sample( m_h3DSample );
	} //if

	if( m_pMem3D )
	{
		if( AIL_3D_sample_status( m_h3DSample ) == SMP_DONE )
		{
			AIL_mem_free_lock( m_pMem3D );
		}
		else
		{
			Assert( !"3D Sample이 해제되지 않았습니다" );
		}	
	}
} //CW3DSoundObject3D::Stop3D

/**
 * 사운드 업데이트 
 * CirclePlay등을 할경우 음원의 위치를 갱신한다.
 */
void CW3DSoundObject3D::Update3D( DWORD dwTick )
{
	Assert( m_btStyle >= SND_NORMAL && m_btStyle <= SND_RANDOM );

	if( m_btStyle == SND_NORMAL || !m_pSoundMgr->GetMultiSpeaker() )
	{
		return;
	}

	m_dwTime += dwTick;
	if( m_dwTime <= SOUNDTICK )
	{
		return;
	}
	else
	{
		m_dwTime = 0;
	}

	switch( m_btStyle )
	{	
	case SND_CIRCLE	:	UpdateRightTurn( dwTick );		break;
	case SND_CROSS1	:	UpdateCross1( dwTick );			break;
	case SND_CROSS2	:	UpdateCross2( dwTick );			break;
	case SND_FR		:	UpdateFrontRight( dwTick  );	break;
	case SND_FL		:	UpdateFrontLeft( dwTick  );		break;
	case SND_RR		:	UpdateRearRight( dwTick  );		break;
	case SND_RL		:	UpdateRearLeft( dwTick  );		break;
	case SND_RANDOM :	UpdateRandom( dwTick );			break;
	}	

	return;
} // CW3DSoundObject3D::Update3D()


/**
 * 사운드 플레이 스타일을 설정한다.
 */
void CW3DSoundObject3D::Set3DStyle( BYTE btStyle )
{
	Assert( btStyle >= SND_NORMAL && btStyle <= SND_RANDOM );
	m_btStyle = btStyle;

	switch( m_btStyle )
	{
	case SND_NORMAL :	
	case SND_RANDOM	:	SetReverb( REVERB_GENERIC, 1.0 );		break;
	default			:	SetReverb( REVERB_MOUNTAINS, 1.0 );		break;
	}
} // CW3DSoundObject3D::Set3DStyle

/**
 * 반향을 설정한다
 */
void CW3DSoundObject3D::SetReverb( BYTE btReverb, float fLevel )
{
	// 룸 타입 설정
	AIL_set_3D_room_type( g_pMilesSound->GetProvider(), btReverb );
	// effect( reverb ) level 설정
	AIL_set_3D_sample_effects_level( m_h3DSample, fLevel );
	
} // CW3DSoundObject3D::SetReverb


/**
 * 사운드 위치 지정.
 *
 * @param	vPos		[IN] 사운드 위치.
 *
 * @author	아샬
 * @date	2003-02-06 오후 10:58
 */
void CW3DSoundObject3D::SetPosition( const Vector3& vPos )
{
	m_vPos = vPos;

	if( m_h3DSample )
	{
		AIL_set_3D_position( m_h3DSample, m_vPos.x, m_vPos.y, m_vPos.z );
	} //if
} //CW3DSoundObject3D::SetPosition


/**
 * 사운드 위치를 얻어옴.
 */
Vector3 CW3DSoundObject3D::GetPosition()
{
	return m_vPos;
} //CW3DSoundObject3D::GetPosition


/**
 * 끝났는가?
 */
BOOL CW3DSoundObject3D::IsEnd()
{
	WORD result = AIL_3D_sample_status( m_h3DSample );

	if( result == SMP_STOPPED || result == SMP_DONE )
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	} //if..else..

} // CW3DSoundObject3D::IsEnd


/**
 * Circle 음원의 위치 갱신.
 */
void CW3DSoundObject3D::UpdateRightTurn( DWORD dwTick )
{
	Vector3 vTemp, vDir;

	m_fAngle += SOUNDANGLE;

	vTemp = m_vPos + Vector3( sin( m_fAngle ) * SOUNDOFFSET, cos( m_fAngle ) * SOUNDOFFSET, 0 );
	AIL_set_3D_position( m_h3DSample, vTemp.x, vTemp.y, vTemp.z );

	vDir = vTemp - m_vPos;
	AIL_set_3D_orientation(	m_h3DSample,
							vDir.x, vDir.y, vDir.z,
							0.0F, 0.0F, 1.0F);
	
} // CW3DSoundObject3D::UpdateRightTurn

/**
 * Cross Play 1
 */
void CW3DSoundObject3D::UpdateCross1( DWORD dwTick )
{
	Vector3 vTemp, vDir;

	m_fAngle += SOUNDANGLE;

	vTemp = m_vPos + Vector3( 2 * cos( m_fAngle ) * SOUNDOFFSET, cos( 2 * m_fAngle ) * SOUNDOFFSET, 0 );
	AIL_set_3D_position( m_h3DSample, vTemp.x, vTemp.y, vTemp.z );

	vDir = vTemp - m_vPos;
	AIL_set_3D_orientation(	m_h3DSample,
							vDir.x, vDir.y, vDir.z,
							0.0F, 0.0F, 1.0F);

} // CW3DSoundObject3D::UpdateCross( DWORD dwTick )

/**
 * Cross Play 2
 */
void CW3DSoundObject3D::UpdateCross2( DWORD dwTick )
{
	Vector3 vTemp, vDir;

	m_fAngle += SOUNDANGLE;

	vTemp = m_vPos + Vector3( cos( 2 * m_fAngle ) * SOUNDOFFSET, 2 * cos( m_fAngle ) * SOUNDOFFSET, 0 );
	AIL_set_3D_position( m_h3DSample, vTemp.x, vTemp.y, vTemp.z );

	vDir = vTemp - m_vPos;
	AIL_set_3D_orientation(	m_h3DSample,
							vDir.x, vDir.y, vDir.z,
							0.0F, 0.0F, 1.0F);

} // CW3DSoundObject3D::UpdateCross( DWORD dwTick )

/**
 *
 */
void CW3DSoundObject3D::UpdateFrontRight( DWORD dwTick )
{
	Vector3 vTemp, vDir;

	m_fAngle += SOUNDANGLE;

	vTemp = m_vPos + Vector3( SOUNDOFFSET * 1000, SOUNDOFFSET * 1000, 0 );
	AIL_set_3D_position( m_h3DSample, vTemp.x, vTemp.y, vTemp.z );

	vDir = vTemp - m_vPos;
	AIL_set_3D_orientation(	m_h3DSample,
							vDir.x, vDir.y, vDir.z,
							0.0F, 0.0F, 1.0F);
} // CW3DSoundObject3D::UpdateFrontRight

/**
 *
 */
void CW3DSoundObject3D::UpdateFrontLeft( DWORD dwTick )
{
	Vector3 vTemp, vDir;

	m_fAngle += SOUNDANGLE;

	vTemp = m_vPos + Vector3( -1 * SOUNDOFFSET * 1000, SOUNDOFFSET * 1000, 0 );
	AIL_set_3D_position( m_h3DSample, vTemp.x, vTemp.y, vTemp.z );

	vDir = vTemp - m_vPos;
	AIL_set_3D_orientation(	m_h3DSample,
							vDir.x, vDir.y, vDir.z,
							0.0F, 0.0F, 1.0F);
} // CW3DSoundObject3D::UpdateFrontLeft

/**
 *
 */
void CW3DSoundObject3D::UpdateRearRight( DWORD dwTick )
{
	Vector3 vTemp, vDir;

	m_fAngle += SOUNDANGLE;

	vTemp = m_vPos + Vector3( SOUNDOFFSET * 1000, -1 * SOUNDOFFSET * 1000, 0 );
	AIL_set_3D_position( m_h3DSample, vTemp.x, vTemp.y, vTemp.z );

	vDir = vTemp - m_vPos;
	AIL_set_3D_orientation(	m_h3DSample,
							vDir.x, vDir.y, vDir.z,
							0.0F, 0.0F, 1.0F);
} // CW3DSoundObject3D::UpdateRearRight

/**
 *
 */
void CW3DSoundObject3D::UpdateRearLeft( DWORD dwTick )
{
	Vector3 vTemp, vDir;

	m_fAngle += SOUNDANGLE;

	vTemp = m_vPos + Vector3( -1 * SOUNDOFFSET * 1000, -1 * SOUNDOFFSET * 1000, 0 );
	AIL_set_3D_position( m_h3DSample, vTemp.x, vTemp.y, vTemp.z );

	vDir = vTemp - m_vPos;
	AIL_set_3D_orientation(	m_h3DSample,
							vDir.x, vDir.y, vDir.z,
							0.0F, 0.0F, 1.0F);
} // CW3DSoundObject3D::UpdateRearLeft

/**
 * Random Position play
 */
void CW3DSoundObject3D::UpdateRandom( DWORD dwTick )
{
	Vector3 vTemp, vDir;
	int		nX, nY;
	int		nNum = rand() % 4;
	switch( nNum )
	{
	case 0 :	nX = 1; nY = 1;		break;
	case 1 :	nX = 1; nY = -1;	break;
	case 2 :	nX = -1; nY = -1;	break;
	case 3 :	nX = -1; nY = 1;	break;
	}

	vTemp = m_vPos + Vector3( nX * SOUNDOFFSET, nY * SOUNDOFFSET, 0 );
	AIL_set_3D_position( m_h3DSample, vTemp.x, vTemp.y, vTemp.z );

	vDir = vTemp - m_vPos;
	AIL_set_3D_orientation(	m_h3DSample,
							vDir.x, vDir.y, vDir.z,
							0.0F, 0.0F, 1.0F);
} // CW3DSoundObject3D::UpdateRearLeft