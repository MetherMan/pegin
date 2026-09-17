/**
 * UI 컴포넌트 인터페이스 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-09-10 11:28p $
 * @version	$Revision: 83 $
 *
 * <HR>
 * $Log: /UInterface/Inc/IW3DUInterface.h $
 * 
 * 83    04-09-10 11:28p Paran
 * 
 * 46    03-12-10 2:04p Paran		인첸츠 카드 추가
 * 
 * 13    03-06-23 2:25a Paran		SetPlayerPower 추가.
 * 
 * 7     03-05-28 4:14p Paran		미니 맵 파티 관련 수정
 * 
 * 2     03-05-17 4:31p Paran		파티원 보이기 부분 수정.
 *
 * 75    03-05-16 8:09p Paran		NPC 스크립트 수정.
 *
 * 66    03-05-06 6:18p Paran		상점 기능 추가.
 *
 * 63    03-04-30 7:11p Paran		스킬 선택 수정.
 *
 * 58    03-04-15 1:27a Paran		NPC 추가.
 *
 * 57    03-04-12 3:03p Paran		캐릭터 장착 아이템 변경 기능 추가.
 *
 * 56    03-04-11 5:29p Paran		캐릭터 장착 부분과 미니맵 수정.
 *
 * 55    03-04-10 6:24p Paran		미니맵 추가.
 *
 * 53    03-04-02 8:11p Paran		플레이어 아이템추가, 메세지 박스 키 입력 추가.
 *
 * 51    03-03-26 9:19p Paran		인벤토리 어색한 부분 수정하고 체크.
 *
 * 50    03-03-26 6:22a Paran		아이템 서버 연결.
 *
 * 48    03-03-25 3:36a Paran		인벤토리 아이템 서버와 연결.
 *
 * 41    03-03-06 11:09p Paran		DeleteCharInfo() 추가.
 * <HR>
 *
 * @file	IW3DUInterface.h
 */


#ifndef __Wind3D_Engine_IW3DUInterface_Header__
#define __Wind3D_Engine_IW3DUInterface_Header__


#include "W3DEventCode.h"
#include <string>

using std::string;

struct SCreatureTarget;
class  CSelectCreature;

/**
 * 유저 인터페이스.
 * @ingroup		UInterface
 */
class IW3DUInterface : public IComponent
{
public:
	enum
	{	// 메세지 박스 종류.0..............................................................................................................................................................................................................0000000
		MSG_NONE	= 0,	///< 그냥 메세지를 출력한다.
		MSG_OK		= 1,	///< 확인 버튼을 출력한다.
		MSG_YESNO	= 2		///< YES/NO 버튼을 출력한다.
	}; //enum

	enum
	{
		YOU_NPC = 0, YOU_CHARACTER = 1
	}; //enum

	enum { NPC_TALK = 0x01, NPC_SHOP = 0x02, NPC_QUEST = 0x04, NPC_STORAGE = 0x08, NPC_GUILDMGR = 0x10,
		   NPC_SMITH = 0x20, NPC_CARD = 0x40, NPC_CARD1 = 0x80,
		   CHAR_DEAL = 0x200, CHAR_PARTY = 0x400 };
	
	enum { CAMERA_DEFAULT = 0, CAMERA_UNLOCK = 1 };	
	enum { CHATT_BROADCAST = 0, CHATT_SELL, CHATT_COUNTRY, CHATT_GUILD, CHATT_PARTY, CHATT_WORLD, CHATT_MEMBER, CHATT_FRIEND };

	/**
	 * 인터페이스를 초기화한다.
	 *
	 * @param	hWnd		[IN] 윈도우 핸들.
	 * @param	pInput		[IN] Input 컴포넌트.
	 *
	 * @return	초기화 성공 여부.
	 */
	virtual BOOL			Init( 
		int			nWidth, 
		int			nHeight,
		BOOL		bWinMouse
		) = 0;

