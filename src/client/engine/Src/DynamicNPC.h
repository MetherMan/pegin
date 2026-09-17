/**
 * 메이
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 02-12-12 4:17p $
 * @version	$Revision: 8 $
 *
 * <HR>
 * $Log: /Engine/Src/DynamicNPC.h $
 * 
 * 8     02-12-12 4:17p Sckyj
 * Sound 객체 변경
 * 
 * 7     03-12-19 4:41a Sckyj
 * 사운드 재생 코드 수정( 들리는 Max_Distance를 script에서 지정할 수
 * 있도록 )
 * 
 * 6     03-12-16 3:48p Sckyj
 * Sound 재생 기능 추가
 * 
 * 5     03-12-11 3:42p Kjmgo
 * 그림자 추가
 * 
 * 4     03-12-11 3:10p Kjmgo
 * 말할 높이 지정
 * 
 * 3     03-12-09 11:28p Kjmgo
 * 말 하는거 대략 완성
 * 
 * 2     03-12-09 2:04a Kjmgo
 * World에서 Entity분리및, Map분리, 그리고 Map내부에, Dynamic NPC 추가
 * 
 * 1     03-12-08 10:18p Kjmgo
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
#include <list>

class CDynamicNPC;

struct SSound
{
	UNDER_MEMORY_MGR( SSound );	
		
	char			 szWave[128];///< 사운드 파일
	DWORD		     dwLimit;	///< 갱신 제한 시간	
	int				 nDistance;

	DWORD			 dwTime;

	IW3DSoundObject3D* pSound;	///< 사운드	

	SSound()
	{
		ZeroMemory( szWave, 128 );
		dwLimit = dwTime = nDistance = 0;
	}
}; //struct

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
	NPCList			m_listUpdateNPC;

	IW3DScene*		m_pScene;

	DWORD			m_dwTime;

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
	BOOL	LoadSound( char* szBuffer, SSound* sSound );


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

	/**
	 *
	 */
	void Update( const DWORD dwTick );

	/**
	 *
	 */
	void Draw( const DWORD dwTime );

private:

	typedef std::vector<Vector3>	 PathArray;
	typedef std::vector<std::string> SayArray;
	typedef std::list<SSound*>		 SoundList;


	IW3DScene*		m_pScene;			///< Scene
	PathArray		m_aPath;			///< Way Point
	IW3DModel*		m_pModel;			///< Model
	float			m_fSpeed;			///< Speed

	BOOL			m_bBack;			///< 다시 되돌아 오는 지의 여부

	int				m_nStart;			///< 시작 점
	int				m_nNow;				///< 현재 점

	BOOL			m_bShow;			///< 보일지의 여부

	Vector3			m_vPos;				///< 현재 위치
	Quaternion		m_qRot;
	Vector3			m_vGoal;			///< 목적지

	DWORD			m_dwSayTick;		///< 말하는 시간( 누적 틱 )
	char			m_szTalk[128];		///< 말할 내용이 담긴 버퍼
	int				m_nTalk;			///< 현재 리스트에 들어가 있는 말할 것 갯수
	SayArray		m_aSay;				///< 말할것 리스트
	
	float			m_fSayHeight;		///< 말할 높이 

	SoundList		m_aSound;			///< 사운드 리스트	

	/**
	 *
	 */
	void UpdateSay();

	/**
	 *
	 */
	void DrawSay();
}; //class CDynamicNPC

#endif //#ifndef __DynamicNPC_Header__