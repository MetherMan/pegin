/**
 * 마법 처리.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-30 5:28p $
 * @version	$Revision: 41 $
 *
 * <HR>
 * $Log: /Engine/Src/Magic.h $
 * 
 * 41    03-12-30 5:28p Sckyj
 * MagicEffect Sound를 두개에서 한개로 줄임
 * 
 * 40    02-12-12 4:17p Sckyj
 * Sound 객체 변경
 * 
 * 39    03-12-14 4:17a Kjmgo
 * 화살 버그 수정
 * 
 * 38    03-12-04 4:58p Kjmgo
 * Model 관련 추가
 * 
 * 37    03-12-03 10:27a Kjmgo
 *  카메라 흔들기 추가
 * 
 * 36    03-12-01 10:25a Kjmgo
 * 코드 정리
 * 
 * 35    03-11-01 3:42p Sckyj
 * 마법 이펙트 사운드 관련 코드 추가( 스크립트에서 사운드 재생 타입을
 * 읽어서 사운드 출력 )
 * 
 * 34    03-10-10 3:45a Kjmgo
 * 상대방에게도  Bone에 이펙트 붙이도록 수정
 * 
 * 33    03-10-08 3:03p Kjmgo
 * 상대방에게도 Bone에 이펙트를 달수 있도록 추가
 * 
 * 32    03-10-01 11:38a Kjmgo
 * BlowTiming 작업
 * 
 * 31    03-09-18 5:15p Kjmgo
 * Magic에서 Bone을 사용할수 있게 수정
 * 
 * 30    03-09-13 1:25p Kjmgo
 * 마법 동작을 Creature Action에서 처리하도록 몇가지 옵션 추가
 * 
 * 29    03-08-22 8:52p Kjmgo
 * 
 * 28    03-08-22 11:11a Kjmgo
 * 
 * 27    03-08-20 12:48p Kjmgo
 * 
 * 26    03-08-19 3:19a Kjmgo
 * 
 * 25    03-07-31 1:36a Kjmgo
 * 
 * 24    03-06-17 10:17p Kjmgo
 * 
 * 23    03-06-15 7:29p Kjmgo
 * 
 * 22    03-06-13 8:19p Kjmgo
 * 
 * 21    03-06-13 3:02a Kjmgo
 * 마법 체계 뒤엎음
 * 
 * 20    03-06-11 9:36p Kjmgo
 * Sound 추가
 * 
 * 19    03-05-11 9:23p Kjmgo
 * 
 * 18    03-05-03 11:07a Ahastudio
 *
 * 10    03-03-21 6:23a Kjmgo		Creature를 포인터로 가지지 않게 고침.
 *
 * 7     03-03-07 8:35p Kjmgo		Bezier 관련 수정.
 *
 * 2     03-02-26 11:03a Kjmgo		구조체 부분만 IW3DMagic.cpp에서 빼냈음.
 * <HR>
 *
 * @file	Magic.h
 */


#ifndef __Magic_Header__
#define __Magic_Header__

#include <string>
#include <list>
#include <map>


#define	TARGET_DIE_STOP			0
#define TARGET_DIE_LASTTARGET	1
#define FRAME_RATE				30

struct MAGIC;
struct EFFECT;
struct PART;
struct BEZIER;

class CMagic;
class CMagicEffect;
class CMagicPart;



/**
 *
 */
namespace Magic
{
	/**
	 *	월드를 세팅한다.
	 */
	void	SetWorld( IW3DWorld* pWorld );

	/**
	 *	월드를 흔든다.
	 */
	void	Shake( DWORD dwTime );
} //namespace Magic

/**
 * Bezier Curve.
 * @ingroup		Engine
 */
struct BEZIER
{
	UNDER_MEMORY_MGR( BEZIER );