	/**
	 * 다이얼로그 박스 초기화
	 */
	virtual void			InitDlg( IW3DCamera* pCamera ) = 0;

	/**
	 * 인벤토리 초기화
	 */
	virtual void			InitInventory() = 0;

	/**
	 * 각 장치 설정.
	 */
	virtual BOOL			SetWinDevice( HWND	hWnd, IW3DInput* pInput ) = 0;

	/**
	 * 유저 정보를 읽어 온다.
	 */
	virtual BOOL			LoadQuickData() = 0;

	/**
	 * 스크립트 파일에서 설정한 모드를 적용합니다.
	 * @see		Load
	 *
	 * @param	szMode		[IN] 스크립트 파일에서 정의한 모드
	 *
	 * @return	적용 성공 여부
	 */
	virtual BOOL			SetModeState( const char* szMode ) = 0;

	/**
	 * 모든 컨드롤러의 상태를 초기화한다.
	 */
	virtual void			Cleanup() = 0;

	/**
	 * 특정 컨트롤러의 상태를 초기화한다.
	 *
	 * @param	szID		[IN] 컨트롤러 ID
	 */
	virtual void			CleanupComponent( const char* szID ) = 0;

	/**
	 *
	 */
	virtual BOOL			ClearText( const char *szID ) = 0;

	/**
	 * 컨트롤러의 텍스트를 가져온다.
	 *
	 * @param	szID		[IN] 컨트롤러 아이디
	 *
	 * @return	Text. (찾는 컨트롤러가 없으면 NULL)
	 */
	virtual const char*		GetText( const char *szID ) = 0;

	/**
	 *
	 */
	virtual BOOL			SetText( const char *szID, const char *szText, DWORD dwColor = 0 ) = 0;

	/**
	 *
	 */
	virtual BOOL			SetText( const char *szID, int nDecimal, DWORD dwColor = 0 ) = 0;

	virtual void			SetNowWeight( int nowWeight, int MaxWeight ) = 0;

	/**
	 * 그림창에 스프라이트를 설정합니다.
	 *
	 * @param	szID		[IN] 컨트롤러 아이디
	 * @param	szPicName	[IN] 스프라이트 아이디
	 *
	 * @return	Text. (찾는 컨트롤러나 스프라이트가 없으면 NULL)
	 */
	virtual BOOL			SetPicture( const char* szID, const char* szPicName ) = 0;
	
	/**
	 * 그래프를 값을 설정.
	 */
	virtual BOOL			SetProgressPos( 
		const char*	szID, 
		int			nMax, 
		int			nPos, 
		BOOL		bProgress = TRUE,
		DWORD		dwColor = 0
		) = 0;
	
	/**
	 * 쳇팅 입력.
	 */
	virtual void			ChattingInput( 
		const string& strId, 			///< [IN] 입력자.
		const string& strContent,		///< [IN] 내용.
		DWORD dwColor = 0,				///< [IN] 텍스트 색.
		BYTE  btFontIndex = 0,
		BYTE  btGroup = 0
		) = 0;

	/**
	 * 멀티바이트 문자확인
	 */
	virtual BOOL			IsDoubleByteCharset( const char* pBuff ) = 0;

	/**
	 * 케릭터 이름 설정.
	 */
	virtual void			SetCharacterName( const char* szName ) = 0;
	
	/**
	 * 클레스 이름을 얻는다. 
	 */
	virtual BOOL			GetClassName( char* szClassName, const BYTE btClass ) = 0;

	/**
	 * 직업 이름 설정.
	 */
	virtual void			SetJobName( const BYTE btClass ) = 0;

	/**
	 * 게임에 쓰이는 HP그래프에 최대값과 현재값을 설정합니다
	 *
	 * @param	nMax		[IN] 그래프 최대값
	 * @param	nPos		[IN] 그래프 현재값
	 */
	virtual void			SetPosHP( int nMax, int nPos ) = 0;

