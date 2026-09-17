/**
 * 아이템.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-11-16 7:11p $
 * @version	$Revision: 38 $
 *
 * <HR>
 * $Log: /Engine/Src/W3DItem.cpp $
 * 
 * 38    04-11-16 7:11p Sk8snow2
 * 
 * 37    04-01-06 12:01a Kjmgo
 * 버그 수정및, 코드 정리
 * 
 * 36    03-12-31 2:44p Paran
 * 스킬
 * 
 * 34    03-12-20 9:08p Kjmgo			버그수정 ㅠ.ㅜ
 * 
 * 33    03-12-20 6:21a Kjmgo			메모리 관리
 * 
 * 32    03-12-12 11:57a Paran			item.dat 로드 수정
 * 
 * 31    03-12-09 4:01p Paran			아이템 데이터 로드수정
 * 
 * 30    03-10-27 3:24p Kjmgo			코드 최적화
 * 
 * 29    03-10-15 4:57p Kjmgo			바운드 박스만 , 반지름과, 피봇을 구합니다.
 * 
 * 26    03-08-30 3:35p Kjmgo			Scene쪽 관련 포인터 정리
 * 
 * 21    03-06-28 10:16a Kjmgo			Assert 추가
 * 
 * 17    03-06-23 5:53p Paran			바운드 박스 처리.
 * 
 * 15    03-06-03 5:33p Paran			아이템 모델 파일 로드 부분 수정
 *
 * <HR>
 *
 * @file	W3DItem.cpp
 */


#include "Engine_Export.h"
#include <string>
#include <map>
//#define SHOW_BOUNDBOX

typedef std::map<DWORD, IW3DModel*>	ModelMap;

static ModelMap	g_mapItemModel;
static DWORD	g_dwItemCount = 0;
static IW3DModel* g_pItemBoundBox = NULL;

/**
 * 아이템 클래스.
 * @ingroup		Engine
 */
class CW3DItem : public IW3DItem
{
public:
	UNDER_MEMORY_MGR( Item );

	CW3DItem();
	~CW3DItem();

	BOOL	Create( IW3DWorld* pWorld, const DWORD dwId, const WORD wType, const int nX, const int nY );
	BOOL	LoadModel( const WORD wType );
	void	UnloadModel();

	BOOL	Update( DWORD dwTick );

	void	DrawName();
	void	Draw();
	void	DrawShadow();

	void	SetLighting( const BOOL bLighting );

	void	SetPosition( const int nX, const int nY );
	Vector3	GetPosition()
	{
		return m_vPos;
	} //if
	
	BOOL	Pick( const Vector3& vOrig, const Vector3& vDir );

	/**
	 * 아이템 이름을 얻어옴.
	 */
	const char*		GetName()
	{
		return m_sName.c_str();
	} //GetName
	
	/**
	 * 아이템 이름설정.
	 */
	void			SetName( const char* szName )
	{
		m_sName = szName;
	} //SetName

	/**
	 * 아이템 아이디 얻어옴.
	 */
	DWORD		GetId()
	{
		return m_dwId;
	}

	/**
	 * 아이템 타입 얻어옴.
	 */
	WORD		GetType()
	{
		return m_wType;
	}

	/**
	 * 아이템 이름 출력여부.
	 */
	void		ShowName( BOOL bShow )
	{
		m_bIsShowName = bShow;
	}


private:
	IW3DWorld*		m_pWorld;
	IW3DModel*		m_pModel;
	IW3DModel*		m_pBoundBox;		///< 바운드 박스.

	IW3DScene*		m_pScene;
	IW3DRectangle*	m_pRectangle;

	DWORD			m_dwId;
	WORD			m_wType;
	std::string		m_sName;
	Vector3			m_vPos;
	Matrix4			m_matWorld;

	DWORD			m_dwTick;
	BOOL			m_bIsShowName;
	BOOL			m_bLighting;
	BOOL			m_bView;
}; //class CW3DItem


IMPLEMENT_COMPONENT( CW3DItem, Item );


/**
 * 생성자.
 */
