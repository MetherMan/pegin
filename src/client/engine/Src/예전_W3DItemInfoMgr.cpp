/**
 * 아이템 정보 관리자.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-12-08 3:06a $
 * @version	$Revision: 69 $
 *
 * <HR>
 * $Log: /Engine/Src/W3DItemInfoMgr.cpp $
 * 
 * 69    04-12-08 3:06a Sk8snow2
 * 
 * 68    04-09-25 2:01p Paran2
 * 
 * 67    04-09-24 4:46p Paran
 * 
 * 66    04-08-27 8:01p Paran
 * 
 * 65    04-08-26 3:31p Paran
 * 
 * 42    03-12-16 5:04p Paran			아이템 데이터 수정
 * 
 * 39    03-12-12 11:57a Paran			item.dat 로드 수정
 * 
 * 38    03-12-10 1:12a Paran			스킬정보 수정
 * 
 * 37    03-12-09 10:02p Paran			스킬데이터 추가
 * 
 * 36    03-12-09 4:01p Paran			아이템 데이터 로드수정
 * 
 * 35    03-11-21 9:59a Paran			몹 아이템 드롭 수정
 * 
 * 33    03-10-27 3:24p Kjmgo			코드 최적화
 * 
 * 29    03-08-16 11:01a Sk8snow		BOOL CW3DItemInfoMgr::DrawTipInfoTextInLine( 
 * 
 * 17    03-06-21 12:39p Paran			팁 정보 출력 함수 추가.
 * 
 * 9     03-04-04 3:37p Paran			IsHaveSkin 추가.
 * <HR>
 *
 * @file	W3DItemInfoMgr.cpp
 */


#include "Engine_Export.h"
#include <map>

#define DII_NAME						0x00000001
#define DII_WEAPONKIND					0x00000002
#define DII_ARMORKIND					0x00000004
#define DII_ATTACKSPEED					0x00000008
#define DII_ATTACKPOWER					0x00000010
#define DII_ARMORPOWER					0x00000020
#define DII_ATTACKDIST					0x00000040
#define DII_MAGICPOWER					0x00000080
#define DII_WEIGHT						0x00000100
#define DII_SELLPRICE					0x00000200
#define DII_MANANEED					0x00000400
#define DII_TOTLEVNEED					0x00000800
#define DII_OTHER						0x00001000

#define FONTCOLOR_TEXT1					0xffc2c2c2
#define DRAW_NEXTTEXTLINE( p )						\
	{ rcText.top  += HEIGHT_TIPLINEBOX;					\
	  rcText.bottom += HEIGHT_TIPLINEBOX;				\
	  m_pimgTipBox_M->Draw( rcText.left, rcText.top );	\
	  pFont->DrawText( (p), &rcText, DT_CENTER | DT_VCENTER, FONTCOLOR_TEXT1 ); }

using std::map;


/**
 * 아이템 정보 관리자.
 * @ingroup		Engine
 */
class CW3DItemInfoMgr : public IW3DItemInfoMgr
{
	SINGLETON_COMPONENT( CW3DItemInfoMgr );

public:
	UNDER_MEMORY_MGR( CW3DItemInfoMgr );

	CW3DItemInfoMgr();
	~CW3DItemInfoMgr();

	BOOL			Init();
	void			CleanUp();

	BOOL			LoadItemInfo( const char* szFileName );
	BOOL			LoadSkillInfo( const char* szFileName );

	void			ReadOneItem( SItemInfoData* ItemInfo, BYTE* pData, int nReadCnt );
	void			ReadOneSkill( SSkillInfoData* pSkillInfo, BYTE* pData );

	void			GetTabString( char *src, char *str, int &nPos );
	int				GetTabNumberic( BYTE *src, int &nPos );

	void			SetSex( int nSex );
	int				GetSex();
	char*			GetFileName( int nId, BYTE btType );
	SItemInfoData*	GetItemInfo( int nIndex );
	SSkillInfoData*	GetSkillInfo( int nIndex );

	int				ItemKind( const int nType );

	void			DrawTipInfo(
		int	anSkillLevel[],
		SItemInfoData* pItemInfoDat, 
		const int nX, 
		const int nY, 
		IW3DFont* pFont,
		BOOL	  bShowInfo,
		int		  nMode,
		int		  nSellPrice
	);

	BOOL			DrawTipInfoTextInLine( 
		int	anSkillLevel[],
		SItemInfoData* pItemInfoDat,
		RECT&	rcText,
		const int nLine,
		IW3DFont* pFont
	);

	BOOL			DrawTipInfoFixedForm( 
		SItemInfoData* pItemInfoDat, 
		IW3DFont* pFont, 
		int nX, 
		int nY,
		int nMode,
		int nSellPrice
	);

	char*			GetUnitNum( DWORD dwNum );

	typedef map<int, SItemInfoData*>		MAP_ITEMINFODATA;
	typedef map<int, SSkillInfoData*>		MAP_SKILLINFODATA;
private:
	MAP_ITEMINFODATA	m_mapItemInfoData;		///< 아이템 맵
	MAP_SKILLINFODATA	m_mapSkillInfoData;		///< 스킬 맵
	int					m_nNumItem;				///< 아이템 개수
	SItemInfoData*		m_pItemInfo;			///< 현재 작업 중인 아이템
	SSkillInfoData*		m_pSkillInfo;			///< 현재 작업 중인 아이템
	char				m_pBuff[64];			///< 파일 이름을 위한 임시 버퍼
	int					m_nSex;

	IW3DSprite*			m_pimgTipBox_T;
	IW3DSprite*			m_pimgTipBox_M;
	IW3DSprite*			m_pimgTipBox_B;
}; //class CW3DItemInfoMgr


IMPLEMENT_SINGLETON_COMPONENT( CW3DItemInfoMgr, ItemInfoMgr );


/**
 * CW3DItemInfoMgr 생성자.
 */
CW3DItemInfoMgr::CW3DItemInfoMgr()
	: m_nNumItem	( 0 ),
	  m_pItemInfo	( NULL ),
	  m_pSkillInfo	( NULL ),
	  m_pimgTipBox_T( NULL ),
	  m_pimgTipBox_M( NULL ),
	  m_pimgTipBox_B( NULL )
{
} //CW3DItemInfoMgr::CW3DItemInfoMgr

/**
 * CW3DItemInfoMgr 소멸자.
 */
CW3DItemInfoMgr::~CW3DItemInfoMgr()
{
	CleanUp();
} //CW3DItemInfoMgr::~CW3DItemInfoMgr

/**
 * 릴리즈.
 */
void CW3DItemInfoMgr::CleanUp()
{
	SAFE_RELEASE( m_pimgTipBox_T );
	SAFE_RELEASE( m_pimgTipBox_M );
	SAFE_RELEASE( m_pimgTipBox_B );
	
	MAP_ITEMINFODATA::iterator	iterItem = m_mapItemInfoData.begin();
	while( iterItem != m_mapItemInfoData.end() )
	{
		MAP_ITEMINFODATA::iterator		itorNext = iterItem;
		itorNext++;
		
		SAFE_FREE( iterItem->second );
		m_mapItemInfoData.erase( iterItem );

		iterItem = itorNext;
	} //while

	MAP_SKILLINFODATA::iterator	iterSkill = m_mapSkillInfoData.begin();
	while( iterSkill != m_mapSkillInfoData.end() )
	{
		MAP_SKILLINFODATA::iterator		itorNext = iterSkill;
		itorNext++;

		SAFE_FREE( iterSkill->second );
		m_mapSkillInfoData.erase( iterSkill );

		iterSkill = itorNext;
	} //while
} //CW3DItemInfoMgr::CleanUp

/**
 * 초기화.
 */
