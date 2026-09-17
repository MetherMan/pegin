/**
 * 유저 인터페이스 파일 로더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-12-08 3:07a $
 * @version	$Revision: 57 $
 *
 * <HR>
 * $Log: /UInterface/Src/W3DGUIFileLoader.cpp $
 * 
 * 57    04-12-08 3:07a Sk8snow2
 * 
 * 56    04-06-23 5:46p Paran
 * 
 * 54    04-05-14 4:31p Paran
 * 
 * 53    04-05-07 9:53a Paran2
 * 
 * 52    04-04-08 12:18a Paran2
 * 
 * 38    03-12-09 5:58p Paran			아이템 데이터 수정
 * 
 * 25    03-09-15 1:42a Paran			Quick Slot 포션 갯수 수정.
 * 
 * 12    03-06-04 11:24a Paran			미니맵 상점 추가
 * 
 * 11    03-05-28 4:14p Paran			미니 맵 파티 관련 수정
 *
 * 9     03-05-25 4:10p Paran			물약 다량 구입 인터페이스 추가
 * 
 * 4     03-05-21 1:27p Paran			크기 안맞는 문제 수정.
 * <HR>
 *
 * @file	W3DGUIFileLoader.cpp
 */


#include "UInterface_Export.h"
#include "W3DUInterface.h"
#include <stdio.h>
#include <io.h>

/**
 * 인터페이스 파일을 읽어온다.
 *
 * @param		szFileName		[IN] 유저 인터페이스 정보 파일 이름
 * @param		szStateName		[IN] 유저 인터페이스 상태 파일 이름
 *
 * @return		로딩 성공.
 *
 * @exception	에러가 발생할 경우 W3DException.
 */
BOOL CW3DUInterface::Load( const char* szFileName, const char* szStateName, int nWidth, int nHeight )
{
	Assert( szFileName );
	Assert( szStateName );

	if( !LoadState( szStateName ) )
	{
		Assert( !"State 읽기 실패" );
		return FALSE;
	} //if

	FILE*	pFile = fopen( szFileName, "rt" );
	if( !pFile )
	{
		Assert( !"파일을 찾을 수 없습니다." );
		return FALSE;
	} //if

	Cleanup();

	m_nWidth	= nWidth, m_nHeight	= nHeight;

	while( 1 )
	{
		char	szRead[64];

		if( fscanf( pFile, "%s", szRead ) == EOF )
		{
			break;
		} //if

		if( strcmp( "UI_FONT", szRead ) == 0 )
		{
			if( !ReadFont( pFile ) )
			{
				Assert( !"실패! 폰트" );
				throw W3DException( "유저 인터페이스 정보 읽기 실패" );
				return FALSE;
			} //if
		}
		else if( strcmp( "UI_DIALOGBOX", szRead ) == 0 )
		{
			if( !ReadUIDialog( pFile ) )
			{
				Assert( !"실패! 다이얼로그정보읽기" );
				throw W3DException( "유저 인터페이스 정보 읽기 실패" );
				return FALSE;
			} //if
		}
		else if( strcmp( "UI_CURSOR", szRead ) == 0 )
		{
			if( !ReadCursor( pFile ) )
			{
				Assert( !"커서읽기실패" );
				throw W3DException( "유저 인터페이스 정보 읽기 실패" );
				return FALSE;
			} //if
		}
		else if( strcmp( "UI_SPRITE", szRead ) == 0 )
		{
			if( !ReadSprite( pFile ) )
			{
				Assert( !"스프라이트읽기실패" );
				throw W3DException( "유저 인터페이스 정보 읽기 실패" );
				return FALSE;
			} //if
		} //if..else if..
	} //while

	fclose( pFile );

//	SetModeState( "[LOGIN_MODE]" );

	return TRUE;
} //CW3DUInterface::Load

/**
 * 유저 정보 저장
 */
void CW3DUInterface::SaveUserData( BOOL bNewPlayer )
{
	char			szFileName[64];
	SCharInfo*		pCharInfo = m_CharSelInfo.GetCharInfo();
	sprintf( szFileName, "User/%s.dat", pCharInfo->szName );
	
	FILE*		pFile = fopen( szFileName, "wb" );
	if( !pFile )
	{
		Assert( !"파일열기 실패" );
		return;
	} //if

	SaveQuickData( pFile, bNewPlayer );
//	SaveInventoryData( pFile );

	fclose( pFile );
} //CW3DUInterface::SaveUserData

/**
 * 유저 정보를 읽어 온다.
 */
BOOL CW3DUInterface::LoadQuickData()
{
	char			szFileName[64];
	SCharInfo*		pCharInfo = m_CharSelInfo.GetCharInfo();
	sprintf( szFileName, "User/%s.dat", pCharInfo->szName );

	FILE*	pFile = fopen( szFileName, "rb" );
	if( !pFile )
	{ // 새로 생성을 한다.
		SaveUserData( TRUE );
		return FALSE;
	} //if

	int				nVersion;
	fread( &nVersion, 1, sizeof( int ), pFile );
	if( nVersion != VER_USERDATAFILE )
	{
		fclose( pFile );
		SaveUserData( TRUE );
		return FALSE;
	} //if
	
	int			nSlotNum;
	fread( &nSlotNum, 1, sizeof( int ), pFile );
	if( nSlotNum <= 0 || nSlotNum > ( NUM_QUICKSLOT * NUM_QUICKSLOTBAR ) )
	{
		Assert( !"No Match Number of Slot" );
		fclose( pFile );
		SaveUserData( TRUE );
		return FALSE;
	} //if
	
	DWORD		adwData[NUM_QUICKSLOT * NUM_QUICKSLOTBAR];
	ZeroMemory( adwData, nSlotNum * sizeof( DWORD ) );

	fread( adwData, nSlotNum, sizeof( DWORD ), pFile );
	fclose( pFile );

	UIQuickSlot*	pQuickSlot;
	DWORD			dwSlotIndex;
	for( int nNumBar = 0; nNumBar < NUM_QUICKSLOTBAR; nNumBar++ )
	{
		pQuickSlot = GetQuickBar( nNumBar );
		if( !pQuickSlot )
		{
			Assert( !"퀵바없다" );
			continue;
		} //if

		for( int nNumSlot = 0; nNumSlot < NUM_QUICKSLOT; nNumSlot++ )
		{
			dwSlotIndex = ( nNumBar * NUM_QUICKSLOT ) + nNumSlot;
			if( adwData[dwSlotIndex] == 0 )
			{ //빈 곳.
				continue;
			} //if

			SItemInfoData*	pItemInfo = m_pItemInfoMgr->GetItemInfo( adwData[dwSlotIndex] );
			if( !pItemInfo )
			{
				Assert( !"퀵 등록 아이템 없음" );
				return FALSE;
			} //if

			SItemData*		pItem = NULL;
			if( pItemInfo->pSkillData )
			{
				DWORD		dwSkillId = pItemInfo->pSkillData->nItemNum;
				pItem = m_ItemMgr.FindItem( dwSkillId );
			}
			else
			{
				pItem = GetItemDataInInventory( adwData[dwSlotIndex] );
			} //if..else

			if( !pItem )
			{
				continue;
			} //if

			pQuickSlot->InsertSlot( nNumSlot, pItem );
		} //for
		RefreshQuickSlot( pQuickSlot );
	} //for

	return TRUE;
} //CW3DUInterface::LoadQuickData

/**
 * 유저 정보를 저장 한다.
 */
BOOL CW3DUInterface::SaveQuickData( FILE* pFile, BOOL bNewPlayer )
{
	Assert( pFile );

	DWORD		adwSlot[NUM_QUICKSLOT * NUM_QUICKSLOTBAR];
	ZeroMemory( adwSlot, sizeof( DWORD ) * NUM_QUICKSLOT * NUM_QUICKSLOTBAR );
	if( bNewPlayer )
	{
		adwSlot[0] = ITEM_SBASICSKILL;
		adwSlot[1] = ITEM_LBASICSKILL;
		adwSlot[2] = ITEM_BBASICSKILL;
		adwSlot[3] = ITEM_WBASICSKILL;
	}
	else
	{
		for( int nBarCnt = 0; nBarCnt < NUM_QUICKSLOTBAR; nBarCnt++ )
		{
			UIQuickSlot*	pQuickSlot = GetQuickBar( nBarCnt );
			if( !pQuickSlot )
			{
				Assert( !"퀵바 얻기 실패" );
				return FALSE;
			} //if

			for( int nSlotCnt = 0; nSlotCnt < NUM_QUICKSLOT; nSlotCnt++ )
			{
				SItemData*		pItemDat = pQuickSlot->GetItem( nSlotCnt );
				if( pItemDat )
				{
					int			nSaveIndex = ( NUM_QUICKSLOT * nBarCnt ) + nSlotCnt;
					adwSlot[nSaveIndex] = pItemDat->nType;
				} //if
			} //for
		} //for
	} //if..else

	int			nVersion = VER_USERDATAFILE;
	int			nSlotNum = NUM_QUICKSLOT * NUM_QUICKSLOTBAR;

	fwrite( &nVersion, 1, sizeof( int ), pFile );
	fwrite( &nSlotNum, 1, sizeof( int ), pFile );
	fwrite( &adwSlot, nSlotNum, sizeof( DWORD ), pFile );

	return TRUE;
} //CW3DUInterface::SaveQuickData

/**
 * 인벤토리 정보읽기
 */
BOOL CW3DUInterface::LoadInventoryData( WORD* pItemTypeBuf, int nBufCount )
{
	if( !pItemTypeBuf || nBufCount <= 0 )
	{
		Assert( !"버퍼오류" );
		return FALSE;
	} //if

	int			nPrevSkipDat =
		sizeof( int ) +
		sizeof( int ) +
		sizeof( DWORD ) * NUM_QUICKSLOT * NUM_QUICKSLOTBAR;

	char			szFileName[64];
	SCharInfo*		pCharInfo = m_CharSelInfo.GetCharInfo();
	sprintf( szFileName, "User/%s.dat", pCharInfo->szName );

	FILE*		pFile = fopen( szFileName, "rb" );
	if( !pFile )
	{
		Assert( !"파일열기실패" );
		return FALSE;
	} //if
	fseek( pFile, nPrevSkipDat, SEEK_SET );

	int			nPageNum;
	int			nItemNumInPage;

	fread( &nPageNum,		1, sizeof( int ),	pFile );
	fread( &nItemNumInPage, 1, sizeof( int ),	pFile );

	int			nTotItemNum = nItemNumInPage * nPageNum;
	if( nBufCount < nTotItemNum )
	{
		Assert( !"모자라는 할당" );
		fclose( pFile );
		return FALSE;
	} //if

	fread( pItemTypeBuf, nTotItemNum, sizeof( WORD ), pFile );

	fclose( pFile );
	return TRUE;
} //CW3DUInterface::LoadInventoryData

