/**
 * NPC.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 62 $
 *
 * <HR>
 * $Log: /GameClient/Src/NPC.CPP $
 * 
 * 62    05-02-18 7:38p Sk8snow2
 * 
 * 61    04-12-09 6:28p Sk8snow2
 * 
 * 60    04-06-08 3:57p Paran2
 * 
 * 59    04-03-12 6:42p Sckyj
 * 운영자 작업중
 * 
 * 58    04-03-12 2:12p Paran
 * Collision & Picking
 * 
 * 57    04-02-03 6:06p Paran
 * 
 * 56    03-12-26 10:33a Kjmgo
 * 선택유지중, 몬스터도 추가
 * 
 * 55    03-12-22 5:00p Sckyj
 * 2D, 3D sound 분리
 * 
 * 54    03-12-22 6:00a Kjmgo
 * 버그 수정
 * 
 * 53    03-12-20 6:18a Kjmgo
 * 메모리 관리
 * 
 * 52    03-12-17 11:46a Sckyj
 * 사운드 재생부분 코드 수정( 초기에 dwTime을 Limit과 같은 값으로 주어
 * 바로 재생이 가능하도록 )
 * 
 * 51    03-12-17 10:18a Sckyj
 * 사운드 재생 기능 추가 - *.snd 스크립트로 처리
 * 
 * 50    03-12-11 3:42p Kjmgo
 * 약간의 소스 수정
 * 
 * 49    03-10-27 3:24p Kjmgo
 * 코드 최적화
 * 
 * 48    03-10-17 8:39p Kjmgo
 * 
 * 47    03-10-11 12:08a Kjmgo
 * AddMagic수정 및, CCreature::~CCreature(); 쓰던거 삭제
 * 
 * 46    03-10-01 2:05a Paran
 * 
 * 45    03-09-29 10:34p Kjmgo
 * Damage가 제대로 뜨지 않던것 수정( 서버쪽에서도 수정 요망 - 몬스터 )
 * 
 * 44    03-09-09 12:15a Kjmgo
 * 
 * 43    03-09-07 4:39a Paran
 * 
 * 32    03-06-27 5:30p Kjmgo			Assert처리
 * 
 * 29    03-06-23 5:53p Paran			바운드 박스 처리.
 * 
 * 24    03-06-03 11:38p Paran			조력자 맴버 추가
 * 
 * 22    03-05-25 7:32p Kjmgo			그림자를 동그란 그림자로 교체
 *
 * 14    03-05-10 12:54p Ahastudio		Engine에서 Creature Implement 관련 클래스를 Game으로 이동함.
 *
 * 13    03-05-10 12:32p Ahastudio		SetLighting() 추가.
 *
 * 9     03-04-30 7:11p Paran			스킬 선택 수정.
 *
 * 2     03-04-15 1:27a Paran			CW3DNPC 추가.
 * <HR>
 *
 * @file	NPC.cpp
 */


#include "Global.h"
#include "Creature.h"
#include <string>
#include <vector>
#include <map>


using std::string;
using std::vector;
using std::map;


typedef map<DWORD, IW3DModel*>		ModelMap;


static ModelMap		gs_mapNPCModel;					///< 모델 데이터.

struct SSound
{
	UNDER_MEMORY_MGR( SSound );

	char	szWave[128];
	DWORD	dwLimit;
	int		nDistance;
	
	DWORD	dwTime;

	SSound()
	{
		ZeroMemory( szWave, 128 );
		dwLimit = dwTime = nDistance = 0;
	}
};

/**
 * NPC 클래스.
 * @ingroup		Game
 */
class CW3DNPC : public CCreature
{
public:
	UNDER_MEMORY_MGR( CW3DNPC );

	CW3DNPC();
	~CW3DNPC();

	BOOL	Create( IW3DWorld* pWorld, const DWORD dwId, const void* pData );
	BOOL	UpdateModel( const WORD wItemType, const BOOL bOn );

	BOOL	Update( const DWORD dwTick );

	void	Draw();
	void	DrawShadow();
	void	DrawName();

