/**
 * 캐릭터 선택/생성 프레임 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 170 $
 *
 * <HR>
 * $Log: /GameClient/Src/SelectCharacter.cpp $
 * 
 * 170   05-02-18 7:38p Sk8snow2
 * 
 * 169   04-09-24 4:45p Paran
 * 
 * 168   04-07-13 10:00a Paran
 * 
 * 163   04-03-15 5:43p Sckyj			운영자 작업 1차완료 - 동작막기 안함
 * 
 * 162   04-03-13 12:13p Sckyj			운영자 작업중  - OnPacketCharInfo() 수정
 * 
 * 161   04-03-03 8:02a Sk8snow2		OnPacketLoginResult 수정 ( 정혁씨 봐죠요! )
 * 
 * 148   04-01-06 2:10p Sckyj			BGM 재생 관련 코드수정
 * 
 * 147   04-01-05 5:33p Sckyj			BGM 관련 코드 수정
 * 
 * 146   04-01-05 3:43p Sckyj			BGM 재생관련 코드 수정
 * 
 * 144   04-01-03 10:59 Sckyj			OnEventButton() - In Chinese Version Change Message
 * 
 * 143   04-01-02 5:36p Paran			스킬 적용대상 수정
 * 
 * 142   04-01-02 2:20p Paran			메세지 define 처리
 * 
 * 141   03-12-30 3:13p Paran			사운드부분 릴리즈
 * 
 * 140   03-12-26 3:46p Sckyj			캐릭터 선택시 효과음 추가
 * 
 * 139   03-12-26 3:18p Sckyj			로딩, 캐릭선택시 BGM재생
 * 
 * 133   03-12-18 11:47p Kjmgo			금칙단어 수정
 * 
 * 125   03-12-16 2:13p Sk8snow2		OnUpdateCha 추가 ( 성향 변경 )
 * 
 * 123   03-12-16 2:42a Kjmgo			예외처리
 * 
 * 122   03-12-15 11:24p Kjmgo			선택시, 인사 애니메이션 1차 적용( せんちゃくする時(じ), 'あいさつ
										アニメ' 一(いち)次 的用(てきよう) )
 * 
 * 119   03-12-14 2:53p Sk8snow2		OnPacketCharInfo : 성향 관련 수정
 * 
 * 115   03-12-10 10:21a Sckyj			SelectCharacter Mode로 넘어올경우 재생중이던 BGM을 종료하도록 수정
 * 
 * 113   03-12-09 12:35p Sk8snow2		OnPacketCharInfo 함수 국적 정리
 * 
 * 112   03-12-09 12:34p Sk8snow2		OnPacketGetAllCharResult 함수 통합레벨, 국적 정리
 * 
 * 107   03-10-13 2:51p Kjmgo			Character의 Bone Animation 연산을 단 한번만 하도록 수정
 * 
 * 106   03-10-06 7:19p Kjmgo			Send Fail. Bug. 
 * 
 * 98    03-08-29 9:19a Kjmgo			Animation 바꿈
 * 
 * 91    03-08-15 4:43a Sk8snow			OnPacketGetAllCharResult() 수정
 * 
 * 89    03-08-10 6:59p Admin			말 타고내리기 적용
 * 
 * 70    03-06-19 8:31p Paran			맵 지역 분리
 * 
 * 66    03-06-15 12:12a Paran			SkipPrologue 추가.
 * 
 * 64    03-06-14 11:03a Paran			Skill zzang~
 * 
 * 61    03-06-05 3:17p Paran			동작 에니 작업
 * 
 * 60    03-05-27 8:41p Paran			활공격, 폴더 정리
 * 
 * 59    03-05-23 4:05a Paran			공격가능거리추가.
 * 
 * 49    03-04-11 5:29p Paran			캐릭터 장착 부분 추가.
 *
 * 42    03-03-28 1:17a Paran			스킬바 추가, 캐릭터 셀렉트 부분 수정.
 *
 * 41    03-03-27 11:03a Ahastudio		Heart Beat 추가.
 *
 * 40    03-03-27 10:45a Ahastudio		이미 접속 중인 아이디 처리.
 *
 * 27    03-03-14 2:29p Ahastudio		받은 패킷을 멤버 변수 m_recvPacket로 설정.
 *
 * 24    03-03-06 11:17p Paran			캐릭터 삭제 처리 & 메세지 박스.
 *
 * 23    03-03-06 10:37p Ahastudio		캐릭터 삭제 결과.
 * <HR>
 *
 * @file	SelectCharacter.cpp
 */


#include "Global.h"
#include "Client.h"
#include "Protocol.h"
#include "Packet.h"
#include "Type.h"
#include "SelectCharProduce.h"
#include "Encrypt.h"
#include <string>
#include <vector>

#define NUM_SELECTCHAR			3
using std::string;
using std::vector;

enum
{
	FOOT_PART = 0,
	HAND_PART,
	LOWER_PART,
	UPPER_PART,
	FACE_PART,
	HAIR_PART,
	MAX_PARTS
}; //enum



/**
 * 캐릭터 선택/생성 프레임 구현부.
 * @ingroup		Game
 */
class CSelectCharacter : public IFrame
{
public:
	UNDER_MEMORY_MGR( CSelectCharacter );

	CSelectCharacter();
	~CSelectCharacter();

	BOOL	Init();
	BOOL	Update( const DWORD dwTick, string* psName );

private:
	/**
	 * 캐릭터 클래스.
	 * @ingroup		Game
	 */
	class CCharacter
	{
	public:
		UNDER_MEMORY_MGR( CSelectCharacter::CCharacter );

		/**
		 * 생성자.
		 */
		CCharacter()
		{
			m_qRot = Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion();
			for( int i = 0 ; i < MAX_PARTS ; i++ )
			{
				m_apModel[i] = NULL;
			} //for

			
			m_bCancel = FALSE;
			m_pAction = NULL;
			m_bSelect = FALSE;
			m_dwActionTick = 0;
		} //CCharacter

		/**
		 * 소멸자.
		 */
		~CCharacter()
		{
			SAFE_DELETE( m_pAction );
			Clear();
		} //~CCharacter

		/**
		 * 캐릭터 모델 생성.
		 */
		void	Create(
			const BYTE	bSex,
			char*		szHair,
			char*		szFace,
			char*		szUpper,
			char*		szLower,
			char*		szHand,
			char*		szFoot
			)
		{
			Clear();

			string		asModel[MAX_PARTS];
			asModel[HAIR_PART]  = string( "High/" ) + szHair;
			asModel[FACE_PART]  = string( "High/" ) + szFace;
			asModel[UPPER_PART] = string( "High/" ) + szUpper;
			asModel[LOWER_PART] = string( "High/" ) + szLower;
			asModel[HAND_PART]  = string( "High/" ) + szHand;
			asModel[FOOT_PART]  = string( "High/" ) + szFoot;

			//에니메이션 관리자를 생성합니다.
			//アニメ 管理人(かんりにん)を 生成(せいせい)します。
			m_pAnimationMgr = MAKE_COMPONENT( AnimationMgr );

			m_bSex = bSex;

			m_pAction = new SAction;

			SAction* pAction = NULL;

			//만약 남자라면,
			if( !bSex )
			{ //もし, 姓(せい)が おとこなら。
				pAction = m_pAnimationMgr->LoadAnimation( "Body/Animation", "intro_m.ani" );
				*m_pAction = *pAction;
				//76_244_297
				m_pAction->nStart = 2;
				m_pAction->nEnd = 75;
			}
			else
			{  //もし, 姓が おんななら,
				pAction = m_pAnimationMgr->LoadAnimation( "Body/Animation", "intro_w.ani" );
				*m_pAction = *pAction;
				//m_pAction->nStart = 1;
				//m_pAction->nEnd = 81;
				m_pAction->nStart = 1;
				m_pAction->nEnd = 81;
			} //if..else..

			Assert( m_pAction );

			for( int i = 0; i < MAX_PARTS; i++ )
			{
				IW3DModel*	pModel = MAKE_COMPONENT( Model );
				if( !pModel )		continue;

				if( !pModel->Load( "Body", asModel[i].c_str() ) )
				{
					Assert( !"모델 로드 실패" );
					SAFE_RELEASE( pModel );
					continue;
				} //if

				pModel->SetAction( m_pAction );
				m_apModel[i]	= pModel;
			} //for
		} //Create

