/**
 * 캐릭터 선택용 GUI.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-20 7:56a $
 * @version	$Revision: 25 $
 *
 * <HR>
 * $Log: /UInterface/Src/UICharSelect.cpp $
 * 
 * 25    03-12-20 7:56a Paran
 * 
 * 24    03-12-18 11:27p Paran
 * 
 * 23    03-12-18 2:58a Sk8snow2
 * 
 * 22    03-12-18 2:05a Paran
 * 
 * 21    03-12-16 5:05p Paran
 * 
 * 20    03-12-12 11:58a Paran
 * 
 * 19    03-12-09 5:58p Paran
 * 아이템 데이터 수정
 * 
 * 18    03-10-07 9:07p Paran
 * 
 * 17    03-10-02 1:53a Paran
 * 
 * 16    03-10-02 1:02a Paran
 * 
 * 15    03-10-01 8:59p Paran
 * 
 * 14    03-10-01 2:05a Paran
 * 
 * 13    03-09-09 4:11p Paran
 * 
 * 12    03-09-07 12:42p Paran
 * 
 * 11    03-08-21 11:29p Paran
 * 
 * 10    03-08-19 11:14a Paran
 * 
 * 9     03-08-15 5:16a Paran
 * 
 * 8     03-08-14 3:59a Paran
 * 
 * 7     03-07-18 1:56p Paran
 * 
 * 6     03-07-17 9:55p Paran
 * 
 * 4     03-06-18 10:16p Paran		인벤토리 손 바꾸기, 캐릭터 선택 창 수정
 * 
 * <HR>
 *
 * @file	UICharSelect.cpp
 */


#include "UInterface_Export.h"
#include "UICharSelect.h"


/**
 * 생성자.
 */
CCharSelectInfo::CCharSelectInfo()
:	m_nIndex		( 0 ),
	m_nHairIndex	( 0 ),
	m_nFaceIndex	( 0 ),
	m_nCoatIndex	( 0 ),
	m_nPantsIndex	( 0 )
{
	m_vecDefine_Hair.push_back( 10085 );
	m_vecDefine_Hair.push_back( 10086 );
	m_vecDefine_Hair.push_back( 10087 );
	m_vecDefine_Hair.push_back( 10088 );
	m_vecDefine_Hair.push_back( 10089 );
	m_vecDefine_Hair.push_back( 10090 );

	m_vecDefine_Face.push_back( 10081 );
	m_vecDefine_Face.push_back( 10082 );
	m_vecDefine_Face.push_back( 10083 );
	m_vecDefine_Face.push_back( 10084 );

	m_vecDefine_Coat.push_back( 3970 );
	m_vecDefine_Coat.push_back( 3991 );
	m_vecDefine_Coat.push_back( 4012 );
	
	m_vecDefine_Pants.push_back( 5986 );
	m_vecDefine_Pants.push_back( 6007 );
	m_vecDefine_Pants.push_back( 6028 );

	m_vecDefine_Hands.push_back( 4978 );
	m_vecDefine_Hands.push_back( 4999 );
	m_vecDefine_Hands.push_back( 5020 );

	m_vecDefine_Foots.push_back( 6994 );
	m_vecDefine_Foots.push_back( 7015 );
	m_vecDefine_Foots.push_back( 7036 );
	
	Cleanup();
} //CCharSelectInfo::CCharSelectInfo

/**
 * 소멸자.
 */
CCharSelectInfo::~CCharSelectInfo()
{
} //CCharSelectInfo::~CCharSelectInfo

/**
 * 모든 캐릭터 정보를 삭제한다.
 */