	//-=-=-=-=-=-=-=-=-=-=-= Vector3 -=-=-=-=-=-=-=-=-=-=-=//
	Vector3		v1;			///< 시작점
	Vector3		v2;			///< Control Point 1
	Vector3		v3;			///< Control Point 2
	Vector3		v4;			///< 종료 점

	float		t;

	/**
	 * 초기화.
	 */
	void Init()
	{
		v1 = Vector3( 0.0F, 0.0F, 0.0F );
		v2 = v3 = v4 = v1;
		t = 0.0F;
	} //Init

	/**
	 *
	 */
	BOOL Move( float time )
	{
		t += time;

		if( t >= 0.98 )
		{
			return FALSE;
		} //if

		return TRUE;
	}//Move

	/**
	 * 곡선 위의 중간 좌표를 얻어 온다.
	 *
	 * @param	t		0부터 1 사이값
	 *
	 * @return		중간 좌표.
	 */
	Vector3		GetBezier()
	{
		Vector3		vPos;

		vPos  = v1     * ( 1 - t ) * ( 1 - t ) * ( 1 - t );
		vPos +=	v2 * 3 * t         * ( 1 - t ) * ( 1 - t );
		vPos +=	v3 * 3 * t         * t         * ( 1 - t );
		vPos +=	v4     * t         * t         * t;

		return vPos;
	} //GetBezier
}; //struct BEZIER

/*
*/
struct BLOW
{
	BYTE	btType;			///< 타입 0 : 타임에 맞는 타이밍
							///<	  1 : ATTACK이 끝나고 타이밍
	DWORD	dwTime;			///< 타격이 뜰 시간
	char	szSound[40];	///< 사운드 파일 이름

}; //struct BLOW

/*
*/
struct PART
{
	UNDER_MEMORY_MGR( PART );
	Vector3				vPos;						///< 시작 위치
	Vector3				vTarget;					///< 종료 위치

	BYTE				btType;						///< 움직이는 종류 (탄도, 직선, ...)
	BOOL				bAttack;					///< Default 0. 1일 경우 피시전자를 따라가는 걸로 한다.

	//-=-=-=-=-=-=-=-=-=-=-= Bone -=-=-=-=-=-=-=-=-=-=-=//
	BOOL				bBone;						///< 캐릭터의 본을 따라 움직이는가?
	char				szBone[32];					///< 본 

	//-=-=-=-=-=-=-=-=-=-=-= Time -=-=-=-=-=-=-=-=-=-=-=//
	DWORD				dwStartTime;				///< 이것이 완료 되면 Play한다.
	DWORD				dwLimit;					///< 제한 시간 ( 이것이 완료 되면 종료 한다. )
	DWORD				dwParticleDelay;			///< 파티클 딜레이 타임
	DWORD				dwSoundDelay;				///< 사운드 딜레이 타임
	DWORD				dwSpeed;					///<

	//-=-=-=-=-=-=-=-=-=-=-= Flag -=-=-=-=-=-=-=-=-=-=-=//
	BOOL				bBlow;						///< 타격
	BOOL				bTrace;						///< 꼬리가 남는지 여부.
	BOOL				bTail;						///< 꼬리용 Effect의 여부.
	BOOL				bLoop;						///< 루프 여부.
	BOOL				bShow;						///< Main Effect를 보일지 여부.
	BOOL				bEnemyPos;					///< 적의 위치에서 시작.
	BOOL				bPause;						///< 마지막 프레임에서 멈출지 여부.
	BOOL				bSound;						///< 사운드가 있는지 여부
	BOOL				bBlowTiming;				///< 타격 타이밍이 지적되어 있는지의 여부
	BOOL				bShowModel;					///< 오브젝트를 보일지의 여부

	float				fTraceDist;					///< 꼬리의 길이..~
	char				szSound[80];				///< 사운드 파일 이름
	BYTE				btPlayType;					///< 사운드 플레이 타입
	DWORD				dwShake;					///<

	BLOW				blowTiming;