/**
 * 인벤토리 정보저장
 */
void CW3DUInterface::SaveInventoryData( FILE* pFile )
{
	int			nPageNum		= INVENTORY_PAGE;
	int			nItemNumInPage	= INVENTORY_ITEMNUM;
	int			nTotItemNum		= nItemNumInPage * nPageNum;

	WORD*		pItemTypeBuff	= (WORD*)_MemAlloc( sizeof( WORD ) * nTotItemNum );
	if( !pItemTypeBuff )
	{
		Assert( !"할당실패" );
		return;
	} //if
	ZeroMemory( pItemTypeBuff, sizeof( WORD ) * nTotItemNum );

	fwrite( &nPageNum,			1, sizeof( int ),	pFile );
	fwrite( &nItemNumInPage,	1, sizeof( int ),	pFile );

	int				nCount = 0;
	UIInventoryIN*	pInventory;

	for( int nCnt = 1 ; pInventory = GetInventoryIN( "INVENTORY_IN_", nCnt ); nCnt++ )
	{
		SItemData*	pItemData = NULL;
		for( BYTE btNumItem = 0; btNumItem < INVENTORY_ITEMNUM; btNumItem++ )
		{
			if( nCount >= nTotItemNum )
			{
				Assert( !"개수가 맞지 않소" );
				return;
			} //if
			pItemData = pInventory->GetItem( btNumItem );
			if( pItemData )
			{
				pItemTypeBuff[nCount] = pItemData->nType;
			}
			else
			{
				pItemTypeBuff[nCount] = 0;
			} //if
			nCount++;
		} //for
	} //for	

	fwrite( pItemTypeBuff, nTotItemNum, sizeof( WORD ), pFile );
	SAFE_FREE( pItemTypeBuff );
} //CW3DUInterface::SaveInventoryData

/**
 * ...
 */
BOOL CW3DUInterface::ReadFont( FILE* pFile )
{
	char	szRead[64];

	fscanf( pFile, "%s", szRead );
	if( strcmp( szRead, "BEGIN" ) != 0 )
	{
		return FALSE;
	} //if

	IW3DFont*		pFont = NULL;
	char			szFontName[64];
	char			szWeight[64];
	int				nSize;
	LONG			lWeight;

	while( fscanf( pFile, "%s", szRead ) != EOF )
	{
		if( strcmp( szRead, "END" ) == 0 )
		{
			return TRUE;
		}
		else
		{
			fscanf( pFile, "%s", szFontName );
			fscanf( pFile, "%d", &nSize );
			fscanf( pFile, "%s", szWeight );

			if( strcmp( szWeight, "BOLD" ) == 0 )
			{
				lWeight = FW_BOLD;
			}
			else 
			{
				lWeight = FW_NORMAL;
			} //if..else..

			if( !g_pScene->AddFont( NULL, szRead, szFontName, nSize, lWeight, FALSE, FALSE ) )
			{
				Assert( 0 );
				return FALSE;
			} //if
		} //if..else
	} //while

	return TRUE;
} //CW3DUInterface::ReadFont

/**
 * ...
 */
BOOL CW3DUInterface::ReadCursor( FILE* pFile )
{
	char	szRead[64];
	char	szRead2[64];
	
	fscanf( pFile, "%s", szRead );
	if( strcmp( szRead, "BEGIN" ) != 0 )
	{
		return FALSE;
	} //if

	IW3DSprite*		pCursor = NULL;
	int				nWidth;
	int				nHeight;
	HCURSOR			hCursor = NULL;

	while( fscanf( pFile, "%s", szRead ) != EOF )
	{
		if( strcmp( szRead, "END" ) == 0 )
		{
			SetCursorMode();
			return TRUE;
		}
		else
		{
			fscanf( pFile, "%d", &nWidth );
			fscanf( pFile, "%d", &nHeight );
			fscanf( pFile, "%s", szRead2 );

			if( m_bWinMouse )
			{
				continue;
				
				strcpy( szRead2 + strlen( szRead2 ) - 4, "w.wtm" );
				hCursor = (HCURSOR)LoadImage( NULL, szRead2, IMAGE_CURSOR, 32, 32, LR_LOADFROMFILE );
				if( !hCursor )
				{
					Assert( !"없는 윈도우커서" );
					return FALSE; 
				} //if
				m_mapWinCursor.insert( make_pair( (string)szRead, hCursor ) );
			}
			else
			{
				pCursor = MAKE_COMPONENT( Sprite );
				if( !pCursor->Load( szRead2, nWidth, nHeight ) )
				{
					SAFE_RELEASE( pCursor );
					Assert( !"텍스쳐 로드실패" );
					return FALSE;
				} //if

				if( strcmp( szRead, "CURSOR_EDIT" ) == 0 )
				{
					pCursor->SetCenter( nWidth >> 1, nHeight >> 1 );
				} //if

				m_mapCursor.insert( make_pair( (string)szRead, pCursor ) );
			} //if..else
		} //if..else..
	} //while

	return FALSE;
} //CW3DUInterface::ReadCursor

/**
 * ...
 */
BOOL CW3DUInterface::ReadSprite( FILE* pFile )
{
	char	szRead[64];
	char	szRead2[64];

	fscanf( pFile, "%s", szRead );
	if( strcmp( szRead, "BEGIN" ) != 0 )
	{
		return FALSE;
	} //if

	IW3DSprite*			pSprite = NULL;
	int					nWidth;
	int					nHeight;

	while( fscanf( pFile, "%s", szRead ) != EOF )
	{
		if( strcmp( szRead, "END" ) == 0 )
		{
			return TRUE;
		}
		else
		{
			fscanf( pFile, "%d", &nWidth );
			fscanf( pFile, "%d", &nHeight );
			fscanf( pFile, "%s", szRead2 );

			pSprite = MAKE_COMPONENT( Sprite );

			if( !pSprite->Load( szRead2, nWidth, nHeight ) )
			{
				SAFE_RELEASE( pSprite );
				Assert( !"텍스쳐 로드실패" );
				return FALSE;
			} //if

			m_mapSprite.insert( make_pair( (string)szRead, pSprite ) );
		} //if..else
	} //while

	return TRUE;
} //CW3DUInterface::ReadSprite

/**
 *
 */
