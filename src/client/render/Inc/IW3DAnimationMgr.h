/**
 * 애니메이션 관리자.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-08-29 9:19a $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DAnimationMgr.h $
 * 
 * 2     03-08-29 9:19a Kjmgo
 * Animation 바꿈
 * 
 * <HR>
 *
 * @file	IW3DAnimationMgr.h
 */


#ifndef __Wind3D_Render_IW3DAnimationMgr_Header__
#define __Wind3D_Render_IW3DAnimationMgr_Header__

class IW3DAnimation;

/**
 *
 */
struct SAction
{
	int					nStart;		///< 시작
	int					nEnd;		///< 끝	
	int					nFrames;	///< 전체 프레임( 끝 - 시작 )
	IW3DAnimation*		pAnimation; ///< Animation 포인터 
}; //struct SAction


/**
 * 애니메이션 클래스.
 * @ingroup		Render
 */
class IW3DAnimation : public IComponent
{
public:
	/**
	 *
	 */
	virtual BOOL Load( const char* szFile ) = 0;

	/**
	 *
	 */
	virtual void* GetTMAnimation( const int iMesh ) = 0;

	/**
	 *
	 */
	virtual void* GetBone( const int iBone ) = 0;

	/**
	 *
	 */
	virtual int	GetNumBone() = 0;

	/**
	 *
	 */
	virtual int GetMaxFrame() = 0;


}; //class IW3DAnimation

/**
 * 애니메이션 관리자 클래스.
 * @ingroup		Render
 */
class IW3DAnimationMgr : public IComponent
{
public:

	/**
	 *
	 */
	virtual SAction*	LoadAnimation( const char* szPath, const char* szFile ) = 0;

}; //class IW3DAnimationMgr

#endif //#ifndef __Wind3D_Render_IW3DAnimationMgr_Header__
