/**
 * 유저 인터페이스 다이얼로그 클래스.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-11-16 7:11p $
 * @version	$Revision: 29 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIDialog.cpp $
 * 
 * 29    04-11-16 7:11p Sk8snow2
 * 
 * 28    04-06-26 8:59p Paran2
 * 
 * 27    04-06-25 5:43p Paran2
 * 
 * 26    04-06-10 5:33p Paran
 * 
 * 25    04-02-13 10:48a Paran
 * 
 * 24    03-12-12 11:58a Paran
 * 
 * 23    03-12-08 8:50p Paran
 * 
 * 22    03-12-04 4:39p Paran
 * 
 * 21    03-10-10 7:28p Paran
 * 
 * 4     03-05-23 3:42p Paran		버그수정 중...	스킨에니
 * 
 * 3     03-05-20 9:35p Paran		계속 수정 중...
 * 
 * 2     03-05-17 4:19a Paran		각 콤포넌트 분리.
 * 
 * 67    03-04-11 5:28p Paran		캐릭터 장착 부분과 미니맵 수정.
 *
 * 66    03-04-10 6:24p Paran		미니맵 추가.
 *
 * 65    03-04-04 3:37p Paran		UICharSelect 분리.
 *
 * 64    03-04-02 9:52p Paran		기본 아이템 추가.
 *
 * 63    03-04-02 8:11p Paran		플레이어 아이템추가, 메세지 박스 키 입력 추가.
 *
 * 61    03-03-26 9:19p Paran		인벤토리 어색한 부분 수정하고 체크.
 *
 * 50    03-03-06 11:16p Paran		인벤토리 마우스 기능 변경과 캐릭터 선택 부분 정보 삭제 처리.
 * <HR>
 *
 * @file	UIDialog.cpp
 */

#include "UIDialog.h"

#define HEIGHT_TITLEBAR			21

/**
 * 생성자.
 */
UIDialog::UIDialog()
{
	m_dwCurTick			= 0;
	m_dwShowTick		= 0;
	m_bPush				= FALSE;
	m_pSprite			= NULL;
	m_pRectangle		= NULL;
	m_pTitle			= NULL;
	m_bShow				= TRUE;
	m_nNumRepeatDraw	= 0;
	m_bTopMost			= FALSE;
	m_bAlways			= FALSE;
	m_szTitleName[0]	= '\0';
	m_pFont				= NULL;
	SetRect( &m_rcTitleBar, 0, 0, 0, 0 );
} //UIDialog::UIDialog

/**
 * 소멸자.
 */
UIDialog::~UIDialog()
{
	LIST_PUICOMP::iterator		listIter;

	for( listIter = m_listComp.begin() ; listIter != m_listComp.end() ; listIter++ )
	{
		SAFE_RELEASE( *listIter );
	} //for
	m_listComp.clear();

	SAFE_RELEASE( m_pSprite );
	SAFE_RELEASE( m_pRectangle );
	SAFE_RELEASE( m_pTitle );
} //UIDialog::~UIDialog

/**
 *
 */
void UIDialog::Cleanup()
{
	UICComponent::Cleanup();

	Show( FALSE );

	UICComponent				*pComp;
	LIST_PUICOMP::iterator		listIter;
	for( listIter = m_listComp.begin(); listIter != m_listComp.end(); listIter++ )
	{
		pComp = (*listIter);
		pComp->Cleanup();
	} //for
} //UIDialog::Cleanup

/**
 *
 */
BOOL UIDialog::Load( const char* szFileName )
{
	if( !szFileName )
	{
		return TRUE;
	} //if

	int		nWidth  = m_rcRegion.right - m_rcRegion.left;
	int		nHeight = m_rcRegion.bottom - m_rcRegion.top;

	if( m_pTitle )
	{
		nHeight -= HEIGHT_TITLEBAR;
	} //if

	m_pSprite = MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_pSprite );

	if( !m_pSprite->Load( szFileName, nWidth, nHeight ) )
	{
		return FALSE;
	} //if

	return TRUE;
} //UIDialog::Load