		/**
		 * 캐릭터 모델 삭제.
		 */
		void	Clear()
		{
			for( int i = 0 ; i < MAX_PARTS ; i++ )
			{
				SAFE_RELEASE( m_apModel[i] );
			} //for
		} //Create

		/**
		 * 위치설정.
		 */
		void	SetPosition( const Vector3& vPos )
		{
			m_vPos = vPos;
		} //SetPosition

		/**
		 * 위치를 얻는다.
		 */
		Vector3 GetPosition()
		{
			return m_vPos;
		} //GetPosition

		/**
		 *
		 */
		void SetAnimation( SAction* p )
		{
			m_pAction = p;
		} //SetAnimation

		void	SetRotate( const char chAxis, float fAngle )
		{
			if( chAxis == 'x' )			m_qRot.SetRotationX( fAngle );
			else if( chAxis == 'y' )	m_qRot.SetRotationY( fAngle );
			else if( chAxis == 'z' )	m_qRot.SetRotationZ( fAngle );
			m_qRot = Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion() * m_qRot;
		} //SetRotate

		BOOL	IsEmpty()
		{
			if( m_apModel[UPPER_PART] )
			{
				return FALSE;
			}
			return TRUE;
		} //IsEmpty()

		/**
		 * 캐릭터 출력.
		 */
		void	Draw( const DWORD dwTick, const DWORD dwTime )
		{
			if( IsEmpty() )				return;

			Matrix4		matWorld;
			matWorld.SetWorld(	m_vPos, m_qRot );

			BOOL bFlag = FALSE;
			DWORD dwTicks = dwTime;
			DWORD dwActionTicks;

			if( m_bSelect && m_pAction )
			{
				if( m_apModel[0] == NULL )
				{
					for( int i = 0 ; i < MAX_PARTS ; i++ )
					{
						if( !m_apModel[i] )
							continue;

						m_apModel[i]->SetAction( m_pAction );
						dwActionTicks = m_apModel[i]->GetActionTicks();	
					}
						
				}
				else
				{
					m_apModel[0]->SetAction( m_pAction );
					dwActionTicks = m_apModel[0]->GetActionTicks();	
				}
			
				if( m_dwActionTick >= dwActionTicks )
				{
				//	SetSelect( FALSE );
					if( m_bSex )
					{ //もし, おんななら,
						m_pAction->nStart = 181;
						m_pAction->nEnd = 231;
					}
					else
					{ //もし, おとこなら
						m_pAction->nStart = 120;
						m_pAction->nEnd = 180;
					} //if..else..

					m_dwActionTick = 0;
				} //if
			} //if
		
			dwTicks = m_dwActionTick;

			for( int i = 0 ; i < MAX_PARTS ; i++ )
			{
				if( !m_apModel[i] )		continue;

				m_apModel[i]->SetApplyWorld( FALSE );

				{
					m_apModel[i]->SetAction( m_pAction );	
				} //if

				m_apModel[i]->SetWorld( matWorld );
				
				if( !bFlag )
				{
					m_apModel[i]->SetVertexBlend( dwTicks );
					bFlag = TRUE;
				} //if

				m_apModel[i]->Render( dwTicks, TRUE );
				m_apModel[i]->SetApplyWorld( TRUE );
			} //for

			m_dwActionTick += dwTick;

		} //Draw

		/**
		 *
		 */
		void SetSelect( BOOL bFlag )
		{
			if( !m_pAction )
			{
				return ;
			} //if

	/*		m_bCancel = FALSE;
			if( m_bSelect && !bFlag )
			{
				m_bCancel = TRUE;
			} //if*/

			if( bFlag )
			{	//もし, せんたくなら,
				if( m_bSex )
				{ //もし, おんななら,
					m_pAction->nStart = 81;
					m_pAction->nEnd = 231;
				}
				else
				{ //もし, おとこなら,
					m_pAction->nStart = 76;
					m_pAction->nEnd = 120;
				} //if..else..
			}
			else
			{  	//もし せんたくではないなら。
				if( m_bSex )
				{ //もし, おんななら,
					m_pAction->nStart = 1;
					m_pAction->nEnd = 81;
				}
				else
				{ //もし, おとこなら,
					m_pAction->nStart = 1;
					m_pAction->nEnd = 76;
				} //if..else..
			} //if

			m_bSelect = bFlag;
			m_dwActionTick = 0; 
		} //SetSelect		

		BOOL	GetSex() { return m_bSex; }

	private:
		IW3DModel*			m_apModel[MAX_PARTS];	///< 모델
		IW3DAnimationMgr*	m_pAnimationMgr;		///< 관리자. アニメ 管理人(かんりにん)		

		BOOL				m_bSex;					///< 성별 姓(せい)
		BOOL				m_bCancel;				///< 취소
		SAction*			m_pAction;				///< 애니메이션 アニメ
		
		Quaternion			m_qRot;
		Vector3				m_vPos;					///< 위치

		DWORD				m_dwActionTick;			///< Action Tick.
		BOOL				m_bSelect;				///< 選擇 Flag.
	}; //class CCharacter

	DWORD				m_dwTick;				///< 누적 tick
	BYTE				m_btMode;				///< 현재 모드

	int					m_nSelected;			///< 선택한 캐릭터 번호 0 base
	BYTE				m_btRotate;				///< 생성 캐릭터 회전
	BYTE				m_btZoom;				///< 생성 캐릭터 회전
	float				m_fCreateAngle;			///< 생성 회전 각

	IW3DCamera*			m_pCamera;				///< 카메라
	IW3DLight*			m_pLight;				///< 빛
	CCharacter*			m_apCharacter;			///< 캐릭터
	CProduce*			m_pProduce;				///< 캐릭터 생성 연출
	IW3DModel*			m_pBackModel;			///< 배경 모델
	IW3DModel*			m_pLightModel;			///< 
	IW3DSoundObject2D*	m_pSound;				///< 사운드 객체
	BOOL				m_bSound;				///< 사운드 재생여부
	
	typedef vector<IW3DModel*>		ARRAY_MODEL;
	ARRAY_MODEL			m_apLightModel1;
	ARRAY_MODEL			m_apLightModel2;
	ARRAY_MODEL			m_apLightModel3;
	ARRAY_MODEL			m_apLightModel4;
	ARRAY_MODEL			m_apLightModel8;
	ARRAY_MODEL			m_apLightModel16;
	ARRAY_MODEL			m_apLightModelde;
	BYTE				m_btState;
	DWORD				m_dwStateTick;

	Matrix4				m_matWorld;
	Vector3				m_vCharacterPos[NUM_SELECTCHAR+1];

	BOOL				m_bQuit;				///< 종료
	BOOL				m_bExitFrame;			///< 프레임 종료
	BOOL				m_bStartGame;			///< 게임 시작

	CPacket				m_recvPacket;			///< 받은 패킷

	void	SelectProduce( const WORD wProduce );
	BOOL	LoadAct( const char* szPath, const char* szFile );
	void	DrawCharacter( DWORD dwTick );
	void	Render( DWORD dwTick );

	BOOL	LoadLightModel();
	BOOL	LoadModelArray( ARRAY_MODEL& aModel, const char* szKey, int nNum );
	void	ReleaseArrayModel( ARRAY_MODEL& ModelArray );
	void	DrawLightModel( BYTE btAct, const DWORD dwTick );
	void	DrawModelArray( ARRAY_MODEL& aModel, const DWORD dwTick, BOOL bLighting  );

