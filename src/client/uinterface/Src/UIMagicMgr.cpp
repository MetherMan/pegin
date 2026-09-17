/**
 * 매직인터패이스
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-05-27 12:17a $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIMagicMgr.cpp $
 * 
 * 2     03-05-27 12:17a Paran
 * 
 * 1     03-05-17 12:00a Paran
 * 
 * 14    03-05-14 10:39p Paran
 * 
 * 13    03-05-14 2:06a Paran
 * 
 * 12    03-05-13 3:01a Paran
 * 
 * 11    03-05-12 5:26a Paran
 * 
 * 10    03-04-29 12:29a Paran
 * 
 * 9     03-04-23 1:38p Paran
 * 
 * 8     03-03-28 6:56p Paran
 * 
 * 7     03-03-25 3:36a Paran
 * 인벤토리 아이템 서버와 연결 1단계 ^^a
 * 
 * 6     03-03-24 5:52p Paran
 * 
 * 5     03-03-22 5:29a Ahastudio
 *
 * 4     03-03-22 2:37a Paran		매직등록부분 수정... 다음은 리스트 연동 ^^
 * <HR>
 *
 * @file	UIMagicMgr.cpp
 */


#include "UIMagicMgr.h"


/**
 * 생성자.
 */
CUIMagicMgr::CUIMagicMgr()
{
	int		nCnt;
	for( nCnt = 0 ; nCnt < 4 ; nCnt++ )
	{
		m_usedMagic[nCnt] = NULL;
	} //for
	
	list<int>::iterator		iterList_Seq = m_listSeq.begin();
	m_listSeq.clear();
	for( nCnt = 0; nCnt < 4; nCnt++ )
	{
		m_listSeq.push_back( nCnt );
	}
} //CUIMagicMgr::CUIMagicMgr

//
// CUIMagicMgr::~CUIMagicMgr
//
CUIMagicMgr::~CUIMagicMgr()
{
} //CUIMagicMgr::~CUIMagicMgr

//
// CUIMagicMgr::MagicShort
//
int CUIMagicMgr::MagicShort()
{
	if( !m_usedMagic[0] )
	{
		return 0;
	}
	
	list<int>::iterator		iterList_Seq = m_listSeq.begin();
	if( (*iterList_Seq) == 0 )
	{
		return 0;
	}
	iterList_Seq = m_listSeq.begin();
	m_listSeq.clear();
	for( int nCnt = 0; nCnt < 4; nCnt++ )
	{
		m_listSeq.push_back( nCnt );
	}
	
	return m_usedMagic[0]->nIndex;
} //CUIMagicMgr::MagicShort

//
// CUIMagicMgr::MagicLong
//
int CUIMagicMgr::MagicLong()
{
	if( !m_usedMagic[1] )
	{
		return 0;
	}
	
	list<int>::iterator		iterList_Seq = m_listSeq.begin();
	if( (*iterList_Seq) == 1 )
	{
		return 0;
	}
	
	m_listSeq.clear();
	for( int nCnt = 0; nCnt < 4; nCnt++ )
	{
		m_listSeq.push_back( nCnt );
	}
	iterList_Seq = m_listSeq.begin();
	advance( iterList_Seq, 1 );
	m_listSeq.erase( iterList_Seq );
	m_listSeq.push_front( 1 );

	return m_usedMagic[1]->nIndex;
} //CUIMagicMgr::MagicLong

//
// CUIMagicMgr::Magic1
//
int CUIMagicMgr::Magic1()
{
	if( !m_usedMagic[2] )
	{
		return 0;
	}

	list<int>::iterator		iterList_Seq = m_listSeq.begin();
	if( (*iterList_Seq) == 2 )
	{
		return 0;
	}

	m_listSeq.clear();
	for( int nCnt = 0; nCnt < 4; nCnt++ )
	{
		m_listSeq.push_back( nCnt );
	}
	iterList_Seq = m_listSeq.begin();	
	advance( iterList_Seq, 2 );
	m_listSeq.erase( iterList_Seq );
	m_listSeq.push_front( 2 );

	return m_usedMagic[2]->nIndex;
} //CUIMagicMgr::Magic1