BOOL CW3DUInterface::ReadUIDialog( FILE* pFile )
{
	char	szRead[64];

	UIDialog*	pDlg = new UIDialog;

	if( !pDlg )
	{
		Assert( !"메모리 할당 실패" );
		return FALSE;
	}

	m_listDialog.push_back( pDlg );

	pDlg->m_pOwnerComp  = NULL;
	pDlg->m_nType		= UI_DIALOGBOX;

	fscanf( pFile, "%s", szRead );
	AddComponent( szRead, (UICComponent*)pDlg );

	pDlg->m_strName	= szRead;						// 이름
	pDlg->m_nID		= GenerateID( szRead );			// 아이뒤

	if( pDlg->m_nID == FAILED_ID )
	{
		Assert( !"아뒤 실패" );
		return FALSE;
	}

	// 영역
	int		nWidth, nHeight, nStyle;
	fscanf( pFile, "%d", &nStyle );
	fscanf( pFile, "%d", &pDlg->m_rcRegion.left );
	fscanf( pFile, "%d", &pDlg->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	switch( nStyle ) 
	{
	case 0://그냥 사용해도 무관
		break;
	case 1:// 창 정 중앙에 위치시키기
		pDlg->m_rcRegion.left	= (int)( ( m_nWidth / 2 ) - ( nWidth / 2 ) );
		pDlg->m_rcRegion.top	= (int)( ( m_nHeight / 2 ) - ( nHeight / 2 ) );
		break;
	case 2:// 채팅창
		pDlg->m_rcRegion.top	= m_nHeight - nHeight;
		break;
	case 3://
		pDlg->m_rcRegion.left	= m_nWidth - nWidth;
		pDlg->m_rcRegion.top	= m_nHeight - nHeight;
		break;
	case 4://
		pDlg->m_rcRegion.left	= m_nWidth - nWidth;
		pDlg->m_rcRegion.top	= ( ( m_nHeight - 47 ) - nHeight );
		break;
	case 5://
		pDlg->m_rcRegion.left	= ( m_nWidth - nWidth ) - 3;
		pDlg->m_rcRegion.top	= 3;
		break;
	case 6://
		pDlg->m_rcRegion.left	= ( m_nWidth - nWidth ) - nWidth;
		pDlg->m_rcRegion.top	= ( ( m_nHeight - 47 ) - nHeight );
		break;
	case 7://
		pDlg->m_rcRegion.top	= ( ( m_nHeight - 47 ) - nHeight );
		break;
	case 8://
		pDlg->m_rcRegion.top	= ( ( ( m_nHeight - 47 ) - nHeight ) - nHeight) - 1;
		break;
	case 9://
		pDlg->m_rcRegion.left	= ( m_nWidth - nWidth ) - 3;
		pDlg->m_rcRegion.top	= 3 + 35;
		break;	
	case 10://
		pDlg->m_rcRegion.left	= (int)( ( m_nWidth / 2 ) - ( nWidth / 2 ) );
		pDlg->m_rcRegion.top	= 3;
		break;
	case 11: //스킬프로그래시브
		nWidth					= m_nWidth;
		pDlg->m_rcRegion.top	= ( ( m_nHeight - 29 ) - nHeight );
		break;
	case 12://로그인창
		pDlg->m_rcRegion.left	= (int)( ( m_nWidth / 2 ) - ( nWidth / 2 ) );
		pDlg->m_rcRegion.top	= (int)( ( m_nHeight / 6 ) * 4 );
		break;
	case 13://캐릭터 설정
		pDlg->m_rcRegion.left	= (int)( ( m_nWidth / 2 ) - ( nWidth / 2 ) );
		pDlg->m_rcRegion.top	= 93;
		break;
	case 14://1캐릭터
		pDlg->m_rcRegion.left	= (int)( ( m_nWidth / 3 ) / 4 );
		pDlg->m_rcRegion.top	= (int)( ( m_nHeight / 6 ) * 3 );
		break;
	case 15:
		pDlg->m_rcRegion.left	= (int)( m_nWidth / 3 );
		pDlg->m_rcRegion.top	= (int)( ( m_nHeight / 6 ) * 3 );
		break;
	case 16:
		pDlg->m_rcRegion.left	= (int)( ( m_nWidth / 3 ) * 2 );
		pDlg->m_rcRegion.top	= (int)( ( m_nHeight / 6 ) * 3 );
		break;
	case 17://캐릭선택화면에서의 캐릭정보
		pDlg->m_rcRegion.left	= 15;
		pDlg->m_rcRegion.top	= ( m_nHeight - 15 ) - nHeight;
		break;	
	case 18:
		pDlg->m_rcRegion.left	= ( m_nWidth - 15 ) - nWidth;
		pDlg->m_rcRegion.top	= ( m_nHeight - 15 ) - nHeight;
		break;
	case 19:
		pDlg->m_rcRegion.left	= (int)( ( m_nWidth / 2 ) - ( nWidth / 2 ) );
		pDlg->m_rcRegion.top	= ( m_nHeight - 15 ) - nHeight;
		break;
	case 20:
		pDlg->m_rcRegion.left	= 355;
		pDlg->m_rcRegion.top	= m_nHeight - 38;
		if( m_nWidth == 800 )
		{
			nWidth = 322;
		}
		else if( m_nWidth == 1024 )
		{
			nWidth = 545;
		}
		else if( m_nWidth == 1280 )
		{
			nWidth = 801;
		}
		else if( m_nWidth == 1360 )
		{
			nWidth = 881;
		}
		else if( m_nWidth == 1400 )
		{
			nWidth = 921;
		}
		else if( m_nWidth == 1440 )
		{
			nWidth = 960;
		}
		else if( m_nWidth == 1600 )
		{
			nWidth = 1121;
		}
		else if( m_nWidth == 1680 )
		{
			nWidth = 1201;
		}
		break;
	case 21:
		pDlg->m_rcRegion.top	= ( m_nHeight - 47 ) - nHeight;
		break;
	}//switch

	pDlg->m_rcRegion.right	= pDlg->m_rcRegion.left + nWidth;
	pDlg->m_rcRegion.bottom	= pDlg->m_rcRegion.top + nHeight;
	
	fscanf( pFile, "%s", szRead );
	if( strcmp(  "NOTITLEBAR", szRead ) )
	{
		IW3DFont*		pFont;
		if( !g_pScene->FindFont( &pFont, "GULLIM12" ) )
		{
			Assert( !"폰트가 없당" );
			return FALSE;
		} //if
		pDlg->SetTitleBar( szRead, pFont );
	} //if

	fscanf( pFile, "%s", szRead );
	if( !strcmp(  "NONE", szRead ) )			pDlg->SetAlways( FALSE );
	else if( !strcmp(  "STATIC", szRead ) )		pDlg->SetAlways( TRUE );

	fscanf( pFile, "%s", szRead );
	if( !strcmp(  "NONE", szRead ) )			pDlg->SetTopMost( FALSE );
	else if( !strcmp(  "TOPMOST", szRead ) )	pDlg->SetTopMost( TRUE );

	if( nStyle == 20 )
	{
		fscanf( pFile, "%s", szRead );
		sprintf( szRead, "%s-%d.tga", szRead, m_nWidth );
	}
	else
	{
		fscanf( pFile, "%s", szRead );
	}

	if( !strcmp( szRead, "NULL" ) )					///< 텍스처 이름이 "NULL"인 경우.
	{ //없다.
		if( !pDlg->Load( NULL ) )
		{
			Assert( !"텍스쳐 로딩 실패" );
			return FALSE;
		}
	}
	else if( !strcmp( szRead, "RECTANGLE" ) )		///< 텍스처 이름이 "RECTANGLE"인 경우.
	{ //박스.
		if( !pDlg->InitRectangle() )
		{
			Assert( !"텍스쳐 로딩 실패" );
			return FALSE;
		}
	}
	else	
	{ //일반적인 텍스처.
		if( !pDlg->Load( szRead ) )
		{
			Assert( !"텍스쳐 로딩 실패" );
			return FALSE;
		}
	} //if..else..

	fscanf( pFile, "%s", szRead );	
	if( strcmp( "BEGIN", szRead ) != 0 )
	{
		return FALSE;
	} //if

	while( 1 )
	{
		if( fscanf( pFile, "%s", szRead ) == EOF )
		{
			return FALSE;
		} //if

		if( !strcmp( "END", szRead ) )
		{
			break;
		}
		else if( !strcmp( "UI_BUTTON", szRead ) )
		{
			UIButton*	pButton = ReadUIButton( (UICComponent*)pDlg, pFile );

			if( !pButton )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pButton );
		}
		else if( !strcmp( "UI_CHECKBUTTON", szRead ) )
		{
			UICheckButton*	pCheckButton = ReadUICheckButton( (UICComponent*)pDlg, pFile );

			if( !pCheckButton )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pCheckButton );
		}
		else if( !strcmp( "UI_EDITTEXT", szRead ) )
		{
			UIEditText*		pEditText = ReadUIEditText( (UICComponent*)pDlg, pFile );

			if( !pEditText )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pEditText );
		}
		else if( !strcmp( "UI_STATICTEXT", szRead ) )
		{
			UIStaticText*	pStaticText = ReadUIStaticText( (UICComponent*)pDlg, pFile );

			if( !pStaticText )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pStaticText );
		}
		else if( !strcmp( "UI_TABBUTTON", szRead ) )
		{
			UITabButton*	pTab = ReadUITab( (UICComponent*)pDlg, pFile );

			if( !pTab )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pTab );
		}
		else if( !strcmp( "UI_SCROLLBAR", szRead ) )
		{
			UIScrollBar*	pScrollBar = ReadUIScrollBar( (UICComponent*)pDlg, pFile );

			if( !pScrollBar )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pScrollBar );
		}
		else if( !strcmp( "UI_STRLIST", szRead ) )
		{
			UIStringList*	pStrList = ReadUIStringList( (UICComponent*)pDlg, pFile );

			if( !pStrList )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pStrList );
		}
		else if( !strcmp( "UI_SELECTSTRING", szRead ) )
		{
			UISelectString*		pSelectString = ReadUISelectString( (UICComponent*)pDlg, pFile );

			if( !pSelectString )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pSelectString );
		}
		else if( !strcmp( "UI_PROGRESS", szRead )  )
		{
			UIProgress*		pProgress = ReadUIProgress( (UICComponent*)pDlg, pFile );

			if( !pProgress )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pProgress );
		}
		else if( !strcmp( "UI_PROGRESSEX", szRead ) )
		{
			UIProgressEx*		pProgress = ReadUIProgressEx( (UICComponent*)pDlg, pFile );

			if( !pProgress )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pProgress );
		}
		else if( !strcmp( "UI_INVENTORYIN", szRead ) )
		{
			UIInventoryIN*		pInvenIN = ReadUIInventoryIN( (UICComponent*)pDlg, pFile );

			if( !pInvenIN )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pInvenIN );
		}
		else if( !strcmp( "UI_INVENTORYON", szRead ) )
		{
			UIInventoryON*		pInvenON = ReadUIInventoryON( (UICComponent*)pDlg, pFile );

			if( !pInvenON )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pInvenON );
		}
		else if( !strcmp( "UI_PICTURE", szRead ) )
		{
			UIPicture*		pPicture = ReadUIPicture( (UICComponent*)pDlg, pFile );

			if( !pPicture )
			{
				Assert( 0 );
				return FALSE;
			} //if

			pDlg->AddControl( (UICComponent*)pPicture );
		}
		else if( !strcmp( "UI_MINIMAP", szRead ) )
		{
			UIMiniMap*		pMiniMap = ReadUIMiniMap(
				reinterpret_cast<UICComponent*>( pDlg ),
				pFile );

			if( !pMiniMap )
			{
				Assert( !"미니맵 생성 실패" );
				return FALSE;
			} //if

			pDlg->AddControl( reinterpret_cast<UICComponent*>( pMiniMap ) );
		}
		else if( !strcmp( "UI_QUICKSLOT", szRead ) )
		{
			UIQuickSlot*	pQuickSlot = ReadUIQuickSlot(
				reinterpret_cast<UICComponent*>( pDlg ),
				pFile );

			if( !pQuickSlot )
			{
				Assert( !"퀵 슬롯 실패" );
				return FALSE;
			} //if

			pDlg->AddControl( reinterpret_cast<UICComponent*>( pQuickSlot ) );
		}
		else if( !strcmp( "UI_SKILLENTRY", szRead ) )
		{
			UISkillEntry*	pSkillEntry = ReadUISkillEntry(
				reinterpret_cast<UICComponent*>( pDlg ),
				pFile );

			if( !pSkillEntry )
			{
				Assert( !"스킬창 생성 실패" );
				return FALSE;
			} //if

			pDlg->AddControl( reinterpret_cast<UICComponent*>( pSkillEntry ) );
		}
		else if( !strcmp( "UI_IMAGELIST", szRead ) )
		{
			UIImageList*	pImageList = ReadUIImageList(
				reinterpret_cast<UICComponent*>( pDlg ),
				pFile );

			if( !pImageList )
			{
				Assert( !"이미지 리스트 생성 실패" );
				return FALSE;
			} //if

			pDlg->AddControl( reinterpret_cast<UICComponent*>( pImageList ) );
		}
		else if( !strcmp( "UI_TIMER", szRead ) )
		{
			UITimer*		pTimer = ReadUITimer( 
				reinterpret_cast<UICComponent*>( pDlg ),
				pFile );

			if( !pTimer )
			{
				Assert( !"타임머 생성 실패" );
				return FALSE;
			} //if

			pDlg->AddControl( reinterpret_cast<UICComponent*>( pTimer ) );
		}
		else if( !strcmp( "UI_RECTANGLE", szRead ) )
		{
			UIRectangle*	pRectangle = ReadUIRectangle(
				reinterpret_cast<UICComponent*>( pDlg ),
				pFile );

			if( !pRectangle )
			{
				Assert( !"스킬창 생성 실패" );
				return FALSE;
			} //if

			pDlg->AddControl( reinterpret_cast<UICComponent*>( pRectangle ) );
		}
		else if( !strcmp( "UI_SHEET", szRead ) )
		{
			UISheet*	pSheet = ReadUISheet(
				dynamic_cast<UICComponent*>( pDlg ),
				pFile );

			if( !pSheet )
			{
				Assert( !"시트 생성실패" );
				return FALSE;
			} //if

			pDlg->AddControl( reinterpret_cast<UICComponent*>( pSheet ) );
		}