	BOOL	UpdateInput( DWORD dwTick );
	BOOL	UpdateNetwork( DWORD dwTick );

	void	UpdateCharacter( const int nNum, BOOL bAnyShow = FALSE );

	void	ProcessRecvPacket();

	void	OnEventButton( const char* szUIName, DWORD dwParam2 );
	void	OnEventReturnKey( const char* szUIName );
	void	OnEventKeyDown( const dwParam1 );

	void	ConnectRequestToGame();

	void	OnEventCreateCharacter();

	void	OnPacketLoginResult();
	void	OnPacketCharInfo();
	void	OnPacketGameStart();
	void	OnPacketGetAllCharResult();
	void	OnPacketNullCharacter();
	void	OnPacketMakeCharResult();
	void	OnPacketDeleteCharacterResult();
	void	OnPacketAlreadyConnected();
#ifdef _dGUILD_WAR_CHINA_	//중국 길드 대항전관련
	void	OnPacketNotConnected();
	void	OnPacketErrorUser();
#endif

	void	SelectCharacter( const int nNum );
	void	CharacterCreate();
	void	OnPacketSelCharSystemMsg();

}; //class CSelectCharacter


IMPLEMENT_FRAME( CSelectCharacter, SelectCharacter );


/**
 * 생성자.
 */
CSelectCharacter::CSelectCharacter()
:	m_dwTick			( 0 ),
	m_pLight			( NULL ),
	m_pProduce			( NULL ),
	m_pBackModel		( NULL ),
	m_pLightModel		( NULL ),
	m_bQuit				( FALSE ),
	m_bExitFrame		( FALSE ),
	m_nSelected			( -1 ),
	m_btMode			( 0 ),
	m_pCamera			( NULL ),
	m_btRotate			( 0 ),
	m_btZoom			( 0 ),
	m_fCreateAngle		( 0.0F ),
	m_dwStateTick		( 0 ),
	m_btState			( 0 ),
	m_pSound			( NULL ),
	m_bSound			( FALSE )
{
	m_apCharacter = NULL;
} //CSelectCharacter::CSelectCharacter

/**
 * 소멸자.
 */
CSelectCharacter::~CSelectCharacter()
{
	SAFE_DELETE_ARRAY( m_apCharacter );
	SAFE_DELETE( m_pProduce );
	SAFE_RELEASE( m_pBackModel );
	SAFE_RELEASE( m_pLightModel );
	SAFE_RELEASE( m_pLight );
	SAFE_RELEASE( m_pCamera );
	
	ReleaseArrayModel( m_apLightModel1 );
	ReleaseArrayModel( m_apLightModel2 );
	ReleaseArrayModel( m_apLightModel3 );
	ReleaseArrayModel( m_apLightModel4 );
	ReleaseArrayModel( m_apLightModel8 );
	ReleaseArrayModel( m_apLightModel16 );
	ReleaseArrayModel( m_apLightModelde );

	SAFE_RELEASE( m_pSound );
} //CSelectCharacter::~CSelectCharacter

/**
 * 배열 모델 릴리즈
 */
void CSelectCharacter::ReleaseArrayModel( ARRAY_MODEL& ModelArray )
{
	for( UINT nCnt = 0; nCnt < ModelArray.size(); nCnt++ )
	{
		SAFE_RELEASE( ModelArray[nCnt] );
	} //for
	ModelArray.clear();
} //CSelectCharacter::ReleaseArrayModel

/**
 * Frame 초기화.
 *
 * @return	초기화 성공 여부 (TRUE/FALSE)
 */
BOOL CSelectCharacter::Init()
{
	Log( "== Character Select Start ==" );	

	m_pLight = MAKE_COMPONENT( Light );
	CHECK_PTR_RETURN( m_pLight );

	m_pLight->SetPosition( Vector3( 0.0F, 10.0F, ANGLE( 200 ) ) );
	m_pLight->SetDirectional();
	m_pLight->SetDiffuse( 0xFFFFDA );
	m_pLight->SetAmbient( 0x382E1E );
	m_pLight->SetSpecular( 0xFFFFFF );
	m_pLight->SetDirection( Vector3( 0.0F, -1.0F, -1.0F ) );

	m_vCharacterPos[0] = Vector3( 2.344F, 0.604F, 4.011F );
	m_vCharacterPos[1] = Vector3( 0.207F, 0.604F, 4.011F );
	m_vCharacterPos[2] = Vector3( -1.93F, 0.604F, 4.011F );
	m_vCharacterPos[3] = Vector3( 0.153F, -15.601F, 0.020F );

	m_pCamera = MAKE_COMPONENT( Camera );
	CHECK_PTR_RETURN( m_pCamera );

	m_pBackModel = MAKE_COMPONENT( Model );
	CHECK_PTR_RETURN( m_pBackModel );

	m_pLightModel = MAKE_COMPONENT( Model );
	CHECK_PTR_RETURN( m_pLightModel );

	if( !m_pLightModel->Load( "Object", "stage_lighton_always_01.mod" ) ||
		!m_pBackModel->Load( "Object", "stage_always_01.mod" ) )
	{
		Assert( !"로드실패" );
		return FALSE;
	} //if

	LoadLightModel();

	Quaternion		m_qRot = Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion();
	m_matWorld.SetWorld( Vector3( 0.0F, 0.0F, 0.0F ), m_qRot );

	m_apCharacter =  new CCharacter[NUM_SELECTCHAR];
	CHECK_PTR_RETURN( m_apCharacter );
	m_apCharacter[0].SetPosition( m_vCharacterPos[0] );
	m_apCharacter[1].SetPosition( m_vCharacterPos[1] );
	m_apCharacter[2].SetPosition( m_vCharacterPos[2] );
	m_apCharacter[0].SetRotate( 'z', ANGLE( 185 ) );
	m_apCharacter[1].SetRotate( 'z', ANGLE( 172 ) );
	m_apCharacter[2].SetRotate( 'z', ANGLE( 170 ) );
	m_fCreateAngle = ANGLE( 200 );

	g_pGUI->Cleanup();

	ConnectRequestToGame();

	m_pSound = MAKE_COMPONENT( SoundObject2D );	

	return TRUE;
} //CSelectCharacter::Init

/**
 * 광원체 로드
 */
BOOL CSelectCharacter::LoadLightModel()
{
	LoadModelArray( m_apLightModel1,  "cut1_",   6 );
	LoadModelArray( m_apLightModel2,  "cut2_",   7 );
	LoadModelArray( m_apLightModel3,  "cut3_",   1 );
	LoadModelArray( m_apLightModel4,  "cut4_",   1 );
	LoadModelArray( m_apLightModel8,  "cut8_",   4 );
	LoadModelArray( m_apLightModel16, "cut16_",  1 );
	LoadModelArray( m_apLightModelde, "cut_de_", 4 );
	return TRUE;
} //CSelectCharacter::LoadLightModel

/**
 * 동작구분 당 라이트 로드
 */
BOOL CSelectCharacter::LoadModelArray( ARRAY_MODEL& aModel, const char* szKey, int nNum )
{
	Assert( szKey );

	IW3DModel*		pModel;
	char			szFileName[32];
	for( int nCnt = 1; nCnt <= nNum; nCnt++ )
	{
		pModel = MAKE_COMPONENT( Model );
		CHECK_PTR_RETURN( pModel );

		sprintf( szFileName, "%s%d.mod", szKey, nCnt );
		if( !pModel->Load(  "Object", szFileName ) )
		{
			Assert( !"로드실패" );
			SAFE_RELEASE( pModel );
			continue;
		} //if

		strcpy( szFileName + strlen( szFileName ) - 3, "ani" );
		if( pModel->LoadAnimation( "Object", szFileName ) )
		{
			DEBUG_OUT( szFileName + "에니파일 포함" );
		} //if

		aModel.push_back( pModel );
	} //for

	return TRUE;
} //CSelectCharacter::LoadModelArray