BOOL CW3DItemInfoMgr::Init()
{
	CleanUp();

	m_pimgTipBox_T = MAKE_COMPONENT( Sprite );
	m_pimgTipBox_M = MAKE_COMPONENT( Sprite );
	m_pimgTipBox_B = MAKE_COMPONENT( Sprite );

	if( !m_pimgTipBox_T || !m_pimgTipBox_M || !m_pimgTipBox_B )
	{
		Assert( !"생성실패" );
		return FALSE;
	} //if

	if( !m_pimgTipBox_T->Load( "Interface/iteminfo-1.tga", WIDTH_TIPLINEBOX, HEIGHT_TIPLINEBOX ) )
	{
		Assert( !"로드실패" );
		return FALSE;
	} //if

	if( !m_pimgTipBox_M->Load( "Interface/iteminfo-2.tga", WIDTH_TIPLINEBOX, HEIGHT_TIPLINEBOX ) )
	{
		Assert( !"로드실패" );
		return FALSE;
	} //if

	if( !m_pimgTipBox_B->Load( "Interface/iteminfo-3.tga", WIDTH_TIPLINEBOX, HEIGHT_TIPLINEBOX ) )
	{
		Assert( !"로드실패" );
		return FALSE;
	} //if

	if( !LoadSkillInfo( "Item/Skill.dat" ) )
	{
		return FALSE;
	} //if

	if( !LoadItemInfo( "Item/Item.dat" ) )
	{
		return FALSE;
	} //if

	return TRUE;
} //CW3DItemInfoMgr::Init

/**
 * 아이템정보 파일 읽기.
 *
 * @param	szFileName			[IN] 아이템정보 데이터 파일
 *
 * @return 성공/실패
 */
BOOL CW3DItemInfoMgr::LoadItemInfo( const char* szFileName )
{
	Assert( szFileName );

	CWtm	cWtm;
	if( !cWtm.Open( szFileName ) )
	{
		Assert( !"파일열기 실패" );
		return FALSE;
	} //if

	m_mapItemInfoData.clear();

	SItemInfoData*			pItemInfo;
	BYTE					btBuff[1024];
	int						nReadCnt = 0;
	int				nCnt = 0;
	while( 1 )
	{
		nReadCnt = cWtm.SGets( btBuff, 1024 );
		if( nReadCnt == 0 )
		{
			break;
		} //if

		if( *btBuff == ';' ||
			*btBuff == '\0' )
		{
			continue;
		} //if

		pItemInfo = (SItemInfoData*)_MemAlloc( sizeof( SItemInfoData ) );
		if( !pItemInfo )
		{
			Assert( !"할당실패" );
			return FALSE;
		} //if
		ZeroMemory( pItemInfo, sizeof( SItemInfoData ) );
		ReadOneItem( pItemInfo, btBuff, nReadCnt );

		if( pItemInfo->btItemType == dITEMTYPE_MAGICSCROLL )
		{
			MAP_SKILLINFODATA::iterator		iterSkill = m_mapSkillInfoData.find( pItemInfo->wItemNum );
			if( iterSkill == m_mapSkillInfoData.end() )
			{
				Assert( !"Exist Not The SkillInfo" );
				SAFE_FREE( pItemInfo );
				continue;
			} //if
			SSkillInfoData*		pSkillData = iterSkill->second;
			Assert( pSkillData );
			pItemInfo->pSkillData = pSkillData;
		} //if
		
		MAP_ITEMINFODATA::iterator	iter = m_mapItemInfoData.find( pItemInfo->wItemNum );
		if( iter != m_mapItemInfoData.end() )
		{
//			Assert( !"있습니다" );
			SAFE_FREE( pItemInfo );
			continue;
		} //if
		m_mapItemInfoData.insert( std::make_pair( pItemInfo->wItemNum, pItemInfo ) );

		nCnt++;
	} //while

	return TRUE;
} //CW3DItemInfoMgr::LoadItemInfo

/**
 * 스킬정보 파일 읽기.
 *
 * @param	szFileName			[IN] 스칼정보 데이터 파일
 *
 * @return 성공/실패
 */
BOOL CW3DItemInfoMgr::LoadSkillInfo( const char* szFileName )
{
	Assert( szFileName );

	CWtm	cWtm;
	if( !cWtm.Open( szFileName ) )
	{
		Assert( !"파일열기 실패" );
		return FALSE;
	} //if

	m_mapSkillInfoData.clear();

	SSkillInfoData*			pSkillInfo;
	BYTE					btBuff[1024];
	while( cWtm.SGets( btBuff, 1024 ) )
	{
		if( *btBuff == ';' ||
			*btBuff == '\0' )
		{
			continue;
		} //if

		pSkillInfo = (SSkillInfoData*)_MemAlloc( sizeof( SSkillInfoData ) );
		if( !pSkillInfo )
		{
			Assert( !"할당실패" );
			return FALSE;
		} //if
		ZeroMemory( pSkillInfo, sizeof( pSkillInfo ) );
		ReadOneSkill( pSkillInfo, btBuff );
		
		MAP_SKILLINFODATA::iterator	iter = m_mapSkillInfoData.find( pSkillInfo->nItemNum );
		if( iter != m_mapSkillInfoData.end() )
		{
			Assert( !"있습니다" );
			return FALSE;
		} //if
		m_mapSkillInfoData.insert( std::make_pair( pSkillInfo->wItemNum, pSkillInfo ) );
	} //while

	return TRUE;
} //CW3DItemInfoMgr::LoadSkillInfo

/**
 * 데이터에서 아이템정보 하나 읽기
 */
