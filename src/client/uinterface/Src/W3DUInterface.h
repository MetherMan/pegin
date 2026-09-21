/**
 * 유저 인터페이스 클래스 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 05-02-22 6:56p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /UInterface/W3DUInterface.h $
 * 
 * 3     05-02-22 6:56p Sk8snow2
 * 
 * 2     05-02-19 7:12p Sk8snow2
 * 
 * 1     05-02-18 6:55p Umenokoji
 *
 * 124   05-02-18 6:49p Umenokoji  Line 478  캐릭터 정보창에 마법저항력 표시
 * 
 * 123   04-12-08 3:07a Sk8snow2
 * 
 * 122   04-11-16 7:11p Sk8snow2
 * 
 * 121   04-09-24 4:46p Paran
 * 
 * 120   04-09-10 11:28p Paran
 * 
 * 83    04-01-02 2:21p Paran		메세지 define 처리
 * 
 * 78    03-12-22 5:00p Sckyj		2D, 3D sound 분리
 * 
 * 67    03-12-10 2:04p Paran		인첸츠 카드 추가
 * 
 * 61    03-11-19 8:08p Sckyj		중국어 IME관련 작업 진행중
 * 
 * 48    03-09-15 1:42a Paran		Quick Slot 포션 갯수 수정.
 * 
 * 17    03-06-24 3:37p Paran		소리 추가.
 * 
 * 16    03-06-23 2:25a Paran		SetPlayerPower 추가.
 * 
 * 11    03-06-04 11:24a Paran		미니맵 상점 추가
 * 
 * 10    03-05-28 4:14p Paran		미니 맵 파티 관련 수정
 * 
 * 8     03-05-25 4:10p Paran		물약 다량 구입 인터페이스 추가
 * 
 * 6     03-05-23 3:43p Paran		버그수정 중...
 * 
 * 5     03-05-20 9:35p Paran		계속 수정 중...
 * 
 * 4     03-05-17 4:31p Paran		파티원 보이기 부분 수정.
 * 
 * 2     03-05-17 4:20a Paran		각 콤포넌트 분리.
 * 
 * 98    03-05-16 8:09p Paran		NPC 스크립트 수정.
 * 
 * 86    03-05-06 6:18p Paran		상점 기능 추가.
 *
 * 80    03-04-15 1:27a Paran		NPC 추가.
 *
 * 79    03-04-11 5:28p Paran		캐릭터 장착 부분과 미니맵 수정.
 *
 * 78    03-04-10 6:24p Paran		미니맵 추가.
 *
 * 76    03-04-02 8:11p Paran		플레이어 아이템 추가, 메세지 박스 키 입력 추가.
 *
 * 72    03-03-26 9:19p Paran		인벤토리 어색한 부분 수정하고 체크.
 *
 * 71    03-03-26 6:22a Paran		아이템 서버 연결.
 *
 * 69    03-03-25 3:36a Paran		인벤토리 아이템 서버와 연결.
 *
 * 57    03-03-06 11:16p Paran		인벤토리 마우스 기능 변경과 캐릭터 선택 부분 정보 삭제 처리.
 * <HR>
 *
 * @file	W3DUInterface.h
 */


#ifndef __W3DUInterface_Header__
#define __W3DUInterface_Header__


#include "UICharacterFace.h"
#include "UIGameReady.h"
#include "UIItemMgr.h"
#include "UICharSelect.h"
#include "UIDialog.h"
#include "InchentCard.h"

//#ifdef _CHINAVERSION
//	#include "ChattingIME_C.h"
//#else
	#include "ChattingIME.h"
//#endif

#include <map>
#include <list>
#include <vector>
#include <string>

using namespace std;

#define	FONT_MSGCOLOR				0xFFFF9C00

#ifdef _CHINAVERSION
#define CHAR_WHISPER			'/'
#else
#define CHAR_WHISPER			'"'
#endif

#define CHAR_SAVEWORD			'!'

#define		STR_COUNTRY1			"모로스"
#define		STR_COUNTRY2			"네메시스"
#define		STR_COUNTRY3			"타나토스"

#define		TICK_DOUBLECLICK		300
#define		MAX_CHATTSTRING			128
#define		MAX_SAVEWHISPERNAME		10

#define		INVENTORY_PAGE			2
#define		INVENTORY_ITEMNUM		20

const int	MAX_EVENTNUM	= 200;					///<
const int	BEGIN_ID		= 201;					///< ( 0 보다 큰 값 )
const int	FAILED_ID		= BEGIN_ID - 1;			///<

const int	CHATLIST_REPEAT	= 2;					///<
const int	MAX_PARTYNUM	= 7;					///< 파티 최대 갯수.

typedef map<string, IW3DSprite*>		MAP_STRSPRITE;		///<
typedef map<int, string>				MAP_INTSTR;			///<
typedef map<string, UICComponent*>		MAP_STRCOMP;		///< 컴포넌트 검색을 위한 맵
typedef list<UIDialog*>					LIST_PUIDLG;		///<
typedef list<string>					LIST_STRING;		///<
typedef map<string, void*>				MAP_LISTSTRING;		///<

															/**
															* 현제 작업중인 크리쳐
*/
struct SCreatureTarget
{
	BYTE		btType;					///< 크리쳐 타입
	DWORD		dwId;					///< 크리쳐 아이디
	