void CSelectCharacter::SelectProduce( const WORD wProduce )
{
	SAFE_DELETE( m_pProduce );

	char			szCamAniName[32] = "";
	switch( wProduce )
	{
	case TYPE_PRODUCE:	
		m_pProduce = reinterpret_cast<CProduce*>( new CSelectChar_Produce );
		strcpy( szCamAniName, "stage_cameraonly" );
		break;

	case TYPE_CREATE:		
		m_pProduce = reinterpret_cast<CProduce*>( new CSelectChar_Create );
		strcpy( szCamAniName, "stage_make_camera" );
		break;

	case TYPE_SELECT:		
		m_pProduce = reinterpret_cast<CProduce*>( new CSelectChar_Select );
		strcpy( szCamAniName, "stage_select_camera" );
		break;
	} //switch
	
	if( !m_pProduce )
	{
		Assert( !"할당실패" );
		return;
	} //if

	if( !m_pProduce->Create( szCamAniName, m_pCamera ) )
	{
		return;
	} //if

	m_dwTick = 0;
	m_pProduce->SetWorld( m_matWorld );
} //CSelectCharacter::SelectProduce

/**
 * 겜 접속 요청.
 */
void CSelectCharacter::ConnectRequestToGame()
{
	/*
#if defined( _SAMVERSION )
    if( g_pClient->Connect( g_asGameServerHost[g_btServer].c_str(), 8777 ) )
#elif defined( _CHINAVERSION )
    if( g_pClient->Connect( g_asGameServerHost[g_btServer].c_str(), 8205 ) )//8205 ) )  //8765 ) )
#else
    if( g_pClient->Connect( g_asGameServerHost[g_btServer].c_str(), 8999 ) )
#endif*/
	//if( g_pClient->Connect( g_asGameServerHost[g_btServer].c_str(), 2560 ) )  //8765 ) )
if( g_pClient->Connect( g_asGameServerHost[g_btServer].c_str(), 2560 ) )

	
	
	{
		int	nPatchVer = 0;
		nPatchVer = dPATCHVER;

		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_LOGINREQUEST )
			<< g_sUserGameId
			<< g_sUserDaumId
			<< nPatchVer;
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	}
	else
	{	// 서버에 접속 실패.
		g_pGUI->MessageBox( STR_SYSERR_NOTCONNECTSERVER, "ERROR", IW3DUInterface::MSG_OK );
	} //if..else
} //CSelectCharacter::ConnectRequestToGame

/**
 * 로그인 결과 패킷을 처리한다.
 */
void CSelectCharacter::OnPacketLoginResult()
{
	Assert( g_pClient );
	Assert( g_pGUI );

	BYTE	bResult;
	m_recvPacket	>> bResult;

	if( bResult == 2 )
	{ // 로그인 성공.
		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_GETALLCHAR );
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	}
	else if( bResult == 3 ) // 계정 블럭중인 경우
	{
		g_pGUI->MessageBox( STR_SYSMSG_ACCOUNTBLOCK, "ACCOUNTBLOCK", IW3DUInterface::MSG_OK );
	}
	else
	{
		m_bQuit = TRUE;
	} //if..eles
} // CSelectCharacter::OnPacketLoginResult

/**
 * Update.
 *
 * @return	계속 진행할지 여부
 */
BOOL CSelectCharacter::Update(
	const DWORD		dwTick,		///< [IN] 경과 Tick
	string*			psName		///< [OUT] 다른 게임 상태 프레임으로 넘길 경우,
								///<       넘어갈 프레임 이름
	)
{
	Assert( psName );
	Assert( g_pClient );
	Assert( g_pGUI );
	Assert( g_pMediaPlayer );	

	UpdateNetwork( dwTick );

	UpdateInput( dwTick );

	if( m_pProduce )
	{
		if( !m_pProduce->Update( dwTick ) )
		{
			if( m_pProduce->GetType() == TYPE_PRODUCE )
			{
				SelectProduce( TYPE_CREATE );
				g_pGUI->SetModeState( "[CHARCREATE_MODE]" );			
			} //if
		} //if
	} //if


	Vector3		vCamera = m_pCamera->GetPosition();
	Vector3		vDir	= m_vCharacterPos[1] - vCamera;
	m_pLight->SetPosition( vCamera );
	m_pLight->SetDirection( vDir );

	Render( dwTick );

	if( m_bQuit )
	{	// 종료.
		Sleep( 4000 );
		return FALSE;
	} //if

	if( m_bExitFrame )
	{
		if( m_bStartGame )
		{
			*psName = "Game";
		} //if

		return TRUE;
	} //if

	if( !m_bSound )
	{
		g_pMediaPlayer->StopAll();
	//	g_pMediaPlayer->Play( BGM_TYPE_EVENT, BGM_EVENT4 );

		m_bSound = TRUE;
	}

	m_dwTick += dwTick;

	return TRUE;
} //CSelectCharacter::Update

/**
 * Render.
 *
 * @param	dwTick		[IN] 경과 Tick
 */
void CSelectCharacter::Render( DWORD dwTick )
{
	Assert( m_pLight );

	if( !g_pScene->BeginFrame() )		return;

	if( m_pProduce )
	{
		Matrix4		matWorld;
		matWorld.SetIdentity();

		m_pLight->SetLight( 0 );

		m_pBackModel->SetWorld( m_matWorld );
		m_pBackModel->Render( dwTick );

		m_pLightModel->SetWorld( m_matWorld );
		m_pLightModel->Render( dwTick, FALSE );

		BYTE		btState = m_pProduce->GetAction();
		if( m_btState != btState )
		{
			m_dwStateTick = 0;
			m_btState = btState;
		} //if

		DrawCharacter( dwTick );
		DrawLightModel( btState, dwTick );
		m_pProduce->Draw();
	} //if

	g_pGUI->Draw();

	g_pScene->EndFrame();
} //CSelectCharacter::Render

/**
 * 캐릭터 그리기
 */
void CSelectCharacter::DrawCharacter( DWORD dwTick )
{
	if( m_pProduce->GetType() == TYPE_CREATE && m_nSelected != -1 )
	{ // 생성시
		if( m_btRotate == 1 )
		{
			m_fCreateAngle -= 0.10F;
			m_apCharacter[m_nSelected].SetRotate( 'z', m_fCreateAngle );
		}
		else if( m_btRotate == 2 )
		{
			m_fCreateAngle += 0.10F;
			m_apCharacter[m_nSelected].SetRotate( 'z', m_fCreateAngle );
		}
		m_apCharacter[m_nSelected].Draw( dwTick, m_dwTick );
	}
	else if( m_pProduce->GetType() == TYPE_SELECT )
	{
		m_apCharacter[0].SetRotate( 'z', ANGLE( 185 ) );
		m_apCharacter[1].SetRotate( 'z', ANGLE( 172 ) );
		m_apCharacter[2].SetRotate( 'z', ANGLE( 170 ) );

		for( int nCnt = 0; nCnt < NUM_SELECTCHAR; nCnt++ )
		{
			if( !m_apCharacter[nCnt].IsEmpty() )
			{ 
				m_apCharacter[nCnt].Draw( dwTick, m_dwTick + ( 300 * nCnt ) );
			} //if..else
		} //for
	} //if
} //CSelectCharacter::DrawCharacter

/**
 * 라이트 모델 그리기
 */