CW3DItem::CW3DItem()
	: m_bLighting	( TRUE ),
	  m_pWorld		( NULL ),
	  m_pModel		( NULL ),
	  m_pBoundBox	( NULL ),
	  m_dwTick	    ( 0 ),
	  m_bIsShowName ( 0 ),
	  m_pScene		( NULL ),
	  m_pRectangle	( NULL )
{
	g_dwItemCount++;
} //CW3DItem::CW3DItem

/**
 * 소멸자.
 */
CW3DItem::~CW3DItem()
{
	UnloadModel();

	g_dwItemCount--;

	if( g_dwItemCount <= 0 )
	{
		Assert( g_pItemBoundBox );
		Assert( g_mapItemModel.empty() );

		if( g_pItemBoundBox )
			Assert( g_pItemBoundBox->GetRefCount() == 1 );

	} //if
	
	if( g_pItemBoundBox && g_pItemBoundBox->GetRefCount() <= 1 )
	{
		SAFE_RELEASE( g_pItemBoundBox );
	}
	else
	{
		SAFE_RELEASE( m_pBoundBox );
	} //if..else..

	SAFE_RELEASE( m_pRectangle );
	
} //CW3DItem::~CW3DItem

/**
 * 아이템을 만든다.
 *
 * @return	생성 성공 여부.
 */
BOOL CW3DItem::Create(	IW3DWorld*		pWorld,		///< [IN] 월드 객체
						const DWORD		dwId,		///< [IN] 아이템 ID
						const WORD		wType,		///< [IN] 아이템 종류
						const int		nX,			///< [IN] 맵 상의 X 좌표
						const int		nY			///< [IN] 맵 상의 Y 좌표
						)
{
	Assert( pWorld );
	Assert( NULL == m_pModel );

	m_pScene = MAKE_COMPONENT( Scene );
	Assert( m_pScene );

	m_pRectangle = MAKE_COMPONENT( Rectangle );
	Assert( m_pRectangle );

	m_pWorld = pWorld;

	if( !LoadModel( wType ) )
	{
		return FALSE;
	} //if

	if( !g_pItemBoundBox )
	{
		g_pItemBoundBox = MAKE_COMPONENT( Model );
		CHECK_PTR_RETURN( g_pItemBoundBox );
		g_pItemBoundBox->Load( "Equip", "item_bb.mod" );
		g_pItemBoundBox->CalculateRadius();
	} 
	else
	{
		g_pItemBoundBox->AddRef();
	} //if

	m_pBoundBox = g_pItemBoundBox;
	// Load BoundBox
	
	SetPosition( nX, nY );

	m_dwId	= dwId;
	m_wType = wType;

	return TRUE;
} //CW3DItem::Create

/**
 * 아이템 모델을 읽어온다.
 */
BOOL CW3DItem::LoadModel( const WORD wType )
{
	ModelMap::iterator itor = g_mapItemModel.find( wType );

	if( itor != g_mapItemModel.end() )
	{
		m_pModel = itor->second;

		if( !m_pModel )
			return FALSE;

		m_pModel->AddRef();

		return TRUE;
	} //if

	m_pModel = MAKE_COMPONENT( Model );
	CHECK_PTR_RETURN( m_pModel );

	IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();

	std::string		sFileName;

	SItemInfoData*	pItemInfo = pItemInfoMgr->GetItemInfo( wType );
	if( !pItemInfo )
	{
		Assert( !"아이템 정보가 없다" );
		return FALSE;
	} //if

	m_sName = pItemInfo->szHName;
	sFileName = ITEMFILENAME2( pItemInfoMgr->GetSex(), pItemInfo );

	if( !m_pModel->Load( "Equip", sFileName.c_str() ) )
	{ //장비폴더에 없을시
		std::string		sFullName = ((std::string)"Sub/" ) + sFileName;
		if( !m_pModel->Load( "Body", sFullName.c_str() ) )
		{ //바디폴더안 서브폴더를
			DEBUG_OUT( "아이템 모델 읽기 실패 - " + sFileName.c_str() );
			SAFE_RELEASE( m_pModel );
			return FALSE;
		} //if
	} //if

	g_mapItemModel.insert( std::make_pair( wType, m_pModel ) );

	return TRUE;
} //CW3DItem::LoadModel

/**
 *
 */