	void		Clean()		{ btType = NONE; dwId = 0; }
	BOOL		IsEmpty()	{ if( btType == NONE )	return TRUE;	return FALSE; }
	void		SetTarget( BYTE btType_, DWORD dwId_ )	{ btType = btType_; dwId = dwId_; }
	BOOL		IsTarget( BYTE btType_, DWORD dwId_ )
	{
		if( btType == btType_ && dwId == dwId_ )
		{
			return TRUE;
		} //if
		return FALSE;
	} //IsTarget
}; //SCreatureTarget


///////////////////  생활 스킬 아이템  //////////////////

#define SI_PARTY_INVIT					10137
#define SI_PARTY_OUT					10138
#define SI_PARTY_BANISH					10139
#define SI_PARTY_BREAK					10140
#define SI_PARTY_BOARD					10141

#define SI_DEAL_REQUEST					10142
#define SI_ITEM_PICKUP					10143
#define SI_QUEST						10144
#define SI_WARINFO						10456
#define SI_TEXGET						10457

#define SI_FRIEND_REGIST				10145
#define SI_FRIEND_LIST					10146
#define SI_FRIEND_DELETE				10147
#define SI_FRIEND_CUTOFF				10148

#define SI_CHATTING_CUTOFF				10149
#define SI_WHISPER_CUTOFF				10150
#define SI_DEAL_CUTOFF					10151
#define SI_PARTY_CUTOFF					10152

#define SI_INFO_POSITION				10153

#define SI_ACT_BOW						10154
#define SI_ACT_SALUTE					10155
#define SI_ACT_HANDCLAP					10156
#define SI_ACT_DEATH					10157
#define SI_ACT_DANCE					10158

#define SI_PERSONAL_OPEN				10451

///////////////////  단 스킬 아이템  //////////////////

#define SI_DAN_TAKE_MEMBER				10159
#define SI_DAN_REMOVE_MEMBER			10160
#define SI_DAN_OUT						10161
#define SI_DAN_ENCAMP					10162
#define SI_DAN_NOTICE					10163
#define SI_DAN_ONLINEMEMBER_LIST		10164
#define SI_DAN_ALLMEMBER_LIST			10165
#define SI_DAN_CAPTAIN_LEAVE			10166
#define SI_DAN_PROMOTE					10167
#define SI_DAN_DEMOTE					10168
#define SI_DAN_SUBCAPTAIN_LEAVE			10169
#define SI_DAN_MEMBER_CLEANUP			10170

///////////////////  군단 스킬 아이템  //////////////////

#define SI_GUNDAN_ALLIANCE				10171
#define SI_GUNDAN_STRIKE				10172
#define SI_GUNDAN_INVIT					10173
#define SI_GUNDAN_HIREDCUT				10174

///////////////////  PK관련 스킬 아이템  //////////////////

#define SI_PLAYERKILL_NONE				10452
#define SI_PLAYERKILL_FREE				10453
#define SI_PLAYERKILL_GUILD				10454
#define SI_PLAYERKILL_PARTY				10455

/**
 * 유저 인터페이스 클래스.
 * @ingroup		Engine
 */
class CW3DUInterface : public IW3DUInterface
{
	SINGLETON_COMPONENT( CW3DUInterface );

public:

	///////////////////  이전 아이템 위치  //////////////////

	enum { FROM_NULL = 0, FROM_INVENTORY, FROM_EQUIP, FROM_SHOP, 
		   FROM_STORAGE, FROM_USERDEAL, FROM_SKILLENTRY, FROM_MYSHOP, FROM_PSHOP, FROM_CASH, FROM_WP, FROM_AM, FROM_CARD, FROM_QUICK = 100 };

	UNDER_MEMORY_MGR( CW3DUInterface );

	CW3DUInterface();
	~CW3DUInterface();

	BOOL				Init( int nWidth, int nHeight, BOOL bWinMouse );
	void				InitDlg( IW3DCamera* pCamera );
	void				InitInventory();
	BOOL				SetWinDevice( HWND	hWnd, IW3DInput* pInput );

	void				SaveUserData( BOOL bNewPlayer );

	BOOL				SetModeState( const char* szMode  );
	BOOL				SetCursorMode( const char* szCursor = "CURSOR_NORMAL" );
	
	void				Cleanup();
	void				CleanupComponent( const char* szID );

	BOOL				ClearText( const char* szID );
	const char*			GetText( const char* szID );
	BOOL				SetText( const char* szID, const char* szText, DWORD dwColor = 0 );
	BOOL				SetText( const char* szID, int nDecimal, DWORD dwColor = 0 );
	BOOL				SetPicture( const char* szID, const char* szPicName );
	BOOL				SetPicture( const char* szID, IW3DSprite* pSprite );
	BOOL				GetPushButtonState( const char* szID );
	BOOL				SetPushButtonState( const char* szID, int nState );		// '0' Disable, '1' Enable, '2' Push, '3' On

	BOOL				PopEvent( char* szUIName, UIEVENT* pnEvent, DWORD* pdwParam1, DWORD* pdwParam2 );
	
	BOOL				IsDoubleByteCharset( const char* pBuff );
	
