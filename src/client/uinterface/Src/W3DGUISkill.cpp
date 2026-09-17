/**
 * 유저 인터페이스 아이템 관련.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-09-10 11:28p $
 * @version	$Revision: 33 $
 *
 * <HR>
 * $Log: /UInterface/Src/W3DGUISkill.cpp $
 * 
 * 33    04-09-10 11:28p Paran
 * 
 * 32    04-08-26 3:31p Paran
 * 
 * 31    04-08-03 10:49p Paran
 * 
 * 30    04-08-03 5:18p Paran
 * 
 * 29    04-07-16 3:08p Paran
 * 
 * 8     04-01-02 2:21p Paran			메세지 define 처리
 * 
 * 55    03-12-10 2:04p Paran			인첸츠 카드 추가
 * 
 * 52    03-12-09 5:58p Paran			아이템 데이터 수정
 * 
 * 50    03-11-18 5:11p Sk8snow2		CheckPlayerSkill 수정
 * 
 * 44    03-09-15 1:42a Paran			Quick Slot 포션 갯수 수정.
 * 
 * 11    03-06-23 1:33a Paran			실드 아이템 처리.
 * 
 * 8     03-06-05 3:19p Paran			양손검
 * 
 * 7     03-05-28 4:14p Paran			미니 맵 파티 관련 수정
 * 
 * 5     03-05-25 4:10p Paran			물약 다량 구입 인터페이스 추가
 * 
 * 3     03-05-23 3:43p Paran			버그수정 중...
 * 
 * <HR>
 *
 * @file	W3DGUIItem.cpp
 */

#include "UInterface_Export.h"
#include "W3DUInterface.h"

/**
 * 스킬 관련 초기화
 */
BOOL CW3DUInterface::InitSkill()
{
	AddSkill( SI_PARTY_INVIT );
	AddSkill( SI_PARTY_OUT );
	AddSkill( SI_PARTY_BANISH );
	AddSkill( SI_PARTY_BREAK );
	AddSkill( SI_PARTY_BOARD );

	AddSkill( SI_DEAL_REQUEST );
	AddSkill( SI_ITEM_PICKUP );
	AddSkill( SI_QUEST );

	AddSkill( SI_FRIEND_REGIST );
	AddSkill( SI_FRIEND_LIST );
	AddSkill( SI_FRIEND_DELETE );
	AddSkill( SI_FRIEND_CUTOFF );

	AddSkill( SI_CHATTING_CUTOFF );
	AddSkill( SI_WHISPER_CUTOFF );
	AddSkill( SI_DEAL_CUTOFF );
	AddSkill( SI_PARTY_CUTOFF );

	AddSkill( SI_INFO_POSITION );
	AddSkill( SI_WARINFO );

//	AddSkill( SI_ACT_BOW );
//	AddSkill( SI_ACT_SALUTE );
//	AddSkill( SI_ACT_HANDCLAP );
//	AddSkill( SI_ACT_DEATH );
//	AddSkill( SI_ACT_DANCE );


	///////////////////  단 스킬 아이템  //////////////////

	AddSkill( SI_DAN_TAKE_MEMBER );
	AddSkill( SI_DAN_REMOVE_MEMBER  );
	AddSkill( SI_DAN_OUT );
	AddSkill( SI_DAN_ENCAMP );
	AddSkill( SI_DAN_NOTICE	);
	AddSkill( SI_DAN_ONLINEMEMBER_LIST );
	AddSkill( SI_DAN_ALLMEMBER_LIST );
	AddSkill( SI_DAN_CAPTAIN_LEAVE );
	AddSkill( SI_DAN_PROMOTE );
	AddSkill( SI_DAN_DEMOTE );
	AddSkill( SI_DAN_SUBCAPTAIN_LEAVE );
	AddSkill( SI_DAN_MEMBER_CLEANUP );

	///////////////////  군단 스킬 아이템  //////////////////

	AddSkill( SI_GUNDAN_ALLIANCE );
	AddSkill( SI_GUNDAN_STRIKE );
	AddSkill( SI_GUNDAN_INVIT );
	AddSkill( SI_GUNDAN_HIREDCUT );
	AddSkill( SI_TEXGET );

	///////////////////  개인상점 스킬 아이템  //////////////////

	AddSkill( SI_PERSONAL_OPEN );

	///////////////////  PK관련 스킬 아이템  //////////////////

	AddSkill( SI_PLAYERKILL_NONE );
	AddSkill( SI_PLAYERKILL_FREE );
	AddSkill( SI_PLAYERKILL_GUILD );
	AddSkill( SI_PLAYERKILL_PARTY );

	
	return TRUE;
} //CW3DUInterface::InitSkill