void CSelectCharacter::DrawLightModel( BYTE btAct, const DWORD dwTick )
{
	m_dwStateTick += dwTick;
	if( m_pProduce->GetType() == TYPE_PRODUCE )
	{
		switch( btAct )
		{
		case 1:
			DrawModelArray( m_apLightModel1, m_dwStateTick, FALSE );
			break;

		case 2:
			DrawModelArray( m_apLightModel2, m_dwStateTick, FALSE );
			break;

		case 3:
			DrawModelArray( m_apLightModelde, m_dwStateTick, FALSE );
			DrawModelArray( m_apLightModel3, m_dwStateTick, FALSE );
			break;

		case 4:
			DrawModelArray( m_apLightModelde, m_dwStateTick, FALSE );
			DrawModelArray( m_apLightModel4, m_dwStateTick, FALSE );
			break;

		case 5:
			//DrawModelArray( m_apLightModel5, m_dwStateTick, FALSE );
			break;

		case 6:	
			DrawModelArray( m_apLightModelde, m_dwStateTick, FALSE );
			break;

		case 7:
			//DrawModelArray( m_apLightModel7, m_dwStateTick, FALSE );	
			break;

		case 8:
			DrawModelArray( m_apLightModel8, m_dwStateTick, FALSE );
			break;

		case 9:
			DrawModelArray( m_apLightModelde, m_dwStateTick, FALSE );
			break;

		case 16:
			DrawModelArray( m_apLightModel16, m_dwStateTick, FALSE );
			break;

		default:
			DrawModelArray( m_apLightModelde, m_dwStateTick, FALSE );
			break;
		} //switch
	}
	else
	{
		DrawModelArray( m_apLightModelde, m_dwStateTick, FALSE );
	} //if..else
} //CSelectCharacter::DrawLightModel

/**
 * 배열 모델을 그린다
 */
void CSelectCharacter::DrawModelArray( ARRAY_MODEL& aModel, const DWORD dwTick, BOOL bLighting )
{
	for( UINT nCnt = 0; nCnt < aModel.size(); nCnt++ )
	{
		if( !aModel[nCnt] )
		{
			Assert( !"없는 포인터" );
			continue;
		} //if
		aModel[nCnt]->SetWorld( m_matWorld );
		aModel[nCnt]->Render( dwTick, bLighting, FALSE );
	} //for
} //CSelectCharacter::DrawModelArray

/**
 * 입력 처리.
 *
 * @param	dwTick		[IN] 경과 Tick
 *
 * @return	진행(TRUE)/종료(FALSE)
 */
BOOL CSelectCharacter::UpdateInput( DWORD dwTick )
{
	Assert( g_pGUI );
	Assert( g_pClient );

	g_pInput->Update();

	if( g_pGUI->PopEvent( g_szUIName, &g_UIevent, &g_dwUIParam1, &g_dwUIParam2 ) )
	{
		switch( g_UIevent )
		{
		case EVENT_BUTTON:		OnEventButton( g_szUIName, g_dwUIParam2 );		break;
		case EVENT_RETURNKEY:	OnEventReturnKey( g_szUIName );					break;
		case EVENT_KEYDOWN:		OnEventKeyDown( g_dwUIParam1 );					break;
		case EVENT_CREATECHAR:	OnEventCreateCharacter();						break;
		} //switch
	} //if

	g_pGUI->Update( dwTick );

	return TRUE;
} //CSelectCharacter::UpdateInput

/**
 * 네트웍 처리.
 *
 * @param	dwTick		[IN] 경과 Tick
 *
 * @return	서버와 접속 여부.
 */
BOOL CSelectCharacter::UpdateNetwork( DWORD dwTick )
{
	Assert( g_pClient );
	Assert( g_pGUI );

	if( !g_pClient->Recv() )
	{
		g_pClient->Disconnect();
	} //if

	if( !g_pClient->IsConnect() )
	{
		static BOOL		s_bConnect = TRUE;
		if( s_bConnect )
		{
			g_pGUI->MessageBox(	STR_SYSERR_SERVERDISCONNECT, "ERROR", IW3DUInterface::MSG_OK );
			s_bConnect = FALSE;
		} //if
	} //if

	static DWORD	s_dwTick = 0;
	if( s_dwTick > 11 * 1000 )
	{	// 10초마다 Heart Beat 패킷을 보냄.
		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_HEARTBEAT );
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

		s_dwTick = 0;
	}
	else
	{
		s_dwTick += dwTick;
	} //if..else..

//	g_pClient->Send();

	while( g_pClient->GetRecvSize() > 2 )
	{	
		char*		pData = const_cast<char*>( g_pClient->GetRecvData() );
		int			nSize = *reinterpret_cast<WORD*>( pData );
		
		if( nSize > g_pClient->GetRecvSize() )	break;
		
		//여기서 압축 푼다
		char	pDecryptData[4096];
		DWORD	key[4] = { dAMHO_KEY1, dAMHO_KEY2, dAMHO_KEY3, dAMHO_KEY4 };
		DWORD	dwEncSize = TEA_DecryptData( pData + 2, pDecryptData, nSize - 2, key );
		
		char	pDec[4096];
		memcpy( pDec, &pDecryptData[2], dwEncSize );
		dwEncSize -= 2;  
		
		m_recvPacket.SetPacket( dwEncSize + 2, pDec );
		ProcessRecvPacket();
		g_pClient->RemoveRecvData( nSize ); 

	//	m_recvPacket.SetPacket( nSize, pData + 2 );
	//	ProcessRecvPacket();
	//	g_pClient->RemoveRecvData( nSize );
		
		if( m_bExitFrame )	break;	// 쀍 !!!
	} //while

	return TRUE;
} //CSelectCharacter::UpdateNetwork

/**
 * 선택된 캐릭터를 보이기 위한 캐릭터 모델 갱신 작업을 한다.
 *
 * @param	bAnyShow	[IN] 무조건 모델을 보이게 할지 여부.
 */
void CSelectCharacter::UpdateCharacter( const int nNum, BOOL bAnyShow )
{
	char	szName[64];		// 이름
	BYTE	bSex;			// 성별

	char		aszModelName[MAX_PARTS][64];

	g_pGUI->GetCharSelModelName(
		nNum,
		szName,
		&bSex,
		aszModelName[HAIR_PART],
		aszModelName[FACE_PART],
		aszModelName[UPPER_PART],
		aszModelName[LOWER_PART],
		aszModelName[HAND_PART],
		aszModelName[FOOT_PART]
	);

	if( bAnyShow || *szName )
	{
		m_apCharacter[nNum].Create(
			bSex,
			aszModelName[HAIR_PART],
			aszModelName[FACE_PART],
			aszModelName[UPPER_PART],
			aszModelName[LOWER_PART],
			aszModelName[HAND_PART],
			aszModelName[FOOT_PART] );
	}
	else
	{
		m_apCharacter[nNum].Clear();
	} //if..else..
} //CSelectCharacter::UpdateCharacter

/**
 * 받은 패킷 처리.
 */
void CSelectCharacter::ProcessRecvPacket()
{
	switch( m_recvPacket.GetTag() )
	{
	case dPACKET_LOGINRESULT:				OnPacketLoginResult();				break;
	case dPACKET_CHARINFO:					OnPacketCharInfo();					break;
	case dPACKET_GAMESTART:					OnPacketGameStart();				break;
	case dPACKET_GETALLCHAR_RESULT:			OnPacketGetAllCharResult();			break;
	case dPACKET_NULL_CHARACTER:			OnPacketNullCharacter();			break;
	case dPACKET_MAKECHAR_RESULT:			OnPacketMakeCharResult();			break;
	case dPACKET_DELETE_CHARACTER_RESULT:	OnPacketDeleteCharacterResult();	break;
	case dPACKET_ALREADY_CONNECTED:			OnPacketAlreadyConnected();			break;
	case dPACKET_SELCHAR_SYSTEMMSG:			OnPacketSelCharSystemMsg();			break;
	} //switch
} //CSelectCharacter::ProcessRecvPacket

  /**
 * UI 버튼 눌림.
 *
 * @param	sName		[IN] 발생 컴포넌트 이름.
 */