void CW3DItemInfoMgr::ReadOneItem( SItemInfoData* pItemInfo, BYTE* pData, int nReadCnt )
{
	Assert( pItemInfo && pData );

	int			nPos = 0;

	pItemInfo->wItemNum			= GetTabNumberic( pData, nPos );			//< 아이템 번호
	pItemInfo->bItemIndex		= GetTabNumberic( pData, nPos );			//< 유니크
	
	GetTabString( (char*)pData, pItemInfo->szHName, nPos );					//< 한글이름
	GetTabString( (char*)pData, pItemInfo->szEName, nPos );					//< 영문이름 

	pItemInfo->btItemType		= GetTabNumberic( pData, nPos );			// 아이템 종류
	pItemInfo->wItemLev			= GetTabNumberic( pData, nPos );			// 아이템 레벨

	pItemInfo->wMinDamage		= GetTabNumberic( pData, nPos );			// 최소 공격력
	pItemInfo->wMaxDamage		= GetTabNumberic( pData, nPos );			// 최대 공격력

	pItemInfo->wMinMana			= GetTabNumberic( pData, nPos );			// 최소 마법력
	pItemInfo->wMaxMana			= GetTabNumberic( pData, nPos );			// 최대 마법력

	pItemInfo->btSlotNum		= GetTabNumberic( pData, nPos );			// 슬롯 개수
	pItemInfo->btSlotProbab		= GetTabNumberic( pData, nPos );			// 슬롯 확률
	pItemInfo->nEquipClass		= GetTabNumberic( pData, nPos );			// 착용직업

	pItemInfo->btAttackRange	= GetTabNumberic( pData, nPos );			// 공격 범위 
	pItemInfo->wDurability		= GetTabNumberic( pData, nPos );			// 내구력 
	GetTabNumberic( pData, nPos );											// 내구력 경험치
	pItemInfo->btAttackSpeed	= GetTabNumberic( pData, nPos );			// 공격 속도 
	
	pItemInfo->nItemAttr		= GetTabNumberic( pData, nPos );			// 아이템 속성
	pItemInfo->nItemDispos		= GetTabNumberic( pData, nPos );			// 아이템 성향 
	pItemInfo->wItemWeight		= GetTabNumberic( pData, nPos );			// 아이템 무게 
	
	pItemInfo->wNeedStr			= GetTabNumberic( pData, nPos );			// 필요힘 
	pItemInfo->wNeedInt			= GetTabNumberic( pData, nPos );			// 필요지혜
	pItemInfo->wNeedDex			= GetTabNumberic( pData, nPos );			// 필요민첩
	pItemInfo->wNeedCha			= GetTabNumberic( pData, nPos );			// 필요카리스마
	
	pItemInfo->nPixPrice		= GetTabNumberic( pData, nPos );			// 수리 가격 
	pItemInfo->nSellPrice		= GetTabNumberic( pData, nPos );			// 판매 가격 
	pItemInfo->nPrice			= GetTabNumberic( pData, nPos );			// 구입 가격 
	
	pItemInfo->wPluseStr		= GetTabNumberic( pData, nPos );			// 힘+
	pItemInfo->wPluseInt		= GetTabNumberic( pData, nPos );			// 지혜+
	pItemInfo->wPluseDex		= GetTabNumberic( pData, nPos );			// 민첩+
	pItemInfo->wPluseCha		= GetTabNumberic( pData, nPos );			// 카리스마+
	pItemInfo->wPluseHP			= GetTabNumberic( pData, nPos );			// 체력+
	pItemInfo->wPluseMP			= GetTabNumberic( pData, nPos );			// 마나+
	pItemInfo->wPlusePhyDamage	= GetTabNumberic( pData, nPos );			// 물리데미지+
	pItemInfo->wPluseMagicDamage= GetTabNumberic( pData, nPos );			// 마법데미지+
	pItemInfo->wPluseDefense	= GetTabNumberic( pData, nPos );			// 방어력+
	pItemInfo->wPluseMagicDefense=GetTabNumberic( pData, nPos );			// 마법방어력+
	pItemInfo->wPluseSAttack	= GetTabNumberic( pData, nPos );			// 근거리
	pItemInfo->wPluseLAttack	= GetTabNumberic( pData, nPos );			// 원거리
	pItemInfo->wPluseBMagic		= GetTabNumberic( pData, nPos );			// 흑마법
	pItemInfo->wPluseWMagic		= GetTabNumberic( pData, nPos );			// 백마법 
	
	pItemInfo->wMinusStr		= GetTabNumberic( pData, nPos );			// 힘-
	pItemInfo->wMinusInt		= GetTabNumberic( pData, nPos );			// 지혜-
	pItemInfo->wMinusDex		= GetTabNumberic( pData, nPos );			// 민첩-
	pItemInfo->wMinusCha		= GetTabNumberic( pData, nPos );			// 카리스마-
	pItemInfo->wMinusHP			= GetTabNumberic( pData, nPos );			// 체력-
	pItemInfo->wMinusMP			= GetTabNumberic( pData, nPos );			// 마나-
	pItemInfo->wMinusPhyDamage	= GetTabNumberic( pData, nPos );			// 물리데미지-
	pItemInfo->wMinusMagicDamage= GetTabNumberic( pData, nPos );			// 마법데미지-
	pItemInfo->wMinusDefense	= GetTabNumberic( pData, nPos );			// 방어력-
	pItemInfo->wMinusMagicDefense=GetTabNumberic( pData, nPos );			// 마법방어력-
	pItemInfo->wMinusSAttack	= GetTabNumberic( pData, nPos );			// 근거리-
	pItemInfo->wMinusLAttack	= GetTabNumberic( pData, nPos );			// 원거리-
	pItemInfo->wMinusBMagic		= GetTabNumberic( pData, nPos );			// 흑마법-
	pItemInfo->wMinusWMagic		= GetTabNumberic( pData, nPos );			// 백마법-
	
	GetTabString( (char*)pData, pItemInfo->szMimageFile1, nPos );			// 남자 이미지 파일1
	GetTabString( (char*)pData, pItemInfo->szMimageFile2, nPos );			// 남자 이미지 파일2
	GetTabString( (char*)pData, pItemInfo->szMimageFile3, nPos );			// 남자 이미지 파일3
	GetTabString( (char*)pData, pItemInfo->szMimageFile4, nPos );			// 남자 이미지 파일4
	GetTabString( (char*)pData, pItemInfo->szMimageFile5, nPos );			// 남자 이미지 파일5
	pItemInfo->btMHaveSkin	= GetTabNumberic( pData, nPos );				// 남자스킨 여부
	
	GetTabString( (char*)pData, pItemInfo->szFimageFile1, nPos );			// 여자 이미지 파일1
	GetTabString( (char*)pData, pItemInfo->szFimageFile2, nPos );			// 여자 이미지 파일2
	GetTabString( (char*)pData, pItemInfo->szFimageFile3, nPos );			// 여자 이미지 파일3
	GetTabString( (char*)pData, pItemInfo->szFimageFile4, nPos );			// 여자 이미지 파일4
	GetTabString( (char*)pData, pItemInfo->szFimageFile5, nPos );			// 여자 이미지 파일5
	pItemInfo->btFHaveSkin	= GetTabNumberic( pData, nPos );				// 여자스킨 여부

	pItemInfo->nItemKind = ItemKind( pItemInfo->btItemType );				// 아이템 종류(파일에서 얻지 않는다)
} //CW3DItemInfoMgr::ReadOneItem

/**
 * 데이터에서 아이템정보 하나 읽기
 */
void CW3DItemInfoMgr::ReadOneSkill( SSkillInfoData* pSkillInfo, BYTE* pData )
{
	Assert( pSkillInfo && pData );

	int			nPos = 0;
	char		szBuff[64];

	pSkillInfo->nItemNum		= GetTabNumberic( pData, nPos );			// 아이템 번호

	GetTabString( (char*)pData, pSkillInfo->szHName, nPos );				// 한글이름 
	GetTabString( (char*)pData, szBuff, nPos );								// 영문이름 

	pSkillInfo->wItemNum		= GetTabNumberic( pData, nPos );			// 아이템 번호
	pSkillInfo->btSkiiKind		= GetTabNumberic( pData, nPos );			// 아이템 종류
	pSkillInfo->shSkillType		= GetTabNumberic( pData, nPos );			// 기술형태
	pSkillInfo->shMobTimer		= GetTabNumberic( pData, nPos );			// 몹타이머
	pSkillInfo->btCheckReuse	= GetTabNumberic( pData, nPos );			// 중복검색
	pSkillInfo->shSkillAttr		= GetTabNumberic( pData, nPos );			// 기술속성
	pSkillInfo->btNeedStr		= GetTabNumberic( pData, nPos );			// 요구힘
	pSkillInfo->btNeedDex		= GetTabNumberic( pData, nPos );			// 요구민첩
	pSkillInfo->btNeedInt		= GetTabNumberic( pData, nPos );			// 요구지혜
	pSkillInfo->btNeedCha		= GetTabNumberic( pData, nPos );			// 요구매력
	pSkillInfo->btNeedClass		= GetTabNumberic( pData, nPos );			// 습득가능직업

	pSkillInfo->wMinDamage		= GetTabNumberic( pData, nPos );			// 최소데미지
	pSkillInfo->wMaxDamage		= GetTabNumberic( pData, nPos );			// 최대데미지

	pSkillInfo->btEquipType		= GetTabNumberic( pData, nPos );			// 장착종류
	pSkillInfo->shAttr			= GetTabNumberic( pData, nPos );			// 성향
	pSkillInfo->wNeedItem		= GetTabNumberic( pData, nPos );			// 시전필요재료 

	pSkillInfo->wNeedHP			= GetTabNumberic( pData, nPos );			// 시전소모 HP
	pSkillInfo->wNeedMP			= GetTabNumberic( pData, nPos );			// 시전소모 MP
	
	pSkillInfo->btTargetType	= GetTabNumberic( pData, nPos );			// 시전대상
	pSkillInfo->btRange			= GetTabNumberic( pData, nPos );			// 시전거리
	pSkillInfo->btTimer			= GetTabNumberic( pData, nPos );			// 지속시간 

	pSkillInfo->btSuccessDice	= GetTabNumberic( pData, nPos );			// 최소시전확률
	pSkillInfo->btSuccessDexDice= GetTabNumberic( pData, nPos );			// 추가시전확률 민첩
	pSkillInfo->btSuccessIntDice= GetTabNumberic( pData, nPos );			// 추가시전확률 지혜
	pSkillInfo->btExtraCode		= GetTabNumberic( pData, nPos );			// 추가기능코드
	pSkillInfo->btCodeVariable	= GetTabNumberic( pData, nPos );			// 코드변수

	GetTabString( (char*)pData, pSkillInfo->szHName, nPos );				// 간단한 설명
} //CW3DItemInfoMgr::ReadOneSkill

/**
 * 다음 탭까지 문자열 얻기
 */
void CW3DItemInfoMgr::GetTabString( char *src, char *str, int &nPos )
{
	int tmpPos = nPos;

	for( int i = 0; i < 128; i++ )
	{
		if( src[tmpPos] == '\t' ||
			src[tmpPos] == '\r' ||
			src[tmpPos] == '\n' ||
			src[tmpPos] == '\0' )
		{
			strncpy( str, &src[nPos], tmpPos - nPos );
			str[tmpPos-nPos] = NULL;
			nPos = tmpPos+1;
			return;
		} //if
		tmpPos++;
	} //for
} //CW3DItemInfoMgr::GetTabString

/**
 * 다음 탭까지 숫자 얻기
 */