	/**
	 * 게임에 쓰이는 MP그래프에 최대값과 현재값을 설정합니다
	 *
	 * @param	nMax		[IN] 그래프 최대값
	 * @param	nPos		[IN] 그래프 현재값
	 */
	virtual void			SetPosMP( int nMax, int nPos ) = 0;

	/**
	 * 게임에 쓰이는 MP그래프에 최대값과 현재값을 설정합니다
	 *
	 * @param	nMax		[IN] 그래프 최대값
	 * @param	nPos		[IN] 그래프 현재값
	 */
	virtual void			SetPosSkill( int nSkill, int nMax, int nPos, BOOL bProgress = TRUE ) = 0;

	/**
	 * 플레이어 레벨.
	 *
	 * @param	nType		[IN] 능력 종류	( 0 - Int, 1 - Str, 2 - Dex )
	 * @param	nLevel		[IN] 레벨
	 */
//	virtual void			SetPlayerPower( const int nType, const int nLevel ) = 0;

	virtual void			SetPlayerPower( const int nType, const int nLevel , BOOL btIs = FALSE ) = 0;

	/**
	 * 타겟HP그래프 보이기.
	 *
	 * @param	nMax		[IN] 그래프 최대값
	 * @param	nPos		[IN] 그래프 현재값
	 * @param	szText		[IN] Text
	 */
	virtual void			ShowTargetInfo( 
		int nId = 0,
		int nMax = 100, 
		int nPos = 0, 
		const char* szText = NULL ) = 0;

	/**
	 * 캐릭터 정보 초기화
	 */
	virtual void			InitCharSelectInfo() = 0;

	/**
	 * 현재 선택한 케릭터 정보를 얻는다
	 */
	virtual BOOL			GetCharSelInfo(
		char*	szName,		///< [OUT] 이름
		BYTE*	pbSex,		///< [OUT] 성별
		WORD*	pwHair,		///< [OUT] 머리
		WORD*	pwFace,		///< [OUT] 얼굴
		WORD*	pwCoat,		///< [OUT] 상의
		WORD*	pwPants,	///< [OUT] 하의
		WORD*	pwHand,		///< [OUT] 장갑
		WORD*	pwFoot,		///< [OUT] 신발
		int*	pnIndex,	///< [OUT] 캐릭터정보 인덱스
		BYTE*	pSTR,		///< [OUT] STR
		BYTE*	pINT,		///< [OUT] DEX
		BYTE*	pDEX,		///< [OUT] INT
		BYTE*	pClass		///< [OUT] 직업
		) = 0;

	/**
	 *
	 */
	virtual BOOL			GetCharSelModelName(
		const int nIndex,
		char*	szName, 
		BYTE*	pSex, 
		char*	szHair, 
		char*	szFace, 
		char*	szCoat, 
		char*	szPants, 
		char*	szHand, 
		char*	szFoot
		) = 0;

	/**
	 * 현재 선택한 캐릭터 정보를 입력한다.
	 */
	virtual void			SetCharSelInfo(
		const char*	szName,		///<
		const int	nLevel,		///<
		const int	nLLevel,	///<
		const int	nSLevel,	///<
		const int	nBLevel,	///<
		const int	nWLevel,	///<
		const BYTE	btSex,		///<
		const WORD	wHair,		///<
		const WORD	wFace,		///<
		const WORD	wCoat,		///<
		const WORD	wPants,		///<
		const WORD	wHand,		///<
		const WORD	wFoot,		///<
		const BYTE	btClass,	///<
		const int	nIndex		///< [IN] 캐릭터정보 인덱스
		) = 0;
	
	/**
	 * 캐릭터 생성
	 */
	virtual int				CreateCharacter() = 0;

	/**
	 * 현재 선택한 캐릭터 번호.
	 */
	virtual int				GetCharSelectNum() = 0;

	/**
	 * 현재 선택한 캐릭터 삭제.
	 */
	virtual void			DeleteCharInfo( const int nIndex  ) = 0;