void CSelectCharacter::OnEventButton( const char* szUIName, DWORD dwParam2 )
{
	Assert( g_pGUI );
	Assert( g_pClient );

	if( !dwParam2 )
	{ 
		if( !strcmp( szUIName, "BTN_CHARSEL_CREATE" ) )
		{
			CharacterCreate();
			for( int i = 0 ; i < 3 ; i++ )
			{
				m_apCharacter[i].SetSelect( FALSE );
			} //for			
		}
		else if( !strcmp( szUIName, "BTN_CHARSEL_SELECT" ) ||
				 !strcmp( szUIName, "BTN_CHARSELECT" ) )
		{	// 캐릭터 선택.
			SelectCharacter( m_nSelected );
		}
		else if( !strcmp( szUIName, "BTN_CHARSEL_DELETE" ) )
		{	// 캐릭터 삭제.
			char	szText[128];
#ifndef _CHINAVERSION
			sprintf( szText, "%s\n%s", g_pGUI->GetText( "TXT_CHARSELINFO_NAME" ), STR_CHARSEL_DELETE );
#else
		//	sprintf( szText, "%s%s%s", STR_CHARSEL_DELETE, g_pGUI->GetText( "TXT_CHARSELINFO_NAME" ), STR_CHARSEL_DELETE2 );
			sprintf( szText, "%s", STR_CHARSEL_DELETE );
#endif
			g_pGUI->MessageBox(	szText,	"DELETE", IW3DUInterface::MSG_YESNO );

			for( int i = 0 ; i < 3 ; i++ )
			{
				m_apCharacter[i].SetSelect( FALSE );
			} //for
			g_pGUI->SetRefreshNewCharacter();
			g_pGUI->SetCharacterInitPOINT();
		}
		else if( !strcmp( szUIName, "BTN_MSG_DELETE_YES" ) )
		{	// 캐릭터 삭제 확인창에서 YES를 고른 경우.
			CPacket		sendPacket;
			sendPacket.Tag( dPACKET_DELETE_CHARACTER )
				<< m_nSelected;
			g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
		}

		else if( !strcmp( szUIName, "BTN_CHARACTER1" ) )
		{	// 캐릭터 선택 화면에서 1번 캐릭터.
			SelectCharacter( 0 );
		}
		else if( !strcmp( szUIName, "BTN_CHARACTER2" ) )
		{	// 캐릭터 선택 화면에서 2번 캐릭터.
			SelectCharacter( 1 );
		}
		else if( !strcmp( szUIName, "BTN_CHARACTER3" ) )
		{	// 캐릭터 선택 화면에서 3번 캐릭터 .
			SelectCharacter( 2 );
		}
		else if( !strcmp( szUIName, "BTN_CHARNEW_CANCEL" ) )
		{	// 캐릭터 생성 화면에서 취소 버튼 눌림.
			if( m_nSelected >= 0 )
			{
				m_apCharacter[m_nSelected].Clear();
				SelectProduce( TYPE_SELECT );
				g_pGUI->SetModeState( "[CHARSELECT_MODE]" );	// 캐릭터 선택 준비창
				m_nSelected = -1;

				for( int i = 0 ; i < 3 ; i++ )
				{
					m_apCharacter[i].SetSelect( FALSE );
				} //for

				m_bSound = FALSE;
			} //if
		}
		else if( !strcmp( szUIName, "BTN_CHARSEL_CANCEL" ) )
		{ // 
			if( m_nSelected < 0 )
			{
				g_pClient->Disconnect();
				m_bQuit = TRUE;
			}
			else if( m_pProduce && m_pProduce->GetType() == TYPE_SELECT )
			{
				CSelectChar_Select*		pProduce_Sel = reinterpret_cast<CSelectChar_Select*>( m_pProduce );
				pProduce_Sel->CharacterSelect( -1 );
				g_pGUI->SetModeState( "[CHARSELECT_MODE]" );
				m_nSelected = -1;

				for( int i = 0 ; i < 3 ; i++ )
				{
					m_apCharacter[i].SetSelect( FALSE );
				} //for
			} //if..else
		}
		else if( !strcmp( szUIName, "BTN_CHARNEW_SEX_L" )   ||
				 !strcmp( szUIName, "BTN_CHARNEW_SEX_R"  )  ||
				 !strcmp( szUIName, "BTN_CHARNEW_HAIR_L" )  ||
				 !strcmp( szUIName, "BTN_CHARNEW_HAIR_R" )  ||
				 !strcmp( szUIName, "BTN_CHARNEW_FACE_L" )  ||
				 !strcmp( szUIName, "BTN_CHARNEW_FACE_R" )  ||
				 !strcmp( szUIName, "BTN_CHARNEW_COAT_L" )  ||
				 !strcmp( szUIName, "BTN_CHARNEW_COAT_R" )  ||
				 !strcmp( szUIName, "BTN_CHARNEW_PANTS_L" ) ||
				 !strcmp( szUIName, "BTN_CHARNEW_PANTS_R" ) )
		{	// 캐릭터 생성 화면에서 부분 교체.
			UpdateCharacter( m_nSelected, TRUE );
		}
		else if( !strcmp( szUIName, "BTN_CHARNEWVIEW_L" ) ||
				 !strcmp( szUIName, "BTN_CHARNEWVIEW_R" ) )
		{
			m_btRotate = 0;
		}
		else if( !strcmp( szUIName, "CHK_CHARNEWVIEW_ZOOM" ) )
		{
			CSelectChar_Create*		pProduce_Cre = reinterpret_cast<CSelectChar_Create*>( m_pProduce );
			pProduce_Cre->SetCameraMode( CSelectChar_Create::CAM_BODY );
		}
		else if( !strcmp( szUIName, "BTN_MSG_ERROR_OK" ) ||
				 !strcmp( szUIName, "BTN_MSG_ACCOUNTBLOCK_OK" ) )
		{	// 이미 접속 중인 ID 등의 에러. 확인.
			g_pClient->Disconnect();
			m_bQuit = TRUE;
		} //if.. else if..
	}
	else
	{
		if( !strcmp( szUIName, "BTN_CHARNEWVIEW_L" ) )
		{
			m_btRotate = 1;
		}
		else if( !strcmp( szUIName, "BTN_CHARNEWVIEW_R" ) )
		{
			m_btRotate = 2;
		}
		else if( !strcmp( szUIName, "CHK_CHARNEWVIEW_ZOOM" ) )
		{
			CSelectChar_Create*		pProduce_Cre = reinterpret_cast<CSelectChar_Create*>( m_pProduce );
			pProduce_Cre->SetCameraMode( CSelectChar_Create::CAM_FACE );
		} //if..else
	} //if
} //CSelectCharacter::OnEventButton

/**
 * UI 엔터 눌림.
 *
 * @param	sName		[IN] 발생 컴포넌트 이름.
 */
void CSelectCharacter::OnEventReturnKey( const char* szUIName )
{
	/* NULL */
} //CSelectCharacter::OnEventReturnKey

/**
 * UI 키 업.
 */