	//-=-=-=-=-=-=-=-=-=-=-= Effect -=-=-=-=-=-=-=-=-=-=-=//
	char				szParticle[40];				///< Particle 이름
	BOOL				bParticle;					///< 파티클인지 Special Effect인지 구별.

	BOOL				bBezier;					///< Bezier 곡선을 사용하는지의 여부.

	IW3DSpecialEffect*	pTailEffect;				///< Tail Effect
	IW3DBillboard*		pBillboard;					///< BillBoard

	Vector3				vBezier2;
	Vector3				vBezier3;

	/**
	 * 초기화 함수 
	 */
	void Init()
	{
		//-=-=-=-=-=-=-=-=-=-=-= Flag -=-=-=-=-=-=-=-=-=-=-=//
		bPause = bParticle = bEnemyPos = bAttack = FALSE;
		bSound = bBezier = bBlow = bTail = bLoop = FALSE;
		bBlowTiming	 = FALSE;
		
		bShow		= TRUE;
		bShowModel	= TRUE;
		pBillboard	= NULL;
		pTailEffect = NULL;

		//-=-=-=-=-=-=-=-=-=-=-= Time -=-=-=-=-=-=-=-=-=-=-=//
		dwSoundDelay = dwParticleDelay = dwStartTime = btType = 0;	// 기본 적으로 정지
		dwLimit = 1000;	// 기본적으로 1초.;;
		dwSpeed = 30;

		vPos = Vector3( 0.0F, 0.0F, 0.0F );						///< 시작 위치
		vTarget = vPos;					///< 종료 위치

		bBone = FALSE;
		sprintf( szBone, "" );

		//-=-=-=-=-=-=-=-=-=-=-= Etc -=-=-=-=-=-=-=-=-=-=-=//
		fTraceDist = 0.1F;
		dwShake = 0;
	} //Init

	/**
	 *
	 */
	~PART()
	{
		SAFE_RELEASE( pBillboard );
	} //while
}; //struct PART

/*
*/
struct EFFECT
{
	UNDER_MEMORY_MGR( EFFECT );
	//-=-=-=-=-=-=-=-=-= Effect -=-=-=-=-=-=-=-=-=//
	IW3DModel*				pModel;						///< 이펙트용 모델
	IW3DSpecialEffect*		pEffect;					///< Effect 포인터
	IW3DSpecialEffect*		pBlowEffect;				///< 타격용 Effect 포인터
	char					szTrace[40];				///< Trace 이름

	//-=-=-=-=-=-=-=-=-= Flag -=-=-=-=-=-=-=-=-=//
	BOOL					bEnemyPos;					///< 적의 위치를 갱신할 필요가 있는가?
	BOOL					bTrace;						///< 꼬리가 남는지 판단.
	BOOL					bSpecialEffect;				///< Particle & SpecialEffect 조합때 사용
	BOOL					bLoop;						///< 루프를 돌릴것인지 
	BOOL					bSound;						///< Sound 여부 

	//-=-=-=-=-=-=-=-=-= Magic -=-=-=-=-=-=-=-=-=//
	typedef std::list<PART*>	MagicDataList;
	MagicDataList			listMagic;					///< 마법 리스트.
	
	BOOL					bDraw;						///< 그릴 상황인지 여부.

	/**
	 *
	 */
	EFFECT()
		: 	pEffect( NULL ),
			pModel( NULL ),
			pBlowEffect( NULL )
	{
	} //EFFECT

	/**
	 *
	 */
	~EFFECT()
	{
		MagicDataList::iterator itor = listMagic.begin();

		while( itor != listMagic.end() )
		{
			PART* pPart = *itor;
			SAFE_DELETE( pPart );
			itor = listMagic.erase( itor );
		} //while
	} //~Effect
}; //struct EFFECT 

/*
	메모리상에 올라갈 MAGIC정보이다.
*/
struct MAGIC
{
	UNDER_MEMORY_MGR( MAGIC );