	/**
	 * 캐릭터 죽고 살고.
	 */
	virtual void			PlayerLive( const BOOL bLive ) = 0;

	/**
	 * 장착창 아이템 얻기.
	 */
	virtual int				GetOnItemType( int nPart ) = 0;

	/**
	 *
	 */
	virtual BOOL			GetPushButtonState( const char *szID ) = 0;

	/**
	 *
	 */
	virtual BOOL			SetPushButtonState( const char *szID, BOOL bPush ) = 0;

	/**
	 * 커서를 설정합니다
	 *
	 * @param	szCursor	[IN] 스크립트에서 설정한 커서이름	
	 *							- "CURSOR_NORMAL"
	 *							- "CURSOR_SELECT"
	 *							- "CURSOR_EDIT"
	 *							- "CURSOR_ATTACK"
	 *
	 * @return	찾는 컨트롤러가 없으면 FALSE
	 */
	virtual BOOL			SetCursorMode( const char* szCursor ) = 0;

	/**
	 * UIComponent 컨트롤을 보이거나 숨긴다.
	 *
	 * @param	szDlgName	[IN] UIComponent 컨트롤 이름
	 * @param	bShow		[IN] TRUE(보이기) / FALSE(숨기기)
	 *
	 * @return	찾는 컨트롤러가 없으면 FALSE
	 */
	virtual BOOL			ShowComponent( const char* szCompName, BOOL bShow ) = 0;

	/**
	 * Dialog 컨트롤을 보이거나 숨긴다.
	 *
	 * @param	szDlgName	[IN] 다이얼로그 컨트롤 이름
	 * @param	bShow		[IN] TRUE(보이기) / FALSE(숨기기)
	 *
	 * @return	찾는 컨트롤러가 없으면 FALSE
	 */
	virtual BOOL			ShowDlg( const char *szDlgName, BOOL bShow, BOOL bEvent = FALSE ) = 0;

	/**
	 * Dialog 컨트롤의 창이 열려 있는지 확인할 수 있다.
	 *
	 * @param	szDlgName	[IN] DLG 컨트롤 이름
	 *
	 * @return	보여지고 있다면 TRUE, 찾는 컨트롤러가 없거나 보이지 않는 상태면 FALSE
	 */
	virtual BOOL			IsShowDlg( const char *szDlgName ) = 0;

	/**
	 * 이벤트 큐에서 이벤트 하나를 얻습니다.
	 *
	 * @return	이벤트가 없으면 FALSE 리턴
	 */
	virtual BOOL			PopEvent(
		char*		szUIName,		///< [OUT] 발생 컨트롤 이름
		UIEVENT*	pEvent,			///< [OUT] 이벤트
		DWORD*		pdwParam1,		///< [OUT] 추가 정보 #1
		DWORD*		pdwParam2		///< [OUT] 추가 정보 #2
		) = 0;

	/**
	 * 메세지 박스를 보여줍니다.
	 * szKey를 PARAN으로 할 경우, 버튼의 이름은 아래와 같다.
	 *	- nType이 1일 때 : BTN_MSG_PARAN_OK
	 *	- nType이 2일 때 : BTN_MSG_PARAN_YES, BTN_MSG_PARAN_NO
	 *
	 * @param	szText		[IN] 메세지 내용
	 * @param	szKey		[IN] 박스키 네임
	 * @param	nType		[IN] 박스 타입
	 *							- MSG_NONE  (0) 버튼없음
	 *							- MSG_OK    (1) 확인
	 *							- MSG_YESNO (2) 예/아니오
	 *
	 * @return	실패하면 FALSE
	 */
	virtual BOOL			MessageBox( const char* szText, char *szKey = NULL, int nType = 0 ) = 0;

	/**
	 * 메세지박스가 보여지고 있는지 확인
	 *
	 * @param	szText		[IN] 메세지 내용
	 *
	 * @return	실패하면 FALSE
	 */
	virtual BOOL			IsShowMessageBox() = 0;

