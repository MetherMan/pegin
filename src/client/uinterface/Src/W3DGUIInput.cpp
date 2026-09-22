/**
 * 유저 인터페이스 외부입력.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-12-12 12:49a $
 * @version	$Revision: 141 $
 *
 * <HR>
 * $Log: /UInterface/Src/W3DGUIInput.cpp $
 * 
 * 141   04-12-12 12:49a Sk8snow2
 * 
 * 140   04-12-08 3:07a Sk8snow2
 * 
 * 139   04-11-16 7:11p Sk8snow2
 * 
 * 138   04-09-24 4:46p Paran
 * 
 * 137   04-09-10 11:28p Paran
 * 
 * 99    04-01-02 2:21p Paran		메세지 define 처리
 * 
 * 82    03-12-10 2:04p Paran		인첸츠 카드 추가
 * 
 * 81    03-12-09 5:58p Paran		아이템 데이터 수정
 * 
 * 79    03-12-04 9:14p Sckyj		비채팅모드일경우 IME Message를 차단
 * 
 * 77    03-12-03 2:35p Sckyj		IMEProc() 에서 m_pCurrEditor가 없을경우에도 IME Message를 받을경우에는
									bProc을 true로 하여 DefWindowProc으로 들어가지 않도록 수정
 * 
 * 73    03-11-19 8:08p Sckyj		중국어 IME관련 작업 진행중
 * 
 * 67    03-10-09 10:42a Sk8snow2	MoveItemInInventory 수정 ( 물약 겹치기 )
 * 
 * 56    03-09-15 1:42a Paran		Quick Slot 포션 갯수 수정.
 * 
 * 19    03-06-24 3:37p Paran		소리 추가.
 * 
 * 18    03-06-18 10:16p Paran		인벤토리 손 바꾸기, 캐릭터 선택 창 수정
 * 
 * 11    03-06-05 3:19p Paran		양손검
 * 
 * 10    03-05-29 3:08p Paran		아이탬 바꿔 줌 
 * 
 * 7     03-05-25 4:10p Paran		물약 다량 구입 인터페이스 추가
 * 
 * 5     03-05-23 3:42p Paran		버그수정 중...
 * 
 * 4     03-05-20 9:35p Paran		계속 수정 중...
 * <HR>
 *
 * @file	W3DGUIInput.cpp
 */

#include "UInterface_Export.h"
#include "W3DUInterface.h"
#include "NotUseWord.h"

/**
 * 마우스 왼쪽버튼 다운.
 */
BOOL CW3DUInterface::MouseButtonL_Down()
{
	ResetTopDlg();

	if( m_pCurrEditor )	m_pCurrEditor->EditOff();
	m_pCurrEditor = NULL;

	m_pOldFocusCom = m_pCurrCommopent;
	if( !m_pCurrCommopent )	
	{
		if( m_pTopDlg && m_pTopDlg->IsTopMost() )
		{
			HideActiveDlg();
			return TRUE;
		} //if
		return FALSE;
	}

	switch( m_pCurrCommopent->m_nType )
	{
	case UI_PUSHBUTTON:			PushButton_LDown( m_pCurrCommopent );		return TRUE;
	case UI_EDITTEXT:			EditBox_LDown( m_pCurrCommopent );			return TRUE;
	case UI_PICTURE:			PictureBox_LDown( m_pCurrCommopent );		return TRUE;
	case UI_SELECTSTRING:		SelectString_LDown( m_pCurrCommopent );		return TRUE;
	case UI_INVENTORYIN:		InventoryIn_LDown( m_pCurrCommopent );		return TRUE;
	case UI_INVENTORYON:		InventoryOn_LDown( m_pCurrCommopent );		return TRUE;
	case UI_SCROLLBAR:			ScrollBar_LDown( m_pCurrCommopent );		return TRUE;
	case UI_QUICKSLOT:			QuickSlot_LDown( m_pCurrCommopent );		return TRUE;
	case UI_SKILLENTRY:			SkillEntry_LDown( m_pCurrCommopent );		return TRUE;
	case UI_DIALOGBOX:			DialogBox_LDown( m_pCurrCommopent );		return TRUE;
	case UI_RECTANGLE:			Rectangle_LDown( m_pCurrCommopent );		return TRUE;
	} //switch

	return IsShowLockDlg();
} //CW3DUInterface::MouseButtonL

/**
 * 마우스 왼쪽버튼 업.
 */
BOOL CW3DUInterface::MouseButtonL_Up()
{
	if( !m_pCurrCommopent )
	{
		if( ItemFromHand() )		return TRUE;
	}
	else
	{
		switch( m_pCurrCommopent->m_nType )
		{
		case UI_QUICKSLOT:			QuickSlot_LUp( m_pCurrCommopent );			return TRUE;
		case UI_INVENTORYIN:		InventoryIn_LUp( m_pCurrCommopent );		return TRUE;
		case UI_INVENTORYON:		InventoryOn_LUp( m_pCurrCommopent );		return TRUE;
		case UI_SKILLENTRY:			SkillEntry_LUp( m_pCurrCommopent );			return TRUE;
		} //switch
	} //if..else

	if( m_pOldFocusCom )
	{
		switch( m_pOldFocusCom->m_nType )
		{
		case UI_PUSHBUTTON:			PushButton_LUp( m_pOldFocusCom );			return TRUE;
		case UI_PICTURE:														return TRUE;
		case UI_CHECKBUTTON:		CheckButton_LUp( m_pOldFocusCom );			return TRUE;
		case UI_TABBUTTON:			Tab_LUp( m_pOldFocusCom );					return TRUE;
		case UI_SELECTSTRING:		SelectString_LUp( m_pOldFocusCom );			return TRUE;
		case UI_SCROLLBAR:														return TRUE;
		case UI_DIALOGBOX:			DialogBox_LUp( m_pOldFocusCom );			return TRUE;
		case UI_EDITTEXT:														return TRUE;
		case UI_MINIMAP:														return TRUE;
		case UI_RECTANGLE:														return TRUE;
		} //switch
	} //if
	
	return IsShowLockDlg();
}

/**
 *  마우스 오른쪽 버튼 업.
 */
BOOL CW3DUInterface::MouseDBClickL()
{
	if( !m_pCurrCommopent )
	{
		return FALSE;
	} //if

	m_pOldFocusCom = m_pCurrCommopent;
	if( !m_pCurrCommopent )
	{ // 포커스가 없을 때...
		if( m_pTopDlg && m_pTopDlg->IsTopMost() )
		{
			HideActiveDlg();
			return TRUE;
		} //if
		return FALSE;
	} //if
	
	switch( m_pCurrCommopent->m_nType )
	{
	case UI_INVENTORYIN:		InventoryInDBClick_L( m_pCurrCommopent );			return TRUE;
	case UI_INVENTORYON:		InventoryOnDBClick_L( m_pCurrCommopent );			return TRUE;
	case UI_SKILLENTRY:			SkillEntryDBClick_L( m_pCurrCommopent );			return TRUE;
	case UI_SELECTSTRING:		SelectStringDBClick_L( m_pCurrCommopent );			return TRUE;
	} //switch
	return FALSE;
} //CW3DUInterface::MouseDBClickL

/**
 *
 */
BOOL CW3DUInterface::MouseButtonR_Down()
{
	ResetTopDlg();

	m_pOldFocusCom = m_pCurrCommopent;
	if( !m_pCurrCommopent )
	{ // 포커스가 없을 때...
		if( m_pTopDlg && m_pTopDlg->IsTopMost() )
		{
			HideActiveDlg();
			return TRUE;
		} //if
		return FALSE;
	} //if

	switch( m_pCurrCommopent->m_nType )
	{
	case UI_INVENTORYIN:		InventoryIn_RUp( m_pCurrCommopent );			return TRUE;
	} //switch

	return IsShowLockDlg();
} //CW3DUInterface::MouseButtonR

/**
 *  마우스 오른쪽 버튼 업.
 */
BOOL CW3DUInterface::MouseButtonR_Up()
{
	return IsShowLockDlg();
} //CW3DUInterface::MouseButtonR_Up

/**
 * 마우스 이동.
 */
BOOL CW3DUInterface::MouseMove()
{
	m_nMouseX = m_pInput->GetMouseX();
	m_nMouseY = m_pInput->GetMouseY();
	if( m_nMouseX == m_nOldMouseX && m_nMouseY == m_nOldMouseY )
	{ // 이동이 없을 시.
		return TRUE;
	} //if

	if( m_pOldFocusCom )
	{
		switch( m_pOldFocusCom->m_nType )
		{ // 마우스 다운시 위치.
		case UI_SCROLLBAR:		
			ScrollThumb_Move( m_pOldFocusCom );				
			break;

		case UI_DIALOGBOX:		
			DialogBox_Move( (UIDialog*)m_pOldFocusCom );	
			break;

		case UI_INVENTORYIN:	
			if( m_pCurrCommopent && m_pCurrCommopent->m_nType == UI_CHECKBUTTON )
			{
				CheckButton_Move( m_pCurrCommopent );
			} //if
			break;
		} //switch
	} //if

	m_nOldMouseX = m_nMouseX;
	m_nOldMouseY = m_nMouseY;

	////////////  포커스 얻기  //////////

	UICComponent*	pComp = GetFocusUIComponent();

	///////////	 Focus Dlg	///////////

	if( m_pCurrCommopent )
	{
		m_pCurrCommopent->MouseMove( m_nMouseX, m_nMouseY );
		if( m_pCurrCommopent == pComp )
		{
			return TRUE;
		}
		else
		{
			m_pCurrCommopent->FocusOut();
		} //if..else
	} //if

	if( pComp )
	{
		SetTipText( pComp->GetInfoText(), 0XFF000000 );

		if( pComp->m_nType == UI_STRLIST )		pComp = NULL;
		else									pComp->FocusIn();
	}
	else
	{
		SetTipText( NULL );
	} //if..else
	
	m_pCurrCommopent = pComp;

	SetCursorMode();

	return TRUE;
} //CW3DUInterface::MouseMove

/**
 * 리턴키 입력시.
 */
BOOL CW3DUInterface::ReturnKey()
{
	if( m_pCurrEditor )
	{
		if( ReturnKey_EditText() )
		{
			return TRUE;
		} //if..else

		PushEvent( (char*)m_pCurrEditor->m_strName.c_str(), EVENT_RETURNKEY, 0, 0 );		
	}
	else if( m_pTopDlg && m_pTopDlg->IsTopMost() )
	{
		DlgYesExe();
	}
	else 
	{
		UICComponent*	pComp = GetComponent( "EDT_CHATINPUT" );
		Assert( pComp );

		EditBox_LDown( pComp );		
		return TRUE;
	} // if..else

	if( ReturnKey_Button() )
	{
		return TRUE;
	}

	return FALSE;
}

/**
 * 탭키 입력시.
 */
BOOL CW3DUInterface::TabKey()
{
	if( m_pCurrEditor ) 
	{
		PushEvent( (char*)m_pCurrEditor->m_strName.c_str(), EVENT_TABKEY, 0, 0 );
	}
	else if( m_pCurrCommopent )
	{
		PushEvent( (char*)m_pCurrCommopent->m_strName.c_str(), EVENT_TABKEY, 0, 0 );
	}
	else 
	{
		PushEvent( NULL, EVENT_TABKEY, 0, 0 );
	}

	if( TabKey_EditText() )
	{
		return TRUE;
	}
	return FALSE;
}