int CW3DItemInfoMgr::GetTabNumberic( BYTE *src, int &nPos )
{
	BYTE	pBuff[32];
	BYTE*	pCurr = src + nPos;

	for( int nCnt = 0; nCnt < 128; nCnt++ )
	{
		if( *pCurr == '\t' || 
			*pCurr == '\r' || 
			*pCurr == '\n' ||
			*pCurr == '\0' )
		{
			pBuff[nCnt] = NULL;
			nPos = nPos + nCnt + 1;
			return atoi( (char*)pBuff );
		} //if
		pBuff[nCnt] = *pCurr;
		pCurr++;
	} //for

	return 0;
} //CW3DItemInfoMgr::GetTabNumberic

/**
 * 성별설정
 */
void CW3DItemInfoMgr::SetSex( int nSex )
{
	m_nSex = nSex;
} //CW3DItemInfoMgr::SetSex

/**
 * 성별얻기
 */
int CW3DItemInfoMgr::GetSex()
{
	return m_nSex;
} //CW3DItemInfoMgr::GetSex

/**
 * 아이템 정보.
 */
SItemInfoData* CW3DItemInfoMgr::GetItemInfo( int nIndex )
{
	MAP_ITEMINFODATA::iterator	iter = m_mapItemInfoData.find( nIndex );

	if( iter == m_mapItemInfoData.end() )
	{
		m_pItemInfo = NULL;
		Assert( !"아이템 정보를 찾을 수 없습니다" );
		return NULL;
	} //if

	m_pItemInfo = iter->second;

	return m_pItemInfo;
} //CW3DItemInfoMgr::GetItemInfo

/**
 * 스킬 정보.
 */
SSkillInfoData* CW3DItemInfoMgr::GetSkillInfo( int nIndex )
{
	MAP_SKILLINFODATA::iterator	iter = m_mapSkillInfoData.find( nIndex );

	if( iter == m_mapSkillInfoData.end() )
	{
		m_pSkillInfo = NULL;
		Assert( !"아이템 정보를 찾을 수 없습니다" );
		return NULL;
	} //if

	m_pSkillInfo = iter->second;

	return m_pSkillInfo;
} //CW3DItemInfoMgr::GetSkillInfo

/**
 * 아이템 종류.
 */
int	CW3DItemInfoMgr::ItemKind( const int nType )
{
	if( ISSHORTWEAPON( nType ) )				return EQUIP_SHORTWEAPON; 
	else if( ISLONGWEAPON( nType ) )			return EQUIP_LONGWEAPON;
	else if( ISUPPERARMOR( nType ) )			return EQUIP_UPPERARMOR;
	else if( ISLOWERARMOR( nType ) )			return EQUIP_LOWERARMOR;
	else if( ISHELMET( nType ) )				return EQUIP_HELMET;
	else if( ISBELT( nType ) )					return EQUIP_BELT;
	else if( ISGLOVE( nType ) )					return EQUIP_GLOVE;
	else if( ISSHOES( nType ) )					return EQUIP_SHOES;
	else if( dITEMTYPE_SHIELD	== nType )		return EQUIP_SHIELD;
	else if( dITEMTYPE_NECKLACE == nType )		return EQUIP_NECKLACE;
	else if( dITEMTYPE_RING		== nType )		return EQUIP_RING;
	else if( dITEMTYPE_JEWEL	== nType )		return EQUIP_JEWEL;
	else if( dITEMTYPE_SCROLL	== nType )		return EQUIP_SCROLL;
	else if( dITEMTYPE_MAGICSCROLL	== nType )	return EQUIP_MAGICSCROLL;
	else if( dITEMTYPE_CONTAINER	== nType )	return EQUIP_CONTAINER;
	else if( dITEMTYPE_POTION	== nType )		return EQUIP_POTION;
	else if( dITEMTYPE_OBJECT	== nType )		return EQUIP_OBJECT;
	else if( dITEMTYPE_FOOD		== nType )		return EQUIP_FOOD;
	else if( dITEMTYPE_ONETIME	== nType )		return EQUIP_ONETIME;

	return 0;
} //CW3DItemInfoMgr::ItemKind

/**
 * 라인별로 팀정보를 얻는다.
 *
 * @return 출력정보가 있다면 TRUE, 아님 FALSE
 */
BOOL CW3DItemInfoMgr::DrawTipInfoTextInLine( 
	int	anSkillLevel[],
	SItemInfoData* pItemInfoDat,
	RECT&	rcText,
	const int nLine,
	IW3DFont* pFont
	)
{
	if( !pItemInfoDat )
	{
		Assert( !"아이템 정보가 없다" );
		return FALSE;
	} //if
	if( !pFont )
	{
		Assert( pFont );
		return FALSE;
	} //if

	static char		szInfoText[64];
	static RECT		rcPart;

	rcPart = rcText;
	switch( nLine )
	{
	case 0:		//< 이름.
		m_pimgTipBox_M->Draw( rcText.left, rcText.top );
		pFont->DrawText( pItemInfoDat->szHName, &rcText, DT_CENTER | DT_VCENTER, FONTCOLOR_TEXT1 );
		break;

	case 1:		//< 살때 가격.
		m_pimgTipBox_M->Draw( rcText.left, rcText.top );
		sprintf( szInfoText, "살때 %d 실드", pItemInfoDat->nPrice );
		pFont->DrawText( szInfoText, &rcText, DT_CENTER | DT_VCENTER, FONTCOLOR_TEXT1 );
		break;

	case 2:
		m_pimgTipBox_M->Draw( rcText.left, rcText.top );
		sprintf( szInfoText, "팔때 %d 실드", pItemInfoDat->nSellPrice );
		pFont->DrawText( szInfoText, &rcText, DT_CENTER | DT_VCENTER, FONTCOLOR_TEXT1 );
		break;

	case 3:		//< 능력.
		if( ISWEAPONITEM( pItemInfoDat->btItemType ) )
		{
			m_pimgTipBox_M->Draw( rcText.left, rcText.top );
			sprintf( szInfoText, "공격력 %d ~ %d", pItemInfoDat->wMinDamage, pItemInfoDat->wMaxDamage );
			pFont->DrawText( szInfoText, &rcPart, DT_CENTER | DT_VCENTER, FONTCOLOR_TEXT1 );
		}
		else if( ISDEFENSEITEM( pItemInfoDat->btItemType ) )
		{
			m_pimgTipBox_M->Draw( rcText.left, rcText.top );
			sprintf( szInfoText, "방어력 %d ~ %d", pItemInfoDat->wMinDamage, pItemInfoDat->wMaxDamage );
			pFont->DrawText( szInfoText, &rcPart, DT_CENTER | DT_VCENTER, FONTCOLOR_TEXT1 );
		}
		else 
			return FALSE;
		break;

	case 4: // STR , DEX, INT +
		if( !pItemInfoDat->wPluseStr &&
			!pItemInfoDat->wPluseDex &&
			!pItemInfoDat->wPluseInt )
			return FALSE;

		m_pimgTipBox_M->Draw( rcText.left, rcText.top );
		sprintf( szInfoText, "힘+%d 민첩+%d 지력+%d", 
			pItemInfoDat->wPluseStr,
			pItemInfoDat->wPluseDex,
			pItemInfoDat->wPluseInt );
		pFont->DrawText( szInfoText, &rcPart, DT_CENTER | DT_VCENTER, FONTCOLOR_TEXT1 );

		break;
	case 5: // STR , DEX, INT -
		if( !pItemInfoDat->wMinusStr &&
			!pItemInfoDat->wMinusDex &&
			!pItemInfoDat->wMinusInt )
			return FALSE;

		m_pimgTipBox_M->Draw( rcText.left, rcText.top );
		sprintf( szInfoText, "힘-%d 민첩-%d 지력-%d", 
			pItemInfoDat->wMinusStr,
			pItemInfoDat->wMinusDex,
			pItemInfoDat->wMinusInt );
		pFont->DrawText( szInfoText, &rcPart, DT_CENTER | DT_VCENTER, FONTCOLOR_TEXT1 );
	
		break;
	case 6:
//		if( pItemInfoDat->btItemType != dITEMTYPE_MAGICSCROLL ||
//			!pItemInfoDat->m_MagicKind ||
//			pItemInfoDat->m_MagicCastType != 2 )
//			return FALSE;

		m_pimgTipBox_M->Draw( rcText.left, rcText.top );
		switch( pItemInfoDat->wItemNum )
		{
		case dMAGIC_NUM_BLESS:
			strcpy( szInfoText, "공격력+" );
			break;
		case dMAGIC_NUM_REMOVEPIRE:
			strcpy( szInfoText, "공격력+,속도+" );
			break;
		case dMAGIC_NUM_CURELIGHT:
		case dMAGIC_NUM_CUREDIGIZ:
			strcpy( szInfoText, "HP 회복" );
			break;
		case dMAGIC_NUM_BUCKSKIN:
		case dMAGIC_NUM_MAGICBESTMENT:
		case dMAGIC_NUM_SHIELD:
			strcpy( szInfoText, "방어력+" );
			break;
		case dMAGIC_NUM_FIRESHIELD:
			strcpy( szInfoText, "불속성 방어력+" );
			break;
		}
		pFont->DrawText( szInfoText, &rcPart, DT_CENTER | DT_VCENTER, FONTCOLOR_TEXT1 );
		break;

	default:		return FALSE;
	} //switch
	
	return TRUE;
} //CW3DItemInfoMgr::GetTipInfoLineText

