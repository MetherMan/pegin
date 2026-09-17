/**
 * Animation 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-08 10:12p $
 * @version	$Revision: 13 $
 *
 * <HR>
 * $Log: /Render/Src/Animation.cpp $
 * 
 * 13    04-01-08 10:12p Kjmgo
 * 
 * 12    04-01-08 6:18p Kjmgo
 * 
 * 11    04-01-08 10:49a Kjmgo
 * 
 * 10    04-01-08 9:48a Kjmgo
 * 코드 정리
 * 
 * <HR>
 *
 * @file	Animation.cpp
 */

#include "Render_Export.h"
#include "Animation.h"

// Macro
#define CHECK_KEYWORD( s )	!strcmp( szKeyword, s )

/*
 * 애니메이션 컴포넌트 클래스 구현부.
 * @ingroup		Render
 */
class CW3DAnimation : public IW3DAnimation
{
public:

	/**
	 * 생성자
	 */
	CW3DAnimation();

	/**
	 * 소멸자
	 */
	~CW3DAnimation();

	/**
	 *
	 */
	BOOL Load( const char* szFile );

	/**
	 *
	 */
	void* GetTMAnimation( const int iMesh );

	/** 
	 *
	 */
	void* GetBone( const int iBone );

	/**
	 *
	 */
	int	GetNumBone()
	{
		return m_numBone;
	} //GetNumBone

	/**
	 *
	 */
	int GetMaxFrame()
	{
		return m_numFrames; 
	} //GetMaxFrame

private:

	/////////////////////////////////////////////////////////
	// Transform Animation
	/////////////////////////////////////////////////////////
	int					m_numMeshs;			///< Mesh 갯수
	BOOL				m_bAnimation;		///< TM 애니메이션 사용 여부.

	SMesh*				m_apMesh;			///< TM 애니메이션 - 위치.
//	vector<SMesh>		m_aMesh;			///< TM 애니메이션 - 위치.

	/////////////////////////////////////////////////////////
	// Bone Animation
	/////////////////////////////////////////////////////////
	int					m_numFrames;		///< 총 프레임수
	int					m_numBone;
	BOOL				m_bHasBone;			///< Bone을 가지고 있는가?

	SBone*				m_apBone;			///< Bone Array
//	vector<SBone>		m_aBone;			///< Bone

private:

	/////////////////////////////////////////////////////////
	// Load Animation
	/////////////////////////////////////////////////////////

	/**
	 * Load Transform Animation
	 */
	void	LoadTMAnimation( Archive& ar, int nMesh );

	/**
	 * Load Bones
	 */
	void	LoadBones( Archive& ar );

	/**
	 * Load BoneAnimation
	 */
	void	LoadBoneAnimation( Archive& ar );


}; //class CW3DAnimation

IMPLEMENT_COMPONENT( CW3DAnimation, Animation );

/**
 * 생성자
 */
CW3DAnimation::CW3DAnimation()
:	m_numFrames			( 0 ),
	m_numMeshs			( 0 ),
	m_numBone			( 0 ),
	m_apBone			( NULL ),
	m_apMesh			( NULL ),
	m_bAnimation		( FALSE ),
	m_bHasBone			( FALSE )
{	

} //CW3DAnimation::CW3DAnimation

/**
 * 소멸자
 */
CW3DAnimation::~CW3DAnimation()
{
	SAFE_DELETE_ARRAY( m_apBone );
	SAFE_DELETE_ARRAY( m_apMesh );
} //CW3DAnimation::~CW3DAnimation

/**
 * Animation 파일을 로드한다.
 */
