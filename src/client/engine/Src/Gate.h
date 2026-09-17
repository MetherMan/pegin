/**
 * Gate 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-03-03 10:10p $
 * @version	$Revision: 1 $
 *
 * <HR>
 * $Log: /Engine/Src/Gate.h $
 * 
 * 1     04-03-03 10:10p Kjmgo
 * 
 *
 * <HR>
 *
 * @file	Gate.h
 */

#ifndef __Gate_Header__
#define __Gate_Header__

#include <map>
#include <string>

/**
 *
 */
static DWORD	GetGateId( char* szName );

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
class CGate
{
public:
	/**
	 *
	 */
	CGate();

	/**
	 *
	 */
	~CGate();

	/**
	 *
	 */
	void	Init();

	/**
	 *
	 */
	BOOL	Add( DWORD dwId, IW3DModel* pModel, Vector3& vPos, Quaternion& qRot );

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

}; //class CGate



#endif //#ifndef __Gate_Header__