/**
 * 스킬을 사용한다.
 */
void CW3DUInterface::UseSkill( BYTE btIndex )
{
	UIQuickSlot*	pQuickSlot = GetQuickBar( m_btQuickBarNum );
	SItemData*		pItemData = pQuickSlot->GetItem( btIndex );
//	m_nSaveSkill = 0;
	
	if( !pItemData )
	{ //등록되지 않은 스킬
		return;
	} //if

/*	if( pItemData->nId == NORMAL_ATTACK1 )
	{
		m_nSaveSkill = btIndex;
		SetCheck( TRUE );
	}

	else if( pItemData->nId == NORMAL_ATTACK2 )
	{
		m_nSaveSkill = btIndex;
		SetCheck( TRUE );
	}*/
	SkillProc( pItemData );
} //CW3DUInterface::UseSkill

/**
 * 스킬 처리
 */
void CW3DUInterface::SkillProc( SItemData* pItemDat )
{
	Assert( pItemDat );

	switch( pItemDat->nType )
	{
	case SI_PARTY_INVIT:			PushEvent( "PARTY", EVENT_PARTY_INVIT, 0, 0 );				break;
	case SI_PARTY_OUT:				PushEvent( "PARTY", EVENT_PARTY_OUT, 0, 0 );				break;
	case SI_PARTY_BANISH:			PushEvent( "PARTY", EVENT_PARTY_BANISH, 0, 0 );				break;
	case SI_PARTY_BREAK:			PushEvent( "PARTY", EVENT_PARTY_DESTROY, 0, 0 );			break;
	case SI_PARTY_BOARD:			ShowDlg( "DLG_PLAYERLIST",TRUE );							break; //PushEvent( "PARTYBOARD", EVENT_PARTY_BOARD, 0, 1 );			break;

	case SI_DEAL_REQUEST:			PushEvent( "DEAL", EVENT_DEAL_REQUEST, 0, 0 );				break;
	case SI_ITEM_PICKUP:			PushEvent( "ITEM", EVENT_ITEM_AUTOPICKUP, 0, 0 );			break;
//	case SI_QUEST:					PushEvent( "QUEST", EVENT_QUEST, 0, 0 );					break;

	case SI_FRIEND_REGIST:			PushEvent( "FRIEND", EVENT_FRIEND_REGIST, 0, 0 );			break;
	case SI_FRIEND_LIST:			PushEvent( "FRIEND", EVENT_FRIEND_LIST, 0, 0 );				break;
	case SI_FRIEND_DELETE:			SkillDeleteFriend();										break;
		
	case SI_INFO_POSITION:			PushEvent( "INFO", EVENT_INFO_POSITION, 0, 0 );				break;
	case SI_WARINFO:				PushEvent( "GUNDAN", EVENT_GUILD_GETCASTLELIST, 0, 0 );		break;
	case SI_TEXGET:					PushEvent( "GUNDAN", EVENT_TEX_GET, 0, 0 );		break;

	case SI_CHATTING_CUTOFF:
	case SI_WHISPER_CUTOFF:
	case SI_DEAL_CUTOFF:
	case SI_PARTY_CUTOFF:
	case SI_FRIEND_CUTOFF:			PushEvent( "UI_KEY", EVENT_USEQUICKITEM, pItemDat->nType, 0 );		break;

	case SI_DAN_TAKE_MEMBER:		PushEvent( "DEAL", EVENT_GUILD_TAKEMEMBER, 0, 0 );				break;
	case SI_DAN_REMOVE_MEMBER:		PushEvent( "DEAL", EVENT_GUILD_REMOVEMEMBER, 0, 0 );			break;
	case SI_DAN_OUT:				PushEvent( "DEAL", EVENT_GUILD_OUT, 0, 0 );						break;
	case SI_DAN_ENCAMP:				PushEvent( "DEAL", EVENT_GUILD_ENCAMP, 0, 0 );					break;
	case SI_DAN_NOTICE:				PushEvent( "DEAL", EVENT_GUILD_NOTICE, 0, 0 );					break;
	case SI_DAN_ONLINEMEMBER_LIST:	PushEvent( "DEAL", EVENT_GUILD_ONLINEMEMBERLIST, 0, 0 );		break;
	case SI_DAN_ALLMEMBER_LIST:		PushEvent( "DEAL", EVENT_GUILD_ALLMEMBERLIST, 0, 0 );			break;
	case SI_DAN_CAPTAIN_LEAVE:		PushEvent( "DEAL", EVENT_GUILD_CAPTAINLEAVE, 0, 0 );			break;
	case SI_DAN_PROMOTE:			PushEvent( "DEAL", EVENT_GUILD_PROMOTE, 0, 0 );					break;
	case SI_DAN_DEMOTE:				PushEvent( "DEAL", EVENT_GUILD_DEMOTE, 0, 0 );					break;
	case SI_DAN_SUBCAPTAIN_LEAVE:	PushEvent( "DEAL", EVENT_GUILD_SUBCAPTAINLEAVE, 0, 0 );			break;
	case SI_DAN_MEMBER_CLEANUP:		PushEvent( "DEAL", EVENT_GUILD_CLEANUP, 0, 0 );					break;

	case SI_PLAYERKILL_NONE:		PushEvent( "PLAYERKILL", EVENT_PKMODE, pItemDat->nType, 0 );	break;
	case SI_PLAYERKILL_FREE:		PushEvent( "PLAYERKILL", EVENT_PKMODE, pItemDat->nType, 1 );	break;
	case SI_PLAYERKILL_GUILD:		PushEvent( "PLAYERKILL", EVENT_PKMODE, pItemDat->nType, 2 );	break;
	case SI_PLAYERKILL_PARTY:		PushEvent( "PLAYERKILL", EVENT_PKMODE, pItemDat->nType, 3 );	break;

	case SI_PERSONAL_OPEN:
		if( IsShowDlg( "DLG_CREATEMYSHOP" ) || 
			IsShowDlg( "DLG_PERSONALSHOP" ) )	ChattingInput( "", STR_UIMSG_ALREADYPSHOP, FONT_MSGCOLOR, 0, 0 );
		else									PushEvent( "MYSHOP", EVENT_MYSHOP_OPEN, 0, 0 );
		break;

	default:
		SItemInfoData*	pItemInfo = pItemDat->pItemInfoData;
		Assert( pItemInfo );

		if( ISMAGICSCROLL( pItemInfo->btItemType ) )
		{ //마법스크롤
			ApplySkill( pItemDat );
		}
		else
		{ //아이템
			ApplyItem( pItemDat );
		} //if..else
		break;
	} //switch
} //CW3DUInterface::SkillProc