	//-=-=-=-=-=-=-=-= Effect -==-=-=-=-=-=-=//
	typedef std::list<EFFECT*>	EffectList;
	EffectList					listEffect;		///< 이펙트 리스트.

	//-=-=-=-=-=-=-=-= Magic -=-=-=-=-=-=-=-=//

	char				szMagicName[40]; ///< 마법 이름
	int					nScriptType;	///< 마법 타입
	BYTE				btType;			///< 
	BYTE				btKind;			///< 마법 종류


	//-=-=-=-=-=-=-=-= Sound -=-=-=-=-=-=-=-=-=//
	BOOL				bSound;			///< 사운드 여부
	DWORD				dwSoundDelay;	///< Time
	char				szSound[40];	///< 사운드 파일 이름	

	/**
	 *
	 */
	~MAGIC()
	{
		EffectList::iterator itor = listEffect.begin();

		while( itor != listEffect.end() )
		{
			EFFECT* pEffect = *itor;
			SAFE_DELETE( pEffect );
			itor = listEffect.erase( itor );
		} //while
	} //~MAGIC
}; //struct MAGIC

/**
 * 방향을 설정해 준다. ( 위아래 전 방향 )
 *
 * @param	vDir	[IN] 방향
 *
 * @return	쿼터니온값으로 방향을 넘겨준다.
 */
static Quaternion GetDir( Vector3& vDir )
{
	if( vDir == Vector3( 0.0F, 0.0F, 0.0F ) )
	{
		vDir = Vector3( 0.0F, 1.0F, 0.0F );
	} //if

	vDir.Normalize();
	vDir *= -1;

	static Matrix4		matWorld;
	matWorld.SetWorld( Vector3( 0.0F, 0.0F, 0.0F ), vDir, Vector3( 0.0F, 0.0F, 1.0F ) );

	return matWorld.GetQuaternion();
} //GetDir

/**
 * 상대적인 좌표를 얻어 온다.
 * ( ex: Vector3 에서 오른쪽으로 3미터 앞쪽으로 3미터 따위를 얻을 때 )
 *
 *	@return		계산된 상대적인 좌표.
 */
static Vector3 RelativeCoordinates(
	const Vector3&	vPos,			///< [IN] 시작 위치
	const Vector3&	vTarget,		///< [IN] 바라볼 위치
	const Vector3&	vDist			///< [IN] 떨어뜨릴 위치
	)
{
	static Vector3	v;
	static Vector3	vUp;
	static Vector3	vDir;
	static Vector3	vRight;

	vDir = vTarget - vPos;

	if( vTarget == vPos )
	{
		vDir = Vector3( 0.0F, 1.0F, 0.0F );
	} //if

	vUp.x = 0.0F;
	vUp.y = 0.0F;
	vUp.z = 1.0F;

	vDir.Normalize();
	vDir.z = 1.0F;

	vRight = vDir.CrossProduct( vUp ) * -1;

	vRight *= vDist.x;
	vDir *= vDist.y;

	vRight += vPos;

	v = vRight;
	v += vDir;
	v.z = vPos.z + vDist.z;

	return v;
} //RelativeCoordinates

/**
 * 마법 클래스.
 * @ingroup		Engine
 */
class CMagicEffect
{
public:
	UNDER_MEMORY_MGR( CMagicEffect );

	CMagicEffect();
	~CMagicEffect();

	//-=-=-=-=-=-=-=-=-= Magic -=-=-=-=-=-=-=-=-=//

	BOOL	Create( EFFECT* pEffect );


	BOOL	Start( const Vector3& vPos, const Vector3& vTarget, IW3DTraceEffect* pTrace );
	BOOL	NextStep();
	BOOL	Tracking();

	void	AddPart( PART* pPart );

	void	SetAttackerInformation( Matrix4 matWorld );

