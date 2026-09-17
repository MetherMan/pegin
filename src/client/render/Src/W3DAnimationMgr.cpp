/**
 * 애니메이션 관리자.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-08 2:01p $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /Render/Src/W3DAnimationMgr.cpp $
 * 
 * 4     04-01-08 2:01p Kjmgo
 * 코드 정리
 * 
 * <HR>
 *
 * @file	IW3DAnimationMgr.cpp
 */

#include "Render_Export.h"
#include <map>
#include <string>

using std::string;

/*
 * 애니메이션 관리자 컴포넌트 클래스 구현부.
 * @ingroup		Render
 */
class CW3DAnimationMgr : public IW3DAnimationMgr
{
	SINGLETON_COMPONENT( CW3DAnimationMgr );

public:

	/**
	 * 생성자
	 */
	CW3DAnimationMgr();

	/**
	 * 소멸자
	 */
	~CW3DAnimationMgr();

	/**
	 *
	 */
	SAction*	LoadAnimation( const char* szPath, const char* szFile );

private:

	typedef std::map<string, SAction*>	ActionMap;
	ActionMap		m_mapAction;			///< Action Map

}; //class CW3DAnimationMgr

IMPLEMENT_SINGLETON_COMPONENT( CW3DAnimationMgr, AnimationMgr );

/**
 *
 */
CW3DAnimationMgr::CW3DAnimationMgr()
{

} //CW3DAnimationMgr::CW3DAnimationMgr

/**
 *
 */
CW3DAnimationMgr::~CW3DAnimationMgr()
{
	ActionMap::iterator itor = m_mapAction.begin();
	ActionMap::iterator itorNext;

	while( itor != m_mapAction.end() )
	{
		itorNext = itor;
		itorNext++;

		SAction* pAction = itor->second;

		SAFE_RELEASE( pAction->pAnimation );
		SAFE_DELETE( pAction );

		m_mapAction.erase( itor );

		itor = itorNext;
	} //while

} //CW3DAnimationMgr::~CW3DAnimationMgr

/**
 *
 */
SAction* CW3DAnimationMgr::LoadAnimation( const char* szPath, const char* szFile )
{
	string sFile = szPath;
	sFile += "/";
	sFile += szFile;

	ActionMap::iterator itor = m_mapAction.find( sFile );

	if( itor != m_mapAction.end() )
	{ // 이미 있는 파일이라면
		return itor->second;
	} //if

	SAction*	pAction = new SAction;
	IW3DAnimation*	pAnimation = MAKE_COMPONENT( Animation );

	if( !pAnimation || !pAnimation->Load( sFile.c_str() ) )
	{ // 로드 실패
		SAFE_DELETE( pAction );
		SAFE_RELEASE( pAnimation );
		return NULL;
	} //if

	pAction->pAnimation = pAnimation;
	pAction->nStart		= 0;
	pAction->nEnd		= pAnimation->GetMaxFrame() - 1;
	pAction->nFrames	= pAction->nEnd - pAction->nStart + 1;

	m_mapAction.insert( std::make_pair( sFile, pAction ) );

	return pAction;
} //CW3DAnimationMgr::LoadAnimation