	/**
	 * 크리쳐 종류를 얻는다.
	 */
	int		GetType()
	{
		return NPC;
	} //GetType

	void	SetLighting( const BOOL bLighting );

	BOOL	MoveTo( const Vector3& vPos );

	BOOL	Pick( const Vector3& vOrig, const Vector3& vDir );

	/**
	 * 배역설정.
	 */
	void	SetCastContent( 
		const WORD wShop,					///< [IN] 상점 번호.
		const WORD wQuest,					///< [IN] 퀘스트 번호.
		const BYTE btHaveStorage,			///< [IN] 창고 유무.
		const BOOL bGuildMgr				///< [IN] 단 관리
		);

	/**
	 * 배역 얻기.
	 */
	void	GetCastContent( 
		WORD& wShop,					///< [OUT] 상점 번호.
		WORD& wQuest,					///< [OUT] 퀘스트 번호.
		BYTE& btHaveStorage,			///< [OUT] 창고 유무.
		BYTE& btGuildMgr				///< [OUT] 단 관리.
		);

	/**
	 * 조력자 설정.
	 */
	void	SetHelper( const IW3DCreature* pCreature )
	{
		//--NULL--
	} //SetHelper

	/**
	 * 조력자 제거.
	 */
	void	RemoveHelper()
	{
		//--NULL--
	} //RemoveHelper

	/**
	 * 성별을 얻는다.
	 */
	UINT	GetSex()
	{
		return 0;
	} //GetSex

	BOOL	LoadSound( const char* szPath, const char* szFile );


private:
	IW3DModel*		m_pModel;		///< 모델	
	IW3DShadow*		m_pShadow;		///< 그림자

	BYTE			m_btShop;		///< 상점번호.
	BYTE			m_btQuest;		///< 퀘스트 번호.
	BYTE			m_btStorage;	///< 창고 번호.
	BYTE			m_btGuildMgr;	///< 길드 관리

	BOOL			m_bLighting;	///< 빛 적용 여부

	IW3DModel*		LoadModel( DWORD dwType );

	char*			ModelName( DWORD dwType );

	char*			GetActionName();
	BOOL			IsEndAction( DWORD dwTick );

	SSound			m_sSound;
}; // class CW3DNPC


IMPLEMENT_COMPONENT( CW3DNPC, NPC );


/**
 * 생성자.
 */
CW3DNPC::CW3DNPC()
:	m_pModel	( NULL ),
	m_pShadow	( NULL ),
	m_bLighting	( TRUE ),
	m_btShop	( 0 ),
	m_btQuest	( 0 ),
	m_btStorage	( 0 ),
	m_btGuildMgr( 0 )
{	
} //CW3DNPC::CW3DNPC

/**
 * 소멸자.
 */
CW3DNPC::~CW3DNPC()
{
	SAFE_RELEASE( m_pShadow );

	if( m_pModel )
	{
		if( m_pModel->GetRefCount() <= 1 )
		{
			ModelMap::iterator	itor = gs_mapNPCModel.find( m_dwType );
			if( itor != gs_mapNPCModel.end() )
			{
				gs_mapNPCModel.erase( itor );
			} //if
		} //if
	} //if

	SAFE_RELEASE( m_pModel );

	if( g_pSelectMgr && g_pSelectMgr->GetType() == NPC &&
		g_pSelectMgr->GetCreature() &&
		g_pSelectMgr->GetCreature()->GetId() == GetId()
		)
	{
		g_pSelectMgr->Select( NULL );
	} //if
} //CW3DNPC::~CW3DNPC

/**
 * NPC를 생성한다.
 *
 * @return	성공 여부
 */