/**
 * 아이템 팁 정보 보이기.
 */
void CW3DItemInfoMgr::DrawTipInfo( 
	int	anSkillLevel[],
	SItemInfoData* pItemInfoDat,
	const int nX, 
	const int nY, 
	IW3DFont* pFont,
	BOOL	  bShowInfo,
	int		  nMode,
	int		  nSellPrice
	)
{
	if( !pItemInfoDat )
	{
		Assert( !"아이템 정보가 없다" );
		return;
	} //if

	if( ISSKIPITEM( pItemInfoDat->wItemNum ) )
	{
		return;
	} //if

	if( !pFont )
	{
		Assert( pFont );
		return;
	} //if

	if( DrawTipInfoFixedForm( pItemInfoDat, pFont, nX, nY, nMode, nSellPrice ) )
	{
		return;
	} //if

	static char		szItemKind[32];
	static char		szBuff[32];

	DWORD		dwFlag = DII_NAME;
	BYTE		btLine = 3;

	if( bShowInfo )
	{
		dwFlag |= DII_SELLPRICE;
		btLine += 1;

		if( dITEMTYPE_RING		== pItemInfoDat->btItemType ||
			dITEMTYPE_NECKLACE	== pItemInfoDat->btItemType )
		{
			dwFlag |= DII_OTHER;
			btLine += 1;
		}
		else if( ISWEAPONITEM( pItemInfoDat->btItemType ) )
		{
			dwFlag |= DII_ATTACKSPEED | DII_ATTACKPOWER | DII_WEIGHT;
			btLine += 3;
			if( dITEMTYPE_STAFF == pItemInfoDat->btItemType )
			{
				dwFlag |= DII_WEAPONKIND | DII_MAGICPOWER;
				btLine += 2;
			}
			else if( ISTWOHANDWEAPON( pItemInfoDat->btItemType ) )
			{
				dwFlag |= DII_WEAPONKIND;
				btLine += 1;
				sprintf( szItemKind, STR_ITEMINFO_TWOHANDWEAPON );
			}
			else if( ISLONGWEAPON( pItemInfoDat->btItemType ) )
			{
				dwFlag |= DII_ATTACKDIST;
				btLine += 1;
			} //if..else
		}
		else if( ISDEFENSEITEM( pItemInfoDat->btItemType ) )
		{
			dwFlag |= DII_ARMORPOWER | DII_WEIGHT | DII_ARMORKIND;
			btLine += 3;
			if( pItemInfoDat->nEquipClass == 0 )		sprintf( szItemKind, STR_ITEMINFO_COMMONCLASS );
			else if( pItemInfoDat->nEquipClass == 1 )	sprintf( szItemKind, STR_ITEMINFO_WARRIORCLASS );
			else if( pItemInfoDat->nEquipClass == 2 )	sprintf( szItemKind, STR_ITEMINFO_ARCHERCLASS );
			else if( pItemInfoDat->nEquipClass == 3 )	sprintf( szItemKind, STR_ITEMINFO_MAGICCLASS );
		}
		else if( ISHORSE( pItemInfoDat->wItemNum ) )
		{
			dwFlag |= DII_TOTLEVNEED;
			btLine += 1;
		}
		else if( pItemInfoDat->btItemType == dITEMTYPE_MAGICSCROLL )
		{
			SSkillInfoData*		pSkillData = pItemInfoDat->pSkillData;
			if( !pSkillData )
			{
				Assert( !"스킬데이터 얻기 실패" );
				return;
			} //if

			if( IS_ATTACK_MAGIC( pSkillData ) )
			{
				dwFlag |= DII_ATTACKPOWER | DII_MANANEED;
				btLine += 2;
			}
			else
			{
				dwFlag |= DII_MANANEED;
				btLine += 1;
			} //if..else
		} //if..else
	} //if

	static RECT		rcText;
	rcText.left			= nX;
	rcText.top			= nY - ( btLine * HEIGHT_TIPLINEBOX );
	rcText.right		= rcText.left + WIDTH_TIPLINEBOX;
	rcText.bottom		= rcText.top + HEIGHT_TIPLINEBOX;

	m_pimgTipBox_T->Draw( rcText.left, rcText.top );

	if( dwFlag & DII_NAME )
	{
		if( ISWEAPONITEM( pItemInfoDat->btItemType ) || ISDEFENSEITEM( pItemInfoDat->btItemType ) )
		{
			if( pItemInfoDat->bItemIndex == 0 )
			{
				sprintf( szBuff, "%s", pItemInfoDat->szHName );
			}	
			else
			{
				sprintf( szBuff, "(+%d) %s",pItemInfoDat->bItemIndex, pItemInfoDat->szHName );	
				
			}			
		}
		else
		{	
			sprintf( szBuff, "%s", pItemInfoDat->szHName );
		}
		DRAW_NEXTTEXTLINE( szBuff );
	}
	if( dwFlag & DII_OTHER )
	{
		if( pItemInfoDat->wItemNum == 10398 )				strcpy( szBuff, STR_ITEMINFO_SEARCHUSER );
		else if( pItemInfoDat->wItemNum == 10399 )			strcpy( szBuff, STR_ITEMINFO_RESURRECTUSER );

		else if( pItemInfoDat->wItemNum == 10401 )			strcpy( szBuff, STR_ITEMINFO_TRANSPANRENTEFFECT );
		else if( pItemInfoDat->wItemNum == 10402 )			strcpy( szBuff, STR_ITEMINFO_STUNEFFECT9PERCENT );
		else if( pItemInfoDat->wItemNum == 10403 )			strcpy( szBuff, STR_ITEMINFO_CHANGEDAMAGE );

		else if( pItemInfoDat->wItemNum == 10403 )			strcpy( szBuff, STR_ITEMINFO_STRENGTH2PLUS );
		else if( pItemInfoDat->wItemNum == 10403 )			strcpy( szBuff, STR_ITEMINFO_STRENGTH5PLUS );
		
//		else if( pItemInfoDat->wItemNum == 10404 )			strcpy( szBuff, "민첩 +2" );
//		else if( pItemInfoDat->wItemNum == 10405 )			strcpy( szBuff, "민첩 +5" );
		
//		else if( pItemInfoDat->wItemNum == 10406 )			strcpy( szBuff, "지력 +2" );
//		else if( pItemInfoDat->wItemNum == 10407 )			strcpy( szBuff, "지력 +5" );
		
//		else if( pItemInfoDat->wItemNum == 10408 )			strcpy( szBuff, "HP회복 +3%" );
		else if( pItemInfoDat->wItemNum == 10409 )			strcpy( szBuff, STR_ITEMINFO_HPGAIN11PERCENTPLUS );
		else if( pItemInfoDat->wItemNum == 10410 )			strcpy( szBuff, STR_ITEMINFO_MPGAIN11PERCENTPLUS );

/*		else if( pItemInfoDat->wItemNum == 10409 )			strcpy( szBuff, "HP회복 +7%" );
		else if( pItemInfoDat->wItemNum == 10410 )			strcpy( szBuff, "HP회복 +12%" );
		else if( pItemInfoDat->wItemNum == 10411 )			strcpy( szBuff, "MP회복 +3%" );
		else if( pItemInfoDat->wItemNum == 10412 )			strcpy( szBuff, "MP회복 +7%" );
		else if( pItemInfoDat->wItemNum == 10413 )			strcpy( szBuff, "MP회복 +12%" );
		else if( pItemInfoDat->wItemNum == 10414 )			strcpy( szBuff, "유저소환" );
*/		
		//else if( pItemInfoDat->wItemNum == 10415 )		strcpy( szBuff, "경험치 복구 1회" );
		else if( pItemInfoDat->wItemNum == 10415 )			strcpy( szBuff, STR_ITEMINFO_SHOPITEMDC5PERCER );
		else if( pItemInfoDat->wItemNum == 10416 )			strcpy( szBuff, STR_ITEMINFO_COMBACKVILLAGE );
		
		//else if( pItemInfoDat->wItemNum == 10418 )		strcpy( szBuff, "HP +30" );
		else if( pItemInfoDat->wItemNum == 10419 )			strcpy( szBuff, STR_ITEMINFO_MAGICDAMAGE15PERCENT );

		else if( pItemInfoDat->wItemNum == 10425 )			strcpy( szBuff, STR_ITEMINFO_MOVEOFF6PERCENT );
		else if( pItemInfoDat->wItemNum == 10426 )			strcpy( szBuff, STR_ITEMINFO_MOVEOFF9PERCENT );

		else if( pItemInfoDat->wItemNum == 10428 )			strcpy( szBuff, STR_ITEMINFO_ATTACKOFF6PERCENT );
		else if( pItemInfoDat->wItemNum == 10429 )			strcpy( szBuff, STR_ITEMINFO_ATTACKOFF9PERCENT );
		else if( pItemInfoDat->wItemNum == 10431 )			strcpy( szBuff, STR_ITEMINFO_MAGICRESIST11PERCENT );
		
		else if( pItemInfoDat->wItemNum == 10383 )			strcpy( szBuff, STR_ITEMINFO_HPGAIN11PERCENTPLUS );

//		else if( pItemInfoDat->wItemNum == 10419 )			strcpy( szBuff, "HP +70" );
//		else if( pItemInfoDat->wItemNum == 10420 )			strcpy( szBuff, "HP +120" );
//		else if( pItemInfoDat->wItemNum == 10421 )			strcpy( szBuff, "MP +30" );
//		else if( pItemInfoDat->wItemNum == 10422 )			strcpy( szBuff, "MP +70" );
//		else if( pItemInfoDat->wItemNum == 10423 )			strcpy( szBuff, "MP +120" );
//		else if( pItemInfoDat->wItemNum == 10424 )			strcpy( szBuff, "마법데미지 +5%" );
//		else if( pItemInfoDat->wItemNum == 10425 )			strcpy( szBuff, "마법데미지 +10%" );
//		else if( pItemInfoDat->wItemNum == 10426 )			strcpy( szBuff, "마법데미지 +15%" );
//		else if( pItemInfoDat->wItemNum == 10427 )			strcpy( szBuff, "이동정지 3%" );
//		else if( pItemInfoDat->wItemNum == 10428 )			strcpy( szBuff, "이동정지 6%" );
//		else if( pItemInfoDat->wItemNum == 10429 )			strcpy( szBuff, "이동정지 9%" );
//		else if( pItemInfoDat->wItemNum == 10430 )			strcpy( szBuff, "공격정지 3%" );
//		else if( pItemInfoDat->wItemNum == 10431 )			strcpy( szBuff, "공격정지 6%" );
//		else if( pItemInfoDat->wItemNum == 10432 )			strcpy( szBuff, "공격정지 9%" );
	
		DRAW_NEXTTEXTLINE( szBuff );
	}
	if( dwFlag & DII_WEAPONKIND )
	{
		DRAW_NEXTTEXTLINE( STR_ITEMINFO_TWOHANDWEAPON );
	}
	if( dwFlag & DII_ARMORKIND )
	{
		DRAW_NEXTTEXTLINE( szItemKind );
	}
	if( dwFlag & DII_ATTACKSPEED )
	{
		switch( pItemInfoDat->btAttackSpeed )
		{
		case 0:		sprintf( szBuff, "%s : %s", STR_ITEMINFO_ATTACKSPEED, STR_ITEMINFO_TOOSLOWSPEED );		break;
		case 1:		sprintf( szBuff, "%s : %s", STR_ITEMINFO_ATTACKSPEED, STR_ITEMINFO_SLOWSPEED );			break;
		case 2:		sprintf( szBuff, "%s : %s", STR_ITEMINFO_ATTACKSPEED, STR_ITEMINFO_NORMALSPEED );		break;
		case 3:		sprintf( szBuff, "%s : %s", STR_ITEMINFO_ATTACKSPEED, STR_ITEMINFO_FASTSPEED );			break;
		case 4:		sprintf( szBuff, "%s : %s", STR_ITEMINFO_ATTACKSPEED, STR_ITEMINFO_VERYFASTSPEED );		break;
		default:	sprintf( szBuff, "" );							break;
		} //switch
		DRAW_NEXTTEXTLINE( szBuff );
	}
	if( dwFlag & DII_ATTACKPOWER )
	{
		if( pItemInfoDat->btItemType == dITEMTYPE_MAGICSCROLL )
		{
			SSkillInfoData*		pSkillData = pItemInfoDat->pSkillData;
			if( pSkillData )
			{
				sprintf( szBuff, 
					"%s : %d - %d", 
					STR_ITEMINFO_ATTACKPOWER, pSkillData->wMinDamage, pSkillData->wMaxDamage );
				DRAW_NEXTTEXTLINE( szBuff );
			} //if
		}
		else
		{
			sprintf( szBuff, 
				"%s : %d - %d", 
				STR_ITEMINFO_ATTACKPOWER, pItemInfoDat->wMinDamage, pItemInfoDat->wMaxDamage );
			DRAW_NEXTTEXTLINE( szBuff );
		} //if..else
	}
	if( dwFlag & DII_ARMORPOWER )
	{
		sprintf( szBuff, "%s : %d", STR_ITEMINFO_ARMORPOWER, pItemInfoDat->wMinDamage );
		DRAW_NEXTTEXTLINE( szBuff );
	}
	if( dwFlag & DII_ATTACKDIST )
	{
		sprintf( szBuff, "%s : %d", STR_ITEMINFO_ATTACKDIST, pItemInfoDat->btAttackRange );
		DRAW_NEXTTEXTLINE( szBuff );
	}
	if( dwFlag & DII_MAGICPOWER )
	{
		sprintf( szBuff, "%s : %d - %d", STR_ITEMINFO_MAGICPOWER, pItemInfoDat->wMinMana, pItemInfoDat->wMaxMana );
		DRAW_NEXTTEXTLINE( szBuff );
	}
	if( dwFlag & DII_WEIGHT )
	{
		sprintf( szBuff, "%s : %d", STR_ITEMINFO_WEIGHT, pItemInfoDat->wItemWeight );
		DRAW_NEXTTEXTLINE( szBuff );
	}
	if( dwFlag & DII_SELLPRICE )
	{
		if( nMode == 0 )	
		{ // 자신
			sprintf( szBuff, "%s:%s%s", STR_ITEMINFO_SELLPRICE, GetUnitNum( pItemInfoDat->nSellPrice ), STR_ITEMINFO_SHILD );
		}
		else if( nMode == 1 )
		{ // 상대방
			sprintf( szBuff, "%s:%s%s", STR_ITEMINFO_BUYPRICE, GetUnitNum( pItemInfoDat->nPrice ), STR_ITEMINFO_SHILD );
		}
		else if( nMode == 2 )
		{
			sprintf( szBuff, "%s:%s%s", STR_ITEMINFO_SELLPRICE, GetUnitNum( nSellPrice ), STR_ITEMINFO_SHILD );
		} //if..else
		DRAW_NEXTTEXTLINE( szBuff );
	}
	if( dwFlag & DII_MANANEED )
	{
		SSkillInfoData*		pSkillData = pItemInfoDat->pSkillData;
		if( pSkillData )
		{
			sprintf( szBuff, "%s : %d", STR_ITEMINFO_NEEDMP, pSkillData->wNeedMP );
			DRAW_NEXTTEXTLINE( szBuff );
		} //if
	}
	if( dwFlag & DII_TOTLEVNEED )
	{
		if( pItemInfoDat->wItemNum == ITEM_HORSEB )
		{ // Black Horse
			sprintf( szBuff, "%s : %d", STR_ITEMINFO_NEEDTOTLEV, 50 );
		} 
		else if( pItemInfoDat->wItemNum == ITEM_HORSEW )
		{ // White Horse
			sprintf( szBuff, "%s : %d", STR_ITEMINFO_NEEDTOTLEV, 75 );
		}
		else
		{
			sprintf( szBuff, "%s : %d", STR_ITEMINFO_NEEDTOTLEV, 0 );
		} //if..else..
		
		DRAW_NEXTTEXTLINE( szBuff );
	}

	if( ISWEAPONITEM( pItemInfoDat->btItemType ) || ISDEFENSEITEM( pItemInfoDat->btItemType ) )
	{
		sprintf( szBuff, "Rank %d", pItemInfoDat->wItemLev );
		DRAW_NEXTTEXTLINE( szBuff );
	}
	
	rcText.top	  += HEIGHT_TIPLINEBOX;
	rcText.bottom += HEIGHT_TIPLINEBOX;
	m_pimgTipBox_B->Draw( rcText.left, rcText.top );
	
	return;
} //CW3DItemInfoMgr::DrawTipInfo