	//-=-=-=-=-=-=-=-=-= Draw -=-=-=-=-=-=-=-=-=//
	BOOL	Update( const DWORD dwTick, const Vector3& vTarget, IW3DParticles* pParticles, Matrix4 matWorld, Matrix4 matTargetWorld, DWORD dwTime, DWORD dwTargetTime );
	BOOL	Draw();

	void	SetModel( IW3DModel* pModel )
	{
		m_pModel = pModel; 
	} //SetModel

	void	SetTargetModel( IW3DModel* pModel )
	{
		m_pTargetModel = pModel;
	} //SetTargetModel

	/**
	 *
	 */
	void	SetAttacker( IW3DCreature* pCreature )
	{
		m_pAttacker = pCreature;
	} //SetAttacker

	void	SoundPlay( BYTE btType, char* szSound );	///< type 별로 sound play

public:

	DWORD		m_dwTrace;						///< Trace 고유 아이디 

	//-=-=-=-=-=-=-=-=-= Time -=-=-=-=-=-=-=-=-=//
	DWORD		m_dwLoopTime;					///< 반복될 시간
	DWORD		m_dwPartTime;					///< 
	DWORD		m_dwLoopTick;					///< 반복될 시간을 체크할 누적 틱

private:

	//-=-=-=-=-=-=-=-=-= Magic -=-=-=-=-=-=-=-=-=//
	typedef std::list<PART*>		MagicDataList;

	MagicDataList				m_listMagic;	///< 마법 리스트.
	MagicDataList::iterator		m_itorList;		///< 마법 리스트 이터레이터.
	
	IW3DTraceEffect*	m_pTrace;				///< Trace Effect.

	//-=-=-=-=-=-=-=-=-= Position -=-=-=-=-=-=-=-=-=//
	Vector3		m_vMove;						///< 중간 움직이는 위치 ( 변화는 위치 )
	Vector3		m_vTarget;						///< 피시전자 위치 (최종)
	Vector3		m_vEnemyPos;					///< 피시전자 위치
	Vector3		m_vPos;							///< 시전자 위치
	Quaternion	m_qRot;							///< 방향
	Matrix4		m_matWorld;
	
	BOOL		m_bDraw;						///< 그릴 상황인지 여부.

	//-=-=-=-=-=-=-=-=-= Time -=-=-=-=-=-=-=-=-=//
	DWORD		m_dwTime;						///< 내부 누적 틱
	DWORD		m_dwTick;						///< 내부 누적 틱
	DWORD		m_dwTimeForTail;				///< 꼬리용 누적 틱
	DWORD		m_dwTimeParticle;
	DWORD		m_dwTimeForDraw;				///< 내부 누적 틱 ( 그리기용 ) 
	DWORD		m_dwTimeForModel;				///< 모델을 위한 누적 틱
	DWORD		m_dwStartTime;
	DWORD		m_dwSpeed;						///< 마법 스피또 


	BOOL		m_bSound;						///< 사운드 여부
	BOOL		m_bBlowTiming;					///< 타격 타이밍
	BYTE		m_btType;						///< 타입
	EFFECT*		m_pEffectData;
	BEZIER		m_bezier;						///< Bezier

	IW3DSoundObject3D*		m_pSound;			///< Sound
	IW3DModel*				m_pModel;			///< Model
	IW3DModel*				m_pTargetModel;		///< 
	IW3DCreature*			m_pAttacker;		///< 공격자의 크리처

	//-=-=-=-=-=-=-=-=-= Move -=-=-=-=-=-=-=-=-=//
	BOOL		TrackingBezier();
	void		UpdateEnemyPos( IW3DParticles* pParticles );	
	BOOL		Move( PART* pMagic, const Vector3& vTarget );

	//-=-=-=-=-=-=-=-=-= Effect -=-=-=-=-=-=-=-=-=//
	void		DrawTail();
	void		EtcEffect( PART* pMagic, IW3DParticles* pParticles );	
}; //class CMagicEffect


#endif //#ifndef __Magic_Header__