void CCharSelectInfo::Cleanup()
{
	for( int nCnt = 0 ; nCnt < MAX_NUMCHARACTER ; nCnt++ )
	{
		strcpy( m_CharInfo[nCnt].szName, "" );
		m_CharInfo[nCnt].bEmpty		= TRUE;
		m_CharInfo[nCnt].nSTR		= 10;
		m_CharInfo[nCnt].nDEX		= 10;
		m_CharInfo[nCnt].nINT		= 10;
		m_CharInfo[nCnt].btSex		= 0;
		m_CharInfo[nCnt].wHair		= m_vecDefine_Hair[0];
		m_CharInfo[nCnt].wFace		= m_vecDefine_Face[0];
		m_CharInfo[nCnt].wCoat		= m_vecDefine_Coat[0];
		m_CharInfo[nCnt].wPants		= m_vecDefine_Pants[0];
		m_CharInfo[nCnt].wHand		= m_vecDefine_Hands[0];
		m_CharInfo[nCnt].wFoot		= m_vecDefine_Foots[0];
		m_CharInfo[nCnt].nLevel		= 0;
		m_CharInfo[nCnt].nLLevel	= 0;
		m_CharInfo[nCnt].nSLevel	= 0;
		m_CharInfo[nCnt].nBLevel	= 0;
		m_CharInfo[nCnt].nWLevel	= 0;
		m_CharInfo[nCnt].nClass   = 0;
	} //for
	
	m_nHairIndex	= 0;
	m_nFaceIndex	= 0;
	m_nCoatIndex	= 0;
	m_nPantsIndex	= 0;
} //CCharSelectInfo::Cleanup

/**
 * 현재 캐릭터 정보를 얻는다.
 *
 * @return	CharInfo 포인터
 */
SCharInfo* CCharSelectInfo::GetCharInfo()
{
	return &m_CharInfo[m_nIndex];
} //CCharSelectInfo::GetCharInfo

/**
 * 캐릭터 정보를 설정한다.
 */
void CCharSelectInfo::SetCharInfo(
	const char*		szName,		///< [IN] 이름
	const int		nLevel,
	const int		nLLevel,
	const int		nSLevel,
	const int		nBLevel,
	const int		nWLevel,
	const BYTE		btSex,		///< [IN] 성별
	const WORD		wHair,		///< [IN] 머리 스타일
	const WORD		wFace,		///< [IN] 얼굴 모양
	const WORD		wCoat,		///< [IN] 윗옷
	const WORD		wPants,		///< [IN] 바지
	const WORD		wHand,		///< [IN] 손
	const WORD		wFoot,		///< [IN] 발
	const BYTE		btClass		///< [IN] 국적
	)
{
	strcpy( m_CharInfo[m_nIndex].szName, szName );
	m_CharInfo[m_nIndex].bEmpty		= FALSE;
	m_CharInfo[m_nIndex].btSex		= btSex;
	m_CharInfo[m_nIndex].nLevel		= nLevel;
	m_CharInfo[m_nIndex].nLLevel	= nLLevel;
	m_CharInfo[m_nIndex].nSLevel	= nSLevel;
	m_CharInfo[m_nIndex].nBLevel	= nBLevel;
	m_CharInfo[m_nIndex].nWLevel	= nWLevel;
	m_CharInfo[m_nIndex].wHair		= wHair;
	m_CharInfo[m_nIndex].wFace		= wFace;
	m_CharInfo[m_nIndex].wCoat		= wCoat;
	m_CharInfo[m_nIndex].wPants		= wPants;
	m_CharInfo[m_nIndex].wFoot		= wFoot;
	m_CharInfo[m_nIndex].wHand		= wHand;
	m_CharInfo[m_nIndex].nClass		= btClass;
	
	if( !m_CharInfo[m_nIndex].wCoat )
	{
		m_CharInfo[m_nIndex].wCoat = 10092;
	} //if
	if( !m_CharInfo[m_nIndex].wPants )
	{
		m_CharInfo[m_nIndex].wPants = 10093;
	} //if
	if( !m_CharInfo[m_nIndex].wHand )
	{
		m_CharInfo[m_nIndex].wHand = 10091;
	} //if
	if( !m_CharInfo[m_nIndex].wFoot )
	{
		m_CharInfo[m_nIndex].wFoot = 10094;
	} //if
} //CCharSelectInfo::SetCharInfo

/**
 * 캐릭터 정보를 삭제한다.
 */
void CCharSelectInfo::DeleteCharInfo( const int nIndex )
{
	if( nIndex >= 0 && nIndex < MAX_NUMCHARACTER )
	{
		strcpy( m_CharInfo[nIndex].szName, "" );
		m_CharInfo[nIndex].bEmpty	= TRUE;
		m_CharInfo[nIndex].btSex	= 0;
		m_CharInfo[nIndex].wHair	= m_vecDefine_Hair[0];
		m_CharInfo[nIndex].wFace	= m_vecDefine_Face[0];
		m_CharInfo[nIndex].wCoat	= m_vecDefine_Coat[0];
		m_CharInfo[nIndex].wPants	= m_vecDefine_Pants[0];
		m_CharInfo[nIndex].wHand	= m_vecDefine_Hands[0];
		m_CharInfo[nIndex].wFoot	= m_vecDefine_Foots[0];
		m_CharInfo[nIndex].nLevel = 0;
		m_CharInfo[nIndex].nLLevel = 0;
		m_CharInfo[nIndex].nSLevel = 0;
		m_CharInfo[nIndex].nBLevel = 0;
		m_CharInfo[nIndex].nWLevel = 0;
	} //if
} //CCharSelectInfo::DeleteCharInfo

