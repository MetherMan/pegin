/**
 * UI 컴포넌트 인터페이스 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-05-16 8:09p $
 * @version	$Revision: 75 $
 *
 * <HR>
 * $Log: /Engine/Inc/IW3DUInterface.h $
 * 
 * 75    03-05-16 8:09p Paran
 * NPC 스크립트 수정.
 * 
 * 74    03-05-16 8:37a Ahastudio
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

/**
 * 유저 인터페이스.
 * @ingroup		Engine
 */
class IW3DUInterface : public IComponent
{
public:
	enum
	{	// 메세지 박스 종류.
		MSG_NONE	= 0,	///< 그냥 메세지를 출력한다.
		MSG_OK		= 1,	///< 확인 버튼을 출력한다.
		MSG_YESNO	= 2		///< YES/NO 버튼을 출력한다.
	}; //enum

	enum
	{
		INVENON_ONEHANDWEAPON	= 2,
		INVENON_TWOHANDWEAPON	= 4,
		INVENON_BOW				= 8,
		INVENON_HELMET			= 16,
		INVENON_SHIRT			= 32,
		INVENON_BELT			= 64,
		INVENON_SHIELD			= 128,
		INVENON_SHOES			= 256,
		INVENON_HAND			= 512,
		INVENON_NECK			= 1024,
		INVENON_EARRING			= 2048,
		INVENON_RING			= 4096,
		INVENON_PANTS			= 8192,
		INVENON_SPELLBOOK		= 16384
	}; //enum

	/**
	 * 인터페이스를 초기화한다.
	 *
	 * @param	hWnd		[IN] 윈도우 핸들.
	 * @param	pInput		[IN] Input 컴포넌트.
	 *
	 * @return	초기화 성공 여부.
	 */
	virtual BOOL			Init( HWND hWnd, IW3DInput* pInput ) = 0;

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
	virtual void			SetPosSkill( int nSkill, int nMax, int nPos ) = 0;