BOOL CW3DNPC::Create(
	IW3DWorld*		pWorld,		///< [IN] 월드 객체
	const DWORD		dwId,		///< [IN] 크리쳐 ID
	const void*		pData		///< [IN] 크리쳐 생성 정보
	)
{
	m_pWorld = pWorld;

	Assert( pData );
	Assert( m_pWorld );
	Assert( NULL == m_pSound );
	Assert( NULL == m_pModel );
	Assert( NULL == m_pShadow );

	m_pScene = g_pScene;
	Assert( m_pScene );
	
	m_pRectangle = MAKE_COMPONENT( Rectangle );
	Assert( m_pRectangle );

	SW3DNPCInfo&	infoNPC = *(SW3DNPCInfo*)pData;

	m_dwId		= dwId;
	m_sName		= "";
	m_vPos		= infoNPC.vPos;
	m_vGoalPos	= m_vPos;
	m_qRot.SetRotationZ( ANGLE( infoNPC.fYaw ) );
	m_qRot		= Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion() * m_qRot;
	m_dwType	= dwId;	

	// Load Model

	m_pModel = LoadModel( m_dwType );
	if( !m_pModel )
	{
//		Assert( !"모델 읽기 실패" );
		return FALSE;
	} //if
	
	m_pSound = MAKE_COMPONENT( SoundObject3D );
	CHECK_PTR_RETURN( m_pSound );

	// Create Shadow

	m_pShadow = MAKE_COMPONENT( Shadow );
	CHECK_PTR_RETURN( m_pShadow ); 

	//m_pShadow->SetSize( 4 );
	if( !m_pShadow->Init( 0 ) )
	{
		Assert( !"그림자 초기화 실패" );
		SAFE_RELEASE( m_pShadow );
		return FALSE;
	} //if

	// 동작차이를 주기 위해서
	static DWORD	s_dwInterval;
	m_dwTick = s_dwInterval;
	s_dwInterval += 1333;

	return TRUE;
} //CW3DNPC::Create

/**
 * NPC 모델을 읽어온다.
 *
 * @param	dwType		[IN] NPC 종류
 *
 * @return	NPC 모델
 */
IW3DModel* CW3DNPC::LoadModel( DWORD dwType )
{
	ModelMap::iterator	itor = gs_mapNPCModel.find( dwType );
	if( itor != gs_mapNPCModel.end() )
	{
		IW3DModel*	pModel = itor->second;
		Assert( pModel );

		if( !pModel )
		{
			return NULL;
		} //if
		pModel->AddRef();
		return pModel;
	} //if

	char*	szName = ModelName( dwType );
	char	szFile[128];	
	char	szWave[128] = "\0";
	FILE*	fp = NULL;

	IW3DModel*	pModel = MAKE_COMPONENT( Model );
	if( !pModel )
	{
		Assert( !"메모리 할당 실패" );
		return NULL;
	} //if

	sprintf( szFile, "%s.mod", szName );	
	if( !pModel->Load( "NPC", szFile ) )
	{
		DEBUG_OUT( "NPC 모델 읽기 실패 - " + szFile );
		SAFE_RELEASE( pModel );
		return NULL;
	} //if
	pModel->CalculateRadius();

	sprintf( szFile, "%s.ani", szName );
	pModel->LoadAnimation( "NPC", szFile );

	gs_mapNPCModel.insert( std::make_pair( dwType, pModel ) );	

	DEBUG_OUT( "Load NPC Model - " + szFile );

	// 사운드 파일 로드
	sprintf( szWave, "%s.snd", szName );
	LoadSound( "NPC", szWave );
	
	return pModel;
} //CW3DNPC::LoadModel

/**
 * NPC 모델 이름을 얻는다.
 *
 * @param	dwType		[IN] NPC 종류
 *
 * @return	NPC 모델 이름
 */
char* CW3DNPC::ModelName( DWORD dwType )
{
	static char		szNPCModel[256];

	int		nNumModel = dwType % 100;
	sprintf( szNPCModel, "cn_00%02d", nNumModel );

	return szNPCModel;
} //CW3DNPC::ModelName

/**
 * 사운드 파일 정보를 로드한다.
 */