	BOOL				ShowComponent( const char* szCompName, BOOL bShow );
	BOOL				ShowDlg( const char* szDlgName, BOOL bShow, BOOL bEvent = TRUE );
	BOOL				ShowDlg( UICComponent* pComp, BOOL bShow, BOOL bEvent = TRUE );
	BOOL				ShowCompGroup( const char* szCompKey, const int nIndex );
	BOOL				Show( const char* szDlgName, BOOL bShow );
	BOOL				IsShowDlg( const char* szDlgName );
	BOOL				IsShowLockDlg();
	
	BOOL				EnableDlg( UICComponent* pComp, BOOL bEnable );

	BOOL				MessageBox( const char* szText, char* szKey = NULL, int nType = 0 );
	BOOL				IsShowMessageBox();
	
	BOOL				Update( DWORD dwTick );
	void				Draw();
	BOOL				HotKey();
	void				CharKeyProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	BOOL				QuickKeyProc();
	BOOL				IsIMEInput();
	BOOL				IMEProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	BOOL				IsExcepChar( const char* szString );

	///////////////////  일괄처리  ///////////////////////

	void				SetCharacterName( const char* szName );
	BOOL				GetClassName( char* szClassName, const BYTE btClass );
	void				SetJobName( const BYTE btClass );
	void				SetPosHP( int nMax, int nPos );
	void				SetPosMP( int nMax, int nPos );
	void				SetPosSkill( int nSkill, int nMax, int nPos, BOOL bProgress );
	void				SetPlayerPower( const int nType, const int nLevel, BOOL btIs );
	void				ShowTargetInfo( int nId, int nMax, int nPos, const char* szText );

	void				InitCharSelectInfo();
	void				RefreshNewCharacter();
	void				SetCharInfo( SCharInfo* pCharInfo );
	void				SetCharInfoEmptyButton( BOOL bEmpty );
	BOOL				VerifyCharacterSet( const char* szChar );
	BOOL				GetCharSelInfo(
		char*	szName,
		BYTE*	pbSex,
		WORD*	pwHair, 
		WORD*	pwFace,
		WORD*	pwCoat,
		WORD*	pwPants,
		WORD*	pwHand,
		WORD*	pwFoot,
		int*	pnIndex,
		BYTE*	pSTR,
		BYTE*	pINT,
		BYTE*	pDEX,
		BYTE*	pCountry
		);
	
	BOOL				GetCharSelModelName(
		const int nIndex,
		char*	szName, 
		BYTE*	pSex, 
		char*	szHair, 
		char*	szFace, 
		char*	szCoat, 
		char*	szPants, 
		char*	szHand, 
		char*	szFoot
		);
	void				SetCharSelInfo(
		const char*	szName,
		const int	nLevel,
		const int	nLLevel,
		const int	nSLevel,
		const int	nBLevel,
		const int	nWLevel,
		const BYTE	btSex,
		const WORD	wHair, 
		const WORD	wFace,
		const WORD	wCoat,
		const WORD	wPants,
		const WORD	wHand,
		const WORD	wFoot,
		const BYTE	btClass,
		const int	nIndex
		);
	int					CreateCharacter();
	int					GetCharSelectNum();
	void				DeleteCharInfo( const int nIndex  );
	int					GetOnItemType( int nPart );

	BOOL				UpdateItemData( const int nId, const WORD btCount );
	int					GetItemNumInInventory( const int nType );
	SItemData*			GetItemDataInInventory( const int nType );
	SItemData*			GetItemDataInEquip( const int nType );
	int					InventoryBlank();
	BOOL				InsertInventory( int nId, int nType, BYTE btPage, BYTE btCol, BYTE btRow, WORD btCnt, BOOL bSave );
	BOOL				InsertSerIntInventory( int nId, int nType, BYTE btPage, BYTE btCol, BYTE btRow, WORD btCnt, BOOL bSave );
	void				ItemInMyInventory( SItemData* pItemDat, UIInventoryIN* pInventory );
	BOOL				ItemToHand( int nId );
	BOOL				ItemFromHand();
	WORD				InsertEquipItem( int nId, int nType );
	void				InsertSmithItem( int nId, int nType, int price );
	BOOL				RemoveItemInInventory( int nId );
	BOOL				RemoveSerInventory( int nId );
	WORD				RemoveEquipItem( int nId, int nPos = 0 );
	void				RemoveSmithItem( int nId );
	void				TakeItemInSerInven( SItemData* pItemDat, int ServerNum, int CharPos );

	char*				GetUnitNum( const DWORD dwNum );
	void				UpdateMoney( int nMoney );	
	void				UpdateCash( int nCash );
	void				InsertItemInShop( int nItemType, BYTE btPage, BYTE btCol, BYTE btRow );
	
	void				PlayerLive( const BOOL bLive );

	BOOL				LoadGameReady( WORD wMap );
	void				DrawGameReady( DWORD dwTick );

	void				ShowTimer( const char* szCompName, DWORD dwShowTick );
	void				GameHelp();
	
	///////////////////  상점  ///////////////////////

	void				MoveItemInInventory( SItemData* pItemDat, UIInventoryIN* pInventory );
	void				RequestSellItem( SItemData* pItemDat );
	void				BuyItemInShop( SItemData* pItemDat );
	void				BuyCardInShop( SItemData* pItemDat, int itemNum );
	void				SetShopNum( const WORD wShop );

	SItemData*			m_pBuyItemData;
	int					m_nSellItem;
	WORD				m_wShop;				///< 선택 샵 번호
	
