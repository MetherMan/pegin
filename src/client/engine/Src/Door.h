/**
 * Door
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-03-10 6:17p $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /Engine/Src/Door.h $
 * 
 * 6     04-03-10 6:17p Paran
 * 
 * 5     03-08-22 11:22a Kjmgo
 * 
 * 4     03-07-18 12:07p Kjmgo
 * 
 * 3     03-07-18 9:36a Kjmgo
 * 
 * 2     03-07-13 3:58p Kjmgo
 *
 * <HR>
 *
 * @file	Door.h
 */


#ifndef __Door_Header__
#define __Door_Header__

#include <map>
#include <list>
#include <string>

using std::string;
using std::map;

/**
 *
 */
class CDoor
{
public:
	UNDER_MEMORY_MGR( CDoor );


	CDoor();
	~CDoor();

	BOOL Load( const char* szName );

	BOOL Pick( const Vector3& vPick, const Vector3& vOrig, const Vector3& vDir );

	/**
	 *
	 */
	DWORD GetDoor()
	{
		if( m_pPick )
		{
			return m_pPick->dwId + 1;
		} //if
		
		return 0;
	} //GetDoor

	void Update( DWORD dwTick );
	void Draw();

private:
	struct SDoor
	{
		UNDER_MEMORY_MGR( SDoor );

		DWORD dwId;
		DWORD dwMoveMap;
		BYTE  btType;
		BOOL  bView;

		DWORD dwTick;

		float fX;
		float fY;

		IW3DModel* m_pModel;
	
		Vector3			vPos;			///< 위치
		Quaternion		qRot;			///< 방향
	}; //struct SDoor

	typedef map<string, IW3DModel*>			ModelMap;
	typedef map<DWORD, SDoor*>				DoorMap;

	ModelMap		m_mapModel;
	DoorMap			m_mapDoor;

	DWORD			m_dwTick;
	SDoor*			m_pPick;

	IW3DScene*		m_pScene;
	//-=-=-=-=-=-= Load -=-=-=-=-=-//
	IW3DModel*		LoadModel( const char* szName );

	
}; //class CDoor

#endif //__Door_Header__