BOOL CW3DAnimation::Load( const char* szFile )
{
	FILE* fp = fopen( szFile, "rb" );
	
	if( !fp )
	{
		return FALSE;
	} //if

	ArchiveFileReader	ar( fp );

	char	szCode[32];
	ar.Serialize( &szCode, 32 );
	ar.Serialize( &m_numFrames, sizeof( int ) );

	////////////////////////////////////////////////
	// Read TM Anmation 
	////////////////////////////////////////////////

	ar.Serialize( &m_numMeshs, sizeof( int ) );

	// Mesh
	// Is Use Transfrom Animation
	if( m_numMeshs )
	{
		m_apMesh = new SMesh[m_numMeshs];
		CHECK_PTR_RETURN( m_apMesh );
		
		for( int i = 0 ; i < m_numMeshs ; i++ )
		{	
			LoadTMAnimation( ar, i );
		} //for
	} //if

	////////////////////////////////////////////////
	// Read Bone & Bone Animation 
	////////////////////////////////////////////////

	LoadBones( ar );
	LoadBoneAnimation( ar );

	ar.Close();

	return TRUE;
} //CW3DAnimation::Load

/**
 * TM 애니메이션을 읽어 온다.
 */
void CW3DAnimation::LoadTMAnimation( Archive& ar, int nMesh )
{
	int		iMesh;

	ar.Serialize( &iMesh, sizeof( int ) );
	Assert( iMesh >= 0 );
	
	m_apMesh[nMesh].bAnimation = TRUE;
	m_apMesh[nMesh].nMesh = iMesh;
	
	m_apMesh[nMesh].Alloc( m_numFrames, m_numFrames );

	ar.Serialize( m_apMesh[nMesh].apPos, m_numFrames * sizeof( Vector3 ) );
	ar.Serialize( m_apMesh[nMesh].apRot, m_numFrames * sizeof( Quaternion ) );
} //CW3DAnimation::LoadTMAnimation

/**
 *  Bone을 읽어 온다.
 */
void CW3DAnimation::LoadBones( Archive& ar )
{
	ar.Serialize( &m_numBone, sizeof( int ) );
	
	if( m_numBone <= 0 )
	{
		m_apBone = NULL;
		return ;
	} //if

	m_apBone = new SBone[m_numBone];
	
	if( !m_apBone )
	{
		return ;
	} //if

	SBone*		pBone = NULL;
	char		szName[32];
	int			nParent;
	Matrix4		matTM;
	Matrix4		matInverse;

	for( int iBone = 0 ; iBone < m_numBone ; iBone++ )
	{
		pBone = &m_apBone[iBone];
		
		ar.Serialize( &szName,	32 );
		ar.Serialize( &matTM,	sizeof( Matrix4 ) );
		ar.Serialize( &nParent,	sizeof( int ) );
		
		sprintf( pBone->szName, "%s", szName );
		pBone->matTM		= matTM;
		pBone->matInverse	= matTM.Inverse();
		pBone->nParent		= nParent - 1;		// 음수(-1)면 부모 없음.
		
		pBone->Alloc( m_numFrames, m_numFrames );
	} //for
} //CW3DAnimation::LoadBones

/**
 * Bone의 애니메이션을 읽어 온다.
 */
void CW3DAnimation::LoadBoneAnimation( Archive& ar )
{
	int		numBones = m_numBone;

	for( int iBone = 0 ; iBone < numBones ; iBone++ )
	{
		SBone*	pBone = &m_apBone[iBone];

		ar.Serialize( pBone->apPos, m_numFrames * sizeof( Vector3 ) );
		ar.Serialize( pBone->apRot, m_numFrames * sizeof( Quaternion ) );
	} //for
} //CW3DAnimation::LoadBoneAnimation

/**
 *
 */
void* CW3DAnimation::GetTMAnimation( const int iMesh )
{
	for( int i = 0 ; i < m_numMeshs ; i++ )
	{
		if( m_apMesh[i].nMesh == iMesh )
		{
			return &m_apMesh[i];
		} //if
	} //for
	
	return NULL;
} //CW3DAnimation::GetTMAnimation

/**
 *
 */
void* CW3DAnimation::GetBone( const int iBone )
{
	Assert( iBone < m_numBone );
	Assert( iBone >= 0 );
	return &m_apBone[iBone];
} //CW3DAnimation::GetBone