/**
 * 사각그리기 초기화.
 */
BOOL UIDialog::InitRectangle()
{
	SAFE_RELEASE( m_pRectangle );
	m_pRectangle = MAKE_COMPONENT( Rectangle );
	if( !m_pRectangle )
	{
		Assert( m_pRectangle );
		return FALSE;
	} //if

	return TRUE;
} //UIDialog::InitRectangle

/**
 * 푸쉬.
 */
void UIDialog::Push( int bPush_ )
{
	if( !m_bEnable )
	{
		return;
	}

	m_bPush = bPush_; 
} //UIDialog::Push

/**
 * 작업 사각 영역.
 */
RECT UIDialog::GetWorkRect()
{
	RECT	rcWork = m_rcRegion;
	if( m_pTitle )
	{
		rcWork.top		+= HEIGHT_TITLEBAR;
	} //if
	return rcWork;
} //UIDialog::GetWorkRect

/**
 * 타이틀 바 인
 */
BOOL UIDialog::PtInTitleBar( const int nX, const int nY )
{
	POINT	ptPos = { nX, nY };
	if( PtInRect( &m_rcTitleBar, ptPos ) )
	{
		return TRUE;
	} //if
	return FALSE;
} //UIDialog::PtInTitleBar

/**
 * 창 이동.
 */
void UIDialog::Move( int nOffsetX, int nOffsetY )
{
	if( IsAlways() )		return;

	UICComponent::Move( nOffsetX, nOffsetY );

	m_rcTitleBar.left	+= nOffsetX;
	m_rcTitleBar.top	+= nOffsetY;
	m_rcTitleBar.right	+= nOffsetX;
	m_rcTitleBar.bottom	+= nOffsetY;

	UICComponent*	pChild;
	LIST_PUICOMP::iterator	iterList_Child;
	iterList_Child = m_listComp.begin();
	while( iterList_Child != m_listComp.end() )
	{
		pChild = *iterList_Child;
		Assert( pChild );
		pChild->Move( nOffsetX, nOffsetY );
		iterList_Child++;
	}
} //UIDialog::Move

/**
 * 위치지정.
 */
void UIDialog::SetPos( const int nLeft, const int nTop )
{
	int		nOffsetX = m_rcRegion.left - nLeft;
	int		nOffsetY = m_rcRegion.top - nTop;

	Move( nOffsetX, nOffsetY );
} //UIDialog::SetPos

/**
 * 기본 버튼을 얻는다.
 */
BOOL UIDialog::GetExeOkName( int* pnType, UICComponent** ppComp )
{
	static char		szCompName[4][64] =
	{
		"YES",
		"OK",
		"NO",
		"CANCEL"
	}; //static char		szCompName

	UICComponent*	pChild = NULL;
	LIST_PUICOMP::iterator	iterList_Child;

	for( int nCnt = 0; nCnt < 2; nCnt++ )
	{ //긍정,부정
		iterList_Child = m_listComp.begin();
		while( iterList_Child != m_listComp.end() )
		{
			pChild = *iterList_Child;
			Assert( pChild );
			if( pChild->m_nType == UI_PUSHBUTTON &&
				pChild->IsShow() )
			{
				int		nFindNum;
				nFindNum  = pChild->m_strName.find( szCompName[nCnt*2] );
				nFindNum += pChild->m_strName.find( szCompName[(nCnt*2)+1] );
				if( nFindNum >= 0 )
				{
					*pnType = UI_PUSHBUTTON;
					*ppComp = pChild;
					return TRUE;
				} //if
			} //if
			iterList_Child++;
		} //while
	} //for

	return FALSE;
} //UIDialog::GetExeOkName

/**
 * 취소버튼을 얻는다.
 */
