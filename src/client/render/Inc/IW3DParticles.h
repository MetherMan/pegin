/**
 * 파티클.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-04-11 4:44a $
 * @version	$Revision: 22 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DParticles.h $
 * 
 * 22    03-04-11 4:44a Ahastudio
 * <HR>
 *
 * @file	IW3DParticles.h
 */


#ifndef __Wind3D_Render_IW3DParticles_Header__
#define __Wind3D_Render_IW3DParticles_Header__


#define STR_FILEINFO		"#W3DParticleInfo File"
#define INT_FILEVERSION		101			// * 1.00


/**
 * 파티클 컴포넌트 인터페이스.
 * @ingroup		Render
 */
class IW3DParticles : public IComponent
{
public:
	/**
	 * 파티클 시스템 초기화 함수.
	 * 파티클을 사용하기 전에 반드시 호출해야줘야 한다.
	 * 내부적으로 사용할 Vertex Buffer 생성 등을 한다.
	 *
	 * @return	초기화 성공 여부
	 */
	virtual BOOL	Init() = 0;

	/**
	 * 모든 파티클을 움직이고 렌더링한다.
	 *
	 * @param	dwTick			[IN] 누적 시간 (1000 Ticks = 1 Second)
	 */
	virtual void	Render( const DWORD dwTick ) =  0;

	/**
	 * 미리 정의한 ParticleType 이름과 위치 정보를 입력해 파티클 하나를 추가한다.
	 *
	 * @return	파티클 추가 성공 여부
	 */
	virtual BOOL	AddParticle(
		const char*		szType,		///< [IN] 파티클 타입 이름
		const Vector3&	vPos		///< [IN] 파티클 생성 위치
		) = 0;

	/**
	 * 현재 처리 중인 전체 파티클의 개수를 리턴해 줍니다.
	 *
	 * @return	파티클 개수
	 */
	virtual UINT	GetCount() = 0;

	/**
	 * 파티클 타입 파일을 로드한다.
	 * 이전에 타입 데이터가 로드되어 있다면 모두 삭제 처리하고 새로운 데이터를 로드한다.
	 *
	 * 파일 로드를 실패할 경우 :
	 * @li 파일형식과 버젼 체크 실패
	 * @li Texture파일 로드실패
	 * @li Header추가를 위한 메모리 할당 실패
	 *
	 * @param	szFileName			[IN] 로드할 파일 이름
	 *
	 * @return	파일 로드 성공 여부.
	 */
	virtual BOOL	LoadParticleTypeFile( const char* szFileName ) = 0;

	/**
	 * 텍스쳐 파일을 로드해서 추가합니다.
	 * LoadParticleTypeFile 함수 내부에서 처리하므로 따로 호출은 필요치 않습니다.
	 *
	 * @param	szTexName			[IN] 텍스처 파일 이름
	 *
	 * @return	텍스처 파일 로드 성공 여부
	 */
	virtual BOOL	AddTexture( const char *szTexName ) = 0;

	/**
	 * 파티클 종류를 추가해주는 함수.
	 * (이 함수는 툴을 위해 존재한다.)
	 *
	 * @return	파티클 종류 추가 성공 여부
	 */
	virtual BOOL	AddParticleType(
		const char*		szType,		///< [IN] 파티클 종류 (이름)
		const void*		pHead		///< [IN] 파티클 정보
		) = 0;
}; //class IW3DParticles


#define	NUM_PARTICLENODE		12


/**
 * 파티클 생성용 정보
 * @ingroup		Render
 */
struct ParticleHead
{
	UNDER_MEMORY_MGR( ParticleHead );

	int				nDelay;
	int				nTick;
	int				nNumber;
	int				nLife;
	float			fFade;
	float			fSpeed;
	float			fResistance;
	Vector3			vAccelerate;		
	float			fForce;
	int				nNumLevel;
	BYTE			btColor[3][NUM_PARTICLENODE];		// RGB Array
	BYTE			btColor_s[3][NUM_PARTICLENODE];		// RGB Second Array
	float			fSize[NUM_PARTICLENODE];
	UINT			nTexIndex;
	BOOL			bInverse;
}; //struct ParticleHead


#endif // #ifndef __Wind3D_Render_IW3DParticles_Header__