/**
 * Apply Skill
 */
void CW3DUInterface::ApplySkill( SItemData* pItemDat )
{
	SSkillInfoData*		pSkillDat;
	if( !pItemDat->pItemInfoData )
	{
		Assert( !"없는 아템" );
		return;
	} //if

	if( pItemDat->pItemInfoData->pSkillData )
	{
		pSkillDat = pItemDat->pItemInfoData->pSkillData;
		if( IS_SKILL_SPECIAL( pSkillDat ) || 
			IS_SKILL_LIFE( pSkillDat ) ||
			IS_SKILL_GUILD( pSkillDat ) ||
			IS_SKILL_COUNTRY( pSkillDat ) )
		{
			PushEvent( "SKILL", EVENT_SKILL_SPECIAL, pItemDat->nType, 0 );
		}
		else if( IS_SATTACK( pSkillDat ) && pItemDat->nType != 10101 )
		{
			PushEvent( "UI_KEY", EVENT_USEQUICKITEM, pItemDat->nType, 0 );
			PushEvent( "SKILL", EVENT_SKILL_SPECIAL, pItemDat->nType, 0 );
		}
		else if( IS_LATTACK( pSkillDat ) && pItemDat->nType != 10102 )
		{
			PushEvent( "UI_KEY", EVENT_USEQUICKITEM, pItemDat->nType, 0 );
			PushEvent( "SKILL", EVENT_SKILL_SPECIAL, pItemDat->nType, 0 );
		}
		else if( IS_BMAGIC( pSkillDat ) && pItemDat->nType != 10102 )
		{
			PushEvent( "UI_KEY", EVENT_USEQUICKITEM, pItemDat->nType, 0 );
			PushEvent( "SKILL", EVENT_SKILL_SPECIAL, pItemDat->nType, 0 );
		}
		else if( IS_WMAGIC( pSkillDat ) && pItemDat->nType != 10102 )
		{
			PushEvent( "UI_KEY", EVENT_USEQUICKITEM, pItemDat->nType, 0 );
			PushEvent( "SKILL", EVENT_SKILL_SPECIAL, pItemDat->nType, 0 );
		}
		else
		{
			PushEvent( "UI_KEY", EVENT_USEQUICKITEM, pItemDat->nType, 0 );
		} //if..else
	}
	else
	{
		PushEvent( "UI_KEY", EVENT_USEQUICKITEM, pItemDat->nType, 0 );
	} //if..else
} //CW3DUInterface::ApplySkill