	///////////////////  창고  ///////////////////////

	BOOL				InsertItemInStorage( const int nId, const int nType, const WORD	btCnt );
	BOOL				RemoveItemInStorage( const int nId );
	BOOL				RemoveSerItemInStorage( const int nId );
	void				RemoveAllItemInStorage();
	void				RemoveAllSerItemInStorage();
	void				RequestSaveItemInStorage( SItemData* pItemDat );
	void				TakeItemInStorage( SItemData* pItemDat );
	void				TakeItemInSerStorage( SItemData* pItemDat, int ServerNum );
	BOOL				InsertItemInSerGarbage( const int nId, const int nType, const WORD	btCnt );

	///////////////////  퀘스트  ///////////////////////

	void				SetQuestNum( const WORD wQuest );
	void				Quest( WORD wNPCId, WORD wQuest, BYTE btProc );
	void				AddQuestInList( WORD wQuestNum, BYTE btState );

	WORD				m_wQuest;				///< 퀘스트 번호

	///////////////////  인첸트 ///////////////////////
	void				RequestSaveitemInSmith( SItemData* pItemDat );	
	
	
	///////////////////  아이템 관리  ///////////////////////

	SItemData*			GetItemDataInSkill( const int nItemNum );
	BOOL				EventCastItem( int nType );
	void				InputPrice( const char* szRequestComp = NULL );
	void				BuyItem( const char* szRequestComp = NULL );

	///////////////////  팁정보  ///////////////////////

	IW3DFont*			m_pfntTip;
	IW3DRectangle*		m_pRectangle;
	char				m_szTipInfo[64];
	DWORD				m_dwTipColor;

	void				SetTipText( const char* szText, DWORD dwColor = 0xffc2c2c2 );
	void				DrawTip( int nX, int nY );
	
	///////////////////  파티  ///////////////////////

	struct sPartyMember
	{
		int		nSlotNum;
		int		nId;
		char	szName[128];
		int		nHp;
		int		nMaxHp;
		int		nMp;
		int		nMaxMp;
	};

	void				AddPartyMember( 
		int nId,				///< [IN] 케릭터 아이디
		const char* szName,		///< [IN] 캐릭터 이름
		int nHp,				///< [IN] HP
		int nMaxHp,				///< [IN] MaxHP
		int nMp,				///< [IN] MP
		int nMaxMp				///< [IN] MaxMP
		);
	
	void				DeletePartyMember( int nId );
	void				DeletePartyAll();
	void				PartyInvitByPlayerList();
	void				HealParty( const int nPartyNum );
	BOOL				IsPartyMember( const DWORD dwId );
	void				RefreshPartyMember( int nId, int nHp, int nMaxHp, int nMp, int nMaxMp );
	void				RefreshPartyMember( int nId, WORD wMap, int nX, int nY );

	void				RefreshPartyInfo();

	void				AddPartyBoardList(
		const char*	szName,			///< [IN] 캐릭터 이름.
		const int	nLevel,			///< [IN] 레벨.
		const int	nSSkill,		///< [IN] 근거리 스킬.
		const int	nLSkill,		///< [IN] 원거리 스킬.
		const int	nBSkill,		///< [IN] 흑마법 스킬.
		const int	nWSkill			///< [IN] 백마법 스킬.
		);
	
	UISheet*			m_pPartyBoard;
	typedef list<sPartyMember>		LIST_PARTYMEMBER;
	LIST_PARTYMEMBER	m_listPartyMember;

	///////////////////  길드  ///////////////////////

private:
	int					m_nInviteGuildMasterId;
	WORD				m_wInviteGuildNumber;
	BYTE				m_btInputSystem;
//	char				*m_PrevText;

public:
	void				ClearGuildMember();
	void				AddGuildMember( const char* szName, const BYTE btClass, const int nTotLev );
	void				RemoveGuildMember( const char* szName );
	void				RequestInviteGuild( WORD wGuildNo, int nMasterId, const char* szGuildName );

	///////////////////  포션 상점구입  ///////////////////////
	
	int					m_nNumPosion;
	BOOL				m_bNumPosion;
	BOOL				m_bCheckSild;
	BOOL				m_bCheckWeight;
	void				RefreshBuyPosion();
	void				SetNowWeight( int nowWeight, int MaxWeight );

	///////////////////  디스플레이 부분  ///////////////////////

	void				SetDamage( SItemInfoData* pItemData );
	void				SetDefence();
	void				SetMagicResis(  );		//캐릭터 정보창에 마법저항력 표시
	
private:
	BOOL				m_bWinMouse;			///< 윈도우 마우스여부
	MAP_STRSPRITE		m_mapCursor;			///<
	IW3DSprite*			m_pCursor;				///<
	char				m_szPrevCursorName[64];
	typedef map<string, HCURSOR>		MAP_HCURSOR;
	MAP_HCURSOR			m_mapWinCursor;
	HWND				m_hWnd;

private:
	BYTE				m_btYou;				///< '0' 캐릭터, '1' 몬스터, '2' NPC
	int					m_nYouId;				///< 상대 아이디~

public:
	IW3DSoundObject2D*	m_pSound;				///< 사운드.
	IW3DInput*			m_pInput;				///<
	int					m_nLastID;				///<
	MAP_LISTSTRING		m_mapModeState;			///<
	MAP_STRSPRITE		m_mapSprite;			///<
	MAP_INTSTR			m_mapControlID;			///<
	MAP_STRCOMP			m_mapComponent;			///<
	QUEUE_UIEVENT		m_queueEvent;			///<
	LIST_PUIDLG			m_listDialog;			///<
	UICComponent*		m_pCurrCommopent;		///<
	UICComponent*		m_pOldFocusCom;			///<
	UIEditText*			m_pCurrEditor;			///<
	UIDialog*			m_pTopDlg;				///<
	char				m_szMegKey[64];			///<
	BOOL				m_bIME;					///< IME 입력중...
	string				m_sModeState;			///< 현재 모드
	int					m_nMainSkill;			///< 현재 스킬.
	int					m_btCharacterNum;		///< 현재 케릭터 번호 ( 1, 2, 3 )