//
// CUIMagicMgr::Magic2
//
int CUIMagicMgr::Magic2()
{
	if( !m_usedMagic[3] )
	{
		return 0;
	}

	list<int>::iterator		iterList_Seq = m_listSeq.begin();
	if( (*iterList_Seq) == 3 )
	{
		return 0;
	}
	
	m_listSeq.clear();
	for( int nCnt = 0; nCnt < 4; nCnt++ )
	{
		m_listSeq.push_back( nCnt );
	}
	iterList_Seq = m_listSeq.begin();
	advance( iterList_Seq, 3 );
	m_listSeq.erase( iterList_Seq );
	m_listSeq.push_front( 3 );

	return m_usedMagic[3]->nIndex;
} //CUIMagicMgr::Magic2

//
// CUIMagicMgr::GetMagicShort
//
UIMagicData* CUIMagicMgr::GetMagicShort()
{
	return m_usedMagic[0];
} //CUIMagicMgr::GetMagicShort

//
// CUIMagicMgr::GetMagicLong
//
UIMagicData* CUIMagicMgr::GetMagicLong()
{
	return m_usedMagic[1];
} //CUIMagicMgr::GetMagicLong

//
// CUIMagicMgr::GetMagicBlack
//
UIMagicData* CUIMagicMgr::GetMagic1()
{
	return m_usedMagic[2];
} //CUIMagicMgr::GetMagicBlack

//
// CUIMagicMgr::GetMagicWhite
//
UIMagicData* CUIMagicMgr::GetMagic2()
{
	return m_usedMagic[3];
} //CUIMagicMgr::GetMagicWhite

/**
 * 메인스킬 인덱스를 얻는다.
 */
int CUIMagicMgr::GetMainSkillIndex()
{
	list<int>::iterator		iterList_Seq = m_listSeq.begin();
	int		nSkillIndex = (*iterList_Seq);
	return nSkillIndex;
} //CUIMagicMgr::GetMainSkillIndex

//
// CUIMagicMgr::FindMagic
//
UIMagicData* CUIMagicMgr::FindMagic( int nMagic )
{
	MAP_MAGICDATA::iterator		iterMap_MagicDat = m_mapMagicData.find( nMagic );
	if( iterMap_MagicDat == m_mapMagicData.end() )
	{
		return NULL;
	}
	return &iterMap_MagicDat->second;
} //CUIMagicMgr::FindMagic

//
// CUIMagicMgr::SetMagicShort
//
BOOL CUIMagicMgr::SetMagicShort( int nMagicIndex, BOOL bUse )
{
	if( !bUse )
	{
		m_usedMagic[0] = NULL;
		return TRUE;
	}

	MAP_MAGICDATA::iterator		iterMap_MagicDat = m_mapMagicData.find( nMagicIndex );
	if( iterMap_MagicDat == m_mapMagicData.end() )
	{
		Assert( !"등록할 수 없는 매직" );
		return FALSE;
	}
	m_usedMagic[0] = &iterMap_MagicDat->second;
	return TRUE;
} //CUIMagicMgr::SetMagicShort

//
// CUIMagicMgr::SetMagicLong
//
BOOL CUIMagicMgr::SetMagicLong( int nMagicIndex, BOOL bUse )
{
	if( !bUse )
	{
		m_usedMagic[1] = NULL;
		return TRUE;
	}

	MAP_MAGICDATA::iterator		iterMap_MagicDat = m_mapMagicData.find( nMagicIndex );
	if( iterMap_MagicDat == m_mapMagicData.end() )
	{
		Assert( !"등록할 수 없는 매직" );
		return FALSE;
	}
	m_usedMagic[1] = &iterMap_MagicDat->second;
	return TRUE;
} //CUIMagicMgr::SetMagicLong

