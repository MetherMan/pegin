/**
 * 마법 처리 구현부.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-13 8:14p $
 * @version	$Revision: 23 $
 *
 * <HR>
 * $Log: /Engine/Inc/IW3DMagic.h $
 * 
 * 23    04-01-13 8:14p Kjmgo
 * Creature및IW3DMagic을 포인터레퍼런스로 관리하도록 수정
 * 
 * 22    03-12-26 11:57a Kjmgo
 * 스킬 관련 코드 추가
 * 
 * 21    03-12-26 11:51a Kjmgo
 * AddSkill함수 추가, ( 다른건 없고, AddMagic에서, 몇가지 수정 )
 * 
 * 20    03-10-15 8:36p Kjmgo
 * 
 * 19    03-10-01 11:38a Kjmgo
 * BlowTiming 작업
 * 
 * 18    03-09-22 9:07p Kjmgo
 * 스킬 쪽 관련 작업( 마법 관리자 )
 * 
 * 17    03-09-13 1:25p Kjmgo
 * 마법 동작을 Creature Action에서 처리하도록 몇가지 옵션 추가
 * 
 * 16    03-08-19 3:19a Kjmgo
 * 
 * 15    03-06-13 8:19p Kjmgo
 * 
 * 14    03-06-13 3:02a Kjmgo
 * 마법 체계 뒤엎음
 * 
 * 13    03-05-11 9:23p Kjmgo
 * 
 * 12    03-04-04 4:58a Ahastudio
 *
 * 8     03-02-24 4:47p Kjmgo		Init() 시 IW3DParticles 를 포인터로 넘겨 주게 수정
 * 3     03-02-21 7:24p Ahastudio	CCreature -> IW3DCreature로 변경. Release -> ReleaseAll로 변경.
 * <HR>
 *
 * @file	IW3DMagic.h
 */


#ifndef __Wind3D_Engine_Magic_Header__
#define __Wind3D_Engine_Magic_Header__

struct MAGIC;

/**
 * 마법 구현 인터페이스
 * @ingroup		Engine
 */
class IW3DMagic : public IComponent
{
public:

	/**
	 * 초기화 함수
	 * @return	초기화 성공여부
	 */
	virtual BOOL Create( 
						IW3DCreature&		pAttacker, 
						IW3DCreature&		pTarget, 
						IW3DMagicMgr*		pMagicMgr,
						MAGIC*				pMagic,
						BOOL				bSendEndAttack = TRUE
						) = 0;

	virtual void SetSkill() = 0;

	/**
	 *
	 *
	 */
	virtual BOOL Update( const DWORD dwTick ) = 0;

	/**
	 *
	 *
	 */
	virtual BOOL Draw() = 0;

	/**
	 *
	 *
	 *
	 */
	virtual BOOL IsEnd() = 0;

	/**
	 *
	 *
	 */
	virtual void Play() = 0;

	/**
	 *
	 *
	 */
	virtual void Pause() = 0;

	/**
	 *
	 *
	 */
	virtual void Stop() = 0;

	/**
	 *
	 */
	virtual void SetID( const DWORD dwID ) = 0;

	/**
	 *
	 */
	virtual DWORD GetID() = 0;

	/**
	 *
	 *
	 */
	virtual IW3DCreature* GetAttacker() = 0;

	/**
	 *
	 *
	 */
	virtual IW3DCreature* GetTarget() = 0;

	/**
	 *
	 *
	 */
	virtual void DieAttacker() = 0;

	/**
	 *
	 *
	 */
	virtual void DieTarget() = 0;

	/**
	 *
	 */
	virtual BYTE GetMagicKind() = 0;
	
	/**
	 *
	 */
	virtual BYTE GetMagicType() = 0;

	/**
	 *
	 */
	virtual BOOL IsSendEndAttack() = 0;
}; //class IW3DMagic



/**
 * 마법 관리 인터페이스.
 * @ingroup		Engine
 */
class IW3DMagicMgr : public IComponent
{
public:
	// 마법을 쓰는 부분 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	/**
	 * 초기화 함수.
	 *
	 * @param	pWorld		[IN] World 객체
	 *
	 * @return	초기화 성공 여부
	 */
	virtual BOOL	Init( IW3DWorld* pWorld ) = 0;

	/**
	 * 마법을 등록한다.
	 *
	 * @return  등록 성공 여부 ( 마법의 존재 여부 )
	 */
	virtual IW3DMagic*	AddMagic( 
		const char*		szMagic,	///< 마법이름
		IW3DCreature*	pAttacker,  ///< 공격자
		IW3DCreature*	pTarget,	///< 타격자
		BOOL			bSendEndAttack = TRUE
		) = 0;

	/**
	 *
	 *
	 */
	virtual IW3DMagic*	AddSkill(
		const char*		szMagic,	///< 마법이름
		IW3DCreature*	pAttacker,  ///< 공격자
		IW3DCreature*	pTarget	///< 타격자
		) = 0;

	/**
	 *
	 *
	 *
	 */
	virtual BOOL		AddMagic( 
		const char*		szScript	///< 스크립트 이름
		) = 0;

	/**
	 * 갱신.
	 *
	 * @param	dwTick		[IN] 경과 tick.
	 */
	virtual void	Update( DWORD dwTick ) = 0;

	/**
	 * Draw.
	 */
	virtual void	Draw() = 0;

	// Magic 을 보관 하는 부분 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	/**
	 * Magic List를 읽어 온다.
	 *
	 * @param	szFileName		[IN] List파일 이름
	 */
	virtual BOOL	Load( const char* szFileName ) = 0;

	/**
	 *	모든 마법들을 릴리즈 한다.
	 */
	virtual void	ReleaseAll() = 0;

	/**
	 *
	 */
	virtual IW3DTraceEffect*	GetTraceEffect() = 0;

	/**
	 *
	 */
	virtual IW3DParticles*		GetParticles() = 0;


	/**
	 *
	 */
	virtual BOOL				LoadParticle( const char* szName ) = 0;

	/**
	 *
	 */
	virtual IW3DSpecialEffect*	LoadEffect( const char* szName ) = 0;

}; //class IW3DMagic


#endif // #ifndef __Wind3D_Engine_Magic_Header__