	int					m_nMouseX;				///<
	int					m_nMouseY;				///<
	int					m_nOldMouseX;			///<
	int					m_nOldMouseY;			///<

	DWORD				m_dwAccTick;			///<
	DWORD				m_dwTick;				///<
	DWORD				m_dwDoubleClick_L;		///< 
	BOOL				m_bLButtonPressed;		///<
	BOOL				m_bRButtonPressed;		///<
	BOOL				m_bReturnKeyPressed;	///<
	BOOL				m_bTabKeyPressed;		///<

	CUICharacterFace	m_CharFace;				///<
	IW3DItemInfoMgr*	m_pItemInfoMgr;			///<
	CCharSelectInfo		m_CharSelInfo;			///<
	CUIItemMgr			m_ItemMgr;				///<
	CUIItemMgr			m_ItemMgrCash;			///<
	CUIGameReady		m_GameReady;			///<
	UIMiniMap* 			m_pMiniMap;				///<
	UIMiniMap*			m_pLargeMap;			///<
	IW3DNPCScript*		m_pNPCScript;			///< NPC 스크립트 처리 객체
	IW3DNPCScript*		m_pQuestScript;			///< Quest 스크립트 처리 객체
	IW3DNPCScript*		m_pSmithScript;			///< 대장장이 스크립트 처리 객체
	BOOL				m_bNPCTalkMode;			///< NPC와 대화 구분( '0' - 일반대화, '1' - 퀘스트, '2' - 대장장이 )
	int					m_NPCNum;			

	IW3DSprite*			m_pHelpImage;			///< 핼프 이미지.
	int					m_nGold;				///< 현재 머니
	int					m_nCash;				///< 현재 캐쉬 머니
	IW3DSprite*			m_pEquipBody;			///< 장착 이미지
	SItemData*			m_pConfirmItemData;		///< 드롭 아이템
	BYTE				m_btFrom;				///< 이전 아이템 위치

	BOOL				m_SelectIntegration;	///< ture = 웨폰, false = 아머
	char				m_szUIccompentSmith[64];
	char				m_szTxtName[64];
	int					m_EnchantPricemoney;
	SItemData*			m_EnchantSmithItem;
	CInchentCard		m_EnchantSmithCard;

	int					m_ServerNum;
	int					m_CharPos;
	int					m_cnt;
	BOOL				m_CheckState;


	//////////////////  파일로드  /////////////////

	BOOL				Load( const char* szFileName, const char* szStateName, int nWidth, int nHeight );
	BOOL				LoadState( const char* szFileName );

	BOOL				ReadFont( FILE* pFile );
	BOOL				ReadCursor( FILE* pFile );
	BOOL				ReadSprite( FILE* pFile );

	BOOL				ReadUIDialog( FILE* pFile );
	UIButton*			ReadUIButton( UICComponent* pComp, FILE* pFile );
	UICheckButton*		ReadUICheckButton( UICComponent* pComp, FILE* pFile );
	UIEditText*			ReadUIEditText( UICComponent* pComp, FILE* pFile );
	UIStaticText*		ReadUIStaticText( UICComponent* pComp, FILE* pFile );
	UITabButton*		ReadUITab( UICComponent* pComp, FILE* pFile );
	UIScrollBar*		ReadUIScrollBar( UICComponent* pComp, FILE* pFile );
	UIStringList*		ReadUIStringList( UICComponent* pComp, FILE* pFile );
	UISelectString*		ReadUISelectString( UICComponent* pComp, FILE* pFile );
	UIProgress*			ReadUIProgress( UICComponent* pComp, FILE* pFile );
	UIProgressEx*		ReadUIProgressEx( UICComponent* pComp, FILE* pFile );
	UIInventoryIN*		ReadUIInventoryIN( UICComponent* pComp, FILE* pFile );
	UIInventoryON*		ReadUIInventoryON( UICComponent* pComp, FILE* pFile );
	UIQuickSlot*		ReadUIQuickSlot( UICComponent* pComp, FILE* pFile );
	UISkillEntry*		ReadUISkillEntry( UICComponent* pComp, FILE* pFile );
	UIImageList*		ReadUIImageList( UICComponent* pComp, FILE* pFile );
	UIPicture*			ReadUIPicture( UICComponent* pComp, FILE* pFile );
	UIMiniMap*			ReadUIMiniMap( UICComponent *pComp, FILE *pFile );
	UITimer*			ReadUITimer( UICComponent* pComp, FILE* pFile );
	UIRectangle*		ReadUIRectangle( UICComponent* pComp, FILE* pFile );
	UISheet*			ReadUISheet( UICComponent* pComp, FILE* pFile );