//void CW3DNPC::LoadSound( FILE* fp )
BOOL CW3DNPC::LoadSound( const char* szPath, const char* szFile )
{
	char szFullPath[256] = "\0";
	strcat( szFullPath, szPath );
	strcat( szFullPath, "/" );
	strcat( szFullPath, szFile );

	FILE* fp = fopen( szFullPath, "rt" );
	if( !fp )
	{
		m_sSound.szWave[0] = '\0';
		return FALSE;
	}
	
	char szBuffer[256] = "\0";
	char szKeyword[128] = "\0";
	char szValue[128] = "\0";	

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( szKeyword[0] == NULL )
		{
			continue;
		}

		if( !strcmp( szKeyword, "[WAVE]" ) )
		{
			if( sscanf( szBuffer, "%s %s", szKeyword, szValue ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			strcpy( m_sSound.szWave, szValue );
		}		
		else
		if( !strcmp( szKeyword, "[LIMIT]" ) )
		{
			if( sscanf( szBuffer, "%s %s", szKeyword, szValue ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			m_sSound.dwLimit = atoi( szValue );
			m_sSound.dwTime = m_sSound.dwLimit;
		}
		else
		if( !strcmp( szKeyword, "[DISTANCE]" ) )
		{
			if( sscanf( szBuffer, "%s %s", szKeyword, szValue ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			m_sSound.nDistance = atoi( szValue );
		}
	}

	fclose( fp );
	return TRUE;
} // CW3DNPC::LoadSound

/**
 * 몬스터를 그려준다.
 */
void CW3DNPC::Draw()
{
	if( !m_bShow ) return ;

#ifdef SHOW_BOUNDBOX
	m_pBoundBox->Render( m_dwTick );
#endif

	if( m_pModel )
	{
		m_pModel->SetWorld( m_vPos, m_qRot );
		m_pModel->Render( m_dwTick, m_bLighting );
	} //if
} //CW3DNPC::Draw

/**
 * NPC의 그림자를 그려준다.
 */
void CW3DNPC::DrawShadow()
{
	if( !m_bShow ) return ;

	Assert( m_pWorld );
	Assert( m_pShadow );


	if( m_pShadow->IsProjection() )
	{
		m_pModel->SetWorld( m_vPos, m_qRot );
		m_pWorld->GetTerrain()->InsertShadow( m_pShadow, m_vPos );
		m_pShadow->Render( m_pScene->GetFrustum().m_matView );
	}
	else
	{		
		Matrix4		matWorld;
		matWorld.SetWorld( m_vPos + Vector3( 0.0F, 0.0F, 0.1F ), m_qRot );
		m_pShadow->Render( matWorld );
	} //if..else..	
	DrawName();
} //CW3DNPC::DrawShadow

/**
 * 이름 찍기.
 */
void CW3DNPC::DrawName()
{
//	if( m_bShowName || !m_bLighting )
	{
		Vector3		vSize = m_pModel->GetSize();
		float	fHeight = vSize.y + 0.2F;
		m_vTextPos = m_pScene->GetScreenPos( m_vPos + Vector3( 0.0F, 0.0F, fHeight ) );
	
		int		nTxtWidth, nTxtHeight;

		if( !g_pGUI->GetNPCName( m_dwId ) )
			return;

		char	szTemp[1024];
		/*if( !g_pGUI->GetNPCName( m_dwId ) )
		{
			sprintf( szTemp, "NoName ID : %04d", m_dwId );
		}
		else
		{*/
			sprintf( szTemp, "%s", g_pGUI->GetNPCName( m_dwId ), m_dwId );
	//	}		
		m_pScene->GetStringSize( nTxtWidth, nTxtHeight, szTemp );
		int		nX = m_vTextPos.x - ( nTxtWidth / 2 );
		int		nY = m_vTextPos.y + 1; 
		//nX = nX - ( nTxtWidth >> 1 );
	//	strcpy( m_sName, g_pGUI->GetNPCName( m_dwId ) );
		m_pRectangle->Draw( nX-2, nY-2, nTxtWidth+10, nTxtHeight+5, 0x4B000000 );
		m_dwColorName = 0xff41cc3a;
		m_pScene->DrawText( nX, nY, m_dwColorName, szTemp );
	} //if
} //CW3DNPC::DrawName

/**
 * 액션이 끝났는지 알려준다.
 */
BOOL CW3DNPC::IsEndAction( DWORD dwTick )
{
	if( dwTick >= m_pModel->GetActionTicks() )
	{
		return TRUE;
	} //if

	return FALSE;
} //CW3DNPC::IsEndAction

/**
 * ...
 */
BOOL CW3DNPC::Update( DWORD dwTick )
{
	Assert( m_pWorld );
	Assert( m_pShadow );
	Assert( m_pSound );

	m_pModel->SetWorld( m_vPos, m_qRot );

	m_bShow = FALSE;

	if( m_pModel->IsVisible() )
	{
		m_bShow = TRUE;
		if( m_pShadow->IsProjection() )
		{
			m_pModel->SetWorld( m_vPos, m_qRot );
			m_pShadow->SetLight( m_vPos );
			m_pShadow->InsertBlocker( m_pModel );
			m_pShadow->ShadowMatrix( m_vPos );

			m_pShadow->MakeShadowMap( m_dwTick + dwTick );
 		} //if
	}  //if


	// 사운드 재생
	if( m_sSound.szWave[0] != '\0' && m_sSound.dwLimit <= m_sSound.dwTime )
	{
		m_pSound->SetPosition( m_vPos );
		m_pSound->Play3D( m_sSound.szWave, 1, m_sSound.nDistance );
		m_sSound.dwTime = 0;
	}	

	m_dwTick += dwTick;
	m_sSound.dwTime += dwTick;

	return TRUE;
} //CW3DNPC::Update

/**
 * NPC 모델을 업데이트 한다.
 */
BOOL CW3DNPC::UpdateModel( const WORD wItemType, BOOL bOn )
{
	return TRUE;
} //CW3DNPC::UpdateModel

/**
 * 현재 Action의 이름을 알려준다.
 *
 * @return	현재 Action의 이름 / NULL
 */
char* CW3DNPC::GetActionName()
{
	return NULL;
} //CW3DNPC::GetActionName

/**
 * 빛 적용 여부를 설정한다.
 * 빛을 적용하지 않으면 원색 그대로 출력된다.
 * 즉, 주변의 오브젝트에 비해 상대적으로 밝게 보인다.
 * 크리쳐를 선택하거나 하는 경우에 사용하면 효과적이다.
 *
 * @param	bLighting	[IN] 빛 적용 여부.
 */
void CW3DNPC::SetLighting( const BOOL bLighting )
{
	m_bLighting = bLighting;
} //CW3DCharacter::SetLighting

/**
 * 길찾기를 통해 목표점까지 이동한다.
 */
BOOL CW3DNPC::MoveTo( const Vector3& vPos )
{
	return FALSE;
} //CW3DNPC::MoveTo

/**
 * 피킹.
 *
 * @return	vOrig		[IN] Picking Ray Origin
 * @return	vDir		[IN] Picking Ray Direction
 *
 * @return	Picking
 */
BOOL CW3DNPC::Pick( const Vector3& vOrig, const Vector3& vDir )
{
	if( !m_bShow || !m_pModel || ( m_btGhost && !m_bVisible ) )
	{
		return FALSE;
	} //if

	m_pModel->SetWorld( m_vPos, m_qRot );
	if( m_pModel->Pick( vOrig, vDir ) )
	{
		return TRUE;
	} //if
	return FALSE;
} //CW3DNPC::Pick

/**
 * 배역 설정.
 */
void CW3DNPC::SetCastContent( const WORD wShop, const WORD wQuest, const BYTE btHaveStorage, const BOOL bGuildMgr )
{
	m_btShop		= wShop;
	m_btQuest		= wQuest;
	m_btStorage		= btHaveStorage;
	m_btGuildMgr	= bGuildMgr;
} //CW3DNPC::SetCastContent

/**
 * 배역 얻기.
 */
void CW3DNPC::GetCastContent( WORD& wShop, WORD& wQuest, BYTE& btHaveStorage, BYTE& btGuildMgr )
{
	wShop			= m_btShop;
	wQuest			= m_btQuest;
	btHaveStorage	= m_btStorage;
	btGuildMgr		= m_btGuildMgr;
} //CW3DNPC::GetCastContent