/**
 * 현재 캐릭터 정보 삭제
 */
BOOL CCharSelectInfo::IsFull()
{
	for( int nCnt = 0 ; nCnt < MAX_NUMCHARACTER ; nCnt++ )
	{
		if( m_CharInfo[nCnt].bEmpty )
		{
			return FALSE;
		} //if
	} //for

	return TRUE;
} //CCharSelectInfo::IsFull

/**
 * 캐릭터 이름설정
 */
void CCharSelectInfo::SetName( const char* szName )
{
	strcpy( m_CharInfo[m_nIndex].szName, szName );
} //CCharSelectInfo::SetName

/**
 * 이전 캐릭터 정보선택
 */
void CCharSelectInfo::Previous()
{
	m_nIndex--;

	if( m_nIndex > MAX_NUMCHARACTER )
	{
		m_nIndex = 0;
	}
	else if( m_nIndex < 0 )
	{
		m_nIndex = MAX_NUMCHARACTER;
	} //if..else if..
} //CCharSelectInfo::Previous

/**
 * 다음 캐릭터 정보선택
 */
void CCharSelectInfo::Next()
{
	m_nIndex++;

	if( m_nIndex > MAX_NUMCHARACTER )
	{
		m_nIndex = 0;
	}
	else if( m_nIndex < 0 )
	{
		m_nIndex = MAX_NUMCHARACTER;
	} //if..else if..
} //CCharSelectInfo::Next

/**
 * 생성 캐릭터 공간 찾기
 */
int CCharSelectInfo::New()
{
	for( int nCnt = 0 ; nCnt < MAX_NUMCHARACTER ; nCnt++ )
	{
		if( m_CharInfo[nCnt].bEmpty )
		{
			m_nIndex = nCnt;
			return m_nIndex;
		} //if
	} //for

	return -1;
} //CCharSelectInfo::New

/**
 * 생성 중인 캐릭터 정보 닫기
 */
void CCharSelectInfo::Save()
{
	m_CharInfo[m_nIndex].bEmpty = FALSE;
} //CCharSelectInfo::Save

/**
 * 작업 배열 인덱스 설정
 */
BOOL CCharSelectInfo::SetIndex( int nIndex )
{
	m_nIndex = nIndex;

	if( m_nIndex > MAX_NUMCHARACTER )
	{
		m_nIndex = 0;
		return FALSE;
	}
	else if( m_nIndex < 0 )
	{
		m_nIndex = MAX_NUMCHARACTER;
		return FALSE;
	} //if..else if..

	return TRUE;
} //CCharSelectInfo::SetIndex

/**
 * 작업 배열 인덱스
 */
int CCharSelectInfo::GetIndex()
{
	return m_nIndex;
} //CCharSelectInfo::GetIndex


/**
 * HP를 얻는다.
 */
int CCharSelectInfo::GetHP()
{
	int		nHP = ( m_CharInfo[m_nIndex].nSTR - 9 ) * 7 * 1.5 + 
				  ( ( m_CharInfo[m_nIndex].nDEX - 9 ) * 3 ) + 
				  ( m_CharInfo[m_nIndex].nINT - 9 ) * 9;

	return nHP;
} //CCharSelectInfo::GetHP

/**
 * MP를 얻는다.
 */
int CCharSelectInfo::GetMP()
{
	int		nMP = ( m_CharInfo[m_nIndex].nSTR - 9 ) + 
				  ( m_CharInfo[m_nIndex].nDEX - 9 ) * 3 + 
				  ( m_CharInfo[m_nIndex].nINT - 9 ) * 9 * 1.5;

	return nMP;
} //CCharSelectInfo::GetMP

/**
 * 현재 포인트를 초기화.
 */
void CCharSelectInfo::InitPOINT()
{
	m_CharInfo[m_nIndex].nSTR = 10;
	m_CharInfo[m_nIndex].nDEX = 10;
	m_CharInfo[m_nIndex].nINT = 10;	
} //CCharSelectInfo::InitPOINT