	//////////////////  컨트롤 관련  /////////////////

	BOOL				IsFocus();
	const char*			FindNameFromID( int nID );

	void				ResetTopDlg();
	UIDialog*			GetFocusDlg();
	UICComponent*		GetFocusUIComponent();

	BOOL				AddComponent( const char* szKey, UICComponent* pComponent );
	UICComponent*		GetComponent( const char* szKey );
	int					GenerateID( const char* szIDName );		// 모든 아이뒤 에러는 BEGIN_ID - 1 값이다

	//////////////////  입 력  /////////////////

	BOOL				MouseButtonL_Up();
	BOOL				MouseButtonL_Down();
	BOOL				MouseDBClickL();
	BOOL				MouseButtonR_Up();
	BOOL				MouseButtonR_Down();
	BOOL				MouseMove();
	
	BOOL				PushButton_LDown( UICComponent* pComp );
	BOOL				PushButton_LUp( UICComponent* pComp );
	BOOL				CheckButton_LUp( UICComponent* pComp );
	BOOL				EditBox_LDown( UICComponent* pComp );
	BOOL				PictureBox_LDown( UICComponent* pComp );
	BOOL				Tab_LUp( UICComponent* pComp );
	BOOL				ScrollThumb_LDown( UICComponent* pComp );
	BOOL				ScrollThumb_Up( UICComponent* pComp );
	BOOL				ScrollThumb_Move( UICComponent* pComp );
	BOOL				SelectString_LDown( UICComponent* pComp );
	BOOL				SelectString_LUp( UICComponent* pComp );
	BOOL				SelectStringDBClick_L( UICComponent* pComp );
	BOOL				InventoryIn_LDown( UICComponent* pComp );
	BOOL				InventoryIn_LUp( UICComponent* pComp );
	BOOL				InventoryInDBClick_L( UICComponent* pComp );
	BOOL				InventoryIn_RDown( UICComponent* pComp );
	BOOL				InventoryIn_RUp( UICComponent* pComp );
	void				DialogBox_LDown( UICComponent* pComp );
	void				DialogBox_LUp( UICComponent* pComp );
	void				Rectangle_LDown( UICComponent* pComp );

	BOOL				InventoryOn_LDown( UICComponent* pComp );
	BOOL				InventoryOn_LUp( UICComponent* pComp );
	BOOL				InventoryOnDBClick_L( UICComponent* pComp );
	BOOL				ScrollBar_LDown( UICComponent* pComp );
	BOOL				QuickSlot_LDown( UICComponent* pComp );
	BOOL				QuickSlot_LUp( UICComponent* pComp );
	BOOL				QuickSlot_HoverMouse( UICComponent* pComp );
	BOOL				SkillEntry_LDown( UICComponent* pComp );
	BOOL				SkillEntry_LUp( UICComponent* pComp );
	BOOL				SkillEntryDBClick_L( UICComponent* pComp );
	void				DialogBox_Move( UIDialog* pDlg );
	void				CheckButton_Move( UICComponent* pComp );
	void				DlgYesExe();
	void				DlgNoExe();

	void				EditTextUpdate();

	BOOL				ReturnKey();
	BOOL				TabKey();

	BOOL				ReturnKey_Button();
	BOOL				ReturnKey_EditText();
	BOOL				TabKey_EditText();

	BOOL				SetProgressPos( const char* szID, int nMax, int nPos, BOOL bProgress, DWORD dwColor = 0 );
	
	//////////////////  Dlg Exit Event  //////////////

	void				Dlg_Exit( UIDialog* pDlg );
	
	//////////////////  PUSH BUTTON  /////////////////

	BOOL				PushButton_LUp_CHARSELECT( UICComponent* pComp );
	BOOL				PushButton_LUp_GAME_MODE( UICComponent* pComp );

	//////////////////  CHECK BUTTON  /////////////////

	BOOL				CheckButtonGroup( const char* szCheckKey, const int nIndex );
	int					GetCheckButtonIndex( const char* szCheckKey );
	
	BOOL				ChatCheckButton( const int nIndex );
	BOOL				CheckGroup( const char* szTabKey, const char* szInvenKey, const int nIndex );
	BOOL				Skill1CheckButton( const int nIndex );
	BOOL				Skill2CheckButton( const int nIndex );
	UISkillEntry*		GetShowSkill1Window();
	UISkillEntry*		GetShowSkill2Window();

	//////////////////  NPC  /////////////////

	const char*			GetNPCName( const int nAddr );
	void				SelectNPC( const int nAddr, const int nTalkNum );
	void				SelectSmithNPC( const int nAddr, const int nTalkNum );
	void				SelectCardSumNPC( const int nAddr, const int nTalkNum );
	void				SelectCardUndoNPC( const int nAddr, const int nTalkNum );
	void				SelectServer( const int nAddr );
	BOOL				BeginNPC( IW3DNPCScript* pNPCScript, const int nTalkNum );
	BOOL				NextNPC( IW3DNPCScript* pNPCScript, int nAnswer );

	//////////////////  인벤토리, 매직, 장착 창 관련  /////////////////

	void				SaveInventoryData( FILE* pFile );
	BOOL				LoadInventoryData( WORD* pItemTypeBuf, int nBufCount );
		
