/**
 * 몬스터 정보 관리자 인터페이스 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-04-04 4:59a $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /Engine/Inc/IW3DMonsterInfo.h $
 * 
 * 4     03-04-04 4:59a Ahastudio
 * <HR>
 *
 * @file	IW3DMonsterInfo.h
 */


#ifndef __Wind3D_Engine_IW3DMonsterInfo_Header__
#define __Wind3D_Engine_IW3DMonsterInfo_Header__


/**
 * 몬스터 정보 관리자 인터페이스.
 * 한꺼번에 정보 파일을 읽고, 몬스터 종류 Index로 접근해서 정보를 얻을 수 있다.
 * @ingroup		Engine
 */
class IW3DMonsterInfo : public IComponent
{
public:
	/**
	 * 인포파일을 로드합니다.
	 *
	 * @param	szFileName		[IN] 인포 파일(e.g. monsters.dat)
	 *
	 * @return	로드 성공/실패
	 */
	virtual BOOL			Load( const char *szFileName ) = 0;

	/**
	 * 원하는 몬스터 정보를 열어줍니다.
	 *
	 * @param	nIndex			[IN] 몬스터 인덱스
	 *
	 * @return	오픈 성공/실패
	 */
	virtual BOOL			OpenMonsterInfo( int nIndex ) = 0;

	/**
	 * 현재 열려 있는 몬스터 이름을 얻는다.
	 *
	 * @return	몬스터이름/NULL
	 */
	virtual char*			GetName() = 0;

	/**
	 * 현재 열려 있는 몬스터 영어 이름을 얻는다.
	 *
	 * @return	몬스터영어이름/NULL
	 */
	virtual char*			GetEngName() = 0;

	/**
	 * 현재 열려 있는 몬스터의 데미지를 얻는다.
	 *
	 * @return	Damage/-1
	 */
	virtual int				GetDamage() = 0;
}; //class IW3DMonsterInfo


#endif // #ifndef __Wind3D_Engine_IW3DMonsterInfo_Header__