	/**
	 * 인벤토리내 아이템 갯수 얻기.
	 */
	virtual int				GetItemNumInInventory( const int nType ) = 0;

	/**
	 *
	 */
	virtual BOOL			InventoryBlank() = 0;

	/**
	 *
	 */
	virtual BOOL			InsertInventory(
		int		nId,			///< [IN]
		int		nType,			///< [IN]
		BYTE	btPage,			///< [IN]
		BYTE	btCol,			///< [IN]
		BYTE	btRow,			///< [IN]
		BYTE	btCount,		///< [IN]
		BOOL	bSave = TRUE
		) = 0;

	virtual BOOL			InsertSerIntInventory(
		int		nId,			///< [IN]
		int		nType,			///< [IN]
		BYTE	btPage,			///< [IN]
		BYTE	btCol,			///< [IN]
		BYTE	btRow,			///< [IN]
		BYTE	btCount,		///< [IN]
		BOOL	bSave = TRUE
		) = 0;

	/**
	 * 아이템 내용 수정.
	 */
	virtual BOOL			UpdateItemData( const int nId, const BYTE btCount ) = 0;

	/**
	 *
	 */
	virtual WORD			InsertEquipItem( int nId, int nType ) = 0;

	/**
	 *
	 */
	virtual void			InsertSmithItem( int nId, int nType, int price ) = 0;

	/**
	 *
	 */	
	virtual BOOL			RemoveItemInInventory( int nId ) = 0;
	virtual BOOL			RemoveSerInventory( int nId ) = 0;

	/**
	 *
	 */
	virtual WORD			RemoveEquipItem( int nId, int nPos = 0 ) = 0;

	/**
	 *
	 */
	virtual void			RemoveSmithItem( int nId ) = 0;

	/**
	 *
	 */
	virtual void			AddSkill( const WORD wType ) = 0;
	/**
	 *
	 */
	virtual const char*		GetNPCName( const int nAddr ) = 0;

	/**
	 *
	 */
	virtual void			SelectNPC( const int nAddr, const int nTalkNum ) = 0;

	/**
	 *                                                                     
	 */
	virtual void			SelectSmithNPC( const int nAddr, const int nTalkNum ) = 0;

	/**
	 *                                                                     
	 */
	virtual void			SelectCardSumNPC( const int nAddr, const int nTalkNum ) = 0;

	virtual void			SelectCardUndoNPC( const int nAddr , const int nTalkNum ) = 0;

	virtual void			SelectServer( const int nAddr ) = 0;

	/**
	 *
	 */
	virtual BOOL			LoadMiniMap( WORD wMap ) = 0;
	
	/**
	 *
	 */
	virtual void			SetPlayerMapPos( const int nX, const int nY ) = 0;

	/**
	 *
	 */
	virtual void			SetPlayerMapDir( const float fDirX, const float fDirY ) = 0;
	
	/**
	 *
	 */
	virtual void			AddShopMapPos( const int nX, const int nY ) = 0;

	/**
	 *
	 */
	virtual void			UpdateMoney( int nMoney ) = 0;

	virtual void			UpdateCash( int nCash ) = 0;

	/**
	 *
	 */
	virtual void			InsertItemInShop(
		int		nItemType,
		BYTE	btPage,
		BYTE	btCol,
		BYTE	btRow
		) = 0;


	/**
	 * 상점 보이기.
	 */
	virtual void			ShowShop( BOOL bShow ) = 0;

	/**
	 * 창고에 아이템 추가.
	 */
	virtual BOOL			InsertItemInStorage( const int nId, const int nType, const BYTE	btCnt ) = 0;

	virtual BOOL			InsertItemInSerGarbage( const int nId, const int nType, const BYTE	btCnt ) = 0;

	/**
	 * 창고 보이기.
	 */
	virtual void			ShowStorage( BOOL bShow ) = 0;