/**
 * Apply Item
 */
void CW3DUInterface::ApplyItem( SItemData* pItemDat )
{
	Assert( pItemDat );

	SItemInfoData*	pItemInfo = pItemDat->pItemInfoData;
	Assert( pItemInfo );

	SItemData* pFindItemDat;
	
	pFindItemDat = GetItemDataInEquip( pItemInfo->wItemNum );
	if( pFindItemDat )
	{
		SoundItem( pFindItemDat );
		PushEvent( "UI_INVENTORYON", EVENT_DBCLICK_ITEM, pFindItemDat->nId, 1 );		// '1' 장착창
		return;
	}
	
	pFindItemDat = GetItemDataInInventory( pItemInfo->wItemNum );
	if( pFindItemDat )
	{
		RequestUseItem( pFindItemDat );													// '0' 인벤토리	
		SoundItem( pFindItemDat );
		return;
	} //if..else
} //CW3DUInterface::ApplyItem

/**
 * 적용스킬 아이콘 추가
 */
void CW3DUInterface::AddSkillImage( WORD wSkillType, DWORD dwTime )
{
	SItemInfoData*		pItemInfo = m_pItemInfoMgr->GetItemInfo( wSkillType );
	if( !pItemInfo )
	{
		Assert( !"없는 아템" );
		return;
	} //if

	if( !pItemInfo->pSkillData )
	{
		Assert( !"스킬 정보 얻기 실패" );
		return;
	} //if


	BOOL		btIs = FALSE;
	DWORD			dwId = pItemInfo->pSkillData->nItemNum;;	///< 아뒤저장 (데이터 관리 편의상..)
	SItemData*		pItemData = m_ItemMgr.FindItem( dwId );
	if( !pItemData )
	{ 
		m_ItemMgr.InsertItem( dwId, wSkillType, pItemInfo );
		pItemData = m_ItemMgr.FindItem( dwId );
		btIs = TRUE;
	} //if
	if( btIs == TRUE )
		SetSkillInfo( TRUE );
	else
		SetSkillInfo( FALSE );

	Assert( pItemData );

	UIImageList*	pImageList = reinterpret_cast<UIImageList*>( GetComponent( "IMGLST_SKILLIMAGE" ) );
	if( !pImageList )
	{
		Assert( !"없는 이미지 리스트 콤프" );
		return;
	} //if
	pImageList->InsertImage( pItemData, dwTime );
} //CW3DUInterface::AddSkillImage

/**
 * 적용스킬 아이콘 삭제
 */
void CW3DUInterface::RemoveSkillImage( WORD wSkillId )
{
	UIImageList*	pImageList = reinterpret_cast<UIImageList*>( GetComponent( "IMGLST_SKILLIMAGE" ) );
	if( !pImageList )
	{
		Assert( !"없는 이미지 리스트 콤프" );
		return;
	} //if
	pImageList->RemoveImage( wSkillId );
} //CW3DUInterface::RemoveSkillImage

/**
 * 친구삭세 스킬
 */
void CW3DUInterface::SkillDeleteFriend()
{
	UISelectString* pSelStr = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_CHATID_SAVE" ) );
	if( !pSelStr )
	{
		Assert( "Not Found SSTR_CHATID_SAVE Comp" );
		return;
	} //if

	char*	pText = pSelStr->GetText();
	if( pText )
	{
		strcpy( m_szRecentWhisperName, pText );
		PushEvent( "FRIEND", EVENT_FRIEND_DELETE, 0, 0 );
	} //if
} //CW3DUInterface::SkillDeleteFriend

/**
 * 메인스킬 설정.
 */