	/**
	 * 게임에 쓰이는 몬스터MP그래프에 최대값과 현재값을 설정합니다
	 *
	 * @param	nMax		[IN] 그래프 최대값
	 * @param	nPos		[IN] 그래프 현재값
	 * @param	szText		[IN] Text
	 */
	virtual void			SetPosMonsterHP( int nMax, int nPos, const char* szText ) = 0;

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
		int*	pnIndex		///< [OUT] 캐릭터정보 인덱스
		) = 0;

	/**
	 *
	 */
	virtual BOOL			GetCharSelModelName(
		char*	szName, 
		BYTE*	pSex, 
		char*	szHair, 
		char*	szFace, 
		char*	szCoat, 
		char*	szPants, 
		char*	szHand, 
		char*	szFoot, 
		int*	pIndex 
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
		const int	nIndex		///< [IN] 캐릭터정보 인덱스
		) = 0;

	/**
	 * 현재 선택한 캐릭터 번호.
	 */
	virtual int				GetCharSelectNum() = 0;

	/**
	 * 현재 선택한 캐릭터 삭제.
	 */
	virtual int				DeleteCharInfo() = 0;

	/**
	 * 현재 선택한 캐릭터 삭제.
	 */
	virtual int				GetOnItemType( int nPart ) = 0;

	/**
	 *
	 */
	virtual BOOL			GetButtonState( const char *szID ) = 0;

	/**
	 *
	 */
	virtual BOOL			SetButtonState( const char *szID, BOOL bPush ) = 0;

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
	 * Dialog 컨트롤을 보이거나 숨긴다.
	 *
	 * @param	szDlgName	[IN] 다이얼로그 컨트롤 이름
	 * @param	bShow		[IN] TRUE(보이기) / FALSE(숨기기)
	 *
	 * @return	찾는 컨트롤러가 없으면 FALSE
	 */
	virtual BOOL			ShowDlg( const char *szDlgName, BOOL bShow ) = 0;

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
		char**		pszCtrlName,	///< [OUT] 발생 컨트롤 이름
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
	 *
	 */
	virtual BOOL			InventoryBlank( int nType ) = 0;

	/**
	 *
	 */
	virtual BOOL			InsertInventory( int nId, int nType ) = 0;
	virtual BOOL			InsertSerIntInventory( int nId, int nType ) = 0;
	
	/**
	 *
	 */
	virtual BOOL			InsertInventory(
		int		nId,		///< [IN]
		int		nType,		///< [IN]
		BYTE	btPage,		///< [IN]
		BYTE	btCol,		///< [IN]
		BYTE	btRow		///< [IN]
		) = 0;

	virtual BOOL			InsertSerIntInventory(
		int		nId,		///< [IN]
		int		nType,		///< [IN]
		BYTE	btPage,		///< [IN]
		BYTE	btCol,		///< [IN]
		BYTE	btRow		///< [IN]
		) = 0;

	/**
	 *
	 */
	virtual BOOL			ItemToHand( int nId ) = 0;

	/**
	 *
	 */
	virtual BOOL			ItemFromHand(
		int		nId,		///< [IN]
		BYTE	btTo,		///< [IN]
		BYTE	btPage,		///< [IN]
		BYTE	btCol,		///< [IN]
		BYTE	btRow		///< [IN]
		) = 0;

	/**
	 *
	 */
	virtual WORD			InsertEquipItem( int nId, int nType ) = 0;


	/**
	 *
	 */
	virtual void			InsertSmithItem( int nId, int nType, int price ) = 0;

	/**
	 * 마법창에 마법을 추가.
	 */
	virtual BOOL			InsertItemInSpellBook( 
		const int	nId,		///< [IN]
		const int	nType,		///< [IN]
		const BYTE	btPage,		///< [IN]
		const BYTE	btPos		///< [IN]
	) = 0;

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
	virtual BOOL			RemoveSpellBook( int nId ) = 0;
	
	/**
	 *
	 */
	virtual int				Attack_Short() = 0;

	/**
	 *
	 */
	virtual int				Attack_Long() = 0;

	/**
	 *
	 */
	virtual int				Attack_Magic1() = 0;

	/**
	 *
	 */
	virtual int				Attack_Magic2() = 0;

	/**
	 *
	 */
	virtual void			SelectNPC( const int nAddr ) = 0;

	
	/**
	 *
	 */
	virtual void			SelectSmithNPC( const int nAddr ) = 0;


	virtual void			SelectCardSumNPC( const int nAddr ) = 0;
	virtual void			SelectCardUndoNPC( const int nAddr ) = 0;

	virtual void			SelectServer( const int nAddr ) = 0;

	/**
	 *
	 */
	virtual BOOL			LoadMiniMap( const char* szMiniMapFile ) = 0;
	
	/**
	 *
	 */
	virtual void			SetPlayerMapPos( int nX, int nY ) = 0;

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
	 *
	 */
	virtual void			ShowShop() = 0;

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
	 *
	 */
	virtual void			SetTipText(
		const char*		szText,					///< [IN]
		DWORD			dwColor = 0xFFC2C2C2	///< [IN]
		) = 0;

	/**
	 *
	 */
	virtual void			ShowUserDealBox( BOOL bShow ) = 0;

	/**
	 *
	 */
	virtual BOOL			IsShowUserDealBox() = 0;


	/**
	 *
	 */
	virtual void			ShowSmithEnchentBox( BOOL bShow ) = 0;

	/**
	 *
	 */
	virtual void			InsertItemInUserDeal(
		int		nItemId,	///< [IN]
		int		nItemType,	///< [IN]
		WORD	wDestCol,	///< [IN]
		WORD	wDestRow,	///< [IN]
		BOOL	bMe			///< [IN]
		) = 0;

	/**
	 *
	 */
	virtual void			RemoveItemInUserDeal(
		int		nItemId,	///< [IN]
		BOOL	bMe			///< [IN]
		) = 0;

	/**
	 *
	 */
	virtual void			UpdateMoneyInUserDeal(
		int		nMoney,		///< [IN]
		BOOL	bMe			///< [IN]
		) = 0;

	/**
	 *
	 */
	virtual void			YouAndMe(
		const char*		szChoice1,	///< [IN]
		const char*		szChoice2,	///< [IN]
		BYTE			btYou,		///< [IN]
		int				nId = -1	///< [IN]
		) = 0;

	/**
	 *
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
	 *
	 */
	virtual void			DeletePartyMember( int nId ) = 0;
}; //class IW3DUInterface


#endif // #ifndef __Wind3D_Engine_IW3DUInterface_Header__