void CW3DItem::UnloadModel()
{
	if( m_pModel && m_pModel->GetRefCount() <= 1 )
	{
		ModelMap::iterator itor = g_mapItemModel.find( m_wType );

		if( itor != g_mapItemModel.end() )
		{
			g_mapItemModel.erase( itor );
		} 
		else
		{
			Assert( !"내부 로직 문제 있음" );
		}//if
	} //if

	SAFE_RELEASE( m_pModel );

} //CW3DItem::UnloadModel

/**
 * 업데이트.
 *
 * @param	dwTick		[IN] 경과 tick
 *
 * @return	계속 존재하는지 여부
 */
BOOL CW3DItem::Update( DWORD dwTick )
{
	m_bView = FALSE;

	Frustum&	frustum = m_pScene->GetFrustum();
	if( frustum.CheckSphere( m_vPos, 1.0F ) )	
	{
		m_bView = TRUE;
	} //if

	return TRUE;
} //CW3DItem::Update

/**
 * 이름 찍기.
 */
void CW3DItem::DrawName()
{
	if( m_bIsShowName || !m_bLighting )
	{
		float		fHeight = m_pBoundBox->GetRadius();
		static	Vector3	vTextPos;
		vTextPos = m_pScene->GetScreenPos( m_vPos + Vector3( 0.0F, 0.0F, 0.5F ) );

		int		nX = (int)vTextPos.x - m_sName.size() * 6 / 2;
		int		nY = (int)vTextPos.y;
		
		int		nTxtWidth, nTxtHeight;
		m_pScene->GetStringSize( nTxtWidth, nTxtHeight, m_sName.c_str() );

		m_pRectangle->Draw( nX-2, nY-2, nTxtWidth+4, nTxtHeight+4, COLOR_ARGB( 150, 0,0,0 ) );
		m_pScene->DrawText( nX, nY, 0xFFD1D1D1, m_sName.c_str() );
	} //if
} //CW3DItem::DrawName

/**
 * 그리기.
 */
void CW3DItem::Draw()
{
	Assert( m_pModel );

	if( !m_bView )
	{
		return;
	} //if

#ifdef SHOW_BOUNDBOX
	m_pModel->SetWorld( m_matWorld );
	m_pBoundBox->Render( m_dwTick );
#endif
	
	m_pModel->SetWorld( m_matWorld );
	m_pModel->Render( m_dwTick, m_bLighting );
} //CW3DItem::Draw

/**
 * 그림자 그리기.
 */
void CW3DItem::DrawShadow()
{
	DrawName();
} //CW3DItem::DrawShadow

/**
 * 위치 설정.
 */
void CW3DItem::SetPosition(	const int	nX,		///< [IN] 맵 상의 X 좌표
							const int	nY		///< [IN] 맵 상의 Y 좌표
							)
{
	Assert( m_pWorld );

	m_vPos.x = (float)nX + 0.5F;
	m_vPos.y = (float)nY + 0.5F;
	m_vPos.z = m_pWorld->GetHeight( m_vPos ) + 0.1F;

	Quaternion	qRot = Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion();
	m_matWorld.SetWorld( m_vPos, qRot );
} //CW3DItem::SetPosition

/**
 * 피킹.
 *
 * @return	vOrig		[IN] Picking Ray Origin
 * @return	vDir		[IN] Picking Ray Direction
 *
 * @return	Picking
 */
BOOL CW3DItem::Pick( const Vector3& vOrig, const Vector3& vDir )
{
	Assert( m_pBoundBox );

	if( !m_bView )
	{
		return FALSE;
	} //if

	m_pBoundBox->SetWorld( m_matWorld );
	if( m_pBoundBox->Pick( vOrig, vDir ) )
	{
		return TRUE;
	} //if
	return FALSE;
} //CW3DItem::Pick

/**
 * 빛 적용 여부를 설정한다.
 * 빛을 적용하지 않으면 원색 그대로 출력된다.
 * 즉, 주변의 오브젝트에 비해 상대적으로 밝게 보인다.
 * 크리쳐를 선택하거나 하는 경우에 사용하면 효과적이다.
 *
 * @param	bLighting	[IN] 빛 적용 여부.
 */
void CW3DItem::SetLighting( const BOOL bLighting )
{
	m_bLighting = bLighting;
} //CW3DCharacter::SetLighting