/*		else if( !strcmp( "UI_DLGLIST", szRead ) )
		{
			UIDlgList*	pDlgList = ReadUIDlgList(
				dynamic_cast<UICComponent*>( pDlg ),
				pFile );

			if( !pDlgList )
			{
				Assert( !"디엘지 리스트 생성실패" );
				return FALSE;
			} //if

			pDlg->AddControl( reinterpret_cast<UICComponent*>( pDlgList ) );
		} //if..else..
*/	} //while

	return TRUE;
} //CW3DUInterface::ReadUIDialog


/**
 *
 */
UIButton* CW3DUInterface::ReadUIButton( UICComponent* pComp, FILE* pFile )
{
	UIButton*	pButton = new UIButton;

	if( !pButton )
	{
		Assert( !"버튼실패" );
		return NULL;
	} //if

	char	szRead[64];
	char	szRead2[64];
	char	szRead3[64];
	char	szRead4[64];

	pButton->m_pOwnerComp	= (UICComponent*)pComp;
	pButton->m_nType		= UI_PUSHBUTTON;

	fscanf( pFile, "%s", szRead );
	AddComponent( szRead, (UICComponent*)pButton );
	pButton->m_strName = szRead;
	pButton->m_nID = GenerateID( szRead );				// 아이뒤
	if( pButton->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pButton );
		Assert( !"아뒤 실패" );
		return NULL;
	} //if

	int		nWidth, nHeight;
	fscanf( pFile, "%d", &pButton->m_rcRegion.left );
	fscanf( pFile, "%d", &pButton->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pButton->m_rcRegion.right	= pButton->m_rcRegion.left + nWidth;
	pButton->m_rcRegion.bottom	= pButton->m_rcRegion.top + nHeight;

	pButton->m_rcRegion.left	+= pComp->m_rcRegion.left;
	pButton->m_rcRegion.top		+= pComp->m_rcRegion.top;
	pButton->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pButton->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	fscanf( pFile, "%s", szRead );
	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, szRead ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	}
	pButton->SetFont( pFont );

	fscanf( pFile, "%s", szRead );
	if( strcmp( szRead, "NULL" ) != 0 )
	{
		pButton->SetText( szRead );		
	} //if
	
	fscanf( pFile, "%s", szRead );
	if( strcmp( szRead, "NULL" ) != 0 )
	{
		pButton->SetInfoText( szRead );		
	} //if

	BYTE		btRGB[3];
	fscanf( pFile, "%d", &btRGB[0] );
	fscanf( pFile, "%d", &btRGB[1] );
	fscanf( pFile, "%d", &btRGB[2] );
	pButton->SetTxtColor( btRGB[0], btRGB[1], btRGB[2] );

	fscanf( pFile, "%s", szRead );
	fscanf( pFile, "%s", szRead2 );
	fscanf( pFile, "%s", szRead3 );
	fscanf( pFile, "%s", szRead4 );

	if( !pButton->Load( strcmp( szRead, "NULL" ) == 0 ? NULL : szRead, 
						strcmp( szRead2, "NULL" ) == 0 ? NULL : szRead2, 
						strcmp( szRead3, "NULL" ) == 0 ? NULL : szRead3,
						strcmp( szRead4, "NULL" ) == 0 ? NULL : szRead4 ) )
	{
		Log( "%d", pButton->m_strName );
		SAFE_RELEASE( pButton );
		Assert( !"텍스쳐 실패" );
		return NULL;
	} //if

	return pButton;
} //CW3DUInterface::ReadUIButton

/**
 *
 */
UICheckButton* CW3DUInterface::ReadUICheckButton( UICComponent* pComp, FILE* pFile )
{
	UICheckButton*	pCheckButton = new UICheckButton;

	if( !pCheckButton )
	{
		Assert( !"버튼실패" );
		return NULL;
	} //if

	char	szRead[64];
	char	szRead2[64];

	pCheckButton->m_pOwnerComp	= (UICComponent*)pComp;
	pCheckButton->m_nType		= UI_CHECKBUTTON;

	fscanf( pFile, "%s", szRead );
	AddComponent( szRead, (UICComponent*)pCheckButton );
	pCheckButton->m_strName = szRead;
	pCheckButton->m_nID = GenerateID( szRead );				// 아이뒤
	if( pCheckButton->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pCheckButton );
		Assert( !"아뒤 실패" );
		return NULL;
	} //if

	int		nWidth, nHeight;
	fscanf( pFile, "%d", &pCheckButton->m_rcRegion.left );
	fscanf( pFile, "%d", &pCheckButton->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pCheckButton->m_rcRegion.right	= pCheckButton->m_rcRegion.left + nWidth;
	pCheckButton->m_rcRegion.bottom	= pCheckButton->m_rcRegion.top + nHeight;

	pCheckButton->m_rcRegion.left	+= pComp->m_rcRegion.left;
	pCheckButton->m_rcRegion.top	+= pComp->m_rcRegion.top;
	pCheckButton->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pCheckButton->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	fscanf( pFile, "%s", szRead );
	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, szRead ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	}
	pCheckButton->SetFont( pFont );

	fscanf( pFile, "%s", szRead );
	if( strcmp( szRead, "NULL" ) != 0 )
	{
		pCheckButton->SetText( szRead );		
	} //if

	BYTE		btRGB[3];
	fscanf( pFile, "%d", &btRGB[0] );
	fscanf( pFile, "%d", &btRGB[1] );
	fscanf( pFile, "%d", &btRGB[2] );
	pCheckButton->SetTxtColor( btRGB[0], btRGB[1], btRGB[2] );

	fscanf( pFile, "%s", szRead );
	fscanf( pFile, "%s", szRead2 );

	if( !pCheckButton->Load( szRead, szRead2 ) )
	{
		SAFE_RELEASE( pCheckButton );
		Assert( !"텍스쳐 실패" );
		return NULL;
	} //if

	return pCheckButton;
} //CW3DUInterface::ReadUICheckButton

/**
 *
 */
UIEditText* CW3DUInterface::ReadUIEditText( UICComponent* pComp, FILE* pFile )
{
	UIEditText*		pEdtText = new UIEditText;
	if( !pEdtText )
	{
		Assert( !"에디트텍스트실패" );
		return NULL;
	}

	char		pRead[64];
	UINT		nLimit;

	pEdtText->m_pOwnerComp = (UICComponent*)pComp;
	pEdtText->m_nType	   = UI_EDITTEXT;
	fscanf( pFile, "%s", pRead );
	AddComponent( pRead, (UICComponent*)pEdtText );
	pEdtText->m_strName = pRead;
	pEdtText->m_nID = GenerateID( pRead );				// 아이뒤
	if( pEdtText->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pEdtText );
		Assert( !"아뒤 실패" );
		return NULL;
	}

	int		nWidth, nHeight;
	fscanf( pFile, "%d", &pEdtText->m_rcRegion.left );
	fscanf( pFile, "%d", &pEdtText->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pEdtText->m_rcRegion.right	= pEdtText->m_rcRegion.left + nWidth;
	pEdtText->m_rcRegion.bottom	= pEdtText->m_rcRegion.top + nHeight;

	pEdtText->m_rcRegion.left	+= pComp->m_rcRegion.left;
	pEdtText->m_rcRegion.top	+= pComp->m_rcRegion.top;
	pEdtText->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pEdtText->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	fscanf( pFile, "%s", pRead );							// 정이한 폰트
	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, pRead ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	}
	pEdtText->SetFont( pFont );

//	fscanf( pFile, "%s", pRead );						// 내용
//	pEdtText->SetText( pRead );

	fscanf( pFile, "%d", &nLimit );
	pEdtText->SetLimit( nLimit );

	fscanf( pFile, "%s", pRead );
	if( strcmp( pRead, "CENTER" ) == 0 )
	{
		pEdtText->SetAlign( UIEditText::ALIGN_CENTER );
	}
	else if( strcmp( pRead, "LEFT" ) == 0 )
	{
		pEdtText->SetAlign( UIEditText::ALIGN_LEFT );
	}
	else if( strcmp( pRead, "RIGHT" ) == 0 )
	{
		pEdtText->SetAlign( UIEditText::ALIGN_RIGHT );
	}

	fscanf( pFile, "%s", pRead );
	if( strcmp( pRead, "HIDE" ) == 0 )
	{
		pEdtText->SetMaskMode( TRUE );
	}
	else
	{
		pEdtText->SetMaskMode( FALSE );
	}

	BYTE		btRGB[3];
	fscanf( pFile, "%d", &btRGB[0] );
	fscanf( pFile, "%d", &btRGB[1] );
	fscanf( pFile, "%d", &btRGB[2] );
	pEdtText->SetTxtColor( btRGB[0], btRGB[1], btRGB[2] );

	return pEdtText;
} //CW3DUInterface::ReadUIEditText

/**
 *
 */