void CW3DUInterface::SetMainSkill( const int nSkillItem )
{
	Assert( m_pItemInfoMgr );

	SSkillInfoData*		pSkillData = m_pItemInfoMgr->GetSkillInfo( nSkillItem );
	if( !pSkillData )
	{
		Assert( !"아이템 데이터 실패" );
		return;
	} //if

	UIProgress*		pSkillComp;	
	if( IS_SATTACK( pSkillData ) )			
	{
		m_nMainSkill = 0;
		pSkillComp = reinterpret_cast<UIProgress*>( GetComponent( "PRO_CINFO_SK1" ) );
	}
	else if( IS_LATTACK( pSkillData ) )
	{
		m_nMainSkill = 1;
		pSkillComp = reinterpret_cast<UIProgress*>( GetComponent( "PRO_CINFO_SK2" ) );
	}
	else if( IS_BMAGIC( pSkillData ) )		
	{
		m_nMainSkill = 2;
		pSkillComp = reinterpret_cast<UIProgress*>( GetComponent( "PRO_CINFO_SK3" ) );
	}
	else if( IS_WMAGIC( pSkillData ) )		
	{
		m_nMainSkill = 3;
		pSkillComp = reinterpret_cast<UIProgress*>( GetComponent( "PRO_CINFO_SK4" ) );
	}
	else
	{
		Assert( !"스킬이 아니다" );
		return;
	} //if

	if( !pSkillComp )
	{
		Assert( !"없다" );
		return;
	} //if

	UIProgressEx*	pMainGraph = reinterpret_cast<UIProgressEx*>( GetComponent( "PRO_SKILLGRAPH" ) );
	Assert( pMainGraph );
	pMainGraph->SetMinMax( 0, pSkillComp->GetMax() );
	pMainGraph->SetPos( pSkillComp->GetPos(), FALSE );

	SItemInfoData*		pItemData = m_pItemInfoMgr->GetItemInfo( nSkillItem );
	if( !pSkillData )
	{
		Assert( !"아이템 데이터 실패" );
		return;
	} //if

	if( !pItemData->pSkillData )
	{
		Assert( !"스킬 데이터 얻기실패" );
		return;
	} //if
	DWORD		dwSkillId = pItemData->pSkillData->nItemNum;
	SItemData*	pItem = m_ItemMgr.FindItem( dwSkillId );
	if( !pItem )
	{
		m_ItemMgr.InsertItem( dwSkillId, pItemData->btItemType, pItemData );
		pItem = m_ItemMgr.FindItem( dwSkillId );
		Assert( pItem );
	} //if

	UIPicture*		pPicMSkill = reinterpret_cast<UIPicture*>( GetComponent( "PIC_CHAR_MAINSKILL" ) );
	Assert( pPicMSkill );
	pPicMSkill->SetPicture( pItem->pimgItem );

	SetDamage( pItemData );
} //CW3DUInterface::SetMainSkill

/**
 * 스킬추가.
 */
void CW3DUInterface::AddSkill( const WORD wType )
{
	SItemInfoData*		pItemInfo = m_pItemInfoMgr->GetItemInfo( wType );
	if( !pItemInfo )
	{
		Assert( !"없는 아템" );
		return;
	} //if

	if( !pItemInfo->pSkillData )
	{
		Assert( !"스킬 정보 얻기 실패" );
		return;
	} //if

	DWORD			dwId = pItemInfo->pSkillData->nItemNum;;	///< 아뒤저장 (데이터 관리 편의상..)
	SItemData*		pItemData = m_ItemMgr.FindItem( dwId );
	if( !pItemData )
	{ 
		m_ItemMgr.InsertItem( dwId, wType, pItemInfo );
		pItemData = m_ItemMgr.FindItem( dwId );
	} //if
	Assert( pItemData );

	SSkillInfoData*		pSkillInfo = m_pItemInfoMgr->GetSkillInfo( wType );
	if( !pSkillInfo )
	{
		Assert( !"없는 스킬정보" );
		return;
	} //if
	
	UISkillEntry*		pSkillEntry = NULL;
	
	if( IS_SKILL_SPECIAL( pSkillInfo ) )
	{ //특화스킬
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_SPECIAL" ) );
	}
	else if( IS_SKILL_LIFE( pSkillInfo ) )
	{ //생활스킬
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_LIFE" ) );
	}
	else if( IS_SKILL_GUILD( pSkillInfo ) )
	{ //단 스킬
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_GUILD" ) );
	}
	else if( IS_SKILL_COUNTRY( pSkillInfo ) )
	{ //군단스킬
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_COUNTRY" ) );
	}
	else if( IS_SATTACK( pSkillInfo ) )
	{ //근거리 스킬
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_SHORT" ) );
	} //if
	else if( IS_LATTACK( pSkillInfo ) )
	{ //원거리 스킬
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_LONG" ) );
	} //if
	else if( IS_BMAGIC( pSkillInfo ) )
	{
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_BLACK" ) );
	}
	else if( IS_WMAGIC( pSkillInfo ) )
	{
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_WHITE" ) );
	} 
	else
	{
		Assert( !"없는 스킬타입" );
		return;
	} //if..else..

	pSkillEntry->InsertSkill( pItemData );
} //CW3DUInterface::AddSkill

/**
 * Set Skill Timer 
 */
void CW3DUInterface::SetSkillTimer( int nItemId, int nTime )
{
	for( int nCnt = 0; nCnt < NUM_QUICKSLOTBAR; nCnt++ )
	{
		m_apQuickBar[nCnt]->SetTimer( nItemId, nTime );
	} //for	
} //CW3DUInterface::SetSkillTimer