	/**
	 * 창고안에 아이템을 삭제.
	 */
	virtual BOOL			RemoveItemInStorage( const int nId ) = 0;
	virtual BOOL			RemoveSerItemInStorage( const int nId ) = 0;

	/**
	 * 창고 모든 아이템을 삭제.
	 */
	virtual void			RemoveAllItemInStorage() = 0;
	virtual void			RemoveAllSerItemInStorage() = 0;

	/**
	 * 상점번호 저장.
	 */
	virtual void			SetShopNum( const WORD wShop ) = 0;

	/**
	 * 퀘스트번호 저장.
	 */
	virtual void			SetQuestNum( const WORD wQuest ) = 0;

	/**
	 * 퀘스트 설정.
	 */
	virtual void			Quest( WORD wNPCId, WORD wQuest, BYTE btProc ) = 0;

	/**
	 * 퀘스트 리스트 추가.
	 */
	virtual void			AddQuestInList( WORD wQuestNum, BYTE btState ) = 0;

	/**
	 *
	 */
	virtual BOOL			LoadGameReady( WORD wMap ) = 0;

	/**
	 * 모든 내용을 표시합니다.
	 */
	virtual void			Draw() = 0;

	/**
	 * 내용을 업데이트합니다.
	 *
	 * @param	dwTick		[IN] 경과 tick
	 *
	 * @return	User Interface에 포커스가 있다면 TRUE를 리턴합니다. 아니면 FALSE.
	 */
	virtual BOOL			Update( DWORD dwTick ) = 0;

	/**
	 * IME 처리를 합니다.
	 *
	 * @return	IME처리가 있다면 TRUE를 리턴합니다.
	 */
	virtual BOOL			IMEProc(
		HWND	hWnd,
		UINT	uMsg,
		WPARAM	wParam,
		LPARAM	lParam
		) = 0;

	/**
	 * 현재 ime 입력 상태인지 체크.
	 */
	virtual BOOL			IsIMEInput() = 0;

	/**
	 *
	 */
	virtual void			SetTipText(
		const char*		szText,					///< [IN]
		DWORD			dwColor = 0xFFC2C2C2	///< [IN]
		) = 0;

	/**
	 * 유저거래 창 보이기 여부.
	 */
	virtual void			ShowUserDealBox( BOOL bShow ) = 0;


	/**
	 * 대장장이 창 보이기 여부.
	 */
	virtual void			ShowSmithEnchentBox( BOOL bShow ) = 0;


	/**
	 * 유저거래 아이템 추가.
	 */
	virtual void			InsertItemInUserDeal(
		int		nItemId,	///< [IN]
		int		nItemType,	///< [IN]
		WORD	wDestCol,	///< [IN]
		WORD	wDestRow,	///< [IN]
		BOOL	bMe			///< [IN]
		) = 0;

	/**
	 * 유저거래 아이템 삭제.
	 */
	virtual void			RemoveItemInUserDeal(
		int		nItemId,	///< [IN]
		BOOL	bMe			///< [IN]
		) = 0;

	/**
	 * 유저거래 머니 업데이트.
	 */
	virtual void			UpdateMoneyInUserDeal(
		int		nMoney,		///< [IN]
		BOOL	bMe			///< [IN]
		) = 0;

	/**
	 * 개인상점 아이템 추가
	 */
	virtual void			InsertItemInMyShop(
		int		nItemId,	///< [IN]
		int		nItemType	///< [IN]
		) = 0;

	/**
	 * 내 개인상점 아이템 삭제
	 */
	virtual void			RemoveItemInMyShop( int nItemId ) = 0;

	/**
	 * 개인상점 아이템 추가
	 */
	virtual void			InsertItemInPersonalShop(
		int		nItemId,
		int		nItemType,
		int		nSellPrice,
		BYTE	btNum
		) = 0;

	/**
	 * 개인상점 아이템 삭제
	 */
	virtual void			RemoveItemInPersonalShop( int nItemId ) = 0;