	BOOL				ShopInvenTabSelect( int nIndex );
	UIInventoryIN*		GetInventoryIN( const char* szKey, const int nIndex );

	void				SetMainSkill( const int nSkillItem );
	void				RemoveItemInEquip( SItemData* pItemDat );
	void				RemoveItemInSmith( SItemData* pItemDat ); 	
	void				RequestUseItem( SItemData* pItemDat );

	//////////////////  창관련  /////////////////

	void				HideActiveDlg();
	void				ShowSkillDlg( BOOL bShow );
	void				ShowMyInventoryDlg( BOOL bShow );
	void				ShowServerInventoryDlg( BOOL bShow );
	void				ShowUserDealBox( BOOL bShow );
	void				ShowShop( BOOL bShow );
	void				ShowStorage( BOOL bShow );
	void				ShowServerStorage( BOOL bShow );
	void				ShowSmithEnchentBox( BOOL bShow );


#ifdef _dUMENOKOJI_QUEST_
	void				ShowQuest( BOOL bShow );
	char				m_PrevText[30][1024];
	BOOL				BeginQuestNPC( IW3DNPCScript* pNPCScript, const int nTalkNum );
	BOOL				NextQuestNPC( IW3DNPCScript* pNPCScript, int nAnswer );
	BOOL				PreQuestNPC( IW3DNPCScript* pNPCScript );
	void				SetPrevText( char *Buf, int arrNum );
	char				*GetPrevText( int arrNum );
	
	
	int					m_StartNum, m_EndNum, m_NowNum;
	void				SetTextArrStartNum( int StartNum );
	int					GetTextArrStartNum(  );
	void				SetTextArrEndNum( int EndNum );
	int					GetTextArrEndNum(  );
	void				InitTextArroNowNum(  );
	void				SetTextArrNowNumPlus( int plusNum );
	void				SetTextArrNowNumMinus( int minusNum );
	int					GetTextArrNowNum(  );
#endif

#ifdef _dUMENOKOJI_RELOAD_
	void				ReLoadUI(  );
#endif 


	//////////////////  퀵슬롯  /////////////////
	BOOL				LoadQuickData();
	BOOL				SaveQuickData( FILE* pFile, const int nSlotIndex );

	void				ShowQuickBar();
	void				RefreshQuickSlot( UIQuickSlot* pQuickSlot );
	
	UIQuickSlot*		GetQuickBar( const int nIndex );

	void				UpdateQuickBar( const DWORD dwTick );

	BYTE				m_btQuickBarNum;
	long				m_lQFileOffset;

	UIQuickSlot*		m_apQuickBar[NUM_QUICKSLOTBAR];

	//////////////////  스킬 관련  /////////////////
	
	BOOL				InitSkill();
public:
	void				UseSkill( BYTE btIndex );
private:
	void				ApplySkill( SItemData* pItemDat );
	void				ApplyItem( SItemData* pItemDat );
	void				SkillProc( SItemData* pItemDat );

	void				AddSkill( const WORD wType );
	BOOL				CheckPlayerSkill( SItemData* pItemDat );

	void				SkillDeleteFriend();

	void				SetSkillTimer( int nItemId, int nTime );

	//////////////////  케릭터 얼굴 설정  /////////////////

	void				SetCharacterFace();

	//////////////////  미니 맵  /////////////////

	BOOL				LoadMiniMap( WORD wMap );
	void				SetPlayerMapPos( const int nX, const int nY );
	void				SetPlayerMapDir( const float fDirX, const float fDirY );
	void				AddShopMapPos( const int nX, const int nY );
	
	//////////////////  유저 거래  /////////////////

	void				RequestInsertItemInUserDeal( SItemData* pItemDat );
	void				RequestInsertItemInMyShop( SItemData* pItemDat );
	void				RequestDeleteItemInUserDeal( SItemData* pItemDat );
	void				InsertItemInUserDeal( 
		int nItemId, 
		int nItemType, 
		WORD wDestCol, 
		WORD wDestRow, 
		BOOL bMe 
		);

	void				RemoveItemInUserDeal( 
		int	nItemId, 
		BOOL bMe 
		);

	void				UpdateMoneyInUserDeal( 
		int	nMoney, 
		BOOL bMe 
		);

	//////////////////  상대와...  /////////////////

	void				YouAndMe( 
		int				nId,				///< [IN] 상대 아뒤.
		BYTE			btYou,				///< [IN] 상대 타입.
		const DWORD		dwChoiceFlag		///< [IN] 선택 프레그.
		);

	//////////////////  사운드  /////////////////

	void				SoundItem( SItemData* pItemDat );
	void				SetSoundState( BOOL bOnBGM, int nVolBGM, BOOL bOnEff, int nVolEff );

	//////////////////  쳇 관련  /////////////////

	void				ChattingInput( 
		const string& strId, 
		const string& strContent, 
		DWORD dwColor, 
		BYTE  btFontIndex,
		BYTE  btGroup
		);
	void				ChangeChatListSize();
	BOOL				CharacterCommand();
	void				WhisperNameScroll( const char* szText );
	BOOL				SaveChattingText();
	void				SaveWhisperName( const char* szName );
	char*				GetWhisperName();
	void				SetWhisperName( const char* szName );
	char*				GetChattingText();
	void				InputChattKeySaveString( const int nIndex );
	void				AddPlayerList( const DWORD dwId, const char* szName, const BYTE btClass, const int nTotLev );
	void				RemovePlayerList( const DWORD dwId, const char* szName );

