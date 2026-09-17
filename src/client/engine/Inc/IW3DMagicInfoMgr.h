/**
 * 마법 정보 관리 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-04-04 4:58a $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /Engine/Inc/IW3DMagicInfoMgr.h $
 * 
 * 6     03-04-04 4:58a Ahastudio
 *
 * 5     03-03-22 2:37a Paran		매직 등록 부분 수정.
 * <HR>
 *
 * @file	IW3DMagicInfoMgr.h
 */


#ifndef __Wind3D_Engine_IW3DMagicInfoMgr_Header__
#define __Wind3D_Engine_IW3DMagicInfoMgr_Header__


/**
 * 마법 정보 관리자.
 * @ingroup		Engine
 */
class IW3DMagicInfoMgr : public IComponent
{
public:
	enum { WHITE_MAGIC = 0, BLACK_MAGIC = 1, NUM_MAGIC = 2 };

	/**
	 * 마법 정보 파일을 읽는다.
	 */
	virtual BOOL		Load( const char* szFileName ) = 0;

	/**
	 * 얻고자 하는 마법 정보를 선택한다.
	 */
	virtual BOOL		OpenMagic( int nIndex ) = 0;

	/**
	 * 이름
	 */
	virtual char*		GetName()	   const = 0;
	
	/**
	 * 종류
	 */
	virtual int			GetType()	   const = 0;

	/**
	 * 텍스트
	 */
	virtual char*		GetText()	   const = 0;	
	/**
	 * 리스트용 이미지를 얻는다.
	 */
	virtual char*		ImgFileName1() const = 0;

	/**
	 * 작은 창 이미지를 얻는다.
	 */
	virtual char*		ImgFileName2() const = 0;

	/**
	 * 큰 창 이미지를 얻는다.
	 */
	virtual char*		ImgFileName3() const = 0;
}; //class IW3DMagicInfoMgr


#endif // #ifndef __Wind3D_Engine_IW3DMagicInfoMgr_Header__