//
// CUIMagicMgr::SetMagic1
//
BOOL CUIMagicMgr::SetMagic1( int nMagicIndex, BOOL bUse )
{
	if( !bUse )
	{
		m_usedMagic[2] = NULL;
		return TRUE;
	}

	MAP_MAGICDATA::iterator		iterMap_MagicDat = m_mapMagicData.find( nMagicIndex );
	if( iterMap_MagicDat == m_mapMagicData.end() )
	{
		Assert( !"등록할 수 없는 매직" );
		return FALSE;
	}
	
	UIMagicData*		pMagicDat = &iterMap_MagicDat->second; 
	m_usedMagic[2] = pMagicDat;
	return TRUE;
} //CUIMagicMgr::SetMagic1

//
// CUIMagicMgr::SetMagic2
//
BOOL CUIMagicMgr::SetMagic2( int nMagicIndex, BOOL bUse )
{
	if( !bUse )
	{
		m_usedMagic[3] = NULL;
		return TRUE;
	}

	MAP_MAGICDATA::iterator		iterMap_MagicDat = m_mapMagicData.find( nMagicIndex );
	if( iterMap_MagicDat == m_mapMagicData.end() )
	{
		Assert( !"등록할 수 없는 매직" );
		return FALSE;
	}
	
	UIMagicData*		pMagicDat = &iterMap_MagicDat->second; 
	m_usedMagic[3] = pMagicDat;
	return TRUE;
} //CUIMagicMgr::SetMagic2

//
// CUIMagicMgr::InsertMagic
//
BOOL CUIMagicMgr::InsertMagic( int nMagicIndex, int nType, char* szName, char* szText,
							   IW3DSprite* psprSelImage, 
							   IW3DSprite* psprScrollImage, 
							   IW3DSprite* psprListImage )
{
	MAP_MAGICDATA::iterator		iterMap_MagicDat = m_mapMagicData.find( nMagicIndex );
	if( iterMap_MagicDat != m_mapMagicData.end() )
	{
		Assert( !"이미있는듯" );
		return FALSE;
	}

	UIMagicData		magicDat;
	memset( &magicDat, 0, sizeof( UIMagicData ) );

	try
	{
		magicDat.pSelImage		= psprSelImage;
		magicDat.pScrollImage	= psprScrollImage;
		magicDat.pListImage		= psprListImage;

		magicDat.nIndex = nMagicIndex;
		magicDat.nType	= nType;
		magicDat.szName = szName;
		magicDat.szText = szText;
		m_mapMagicData.insert( make_pair( nMagicIndex, magicDat ) );
	}
	catch( std::bad_alloc )
	{
		Assert( !"이미지 생성 실패" );
		throw W3DException( "메모리 할당 실패" );
		return FALSE;
	}
	catch( W3DException& )
	{
		throw;
		return FALSE;
	} //try..catch..

	return TRUE;
} //CUIMagicMgr::InsertMagic

//
// CUIMagicMgr::GetUsedMagicImage
//
IW3DSprite* CUIMagicMgr::GetUsedMagicImage( int nNum )
{
	if( m_listSeq.empty() )
	{
		return NULL;
	} //if
	
	int				nIndex;
	IW3DSprite*		pImage;
	UIMagicData*	pMagicDat;

	list<int>::iterator		iterList_Seq = m_listSeq.begin();
	advance( iterList_Seq, nNum );
	nIndex = *iterList_Seq;
	pMagicDat = m_usedMagic[nIndex];

	if( !pMagicDat )
	{
		return NULL;
	}

	if( 0 == nNum )
	{
		pImage = m_usedMagic[nIndex]->pSelImage;
	}
	else
	{
		pImage = m_usedMagic[nIndex]->pScrollImage;
	}

	return pImage;
} //CUIMagicMgr::GetUsedMagicImage

/**
 * 현재 사용중인 마법 위치 인덱스
 */
int CUIMagicMgr::GetUsePosIndex( int nNum )
{
	if( m_listSeq.empty() )
	{
		return -1;
	} //if
	
	int				nIndex;

	list<int>::iterator		iterList_Seq = m_listSeq.begin();
	advance( iterList_Seq, nNum );
	nIndex = *iterList_Seq;

	return nIndex;
}