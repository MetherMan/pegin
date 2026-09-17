/**
 * 날으는 오브젝트 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-13 9:43p $
 * @version	$Revision: 20 $
 *
 * <HR>
 * $Log: /Engine/Src/FlyingObject.h $
 * 
 * 20    04-01-13 9:43p Kjmgo
 * 사양 설정 적용
 * 
 * 19    02-12-12 4:17p Sckyj
 * Sound 객체 변경
 * 
 * 18    03-12-20 6:21a Kjmgo
 * 메모리 관리
 * 
 * 17    03-12-20 3:33a Sckyj
 * 사운드 재생 거리(Max Script)를 스크립트로 처리함
 * 
 * 16    03-12-18 5:47p Kjmgo
 * 구조 변경
 * 
 * 15    03-12-17 11:01p Kjmgo
 * Map마다 따로 적용가능 하도록 수정 ( 'Map' ごとに 的用 )
 * 
 * 14    03-12-14 2:34p Kjmgo
 * 
 * 13    03-12-12 6:37p Sckyj
 * Sound 재생 기능 추가
 * 
 * 12    03-12-12 5:45a Kjmgo
 * 수정
 * 
 * 11    03-12-01 10:05a Kjmgo
 * 필요없는 코드 삭제, Scene 을 생성자에서 생성
 * 
 * 10    03-08-22 11:12a Kjmgo
 * 
 * 9     03-08-15 4:01a Kjmgo
 * <HR>
 *
 * @file	FlyingObject.h
 */


#ifndef __FlyingObject_Header__
#define __FlyingObject_Header__


#include <string>
#include <list>
#include <map>
#include <vector>


using std::string;
using std::list;
using std::map;
using std::vector;

#define MAX_GRID	50


/**
 * 날으는 오브젝트 데이터 구조체.
 * @ingroup		Engine
 */
struct SFlyingObject
{
	UNDER_MEMORY_MGR( SFlyingObject );

	int			nSpeed;			///< 움직이는 속도
	float		fRange;			///< 한번에 움직이는 최대 거리
	float		fHeight;		///< 높이

	IW3DModel*	pModel;			///< Model

	DWORD		dwTick;			///< 내부 누적 Tick
	DWORD		dwStopTime;		///< 멈춰 있을 시간

	Vector3		vPos;			///< 위치
	Vector3		vTarget;		///< 도착위치
	Quaternion	qRot;			///< 방향

	char		szSound[128];	///< 사운드 파일이름
	int			nLimit;			///< 사운드 파일 재생 Limit
	int			nDistance;		///< Max Distance;
	DWORD		dwSoundTick;	///< 사운드 tick
	IW3DSoundObject3D* pSound;	///< 사운드 객체	

	SFlyingObject()
	{
		nLimit		= 0;
		dwSoundTick = 0;
		nDistance	= 0;
		pSound		= NULL;
		ZeroMemory( szSound, sizeof( char ) * 128 );		
	} //SFlyingObject

	~SFlyingObject()
	{		
		SAFE_RELEASE( pSound );
	} //~SFlyingObject
}; //struct SFlyingObject


/**
 * 날으는 오브젝트 관리자 클래스.
 * @ingroup		Engine
 */
class CFlyingObject
{
public:
	UNDER_MEMORY_MGR( CFlyingObject );

	CFlyingObject();
	~CFlyingObject();

	/**
	 * 개임 월드를 설정한다.
	 */
	void	SetWorld( IW3DWorld* pWorld )
	{
		m_pWorld = pWorld;
	} //SetWorld

	BOOL	Load( const int nId, const char* szFile );
	BOOL	Add();

	void	SetQuality( int nQuality )
	{
		m_nQuality = nQuality;
	} //SetQuality 

	void	Update( const DWORD dwTick );

	void	Draw();
	void	DrawShadow();

private:
	typedef list<SFlyingObject*>			ObjectList;		///< 오브젝트 LIST
	typedef vector<SFlyingObject*>			ObjectArray;		///< 오브젝트 LIST
	typedef map<string, IW3DModel*>			ModelMap;		///< 이름-모델 B-TREE	
		
	
	int				m_nCount;
	int				m_nQuality;			///< 퀄리티
	IW3DScene*		m_pScene;
	ObjectList		m_listObject;		///< 오브젝트 리스트
	ObjectArray		m_aObject;			///< 오브젝트 리스트
	ModelMap		m_mapModel;			///< Model
	
	DWORD			m_dwTick;			///< 누적 Tick
	IW3DWorld*		m_pWorld;			///< WORLD

	IW3DModel*		LoadModel( const string& sFileName );
	SFlyingObject*	ReadObject( FILE* fp );	

}; //class CFlyingObject


#endif // #ifndef __FlyingObject_Header__