void CSelectCharacter::OnEventKeyDown( const dwParam1 )
{
	CSelectChar_Select*		pProduce_Sel = reinterpret_cast<CSelectChar_Select*>( m_pProduce );
	
	switch( dwParam1 )
	{
	case DIK_ESCAPE:			// 'ESC'
		if( m_pProduce && 
			m_pProduce->GetType() == TYPE_PRODUCE )
		{
			SelectProduce( TYPE_CREATE );
			g_pGUI->SetModeState( "[CHARCREATE_MODE]" );

			g_pMediaPlayer->StopAll();
		}
		else if( m_pProduce && 
				 m_pProduce->GetType() == TYPE_SELECT &&
				 m_nSelected != -1)
		{
			CSelectChar_Select*		pProduce_Sel = reinterpret_cast<CSelectChar_Select*>( m_pProduce );
			pProduce_Sel->CharacterSelect( -1 );
			g_pGUI->SetModeState( "[CHARSELECT_MODE]" );
			m_nSelected = -1;

			for( int i = 0 ; i < 3 ; i++ )
			{
				m_apCharacter[i].SetSelect( FALSE );
			} //for
		} //if
		break;
	case DIK_F1:		pProduce_Sel->CharacterSelect( 0 );		break;
	case DIK_F2:		pProduce_Sel->CharacterSelect( 1 );		break;
	case DIK_F3:		pProduce_Sel->CharacterSelect( 2 );		break;
	} //switch

	if( m_pProduce &&
		m_pProduce->GetType() == TYPE_SELECT )
	{
		CSelectChar_Select*		pProduce_Sel = reinterpret_cast<CSelectChar_Select*>( m_pProduce );
		switch( dwParam1 )
		{
		case DIK_1:		pProduce_Sel->CharacterSelect( 0 );		break;
		case DIK_2:		pProduce_Sel->CharacterSelect( 1 );		break;
		case DIK_3:		pProduce_Sel->CharacterSelect( 2 );		break;
		} //switch
	}
	else if( m_pProduce &&
		m_pProduce->GetType() == TYPE_CREATE )
	{
		CSelectChar_Create*		pProduce_Cre = reinterpret_cast<CSelectChar_Create*>( m_pProduce );
		switch( dwParam1 )
		{
		case DIK_1:		pProduce_Cre->SetCameraMode( CSelectChar_Create::CAM_FACE );		break;
		case DIK_2:		pProduce_Cre->SetCameraMode( CSelectChar_Create::CAM_BODY );		break;
		} //switch
	} //if


} //CSelectCharacter::OnEventKeyDown

/**
 * 캐릭터 생성 버튼 눌림.
 */
void CSelectCharacter::OnEventCreateCharacter()
{
	char	szName[64];		// 이름
	BYTE	bSex;			// 성별
	WORD	wHair;			// 헤어 스타일
	WORD	wFace;			// 얼굴
	WORD	wUpper;			// 상의
	WORD	wLower;			// 하의
	WORD	wHand;			// 장갑
	WORD	wFoot;			// 신발
	int		iNum;			// 캐릭터 번호
	BYTE	btSTR;			// STR
	BYTE	btINT;			// DEX
	BYTE	btDEX;			// INT
	BYTE	btClass;		// 직업

	g_pGUI->GetCharSelInfo(
		szName,
		&bSex,
		&wHair,
		&wFace,
		&wUpper,
		&wLower,
		&wHand,
		&wFoot,
		&iNum,
		&btSTR,
		&btINT,
		&btDEX,
		&btClass
		);

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MAKECHAR )
		<< bSex
		<< wHair
		<< wFace
		<< wUpper
		<< wLower
		<< wHand
		<< wFoot
		<< btSTR
		<< btINT
		<< btDEX
		<< (BYTE)iNum
		<< szName;
	
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );	
} //CSelectCharacter::OnEventCreateCharacter

/**
 * 게임용 캐릭터 정보.
 * 게임에 들어가기 바로 직전에 받는다.
 */
void CSelectCharacter::OnPacketCharInfo()
{
	char	szName[256];

	BYTE	btClass;		// 국가
	g_infoPlayer.nCash = 0;

	// 타고있는 것의 번호는 0 일 경우 
	// 타고 있는게 아무것도 없는것임
	//여기서 클라이언트 버전을 만든다.
	DWORD	m_dVer = 0;
	DWORD	test=0;
	//DWORD	m_dtest = g_dwPlayerId << 16 | m_dVer;

	m_recvPacket
		>> szName
		>> g_infoPlayer.bSex
		>> g_dwPlayerId
		>> g_infoPlayer.nHp[0]
		>> g_infoPlayer.nHp[1]
		>> g_infoPlayer.nMp[0]
		>> g_infoPlayer.nMp[1]
		>> g_infoPlayer.wStr
		>> g_infoPlayer.wInt
		>> g_infoPlayer.wDex
		>> g_infoPlayer.wLevel[0]
		>> g_infoPlayer.wLevel[1]
		>> g_infoPlayer.wLevel[2]
		>> g_infoPlayer.wLevel[3]
		>> g_infoPlayer.nExp[0][0]
		>> g_infoPlayer.nExp[1][0]
		>> g_infoPlayer.nExp[2][0]
		>> g_infoPlayer.nExp[3][0]
		>> g_infoPlayer.nExp[0][1]
		>> g_infoPlayer.nExp[1][1]
		>> g_infoPlayer.nExp[2][1]
		>> g_infoPlayer.nExp[3][1]
		>> g_infoPlayer.nGold
		>> g_infoPlayer.nCash
		>> g_infoPlayer.wRide
		>> g_infoPlayer.btSpeed
		>> g_infoPlayer.nCha
		>> g_infoPlayer.btClass
		>> g_infoPlayer.nGuildId;
		// from sk8snow
	
	g_infoPlayer.sName		= szName;
/*	g_dwPlayerId = test >> 16;
	m_dVer = test & 0xff;
	
/*	if( m_dVer != 100 )
	{
		// dPACKET_CHARACTER_DISCONN 보내기
		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_CHARACTER_DISCONN );
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
		//버전 틀리다라는 메시지 보내기
		//MessageBox( g_hWnd, STR_SYSERR_ERRUSER, STR_SYSERR_ERRORMESSAGE, MB_OK );
		g_pGUI->MessageBox( STR_SYSERR_ERRUSER );
	}*/
	
	BOOL bMaster = FALSE;
	if( _MASTER == g_infoPlayer.bSex )
	{
		bMaster = TRUE;
	}
	
	int		iNum;			// 캐릭터 번호 ( for OUT parameter )
	BYTE	btSTR;			// STR
	BYTE	btINT;			// DEX
	BYTE	btDEX;			// INT

	g_pGUI->GetCharSelInfo(
		szName,
		&g_infoPlayer.bSex,
		&g_infoPlayer.wHair,
		&g_infoPlayer.wFace,
		&g_infoPlayer.wUpper,
		&g_infoPlayer.wLower,
		&g_infoPlayer.wHand,
		&g_infoPlayer.wFoot,
		&iNum,
		&btSTR,
		&btINT,
		&btDEX,
		&btClass
		);

	g_pGUI->SetJobName( btClass );
} //CSelectCharacter::OnPacketCharInfo

/**
 * 게임 시작.
 */
void CSelectCharacter::OnPacketGameStart()
{
	DEBUG_OUT( "게임 시작 성공" );
	m_bExitFrame = TRUE;
	m_bStartGame = TRUE;
	
	IW3DItemInfoMgr*		pItemInfoMgr = MAKE_COMPONENT( ItemInfoMgr );
	if( !pItemInfoMgr )
	{
		Assert( pItemInfoMgr );
		return;
	} //if
	pItemInfoMgr->SetSex( g_infoPlayer.bSex );
} //CSelectCharacter::OnPacketGameStart

/**
 * 선택할 수 있는 캐릭터 정보를 얻음.
 */