UIStaticText* CW3DUInterface::ReadUIStaticText( UICComponent* pComp, FILE* pFile )
{
	UIStaticText*		pStaText = new UIStaticText;
	if( !pStaText )
	{
		Assert( !"스태틱텍스트실패" );
		return NULL;
	}

	char		pRead[64];

	pStaText->m_pOwnerComp = (UICComponent*)pComp;
	pStaText->m_nType	   = UI_STATICTEXT;
	fscanf( pFile, "%s", pRead );
	AddComponent( pRead, (UICComponent*)pStaText );
	pStaText->m_strName = pRead;
	pStaText->m_nID = GenerateID( pRead );				// 아이뒤
	if( pStaText->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pStaText );
		Assert( !"아뒤 실패" );
		return NULL;
	}
	
	int		nWidth, nHeight;
	fscanf( pFile, "%d", &pStaText->m_rcRegion.left );
	fscanf( pFile, "%d", &pStaText->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pStaText->m_rcRegion.right	= pStaText->m_rcRegion.left + nWidth;
	pStaText->m_rcRegion.bottom	= pStaText->m_rcRegion.top + nHeight;

	pStaText->m_rcRegion.left	+= pComp->m_rcRegion.left;
	pStaText->m_rcRegion.top	+= pComp->m_rcRegion.top;
	pStaText->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pStaText->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	fscanf( pFile, "%s", pRead );							// 정이한 폰트
	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, pRead ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	}
	pStaText->SetFont( pFont );

	fscanf( pFile, "%s", pRead );						// 내용
	if( !strcmp( pRead, "NULL" ) )
	{
		pStaText->SetText( "" );	
	}
	else
	{
		pStaText->SetText( pRead );
	}

	fscanf( pFile, "%s", pRead );
	if( strcmp( pRead, "CENTER" ) == 0 )
	{
		pStaText->SetAlign( UIStaticText::ALIGN_CENTER );
	}
	else if( strcmp( pRead, "LEFT" ) == 0 )
	{
		pStaText->SetAlign( UIStaticText::ALIGN_LEFT );
	}
	else if( strcmp( pRead, "RIGHT" ) == 0 )
	{
		pStaText->SetAlign( UIStaticText::ALIGN_RIGHT );
	}

	BYTE		btRGB[3];
	fscanf( pFile, "%d", &btRGB[0] );
	fscanf( pFile, "%d", &btRGB[1] );
	fscanf( pFile, "%d", &btRGB[2] );
	pStaText->SetTxtColor( btRGB[0], btRGB[1], btRGB[2] );
	
	return pStaText;
} //CW3DUInterface::ReadUIStaticText

/**
 *
 */
UITabButton* CW3DUInterface::ReadUITab( UICComponent* pComp, FILE* pFile )
{
	UITabButton*	pTab = new UITabButton;
	if( !pTab )
	{
		Assert( !"탭실패" );
		return NULL;
	}

	char		pRead[64];

	pTab->m_pOwnerComp  = (UICComponent*)pComp;
	pTab->m_nType		= UI_TABBUTTON;
	fscanf( pFile, "%s", pRead );
	AddComponent( pRead, (UICComponent*)pTab );
	pTab->m_strName = pRead;
	pTab->m_nID = GenerateID( pRead );				// 아이뒤
	if( pTab->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pTab );
		Assert( !"아뒤 실패" );
		return NULL;
	}

	int		nWidth, nHeight;
	fscanf( pFile, "%d", &pTab->m_rcRegion.left );
	fscanf( pFile, "%d", &pTab->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pTab->m_rcRegion.right	= pTab->m_rcRegion.left + nWidth;
	pTab->m_rcRegion.bottom	= pTab->m_rcRegion.top + nHeight;

	pTab->m_rcRegion.left	+= pComp->m_rcRegion.left;
	pTab->m_rcRegion.top	+= pComp->m_rcRegion.top;
	pTab->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pTab->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	int			nTabCount, nCnt;
	RECT		rcTab;
	fscanf( pFile, "%d", &nTabCount );
	pTab->SetCount( nTabCount );
	for( nCnt = 0; nCnt < nTabCount; nCnt++ )
	{
		fscanf( pFile, "%d", &rcTab.left );	// 영역
		fscanf( pFile, "%d", &rcTab.top );
		fscanf( pFile, "%d", &nWidth );
		fscanf( pFile, "%d", &nHeight );
		rcTab.left		+= pTab->m_rcRegion.left;
		rcTab.top		+= pTab->m_rcRegion.top;
		rcTab.right		= rcTab.left + nWidth;
		rcTab.bottom	= rcTab.top + nHeight;
		fscanf( pFile, "%s", pRead );

		pTab->AddTabInfo( pRead, &rcTab );
	} //for

	fscanf( pFile, "%s", pRead );
	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, pRead ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	}
	pTab->SetFont( pFont );

	BYTE		btRGB[3];
	fscanf( pFile, "%d", &btRGB[0] );
	fscanf( pFile, "%d", &btRGB[1] );
	fscanf( pFile, "%d", &btRGB[2] );
	pTab->SetTxtColor( btRGB[0], btRGB[1], btRGB[2] );

	fscanf( pFile, "%s", pRead );
	if( !pTab->Load( pRead ) )
	{
		SAFE_RELEASE( pTab );
		Assert( !"텍스쳐 실패" );
		return NULL;
	}

	return pTab;
} //CW3DUInterface::ReadUITab

/**
 *
 */
UIScrollBar* CW3DUInterface::ReadUIScrollBar( UICComponent* pComp, FILE* pFile )
{
	UIScrollBar*	pScrollBar = new UIScrollBar;
	if( !pScrollBar )
	{
		Assert( !"스크롤바실패" );
		return NULL;
	}

	char		pRead[64];

	pScrollBar->m_pOwnerComp  = (UICComponent*)pComp;
	pScrollBar->m_nType		  = UI_SCROLLBAR;
	fscanf( pFile, "%s", pRead );
	AddComponent( pRead, (UICComponent*)pScrollBar );
	pScrollBar->m_strName = pRead;
	pScrollBar->m_nID	  = GenerateID( pRead );				// 아이뒤
	if( pScrollBar->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pScrollBar );
		Assert( !"아뒤 실패" );
		return NULL;
	} //if

	int		nWidth, nHeight;
	fscanf( pFile, "%d", &pScrollBar->m_rcRegion.left );
	fscanf( pFile, "%d", &pScrollBar->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pScrollBar->m_rcRegion.right	= pScrollBar->m_rcRegion.left + nWidth;
	pScrollBar->m_rcRegion.bottom	= pScrollBar->m_rcRegion.top + nHeight;

	pScrollBar->m_rcRegion.left		+= pComp->m_rcRegion.left;
	pScrollBar->m_rcRegion.top		+= pComp->m_rcRegion.top;
	pScrollBar->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pScrollBar->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	int			nMax;
	fscanf( pFile, "%d", &nMax );
	pScrollBar->SetMax( nMax );

	BOOL		bType;
	fscanf( pFile, "%s", pRead );
	if( !strcmp( pRead, "HORIZONTAL" ) )		bType = UIScrollBar::HORIZONTAL;
	else if( !strcmp( pRead, "VERTICAL" ) )		bType = UIScrollBar::VERTICAL;
	else										Assert( 0 );

	pScrollBar->SetType( bType );
	
	pScrollBar->Init();

	return pScrollBar;
} //CW3DUInterface::ReadUIScrollBar

/**
 *
 */
UIStringList* CW3DUInterface::ReadUIStringList( UICComponent* pComp, FILE* pFile )
{
	UIStringList*	pStringList = new UIStringList;
	if( !pStringList )
	{
		Assert( !"스트링리스트실패" );
		return NULL;
	}

	char		pRead[64];
	int			nDisplayCount, nMax;

	pStringList->m_pOwnerComp = (UICComponent*)pComp;
	pStringList->m_nType	   = UI_STRLIST;
	fscanf( pFile, "%s", pRead );
	AddComponent( pRead, (UICComponent*)pStringList );
	pStringList->m_strName = pRead;
	pStringList->m_nID = GenerateID( pRead );				// 아이뒤
	if( pStringList->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pStringList );
		Assert( !"아뒤 실패" );
		return NULL;
	}

	int		nWidth, nHeight;
	fscanf( pFile, "%d", &pStringList->m_rcRegion.left );
	fscanf( pFile, "%d", &pStringList->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pStringList->m_rcRegion.right	= pStringList->m_rcRegion.left + nWidth;
	pStringList->m_rcRegion.bottom	= pStringList->m_rcRegion.top + nHeight;

	pStringList->m_rcRegion.left	+= pComp->m_rcRegion.left;
	pStringList->m_rcRegion.top		+= pComp->m_rcRegion.top;
	pStringList->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pStringList->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	fscanf( pFile, "%s", pRead );							// 정이한 폰트
	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, pRead ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	}
	pStringList->SetFont( pFont, 0 );

	fscanf( pFile, "%s", pRead );
	if( strcmp( pRead, "CENTER" ) == 0 )
	{
		pStringList->SetAlign( UIStringList::ALIGN_CENTER );
	}
	else if( strcmp( pRead, "LEFT" ) == 0 )
	{
		pStringList->SetAlign( UIStringList::ALIGN_LEFT );
	}
	else if( strcmp( pRead, "RIGHT" ) == 0 )
	{
		pStringList->SetAlign( UIStringList::ALIGN_RIGHT );
	}

	BYTE		btRGB[3];
	fscanf( pFile, "%d", &btRGB[0] );
	fscanf( pFile, "%d", &btRGB[1] );
	fscanf( pFile, "%d", &btRGB[2] );
	pStringList->SetTxtColor( btRGB[0], btRGB[1], btRGB[2] );

	fscanf( pFile, "%d", &nDisplayCount );
	fscanf( pFile, "%d", &nMax );
	pStringList->SetCount( nDisplayCount );
	pStringList->SetMax( nMax );

	return pStringList;
} //CW3DUInterface::ReadUIStringList

/**
 *
 */
UISelectString* CW3DUInterface::ReadUISelectString( UICComponent* pComp, FILE* pFile )
{
	UISelectString*		pSelectString = new UISelectString;
	if( !pSelectString )
	{
		Assert( !"스트링리스트실패" );
		return NULL;
	}

	char		pRead[64];
	int			nMaxCount;

	pSelectString->m_pOwnerComp = (UICComponent*)pComp;
	pSelectString->m_nType	    = UI_SELECTSTRING;
	fscanf( pFile, "%s", pRead );
	AddComponent( pRead, (UICComponent*)pSelectString );
	pSelectString->m_strName = pRead;
	pSelectString->m_nID = GenerateID( pRead );				// 아이뒤
	if( pSelectString->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pSelectString );
		Assert( !"아뒤 실패" );
		return NULL;
	} //if

	int		nWidth, nHeight;
	fscanf( pFile, "%d", &pSelectString->m_rcRegion.left );
	fscanf( pFile, "%d", &pSelectString->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pSelectString->m_rcRegion.right		= pSelectString->m_rcRegion.left + nWidth;
	pSelectString->m_rcRegion.bottom	= pSelectString->m_rcRegion.top + nHeight;

	pSelectString->m_rcRegion.left		+= pComp->m_rcRegion.left;
	pSelectString->m_rcRegion.top		+= pComp->m_rcRegion.top;
	pSelectString->m_rcRegion.right		+= pComp->m_rcRegion.left;
	pSelectString->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	fscanf( pFile, "%s", pRead );							// 정이한 폰트
	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, pRead ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	}
	pSelectString->SetFont( pFont );

	fscanf( pFile, "%s", pRead );
	if( strcmp( pRead, "CENTER" ) == 0 )
	{
		pSelectString->SetAlign( UISelectString::ALIGN_CENTER );
	}
	else if( strcmp( pRead, "LEFT" ) == 0 )
	{
		pSelectString->SetAlign( UISelectString::ALIGN_LEFT );
	}
	else if( strcmp( pRead, "RIGHT" ) == 0 )
	{
		pSelectString->SetAlign( UISelectString::ALIGN_RIGHT );
	}

	fscanf( pFile, "%d", &nMaxCount );

	if( !pSelectString->Init( nMaxCount ) )
	{
		return NULL;
	} //if

	return pSelectString;
} //CW3DUInterface::ReadUISelectString