/**
 * 현재 포이트를 얻는다.
 */
int CCharSelectInfo::GetPOINT()
{
	int		nTotal = m_CharInfo[m_nIndex].nSTR + 
					 m_CharInfo[m_nIndex].nDEX + 
					 m_CharInfo[m_nIndex].nINT;
	return 35 - nTotal;
} //CCharSelectInfo::GetPOINT

/**
 * 현재 STR를 얻는다.
 */
int CCharSelectInfo::GetSTR()
{
	return m_CharInfo[m_nIndex].nSTR;
} //CCharSelectInfo::GetSTR

/**
 * STR 하나 더.
 */
void CCharSelectInfo::Plus_STR()
{
	if( GetPOINT() <= 0 )		return;

	m_CharInfo[m_nIndex].nSTR++;
} //CCharSelectInfo::Plus_STR

/**
 * STR 하나 빼기.
 */
void CCharSelectInfo::Minus_STR()
{
	if( m_CharInfo[m_nIndex].nSTR <= 10 )		return;

	m_CharInfo[m_nIndex].nSTR--;
} //CCharSelectInfo::Minus_STR

/**
 * 현재 DEX를 얻는다.
 */
int CCharSelectInfo::GetDEX()
{
	return m_CharInfo[m_nIndex].nDEX;
} //CCharSelectInfo::GetDEX

/**
 * DEX 하나 더.
 */
void CCharSelectInfo::Plus_DEX()
{
	if( GetPOINT() <= 0 )		return;

	m_CharInfo[m_nIndex].nDEX++;
} //CCharSelectInfo::Plus_DEX

/**
 * DEX 하나 빼기.
 */
void CCharSelectInfo::Minus_DEX()
{
	if( m_CharInfo[m_nIndex].nDEX <= 10 )		return;

	m_CharInfo[m_nIndex].nDEX--;
} //CCharSelectInfo::Minus_DEX

/**
 * 현재 INT를 얻는다.
 */
int CCharSelectInfo::GetINT()
{
	return m_CharInfo[m_nIndex].nINT;
} //CCharSelectInfo::GetINT

/**
 * INT 하나 더.
 */
void CCharSelectInfo::Plus_INT()
{
	if( GetPOINT() <= 0 )		return;

	m_CharInfo[m_nIndex].nINT++;
} //CCharSelectInfo::Plus_INT

/**
 * INT 하나 빼기.
 */
void CCharSelectInfo::Minus_INT()
{
	if( m_CharInfo[m_nIndex].nINT <= 10 )		return;

	m_CharInfo[m_nIndex].nINT--;
} //CCharSelectInfo::Minus_INT

/**
 * 이전 다음 성별 선택
 */
void CCharSelectInfo::Next_Sex()
{
	m_CharInfo[m_nIndex].btSex = !m_CharInfo[m_nIndex].btSex;

	m_CharInfo[m_nIndex].wHair		= m_vecDefine_Hair[0];
	m_CharInfo[m_nIndex].wFace		= m_vecDefine_Face[0];

	m_CharInfo[m_nIndex].wCoat		= m_vecDefine_Coat[0];
	m_CharInfo[m_nIndex].wPants		= m_vecDefine_Pants[0];
	m_CharInfo[m_nIndex].wHair		= m_vecDefine_Hair[0];
	m_CharInfo[m_nIndex].wFace		= m_vecDefine_Face[0];		
	m_CharInfo[m_nIndex].wHand		= m_vecDefine_Hands[0];
	m_CharInfo[m_nIndex].wFoot		= m_vecDefine_Foots[0];
}

/**
 * 이전 헤어 스타일 선택
 */
void CCharSelectInfo::Previous_Hair()
{
	m_nHairIndex--;

	if( m_nHairIndex < 0 )
	{
		m_nHairIndex = m_vecDefine_Hair.size() - 1;
	}

	m_CharInfo[m_nIndex].wHair = m_vecDefine_Hair[m_nHairIndex];
} //CCharSelectInfo::Previous_Hair

/**
 * 다음 헤어 스타일을 선택한다.
 */