//
// ReturnKey_Button
//
BOOL CW3DUInterface::ReturnKey_Button()
{
	if( m_pCurrCommopent )
	{
		if( m_pTopDlg )
		{
			/////////////////////// 메세지 박스 ///////////////////

			if( !m_pTopDlg->m_strName.compare( "DLG_MESSAGEBOX" ) )
			{
				UICComponent*		pComp = GetComponent( "BTN_MSG_OK" );
				Assert( pComp );
				if( pComp->IsShow() )
				{
					PushButton_LUp( pComp );
					return TRUE;
				}

				pComp = GetComponent( "BTN_MSG_YES" );
				Assert( pComp );
				if( pComp->IsShow() )
				{
					PushButton_LUp( pComp );
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

//
// CW3DUInterface::ReturnKey_EditText
//
BOOL CW3DUInterface::ReturnKey_EditText()
{
	if( m_pCurrEditor )
	{
		/////////////////////////
		//	Chatting Bar	   //
		/////////////////////////
		if( m_pCurrEditor->m_strName == "EDT_IDINPUT" )					// 쳇아이디 창
		{
/*			EditBox_LDown( m_pCurrEditor );

			UISelectString*		pSelStr = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_CHATID_SAVE" ) );
			Assert( pSelStr );
			pSelStr->AddString( m_pCurrEditor->GetText() );
*/		}
		else if( m_pCurrEditor->m_strName == "EDT_CHATINPUT" )			// 쳇팅입력창에서 
		{
			m_pCurrEditor->EditOff();

			if( m_btInputSystem )
			{
				m_btInputSystem = FALSE;
				char*	pText = m_pCurrEditor->GetText();
				if( pText || VerifyCharacterSet( pText ) )
				{
					int		nGuildNameLen = strlen( pText );
					if( nGuildNameLen <= 0 )
					{
						MessageBox( STR_SYSMSG_EXCEPTSTRING );
					}
					else if( nGuildNameLen <= 32 )
					{
						char		szMessage[64];
						sprintf( szMessage, "%s\n%s", pText, STR_UIASK_CREATEGUILD );
						strcpy( m_szRecentChattText, pText );

						MessageBox( szMessage, "CREATEGUILD", MSG_YESNO );
					} //if..else
				} //if
			}
			else 
			{
				char*		szText = m_pCurrEditor->GetText();
				if( szText && CheckWord( szText ) )
				{
					if( SaveChattingText() )
					{
						BYTE	btGroupIdx = GetCheckButtonIndex( "CHK_CHAT_TAB" );
						btGroupIdx--;
						PushEvent( (char*)m_pCurrEditor->m_strName.c_str(), EVENT_SENDCHATTING, 0, btGroupIdx );
					} //if
				}
				else if( szText )
				{
					ChattingInput( "", STR_SYSMSG_EXCEPTSTRING, FONT_MSGCOLOR, 0, 0 );
				} //if..else
			} //if..else

			m_pCurrEditor->Cleanup();
			m_pCurrEditor = NULL;

			return TRUE;
		}

		/////////////////////////
		//	Friend Chatting	   //
		/////////////////////////
		else if( m_pCurrEditor->m_strName == "EDT_MESSENSGER" )					// 쳇아이디 창
		{
			m_pCurrEditor->EditOff();

			char*		szText = m_pCurrEditor->GetText();
			if( szText && CheckWord( szText ) )
			{
				strcpy( m_szRecentChattText, m_pCurrEditor->GetText() );

				UISelectString*		pSelStr = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_CHATID_SAVE" ) );
				if( !pSelStr )
				{
					Assert( !"Not Nound Messeenger Comp" );
					return FALSE;
				} //if

				char*	pText = pSelStr->GetText();
				if( !pText )
				{
					return FALSE;
				} //if
				strcpy( m_szRecentWhisperName, pSelStr->GetText() );
				PushEvent( (char*)m_pCurrEditor->m_strName.c_str(), EVENT_SENDCHATTING, 0, CHATT_FRIEND );
				PushEvent( 0, 0, 0, 0 );
			}
			else if( szText )
			{
				ChattingInput( "", STR_SYSMSG_EXCEPTSTRING, FONT_MSGCOLOR, 0, 0 );
			} //if..else

			m_pCurrEditor->Cleanup();
			m_pCurrEditor->EditOn();

			return TRUE;
		}

		/////////////////////////
		//	Login Box		   //
		/////////////////////////
		else if( m_pCurrEditor->m_strName == "EDT_LOGIN_ID" ||			// 로긴에서
				 m_pCurrEditor->m_strName == "EDT_LOGIN_PW" )
		{
			UIEditText*		pEditID = (UIEditText*)GetComponent( "EDT_LOGIN_ID" );
			//pEditID->m_nID = strlwr( pTestID->m_pText );
			if( !pEditID )
			{
				Assert( !"EDT_LOGIN_ID not found!!" );
				return FALSE;
			}

			UIEditText*		pEditPW = (UIEditText*)GetComponent( "EDT_LOGIN_PW" );
			if( !pEditPW )
			{
				Assert( !"EDT_LOGIN_PW not found!!" );
				return FALSE;
			}

			if( pEditID->GetText()[0] == '\0' )
			{
				m_pCurrEditor->EditOff();
				EditBox_LDown( (UICComponent*)pEditID );
				return TRUE;
			}
			else if( pEditPW->GetText()[0] == '\0' )
			{
				m_pCurrEditor->EditOff();
				EditBox_LDown( (UICComponent*)pEditPW );
				return TRUE;
			}
			UICComponent*	pCompOK = GetComponent( "BTN_LOGIN_YES" );
			if( !pCompOK )
			{
				Assert( !"BTN_LOGIN_YES not found!!" );
				return FALSE;
			}
			PushButton_LUp( pCompOK );
		}
	}
	else
	{
		return FALSE;
	} //if..else

	PushEvent( (char*)m_pCurrEditor->m_strName.c_str(), EVENT_EDITTEXTNEW, 0, 0 );
	return TRUE;
}

//
// CW3DUInterface::TabKey_EditText
//
BOOL CW3DUInterface::TabKey_EditText()
{
	if( m_pCurrEditor )
	{
		/////////////////////////
		//	Chatting Bar	   //
		/////////////////////////
		if( m_pCurrEditor->m_strName == "EDT_IDINPUT" )					// 쳇아이디 창
		{
			m_pCurrEditor->EditOff();

			UIEditText*		pChatInput = reinterpret_cast<UIEditText*>( GetComponent( "EDT_CHATINPUT" ) );
			Assert( pChatInput );
			EditBox_LDown( (UICComponent*)pChatInput );

/*			UISelectString*		pSelStr = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_CHATID_SAVE" ) );
			Assert( pSelStr );
			pSelStr->AddString( m_pCurrEditor->GetText() );
*/		}
		else if( m_pCurrEditor->m_strName == "EDT_CHATINPUT" )				// 쳇 창
		{
			UIEditText*		pIdInput = (UIEditText*)GetComponent( "EDT_IDINPUT" );
			if( !pIdInput )
			{
				Assert( !"EDT_IDINPUT 찾지 못했습니다" );
				return FALSE;
			}
			m_pCurrEditor->EditOff();
			EditBox_LDown( (UICComponent*)pIdInput );
		}

		/////////////////////////
		//	Login Box		   //
		/////////////////////////
		else if( m_pCurrEditor->m_strName == "EDT_LOGIN_ID" )				// 로긴 창
		{
			UIEditText*		pLoginPW = (UIEditText*)GetComponent( "EDT_LOGIN_PW" );
			if( !pLoginPW )
			{
				Assert( !"EDT_LOGIN_PW 찾지 못했습니다" );
				return FALSE;
			}
			m_pCurrEditor->EditOff();
			EditBox_LDown( (UICComponent*)pLoginPW );
		}
		else if( m_pCurrEditor->m_strName == "EDT_LOGIN_PW" )				// 로긴 창
		{
			UIEditText*		pLoginID = (UIEditText*)GetComponent( "EDT_LOGIN_ID" );
			if( !pLoginID )
			{
				Assert( !"EDT_LOGIN_ID 찾지 못했습니다" );
				return FALSE;
			}
			m_pCurrEditor->EditOff();
			EditBox_LDown( (UICComponent*)pLoginID );
		}
	}
	else
	{
		return FALSE;
	}

	PushEvent( (char*)m_pCurrEditor->m_strName.c_str(), EVENT_EDITTEXTNEW, 0, 0 );
	return TRUE;
}

/**
 * 푸쉬버튼 마우스 L 버튼다운.
 */
BOOL CW3DUInterface::PushButton_LDown( UICComponent* pComp )
{
	UIButton*		pButton = (UIButton*)pComp;
	Assert( pButton );
	pButton->Push( TRUE );

	PushEvent( pComp->m_strName.c_str(), EVENT_BUTTON, 0, 1 );
	return TRUE;
} //CW3DUInterface::PushButton_LDown

/**
 * 푸쉬버튼 마우스 L 버튼 업.
 */
BOOL CW3DUInterface::PushButton_LUp( UICComponent* pComp )
{
	char			szBtnNameYes[64], szBtnNameNo[64], szBtnNameOk[64];

	sprintf( szBtnNameYes, "%s%s", m_szMegKey, "_YES" );
	sprintf( szBtnNameNo, "%s%s", m_szMegKey, "_NO" );
	sprintf( szBtnNameOk, "%s%s", m_szMegKey, "_OK" );

	UIButton*	pButton = static_cast<UIButton*>( pComp );
	Assert( pButton );
	if( !pButton->IsEnable() )
	{
		return TRUE;
	}
	pButton->Push( FALSE );

	//////////////////////////
	//	MessageBox Buttons  //
	//////////////////////////
	if( strcmp( pComp->m_strName.c_str(), szBtnNameYes ) == 0 )
	{
		ShowDlg( pComp, FALSE );
		SetCursorMode();
	}
	else if( strcmp( pComp->m_strName.c_str(), szBtnNameNo ) == 0 )
	{
		ShowDlg( pComp, FALSE );
		SetCursorMode();
	}
	else if( strcmp( pComp->m_strName.c_str(), szBtnNameOk ) == 0 )
	{
		ShowDlg( pComp, FALSE );
		SetCursorMode();
	} //if..else
	
	if( m_sModeState == "[GAME_MODE]" )
	{
		PushButton_LUp_GAME_MODE( pComp );
	}
	else
	{
		PushButton_LUp_CHARSELECT( pComp );
	} //if..else

	m_pSound->Play( "button.wav" );
	PushEvent( pComp->m_strName.c_str(), EVENT_BUTTON, 0, 0 );

	return TRUE;
} //CW3DUInterface::PushButton_LUp

/**
 * 푸쉬버튼 케릭터 선택 모드.
 */
BOOL CW3DUInterface::PushButton_LUp_CHARSELECT( UICComponent* pComp )
{
	//////////////////////////
	//	Login Buttons       //
	//////////////////////////
	if( pComp->m_strName == "BTN_LOGIN_YES" )
	{
		SetCursorMode();
	}
	else if( pComp->m_strName == "BTN_LOGIN_NO" )
	{
		SetCursorMode();
	}

	////////////////////////////////
	// Character Select Buttons	  //
	////////////////////////////////
	else if( pComp->m_strName == "BTN_CHARACTER1" )
	{
		m_CharSelInfo.SetIndex( 0 );
		SCharInfo*	pCharInfo = m_CharSelInfo.GetCharInfo();
		if( !pCharInfo->bEmpty )	
		{
			RefreshNewCharacter();
			SetCharInfo( pCharInfo );
			SetModeState( "[CHARINFO_MODE]" );
		} //if
	}
	else if( pComp->m_strName == "BTN_CHARACTER2")
	{
		m_CharSelInfo.SetIndex( 1 );
		SCharInfo*	pCharInfo = m_CharSelInfo.GetCharInfo();
		if( !pCharInfo->bEmpty )	
		{
			RefreshNewCharacter();
			SetCharInfo( pCharInfo );
			SetModeState( "[CHARINFO_MODE]" );
		} //if
	}
	else if( pComp->m_strName == "BTN_CHARACTER3" )
	{
		m_CharSelInfo.SetIndex( 2 );
		SCharInfo*	pCharInfo = m_CharSelInfo.GetCharInfo();
		if( !pCharInfo->bEmpty )	
		{
			RefreshNewCharacter();
			SetCharInfo( pCharInfo );
			SetModeState( "[CHARINFO_MODE]" );
		} //if
	}
	else if( pComp->m_strName == "BTN_CHARSEL_CANCEL" )
	{
		SetCharInfo( NULL );
	}

	////////////////////////////////
	// New Character Buttons      //
	////////////////////////////////
	else if( pComp->m_strName == "BTN_CHARNEW_STR_P" )
	{
		m_CharSelInfo.Plus_STR();
		RefreshNewCharacter();
	}
	else if( pComp->m_strName == "BTN_CHARNEW_STR_M" )
	{
		m_CharSelInfo.Minus_STR();
		RefreshNewCharacter();
	}
	else if( pComp->m_strName == "BTN_CHARNEW_DEX_P" )
	{
		m_CharSelInfo.Plus_DEX();
		RefreshNewCharacter();
	}
	else if( pComp->m_strName == "BTN_CHARNEW_DEX_M" )
	{
		m_CharSelInfo.Minus_DEX();
		RefreshNewCharacter();
	}
	else if( pComp->m_strName == "BTN_CHARNEW_INT_P" )
	{
		m_CharSelInfo.Plus_INT();
		RefreshNewCharacter();
	}
	else if( pComp->m_strName == "BTN_CHARNEW_INT_M" )
	{
		m_CharSelInfo.Minus_INT();
		RefreshNewCharacter();
	}

	else if( pComp->m_strName == "BTN_CHARNEW_SEX_L" )			m_CharSelInfo.Next_Sex();
	else if( pComp->m_strName == "BTN_CHARNEW_SEX_R" )			m_CharSelInfo.Next_Sex();
	else if( pComp->m_strName == "BTN_CHARNEW_HAIR_L" )			m_CharSelInfo.Previous_Hair();
	else if( pComp->m_strName == "BTN_CHARNEW_HAIR_R" )			m_CharSelInfo.Next_Hair();
	else if( pComp->m_strName == "BTN_CHARNEW_FACE_L" )			m_CharSelInfo.Previous_Face();
	else if( pComp->m_strName == "BTN_CHARNEW_FACE_R" )			m_CharSelInfo.Next_Face();
	else if( pComp->m_strName == "BTN_CHARNEW_COAT_L" )			m_CharSelInfo.Previous_Coat();
	else if( pComp->m_strName == "BTN_CHARNEW_COAT_R" )			m_CharSelInfo.Next_Coat();
	else if( pComp->m_strName == "BTN_CHARNEW_PANTS_L" )		m_CharSelInfo.Previous_Pants();
	else if( pComp->m_strName == "BTN_CHARNEW_PANTS_R" )		m_CharSelInfo.Next_Pants();
	else if( pComp->m_strName == "BTN_CHARNEW_OK" )
	{
		char*		szName = (char*)GetText( "EDT_CHARNEW_NAME" );
		if( VerifyCharacterSet( szName ) )
		{
			m_CharSelInfo.SetName( szName );

			SCharInfo*	pCharInfo = m_CharSelInfo.GetCharInfo();
			SetCharInfo( pCharInfo );

			int		nTotal = pCharInfo->nSTR + pCharInfo->nDEX + pCharInfo->nINT;
			if( nTotal < 35 )
			{
				MessageBox( STR_CHARSEL_LEFTPOINT );
				return FALSE;
			} //if
			m_CharSelInfo.Save();
				
			ClearText( "EDT_CHARNEW_NAME" );

			SetCharacterFace();
			PushEvent( "CREATECHAR", EVENT_CREATECHAR, 0, 0 );
			RefreshNewCharacter();
		} //if
	}
	else if( pComp->m_strName == "BTN_CHARNEW_CANCEL" )
	{
		ShowDlg( pComp, FALSE );
		m_CharSelInfo.InitPOINT();
		RefreshNewCharacter();
		SetCharInfo( NULL );
	}
	
	return TRUE;
} //CW3DUInterface::PushButton_LUp_CHARSELECT

/**
 * 푸쉬버튼 게임 모드.
 */
BOOL CW3DUInterface::PushButton_LUp_GAME_MODE( UICComponent* pComp )
{
	//////////////////////////
	//	LeftMenu Buttons   //
	//////////////////////////
	if( pComp->m_strName == "BTN_LB_SOCIETY" )
	{
		ShowDlg( "DLG_SOCIETY", TRUE );
	}

	//////////////////////////
	//	RightMenu Buttons   //
	//////////////////////////
	else if( pComp->m_strName == "BTN_RB_CHARACTER" )
	{
		ShowDlg( "DLG_CHARACTERINFO", !IsShowDlg( "DLG_CHARACTERINFO" ) );
	}
	else if( pComp->m_strName == "BTN_RB_INVENTORY" )
	{
		ShowMyInventoryDlg( !IsShowDlg( "DLG_INVENTORY" ) );
	}
	else if( pComp->m_strName == "BTN_RB_SKILL" )
	{
		ShowSkillDlg( !IsShowDlg( "DLG_SKILL" ) );
	}
	else if( pComp->m_strName == "BTN_RB_OPTION" )
	{
		ShowDlg( "DLG_OPTION", !IsShowDlg( "DLG_OPTION" ) );
	}
	else if( pComp->m_strName == "BTN_RB_COMMUNITY" )
	{
		ShowDlg( "DLG_COMMUNITY", !IsShowDlg( "DLG_COMMUNITY" ) );
	}

	//////////////////////////
	//	PartyBoard Buttons	//
	//////////////////////////

	else if( pComp->m_strName == "BTN_PARTYBOARD_X" )
	{
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_PARTYBOARD_PREV" )
	{
	}
	else if( pComp->m_strName == "BTN_PARTYBOARD_NEXT" )
	{
	}
	else if( pComp->m_strName == "BTN_PARTYBOARD_REGIST" )
	{
		PushEvent(  "PARTYGETHER", EVENT_PARTY_BOARDREGIST, 0, 0 );
	}

	//////////////////////////
	//	Guild Buttons	    //
	//////////////////////////

	else if( pComp->m_strName == "BTN_MSG_GUILDINVITE_YES" )
	{
		m_nInviteGuildMasterId;
		DWORD		dwParam2 = ( m_wInviteGuildNumber << 16 ) | 1;
		PushEvent(  "GUILD", EVENT_GUILD_AID, m_nInviteGuildMasterId, dwParam2 );
	}
	else if( pComp->m_strName == "BTN_WARGUILDINFO_X" ||
			 pComp->m_strName == "BTN_CASTLELIST_X" )
	{
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_MSG_GUILDINVITE_NO" )
	{
		m_nInviteGuildMasterId;
		DWORD		dwParam2 = ( m_wInviteGuildNumber << 16 ) | 0;
		PushEvent(  "GUILD", EVENT_GUILD_AID, m_nInviteGuildMasterId, dwParam2 );
	}
	else if( pComp->m_strName == "BTN_CASTLELIST_MAP1" )
	{
		PushEvent(  "GUILD", EVENT_GUILD_WARINFO, m_anCastleNum[0], 0 );
	}
	else if( pComp->m_strName == "BTN_CASTLELIST_MAP2" )
	{
		PushEvent(  "GUILD", EVENT_GUILD_WARINFO, m_anCastleNum[1], 0 );
	}
	else if( pComp->m_strName == "BTN_CASTLELIST_MAP3" )
	{
		PushEvent(  "GUILD", EVENT_GUILD_WARINFO, m_anCastleNum[2], 0 );
	}
	else if( pComp->m_strName == "BTN_CASTLELIST_MAP4" )
	{
		PushEvent(  "GUILD", EVENT_GUILD_WARINFO, m_anCastleNum[3], 0 );
	}
	else if( pComp->m_strName == "BTN_CASTLELIST_MAP5" )
	{
		PushEvent(  "GUILD", EVENT_GUILD_WARINFO, m_anCastleNum[4], 0 );
	}
	
	//////////////////////////
	//	Quest Buttons	    //
	//////////////////////////
	else if( pComp->m_strName == "BTN_QUEST_X" )
	{
		ShowDlg( pComp, FALSE );
	}

	///////////////////////////////////
	//		You And Me Buttons		 //
	///////////////////////////////////
	else if( pComp->m_strName == "BTN_YOUME_X" )
	{
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_YOUME_1" )
	{ //거래, 대화
		switch( m_btYou )
		{
		case YOU_CHARACTER:		
			PushEvent( const_cast<char*>( "YOUANDME" ), EVENT_DEAL_REQUEST, m_nYouId, 0 );		
			ShowDlg( pComp, FALSE );
			break;
		
		case YOU_NPC:
			PushEvent( const_cast<char*>( "YOUANDME" ), EVENT_TALK_REQUEST, m_nYouId, 0 );		
			break;
		}
	}
	else if( pComp->m_strName == "BTN_YOUME_2" )
	{ //파티, 샾
		switch( m_btYou )
		{
		case YOU_CHARACTER:		
			PushEvent( const_cast<char*>( "YOUANDME" ), EVENT_PARTY_REQUEST, m_nYouId, 0 );		
			ShowDlg( pComp, FALSE );
			break;

		case YOU_NPC:
			UIButton*	pButton = reinterpret_cast<UIButton*>( pComp );
			Assert( pButton );
			Assert( pButton->m_nType == UI_PUSHBUTTON );

			if( !strcmp( pButton->GetText(), STR_UIMSG_CREATEGUILD ) )
			{
				ShowDlg( pComp, FALSE );
				UICComponent*	pComp = GetComponent( "EDT_CHATINPUT" );
				Assert( pComp );
				EditBox_LDown( pComp );

				m_btInputSystem = TRUE;
				ChattingInput( "", STR_UIMSG_INPUTGUILDNAME, FONT_MSGCOLOR, 0, 0 );
			}
			else
			{
				if( !IsShowDlg( "DLG_SHOPINVENTORY" ) )
				{
					PushEvent( "YOUANDME", EVENT_SHOP_REQUEST, m_nYouId, m_wShop );
				} //if
			} //if..else
			break;
		}
	}
	else if( pComp->m_strName == "BTN_YOUME_3" )
	{ //퀘스트 서버통합
		UIButton*	pButton = reinterpret_cast<UIButton*>( pComp );
		Assert( pButton );
		Assert( pButton->m_nType == UI_PUSHBUTTON );

		if( !strcmp( pButton->GetText(), STR_UIMSG_RELEASEGUILD ) )
		{
			MessageBox( STR_UIASK_RELEASEGUILD, "RELEASEGUILD", MSG_YESNO );
		}
		else
		{
			if( !IsShowDlg( "DLG_QUESTLIST" ) )
			{
				PushEvent( const_cast<char*>( "YOUANDME" ), EVENT_QUEST_REQUEST, m_nYouId, m_wQuest );
			} //if
		} //if..else
	}
	else if( pComp->m_strName == "BTN_YOUME_4" )
	{ //창고
		UIButton*	pButton = reinterpret_cast<UIButton*>( pComp );
		Assert( pButton );
		Assert( pButton->m_nType == UI_PUSHBUTTON );
		
		//여기서 NPC번호가 0003이 아니면 창고로 표시
		if( !strcmp( pButton->GetText(), STR_UIMSG_ENCHANT )  )
		{
			PushEvent( const_cast<char*>( "YOUANDME" ), EVENT_SMITH_REQUEST, m_nYouId, 0 );
		}
		else if( !strcmp( pButton->GetText(), STR_UIMSG_CARDSUM ) )
		{
			PushEvent( const_cast<char*>( "YOUANDME" ), EVENT_CARDSUM, m_nYouId, 0 );
		}
		else if( !strcmp( pButton->GetText(), STR_UIMSG_CARDUNDO ) )
		{
			PushEvent( const_cast<char*>( "YOUANDME" ), EVENT_CARDUNDONPC, m_nYouId, 0 );
		}
		else
		{
			if( !IsShowDlg( "DLG_STORAGE" ) )
			{
				PushEvent( const_cast<char*>( "YOUANDME" ), EVENT_STORAGE_REQUEST, 0, 0 );
			} //if
		}
	}
	else if( pComp->m_strName == "BTN_YOUME_5" )
	{ //상점 맵으로...
		ShowDlg( pComp, FALSE );
		PushEvent( const_cast<char*>( "YOUANDME" ), EVENT_GOTODEALMAP, 0, 0 );
	}	
	else if( pComp->m_strName == "BTN_MSG_CREATEGUILD_YES" )
	{
		PushEvent( "YOUANDME", EVENT_GUILD_CREATE, 0, 0 );
	}
	else if( pComp->m_strName == "BTN_MSG_RELEASEGUILD_YES" )
	{
		PushEvent( "YOUANDME", EVENT_GUILD_RELEASE, 0, 0 );
	}
	
	//////////////////////////
	//	CharInfo Buttons   //
	//////////////////////////
	else if( pComp->m_strName == "BTN_CINFO_X" )			ShowDlg( pComp, FALSE );

	//////////////////////////
	//	Inventory Buttons   //
	//////////////////////////
	else if( pComp->m_strName == "BTN_INVEN_X" )			ShowMyInventoryDlg( FALSE );
	else if( pComp->m_strName == "BTN_INVEN_GOLD" )			InputPrice( "BTN_INVEN_GOLD" );

	//////////////////////////
	//	Gold Buttons		//
	//////////////////////////
	else if( pComp->m_strName == "BTN_GOLD_OK" )
	{
		InputPrice();	
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_GOLD_CANCEL" )		ShowDlg( pComp, FALSE );
	
	//////////////////////////
	//	Skill Buttons	    //
	//////////////////////////
	else if( pComp->m_strName == "BTN_SKILL_X" )			ShowDlg( pComp, FALSE );

	//////////////////////////
	//	Shop Buttons        //
	//////////////////////////
	else if( pComp->m_strName == "BTN_SHOP_X" )				ShowShop( FALSE );

	//////////////////////////
	//	Storage Buttons     //
	//////////////////////////
	else if( pComp->m_strName == "BTN_STORAGE_X" )			ShowStorage( FALSE );
	else if( pComp->m_strName == "BTN_STORAGE_GOLD" )		InputPrice( "BTN_STORAGE_GOLD" );
	
	//////////////////////////
	//	Buy Posion Buttons  //
	//////////////////////////
	else if( pComp->m_strName == "BTN_POSIONBUY_PLUS" || pComp->m_strName == "BTN_CARDBUY_PLUS" )
	{
		m_nNumPosion++;
		RefreshBuyPosion();
	}
	else if( pComp->m_strName == "BTN_POSIONBUY_MINUS" || pComp->m_strName == "BTN_CARDBUY_MINUS" )
	{
		m_nNumPosion--;
		RefreshBuyPosion();
	}
	else if( pComp->m_strName == "BTN_POSIONBUY_OK" || pComp->m_strName == "BTN_CARDBUY_OK" )
	{
		ShowDlg( pComp, FALSE );
		
		//여기서 인벤토리 무게를 체크
		if( m_bCheckWeight == TRUE )
		{
			RefreshBuyPosion();
			if( m_bNumPosion == TRUE && m_bCheckSild == TRUE )
			{
				if( m_nNumPosion >= 1 )
				{
					DWORD		dwParam1 = ( m_pBuyItemData->nId << 16 ) | m_wShop;
					PushEvent(
						const_cast<char*>( "SHOP" ),
						EVENT_ITEM_BUY,
						dwParam1,
						m_nNumPosion );
				} //if
			}	
			else
			{
				ShowDlg( "DLG_BUYCARD", FALSE );
			}
			
		}
		else
		{
			ChattingInput( "", STR_CARDSUM_ERRORMSG, 0xFFFFFF00, NULL, NULL );
		}
		m_nNumPosion = 0;
	}
	else if( pComp->m_strName == "BTN_POSIONBUY_CANCEL" || pComp->m_strName == "BTN_CARDBUY_CANCEL" )
	{
		ShowDlg( pComp, FALSE );

		m_nNumPosion = 0;
		RefreshBuyPosion();
	}

	//////////////////////////
	//	Messenger Buttons   //
	//////////////////////////
	else if( pComp->m_strName == "BTN_MESSENGER_X" )
	{
		ShowDlg( pComp, FALSE );
	}

	//////////////////////////
	//	Option Box Buttons  //
	//////////////////////////
/*	else if( pComp->m_strName == "BTN_OPTION_SOUND" )	
	{
		UIButton*	pButton = (UIButton*)GetComponent( "BTN_OPTION_SOUND" );
		Assert( pButton );

		if( !strcmp( pButton->GetText(), "배경음OFF" ) )
		{
			pButton->SetText( "배경음ON" );
			PushEvent( "SOUND",	EVENT_OPTION_BGMONOFF,	0, 0 );
		}
		else if( !strcmp( pButton->GetText(), "배경음ON" ) )
		{
			pButton->SetText( "배경음OFF" );
			PushEvent( "SOUND",	EVENT_OPTION_BGMONOFF,	0, 1 );
		} //if..else
	}
*/	else if( pComp->m_strName == "BTN_OPTION_HELP" )
	{
		GameHelp();
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_OPTION_CHATOFF" )
	{
		ShowDlg( "DLG_CHATCUTOFF", TRUE );
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_OPTION_SOUND" )
	{
		PushEvent( "SOUND",	EVENT_OPTION_SOUND,	0, 0 );
		ShowDlg( "DLG_SOUND", TRUE );
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_OPTION_X" )
	{
		ShowDlg( pComp, FALSE );
		SetCursorMode();
	}
	else if( pComp->m_strName == "BTN_OPTION_EXIT" )
	{
		ShowDlg( pComp, FALSE );
		ShowDlg( "DLG_EXIT", TRUE );
	}

	//////////////////////////
	//	Sound Buttons       //
	//////////////////////////

	else if( pComp->m_strName == "BTN_SOUND_X" )
	{
		ShowDlg( pComp, FALSE );
	}

	//////////////////////////
	//	Map Buttons         //
	//////////////////////////
	else if( pComp->m_strName == "BTN_MAP_SHOW" )
	{
		UICComponent*		pMiniMap = GetComponent( "DLG_MINIMAP" );
		if( !pMiniMap )
		{
			Assert( !"없다" );
			return TRUE;
		} //if
		ShowDlg( pMiniMap, !pMiniMap->IsShow() );
	}
	else if( pComp->m_strName == "BTN_MAP_BIG" )
	{
		m_pLargeMap = reinterpret_cast<UIMiniMap*>( GetComponent( "MAP_LARGE" ) );
		if( !m_pLargeMap )
		{
			Assert( m_pLargeMap );
			return TRUE;
		} //if

		if( m_pLargeMap->IsShow() )
		{
			ShowDlg( m_pLargeMap, FALSE );
		}
		else
		{
			WORD		wMap = m_pMiniMap->GetMapIndex();
			char		szMapName[64];
			sprintf( szMapName, "landb%04d", wMap );
			m_pLargeMap->LoadMap( szMapName, 500, 500 );

			ShowDlg( m_pLargeMap, TRUE );
		} //if..else
	}

	//////////////////////////
	//	Exit Buttons        //
	//////////////////////////
	else if( pComp->m_strName == "BTN_EXIT_X" )
	{
		ShowDlg( pComp, FALSE );
		SetCursorMode();
	}

	//////////////////////////
	//	NPC TalkBox Buttons //
	//////////////////////////
	else if( pComp->m_strName == "BTN_SAY_OK" || pComp->m_strName == "BTN_QUEST_SAY_NEXT" )
	{
		if( m_bNPCTalkMode == 0 )			NextNPC( m_pNPCScript, 0 );
#ifdef _dUMENOKOJI_QUEST_
		else if( m_bNPCTalkMode == 1 )		NextQuestNPC( m_pQuestScript, 0 );
#else
		else if( m_bNPCTalkMode == 1 )		NextNPC( m_pQuestScript, 0 );
#endif
		else if( m_bNPCTalkMode == 2 )		NextNPC( m_pNPCScript, 0 );
	}

#ifdef _dUMENOKOJI_QUEST_
	else if( pComp->m_strName == "BTN_QUEST_SAY_PRE" )
	{
		if( m_bNPCTalkMode )	
		{
			PreQuestNPC( m_pQuestScript );
		}
	}
#endif

	else if( pComp->m_strName == "BTN_SAY_END" || pComp->m_strName == "BTN_QUEST_SAY_END" )
	{
		ShowDlg( pComp, FALSE );
	}

#ifdef _dUMENOKOJI_QUEST_
	////////////////////////////////
	//	 QUEST BealBox Buttons	  //
	////////////////////////////////
	else if( pComp->m_strName == "BTN_QUEST_YES" )
	{
		ShowDlg( pComp, FALSE );
	}
	
	else if( pComp->m_strName == "BTN_QUEST_NO" )
	{
		ShowDlg( pComp, FALSE );
	}

	else if( pComp->m_strName == "BTN_MYQUEST_X" )
	{
		ShowDlg( pComp, FALSE );
	}
#endif

	//////////////////////////
	//	SMITH SYSTEM		//
	//////////////////////////
	else if( pComp->m_strName == "BTN_SMITH_SAY_X" )
	{
		ShowDlg( pComp, FALSE ); 
	}
	else if( pComp->m_strName == "BTN_SMITH_CANCEL" )
	{
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_SMITH_ENCHANT" )
	{
		ShowDlg( pComp, FALSE );
		ShowDlg( "DLG_SMITH_ENCHENT_CARD_SUM", TRUE );
	}
	else if( pComp->m_strName == "BTN_SMITH_INTEGRATION" )
	{
		//이거구나
		ShowDlg( pComp, FALSE );
		ShowSmithEnchentBox( TRUE );
	}
	//통합
	else if( pComp->m_strName == "BTN_WP_INTEGRATION_X" || pComp->m_strName == "BTN_AM_INTEGRATION_X" )
	{
		//ShowDlg( pComp, FALSE );
		ShowSmithEnchentBox( FALSE );
		ShowMyInventoryDlg( FALSE );
		m_EnchantPricemoney = 0;
	}

	else if( pComp->m_strName == "BTN_WP_INTEGRATION_NO" || pComp->m_strName == "BTN_AM_INTEGRATION_NO"  )
	{
		ShowSmithEnchentBox( FALSE );
		ShowMyInventoryDlg( FALSE );
		m_EnchantPricemoney = 0;
		if( !m_EnchantSmithItem || !m_EnchantSmithCard.GetCardItem() )
		{
			m_EnchantSmithItem = NULL;
			m_EnchantSmithCard.Cleanup();
		}
	}

	else if( pComp->m_strName == "BTN_WP_INTEGRATION_OK" || pComp->m_strName == "BTN_AM_INTEGRATION_OK" )
	{
		UseSmithEnchant();
	}

	else if( pComp->m_strName == "BTN_CARD_SAY_X" || pComp->m_strName == "BTN_CARD_CANCEL" ||
			 pComp->m_strName == "BTN_CARDUNDO_SAY_X" || pComp->m_strName == "BTN_CARDUNDO_CANCEL" )
	{
 		ShowDlg( pComp, FALSE ); 
	}

	else if( pComp->m_strName == "BTN_CARD_WP" )
	{
		SItemData*	pItemDat = NULL;
		BuyCardInShop( pItemDat, 12097 );
	}

	else if(  pComp->m_strName == "BTN_CARD_AM" )
	{
		SItemData*	pItemDat = NULL;
		BuyCardInShop( pItemDat, 12098 );
	}

	else if( pComp->m_strName == "BTN_CARDUNDO_WP" || pComp->m_strName == "BTN_CARDUNDO_AM" )
	{
		int	nCardNum = 0;
		if( pComp->m_strName == "BTN_CARDUNDO_WP" )
		{
			nCardNum = 12097;
		}
		else
		{
			nCardNum = 12098;
		}
		SItemData*	pItemDat = GetItemDataInInventory( nCardNum );// NULL;//GetItemDataInInventory( 12097 );
		if( !pItemDat )
		{
			ChattingInput( "", STR_UIMSG_CARDUNDO_ERR, FONT_MSGCOLOR, 0, 0 );
		}
		else
		{
			PushEvent( "ENCHANT_CARD_UNDO", EVENT_CARDUNDO, pItemDat->nId, 0 );
		}
	}

	////////////////////////////////
	//	Buy DealBox Buttons		  //
	////////////////////////////////
	else if( pComp->m_strName == "BTN_DEALBUY_YES" )
	{
		ShowDlg( pComp, FALSE );
		BuyItem();
	}
	else if( pComp->m_strName == "BTN_DEALBUY_NO" )
	{
		ShowDlg( pComp, FALSE );
	}

	////////////////////////////////
	//	Sell DealBox Buttons	  //
	////////////////////////////////
	else if( pComp->m_strName == "BTN_DEALSELL_YES" )
	{
		ShowDlg( pComp, FALSE );

		DWORD		dwParam1 = ( m_nSellItem << 16 ) | m_wShop;
		DWORD		dwParam2 = 0;
		PushEvent(
			const_cast<char*>( "SHOP" ),
			EVENT_ITEM_SELL,
			dwParam1,
			dwParam2 );
	}
	else if( pComp->m_strName == "BTN_DEALSELL_NO" )
	{
		ShowDlg( pComp, FALSE );
	}

	////////////////////////////////
	//	User Deal Buttons		  //
	////////////////////////////////
	else if( pComp->m_strName == "BTN_DEAL_X" )
	{
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_DEAL_OK" )
	{
		UIInventoryIN*		pDealInven;
		int					nIndex = 1;
		int					nItemNum = 0;
		while( pDealInven = GetInventoryIN( "INVENTORY_DEALYOU_", nIndex ) )
		{
			nItemNum = pDealInven->GetItemNum();
			nIndex++;
		} //while

		int		nBlankNum = InventoryBlank();
		if( nBlankNum < nItemNum )
		{
			MessageBox( STR_INVENERR_NOTENOUGHSPACE );
			return TRUE;
		} //if
		PushEvent( pComp->m_strName.c_str(), EVENT_DEAL_CHECKIN, 0, 0 );
	}
	else if( pComp->m_strName == "BTN_DEAL_CANCEL" )
	{
		PushEvent( pComp->m_strName.c_str(), EVENT_DEAL_CANCEL, 0, 0 );
	}

	////////////////////////////////
	//	Quick Buttons			  //
	////////////////////////////////
	else if( pComp->m_strName == "BTN_QUICK_UP" )
	{
		if( m_btQuickBarNum == NUM_QUICKSLOTBAR-1 )		m_btQuickBarNum = 0;
		else											m_btQuickBarNum++;
		ShowQuickBar();
	}
	else if( pComp->m_strName == "BTN_QUICK_DN" )
	{
		if( m_btQuickBarNum == 0 )						m_btQuickBarNum = NUM_QUICKSLOTBAR-1;
		else											m_btQuickBarNum--;
		ShowQuickBar();
	}

	////////////////////////////////
	//	Death Buttons			  //
	////////////////////////////////
	else if( pComp->m_strName == "BTN_DEATH_TOWN" )
	{
		PushEvent( "Die", EVENT_DIE, 0, 0 );
		ShowDlg( pComp, FALSE );
	} 
/*	else if( pComp->m_strName == "BTN_DEATH_GHOST" ) // 유령모드
	{
		PushEvent( "Die", EVENT_DIE, 1, 0 );
		ShowDlg( pComp, FALSE );
	}*/
	else if( pComp->m_strName == "BTN_DEATH_DEAD" )
	{
		PushEvent( "Die", EVENT_DIE, 2, 0 );
		ShowDlg( pComp, FALSE );
	}

	//////////////////////////
	//	Chatting		    //
	//////////////////////////

	else if( pComp->m_strName == "BTN_CHAT_LISTONOFF" )
	{
		ChangeChatListSize();
	}
	else if( pComp->m_strName == "BTN_CHATOFF_X" )
	{
		ShowDlg( pComp, FALSE );
	}

	//////////////////////////
	//	Friend			    //
	//////////////////////////

	else if( pComp->m_strName == "BTN_CHATID_X" )
	{
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_CHATID_TALK" )
	{
		RequestTalkFriend();
	}
	else if( pComp->m_strName == "BTN_CHATID_DELETE" )
	{
		SkillDeleteFriend();
	}

	//////////////////////////
	//	Drop Item		    //
	//////////////////////////

	else if( pComp->m_strName == "BTN_MSG_DROPITEM_YES" )
	{
		if( m_pConfirmItemData )
		{
			PushEvent( NULL, EVENT_ITEM_DROP, m_pConfirmItemData->nId, m_btFrom-1 );
			m_pConfirmItemData = NULL;
		} //if
	} 
	
	//////////////////////////
	//	Confirm Enchant	    //
	//////////////////////////
	
	else if( pComp->m_strName == "BTN_MSG_ENCHANTCONFIRM_YES" )
	{
		DWORD		dwCardId, dwItemId;
		if( m_EnchantCard.GetEnchantInfo( dwCardId, dwItemId ) )
		{
			PushEvent( "ENCHANT", EVENT_ENCHANT, dwItemId, dwCardId );	
		}
		else
		{
			Assert( !"인첸트 정보가 없다" ); 
		} //if..else
	}

	else if( pComp->m_strName == "BTN_MSG_CONFIRM_ITEMDEAL_YES" )
	{
		RequestInsertItemInUserDeal( m_pConfirmItemData );
	}
	else if( pComp->m_strName == "BTN_MSG_CONFIRM_ITEMSHOP_YES" )
	{
		RequestSellItem( m_pConfirmItemData );
	}
	else if( pComp->m_strName == "BTN_MSG_CONFIRM_ITEMMYSHOP_YES" )
	{
		RequestInsertItemInMyShop( m_pConfirmItemData );
	}
	
	//////////////////////////
	//	China Event		    //
	//////////////////////////

	else if( pComp->m_strName == "BTN_EVENTMSG_OK" )
	{
		ShowDlg( pComp, FALSE );
	}

	//////////////////////////
	//	Pet				    //
	//////////////////////////

	else if( pComp->m_strName == "BTN_PET_X" )
	{
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_PET_USE" )
	{
		PushEvent( "Pet", EVENT_PET_ONOFF, 0, 0 );
	}
	else if( pComp->m_strName == "BTN_PET_NAMECHANGE" )
	{
		PushEvent( "Pet", EVENT_PET_CHANGENAME, 0, 0 );
	}

	//////////////////////////
	//	Enchant Card	    //
	//////////////////////////

	else if( pComp->m_strName == "BTN_ECARD_1" )
	{
		m_EnchantCard.ClickCard( 0 );
	}
	else if( pComp->m_strName == "BTN_ECARD_2" )
	{
		m_EnchantCard.ClickCard( 1 );
	}

	//스미스 인첸트
	

	//////////////////////////
	//	Player List 	    //
	//////////////////////////

	else if( pComp->m_strName == "BTN_PLAYER_X" )
	{
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_PLAYER_PARTY" )
	{
		PartyInvitByPlayerList();
	}
	else if( pComp->m_strName == "BTN_PLAYER_WHISPER" )
	{
		char*		szPlayer = (char*)GetText( "SSTR_PLAYER_LIST" );
		if( !szPlayer )
		{
			Assert( !"Unselected" );
			return FALSE;
		} //if
		SetWhisperName( szPlayer );
	}

	//////////////////////////
	//	Personal Shop 	    //
	//////////////////////////

	else if( pComp->m_strName == "BTN_PSHOP_X" )
	{
		ShowDlg( pComp, FALSE );
		UIInventoryIN*		pInven = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVENTORY_PERSONALSHOP" ) );
		if( pInven )
		{
			pInven->Cleanup();
			m_ItemMgr.ClearItemInTemp();
		} //if
	} 
	else if( pComp->m_strName == "BTN_MYSHOP_TITLE" )
	{
		char*		szText = const_cast<char*>( GetText( "EDT_MYSHOP_TITLE" ) );
		if( szText && CheckWord( szText ) )
		{
			PushEvent( "MYSHOP", EVENT_MYSHOP_CHANGETITLE, 0, 0 );
		}
		else
		{
			ChattingInput( "", STR_SYSMSG_EXCEPTSTRING, FONT_MSGCOLOR, 0, 0 );
		} //if..else
	}
	else if( pComp->m_strName == "BTN_MYSHOP_BEGIN" )
	{
		if( !strcmp( GetText( "BTN_MYSHOP_BEGIN" ), STR_MYSHOP_OPEN ) )
		{
			PushEvent( "MYSHOP", EVENT_MYSHOP_SELLBEGIN, 0, 0 );
		}
		else
		{
			PushEvent( "MYSHOP", EVENT_MYSHOP_SELLFINISH, 0, 0 );
		} //if..else
	}
	else if( pComp->m_strName == "BTN_MYSHOP_CALLBACK" )
	{
		PushEvent( "MYSHOP", EVENT_MYSHOP_CHANGETITLE, 0, 0 );
	} 

	//////////////////////////
	//	Personal Shop 	    //
	//////////////////////////

	else if( pComp->m_strName == "BTN_GUILDMEMBERLIST_X" )
	{
		ShowDlg( pComp,FALSE );
	}
	else if( pComp->m_strName == "BTN_GUILDMEMBERLIST_WHISPER" )
	{
		char*		szPlayer = (char*)GetText( "SSTR_GUILDMEMBERLIST_LIST" );
		if( !szPlayer )
		{
			Assert( !"Unselected" );
			return FALSE;
		} //if
		SetWhisperName( szPlayer );
	}

	//////////////////////////
	//	Cash Shop 			//
	//////////////////////////

	else if( pComp->m_strName == "BTN_CASH_X" )
	{
		ShowDlg( pComp,FALSE );
	}

	//////////////////////////
	//	Tex				    //
	//////////////////////////

	else if( pComp->m_strName == "BTN_TEX_X" )
	{
		ShowDlg( pComp, FALSE );
	}
	else if( pComp->m_strName == "BTN_TEX_UP" )
	{
		UIStaticText*	pStaticText = reinterpret_cast<UIStaticText*>( GetComponent( "TXT_TEX_NEXT" ) );
		if( pStaticText )
		{
			char*	pText = pStaticText->GetText();
			char	szNextTex[8];
			if( sscanf( pText, "%s", szNextTex ) != 1 )
			{
				Assert( !"sscanf 오류" );
			}

			int		nNextTex = atoi( szNextTex ) + 1;
			if( nNextTex > 20 )
			{
				nNextTex = 20;
			} //if

			sprintf( szNextTex, "%d %%", nNextTex );
			pStaticText->SetText( szNextTex );
		} //if
	}
	else if( pComp->m_strName == "BTN_TEX_DN" )
	{
		UIStaticText*	pStaticText = reinterpret_cast<UIStaticText*>( GetComponent( "TXT_TEX_NEXT" ) );
		if( pStaticText )
		{
			char*	pText = pStaticText->GetText();
			char	szNextTex[8];
			if( sscanf( pText, "%s", szNextTex ) != 1 )
			{
				Assert( !"sscanf 오류" );
			}

			int		nNextTex = atoi( szNextTex ) - 1;
			if( nNextTex < 5 )
			{
				nNextTex = 5;
			} //if
			sprintf( szNextTex, "%d %%", nNextTex );
			pStaticText->SetText( szNextTex );
		} //if
	}
	else if( pComp->m_strName == "BTN_TEX_OK" )
	{
		UIStaticText*	pStaticText = reinterpret_cast<UIStaticText*>( GetComponent( "TXT_TEX_NEXT" ) );
		if( pStaticText )
		{
			char*	pText = pStaticText->GetText();
			char	szNextTex[8];
			if( sscanf( pText, "%s", szNextTex ) != 1 )
			{
				Assert( !"sscanf 오류" );
			}

			int		nNextTex = atoi( szNextTex );

			if( nNextTex >= 5 && nNextTex <= 20 )
			{
				PushEvent( "TEX", EVENT_TEX_CHANGE, nNextTex, 0 );
			}
			else
			{
				MessageBox( "세율조정 범위가 잘 못 되었습니다" );
			} //if..else
		} //if
	}
	else if( pComp->m_strName == "BTN_TEX_CANCEL" )
	{
		ShowDlg( pComp, FALSE );
	} //if..else..

	//////////////////////////
	//	FRIEND ADD			//
	//////////////////////////
	if( pComp->m_strName == "BTN_FRIENDADD_X" )
	{
		ShowDlg( pComp, FALSE );
	}

	return TRUE;
} //PushButton_LUp_GAME_MODE

/**
 * 체크버튼 마우스 업.
 */
BOOL CW3DUInterface::CheckButton_LUp( UICComponent* pComp )
{
	UICheckButton*	pCheckBtn = static_cast<UICheckButton*>( pComp );
	Assert( pCheckBtn );

	if( !pCheckBtn->IsEnable() )
	{
		return TRUE;
	}
	
	//////////////////////////
	//	Shop			    //
	//////////////////////////

	if( pComp->m_strName == "CHK_SHOP_TAB1" )			CheckGroup( "CHK_SHOP_TAB", "SHOPINVENTORY_", 1 );
	else if( pComp->m_strName == "CHK_SHOP_TAB2" )		CheckGroup( "CHK_SHOP_TAB", "SHOPINVENTORY_", 2 );
	else if( pComp->m_strName == "CHK_SHOP_TAB3" )		CheckGroup( "CHK_SHOP_TAB", "SHOPINVENTORY_", 3 );

	else if( pComp->m_strName == "CHK_CHARNEWVIEW_ZOOM" )
	{
		UICheckButton*	pCheckBtn = reinterpret_cast<UICheckButton*>( pComp );
		if( pCheckBtn->IsCheck() )	
		{
			pCheckBtn->Check( FALSE );
			PushEvent( pComp->m_strName.c_str(), EVENT_BUTTON, 0, 0 );
		}
		else
		{
			pCheckBtn->Check( TRUE );
			PushEvent( pComp->m_strName.c_str(), EVENT_BUTTON, 0, 1 );
		} 
	}
	
	//////////////////////////
	//	Storage			    //
	//////////////////////////

	else if( pComp->m_strName == "CHK_STORAGE_TAB1" )	CheckGroup( "CHK_STORAGE_TAB", "INVEN_STORAGE_", 1 );
	else if( pComp->m_strName == "CHK_STORAGE_TAB2" )	CheckGroup( "CHK_STORAGE_TAB", "INVEN_STORAGE_", 2 );

	//////////////////////////
	// 통합서버 창고        //
	//////////////////////////
	else if( pComp->m_strName == "CHK_SERVER_STORAGE_TAB1" ) CheckGroup( "CHK_SERVER_STORAGE_TAB", "INVEN_SERVER_STORAGE_", 1  );
	else if( pComp->m_strName == "CHK_SERVER_STORAGE_TAB2" ) CheckGroup( "CHK_SERVER_STORAGE_TAB", "INVEN_SERVER_STORAGE_", 2  );

	//////////////////////////
	//	Inventory		    //
	//////////////////////////
	else if( pComp->m_strName == "CHK_INVEN_TAB1" )		CheckGroup( "CHK_INVEN_TAB", "INVENTORY_IN_", 1 );
	else if( pComp->m_strName == "CHK_INVEN_TAB2" )		CheckGroup( "CHK_INVEN_TAB", "INVENTORY_IN_", 2 );

	//////////////////////////
	// 통합서버 창고        //
	//////////////////////////
	else if( pComp->m_strName == "CHK_SERVER_INVEN_TAB1" ) CheckGroup( "CHK_SERVER_INVEN_TAB", "INVENTORY_SERVER_IN_", 1 );
	else if( pComp->m_strName == "CHK_SERVER_INVEN_TAB2" ) CheckGroup( "CHK_SERVER_INVEN_TAB", "INVENTORY_SERVER_IN_", 2 ); 

	//////////////////////////
	//	Deal Inventory		//
	//////////////////////////
	
	else if( pComp->m_strName == "CHK_DEALME_TAB1" )	CheckGroup( "CHK_DEALME_TAB", "INVENTORY_DEALME_", 1 );
	else if( pComp->m_strName == "CHK_DEALME_TAB2" )	CheckGroup( "CHK_DEALME_TAB", "INVENTORY_DEALME_", 2 );
	else if( pComp->m_strName == "CHK_DEALYOU_TAB1" )	CheckGroup( "CHK_DEALYOU_TAB", "INVENTORY_DEALYOU_", 1 );
	else if( pComp->m_strName == "CHK_DEALYOU_TAB2" )	CheckGroup( "CHK_DEALYOU_TAB", "INVENTORY_DEALYOU_", 2 );

	//////////////////////////
	//	Cash Inventory		//
	//////////////////////////
	
	else if( pComp->m_strName == "CHK_CASH_TAB1" )		CheckGroup( "CHK_CASH_TAB", "CASHITEM_IN_", 1 );
	else if( pComp->m_strName == "CHK_CASH_TAB2" )		CheckGroup( "CHK_CASH_TAB", "CASHITEM_IN_", 2 );

	//////////////////////////
	//	Skill 1			    //
	//////////////////////////

	else if( pComp->m_strName == "CHK_SKILL1" )				Skill1CheckButton( 1 );
	else if( pComp->m_strName == "CHK_SKILL2" )				Skill1CheckButton( 2 );
	else if( pComp->m_strName == "CHK_SKILL3" )				Skill1CheckButton( 3 );
	else if( pComp->m_strName == "CHK_SKILL4" )				Skill1CheckButton( 4 );

	//////////////////////////
	//	Skill 2			    //
	//////////////////////////

	else if( pComp->m_strName == "CHK_SKILL_ATAB1" )		Skill2CheckButton( 1 );
	else if( pComp->m_strName == "CHK_SKILL_ATAB2" )		Skill2CheckButton( 2 );
	else if( pComp->m_strName == "CHK_SKILL_ATAB3" )		Skill2CheckButton( 3 );
	else if( pComp->m_strName == "CHK_SKILL_ATAB4" )		Skill2CheckButton( 4 );

	//////////////////////////
	//	Chatting		    //
	//////////////////////////

	else if( pComp->m_strName == "CHK_CHAT_TAB1" )			ChatCheckButton( 1 );
	else if( pComp->m_strName == "CHK_CHAT_TAB2" )			ChatCheckButton( 2 );
	else if( pComp->m_strName == "CHK_CHAT_TAB3" )			ChatCheckButton( 3 );
	else if( pComp->m_strName == "CHK_CHAT_TAB4" )			ChatCheckButton( 4 );
	else if( pComp->m_strName == "CHK_CHAT_TAB5" )			ChatCheckButton( 5 );
	else if( pComp->m_strName == "CHK_CHAT_TAB6" )			ChatCheckButton( 6 );

	else if( pComp->m_strName == "CHK_CHAT_IDONOFF" )
	{
		pCheckBtn->Check( !pCheckBtn->IsCheck() );
		ShowDlg( "DLG_CHATIDLIST", pCheckBtn->IsCheck() );
	}

	//////////////////////////
	//	Chatting		    //
	//////////////////////////

	else if( pComp->m_strName == "CHK_SOUND_BGM" )
	{
		pCheckBtn->Check( !pCheckBtn->IsCheck() );
		if( pCheckBtn->IsCheck() )	PushEvent( "SOUND",	EVENT_OPTION_BGMONOFF,	0, 1 );
		else						PushEvent( "SOUND",	EVENT_OPTION_BGMONOFF,	0, 0 );
	}
	else if( pComp->m_strName == "CHK_SOUND_EFFECT" )
	{
		pCheckBtn->Check( !pCheckBtn->IsCheck() );
		if( pCheckBtn->IsCheck() )	PushEvent( "SOUND",	EVENT_OPTION_EFFONOFF,	0, 1 );
		else						PushEvent( "SOUND",	EVENT_OPTION_EFFONOFF,	0, 0 );
	}
	else if( pComp->m_strName == "CHK_5_1CHA" )
	{
		pCheckBtn->Check( !pCheckBtn->IsCheck() );
		if( pCheckBtn->IsCheck() )	PushEvent( "SOUND",	EVENT_OPTION_51CHONOFF,	0, 1 );
		else						PushEvent( "SOUND",	EVENT_OPTION_51CHONOFF,	0, 0 );
	}

	//////////////////////////
	//	Other			    //
	//////////////////////////

	else
	{
		BOOL	bCheck = pCheckBtn->IsCheck();
		pCheckBtn->Check( !bCheck );
	} //if..else..
	
	m_pSound->Play( "button.wav" );
	return TRUE;
} //CW3DUInterface::CheckButton_LUp

/**
 * 에디트 박스 마우스 L 다운.
 */
BOOL CW3DUInterface::EditBox_LDown( UICComponent* pComp )
{
	PushEvent( (char*)pComp->m_strName.c_str(), EVENT_EDITTEXTCLICK, 0, 0 );

	Assert( pComp );

	m_pCurrEditor = reinterpret_cast<UIEditText*>( pComp );
	Assert( m_pCurrEditor );
	m_pCurrEditor->EditOn();

	return TRUE;
} //CW3DUInterface::EditBox_Down

/**
 * 픽쳐 박스 마우스 L 다운.
 */
BOOL CW3DUInterface::PictureBox_LDown( UICComponent* pComp )
{
	Assert( pComp );

	if( !pComp->m_strName.compare( "PIC_PET_INFO" ) )
	{
		UICComponent*	pDlg = GetComponent( "DLG_PET_INFO" );
		if( !pDlg )
		{
			Assert( !"Not Found the Component" );
			return FALSE;
		} //if
		
		if( pDlg->IsShow() )	ShowDlg( pDlg, FALSE );
		else					ShowDlg( pDlg, TRUE );
	} //if

	PushEvent( (char*)pComp->m_strName.c_str(), EVENT_PICTUREBOXCLICK, 0, 0 );
	return TRUE;
} //CW3DUInterface::PictureBox_Down

/**
 * 탭 버튼 마우스 L 업.
 */
BOOL CW3DUInterface::Tab_LUp( UICComponent* pComp )
{
	Assert( pComp );
	return FALSE; 
} //CW3DUInterface::Tab_LUp

/**
 * 체크버튼 처리.
 */
BOOL CW3DUInterface::CheckButtonGroup( 
	const char* szCheckKey,				///< 체크버튼 키.
	const int	nIndex					///< 선택 인덱스. '1' base
	)
{
	UICheckButton*	pCheckButton = NULL;
	char			szCheckBtn[64];

	int nCnt = 1;
	do
	{
		sprintf( szCheckBtn, "%s%d", szCheckKey, nCnt );
		pCheckButton = reinterpret_cast<UICheckButton*>( GetComponent( szCheckBtn ) );
		if( !pCheckButton )
		{
			if( nCnt > 1 )		return TRUE;
			else				return FALSE;
		} //if

		if( nIndex == nCnt )	pCheckButton->Check( TRUE );
		else					pCheckButton->Check( FALSE );
		
		nCnt++;
	} //do
	while( pCheckButton );

	return TRUE;
} //CW3DUInterface::CheckButtonGroup

/**
 * 현재 체크버튼 인덱스.
 */
int CW3DUInterface::GetCheckButtonIndex( 
	const char* szCheckKey				///< 체크버튼 키.
	)
{
	UICheckButton*	pCheckButton = NULL;
	char			szCheckBtn[64];

	int nCnt = 1;
	do
	{
		sprintf( szCheckBtn, "%s%d", szCheckKey, nCnt );
		pCheckButton = reinterpret_cast<UICheckButton*>( GetComponent( szCheckBtn ) );
		if( !pCheckButton )
		{
			return 0;
		} //if

		if( pCheckButton->IsCheck() )
		{
			return nCnt;
		} //if

		nCnt++;
	} //do
	while( pCheckButton );

	return 0;
}

/**
 * 콤포넌트 그룹 보이기 처리.
 */
BOOL CW3DUInterface::ShowCompGroup( 
	const char* szCompKey,				///< 체크버튼 키.
	const int	nIndex					///< 인덱스.
	)
{
	UICComponent*	pComp;
	char			szCompName[64];

	int nCnt = 1;
	do
	{
		sprintf( szCompName, "%s%d", szCompKey, nCnt );
		pComp = GetComponent( szCompName ); 
		if( !pComp )
		{
			if( nCnt > 1 )		return TRUE;
			else				return FALSE;
		} //if
		
		if( nIndex == nCnt )	pComp->Show( TRUE );
		else					pComp->Show( FALSE );

		nCnt++;
	} //do
	while( pComp );

	return TRUE;
} //CW3DUInterface::ShowCompGroup

/**
 * 쳇 해당 체크 버튼.
 */
BOOL CW3DUInterface::ChatCheckButton( const int nIndex )
{
	m_btChatOption = nIndex;
	CheckButtonGroup( "CHK_CHAT_TAB", nIndex );
	return TRUE;
} //CW3DUInterface::ChatCheckButton

/**
 * 이벤토리 해당 탭 체크 버튼.
 */
BOOL CW3DUInterface::CheckGroup( const char* szTabKey, const char* szInvenKey, const int nIndex )
{
	CheckButtonGroup( szTabKey, nIndex );
	ShowCompGroup( szInvenKey, nIndex );

	return TRUE;
} //CW3DUInterface::InventoryCheckButton

/**
 * 스킬1 해당 탭 체크 버튼.
 */
BOOL CW3DUInterface::Skill1CheckButton( const int nIndex )
{
	Assert( nIndex > 0 && nIndex < 5 );

	CheckButtonGroup( "CHK_SKILL", nIndex );

	UISkillEntry*	pSkillEntry1 = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_SHORT" ) );
	UISkillEntry*	pSkillEntry2 = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_LONG"  ) );
	UISkillEntry*	pSkillEntry3 = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_BLACK" ) );
	UISkillEntry*	pSkillEntry4 = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_WHITE" ) );

	if( !pSkillEntry1 || !pSkillEntry2 || !pSkillEntry3 || !pSkillEntry4 )
	{
		Assert( !"없는 컨트럴" );
		return FALSE;
	} //if

	switch( nIndex )
	{
	case 1:
		pSkillEntry1->Show( TRUE );
		pSkillEntry2->Show( FALSE );
		pSkillEntry3->Show( FALSE );
		pSkillEntry4->Show( FALSE );
		pSkillEntry1->SetScrollPos( 0 );
		break;

	case 2:
		pSkillEntry1->Show( FALSE );
		pSkillEntry2->Show( TRUE );
		pSkillEntry3->Show( FALSE );
		pSkillEntry4->Show( FALSE );
		pSkillEntry2->SetScrollPos( 0 );
		break;

	case 3:
		pSkillEntry1->Show( FALSE );
		pSkillEntry2->Show( FALSE );
		pSkillEntry3->Show( TRUE );
		pSkillEntry4->Show( FALSE );
		pSkillEntry3->SetScrollPos( 0 );
		break;

	case 4:
		pSkillEntry1->Show( FALSE );
		pSkillEntry2->Show( FALSE );
		pSkillEntry3->Show( FALSE );
		pSkillEntry4->Show( TRUE );
		pSkillEntry4->SetScrollPos( 0 );
		break;
	} //if..else

	UIScrollBar*	pScrollComp = reinterpret_cast<UIScrollBar*>( GetComponent( "SCR_SKILL_LIST1" ) );
	Assert( pScrollComp );
	pScrollComp->SetScrollPos( 0 );

	return TRUE;
} //CW3DUInterface::Skill1CheckButton

/**
 * 스킬1 해당 탭 체크 버튼.
 */
BOOL CW3DUInterface::Skill2CheckButton( const int nIndex )
{
	Assert( nIndex > 0 && nIndex < 5 );

	CheckButtonGroup( "CHK_SKILL_ATAB", nIndex );

	UISkillEntry*	pSkillEntry1 = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_SPECIAL" ) );
	UISkillEntry*	pSkillEntry2 = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_LIFE"  ) );
	UISkillEntry*	pSkillEntry3 = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_GUILD" ) );
	UISkillEntry*	pSkillEntry4 = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_COUNTRY" ) );

	if( !pSkillEntry1 || !pSkillEntry2 || !pSkillEntry3 || !pSkillEntry4 )
	{
		Assert( !"없는 컨트럴" );
		return FALSE;
	} //if

	switch( nIndex )
	{
	case 1:
		pSkillEntry1->Show( TRUE );
		pSkillEntry2->Show( FALSE );
		pSkillEntry3->Show( FALSE );
		pSkillEntry4->Show( FALSE );
		pSkillEntry1->SetScrollPos( 0 );
		break;

	case 2:
		pSkillEntry1->Show( FALSE );
		pSkillEntry2->Show( TRUE );
		pSkillEntry3->Show( FALSE );
		pSkillEntry4->Show( FALSE );
		pSkillEntry2->SetScrollPos( 0 );
		break;

	case 3:
		pSkillEntry1->Show( FALSE );
		pSkillEntry2->Show( FALSE );
		pSkillEntry3->Show( TRUE );
		pSkillEntry4->Show( FALSE );
		pSkillEntry3->SetScrollPos( 0 );
		break;

	case 4:
		pSkillEntry1->Show( FALSE );
		pSkillEntry2->Show( FALSE );
		pSkillEntry3->Show( FALSE );
		pSkillEntry4->Show( TRUE );
		pSkillEntry4->SetScrollPos( 0 );
		break;
	} //if..else

	UIScrollBar*	pScrollComp = reinterpret_cast<UIScrollBar*>( GetComponent( "SCR_SKILL_LIST2" ) );
	Assert( pScrollComp );
	pScrollComp->SetScrollPos( 0 );

	return TRUE;
} //CW3DUInterface::Skill2CheckButton

/**
 * 현재 보여주고 있는 스킬 창을 얻는다
 */
UISkillEntry* CW3DUInterface::GetShowSkill1Window()
{
	UICComponent*	pSkillEntry[4];
	pSkillEntry[0] = GetComponent( "SKILL_SHORT" );
	pSkillEntry[1] = GetComponent( "SKILL_LONG" );
	pSkillEntry[2] = GetComponent( "SKILL_BLACK" );
	pSkillEntry[3] = GetComponent( "SKILL_WHITE" );

	if( !pSkillEntry[0] || !pSkillEntry[1] || !pSkillEntry[2] || !pSkillEntry[3] )
	{
		Assert( !"없는 컨트럴" );
		return NULL;
	} //if

	for( int nCnt = 0; nCnt < 4; nCnt++ )
	{
		if( pSkillEntry[nCnt]->IsShow() )
		{
			return reinterpret_cast<UISkillEntry*>( pSkillEntry[nCnt] );
		} //if
	} //for

	return NULL;
} //CW3DUInterface::GetShowSkill1Window

/**
 * 현재 보여주고 있는 스킬 창을 얻는다
 */
UISkillEntry* CW3DUInterface::GetShowSkill2Window()
{
	UICComponent*	pSkillEntry[4];
	pSkillEntry[0] = GetComponent( "SKILL_SPECIAL" );
	pSkillEntry[1] = GetComponent( "SKILL_LIFE" );
	pSkillEntry[2] = GetComponent( "SKILL_GUILD" );
	pSkillEntry[3] = GetComponent( "SKILL_COUNTRY" );

	if( !pSkillEntry[0] || !pSkillEntry[1] || !pSkillEntry[2] || !pSkillEntry[3] )
	{
		Assert( !"없는 컨트럴" );
		return NULL;
	} //if

	for( int nCnt = 0; nCnt < 4; nCnt++ )
	{
		if( pSkillEntry[nCnt]->IsShow() )
		{
			return reinterpret_cast<UISkillEntry*>( pSkillEntry[nCnt] );
		} //if
	} //for

	return NULL;
} //CW3DUInterface::GetShowSkill2Window

/**
 * 스크롤 덤브 이동.
 */
BOOL CW3DUInterface::ScrollThumb_Move( UICComponent* pComp )
{
	Assert( pComp );
	if( !m_bLButtonPressed )	return FALSE;

	UIScrollBar*		pScrollBar = (UIScrollBar*)pComp;
	if( !pScrollBar )
	{
		Assert( !"UIScrollBar를 찾지 못했습니다" );
		return FALSE;
	} //if

	//////////////////////////////////////
	//	쳇팅창 스크롤바					//
	//////////////////////////////////////

	if( pScrollBar->m_strName == "SCR_CHAT_LIST" )
	{
		UIStringList*	pChatComp	= (UIStringList*)GetComponent( "LIST_CHATTING" );
		if( !pChatComp )
		{
			Assert( pChatComp );
			return FALSE;
		} //if

		int		nScrollMax = pChatComp->GetListSize() - pChatComp->GetDisplayLineCount();

		pScrollBar->SetMax( nScrollMax );
		pScrollBar->MoveThumb( m_nMouseY - m_nOldMouseY );

		int		nScrollPos = nScrollMax - pScrollBar->GetScrollPos();
		pChatComp->SetPos( nScrollPos );
	}

	//////////////////////////////////////
	//	메시지창 스크롤바				//
	//////////////////////////////////////
	if( pScrollBar->m_strName == "SCR_MSG_LIST" )
	{
		UIStringList*	pChatComp	= (UIStringList*)GetComponent( "LIST_MSG" );
		if( !pChatComp )
		{
			Assert( pChatComp );
			return FALSE;
		} //if
		
		int		nScrollMax = pChatComp->GetListSize() - pChatComp->GetDisplayLineCount();
		
		pScrollBar->SetMax( nScrollMax );
		pScrollBar->MoveThumb( m_nMouseY - m_nOldMouseY );
		
		int		nScrollPos = nScrollMax - pScrollBar->GetScrollPos();
		pChatComp->SetPos( nScrollPos );
	}

	//////////////////////////////////////
	//	일반공격 스킬창 스크롤			//
	//////////////////////////////////////

	else if( pScrollBar->m_strName == "SCR_SKILL_LIST1" )
	{
		UISkillEntry*	pSkillComp = GetShowSkill1Window();
		if( !pSkillComp )
		{
			return FALSE;
		} //if

		int		nScrollMax = pSkillComp->GetScrollMax();
		pScrollBar->SetMax( nScrollMax );
		pScrollBar->MoveThumb( m_nMouseY - m_nOldMouseY );

		int		nScrollPos = pScrollBar->GetScrollPos();
		pSkillComp->SetScrollPos( nScrollPos );
	}

	//////////////////////////////////////
	//	생활 스킬창 스크롤				//
	//////////////////////////////////////

	else if( pScrollBar->m_strName == "SCR_SKILL_LIST2" )
	{
		UISkillEntry*	pSkillComp = GetShowSkill2Window();
		if( !pSkillComp )
		{
			return FALSE;
		} //if

		int		nScrollMax = pSkillComp->GetScrollMax();
		pScrollBar->SetMax( nScrollMax );
		pScrollBar->MoveThumb( m_nMouseY - m_nOldMouseY );

		int		nScrollPos = pScrollBar->GetScrollPos();
		pSkillComp->SetScrollPos( nScrollPos );
	}

	//////////////////////////////////////
	//	친구리스트창 스크롤				//
	//////////////////////////////////////

	else if( pScrollBar->m_strName == "SCR_CHATID_LIST" )
	{
		UISelectString*		pSStringComp = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_CHATID_SAVE" ) );
		if( !pSStringComp )
		{
			return FALSE;
		} //if

		int		nScrollMax = pSStringComp->GetScrollMax();
		pScrollBar->SetMax( nScrollMax );
		pScrollBar->MoveThumb( m_nMouseY - m_nOldMouseY );
		int		nScrollPos = pScrollBar->GetScrollPos();
		pSStringComp->SetScrollPos( nScrollPos );
	} 

	//////////////////////////////////////
	//	길드리스트창 스크롤				//
	//////////////////////////////////////

	else if( pScrollBar->m_strName == "SCR_GUILDMEMBERLIST_LIST" )
	{
		UISelectString*		pSStringComp = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_GUILDMEMBERLIST_LIST" ) );
		if( !pSStringComp )
		{
			return FALSE;
		} //if

		int		nScrollMax = pSStringComp->GetScrollMax();
		pScrollBar->SetMax( nScrollMax );
		pScrollBar->MoveThumb( m_nMouseY - m_nOldMouseY );
		int		nScrollPos = pScrollBar->GetScrollPos();
		pSStringComp->SetScrollPos( nScrollPos );
	} 

	//////////////////////////////////////
	//	플레이어리스트창 스크롤			//
	//////////////////////////////////////

	else if( pScrollBar->m_strName == "SCR_PLAYER_LIST" )
	{
		UISelectString*		pSStringComp = reinterpret_cast<UISelectString*>( GetComponent( "SSTR_PLAYER_LIST" ) );
		if( !pSStringComp )
		{
			return FALSE;
		} //if

		int		nScrollMax = pSStringComp->GetScrollMax();
		pScrollBar->SetMax( nScrollMax );
		pScrollBar->MoveThumb( m_nMouseY - m_nOldMouseY );
		int		nScrollPos = pScrollBar->GetScrollPos();
		pSStringComp->SetScrollPos( nScrollPos );
	} 

	//////////////////////////////////////
	//	사운드 볼륨 스크롤				//
	//////////////////////////////////////

	else if( pScrollBar->m_strName == "SCR_SOUND_BGM" )
	{
		pScrollBar->SetMax( 500 );
		pScrollBar->MoveThumb( m_nMouseX - m_nOldMouseX );
		int		nScrollPos = pScrollBar->GetScrollPos();
		PushEvent( "SOUND", EVENT_SOUND_VOLUME, 0, nScrollPos );
	}
	else if( pScrollBar->m_strName == "SCR_SOUND_EFFECT" )
	{
		pScrollBar->SetMax( 1000 );
		pScrollBar->MoveThumb( m_nMouseX - m_nOldMouseX );
		int		nScrollPos = pScrollBar->GetScrollPos();
		PushEvent( "SOUND", EVENT_SOUND_VOLUME, 1, nScrollPos );
	} //if..else..
	
	return TRUE;
} //CW3DUInterface::ScrollThumb_Move

/**
 * 셀렉트 스티링 마우스 L 다운.
 */
BOOL CW3DUInterface::SelectString_LDown( UICComponent* pComp )
{
	Assert( pComp );

	UISelectString*		pSelStr = reinterpret_cast<UISelectString*>( pComp );
	Assert( pSelStr );
	pSelStr->Push( TRUE, m_nMouseX, m_nMouseY );
	
	return TRUE;
} //CW3DUInterface::SelectString_LDown

/**
 * 셀렉트 스티링 마우스 L 업.
 */
BOOL CW3DUInterface::SelectString_LUp( UICComponent* pComp )
{
	Assert( pComp );

	UISelectString*		pUISStr = reinterpret_cast<UISelectString*>( pComp );
	Assert( pUISStr );
	pUISStr->Push( FALSE );

	if( pComp->m_strName == "SSTR_NPC_ANSWER" )
	{
		int		nNPCAnswer = pUISStr->GetSelectIndex();

		if( nNPCAnswer >= 0 )
		{
			if( m_bNPCTalkMode == 0 )
			{
				NextNPC( m_pNPCScript, nNPCAnswer );
			}

			else if( m_bNPCTalkMode == 1 )
			{
				NextNPC( m_pQuestScript, nNPCAnswer );
				if( nNPCAnswer == 0 )
				{
					PushEvent( pComp->m_strName.c_str(), EVENT_QUEST_ACCEPT, m_nYouId, m_wQuest );				
				} //if
			}
			else if( m_bNPCTalkMode == 2 )
			{
				NextNPC( m_pSmithScript, nNPCAnswer );
			} //if..else
		} //if
	} //if

	return TRUE;
} //CW3DUInterface::SelectString_LUp

/**
 * 셀렉트 스티링 마우스 L 더블클릭.
 */
BOOL CW3DUInterface::SelectStringDBClick_L( UICComponent* pComp )
{
	if( pComp->m_strName == "SSTR_CHATID_SAVE" )
	{
		RequestTalkFriend();
	} //if
	return TRUE;
} //CW3DUInterface::SelectStringDBClick_L

/**
 * 인벤토리 마우스 L 다운.
 */
BOOL CW3DUInterface::InventoryIn_LDown( UICComponent* pComp )
{
	UIInventoryIN*	pInventory = reinterpret_cast<UIInventoryIN*>( pComp );
	Assert( pComp );

	SItemData*	pItemData = pInventory->GetItem( m_nMouseX, m_nMouseY );
	if( !pItemData )
	{ // 아이템이 존재 하는 경우.
		return TRUE;
	} //if

	UICComponent*		pParent = pComp->m_pOwnerComp;
	Assert( pParent );

	if( pParent->m_strName == "DLG_INVENTORY" )
	{ // 내 인벤토리
		m_ItemMgr.SetHandInItemDat( pItemData, FROM_INVENTORY );
	}
	else if( pParent->m_strName == "DLG_SHOPINVENTORY" )
	{ // 상점
		m_ItemMgr.SetHandInItemDat( pItemData, FROM_SHOP );
	}
	else if( pParent->m_strName == "DLG_STORAGE" )
	{ // 창고
		m_ItemMgr.SetHandInItemDat( pItemData, FROM_STORAGE );
	}
	else if( pParent->m_strName == "DLG_DEAL" )
	{ // 유저거래 
		m_ItemMgr.SetHandInItemDat( pItemData, FROM_USERDEAL );
	} 
	else if( pParent->m_strName == "DLG_CREATEMYSHOP" )
	{ // 창고
		m_ItemMgr.SetHandInItemDat( pItemData, FROM_MYSHOP );
	}
	else if( pParent->m_strName == "DLG_PERSONALSHOP" )
	{ // 창고
		m_ItemMgr.SetHandInItemDat( pItemData, FROM_PSHOP );
	}
	else if( pComp->m_strName == "CASHITEM_IN_1" ||
			 pComp->m_strName == "CASHITEM_IN_2" )
	{
		m_ItemMgr.SetHandInItemDat( pItemData, FROM_CASH );
	} //if..else
	else if( pParent->m_strName == "DLG_SMITH_WP_INTEGRATION" )
	{
		m_ItemMgr.SetHandInItemDat( pItemData, FROM_WP );
	}
	else if( pParent->m_strName == "DLG_SMITH_AM_INTEGRATION" )
	{
		m_ItemMgr.SetHandInItemDat( pItemData, FROM_AM );
	}

	PushEvent( "UI_INVENTORYIN", EVENT_MOUSEBOTTON, 0, 1 );			///< 0 왼쪽, 1 눌림

	return TRUE;
} //CW3DUInterface::InventoryIn_LDown

/**
 * 인벤토리 마우스 L 업.
 */
BOOL CW3DUInterface::InventoryIn_LUp( UICComponent* pComp )
{
	Assert( pComp );

	SItemData*	pItemDat = m_ItemMgr.GetHandInItemDat();
	if( !pItemDat )
	{
		return TRUE;
	} //if

	UIInventoryIN*		pInventory = reinterpret_cast<UIInventoryIN*>( pComp );
	UICComponent*		pParent = pComp->m_pOwnerComp;
	Assert( pParent );

	m_pConfirmItemData = pItemDat;

	if( pParent->m_strName == "DLG_STORAGE" )
	{ // 창고
		RequestSaveItemInStorage( pItemDat );
	}
	else if( pParent->m_strName == "DLG_DEAL" )
	{ // 유저거래 
		if( pItemDat->pItemInfoData->bItemIndex )	MessageBox( STR_UIMSG_ENCHANTITEM, "CONFIRM_ITEMDEAL", MSG_YESNO );
		else										RequestInsertItemInUserDeal( pItemDat );
	}
	else if( pParent->m_strName == "DLG_SHOPINVENTORY" )
	{ // 상점
	//	if( pItemDat->pItemInfoData->bItemIndex )	MessageBox( STR_UIMSG_ENCHANTITEM, "CONFIRM_ITEMSHOP", MSG_YESNO );
	//	else										RequestSellItem( pItemDat );
		RequestSellItem( pItemDat );
	}
	else if( pParent->m_strName == "DLG_CREATEMYSHOP" )
	{ // 개인상점
		if( pItemDat->pItemInfoData->bItemIndex )	MessageBox( STR_UIMSG_ENCHANTITEM, "CONFIRM_ITEMMYSHOP", MSG_YESNO );
		else										RequestInsertItemInMyShop( pItemDat );
	}
	else if( pParent->m_strName == "DLG_INVENTORY" )
	{ // 내 인벤토리
		ItemInMyInventory( pItemDat, pInventory );
	} //if..else
	else if( pParent->m_strName == "DLG_SMITH_WP_INTEGRATION" )
	{
		m_SelectIntegration = TRUE;
		RequestSaveitemInSmith( pItemDat );
	}
	else if( pParent->m_strName == "DLG_SMITH_AM_INTEGRATION" )
	{
		m_SelectIntegration = FALSE;
		RequestSaveitemInSmith( pItemDat );
	}

	return TRUE;
} //CW3DUInterface::InventoryIn_LUp

/**
 * 인벤토리 더블 클릭.
 */
BOOL CW3DUInterface::InventoryInDBClick_L( UICComponent* pComp )
{
	Assert( pComp );

	UIInventoryIN*		pInventory = reinterpret_cast<UIInventoryIN*>( pComp );
	UICComponent*		pParent = pComp->m_pOwnerComp;
	Assert( pParent );

	SItemData*	pItemDat = pInventory->GetItem( m_nMouseX, m_nMouseY );
	if( !pItemDat )
	{
		return TRUE;
	} //if

	if( pParent->m_strName == "DLG_STORAGE" )
	{ // 창고
		TakeItemInStorage( pItemDat );
	}
	else if( pParent->m_strName == "DLG_SERVER_STORAGE" )
	{ // 통합서버 창고
		TakeItemInSerStorage( pItemDat, m_ServerNum );
	}
	else if( pParent->m_strName == "DLG_SERVER_INVENTORY" )
	{
		TakeItemInSerInven( pItemDat, m_ServerNum, m_CharPos );
	}
	else if( pParent->m_strName == "DLG_DEAL" )
	{ // 유저거래 
		RequestDeleteItemInUserDeal( pItemDat );
	}
	else if( pParent->m_strName == "DLG_SHOPINVENTORY" )
	{ // 상점
		BuyItemInShop( pItemDat );
	}
	else if( pParent->m_strName == "DLG_CREATEMYSHOP" )
	{ // 내 개인상점
		RequestDeleteItemInPersonalShop( pItemDat );
	}
	else if( pParent->m_strName == "DLG_PERSONALSHOP" )
	{ // 개인상점
		BuyItemToPersonalShop( pItemDat );
	}
	else if( pComp->m_strName == "CASHITEM_IN_1" ||
			 pComp->m_strName == "CASHITEM_IN_2" )
	{
		CashItemUse( pItemDat );
	} //if..else
	else if( pParent->m_strName == "DLG_INVENTORY" )
	{ // 내 인벤토리
		UICComponent*		pShop	    = GetComponent( "DLG_SHOPINVENTORY" );
		UICComponent*		pStorage    = GetComponent( "DLG_STORAGE" );
		UICComponent*		pUserDeal   = GetComponent( "DLG_DEAL" );
		UICComponent*		pMyShopDeal = GetComponent( "DLG_CREATEMYSHOP" );

		Assert( pShop );
		Assert( pStorage );
		Assert( pUserDeal );

		if( pShop->IsShow() )
		{
			if( pItemDat->pItemInfoData->bItemIndex )	MessageBox( STR_UIMSG_ENCHANTITEM, "CONFIRM_ITEMSHOP", MSG_YESNO );
			else										RequestSellItem( pItemDat );
		}
		else if( pUserDeal->IsShow() )
		{
			if( pItemDat->pItemInfoData->bItemIndex )	MessageBox( STR_UIMSG_ENCHANTITEM, "CONFIRM_ITEMDEAL", MSG_YESNO );
			else										RequestInsertItemInUserDeal( pItemDat );
		}
		else if( pMyShopDeal && pMyShopDeal->IsShow() )
		{
			if( pItemDat->pItemInfoData->bItemIndex )	MessageBox( STR_UIMSG_ENCHANTITEM, "CONFIRM_ITEMMYSHOP", MSG_YESNO );
			else										RequestInsertItemInMyShop( pItemDat );
		}
		else if( pStorage->IsShow() )
		{
			RequestSaveItemInStorage( pItemDat );
		}
		else
		{
			RequestUseItem( pItemDat );
		} //if.. if else..else
	} //if..else
	
	return TRUE;
} //CW3DUInterface::InventoryInDBClick_L

/**
 * 인벤토리 마우스 R 업.
 */
BOOL CW3DUInterface::InventoryIn_RUp( UICComponent* pComp )
{
	Assert( pComp );
	UIInventoryIN*		pInventory = reinterpret_cast<UIInventoryIN*>( pComp );
	Assert( pInventory );

	SItemData*	pItemData = pInventory->GetItem( m_nMouseX, m_nMouseY );
	if( pItemData )
	{
		switch( pItemData->nType )
		{
		case ITEM_MP_L:
		case ITEM_HP_L:
		case ITEM_TELEPORT_VAIL:
		case ITEM_TELEPORT_LEGYON:
		case ITEM_TELEPORT_MOHAS:
		case ITEM_TELEPORT_MIRAS:
			PushEvent( "UI_INVENTORYIN", EVENT_CAST_ITEM, pItemData->nId, 0 );
			break;
		}
	}
	return TRUE;
} //CW3DUInterface::InventoryIn_RUp

/**
 * 장착창 마우스 다운.
 */
BOOL CW3DUInterface::InventoryOn_LDown( UICComponent* pComp )
{
	Assert( pComp );
	UIInventoryON*	pInvenON = reinterpret_cast<UIInventoryON*>( pComp );

	SItemData*	pOldItemData = pInvenON->GetItem( m_nMouseX, m_nMouseY );
	if( pOldItemData )
	{ // 아이템이 존재 하는 경우.
		m_ItemMgr.SetHandInItemDat( pOldItemData, FROM_EQUIP );
	} //if
	
	return TRUE;
} //CW3DUInterface::InventoryOn_LDown

/**
 * 장착창 마우스 업.
 */
BOOL CW3DUInterface::InventoryOn_LUp( UICComponent* pComp )
{
	Assert( pComp );

	SItemData*		pItemData = m_ItemMgr.GetHandInItemDat();
	if( pItemData && m_ItemMgr.FromHandItem() == FROM_INVENTORY )
	{
		RequestUseItem( pItemData );
		SoundItem( pItemData );
	} //if

	return TRUE;
} //CW3DUInterface::InventoryOn_LUp

/**
 * 장착창 더블 클릭.
 */
BOOL CW3DUInterface::InventoryOnDBClick_L( UICComponent* pComp )
{
	Assert( pComp );

	UIInventoryON*		pInvenOn = reinterpret_cast<UIInventoryON*>( pComp );
	Assert( pInvenOn );

	SItemData*	pItemData = pInvenOn->GetItem( m_nMouseX, m_nMouseY );
	if( pItemData )
	{
		PushEvent( "UI_INVENTORYON", EVENT_DBCLICK_ITEM, pItemData->nId, 1 );		// '1' 장착창
		SoundItem( pItemData );
	} //if
	return TRUE;
} //CW3DUInterface::InventoryInDBClick_L

/**
 * 스킬창 더블 클릭
 */
BOOL CW3DUInterface::SkillEntryDBClick_L( UICComponent* pComp )
{
	UISkillEntry*		pSkillEntry = reinterpret_cast<UISkillEntry*>( pComp );
	Assert( pSkillEntry );
	
	SItemData*	pItemData = pSkillEntry->GetItemData( m_nMouseX, m_nMouseY );
	if( pItemData )
	{
		SkillProc( pItemData );
		return TRUE;
	} //if
	return FALSE;
} //CW3DUInterface::SkillEntryDBClick_L

/**
 * 스크롤 바 클릭.
 */
BOOL CW3DUInterface::ScrollBar_LDown( UICComponent* pComp )
{
	Assert( pComp );
	UIScrollBar*	pScrollBar = reinterpret_cast<UIScrollBar*>( pComp );

	if( pScrollBar->m_strName == "SCR_CHAT_LIST" )			// 쳇팅창 스크롤바
	{
		UIStringList*	pChatComp	= (UIStringList*)GetComponent( "LIST_CHATTING" );
		Assert( pChatComp );
		
		int		nScrollMax = pChatComp->GetListSize() - pChatComp->GetDisplayLineCount();
		pScrollBar->SetMax( nScrollMax );
		pScrollBar->ClickBar( m_nMouseX, m_nMouseY );

		int		nScrollPos = nScrollMax - pScrollBar->GetScrollPos();
		pChatComp->SetPos( nScrollPos );

		return TRUE;
	}
	else if(  pScrollBar->m_strName == "SCR_MSG_LIST"  )
	{
		UIStringList*	pChatComp	= (UIStringList*)GetComponent( "LIST_MSG" );
		Assert( pChatComp );
		
		int		nScrollMax = pChatComp->GetListSize() - pChatComp->GetDisplayLineCount();
		pScrollBar->SetMax( nScrollMax );
		pScrollBar->ClickBar( m_nMouseX, m_nMouseY );
		
		int		nScrollPos = nScrollMax - pScrollBar->GetScrollPos();
		pChatComp->SetPos( nScrollPos );
		
		return TRUE;
	}
	else if( pScrollBar->m_strName == "SCR_SKILL_LIST1" )
	{
		UISkillEntry*	pSkillComp = GetShowSkill1Window();
		if( !pSkillComp )
		{
			return TRUE;
		}//if

		int		nScrollMax = pSkillComp->GetScrollMax();
		pScrollBar->SetMax( nScrollMax );
		pScrollBar->ClickBar( m_nMouseX, m_nMouseY );

		int		nScrollPos = pScrollBar->GetScrollPos();
		pSkillComp->SetScrollPos( nScrollPos );
	}
	else if( pScrollBar->m_strName == "SCR_SKILL_LIST2" )
	{
		UISkillEntry*	pSkillComp = GetShowSkill2Window();
		if( !pSkillComp )
		{
			return TRUE;
		}//if

		int		nScrollMax = pSkillComp->GetScrollMax();
		pScrollBar->SetMax( nScrollMax );
		pScrollBar->ClickBar( m_nMouseX, m_nMouseY );

		int		nScrollPos = pScrollBar->GetScrollPos();
		pSkillComp->SetScrollPos( nScrollPos );
	}
	else if( pScrollBar->m_strName == "SCR_SOUND_BGM" )
	{
		pScrollBar->SetMax( 500 );
		pScrollBar->ClickBar( m_nMouseX, m_nMouseY );
		float	fScrollPos = pScrollBar->GetScrollPos() * 0.001f;		
	}
	else if( pScrollBar->m_strName == "SCR_SOUND_EFFECT" )
	{
		pScrollBar->SetMax( 500 );
		pScrollBar->ClickBar( m_nMouseX, m_nMouseY );
		float	fScrollPos = pScrollBar->GetScrollPos() * 0.001f;		
	} //if..else

	return FALSE;
} //CW3DUInterface::ScrollBar_LDown

/**
 * 퀵슬롯 마우스 다운.
 */
BOOL CW3DUInterface::QuickSlot_LDown( UICComponent* pComp )
{
	Assert( pComp );
	UIQuickSlot*	pQuickSlot = reinterpret_cast<UIQuickSlot*>( pComp );
	int				nSlotNum   = pQuickSlot->GetSlotIndex( m_nMouseX, m_nMouseY );
	SItemData*		pItemDat   = pQuickSlot->GetItem( nSlotNum );
	pQuickSlot->RemoveItem( nSlotNum );
	if( pItemDat )
	{
		m_ItemMgr.SetHandInItemDat( pItemDat, FROM_QUICK + nSlotNum );
	} //if

	return TRUE;
} //CW3DUInterface::QuickSlot_LDn

BOOL CW3DUInterface::QuickSlot_HoverMouse( UICComponent* pComp )
{
	Assert( pComp );

	UIQuickSlot*	pQuickSlot = reinterpret_cast<UIQuickSlot*>( pComp );
	int				nSlotNum   = pQuickSlot->GetSlotIndex( m_nMouseX, m_nMouseY );
	SItemData*		pItemDat   = pQuickSlot->GetItem( nSlotNum );

	if( pItemDat )
	{
		if( pItemDat->pItemInfoData )
		{
			pQuickSlot->UpdateTipInfo( m_nMouseX, pItemDat->pItemInfoData->szHName, "", "" );
			return 1;
		}
	}
	return 0;
}

/**
 * 퀵슬롯 마우스 업.
 */
BOOL CW3DUInterface::QuickSlot_LUp( UICComponent* pComp )
{
	Assert( pComp );
	UIQuickSlot*	pQuickSlot  = reinterpret_cast<UIQuickSlot*>( pComp );

	int				nSlotIndex = pQuickSlot->GetSlotIndex( m_nMouseX, m_nMouseY );
	SItemData*		pItemDat   = m_ItemMgr.GetHandInItemDat();
	if( pItemDat && m_ItemMgr.FromHandItem() != FROM_SHOP )
	{
		SItemInfoData*	pItemInfo = pItemDat->pItemInfoData;
		if( pItemInfo )
		{
			if( GetItemDataInSkill( pItemInfo->wItemNum ) )
			{
			}
			else if( GetItemDataInInventory( pItemInfo->wItemNum ) )
			{
			}
			else if( GetItemDataInEquip( pItemInfo->wItemNum ) )
			{
			}
			else
			{
				m_ItemMgr.SetHandInItemDat( NULL, FROM_NULL );
				return TRUE;
			} //if

			pQuickSlot->InsertSlot( nSlotIndex, pItemDat );
			RefreshQuickSlot( pQuickSlot );
			SaveUserData( FALSE );
		} //if
	} //if
	m_ItemMgr.SetHandInItemDat( NULL, FROM_NULL );

	return TRUE;
} //CW3DUInterface::QuickSlot_LUp


/**
 * 스킬등록 마우스 다운.
 */
BOOL CW3DUInterface::SkillEntry_LDown( UICComponent* pComp )
{
	Assert( pComp );
	UISkillEntry*	pSkillEntry = reinterpret_cast<UISkillEntry*>( pComp );
	SItemData*		pItemDat    = pSkillEntry->GetItemData( m_nMouseX, m_nMouseY );
	if( pItemDat )
	{
		m_ItemMgr.SetHandInItemDat( pItemDat, FROM_SKILLENTRY );
	} //if

	return TRUE;
} //CW3DUInterface::SkillEntry_LDown

/**
 * 스킬등록 마우스 업.
 */
BOOL CW3DUInterface::SkillEntry_LUp( UICComponent* pComp )
{
	SItemData*	pItemDat = m_ItemMgr.GetHandInItemDat();
	if( !pItemDat )
	{
		return TRUE;
	} //if

	RequestUseItem( pItemDat );

	return TRUE;
} //CW3DUInterface::SkillEntry_LUp

/**
 * 다이얼로그 박스 마우스 버튼 업.
 */
void CW3DUInterface::DialogBox_LDown( UICComponent* pComp )
{
	Assert( pComp );
	UIDialog*	pDlg = reinterpret_cast<UIDialog*>( pComp );

	if( pDlg->PtInTitleBar( m_nMouseX, m_nMouseY ) )
	{
		pDlg->Push( TRUE );
	} //if
} //CW3DUInterface::DialogBox_LDown

/**
 * 다이얼로그 박스 마우스 버튼 업.
 */
void CW3DUInterface::DialogBox_LUp( UICComponent* pComp )
{
	Assert( pComp );
	UIDialog*	pDlg = reinterpret_cast<UIDialog*>( pComp );

	pDlg->Push( FALSE );
} //CW3DUInterface::DialogBox_LUp

/**
 * Dlg 박스 이동.
 */
void CW3DUInterface::DialogBox_Move( UIDialog* pDlg )
{
	Assert( pDlg );
	if( m_bLButtonPressed )
	{
		if( pDlg->IsPush() )
		{
			pDlg->Move( m_nMouseX - m_nOldMouseX, m_nMouseY - m_nOldMouseY );
		} //if
	} //if
} //CW3DUInterface::DialogBox_Box

/**
 * Rectangle 마우스 왼쪽 버튼 다운.
 */
void CW3DUInterface::Rectangle_LDown( UICComponent* pComp )
{
	char	szPartyDlg[64];
	for( int nCnt = 0; nCnt < MAX_PARTYNUM; nCnt++ )
	{
		sprintf( szPartyDlg, "RECT_PARTY%d",	nCnt );
		if( !pComp->m_strName.compare( szPartyDlg ) )
		{
			HealParty( nCnt );
			break;
		} //if
	} //for

	PushEvent( "UI_RECTANGLE", EVENT_MOUSEBOTTON, 0, 1 );			///< 0 왼쪽, 1 눌림
} //CW3DUInterface::Rectangle_LDown

/**
 * 체크버튼 내 마우스 이동.
 */
void CW3DUInterface::CheckButton_Move( UICComponent* pComp )
{
	SItemData*	pItemDat = m_ItemMgr.GetHandInItemDat();
	if( !pItemDat )
	{ // 손에 아이템 없을 시.
		return;
	} //if

	if( m_ItemMgr.FromHandItem() != FROM_INVENTORY )
	{ // 아이템이 인벤토리에서 오지 않을 시.
		return;
	} //if

	UICheckButton*	pCheckButton = reinterpret_cast<UICheckButton*>( pComp );
	Assert( pCheckButton );

	if( pCheckButton->IsCheck() )
	{ // 이미 체크.
		return;
	} //if

	if( pCheckButton->m_pOwnerComp->m_strName == "DLG_INVENTORY" )
	{ // 내 인벤토리.
		char*	pName = const_cast<char*>( pCheckButton->m_strName.c_str() );
		Assert( pName );

		int		nIndex = atoi( pName + strlen( pName ) - 1 );
		CheckGroup( "CHK_INVEN_TAB", "INVENTORY_IN_", nIndex );
	} //if
} //CW3DUInterface::CheckButton_Move

/**
 * 창을 닫는다
 */
void CW3DUInterface::Dlg_Exit( UIDialog* pDlg )
{
	Assert( pDlg );

	if( pDlg->IsShow() )
	{
		return;
	} //if

	if( !pDlg->m_strName.compare( "DLG_SHOPINVENTORY" ) )
	{
		ShowDlg( "DLG_BUYPOSION",   FALSE );
		ShowDlg( "DLG_DEALBUYBOX",  FALSE );
		ShowDlg( "DLG_DEALSELLBOX", FALSE );
	}
	else if( !pDlg->m_strName.compare( "DLG_CREATEMYSHOP" ) )
	{
		PushEvent( "MYSHOP", EVENT_MYSHOP_SELLFINISH, 0, 0 );
	}
	else if( !pDlg->m_strName.compare( "DLG_DEAL" ) )
	{
		ShowUserDealBox( FALSE );
		PushEvent( pDlg->m_strName.c_str(), EVENT_DEAL_CANCEL, 0, 0 );
	} //if..else
} //CW3DUInterface:Dlg_Exit

/**
 * 인벤토리 아이템 이동.
 */
void CW3DUInterface::MoveItemInInventory( SItemData* pItemDat, UIInventoryIN* pInventory )
{
/*	int		nX = m_nMouseX;
	int		nY = m_nMouseY;

	SItemData*	pExistItemDat = pInventory->GetItem( m_nMouseX, m_nMouseY );
	if( pItemDat == pExistItemDat )
	{
		return;
	} //if

	if( pExistItemDat )
	{
		if(	SelectedEnchantItem( pItemDat, FALSE ) )
		{
			UseEnchantItem( pExistItemDat );
			return;
		} //if
	} //if

	if( m_pOldFocusCom &&
		m_pOldFocusCom != pInventory )
	{ // 페이지 이동
		if( m_pOldFocusCom->m_nType == UI_INVENTORYIN )
		{
			if( !pInventory->GetBlankNum() )
			{
				return;
			} //if

			((UIInventoryIN*)m_pOldFocusCom)->DeleteItem( pItemDat->nId );
		} //if
	}
	else
	{
		pInventory->DeleteItem( pItemDat->nId );
	} //if..else

	if( !pInventory->InsertItem( nX, nY, pItemDat ) )
	{
		Assert( !"아템이동실패" );
		if( !pInventory->InsertItem( pItemDat ) )
		{
			Assert( !"아템이동실패" );
		} //if
	} //if

	SaveUserData( FALSE );
	return;*/

	/*if( pInventory->GetItem( m_nMouseX, m_nMouseY ) == pItemDat )
	{// 자기 자신과는 처리하지 않는다
	return;
	}*/

	Assert( pItemDat );
	Assert( pInventory );

	SItemData*	pExistItemDat = pInventory->GetItem( m_nMouseX, m_nMouseY );

	if( pExistItemDat )
	{
		if( !((pItemDat->nType == 10193 || pItemDat->nType == 10194) && pItemDat->nType == pExistItemDat->nType) && SelectedEnchantItem( pItemDat, FALSE ) )
		{ //인첸트
			UseEnchantItem( pExistItemDat );
			return;
		} //if
	}

  /*if( pInventory->GetItem( m_nMouseX, m_nMouseY ) == pItemDat )
	  {// 자기 자신과는 처리하지 않는다
	  return;
}*/
	
	int		nX = m_nMouseX;
	int		nY = m_nMouseY;

	if( !pInventory->GetCoord( nX, nY ) )
	{
		return;
	} //if
	
	int			nPage = GetCheckButtonIndex( "CHK_INVEN_TAB" ) - 1;		// 1 base

	if( nPage < 0 )
	{
		Assert( !"이상타" );
		return;
	} //if
	
	DWORD		dwParam1 = pItemDat->nId;
	DWORD		dwParam2 = nPage << 16 | nX << 8 | nY ;
	PushEvent( "UI_INVENTORYIN", EVENT_ITEM_MOVE, dwParam1, dwParam2 );		// '1' 장착창
} //CW3DUInterface::MoveItemInInventory

/**
 * 푸쉬버튼 상태.
 */
BOOL CW3DUInterface::GetPushButtonState( const char *szID )
{
	UICComponent	*pComp = GetComponent( szID );
	Assert( pComp );

	switch( pComp->m_nType )
	{
		case UI_PUSHBUTTON:
			return ((UIButton*)pComp)->IsPush();

		default:
			break;
	}
	return FALSE;
} //CW3DUInterface::GetPushButtonState

/**
 * 푸쉬버튼 상태 설정.
 */
BOOL CW3DUInterface::SetPushButtonState( const char *szID, int nState )
{
	UICComponent*	pComp = GetComponent( szID );
	Assert( pComp );

	if( pComp->m_nType != UI_PUSHBUTTON )
	{
		Assert( !"버튼이 아닙니다." );
		return FALSE;
	}

	switch( nState )
	{
	case 0:		pComp->Enable( TRUE );					break;		//< Enable
	case 1:		pComp->Enable( FALSE );					break;		//< Disable
	case 2:		((UIButton*)pComp)->Push( TRUE );		break;		//< Push
	case 3:		((UIButton*)pComp)->Push( FALSE );		break;		//< On
	}
	return TRUE;
} //CW3DUInterface::SetPushButtonState

/**
 * 에디트 텍스트 업뎃
 */
void CW3DUInterface::EditTextUpdate()
{
	if( !m_pCurrEditor )
	{
		return;
	} //if

	if( m_pCurrEditor->m_strName == "EDT_POSIONBUY_NUM" || m_pCurrEditor->m_strName == "EDT_CARDBUY_NUM" )
	{
		char*	pText = g_ChatIME.GetString();
		if( !pText )
		{
			Assert( !"텍스트 얻기 실패" );
			return;
		} //if
		m_nNumPosion = atoi( pText );
		RefreshBuyPosion();
	} //if

	m_pCurrEditor->Reflesh();
} //CW3DUInterface::EditTextUpdate

/**
 * IME용 Window Process 함수.
 *
 * @param	hWnd		[IN] 윈도우 핸들.
 * @param	msg			[IN] 윈도우 메세지.
 * @param	wParam		[IN] 파라메터 #1
 * @param	lParam		[IN] 파라메터 #2
 *
 * @return	Window Process 리턴값.
 */
BOOL CW3DUInterface::IMEProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	BOOL		bProc = FALSE;

	if( msg == WM_KEYDOWN && 
		wParam == VK_RETURN )
	{
		ReturnKey();
	} //if

	if( m_pCurrEditor )
	{
		CharKeyProc( hWnd, msg, wParam, lParam );

		m_bIME = TRUE;
		if( g_ChatIME.MessageProc( msg, wParam, lParam ) )
		{
			CharacterCommand();

			EditTextUpdate();

			bProc = TRUE;
		} //if
	}
	else
	{
		m_bIME = FALSE;

	/*	if( 
			msg == WM_IME_STARTCOMPOSITION	|| 
			msg == WM_IME_COMPOSITION		||
			msg == WM_IME_NOTIFY
			)
		{
			bProc = TRUE;
		} //if*/
	} //if..else

	return bProc;
} //CW3DUInterface::IMEProc

/**
 * 현재 ime 입력 상태인지 체크.
 */
BOOL CW3DUInterface::IsIMEInput()
{
	return m_bIME;
} //CW3DUInterface::IsIMEInput

/**
 * 문자 입력 중에 처리할 키 처리
 */
void CW3DUInterface::CharKeyProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( msg != WM_KEYDOWN )
	{
		return;
	} //if

	Assert( m_pCurrEditor );

	switch( wParam )
	{
	case VK_TAB:
		TabKey();
		break;

	case 0x26:					// Up Arrow
		{
			char*		pText = m_pCurrEditor->GetText();
			if( m_pCurrEditor && 
				m_pCurrEditor->m_strName != "EDT_CHATINPUT" )
			{
				break;
			} //if

			if( pText[0] == CHAR_WHISPER )
			{
				if( ++m_nWNameIndex > m_listWhisperName.size() - 1 )
				{
					m_nWNameIndex = 0;
				} //if
				WhisperNameScroll( pText );
			}
			else
			{
				break;
			} //if..else
		}
		break;

	case 0x28:					// Down Arrow
		{
			char*		pText = m_pCurrEditor->GetText();
			if( m_pCurrEditor && 
				m_pCurrEditor->m_strName != "EDT_CHATINPUT" )
			{
				break;
			} //if

			if( pText[0] == CHAR_WHISPER )
			{
				if( --m_nWNameIndex < 0 )
				{
					m_nWNameIndex = m_listWhisperName.size() - 1;
				} //if
				WhisperNameScroll( pText );
			}
			else
			{
				break;
			} //if..else
		}
		break;

	case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: case 0x38: case 0x39: // Num 0 ~ Num 9
		if( m_pInput->GetKeyDown( DIK_LCONTROL ) )
		{
			int		nIndex = wParam - 0x30;
			Assert( nIndex >= 0 && nIndex < 10 );

			InputChattKeySaveString( nIndex );
		} //if
		break;
	} //switch
} //CW3DUInterface::CharKeyProc

/**
 * 윈도우 메세지 키 처리.
 */
BOOL CW3DUInterface::QuickKeyProc()
{
	if( m_sModeState != "[GAME_MODE]" )
	{
		return FALSE;
	} //if

	static DWORD	s_dwFuncKeyPress = 0;

	if( m_pInput->GetKeyDown( DIK_F1 ) )
	{
		m_btQuickBarNum = 0;
		if( !(s_dwFuncKeyPress & 0x00000001) )	ShowQuickBar();
		s_dwFuncKeyPress |= 0x00000001;
	}
	else if( m_pInput->GetKeyDown( DIK_F2 ) )
	{
		m_btQuickBarNum = 1;
		if( !(s_dwFuncKeyPress & 0x00000002) )	ShowQuickBar();
		s_dwFuncKeyPress |= 0x00000002;
	}
	else if( m_pInput->GetKeyDown( DIK_F3 ) )
	{
		m_btQuickBarNum = 2;
		if( !(s_dwFuncKeyPress & 0x00000004) )	ShowQuickBar();
		s_dwFuncKeyPress |= 0x00000004;
	}
	else if( m_pInput->GetKeyDown( DIK_F4 ) )
	{
		m_btQuickBarNum = 3;
		if( !(s_dwFuncKeyPress & 0x00000008) )	ShowQuickBar();
		s_dwFuncKeyPress |= 0x00000008;
	}
	else if( m_pInput->GetKeyDown( DIK_F5 ) )
	{
		m_btQuickBarNum = 4;
		if( !(s_dwFuncKeyPress & 0x00000010) )	ShowQuickBar();
		s_dwFuncKeyPress |= 0x00000010;
	}
	else if( m_pInput->GetKeyDown( DIK_F6 ) )
	{
		m_btQuickBarNum = 5;
		if( !(s_dwFuncKeyPress & 0x00000020) )	ShowQuickBar();
		s_dwFuncKeyPress |= 0x00000020;
	}
	else if( m_pInput->GetKeyDown( DIK_F7 ) )
	{
		m_btQuickBarNum = 6;
		if( !(s_dwFuncKeyPress & 0x00000040) )	ShowQuickBar();
		s_dwFuncKeyPress |= 0x00000040;
	}
	else if( m_pInput->GetKeyDown( DIK_F8 ) )
	{
		m_btQuickBarNum = 7;
		if( !(s_dwFuncKeyPress & 0x00000080) )	ShowQuickBar();
		s_dwFuncKeyPress |= 0x00000080;
	}
	else if( m_pInput->GetKeyDown( DIK_1 ) )
	{
		if( !(s_dwFuncKeyPress & 0x00000100) )
		{
			UseSkill( 0 );			
		//	SetAttackMonster( TRUE );
		}
		s_dwFuncKeyPress |= 0x00000100;		
	}
	else if( m_pInput->GetKeyDown( DIK_2 ) )
	{
		if( !(s_dwFuncKeyPress & 0x00000200) )	
		{
			UseSkill( 1 );
		//	SetAttackMonster( TRUE );
		}
		s_dwFuncKeyPress |= 0x00000200;		
	}
	else if( m_pInput->GetKeyDown( DIK_3 ) )
	{
		if( !(s_dwFuncKeyPress & 0x00000100) )	
		{
			UseSkill( 2 );
		//	SetAttackMonster( TRUE );
		}
		s_dwFuncKeyPress |= 0x00000100;
	}
	else if( m_pInput->GetKeyDown( DIK_4 ) )
	{
		if( !(s_dwFuncKeyPress & 0x00000400) )	
		{
			UseSkill( 3 );
		//	SetAttackMonster( TRUE );
		}
		s_dwFuncKeyPress |= 0x00000400;
	}
	else if( m_pInput->GetKeyDown( DIK_5 ) )
	{
		if( !(s_dwFuncKeyPress & 0x00000800) )		
		{
			UseSkill( 4 );
		//	SetAttackMonster( TRUE );
		}
		s_dwFuncKeyPress |= 0x00000800;
	}
	else if( m_pInput->GetKeyDown( DIK_6 ) )
	{
		if( !(s_dwFuncKeyPress & 0x00001000) )		
		{
			UseSkill( 5 );
		//	SetAttackMonster( TRUE );
		}
		s_dwFuncKeyPress |= 0x00001000;
	}
	else if( m_pInput->GetKeyDown( DIK_7 ) )
	{
		if( !(s_dwFuncKeyPress & 0x00002000) )		
		{
			UseSkill( 6 );
		//	SetAttackMonster( TRUE );
		}
		s_dwFuncKeyPress |= 0x00002000;
	}
	else if( m_pInput->GetKeyDown( DIK_8 ) )
	{
		if( !(s_dwFuncKeyPress & 0x00004000) )		
		{
			UseSkill( 7 );
		//	SetAttackMonster( TRUE );
		}
		s_dwFuncKeyPress |= 0x00004000;
	}
	else if( m_pInput->GetKeyDown( DIK_SPACE ) )
	{
		if( !(s_dwFuncKeyPress & 0x00008000) )	PushEvent( "CAMERA", EVENT_CAMERA, CAMERA_DEFAULT, 0 );
		s_dwFuncKeyPress |= 0x00008000;
	}
	else
	{
		if( s_dwFuncKeyPress )					PushEvent( "", EVENT_KEYUP, 0, 0 );
		s_dwFuncKeyPress = 0;

		return FALSE;
	} //if..else

	return TRUE;
} //CW3DUInterface::QuickKeyProc

/**
 * 단축 키 처리.
 */
BOOL CW3DUInterface::HotKey()
{
	//BOOL CW3DUInterface::PopUpKeyProc();

	if( QuickKeyProc() )
	{
		return TRUE;
	} //if

	static BOOL		s_bKeyState[23];

	BOOL		bKeyProc = FALSE;
	// 'ESC'
	if( m_pInput->GetKeyDown( DIK_ESCAPE ) && !s_bKeyState[0] )
	{
		if( m_sModeState != "[GAME_MODE]" )
		{
			SetCharInfoEmptyButton( TRUE );
		} //if
		HideActiveDlg();
		PushEvent( "UI_KEYDOWN", EVENT_KEYDOWN, DIK_ESCAPE, 1 );
		bKeyProc = TRUE;
	}
	s_bKeyState[0] = m_pInput->GetKeyDown( DIK_ESCAPE );

	if( m_sModeState == "[GAME_MODE]" )
	{
		// 'TAB'
		if( m_pInput->GetKeyDown( DIK_TAB ) && !s_bKeyState[1])
		{
			ShowDlg( "DLG_MINIMAP", !IsShowDlg( "DLG_MINIMAP" ) );
			bKeyProc = TRUE;
		}
		s_bKeyState[1] = m_pInput->GetKeyDown( DIK_TAB );

		// 'Ctrl'
		if( m_pInput->GetKeyDown( DIK_LMENU ) && !s_bKeyState[2] )
		{
			PushEvent( "UI_KEYDOWN", EVENT_KEYDOWN, DIK_LMENU, 1 );
			bKeyProc = TRUE;
		}
		else if( !m_pInput->GetKeyDown( DIK_LMENU ) && s_bKeyState[2] )
		{
			PushEvent( "UI_KEYUP", EVENT_KEYUP, DIK_LMENU, 0 );
			bKeyProc = TRUE;
		} 
		s_bKeyState[2] = m_pInput->GetKeyDown( DIK_LMENU );

		// 'i'
		if( m_pInput->GetKeyDown( DIK_I ) && !s_bKeyState[3] )
		{
			ShowMyInventoryDlg( !IsShowDlg( "DLG_INVENTORY" ) );
			bKeyProc = TRUE;
		}
		s_bKeyState[3] = m_pInput->GetKeyDown( DIK_I );

		if( m_pInput->GetKeyDown( DIK_C ) && !s_bKeyState[5] )
		{
			ShowDlg( "DLG_CHARACTERINFO", !IsShowDlg( "DLG_CHARACTERINFO" ) );
			bKeyProc = TRUE;
		}
		s_bKeyState[5] = m_pInput->GetKeyDown( DIK_C );

		// 'o'
		if( m_pInput->GetKeyDown( DIK_O ) && !s_bKeyState[6] )
		{
			ShowDlg( "DLG_OPTION", !IsShowDlg( "DLG_OPTION" ) );
			bKeyProc = TRUE;
		}
		s_bKeyState[6] = m_pInput->GetKeyDown( DIK_O );

		// 'k'
		if( m_pInput->GetKeyDown( DIK_K ) && !s_bKeyState[7] )
		{
			ShowSkillDlg( !IsShowDlg( "DLG_SKILL" ) );
			bKeyProc = TRUE;
		}
		s_bKeyState[7] = m_pInput->GetKeyDown( DIK_K );
		
		// 'h'
		if( m_pInput->GetKeyDown( DIK_H ) && !s_bKeyState[8] )
		{
			GameHelp();
			bKeyProc = TRUE;
		}
		s_bKeyState[8] = m_pInput->GetKeyDown( DIK_H );

		// 'u'
		if( m_pInput->GetKeyDown( DIK_U ) && !s_bKeyState[9] )
		{
			bKeyProc = TRUE;
			if( !IsShowDlg( "DLG_CASH" ) )
			{
				PushEvent( "CASHITEM", EVENT_GETBILLINGITEM, 0, 0 );
			}
			else
			{
				ShowDlg( "DLG_CASH", FALSE  );
			} //if..else
		}
		s_bKeyState[9] = m_pInput->GetKeyDown( DIK_U );

#ifdef _dUMENOKOJI_QUEST_
		// 'q'
		if( m_pInput->GetKeyDown( DIK_Q ) && !s_bKeyState[10] )
		{
			ShowDlg( "DLG_MYQUEST", !IsShowDlg( "DLG_MYQUEST" ) );
			bKeyProc = TRUE;
		}
		s_bKeyState[10] = m_pInput->GetKeyDown( DIK_Q );
#endif		

		//'r'
		if( m_pInput->GetKeyDown( DIK_R ) && !s_bKeyState[11] )
		{
			ShowDlg( "DLG_RECEIVE", !IsShowDlg( "DLG_RECEIVE" ) );
			bKeyProc = TRUE;
		}
		s_bKeyState[11] = m_pInput->GetKeyDown( DIK_R );

#ifdef _dUMENOKOJI_RELOAD_
		if( m_pInput->GetKeyDown( DIK_R ) && !s_bKeyState[22] )
		{
			ReLoadUI();
			bKeyProc = TRUE;
		}
		s_bKeyState[22] = m_pInput->GetKeyDown( DIK_R );
#endif
	}

	return bKeyProc;
} //CW3DUInterface::HotKey

/**
 * 귓속말 이름 스크롤
 */
void CW3DUInterface::WhisperNameScroll( const char* szText )
{
	if( m_listWhisperName.size() )
	{
		if( sscanf( szText, "%s", m_szRecentWhisperName ) != 1 )
		{
			Assert( !"sscanf 오류" );
		}

		m_pCurrEditor->DeleteText( 1, strlen( m_szRecentWhisperName ) );

		LIST_WHISPERNAME::iterator	iterWhisperName = m_listWhisperName.begin();
		std::advance( iterWhisperName, m_nWNameIndex );

		m_pCurrEditor->InsertText( (*iterWhisperName), 1 );
		g_ChatIME.SetString( m_pCurrEditor->GetText() );
	} //if

} //CW3DUInterface::WhisperNameScroll

/**
 * 기본 컨트럴 실행.
 */
void CW3DUInterface::DlgYesExe()
{
	if( !m_pTopDlg )			return;

	int				nType;
	UICComponent*	pComp;
	if( m_pTopDlg->GetExeOkName( &nType, &pComp ) )
	{
		Assert( pComp );

		switch( nType )
		{
		case UI_PUSHBUTTON:			PushButton_LUp( pComp );			break;
		case UI_EDITTEXT:			EditBox_LDown( pComp );				break;
		}
	}
} //CW3DUInterface::DlgYesExe

/**
 * 기본 취소 컨트럴 실행.
 */
void CW3DUInterface::DlgNoExe()
{
	if( !m_pTopDlg )			return;

	int				nType;
	UICComponent*	pComp = NULL;
	if( m_pTopDlg->GetExeCancelName( &nType, &pComp ) )
	{
		Assert( pComp );

		switch( nType )
		{
		case UI_PUSHBUTTON:			PushButton_LUp( pComp );			break;
		case UI_EDITTEXT:			EditBox_LDown( pComp );				break;
		}
	}
} //CW3DUInterface::DlgNoExe

/**
 * 캐릭터 생성창 초기화.
 */
void CW3DUInterface::RefreshNewCharacter()
{
	SetText( "TXT_CHARNEW_STR",		m_CharSelInfo.GetSTR() );
	SetText( "TXT_CHARNEW_DEX",		m_CharSelInfo.GetDEX() );
	SetText( "TXT_CHARNEW_INT",		m_CharSelInfo.GetINT() );
	SetText( "TXT_CHARNEW_POINT",	m_CharSelInfo.GetPOINT() );
} //CW3DUInterface::RefreshNewCharacter

/**
 * 스킬창에 해당타입 아이템을 얻는다.
 */
SItemData* CW3DUInterface::GetItemDataInSkill( const int nItemNum )
{
	UISkillEntry*	pSkillEntry;
	SItemData*		pItemDat = NULL;

	pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_BLACK" ) );
	Assert( pSkillEntry );

	pItemDat = pSkillEntry->GetItem( nItemNum );
	if( !pItemDat )
	{
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_WHITE" ) );
		Assert( pSkillEntry );
		pItemDat = pSkillEntry->GetItem( nItemNum );
	} //if

	if( !pItemDat )
	{
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_SHORT" ) );
		Assert( pSkillEntry );
		pItemDat = pSkillEntry->GetItem( nItemNum );
	} //if

	if( !pItemDat )
	{
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_LONG" ) );
		Assert( pSkillEntry );
		pItemDat = pSkillEntry->GetItem( nItemNum );
	} //if

	if( !pItemDat )
	{
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_LIFE" ) );
		Assert( pSkillEntry );
		pItemDat = pSkillEntry->GetItem( nItemNum );
	} //if

	if( !pItemDat )
	{
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_GUILD" ) );
		Assert( pSkillEntry );
		pItemDat = pSkillEntry->GetItem( nItemNum );
	} //if

	if( !pItemDat )
	{
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_COUNTRY" ) );
		Assert( pSkillEntry );
		pItemDat = pSkillEntry->GetItem( nItemNum );
	} //if

	if( !pItemDat )
	{
		pSkillEntry = reinterpret_cast<UISkillEntry*>( GetComponent( "SKILL_SPECIAL" ) );
		Assert( pSkillEntry );
		pItemDat = pSkillEntry->GetItem( nItemNum );
	} //if
	
	return pItemDat;
} //CW3DUInterface::GetItemDataInSkill

/**
 * 문자열 체크.
 */
BOOL CW3DUInterface::IsExcepChar( const char* szString )
{
	Assert( szString );

	int			nLength = strlen( szString );
	BYTE*		pbtString = reinterpret_cast<BYTE*>( const_cast<char*>( szString ) );
	for( int nCnt = 0; nCnt < nLength; nCnt++ )
	{ 
		if( ( pbtString[nCnt] >= 0xB0 ) && ( pbtString[nCnt] <= 0xC8 ) &&
			( pbtString[nCnt+1] >= 0xA1 )&& ( pbtString[nCnt+1] <= 0xFE ) )
		{
			nCnt++;
		}
		else if( ( ( pbtString[nCnt]>= 0x41 ) && ( pbtString[nCnt] <= 0x5A ) ) || 
				 ( ( pbtString[nCnt] >= 0x61 ) && ( pbtString[nCnt] <= 0x7A) ) ) 
		{ // 영어 해당 부분
			
		} 
		else if( ( pbtString[nCnt] >= '0' ) && ( pbtString[nCnt] <= '9' ) ) 
		{ // 숫자 처리 부분
		} 
		else if( pbtString[nCnt] == 0x39 )
		{ // 스페이스 바
			return TRUE;
		}
/*#ifndef _CHINAVERSION
		else if( ( pbtString[nCnt] >= 0xCA ) && ( pbtString[nCnt] <= 0xFD ) && 
				 ( pbtString[nCnt+1] >= 0xA1 ) && ( pbtString[nCnt+1] <= 0xFE) ) 
		{ // 한자 해당 부분 
			nCnt++; 
			return TRUE;
		} 
		else if( ( pbtString[nCnt] >= 0xA1 ) && ( pbtString[nCnt+1] >= 0xA1 ) ) 
		{ // 2byte special char
			nCnt++; 
			return TRUE;
		} 
		else 
		{ // 1byte specia character
			return TRUE;
		} 
#endif //_CHINAVERSION*/
	} 

	return FALSE;
} //CW3DUInterface::IsExcepChar

/**
 * 사운드 상태 설정.
 */
void CW3DUInterface::SetSoundState( BOOL bOnBGM, int nVolBGM, BOOL bOnEff, int nVolEff )
{
	UIScrollBar*	pScrollEff = reinterpret_cast<UIScrollBar*>( GetComponent( "SCR_SOUND_EFFECT" ) );
	UIScrollBar*	pScrollBGM = reinterpret_cast<UIScrollBar*>( GetComponent( "SCR_SOUND_BGM" ) );
	if( !pScrollEff || !pScrollBGM )
	{
		Assert( !"콤프없다" );
		return;
	} //if
	pScrollEff->SetScrollPos( nVolEff );
	pScrollBGM->SetScrollPos( nVolBGM );

	UICheckButton*	pCheckEff = reinterpret_cast<UICheckButton*>( GetComponent( "CHK_SOUND_EFFECT" ) );
	UICheckButton*	pCheckBGM = reinterpret_cast<UICheckButton*>( GetComponent( "CHK_SOUND_BGM" ) );
	if( !pCheckEff || !pCheckBGM )
	{
		Assert( !"콤프없다" );
		return;
	} //if
	pCheckBGM->Check( bOnBGM );
	pCheckEff->Check( bOnEff );
} //CW3DUInterface::SetSoundState

/**
 * 문자 코멘드 처리
 */
BOOL CW3DUInterface::CharacterCommand()
{
	if( !m_pCurrEditor )
	{
		return FALSE;
	} //if

	char*	pText   = g_ChatIME.GetString();
	int		nLength = strlen( pText );
	if( nLength <= 0 && nLength > 63 )
	{
		return FALSE;
	} //if

	if( m_pCurrEditor )
	{
		if( pText[0] == CHAR_WHISPER )
		{ // 쳇 코멘드 처리
			static char		szName[64] = "";
			if( sscanf( pText, "%s", szName ) != 1 )
			{
				Assert( !"sscanf 오류" );
			}

			if( m_pCurrEditor->m_strName == "EDT_CHATINPUT" )
			{
				char*	pEditText = m_pCurrEditor->GetText();
				if( strlen( szName ) == 1		&& 
					m_listWhisperName.size()	&&
					strlen( pEditText ) == 0 )
				{
					LIST_WHISPERNAME::iterator	iterWhisperName = m_listWhisperName.begin();
					
					strcpy( &szName[1], (*iterWhisperName) );
					strcat( szName, " " );
					g_ChatIME.SetString( szName );
					
					m_nWNameIndex = 0;

					return TRUE;
				} //if
			} //if
		} //if
	} //if

	return FALSE;
} //CW3DUInterface::CharacterCommand

/**
 * 귓속말 이름 저장
 */
BOOL CW3DUInterface::SaveChattingText()
{
	if( !m_pCurrEditor )
	{
		return FALSE;
	} //if
	
	char*	pText = m_pCurrEditor->GetText();

	if( pText[0] == CHAR_WHISPER )
	{
		char		szName[MAX_CHATTSTRING];
		if( sscanf( &pText[1], "%s", szName ) != 1 )
		{
			Assert( !"sscanf 오류" );
		}

		strcpy( m_szRecentWhisperName, szName );
		m_pCurrEditor->DeleteText( 0, strlen( szName ) + 2 );			// ' " ' 문자와 빈칸을 포함해서 2.
		SaveWhisperName( szName );
	}
	else if( pText[0] == CHAR_SAVEWORD )
	{
		int		nIndex = pText[1] - 0x30;
		if( nIndex >= 0 && nIndex < 10 && pText[2] == ' ')
		{ // character '0' ~ '9' and ' '
			strcpy( m_aszWordsSave[nIndex], pText + 3 );
			return FALSE;
		} //if
	}
	else
	{
		m_szRecentWhisperName[0] = '\0';
	} //if..else..

	if( strlen( pText ) > MAX_CHATTSTRING )
	{
		strncpy( m_szRecentChattText, pText, MAX_CHATTSTRING );
	}
	else
	{
		strcpy( m_szRecentChattText, pText );
	} //if..else

	return TRUE;
} //CW3DUInterface::SaveChattingText

/**
 * Update Pet
 */
void CW3DUInterface::UpdatePet( 
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
	)
{
	Assert( szName );

	char	szBuff[32];

	SetText( "EDT_PET_NAME", szName );
	
	if( byType == 1 ) SetText( "TXT_PET_TYPE", STR_PET_TYPE1 );
	else if( byType == 2 ) SetText( "TXT_PET_TYPE", STR_PET_TYPE2 );
	else if( byType == 3 ) SetText( "TXT_PET_TYPE", STR_PET_TYPE3 );
	else Assert( "No Pet Type" );
	
	SetText( "TXT_PET_LV", nLevel );
	sprintf( szBuff, "%d/%d", nExp, nNeedExp );
	SetText( "TXT_PET_EXP", szBuff );

	sprintf( szBuff, "%d/%d", wHp, wMaxHp );
	SetText( "TXT_PET_HP", szBuff );
	sprintf( szBuff, "%d/%d", wMp, wMaxMp );
	SetText( "TXT_PET_MP", szBuff );

	sprintf( szBuff, "%d/%d", wMinAp, wMaxAp );
	SetText( "TXT_PET_AP", szBuff );
	SetText( "TXT_PET_DP", wDp );

	SetProgressPos( "PRO_PET_HP",	wMaxHp,   wHp,  TRUE );
	SetProgressPos( "PRO_PET_MP",	wMaxMp,	  wMp,  TRUE );
	SetProgressPos( "PRO_PET_EXP",	nNeedExp, nExp, TRUE );

	UIPicture*		pPic = reinterpret_cast<UIPicture*>( GetComponent( "PIC_PET_FACE" ) );
	if( !pPic )
	{
		Assert( !"Not Found Picture" );
		return;
	} //if

	BOOL		bLoadResult;
	switch( byType )
	{
	case 1:		bLoadResult = pPic->Load( "Interface/pet_info_01.bmp", 40, 40 );	break;
	case 2:		bLoadResult = pPic->Load( "Interface/pet_info_02.bmp", 40, 40 );	break;	
	case 3:		bLoadResult = pPic->Load( "Interface/pet_info_03.bmp", 40, 40 );	break;
	case 4:		bLoadResult = pPic->Load( "Interface/pet_info_04.bmp", 40, 40 );	break;
	} //switch

	if( !bLoadResult )
	{
		Assert( !"Failed Load Sprite" );
		return;
	} //if
} //CW3DUInterface::UpdatePet