/**
 * 성문에 대한 처리
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-11 7:40p $
 * @version	$Revision: 1 $
  *
 * <HR>
 * $Log: /GameClient/Src/CastleGate.h $
 * 
 * 1     04-01-11 7:40p Kjmgo
 * 생성
 * 
 * 
 * <HR>
 *
 * @file	CastleGate.h
 */

#ifndef __Castle_Gate_Header__
#define __Castle_Gate_Header__

#include <map>
#include <string>

/**
 *
 */
struct CastleGate
{
	Vector3		vPos;		///< 위치
	Quaternion	qRot;		///< 방향

	IW3DModel*	pModel;		///< 성문 모델.
	SAction*	pAction;	///< 애니메이션 

	BOOL		bOpen;		///< 열려 있는지의 여부

	DWORD		dwTicks;	///< 

	/**
	 *
	 */
	CastleGate()
		: pModel	( NULL ),
		  pAction	( NULL ),
		  dwTicks	( 0 ),
		  bOpen		( FALSE )		// 처음엔 닫혀 있다.
	{
	} //CastleGate

	/**
	 *
	 */
	~CastleGate()
	{
	} //~CastleGate
}; //struct CastleGate

/**
 *
 */
class CCastleGate
{
public:
	CCastleGate();
	~CCastleGate();

	/**
	 *
	 */
	void	Init();

	/**
	 *
	 */
	void	AddGate( WORD wId, IW3DModel* pModel, const Vector3& vPos, const Quaternion& qRot );

	/**
	 *
	 */
	void	Cleanup();

	/**
	 *
	 */
	CastleGate*	GetGate( WORD wId );

	/**
	 *
	 */
	void	CloseGate( WORD wId );

	/**
	 *
	 */
	void	OpenGate( WORD wId );

	/**
	 *
	 */
	void	Update( const DWORD dwTick );

	/**
	 *
	 */
	void	Draw();

private:
	typedef std::map<WORD, CastleGate*>	GateMap;

	GateMap				m_mapGate;		///< Gate 리스트
	IW3DAnimationMgr*	m_pAnimation;	///< Animation Manager

	SAction*			m_pOpenAni;		///< 열리는 애니메이션 
										// 열리는 애니메이션이 끝나면 끝프레임 Loop
	SAction*			m_pCloseAni;	///< 닫혀 있는 애니메이션
										// 닫혀 있는 애니메이션은 전 프레임 Loop

	DWORD				m_dwTick;		///< 경과 Tick

	/**
	 *djE
	 */
	void		Update_OpenAni( CastleGate* pGate );

	/**
	 *
	 */
	void		Update_CloseAni( CastleGate* pGate );

}; //class CCastleGate

#endif //#ifndef __Castle_Gate_Header__