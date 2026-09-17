/**
 * 메이
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-11 4:12a $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /GameClient/Src/DynamicNPC.h $
 * 
 * 2     03-12-11 4:12a Kjmgo
 * 
 * 1     03-12-08 10:18a Kjmgo
 * 
 * <HR>
 *
 * @file	DynamicNPC.h
 */

#ifndef __DynamicNPC_Header__
#define __DynamicNPC_Header__

#include <vector>


class CDynamicNPC;

/**
 *
 */
class CDynamicNPCMgr
{
public:
	MEMORY_UNDER_MGR( CDynamicNPCMgr );

	/**
	 *
	 */
	CDynamicNPCMgr();

	/**
	 *
	 */
	~CDynamicNPCMgr();

	/**
	 *
	 */
	void	Init( IW3DTerrain* pTerrain );

	/**
	 *
	 */
	BOOL	Load( const char* szFile );

	/**
	 *
	 */
	void	Update( DWORD dwTick );

	/**
	 *
	 */
	void	Draw();

	/**
	 *
	 */
	CDynamicNPC*	Pick();

private:
	typedef std::list<CDynamicNPC*>		NPCList;
	NPCList			m_listNPC;

}; //class CDynamicNPCMgr

/**
 *
 */
class CDynamicNPC
{
public:
	MEMORY_UNDER_MGR( CDynamicNPC );

	/**
	 *
	 */
	CDynamicNPC();

	/**
	 *
	 */
	~CDynamicNPC();

	/**
	 *
	 */
	BOOL	Load( const char* szFile );
	
	/**
	 *
	 */
	BOOL	LoadWaypoint( const char* szFile );


	/**
	 *
	 */
	void	SetShow( BOOL bShow )
	{
		m_bShow = bShow;
	} //SetShow

	/**
	 *
	 */
	Vector3&	GetPosition()
	{
		return m_vPos;
	} //GetPosition

private:
	typedef std::vector<Vector3>	PathArray;

	PathArray		m_aPath;			///< Way Point
	IW3DModel*		m_pModel;			///< Model
	float			m_fSpeed;			///< Speed

	BOOL			m_bBack;			///< 다시 되돌아 오는 지의 여부

	int				m_nStart;			///< 시작 점
	int				m_nNow;				///< 현재 점

	BOOL			m_bShow;			///< 보일지의 여부

	Vector3			m_vPos;				///< 현재 위치
}; //class CDynamicNPC

#endif //#ifndef __DynamicNPC_Header__