/**
 *  
 */
BOOL CW3DItemInfoMgr::DrawTipInfoFixedForm( SItemInfoData* pItemInfoDat, IW3DFont* pFont, int nX, int nY, int nMode, int nSellPrice )
{
/*#ifdef _CHINAVERSION
	return FALSE;
#else*/
#define DRAW_BEGINTEXTLINE( p, n )								\
	{ rcText.top		= nY - ( (n) * HEIGHT_TIPLINEBOX );		\
	rcText.bottom		= rcText.top + HEIGHT_TIPLINEBOX;		\
	m_pimgTipBox_T->Draw( rcText.left, rcText.top );			\
	DRAW_NEXTTEXTLINE( (p) ); }
//#define DRAW_BEGINTEXTLINE

	Assert( pItemInfoDat );
	Assert( pFont );

	RECT		rcText;
	rcText.left			= nX;
	rcText.right		= rcText.left + WIDTH_TIPLINEBOX;

switch( pItemInfoDat->wItemNum )
	{
	case ITEM_WOARAIM13_10H:					///< 웜쓰오브 아라임
	case ITEM_WOARAIM13_20H:
	case ITEM_WOARAIM13_30H:
	case ITEM_WOARAIM13_1M:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Extra Exp while" );
		DRAW_NEXTTEXTLINE( "hunting monsters" );
		DRAW_NEXTTEXTLINE( "(x1.3)" );
		//DRAW_NEXTTEXTLINE( "(프리미엄죤 플러스상태" );
		//DRAW_NEXTTEXTLINE( "사용불가)" );
		break;

	case ITEM_WOARAIM15_10H:					///< 웜쓰오브 아라임
	case ITEM_WOARAIM15_20H:
	case ITEM_WOARAIM15_30H:
	case ITEM_WOARAIM15_1M:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Extra Exp while" );
		DRAW_NEXTTEXTLINE( "hunting monsters");
		DRAW_NEXTTEXTLINE( "(x1.5)" );
	/*DRAW_NEXTTEXTLINE( "(프리미엄죤 플러스상태" );
		DRAW_NEXTTEXTLINE( "사용불가)" );*/
		break;

	case ITEM_AOBRILIANCE_5:					///< 오오라 오브 브릴리언스
	case ITEM_AOBRILIANCE_10:
	case ITEM_AOBRILIANCE_15:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Exp loss preventing" );
		DRAW_NEXTTEXTLINE( " item when character dies" );
	/*	DRAW_NEXTTEXTLINE( "(인벤토리내 소지시" );
		DRAW_NEXTTEXTLINE( "자동적용)" );*/
		break;

	case ITEM_PHANTOMGET_5:						///< 팬텀 게더러
	case ITEM_PHANTOMGET_10:
	case ITEM_PHANTOMGET_15:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Item drop preventing" );
		DRAW_NEXTTEXTLINE( "item when character dies" );
	/*	DRAW_NEXTTEXTLINE( "(인벤토리내 소지시" );
		DRAW_NEXTTEXTLINE( "자동적용)" );*/
		break;

	case ITEM_SHOUTPILL_10:						///< 외침용 알약				
	case ITEM_SHOUTPILL_20:
	case ITEM_SHOUTPILL_30:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Big shouting item" );
		DRAW_NEXTTEXTLINE( "through same" );
		DRAW_NEXTTEXTLINE( "map area" );
	//	DRAW_NEXTTEXTLINE( "(+ 사용)" );
		break;

	case ITEM_IOARAIM13_10H:					///< 아이템 오브 아라임
	case ITEM_IOARAIM13_20H:
	case ITEM_IOARAIM13_30H:
	case ITEM_IOARAIM13_1M:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Extra item drop rate" );
		DRAW_NEXTTEXTLINE( "while hunting monsters" );
		DRAW_NEXTTEXTLINE( "(x1.3)" );
	/*	DRAW_NEXTTEXTLINE( "(프리미엄죤 플러스상태" );
		DRAW_NEXTTEXTLINE( "사용불가)" );*/
		break;

	case ITEM_IOARAIM15_10H:					///< 아이템 오브 아라임
	case ITEM_IOARAIM15_20H:
	case ITEM_IOARAIM15_30H:
	case ITEM_IOARAIM15_1M:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Extra item drop rate" );
		DRAW_NEXTTEXTLINE( "while hunting monsters" );
		DRAW_NEXTTEXTLINE( "(x1.5)");
	/*	DRAW_NEXTTEXTLINE( "(프리미엄죤 플러스상태" );
		DRAW_NEXTTEXTLINE( "사용불가)" );*/
		break;

	case ITEM_REVIVEROLLPAPER_5:				///< 부활의 두루마리
	case ITEM_REVIVEROLLPAPER_10:
	case ITEM_REVIVEROLLPAPER_15:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Character rebirth item" );
	//	DRAW_NEXTTEXTLINE( "(더블클릭 사용)" );
		break;

	case ITEM_GUILDNOTICEPILL10:				///< 길드공지의 알약
	case ITEM_GUILDNOTICEPILL20:
	case ITEM_GUILDNOTICEPILL30:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Send notification to" );
		DRAW_NEXTTEXTLINE( "guild or army members" );
	/*	DRAW_NEXTTEXTLINE( "(& 사용," );
		DRAW_NEXTTEXTLINE( "체팅모드->단모드)" );*/
		break;

	case ITEM_PREMIUMZONE10H:					///< 프리미엄존 서비스
	case ITEM_PREMIUMZONE30H:
	case ITEM_PREMIUMZONE1M:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Premium zone" );
		DRAW_NEXTTEXTLINE( "transfering item" );
		break;

	case ITEM_PREMIUMACCOUNT10H:				///< 프리미엄계정 서비스
	case ITEM_PREMIUMACCOUNT20H:
	case ITEM_PREMIUMACCOUNT30H:
	case ITEM_PREMIUMACCOUNT1M:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "겸헝치, 아이템 취득" );
		DRAW_NEXTTEXTLINE( "확률 2배 상승" );
		DRAW_NEXTTEXTLINE( "Premium zone" );
		break;

	case ITEM_BLESSOFGOD10H:					///< 신의 축복
	case ITEM_BLESSOFGOD20H:
	case ITEM_BLESSOFGOD30H:
	case ITEM_BLESSOFGOD1M:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Enchant card" );
		DRAW_NEXTTEXTLINE( "drop rate" );
		DRAW_NEXTTEXTLINE( "10% upgrading item");
	/*	DRAW_NEXTTEXTLINE( "(프리미엄죤 플러스상태" );
		DRAW_NEXTTEXTLINE( "사용불가)" );*/
		break;

	case ITEM_ENCHANTCHANGE5:					///< 인첸트 체인지
	case ITEM_ENCHANTCHANGE10:
	case ITEM_ENCHANTCHANGE15:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "아머카드<->웨폰카드" );
		break;

	case ITEM_ENCHANTCOLLECTION5:				///< 인첸트 콜렉션
	case ITEM_ENCHANTCOLLECTION10:
	case ITEM_ENCHANTCOLLECTION15:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Enchant card" );
		DRAW_NEXTTEXTLINE( "roll-back item" );
		break;

	case ITEM_ROOK10:							///< 아이템 룩
	case ITEM_ROOK20:
	case ITEM_ROOK30:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Checking objective's" );
		DRAW_NEXTTEXTLINE( "equipped item" );
		DRAW_NEXTTEXTLINE( "information" );
		break;

	case ITEM_PROTECTION10:						///< 아이템 프로텍션
	case ITEM_PROTECTION20:
	case ITEM_PROTECTION30:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Prevent others' from" );
		DRAW_NEXTTEXTLINE( "checking your equipped" );
		DRAW_NEXTTEXTLINE( "item information" );
		break;

	case ITEM_LVROOK10:							///< 레벨 룩
	case ITEM_LVROOK20:
	case ITEM_LVROOK30:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Checking objective's" );
		DRAW_NEXTTEXTLINE( "character ability" );
		DRAW_NEXTTEXTLINE( "information" );
		break;

	case ITEM_LVPROTECTION10:					///< 레벨 프로텍션
	case ITEM_LVPROTECTION20:
	case ITEM_LVPROTECTION30:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Prevent others' from" );
		DRAW_NEXTTEXTLINE( "checking your character" );
		DRAW_NEXTTEXTLINE( "ability information" );
		break;
		
	case ITEM_MOBILELIQUOR20H:					///< 기동성 물약
	case ITEM_MOBILELIQUOR40H:
	case ITEM_MOBILELIQUOR1M:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Upgrade 25% running" );
		DRAW_NEXTTEXTLINE( "speed for certain time" );
	//	DRAW_NEXTTEXTLINE( "(말은 제외)" );
		break;

	case ITEM_WEIGHTDRINK5:						///< 웨이트 드링크
	case ITEM_WEIGHTDRINK10:
	case ITEM_WEIGHTDRINK15:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Inrease 20% or character" );
		DRAW_NEXTTEXTLINE( "weight limit for an hour" );
		break;

	case ITEM_HPDRINK5:							///< HP 드링크
	case ITEM_HPDRINK10:
	case ITEM_HPDRINK15:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Increase 20% HP" );
		DRAW_NEXTTEXTLINE( "for an hour" );
		break;

	case ITEM_MPDRINK5:							///< MP 드링크
	case ITEM_MPDRINK10:
	case ITEM_MPDRINK15:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Increase 20% MP");
		DRAW_NEXTTEXTLINE( "for an hour" );
		break;

	case ITEM_GWOARAIM1:						///< 그룹 웜스 오브 아라임
	case ITEM_GWOARAIM5:
	case ITEM_GWOARAIM10:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Give extra Exp to" );
		DRAW_NEXTTEXTLINE( "all party members" );
		DRAW_NEXTTEXTLINE( "for an hour" );
	//	DRAW_NEXTTEXTLINE( "(파티장이 사용가능," );
	//	DRAW_NEXTTEXTLINE( "파티장기준으로 효과)" );
		break;

	case ITEM_GROUPHPDRINK1:					///< 그룹 HP 드링크
	case ITEM_GROUPHPDRINK5:
	case ITEM_GROUPHPDRINK10:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Increase 20% of" );
		DRAW_NEXTTEXTLINE( "all party members' HP" );
		DRAW_NEXTTEXTLINE( "for an hour" );
	//	DRAW_NEXTTEXTLINE( "(파티장이 사용가능," );
	//	DRAW_NEXTTEXTLINE( "파티장기준으로 효과)" );
		break;

	case ITEM_GROUPMPDRINK1:					///< 그룹 MP 드링크
	case ITEM_GROUPMPDRINK5:
	case ITEM_GROUPMPDRINK10:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Increase 20% of" );
		DRAW_NEXTTEXTLINE( "all party members' MP" );
		DRAW_NEXTTEXTLINE( "for an hour" );
	//	DRAW_NEXTTEXTLINE( "(파티장이 사용가능," );
	//	DRAW_NEXTTEXTLINE( "파티장기준으로 효과)" );
		break;

	case ITEM_GROUPHPARMYDRINK1:				///< 군단원 HP 드링크
	case ITEM_GROUPHPARMYDRINK5:
	case ITEM_GROUPHPARMYDRINK10:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Increase 20% of" );
		DRAW_NEXTTEXTLINE( "all connected");
		DRAW_NEXTTEXTLINE( "guild(Army) members'" );
		DRAW_NEXTTEXTLINE( "HP for 30 minutes" );
		break;

	case ITEM_GROUPMPARMYDRINK1:				///< 군단원 MP 드링크
	case ITEM_GROUPMPARMYDRINK5:
	case ITEM_GROUPMPARMYDRINK10:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Increase 20% of" );
		DRAW_NEXTTEXTLINE( "all connected" );
		DRAW_NEXTTEXTLINE( "guild(Army) members'" );
		DRAW_NEXTTEXTLINE( "MP for 30 minutes" );
		break;

	case ITEM_SUMMON_GUILD_MEMBER:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Summon 3 random guild" );
		DRAW_NEXTTEXTLINE( "members connected" );
		break;

	case ITEM_MINUS:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Double G/E point while" );
		DRAW_NEXTTEXTLINE( "Hunting monsters" );
		break;

	case ITEM_WARCRYOFCURSE:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Curse effect will appear" );
		DRAW_NEXTTEXTLINE( "to all guild members" );
		break;

	case ITEM_WARCRYOFPROVOCATION:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Provocating effect will" );
		DRAW_NEXTTEXTLINE( "appear to all guild members" );
		break;

	case ITEM_WARCRYOFVICTORY:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Victory effect will appear" );
		DRAW_NEXTTEXTLINE( "to all guild members" );
		break;

	case ITEM_WARCRYOFBLESSING:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Blessing effect will appear" );
		DRAW_NEXTTEXTLINE( "to all guild members" );
		break;

	case ITEM_WARCRYOFPRAYER:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Prayer effect will appear" );
		DRAW_NEXTTEXTLINE( "to all guild members" );
		break;

	case ITEM_HEARTEMOTICON:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Heart effect on the upper" );
		DRAW_NEXTTEXTLINE( "part of your character" );
		break;

	case ITEM_HAPPYEMOTICON:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Happy effect on the upper" );
		DRAW_NEXTTEXTLINE( "part of your character" );
		break;
		
	case ITEM_SORROWEMOTICON:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Sorrow effect on the upper" );
		DRAW_NEXTTEXTLINE( "part of your character" );
		break;

	case ITEM_WRATHEMOTICON:
		DRAW_BEGINTEXTLINE( pItemInfoDat->szHName, 7 );
		DRAW_NEXTTEXTLINE( "Wrath effect on the upper" );
		DRAW_NEXTTEXTLINE( "part of your character" );
		break;

	default:
		return FALSE;
	} //switch


	static char		szBuff[32];
	if( nMode == 0 )		sprintf( szBuff, "%s:%s%s", STR_ITEMINFO_SELLPRICE, GetUnitNum( pItemInfoDat->nSellPrice ), STR_ITEMINFO_SHILD );
	else if( nMode == 1 )	sprintf( szBuff, "%s:%s%s", STR_ITEMINFO_BUYPRICE, GetUnitNum( pItemInfoDat->nPrice ), STR_ITEMINFO_SHILD );
	else if( nMode == 2 )	sprintf( szBuff, "%s:%s%s", STR_ITEMINFO_SELLPRICE, GetUnitNum( nSellPrice ), STR_ITEMINFO_SHILD );
	DRAW_NEXTTEXTLINE( szBuff );

	rcText.top			+= HEIGHT_TIPLINEBOX;
	rcText.bottom		+= HEIGHT_TIPLINEBOX;
	m_pimgTipBox_B->Draw( rcText.left, rcText.top );

#undef DRAW_BEGINTEXTLINE

	return TRUE;
//#endif //_CHINAVERSION
} //CW3DItemInfoMgr::DrawTipInfoFixedForm

/**
 * 
 */
char* CW3DItemInfoMgr::GetUnitNum( DWORD dwNum )
{
	static char		s_szBuff[32];

	char			szTemp[32];
	sprintf( szTemp, "%d", dwNum );

	int		nUnit = strlen( szTemp ) / 3;
	if( ( strlen( szTemp ) % 3 ) == 0 )
	{
		nUnit--;
	} //if

	strcpy( s_szBuff, szTemp );
	if( nUnit > 0 )
	{
		char*	pSrcPtr  = szTemp + strlen( szTemp );
		char*	pDestPtr = s_szBuff + strlen( szTemp ) + nUnit;
		*pDestPtr = '\0';

		for( int nCnt = nUnit; nCnt > 0; nCnt-- )
		{ 
			pSrcPtr		-= 3;
			pDestPtr	-= 3;
			memcpy( pDestPtr, pSrcPtr, 3 );
			pDestPtr--;
			*pDestPtr = ',';
		} //for
	} //if

	return s_szBuff;
} //CW3DItemInfoMgr::GetUnitNum