void CSelectCharacter::OnPacketGetAllCharResult()
{
	Assert( g_pGUI );
	Assert( m_apCharacter );
	
	g_pGUI->InitCharSelectInfo();

	BOOL		bIsEmpty = TRUE;
	for( int i = 0 ; i < 3 ; i++ )
	{
		char	szName[256];	// 이름
		BYTE	bSex;			// 성별
		WORD	wHair;			// 머리
		WORD	wFace;			// 얼굴
		WORD	wItem[5];		// 장착 아이템
		BYTE	bNum;			// 캐릭터 번호
		int		nLevel;			// 통합 레벨
		int		nSLevel;		// 근거리 레벨
		int		nLLevel;		// 원거리 레벨
		int		nBLevel;		// 흑마법 레벨
		int		nWLevel;		// 백마법 레벨
		BYTE	btClass;		// 직업 

		m_recvPacket
			>> szName
			>> nSLevel
			>> nLLevel
			>> nBLevel
			>> nWLevel
			>> bSex
			>> wHair
			>> wFace
			>> wItem[0]
			>> wItem[1]
			>> wItem[2]
			>> wItem[3]
			>> wItem[4]
			>> btClass
			>> bNum;

		// 통합 레벨 
		nLevel = nSLevel + nLLevel + nBLevel + nWLevel;

		if( !strcmp( szName, "UNUSE" ) )
		{
			// 비어있는 캐릭터.
		}
		else
		{
			g_pGUI->SetCharSelInfo(
				szName,
				nLevel,
				nLLevel,
				nSLevel,
				nBLevel,
				nWLevel,
				bSex,
				wHair,
				wFace, 
				wItem[1],
				wItem[2],
				wItem[4],
				wItem[3],
				btClass,
				bNum );
			
				UpdateCharacter( bNum );
				bIsEmpty = FALSE;
		} //if..else..
	} //for

	if( bIsEmpty )
	{
		CharacterCreate();
	}
	else
	{
		SelectProduce( TYPE_SELECT );
		g_pGUI->SetModeState( "[CHARSELECT_MODE]" );	// 캐릭터 선택 준비창
	} //if..else
} //CSelectCharacter::OnPacketGetAllCharResult

/**
 * 캐릭터 생성 모드로 돌입함.
 */
void CSelectCharacter::OnPacketNullCharacter()
{
	DEBUG_OUT( "캐릭터 생성" );

	g_pGUI->SetModeState( "[CHARCREATE_MODE]" );	// 캐릭터 생성창
} //CSelectCharacter::OnPacketNullCharacter

/**
 * 캐릭터 생성 결과.
 */
void CSelectCharacter::OnPacketMakeCharResult()
{
	BYTE	bResult;
	m_recvPacket	>> bResult;

	switch( bResult )
	{
	case 0:		// 예외적 오류. 연결 종료.
		g_pClient->Disconnect();
		break;

	case 1:		// 이미 같은 이름의 캐릭터가 존재함.
		g_pGUI->SetModeState( "[CHARCREATE_MODE]" );	// 캐릭터 생성창

		g_pGUI->MessageBox( STR_CHARSEL_EXISTNAME, NULL, IW3DUInterface::MSG_OK );

		break;

	case 2:		// 캐릭터 만들기 성공.
		g_pGUI->SetModeState( "[CHARSELECT_MODE]" );	// 캐릭터 생성창
		SelectProduce( TYPE_SELECT );
		m_apCharacter[m_nSelected].SetPosition( m_vCharacterPos[m_nSelected] );
		m_apCharacter[m_nSelected].SetRotate( 'z', ANGLE( 200 ) );
		m_nSelected = -1;
		
		m_bSound = FALSE;
		break;
	} //switch

	m_apCharacter[0].SetRotate( 'z', ANGLE( 185 ) );
	m_apCharacter[1].SetRotate( 'z', ANGLE( 172 ) );
	m_apCharacter[2].SetRotate( 'z', ANGLE( 170 ) );
} //CSelectCharacter::OnPacketMakeCharResult

/**
 * 캐릭터 삭제 결과.
 */
void CSelectCharacter::OnPacketDeleteCharacterResult()
{
	BYTE	bResult;
	m_recvPacket	>> bResult;

	switch( bResult )
	{
	case 0:		// 실패
		g_pGUI->MessageBox( STR_CHARSEL_FAILDELETE,	NULL, IW3DUInterface::MSG_OK );
		break;

	case 1:		// 성공
		m_apCharacter[m_nSelected].Clear();
		g_pGUI->DeleteCharInfo( m_nSelected );
		CSelectChar_Select*		pProduce_Sel = reinterpret_cast<CSelectChar_Select*>( m_pProduce );
		pProduce_Sel->CharacterSelect( -1 );
		g_pGUI->SetModeState( "[CHARSELECT_MODE]" );			// 캐릭터 선택창
		m_nSelected = -1;
		break;
	} //switch
} //CSelectCharacter::OnPacketDeleteCharacterResult

/**
 * 이미 접속 중인 아이디.
 */
void CSelectCharacter::OnPacketAlreadyConnected()
{
	Assert( g_pGUI );

   g_pGUI->MessageBox( STR_SYSERR_ALREADYCONNECTED, "ERROR", IW3DUInterface::MSG_OK );
   //이미 접속중인 아이디 접속끊기 ^^;


   
} //CSelectCharacter::OnPacketAlreadyConnected

#ifdef _dGUILD_WAR_CHINA_	//중국 길드 대항전관련
/**
 * 허용된 접속시간아님.	
 */
void CSelectCharacter::OnPacketNotConnected()
{
	g_pGUI->MessageBox( STR_SYSERR_NOTCONNECTED, "ERROR", IW3DUInterface::MSG_OK ); //이세호
}//CGame::OnPacketNotConnected()


/**
 * 허용된 접속유저 아님.
 */
void CSelectCharacter::OnPacketErrorUser()
{
	g_pGUI->MessageBox( STR_SYSERR_ERRUSER, "ERROR", IW3DUInterface::MSG_OK );
}
#endif


/**
 * 캐릭터 선택.
 */
void CSelectCharacter::SelectCharacter( const int nNum )
{
	if( nNum < 0 || nNum >= NUM_SELECTCHAR )
	{
		return;
	} //if

	if( !m_pProduce || m_pProduce->GetType() != TYPE_SELECT )
	{
		return;
	} //if

	if( !m_apCharacter[nNum].IsEmpty() )
	{ 
		if( m_nSelected == nNum )
		{ // 재 선택
			CPacket		sendPacket;
			sendPacket.Tag( dPACKET_SELECT_CHARACTER )
				<< m_nSelected;
			g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

			g_pGUI->SetModeState( "[NONE_MODE]" );			
		}
		else
		{
			CSelectChar_Select*		pProduce_Sel = reinterpret_cast<CSelectChar_Select*>( m_pProduce );
			pProduce_Sel->CharacterSelect( nNum );

			for( int i = 0 ; i < 3 ; i++ )
			{
				m_apCharacter[i].SetSelect( FALSE );
			} //for

			m_apCharacter[nNum].SetSelect( TRUE );	
			
			if( m_pSound && m_apCharacter[nNum].GetSex() )
			{
				m_pSound->Play( "SelectWoman.wav" );				
			}
			else
			{
				m_pSound->Play( "SelectMan.wav" );
			}
		} //if..else
		m_nSelected = nNum;

	} //if..else
} //CSelectCharacter::SelectCharacter

void CSelectCharacter::CharacterCreate()
{
	int		nIndex = g_pGUI->CreateCharacter();
	if( nIndex < 0 || NUM_SELECTCHAR <= nIndex )
	{
		return;
	} //if

	m_apCharacter[nIndex].SetPosition( m_vCharacterPos[NUM_SELECTCHAR] );
	UpdateCharacter( nIndex, TRUE );
	m_nSelected = nIndex;

	SelectProduce( TYPE_PRODUCE );
	g_pGUI->SetModeState( "[NONE_MODE]" );
} //CSelectCharacter::CharacterCreate

/*
 *	캐릭터 셀렉트에서의 시스템 메시지
 */
void CSelectCharacter::OnPacketSelCharSystemMsg()
{
	char szMsg[256];

	m_recvPacket
		>> szMsg;

	g_pGUI->MessageBox( szMsg, "AlreadyConnected", IW3DUInterface::MSG_OK );
}// CGame::OnPacketSelCharSystemMsg()