void CCharSelectInfo::Next_Hair()
{
	m_nHairIndex++;

	if( m_nHairIndex >= (int)m_vecDefine_Hair.size() )
	{
		m_nHairIndex = 0;
	} //if

	Assert( m_nHairIndex >= 0 );
	Assert( m_nHairIndex < (int)m_vecDefine_Hair.size() );

	m_CharInfo[m_nIndex].wHair = m_vecDefine_Hair[m_nHairIndex];
} //CCharSelectInfo::Next_Hair

/**
 * 이전 얼굴을 선택한다.
 */
void CCharSelectInfo::Previous_Face()
{
	m_nFaceIndex--;

	if( m_nFaceIndex < 0 )
	{
		m_nFaceIndex = (int)m_vecDefine_Face.size() - 1;
	} //if

	Assert( m_nHairIndex >= 0 );
	Assert( m_nHairIndex < (int)m_vecDefine_Hair.size() );

	m_CharInfo[m_nIndex].wFace = m_vecDefine_Face[m_nFaceIndex];
} //CCharSelectInfo::Previous_Face

/**
 * 다음 얼굴을 선택한다.
 */
void CCharSelectInfo::Next_Face()
{
	++m_nFaceIndex;

	if( m_nFaceIndex >= (int)m_vecDefine_Face.size() )
	{
		m_nFaceIndex = 0;
	} //if

	Assert( m_nFaceIndex >= 0 );
	Assert( m_nFaceIndex < (int)m_vecDefine_Face.size() );

	m_CharInfo[m_nIndex].wFace = m_vecDefine_Face[m_nFaceIndex];
} //CCharSelectInfo::Next_Face

/**
 * 상의 이전 선택
 */
void CCharSelectInfo::Previous_Coat()
{
	if( --m_nCoatIndex < 0 )
	{
		m_nCoatIndex = (int)m_vecDefine_Coat.size() - 1;
	} //if

	m_CharInfo[m_nIndex].wCoat = m_vecDefine_Coat[m_nCoatIndex];
	m_CharInfo[m_nIndex].wHand = m_vecDefine_Hands[m_nCoatIndex];
} //CCharSelectInfo::Previous_Coat

/**
 * 상의 다음 선택
 */
void CCharSelectInfo::Next_Coat()
{
	++m_nCoatIndex;

	if( m_nCoatIndex >= (int)m_vecDefine_Coat.size() )
	{
		m_nCoatIndex = 0;
	} //if

	m_CharInfo[m_nIndex].wCoat = m_vecDefine_Coat[m_nCoatIndex];
	m_CharInfo[m_nIndex].wHand = m_vecDefine_Hands[m_nCoatIndex];
} //CCharSelectInfo::Next_Coat

/**
 * 하의 이전 선택
 */
void CCharSelectInfo::Previous_Pants()
{
	if( --m_nPantsIndex < 0 )
	{
		m_nPantsIndex = (int)m_vecDefine_Pants.size() - 1;
	} //if

	m_CharInfo[m_nIndex].wPants = m_vecDefine_Pants[m_nPantsIndex];
	m_CharInfo[m_nIndex].wFoot  = m_vecDefine_Foots[m_nPantsIndex];
} //CCharSelectInfo::Previous_Pants

/**
 * 하의 다음 선택
 */
void CCharSelectInfo::Next_Pants()
{
	++m_nPantsIndex;

	if( m_nPantsIndex >= (int)m_vecDefine_Pants.size() )
	{
		m_nPantsIndex = 0;
	} //if

	m_CharInfo[m_nIndex].wPants = m_vecDefine_Pants[m_nPantsIndex];
	m_CharInfo[m_nIndex].wFoot  = m_vecDefine_Foots[m_nPantsIndex];
} //CCharSelectInfo::Next_Pants

/**
 * 모델파일 얻기.
 */
void CCharSelectInfo::GetModel1FileName( 
	int					nSex,
	char*				szFileName,
	SItemInfoData*		pItemInfo,
	char				cRaceNum
	)
{
	strcpy( szFileName, ITEMFILENAME1( nSex, pItemInfo ) );

	BOOL		bSkin = ITEMHAVESKIN( nSex, pItemInfo );
	if( !bSkin || cRaceNum == '0')
	{
		return;
	} //if
	
	if( szFileName[1] == 'b' )
	{
		szFileName[strlen( szFileName ) - 7] = cRaceNum;
	}
	else
	{
		szFileName[strlen( szFileName ) - 9] = cRaceNum;
	} //if..else
} //CCharSelectInfo::GetModel1FileName