	/**
	 * 상대와 대화 처리.
	 */
	virtual void			YouAndMe(
		int				nId,				///< [IN] 상대 아뒤.
		BYTE			btYou,				///< [IN] 상대 타입.
		const DWORD		dwChoiceFlag		///< [IN] 선택 프레그.
		) = 0;

	/**
	 * 파티 게시판 리스트 추가.
	 */
	virtual void			AddPartyBoardList(
		const char*		szName,				///< [IN] 캐릭터 이름.
		const int		nLevel,				///< [IN] 레벨.
		const int		nSSkill,			///< [IN] 근거리 스킬.
		const int		nLSkill,			///< [IN] 원거리 스킬.
		const int		nBSkill,			///< [IN] 흑마법 스킬.
		const int		nWSkill				///< [IN] 백마법 스킬.
		) = 0;

	/**
	 * 파티원 추가.
	 */
	virtual void			AddPartyMember(
		int				nId,		///< [IN]
		const char*		szName,		///< [IN]
		int				nHp,		///< [IN]
		int				nMaxHp,		///< [IN]
		int				nMp,		///< [IN]
		int				nMaxMp		///< [IN]
		) = 0;

	/**
	 * 파티원 탈퇴.
	 *
	 * @param	nId		[IN] 파티원 ID
	 */
	virtual void			DeletePartyMember( int nId ) = 0;

	/**
	 * 파티원 전체 해제.
	 */
	virtual void			DeletePartyAll() = 0;
	
	/**
	 * 파티원인지 확인.
	 */
	virtual BOOL			IsPartyMember( const DWORD dwId ) = 0;

	/**
	 * 파티원 정보 갱신.
	 */
	virtual void			RefreshPartyMember( int nId, int nHp, int nMaxHp, int nMp, int nMaxMp ) = 0;
	
	/**
	 * 파티원 정보 갱신.
	 */
	virtual void			RefreshPartyMember( int nId, WORD wMap, int nX, int nY ) = 0;
	
	/** 
	 * 길드 초청 받았다.
	 */
	virtual void			RequestInviteGuild( WORD wGuildNo, int nMasterId, const char* szGuildName ) = 0;
	
	/**
	 * 메인스킬 설정.
	 */
	virtual	void			SetMainSkill( const int nSkillItem ) = 0;
	
	/**
	 * 타임머 보이기.
	 */
	virtual void			ShowTimer( const char* szCompName, DWORD dwShowTick ) = 0;
	
	/**
	 * 사운드 상태 설정.
	 */
	virtual void			SetSoundState( BOOL bOnBGM, int nVolBGM, BOOL bOnEff, int nVolEff ) = 0;
	
	/**
	 * 귓속말 상대 이름 저장.
	 */
	virtual void			SaveWhisperName( const char* szName ) = 0;

	/**
	 * 귓속말 상대 이름 얻기
	 */
	virtual char*			GetWhisperName() = 0;

	/**
	 * 귓속말 상대 설정
	 */
	virtual void			SetWhisperName( const char* szName ) = 0;

	/**
	 * 쳇팅 내용 얻기
	 */
	virtual char*			GetChattingText() = 0;

	/** 
	 * Add Player List
	 */
	virtual void			AddPlayerList( const DWORD dwId, const char* szName, const BYTE btClass, const int nTotLev ) = 0;


	/**
	 * Remove Player List
	 */
	virtual void			RemovePlayerList( const DWORD dwId, const char* szName ) = 0;
	
	/**
	 * 인첸트 카드 설정
	 */
	virtual void			EnchantResult( BOOL bResult ) = 0;

	/** 
	 * 적용스킬 아이콘 추가
	 */
	virtual void			AddSkillImage( WORD wSkillId, DWORD dwTime ) = 0;
	
	/**
	 * 적용스킬 아이콘 삭제
	 */
	virtual void			RemoveSkillImage( WORD wSkillId ) = 0;