BOOL UIDialog::GetExeCancelName( int* pnType, UICComponent** ppComp )
{
	static char		szCompName[4][64] =
	{
		"NO",
		"CANCEL",
		"YES",
		"OK"
	}; //static char		szCompName

	UICComponent*	pChild = NULL;
	LIST_PUICOMP::iterator	iterList_Child;

	for( int nCnt = 0; nCnt < 2; nCnt++ )
	{ //긍정,부정
		iterList_Child = m_listComp.begin();
		while( iterList_Child != m_listComp.end() )
		{
			pChild = *iterList_Child;
			Assert( pChild );
			if( pChild->m_nType == UI_PUSHBUTTON &&
				pChild->IsShow() )
			{
				int		nFindNum;
				nFindNum  = pChild->m_strName.find( szCompName[nCnt*2] );
				nFindNum += pChild->m_strName.find( szCompName[(nCnt*2)+1] );
				if( nFindNum >= 0 )
				{
					*pnType = UI_PUSHBUTTON;
					*ppComp = pChild;
					return TRUE;
				} //if
			} //if
			iterList_Child++;
		} //while
	} //for

	return FALSE;
} //UIDialog::GetExeCancelName

/**
 * 컨트럴 추가.
 */
void UIDialog::AddControl( UICComponent* pComponent )
{
	m_listComp.push_back( pComponent );
} //UIDialog::AddControl

/**
 * 컨트럴 삭제.
 */
BOOL UIDialog::DeleteControl( UICComponent* pComponent )
{
	UICComponent				*pTempComp = NULL;
	LIST_PUICOMP::iterator		iterList_Comp = m_listComp.begin();

	while( iterList_Comp != m_listComp.end() )
	{
		pTempComp = *iterList_Comp;

		if( pTempComp->m_strName == pComponent->m_strName )
		{
			iterList_Comp = m_listComp.erase( iterList_Comp );
			m_iterList_Component = iterList_Comp;
			return TRUE;
		} //if

		iterList_Comp++;
	} //while

	return FALSE;
} //UIDialog::DeleteControl

/**
 * 첫 컨트럴을 얻는다.
 */
UICComponent* UIDialog::GetFirstComponent()
{
	m_iterList_Component = m_listComp.begin();

	if( m_iterList_Component != m_listComp.end() )
	{
		return *m_iterList_Component;
	} //if

	return NULL;
} //UIDialog::GetFirstComponent

/**
 * 다음 컨트럴을 얻는다.
 */
UICComponent* UIDialog::GetNextComponent()
{
	m_iterList_Component++;

	if( m_iterList_Component != m_listComp.end() )
	{
		return *m_iterList_Component;
	} //if

	return NULL;
} //UIDialog::GetNextComponent

/**
 * 컨트럴을 찾아 줍니다.
 */
UICComponent* UIDialog::FindComponent( const char* szName )
{
	UICComponent*			pChild;
	LIST_PUICOMP::iterator	iterList_Comp = m_listComp.begin();
	while( iterList_Comp != m_listComp.end() )
	{
		pChild = (*iterList_Comp);
		if( !pChild->m_strName.compare( szName ) )
		{
			return pChild;
		} //if
		iterList_Comp++;
	} //while
	return NULL;
} //UIDialog::FindComponent

/**
 * 콤포넌트들을 정렬 ( 같은 크기일때 잘 동작한다)
 */
void UIDialog::SortComp()
{
	UICComponent*			pChild;
	LIST_PUICOMP::iterator	iterList_Comp = m_listComp.begin();

	int			nX = 0;
	int			nY = 0;
	int			nDlgWidth  = m_rcRegion.right - m_rcRegion.left;
	int			nDlgHeight = m_rcRegion.bottom - m_rcRegion.top;

	BOOL		bShow = TRUE;

	while( iterList_Comp != m_listComp.end() )
	{
		pChild = (*iterList_Comp);
		if( !bShow )
		{
			pChild->Show( FALSE );
			continue;
		} //if

		SetPos( nX, nY );
		Move( m_rcRegion.left, m_rcRegion.top );

		int		nWidth  = pChild->m_rcRegion.right - pChild->m_rcRegion.left;
		int		nHeight = pChild->m_rcRegion.bottom - pChild->m_rcRegion.top;
		nX += nWidth;

		if( nX > nDlgWidth )
		{
			nX = 0;
			nY += nHeight;
			if( nY > nDlgHeight )
			{
				bShow = FALSE;			
			} //if
		} //if

		iterList_Comp++;
	} //while
} //if