/**
 *
 */
UIProgress* CW3DUInterface::ReadUIProgress( UICComponent* pComp, FILE* pFile )
{
	UIProgress*		pProgress = new UIProgress;
	if( !pProgress )
	{
		Assert( !"버튼실패" );
		return NULL;
	}

	char		pRead[64];
	
	pProgress->m_pOwnerComp  = (UICComponent*)pComp;
	pProgress->m_nType		 = UI_PROGRESS;
	fscanf( pFile, "%s", pRead );
	AddComponent( pRead, (UICComponent*)pProgress );
	pProgress->m_strName = pRead;
	pProgress->m_nID = GenerateID( pRead );				// 아이뒤
	if( pProgress->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pProgress );
		Assert( !"아뒤 실패" );
		return NULL;
	}

	int		nWidth, nHeight;
	fscanf( pFile, "%d", &pProgress->m_rcRegion.left );
	fscanf( pFile, "%d", &pProgress->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pProgress->m_rcRegion.right		= pProgress->m_rcRegion.left + nWidth;
	pProgress->m_rcRegion.bottom	= pProgress->m_rcRegion.top + nHeight;

	pProgress->m_rcRegion.left		+= pComp->m_rcRegion.left;
	pProgress->m_rcRegion.top		+= pComp->m_rcRegion.top;
	pProgress->m_rcRegion.right		+= pComp->m_rcRegion.left;
	pProgress->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	fscanf( pFile, "%s", pRead );
	if( strcmp( pRead, "HORIZONTAL" ) == 0 )
	{
		pProgress->SetType( UIProgress::HORIZONTAL );
	}
	else if( strcmp( pRead, "VERTICAL" ) == 0 )
	{
		pProgress->SetType( UIProgress::VERTICAL );
	}

	int		nMax;
	fscanf( pFile, "%d", &nMax );
	pProgress->SetMinMax( 0, nMax );

	int		nRow, nCol;
	fscanf( pFile, "%d", &nRow );
	fscanf( pFile, "%d", &nCol );

	fscanf( pFile, "%s", pRead );
	if( !strcmp( pRead, "NONE" ) )
	{
		BYTE	byRGB[3];
		fscanf( pFile, "%d", &byRGB[0] );
		fscanf( pFile, "%d", &byRGB[1] );
		fscanf( pFile, "%d", &byRGB[2] );
		if( !pProgress->CreateRectangle( COLOR_ARGB( 255, byRGB[0], byRGB[1], byRGB[2] ) ) )
		{
			SAFE_RELEASE( pProgress );
			Assert( !"Failed Create Rectangle" );
			return NULL;
		} //if
	}
	else
	{
		if( !pProgress->Load( pRead, nRow, nCol ) )
		{
			SAFE_RELEASE( pProgress );
			Assert( !"텍스쳐 실패" );
			return NULL;
		} //if
	} //if..else
	pProgress->SetPos( 100 );
	
	return pProgress;
} //CW3DUInterface::ReadUIProgress

/**
 *
 */
UIProgressEx* CW3DUInterface::ReadUIProgressEx( UICComponent* pComp, FILE* pFile )
{
	UIProgressEx*		pProgress = new UIProgressEx;
	if( !pProgress )
	{
		Assert( !"버튼실패" );
		return NULL;
	}

	char		pRead[64], pRead2[64];
	
	pProgress->m_pOwnerComp  = (UICComponent*)pComp;
	pProgress->m_nType		 = UI_PROGRESS;
	fscanf( pFile, "%s", pRead );
	AddComponent( pRead, (UICComponent*)pProgress );
	pProgress->m_strName = pRead;
	pProgress->m_nID = GenerateID( pRead );				// 아이뒤
	if( pProgress->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pProgress );
		Assert( !"아뒤 실패" );
		return NULL;
	}

	int		nWidth, nHeight, nMaxNum, nMax;
	fscanf( pFile, "%d", &pProgress->m_rcRegion.left );
	fscanf( pFile, "%d", &pProgress->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );

	fscanf( pFile, "%d", &nMaxNum );
	fscanf( pFile, "%d", &nMax );

	nWidth = ( m_nWidth / nMaxNum );

	pProgress->m_rcRegion.right		= pProgress->m_rcRegion.left + nWidth;
	pProgress->m_rcRegion.bottom	= pProgress->m_rcRegion.top + nHeight;

	pProgress->m_rcRegion.left	+= pComp->m_rcRegion.left;
	pProgress->m_rcRegion.top	+= pComp->m_rcRegion.top;
	pProgress->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pProgress->m_rcRegion.bottom	+= pComp->m_rcRegion.top;
	
	pProgress->SetMinMax( 0, nMax );

	fscanf( pFile, "%s", pRead );
	fscanf( pFile, "%s", pRead2 );

	if( m_nWidth == 800 )
	{
		sprintf( pRead, "%s.tga", pRead );
		sprintf( pRead2, "%s.tga", pRead2 );		
	}
	else
	{
		sprintf( pRead, "%s-%4d.tga", pRead, m_nWidth );
		sprintf( pRead2, "%s-%4d.tga", pRead2, m_nWidth );
	}

	if( !pProgress->Load( pRead, pRead2, nMaxNum ) )
	{
		SAFE_RELEASE( pProgress );
		Assert( !"텍스쳐 실패" );
		return NULL;
	}
	pProgress->SetPos( 100 );
	
	return pProgress;
} //CW3DUInterface::ReadUIProgressEx

/**
 *
 */
UIInventoryIN* CW3DUInterface::ReadUIInventoryIN( UICComponent* pComp, FILE* pFile )
{
	UIInventoryIN*	pInvenIN = new UIInventoryIN;
	if( !pInvenIN )
	{
		Assert( !"인벤토리온" );
		return NULL;
	}

	char		pRead[64];

	pInvenIN->m_pOwnerComp  = (UICComponent*)pComp;
	pInvenIN->m_nType		= UI_INVENTORYIN;
	fscanf( pFile, "%s", pRead );
	AddComponent( pRead, (UICComponent*)pInvenIN );
	pInvenIN->m_strName = pRead;
	pInvenIN->m_nID = GenerateID( pRead );				// 아이뒤
	if( pInvenIN->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pInvenIN );
		Assert( !"아뒤 실패" );
		return NULL;
	}

	int		nWidth, nHeight;
	fscanf( pFile, "%d", &pInvenIN->m_rcRegion.left );
	fscanf( pFile, "%d", &pInvenIN->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pInvenIN->m_rcRegion.right	= pInvenIN->m_rcRegion.left + nWidth;
	pInvenIN->m_rcRegion.bottom	= pInvenIN->m_rcRegion.top + nHeight;

	pInvenIN->m_rcRegion.left	+= pComp->m_rcRegion.left;
	pInvenIN->m_rcRegion.top	+= pComp->m_rcRegion.top;
	pInvenIN->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pInvenIN->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	fscanf( pFile, "%s", pRead );
	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, pRead ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	}
	pInvenIN->SetFont( pFont );

	int			nRow, nCol;
	int			nGap;
	fscanf( pFile, "%d", &nCol );
	fscanf( pFile, "%d", &nRow );
	fscanf( pFile, "%d", &nGap );
	pInvenIN->Init( nCol, nRow, nGap, m_pItemInfoMgr );
	
	if( pComp->m_strName == "DLG_SHOPINVENTORY" )
	{
		pInvenIN->SetOwnerMode( 1 );
	}
	else if( pComp->m_strName == "DLG_CREATEMYSHOP" ||
			 pComp->m_strName == "DLG_PERSONALSHOP" )
	{
		pInvenIN->SetOwnerMode( 2 );
	} //if..else

	return pInvenIN;
} //CW3DUInterface::ReadUIInventoryIN

/**
 *
 */
UIInventoryON* CW3DUInterface::ReadUIInventoryON( UICComponent* pComp, FILE* pFile )
{
	UIInventoryON*	pInvenON = new UIInventoryON;
	if( !pInvenON )
	{
		Assert( !"인벤토리온" );
		return NULL;
	}

	char		pRead[64];
	int			nTemp;

	pInvenON->m_pOwnerComp  = (UICComponent*)pComp;
	pInvenON->m_nType = UI_INVENTORYON;
	fscanf( pFile, "%s", pRead );
	AddComponent( pRead, (UICComponent*)pInvenON );
	pInvenON->m_strName = pRead;
	pInvenON->m_nID = GenerateID( pRead );				// 아이뒤
	if( pInvenON->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pInvenON );
		Assert( !"아뒤 실패" );
		return NULL;
	}

	int		nWidth, nHeight;
	fscanf( pFile, "%d", &pInvenON->m_rcRegion.left );
	fscanf( pFile, "%d", &pInvenON->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pInvenON->m_rcRegion.right	= pInvenON->m_rcRegion.left + nWidth;
	pInvenON->m_rcRegion.bottom	= pInvenON->m_rcRegion.top + nHeight;

	pInvenON->m_rcRegion.left	+= pComp->m_rcRegion.left;
	pInvenON->m_rcRegion.top	+= pComp->m_rcRegion.top;
	pInvenON->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pInvenON->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	fscanf( pFile, "%s", pRead );
	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, pRead ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	}
	pInvenON->SetFont( pFont );

	fscanf( pFile, "%d", &nTemp );
	pInvenON->SetPartCount( nTemp );

	int			nX, nY;
	int			nPartIndex;
	for( int i = 0 ; i < pInvenON->GetPartCount() ; i++ )
	{
		fscanf( pFile, "%d", &nPartIndex );
		fscanf( pFile, "%d", &nX );
		fscanf( pFile, "%d", &nY );

		nX += pInvenON->m_rcRegion.left;
		nY += pInvenON->m_rcRegion.top;
		pInvenON->AddPartRect( nPartIndex, nX, nY );
	} //for
	pInvenON->Init( m_pItemInfoMgr );

	return pInvenON;
} //CW3DUInterface::ReadUIInventoryON