	/**
	 * 이벤트 메세지 박스 보이기(중국)
	 */
	virtual void			ShowEventMessageBox( const BYTE btResult ) = 0;

	/**
	 * Is My Friend
	 */
	virtual BOOL			IsFriend( const char* szName ) = 0;

	/**
	 * Add Friend
	 */
	virtual void			AddFriend( const char* szFriendName ) = 0;

	/**
	 * Delete Friend
	 */
	virtual void			RemoveFriend( const char* szFriendName ) = 0;

	virtual void			ClearFriendList() = 0;

	/**
	 * Clear GuildMember 
	 */
	virtual void			ClearGuildMember() = 0;

	/**
	 * Add Guild
	 */
	virtual void			AddGuildMember( const char* szName, const BYTE btClass, const int nTotLev ) = 0;

	/**
	 * Remove Guild
	 */
	virtual void			RemoveGuildMember( const char* szName ) = 0;

	/**
	 * Friend Login Or Logout
	 */
	virtual void			FriendLogInOut( const char* szFriendName, BOOL bLogin ) = 0;

	/**
	 * Chatting with friend
	 */
	virtual void			ChattingFriend( const char* szFriendName, const char* szMemo ) = 0;
	
	/**
	 * Update Pet Info
	 */
	virtual void			UpdatePet( 
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
		) = 0;

	/**
	 * Show the OpenMyShop
	 */
	virtual void			ShowOpenMyShop( BOOL bShow ) = 0;

	/**
	 * Show the PersonalShop
	 */
	virtual void			ShowPersonalShop( BOOL bShow ) = 0;
	
	/**
	 * Check the group comp 
	 */
	virtual BOOL			CheckGroup( const char* szTabKey, const char* szInvenKey, const int nIndex ) = 0;

	/**
	 * 성 정보  
	 */
	virtual void			AddCastleList( int nIndex, int nMapNum ) = 0;

	/**
	 *  전쟁정보 추가
	 */
	virtual void			AddWarGuildInfo( 
		int nIndex,
		const char* szGuildName1, int nType1, int nPoint1,
		const char* szGuildName2, int nType2, int nPoint2
	) = 0;

	/**
	 * 캐쉬 인벤 클린업 
	 */
	virtual void			CleanupCashInven() = 0;

	/**
	 *  버프인벤에 아이템 삽입
	 */
	virtual void			InsertItemInCash_Buff( int nItemNum ) = 0;

	/**
	 * 케쉬인벤에 아이템 삽입 
	 */
	virtual void			InsertItemInCash( int nIdx, int nItemNum, int nContainCnt ) = 0;

	/**
	 * 케쉬이벤에 아이템 삭제 
	 */
	virtual void			DeleteItemInCash( int nIdx ) = 0;

	/**
	 * 스킬 타이머 작동 
	 */
	virtual void			SetSkillTimer( int nItemId, int nTime ) = 0;

	/**
	* 키보드 공격 어택	
	*/
	virtual	void			SetAttackMonster( BOOL btIsAttack ) = 0;	
	virtual	BOOL			GetAttackMonster() = 0;
	virtual	void			SetAttackUser( BOOL btIsAttack ) = 0;	
	virtual	BOOL			GetAttackUser() = 0;
	virtual	void			SetCheck( BOOL btIsAttack ) = 0;	
	virtual	BOOL			GetCheck() = 0;

	virtual	void			UseSkill2( BYTE nIndx ) = 0;
	virtual int				GetSaveQuickSlot() = 0;
	virtual void			SetSaveQuickSlot( int nSlot ) = 0;

	virtual void			SetSkillInfo( BOOL btIS ) = 0;


	//
	virtual	void			SetRefreshNewCharacter() = 0;
	virtual	void			SetCharacterInitPOINT() = 0;

}; //class IW3DUInterface

#endif // #ifndef __Wind3D_Engine_IW3DUInterface_Header__