/**
 * 현재 위치 차일드 컨트럴을 얻는다.
 */
UICComponent* UIDialog::GetFocus( int nX, int nY )
{
	if( m_listComp.empty() )
	{
		return NULL;
	} //if

	LIST_PUICOMP::iterator	itor = m_listComp.end();
	while( 1 )
	{
		itor--;

		UICComponent*	pComp = *itor;
		Assert( pComp );

		if( pComp->PointIn( nX, nY ) )
		{
			return pComp;
		} //if

		if( itor == m_listComp.begin() )
		{
			break;
		} //if
	} //if

	return NULL;
} //UIDialog::GetFocus

/**
 * 최상위 설정.
 */
void UIDialog::SetTopMost( BOOL bTop )
{
	m_bTopMost = bTop;
} //UIDialog::SetTopMost

/**
 * 최상위 여부.
 */
BOOL UIDialog::IsTopMost()
{
	return m_bTopMost;
} //UIDialog::IsTopMost

/**
 * 항상 설정.
 */
void UIDialog::SetAlways( BOOL bAlways )
{
	m_bAlways = bAlways;
} //UIDialog::SetAlways

/**
 * 항상 여부.
 */
BOOL UIDialog::IsAlways()
{
	return m_bAlways;
} //UIDialog::IsAlways

/**
 * 보이기.
 */
void UIDialog::Show( const BOOL bShow )
{
	UICComponent::Show( bShow );

	LIST_PUICOMP::iterator		listIter;
	for( listIter = m_listComp.begin(); listIter != m_listComp.end(); listIter++ )
	{
		(*listIter)->Show( bShow );
	} //for
} //UIDialog::Show

/**
 * 타이틀 이름 설정.
 */
void UIDialog::SetTitleBar( const char* szName, const IW3DFont* pFont )
{
	Assert( pFont );
	
	m_pFont = const_cast<IW3DFont*>( pFont );
	strcpy( m_szTitleName, szName );

	m_pTitle = MAKE_COMPONENT( Sprite );
	if( !m_pTitle )
	{
		Assert( !"이미지 로드 실패" );
		return;
	} //if

	m_rcTitleBar.left	= m_rcRegion.left;
	m_rcTitleBar.top	= m_rcRegion.top;
	m_rcTitleBar.bottom	= m_rcTitleBar.top + 21;

	int		nTitleW = m_rcRegion.right - m_rcRegion.left;
	if( 174 == nTitleW )
	{
		if( !m_pTitle->Load( "interface/bar-1.tga", 174, HEIGHT_TITLEBAR ) )
		{
			Assert( !"이미지 로드 실패" );
			return;
		} //if
		m_rcTitleBar.right	= m_rcTitleBar.left + 174;
	}
	else if( 215 == nTitleW )
	{
		if( !m_pTitle->Load( "interface/bar-l-1.tga", 215, HEIGHT_TITLEBAR ) )
		{
			Assert( !"이미지 로드 실패" );
			return;
		} //if
		m_rcTitleBar.right	= m_rcTitleBar.left + 215;
	}


	else if( 260 == nTitleW )
	{
		if( !m_pTitle->Load( "interface/bar-1-2.tga", 260, HEIGHT_TITLEBAR ) )
		{
			Assert( !"이미지 로드 실패" );
			return;
		}
		m_rcTitleBar.right = m_rcTitleBar.left + 260;
	}
#ifdef _dUMENOKOJI_QUEST_	
	else
	{
		if( !m_pTitle->Load( "interface/bar-1-3.tga", 415, HEIGHT_TITLEBAR ) )
		{
			Assert( !"이미지 로드 실패" );
			return;
		} //if
		m_rcTitleBar.right	= m_rcTitleBar.left + 415;		
	} 
#else
	
	else
	{
		if( !m_pTitle->Load( "interface/bar-1-3.tga", 415, HEIGHT_TITLEBAR ) )
		{
			Assert( !"이미지 로드 실패" );
			return;
		} //if
		m_rcTitleBar.right	= m_rcTitleBar.left + 415;		
	} //if..else
#endif
} //UIDialog::SetTitleBar