	typedef list<char*>		LIST_WHISPERNAME;
	typedef map<DWORD, string>		MAP_PLAYERLIST;
	LIST_WHISPERNAME	m_listWhisperName;			///< 귓속말 아이디 리스트.
	int					m_nWNameIndex;				///< 귓속말 아이디 리스트 인덱스.
	char				m_szRecentWhisperName[MAX_CHATTSTRING];	///< 최근 귀속말상대 이름
	char				m_szRecentChattText[MAX_CHATTSTRING];
	char				m_aszWordsSave[10][MAX_CHATTSTRING];
	BYTE				m_btChatOption;
	MAP_PLAYERLIST		m_mapPlayerList;

	//////////////////  버프 관련  ///////////////////

	void				AddSkillImage( WORD wSkillId, DWORD dwTime );
	void				RemoveSkillImage( WORD wSkillId );

	//////////////////  인첸트 관련  /////////////////

	BOOL				SelectedEnchantItem( SItemData* pItemDat, BOOL bShowMsg );
	void				UseEnchantItem( SItemData* pItemDat );
	void				UseSmithEnchant();

	void				EnchantResult( BOOL bResult );
	CInchentCard		m_EnchantCard;

	//////////////////  이벤트 메세지 박스 보이기  /////////////////

	void				ShowEventMessageBox( const BYTE btResult );


	//////////////////  친구 관련  /////////////////
public:
	BOOL				IsFriend( const char* szName );
	void				AddFriend( const char* szFriendName );
	void				RemoveFriend( const char* szFriendName );
	void				ClearFriendList();
	void				FriendLogInOut( const char* szFriendName, BOOL bLogin );
	void				ChattingFriend( const char* szFriendName, const char* szMemo );
	void				RequestTalkFriend();
	
	//////////////////  팻 관련  /////////////////

	void				UpdatePet( 
		const char* szName,
		BYTE byType,
		int nLevel, 
		int nExp, 
		int nNeedExp, 
		int wAngerGage, 
		WORD wHp, 
		WORD wMaxHp, 
		WORD wMp, 
		WORD wMaxMp,
		WORD wMinAp,
		WORD wMaxAp,
		WORD wDp
		);

	//////////////////  Cash Item  /////////////////

	void				CleanupCashInven();
	void				InsertItemInCash_Buff( int nItemNum );
	void				InsertItemInCash( int nIdx, int nItemNum, int nContainCnt );
	void				DeleteItemInCash( int nIdx );
	void				CashItemUse( SItemData* pItemDat );

	//////////////////  개인상점 관련  /////////////////

	void				ShowOpenMyShop( BOOL bShow );
	void				ShowPersonalShop( BOOL bShow );
	
	void				InsertItemInMyShop(
		int		nItemId,
		int		nItemType
		);
	void				RemoveItemInMyShop( int nItemId );
	void				InsertItemInPersonalShop(
		int		nItemId,
		int		nItemType,
		int		nSellPrice,
		BYTE	btNum
		);
	void				RemoveItemInPersonalShop( int nItemId );

	void				BuyItemToPersonalShop( SItemData* pItemDat );
	void				RequestDeleteItemInPersonalShop( SItemData* pItemDat );

	//////////////////  무한전쟁  /////////////////

	void				AddCastleList( int nIndex, int nMapNumm );
	void				AddWarGuildInfo( 
		int nIndex,
		const char* szGuildName1, int nType1, int nPoint1,
		const char* szGuildName2, int nType2, int nPoint2
	);

	int					m_anCastleNum[20];
	int					m_nCastlePage;

	//
public:
	//////////////////  키보드어택  /////////////////
	BOOL			m_btIsMonsterAttack;
	BOOL			m_btIsUserAttack;
	BOOL			m_btIsCheck;
	int				m_nSaveSkill;
	BOOL			m_btIsCheck2;

	void			SetAttackMonster( BOOL btIsAttack ){ m_btIsMonsterAttack = btIsAttack; };	
	BOOL			GetAttackMonster(){ return m_btIsMonsterAttack; };

	void			SetCheck(BOOL btIsAttack ){ m_btIsCheck = btIsAttack; };
	BOOL			GetCheck(){ return m_btIsCheck; };

	void			SetAttackUser( BOOL btIsAttack ){ m_btIsUserAttack = btIsAttack; };	
	BOOL			GetAttackUser(){ return m_btIsUserAttack; };

	void			SetSaveQuickSlot( int nSlot ){ m_nSaveSkill = nSlot; };
	int				GetSaveQuickSlot( ){ return m_nSaveSkill; };
	void			UseSkill2( BYTE nIndx );


	void			SetSkillInfo( BOOL btIs ){ m_ItemMgr.SetCheckSkillInfo( btIs ); };

	void			SetRefreshNewCharacter( ) { RefreshNewCharacter(); };
	void			SetCharacterInitPOINT() { m_CharSelInfo.InitPOINT(); };


	int				m_nWidth;
	int				m_nHeight;
	

}; //class CW3DUInterface


#define			NORMAL_ATTACK1		100000001
#define			NORMAL_ATTACK2		100000002

#endif // #ifndef __W3DUInterface_Header__
