/**
 * 매직인터패이스 헤더
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-05-27 12:17a $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIMagicMgr.h $
 * 
 * 2     03-05-27 12:17a Paran
 * 
 * 9     03-04-30 7:11p Paran		스킬 선택 수정
 * 
 * 4     03-03-22 2:37a Paran		매직등록부분 수정... 다음은 리스트연동 ^^
 * <HR>
 *
 * @file	UIMagicMgr.h
 */
#ifndef __UIMagicMgr_Header__
#define __UIMagicMgr_Header__

#include "UInterface_Export.h"
#include <map>
#include <list>
#include <string>

using namespace std;

struct UIMagicData
{
	int				nIndex;
	int				nType;
	char*			szName;
	char*			szText;
	IW3DSprite*		pSelImage;
	IW3DSprite*		pScrollImage;
	IW3DSprite*		pListImage;
};

typedef map<int, UIMagicData>		MAP_MAGICDATA;
class CUIMagicMgr
{
public:
	BOOL				InsertMagic( int nMagicIndex, int nType, char* szName, char* szText,
								     IW3DSprite* psprSelImage, 
								     IW3DSprite* psprScrollImage, 
								     IW3DSprite* psprListImage );		///< 사용가능마법추가
						
	int					MagicShort();			///< 근거리 공격 (등록기본)
	int					MagicLong();			///< 원거리 공격 (등록기본)
	int					Magic1();				///< 백마법 공격 (등록된 매직이 백마법이 없다면 사용할 수 없다)
	int					Magic2();				///< 흑마법 공격 (등록된 매직이 흑마법이 없다면 사용할 수 없다)

	UIMagicData*		GetMagicShort();		///< 근거리 마법
	UIMagicData*		GetMagicLong();			///< 원거리 마법
	UIMagicData*		GetMagic1();			///< 흑마법
	UIMagicData*		GetMagic2();			///< 백마법
	
	int					GetMainSkillIndex();			///< 메인스킬 인덱스를 얻는다.

	UIMagicData*		FindMagic( int nMagic );

	BOOL				SetMagicShort( int nMagicIndex, BOOL bUse = TRUE );		///< 근거리 마법 등록
	BOOL				SetMagicLong( int nMagicIndex, BOOL bUse = TRUE );		///< 원거리 마법 등록
	BOOL				SetMagic1( int nMagicIndex, BOOL bUse = TRUE );			///< 1마법 등록
	BOOL				SetMagic2( int nMagicIndex, BOOL bUse = TRUE );			///< 2마법 등록

	int					GetUsePosIndex( int nNum );
	IW3DSprite*			GetUsedMagicImage( int nNum );				///< 등록한 마법들 이미지
	
	CUIMagicMgr();
	~CUIMagicMgr();

private:
	MAP_MAGICDATA			m_mapMagicData;
	UIMagicData*			m_usedMagic[4];
	list<int>				m_listSeq;
};

#endif