/**
 * 퀵 슬롯 로드.
 */
UIQuickSlot* CW3DUInterface::ReadUIQuickSlot( UICComponent* pComp, FILE* pFile )
{
	UIQuickSlot*	pQuickSlot = new UIQuickSlot;
	CHECK_PTR_RETURN( pQuickSlot );

	pQuickSlot->m_pOwnerComp	= (UICComponent*)pComp;
	pQuickSlot->m_nType			= UI_QUICKSLOT;

	char	szMagicListName[64];

	fscanf( pFile, "%s", szMagicListName );

	AddComponent( szMagicListName, (UICComponent*)pQuickSlot );

	pQuickSlot->m_strName		= szMagicListName;
	pQuickSlot->m_nID			= GenerateID( szMagicListName );

	if( FAILED_ID == pQuickSlot->m_nID )
	{
		Assert( !"아뒤 실패" );
		SAFE_RELEASE( pQuickSlot );
		return NULL;
	} //if

	int			nWidth, nHeight;
	fscanf( pFile, "%d", &pQuickSlot->m_rcRegion.left );
	fscanf( pFile, "%d", &pQuickSlot->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );

	pQuickSlot->m_rcRegion.right	= pQuickSlot->m_rcRegion.left + nWidth;
	pQuickSlot->m_rcRegion.bottom	= pQuickSlot->m_rcRegion.top + nHeight;

	pQuickSlot->m_rcRegion.left		+= pComp->m_rcRegion.left;
	pQuickSlot->m_rcRegion.top		+= pComp->m_rcRegion.top;
	pQuickSlot->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pQuickSlot->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	int		nNum, nGap;
	fscanf( pFile, "%d", &nNum );
	fscanf( pFile, "%d", &nGap );
	pQuickSlot->SetUintPosInto( nNum, nGap );

	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, "GULLIM12" ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	} //if
	pQuickSlot->SetFont( pFont );

	return pQuickSlot;
} //CW3DUInterface::ReadUIQuickSlot

/**
 * 스킬 창 로드.
 */
UISkillEntry* CW3DUInterface::ReadUISkillEntry( UICComponent* pComp, FILE* pFile )
{
	UISkillEntry*	pSkillEntry = new UISkillEntry;
	CHECK_PTR_RETURN( pSkillEntry );

	pSkillEntry->m_pOwnerComp	= (UICComponent*)pComp;
	pSkillEntry->m_nType		= UI_SKILLENTRY;

	char	szSkillName[64];
	BYTE	btSkillType;

	fscanf( pFile, "%s", szSkillName );
	if( !strcmp( szSkillName, "SKILL_SHORT" ) )			btSkillType = UISkillEntry::SKILL_SHORT;
	else if( !strcmp( szSkillName, "SKILL_LONG" ) )		btSkillType = UISkillEntry::SKILL_LONG;
	else if( !strcmp( szSkillName, "SKILL_BLACK" ) )	btSkillType = UISkillEntry::SKILL_BLACK;
	else if( !strcmp( szSkillName, "SKILL_WHITE" ) )	btSkillType = UISkillEntry::SKILL_WHITE;

	else if( !strcmp( szSkillName, "SKILL_SPECIAL" ) )	btSkillType = UISkillEntry::SKILL_SPECIAL;
	else if( !strcmp( szSkillName, "SKILL_LIFE" ) )		btSkillType = UISkillEntry::SKILL_LIFE;
	else if( !strcmp( szSkillName, "SKILL_GUILD" ) )	btSkillType = UISkillEntry::SKILL_GUILD;
	else if( !strcmp( szSkillName, "SKILL_COUNTRY" ) )	btSkillType = UISkillEntry::SKILL_COUNTRY;

	pSkillEntry->SetType( btSkillType );

	AddComponent( szSkillName, (UICComponent*)pSkillEntry );

	pSkillEntry->m_strName		= szSkillName;
	pSkillEntry->m_nID			= GenerateID( szSkillName );

	if( FAILED_ID == pSkillEntry->m_nID )
	{
		Assert( !"아뒤 실패" );
		SAFE_RELEASE( pSkillEntry );
		return NULL;
	} //if

	int			nWidth, nHeight;
	fscanf( pFile, "%d", &pSkillEntry->m_rcRegion.left );
	fscanf( pFile, "%d", &pSkillEntry->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pSkillEntry->m_rcRegion.right	= pSkillEntry->m_rcRegion.left + nWidth;
	pSkillEntry->m_rcRegion.bottom	= pSkillEntry->m_rcRegion.top + nHeight;

	pSkillEntry->m_rcRegion.left	+= pComp->m_rcRegion.left;
	pSkillEntry->m_rcRegion.top		+= pComp->m_rcRegion.top;
	pSkillEntry->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pSkillEntry->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	char	szFontName[64];
	fscanf( pFile, "%s", szFontName );
	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, szFontName ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	} //if
	pSkillEntry->SetFont( pFont ); 

	int		nCol, nRow, nGap;
	fscanf( pFile, "%d", &nCol );
	fscanf( pFile, "%d", &nRow );
	fscanf( pFile, "%d", &nGap );
	pSkillEntry->SetUintPosInfo( nCol, nRow, nGap, nGap );
	
	pSkillEntry->Init( m_pItemInfoMgr );
	
	return pSkillEntry;
} //CW3DUInterface::ReadUISkillEntry

/**
 * 스킬 창 로드.
 */
UIImageList* CW3DUInterface::ReadUIImageList( UICComponent* pComp, FILE* pFile )
{
	UIImageList*	pImageList = new UIImageList;
	//남은 시간을 위한 폰트
	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, "GULLIM12" ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	} //if
	pImageList->SetFont( pFont );

	CHECK_PTR_RETURN( pImageList );

	pImageList->m_pOwnerComp	= (UICComponent*)pComp;
	pImageList->m_nType			= UI_IMAGELIST;

	char	szName[64];
	fscanf( pFile, "%s", szName );
	AddComponent( szName, (UICComponent*)pImageList );

	pImageList->m_strName		= szName;
	pImageList->m_nID			= GenerateID( szName );

	if( FAILED_ID == pImageList->m_nID )
	{
		Assert( !"아뒤 실패" );
		SAFE_RELEASE( pImageList );
		return NULL;
	} //if

	int			nWidth, nHeight;
	fscanf( pFile, "%d", &pImageList->m_rcRegion.left );
	fscanf( pFile, "%d", &pImageList->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pImageList->m_rcRegion.right	= pImageList->m_rcRegion.left + nWidth;
	pImageList->m_rcRegion.bottom	= pImageList->m_rcRegion.top + nHeight;

	pImageList->m_rcRegion.left		+= pComp->m_rcRegion.left;
	pImageList->m_rcRegion.top		+= pComp->m_rcRegion.top;
	pImageList->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pImageList->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	int		nCol, nRow, nGap;
	fscanf( pFile, "%d", &nCol );
	fscanf( pFile, "%d", &nRow );
	fscanf( pFile, "%d", &nGap );
	pImageList->SetUintPosInfo( nCol, nRow, nGap, nGap );

	return pImageList;
} //CW3DUInterface::ReadUIImageList

/**
 *
 */