//
// UIDialog::SetTitleName
//
void UIDialog::SetTitleName( const char* szTitleName )
{
	strcpy( m_szTitleName, szTitleName );
} //UIDialog::SetTitleName

//
// UIDialog::SetRepeatNum
//
void UIDialog::SetRepeatNum( int nReNum )
{
	int			nHeight = m_rcRegion.bottom - m_rcRegion.top;
	m_rcRegion.top	 += ( m_nNumRepeatDraw * nHeight );
	m_nNumRepeatDraw = nReNum;
	m_rcRegion.top	 -= ( m_nNumRepeatDraw * nHeight );
	m_rcRegion.bottom = m_rcRegion.top + nHeight;
}

//
// UIDialog::GetRepeatNum
//
int UIDialog::GetRepeatNum()
{
	return m_nNumRepeatDraw;
}

//
// UIDialog::GetRegion
//
const RECT* UIDialog::GetRegion()
{
	static RECT		rcRegion;
	int				nHeight = m_rcRegion.bottom - m_rcRegion.top;

	rcRegion.left	= m_rcRegion.left;
	rcRegion.top	= m_rcRegion.top;
	rcRegion.right	= m_rcRegion.right; 
	rcRegion.bottom = m_rcRegion.top + ( nHeight * m_nNumRepeatDraw ) + nHeight;

	return &rcRegion;
}

/**
 * 보여줄 시간.
 */
void UIDialog::SetShowTick( const DWORD dwShowTick )
{
	m_dwShowTick = dwShowTick;
	m_dwCurTick = 0;
} //

/**
 * 업데이트.
 */
void UIDialog::Update( const DWORD dwTick )
{
	LIST_PUICOMP::iterator		listIter;
	for( listIter = m_listComp.begin(); listIter != m_listComp.end(); listIter++ )
	{
		(*listIter)->Update( dwTick );
	} //for
	
	m_dwCurTick += dwTick;
	if( m_dwShowTick > 0 )
	{
		if( m_dwCurTick > m_dwShowTick )
		{
			m_bShow = FALSE;
		} //if
	} //if
} //UIDialog::Update

//
// UIDialog::Draw
//
void UIDialog::Draw()
{
	if( !IsShow() )
	{
		return;
	} //if

	int		nX = m_rcRegion.left;
	int		nY = m_rcRegion.top;

	if( m_pTitle )
	{
		m_pTitle->Draw( nX, nY );
		m_pFont->DrawText( m_szTitleName, &m_rcTitleBar, DT_CENTER | DT_VCENTER, 0xFFC2C2C2 );

		nY += HEIGHT_TITLEBAR;
	} //if

	if( m_pSprite )
	{ // 이미지.
		m_pSprite->Draw( nX, nY );
	}
	else if( m_pRectangle )
	{ //사각형
		int		nWidth  = m_rcRegion.right - m_rcRegion.left;
		int		nHeight = m_rcRegion.bottom - m_rcRegion.top;
		m_pRectangle->Draw( m_rcRegion.left, m_rcRegion.top, nWidth, nHeight, COLOR_ARGB( 180, 0, 0, 0 ) );//RECTANGLE_COLOR );
	} //if..else

	UICComponent*		pComp;
	LIST_PUICOMP::iterator		listIter = m_listComp.begin();
	while( listIter != m_listComp.end() )
	{
		pComp = (*listIter);
		pComp->Draw();
		listIter++;
	} //for
}