UIPicture* CW3DUInterface::ReadUIPicture( UICComponent* pComp, FILE* pFile )
{
	UIPicture*	pPic = new UIPicture;
	CHECK_PTR_RETURN( pPic );

	pPic->m_pOwnerComp	= reinterpret_cast<UICComponent*>( pComp );
	pPic->m_nType		= UI_PICTURE;

	char	szPictureName[64];

	fscanf( pFile, "%s", szPictureName );

	AddComponent( szPictureName, reinterpret_cast<UICComponent*>( pPic ) );

	pPic->m_strName		= szPictureName;
	pPic->m_nID			= GenerateID( szPictureName );

	if( FAILED_ID == pPic->m_nID )
	{
		Assert( !"아뒤 실패" );
		SAFE_RELEASE( pPic );
		return NULL;
	} //if

	int			nWidth, nHeight;
	fscanf( pFile, "%d", &pPic->m_rcRegion.left );
	fscanf( pFile, "%d", &pPic->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	if( pPic->m_rcRegion.left == 1440 )
	{
		pPic->m_rcRegion.left = m_nWidth;
	}
	pPic->m_rcRegion.right	= pPic->m_rcRegion.left + nWidth;
	pPic->m_rcRegion.bottom	= pPic->m_rcRegion.top + nHeight;

	pPic->m_rcRegion.left	+= pComp->m_rcRegion.left;
	pPic->m_rcRegion.top	+= pComp->m_rcRegion.top;
	pPic->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pPic->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	char	szPicName[64];
	fscanf( pFile, "%s", szPicName );

	if( string( szPicName ) != "NULL" )
	{
		MAP_STRSPRITE::iterator		iterMap_Sprite = m_mapSprite.find( szPicName );
		if( iterMap_Sprite == m_mapSprite.end() )
		{
			Assert( !"등록되지 않은 이미지" );
			return NULL;
		}
		IW3DSprite*		pSprite = iterMap_Sprite->second;
		Assert( pSprite );
		pPic->SetPicture( pSprite );
	} //if

	return pPic;
} //CW3DUInterface::ReadUIPicture

/**
 *
 */
UIMiniMap* CW3DUInterface::ReadUIMiniMap( UICComponent* pComp, FILE* pFile )
{
	UIMiniMap*		pMiniMap = new UIMiniMap;
	CHECK_PTR_RETURN( pMiniMap );

	pMiniMap->m_pOwnerComp	= reinterpret_cast<UICComponent*>( pComp );
	pMiniMap->m_nType		= UI_MINIMAP;

	char	szMiniMapName[64];

	fscanf( pFile, "%s", szMiniMapName );

	AddComponent( szMiniMapName, reinterpret_cast<UICComponent*>( pMiniMap ) );

	pMiniMap->m_strName		= szMiniMapName;
	pMiniMap->m_nID			= GenerateID( szMiniMapName );

	if( pMiniMap->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pMiniMap );
		Assert( !"아뒤 실패" );
		return NULL;
	} //if

	int			nWidth, nHeight;
	fscanf( pFile, "%d", &pMiniMap->m_rcRegion.left );
	fscanf( pFile, "%d", &pMiniMap->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pMiniMap->m_rcRegion.right	= pMiniMap->m_rcRegion.left + nWidth;
	pMiniMap->m_rcRegion.bottom	= pMiniMap->m_rcRegion.top + nHeight;

	pMiniMap->m_rcRegion.left	+= pComp->m_rcRegion.left;
	pMiniMap->m_rcRegion.top	+= pComp->m_rcRegion.top;
	pMiniMap->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pMiniMap->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	UIDialog*		pDlg = static_cast<UIDialog*>( pComp );
	Assert( pDlg );

	char	szBuff[32];
	fscanf( pFile, "%s", szBuff );
	if( !strcmp( szBuff, "ALWAYS" ) )
	{
		pMiniMap->SetAlways( TRUE );
	}
	else
	{
		pMiniMap->SetAlways( FALSE );
	} //if..else

	if( !pMiniMap->Init() )
	{
		Assert( !"이미지 로드 실패" );
		return NULL;
	} //if

	return pMiniMap;
} //CW3DUInterface::ReadUIMiniMap

/** 
 * 타이머.
 */
UITimer* CW3DUInterface::ReadUITimer( UICComponent* pComp, FILE* pFile )
{
	UITimer*		pTimer = new UITimer;
	CHECK_PTR_RETURN( pTimer);

	pTimer->m_pOwnerComp	= reinterpret_cast<UICComponent*>( pComp );
	pTimer->m_nType			= UI_TIMER;

	char	szName[64];

	fscanf( pFile, "%s", szName );

	AddComponent( szName, reinterpret_cast<UICComponent*>( pTimer ) );

	pTimer->m_strName		= szName;
	pTimer->m_nID			= GenerateID( szName );

	if( pTimer->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pTimer );
		Assert( !"아뒤 실패" );
		return NULL;
	} //if

	int			nWidth, nHeight;
	fscanf( pFile, "%d", &pTimer->m_rcRegion.left );
	fscanf( pFile, "%d", &pTimer->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pTimer->m_rcRegion.right	= pTimer->m_rcRegion.left + nWidth;
	pTimer->m_rcRegion.bottom	= pTimer->m_rcRegion.top + nHeight;

	pTimer->m_rcRegion.left		+= pComp->m_rcRegion.left;
	pTimer->m_rcRegion.top		+= pComp->m_rcRegion.top;
	pTimer->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pTimer->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	fscanf( pFile, "%s", szName );
	if( strcmp( szName, "HORIZONTAL" ) == 0 )
	{
		pTimer->SetType( UITimer::HORIZONTAL );
	}
	else if( strcmp( szName, "VERTICAL" ) == 0 )
	{
		pTimer->SetType( UITimer::VERTICAL );
	}

	fscanf( pFile, "%s", szName );
	if( !pTimer->Load( szName ) )
	{
		Assert( !"이미지 로드 실패" );
		return NULL;
	} //if

	return pTimer;
} //CW3DUInterface::ReadUITimer

/**
 * 사각형.
 */
UIRectangle* CW3DUInterface::ReadUIRectangle( UICComponent* pComp, FILE* pFile )
{
	UIRectangle*		pRectangle = new UIRectangle;
	CHECK_PTR_RETURN( pRectangle );

	pRectangle->m_pOwnerComp	= reinterpret_cast<UICComponent*>( pComp );
	pRectangle->m_nType			= UI_RECTANGLE;

	char	szMiniMapName[64];

	fscanf( pFile, "%s", szMiniMapName );

	AddComponent( szMiniMapName, reinterpret_cast<UICComponent*>( pRectangle ) );

	pRectangle->m_strName		= szMiniMapName;
	pRectangle->m_nID			= GenerateID( szMiniMapName );

	if( pRectangle->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pRectangle );
		Assert( !"아뒤 실패" );
		return NULL;
	} //if

	int			nWidth, nHeight;
	fscanf( pFile, "%d", &pRectangle->m_rcRegion.left );
	fscanf( pFile, "%d", &pRectangle->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pRectangle->m_rcRegion.right	= pRectangle->m_rcRegion.left + nWidth;
	pRectangle->m_rcRegion.bottom	= pRectangle->m_rcRegion.top + nHeight;

	pRectangle->m_rcRegion.left		+= pComp->m_rcRegion.left;
	pRectangle->m_rcRegion.top		+= pComp->m_rcRegion.top;
	pRectangle->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pRectangle->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	BYTE	btColor[4];
	fscanf( pFile, "%d", &btColor[0] );
	fscanf( pFile, "%d", &btColor[1] );
	fscanf( pFile, "%d", &btColor[2] );
	fscanf( pFile, "%d", &btColor[3] );

	DWORD	dwColor = btColor[0] << 24 | btColor[1] << 16 | btColor[2] << 8 | btColor[3];
	pRectangle->SetColor( dwColor );

	int			nDelay;
	fscanf( pFile, "%d", &nDelay );
	pRectangle->SetDelay( nDelay );

	return pRectangle;
} //CW3DUInterface::ReadUIRectangle

/**
 * 시트 로드.
 */
UISheet* CW3DUInterface::ReadUISheet( UICComponent* pComp, FILE* pFile )
{
	UISheet*		pSheet = new UISheet;
	CHECK_PTR_RETURN( pSheet );

	pSheet->m_pOwnerComp	= pComp;
	pSheet->m_nType			= UI_SHEET;

	char	szName[64];
	fscanf( pFile, "%s", szName );

	AddComponent( szName, reinterpret_cast<UICComponent*>( pSheet ) );
	pSheet->m_strName		= szName;
	pSheet->m_nID			= GenerateID( szName );

	if( pSheet->m_nID == FAILED_ID )
	{
		SAFE_RELEASE( pSheet );
		Assert( !"아뒤 실패" );
		return NULL;
	} //if

	int			nWidth, nHeight;
	fscanf( pFile, "%d", &pSheet->m_rcRegion.left );
	fscanf( pFile, "%d", &pSheet->m_rcRegion.top );
	fscanf( pFile, "%d", &nWidth );
	fscanf( pFile, "%d", &nHeight );
	pSheet->m_rcRegion.right	= pSheet->m_rcRegion.left + nWidth;
	pSheet->m_rcRegion.bottom	= pSheet->m_rcRegion.top + nHeight;

	pSheet->m_rcRegion.left		+= pComp->m_rcRegion.left;
	pSheet->m_rcRegion.top		+= pComp->m_rcRegion.top;
	pSheet->m_rcRegion.right	+= pComp->m_rcRegion.left;
	pSheet->m_rcRegion.bottom	+= pComp->m_rcRegion.top;

	char	szFontName[64];
	fscanf( pFile, "%s", szFontName );
	IW3DFont*		pFont;
	if( !g_pScene->FindFont( &pFont, szFontName ) )
	{
		Assert( !"폰트가 없당" );
		return FALSE;
	} //if
	pSheet->SetFont( pFont ); 

	int		nInterval, nNumLine;
	fscanf( pFile, "%d%d", &nInterval, &nNumLine );
	pSheet->SetLineNum( nInterval, nNumLine );

	char	szColTitle[64];
	int		nColWidth;
	int		nNumCol;
	fscanf( pFile, "%d", &nNumCol );
	for( int nCnt = 0; nCnt < nNumCol; nCnt++ )
	{
		fscanf( pFile, "%s", szColTitle );
		fscanf( pFile, "%d", &nColWidth );
		pSheet->AddColumn( szColTitle, nColWidth );
	} //for

	return pSheet;
} //CW3DUInterface::ReadUISheet

/**
 * 미니 맵을 읽어온다.
 */
BOOL CW3DUInterface::LoadMiniMap(
	WORD wMap	///< [IN] 미니맵
	)
{
	m_pMiniMap  = reinterpret_cast<UIMiniMap*>( GetComponent( "MAP_MINI" ) );
	m_pLargeMap = reinterpret_cast<UIMiniMap*>( GetComponent( "MAP_LARGE" ) );
	Assert( m_pMiniMap );
	Assert( m_pLargeMap );

	int		nWidth;
	int		nHeight;

	if( wMap == 20 )
	{
		nWidth  = 200;
		nHeight = 200;
	}
	else
	{
		nWidth  = 500;
		nHeight = 500;
	} //if..else

	if( !m_pMiniMap->LoadMap( wMap, nWidth, nHeight ) )
	{
		return FALSE;
	} //if
	
	m_pLargeMap->Cleanup();
	m_pLargeMap->SetMapSize( nWidth, nHeight );

	char		szScrFileName[64];
	sprintf( szScrFileName, "NPC/npcinfo%04d.scr", wMap );
	if( !m_pNPCScript->Load( szScrFileName ) )
	{
		Assert( !"NPC 스크립트 로드 실패" );
		return FALSE;
	} //if

	return TRUE;
} //CW3DUInterface::LoadMiniMap

/**
 * 상태 정보를 읽어온다.
 *
 * @param	szFileName		[IN] 상태 정보 파일.
 *
 * @return	상태 정보 읽기 성공 여부.
 */
BOOL CW3DUInterface::LoadState( const char *szFileName )
{
	Assert( szFileName );

	FILE*		pFile = fopen( szFileName, "rt" );
	if( !pFile )
	{
		Assert( !"파일을 열 수 없습니다" );
		return FALSE;
	} //if

	char			szRead[64];
	int				nLen = 0;
	LIST_STRING*	listShowDlg;
	string			strDlgName;

	while( fscanf( pFile, "%s", szRead ) != EOF )
	{
		if( szRead[0] == '[' )
		{
			nLen = strlen( szRead );
			if( szRead[nLen - 1] != ']' )
			{
				Assert( !"모드정의는 요렇게 '['name']'" );
				return FALSE;
			} //if

			listShowDlg = new LIST_STRING;
			if( !listShowDlg )
			{
				Assert( !"메모리 할당 실패" );
				return FALSE;
			} //if

			m_mapModeState.insert( std::make_pair( (string)szRead, (void*)listShowDlg ) );
		}
		else if(
			szRead[0] == 'D' &&
			szRead[1] == 'L' &&
			szRead[2] == 'G' )
		{
			strDlgName = szRead;
			listShowDlg->push_back( strDlgName );
		} //if..else if
	} //while

	fclose( pFile );

	return TRUE;
} //CW3DUInterface::LoadState
