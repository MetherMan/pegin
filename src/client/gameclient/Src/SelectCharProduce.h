/**
 * 캐릭터 선택/생성 화면 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 34 $
 *
 * <HR>
 * $Log: /GameClient/Src/SelectCharProduce.h $
 * 
 * 34    05-02-18 7:38p Sk8snow2
 * 
 * 33    04-06-23 12:08p Sckyj
 * 
 * 32    04-01-05 5:33p Sckyj
 * BGM 관련 코드 수정
 * 
 * 31    04-01-05 3:43p Sckyj
 * BGM 재생관련 코드 수정
 * 
 * 30    03-12-26 3:46p Sckyj
 * 캐릭터 선택시 효과음 추가
 * 
 * 29    03-12-22 1:08a Paran
 * 
 * 28    03-12-19 2:02a Sckyj
 * 
 * 27    03-12-19 12:43a Kjmgo
 * 
 * 26    03-12-18 11:28p Paran
 * 
 * 25    03-12-15 4:42p Paran
 * 
 * 24    03-12-15 11:43a Paran
 * 
 * 23    03-12-14 6:10p Sckyj
 * BGM 재생관련 코드 수정
 * 
 * 22    03-12-14 2:50p Paran
 * 
 * 21    03-12-12 6:45p Paran
 * 
 * 20    03-12-12 11:57a Paran
 * 
 * 19    03-12-09 8:59p Sckyj
 * Character Select Mode로 넘어갈경우 Login 음악을 꺼지도록 수정
 * 
 * 18    03-12-08 8:50p Paran
 * 
 * 17    03-11-12 10:07a Paran
 * 
 * 15    03-10-03 9:10p Sckyj
 * 
 * 14    03-10-03 5:53p Sckyj
 * 
 * 13    03-09-17 10:38a Sckyj
 * 
 * 12    03-09-07 4:39a Paran
 * 
 * 11    03-09-03 6:21p Sckyj
 * 
 * 10    03-09-03 10:59a Sckyj
 * 
 * 9     03-09-02 8:50p Sckyj
 * 
 * 8     03-08-29 9:19a Kjmgo
 * Animation 바꿈
 * 
 * 7     03-08-14 2:40p Paran
 * 
 * 6     03-06-27 5:43p Paran
 * 
 * 5     03-06-16 8:31p Paran
 * 
 * 3     03-06-15 12:12a Paran			SkipPrologue 추가.
 *
 * <HR>
 *
 * @file	SelectCharProduce.cpp
 */

#ifndef __CSelectCharProduce_Header__
#define __CSelectCharProduce_Header__

#include <map>
#include <list>
#include <vector>
#include <string>

using namespace std;

#define	NUM_BACKMODEL			2
#define TYPE_PRODUCE			1
#define TYPE_SELECT				2
#define TYPE_CREATE				3

/**
 * 에니 모델
 */
class CAniModel
{
public:
	/**
	 * 생성자
	 */
	CAniModel()
		:	m_pAnimation	( NULL ),
			m_pModel		( NULL ),
			m_pAction		( NULL ),
			m_dwTick		( 0 )
	{
	} //CAniModel

	/**
	 * 소멸자
	 */
	~CAniModel()
	{
		ActionMap::iterator		iter = m_mapAction.begin();
		while( iter != m_mapAction.end() )
		{
			SAFE_DELETE( iter->second );
			iter++;
		} //while
		SAFE_RELEASE( m_pAnimation );
		SAFE_RELEASE( m_pModel );
	} //~CAniModel
	
	/**
	 * 로드
	 */
	BOOL		Load( const char* szPath, const char* szKeyName )
	{
		m_pModel = MAKE_COMPONENT( Model );
		CHECK_PTR_RETURN( m_pModel );

		std::string		strFileName;

		strFileName = szKeyName;
		strFileName.append( ".mod" );
		if( !m_pModel->Load( szPath, strFileName.c_str() ) )
		{
			Assert( !"로드실패" );
			return FALSE;
		} //if

		if( !LoadAction( szPath, szKeyName ) )
		{
			Assert( !"로드실패" );
			return FALSE;
		} //if

		return TRUE;
	} //Load

	int			GetActionNum()
	{
		return m_mapAction.size();
	} //GetActionNum

	/**
	 * 현재 메트릭스
	 */
	Matrix4		GetTMMatrix()
	{
		if( !m_pModel )
		{
			Assert( m_pModel );
			Matrix4		matTm;
			matTm.SetIdentity();
			return matTm;
		} //if
		return m_pModel->GetTMMatrix( m_dwTick );
	} //GetTMMatrix

	/**
	 * 동작 부분 설정.
	 */
	void	SetAction( int nAction )
	{
		ActionMap::iterator itor = m_mapAction.find( nAction );

		if( itor == m_mapAction.end() )
		{
			m_pAction = NULL;
			return ;
		} //if

		m_pAction = itor->second;
		m_pModel->SetAction( m_pAction );
		
		m_dwTick = 0;
	} //SetAction

	/**
	 * 월드 매트릭스 적용
	 */
	void		SetWorld( const Matrix4& matWorld )
	{
		if( !m_pModel )
		{
//			Assert( m_pModel );
			return;
		} //if
		m_pModel->SetWorld( matWorld );
	} //SetWorld

	/**
	 * 현재 동작 완료 체크
	 */
	BOOL		IsEndAction()
	{
		if( m_pModel->GetActionTicks() <= m_dwTick )
		{
			return TRUE;
		} //if
		return FALSE;
	} //IsEndAction

	/**
	 * 업뎃
	 */
	BOOL		Update( const DWORD dwTick )
	{
		m_dwTick += dwTick;
		return TRUE;
	} //Update

	/**
	 * 렌더
	 */
	void		Render()
	{
		if( !m_pModel )
		{
	//		Assert( !"모델" );
			return;
		} //if

		if( !m_pAction )
		{
			return;
		} //if
		m_pModel->Render( m_dwTick, TRUE );
	} //Render

private:
	/**
	 * 동작 파일 로드
	 */
	BOOL	LoadAction( const char* szPath, const char* szFile )
	{
		string	sFile = szPath;
		sFile.append( "/" );
		sFile.append( szFile );
		sFile.append( ".act" );

		FILE* fp = fopen( sFile.c_str(), "rt" );

		if( NULL == fp )
		{
			Assert( !"파일이 없습니다." );
			return FALSE; 
		} //if

		sFile = szPath;
		sFile.append( "/" );
		sFile.append( szFile );
		sFile.append( ".ani" );

		SAFE_RELEASE( m_pAnimation );
		m_pAnimation = MAKE_COMPONENT( Animation );
		if( !m_pAnimation || !m_pAnimation->Load( sFile.c_str() ) )
		{
			fclose( fp );
			return FALSE;
		} //if

		int		nActionNum = 0;
		char	szBuffer[256];
		char	szKeyword[80];
		while( !feof( fp ) )
		{
			fgets( szBuffer, 256, fp );
			*szKeyword = NULL;
			sscanf( szBuffer, "%s", szKeyword );

			if( !strcmp( szKeyword, "[ACTION]" ) )
			{
				char szWord[80];
				SAction* pAction = new SAction;
				if( sscanf( szBuffer, "%s %s %d %d", szKeyword, szWord, &pAction->nStart, &pAction->nEnd ) != 4 )
				{
					Assert( !"sscanf 오류" );
				}


				pAction->nFrames = ( pAction->nEnd - pAction->nStart ) + 1;
				pAction->pAnimation = m_pAnimation;

				nActionNum = atoi( szWord );
				m_mapAction.insert( std::make_pair( nActionNum, pAction ) );
			} //if
		} //while

		fclose( fp );
		return TRUE;
	} //LoadAction

private:
	IW3DModel*		m_pModel;						///< 모델
	IW3DAnimation*  m_pAnimation;					///< 애니메이션
	SAction*		m_pAction;						///< 동작
	DWORD			m_dwTick;						///< 동적 틱

	typedef			std::map<int, SAction*>	ActionMap;
	ActionMap		m_mapAction;					///< 동작 Map
}; //class CAniModel

/**
 * 연출 클래스
 */
class CProduce
{
public:
	CProduce()
	{
		m_wState	 = 0;
		m_nActionNum = 0;
		m_btType	 = 0;
	} //CProduce

	virtual ~CProduce(){} //~CProduce

	virtual BOOL		Create( const char* szCameraModel, const IW3DCamera* pCamera )
	{
		Assert( pCamera );
		Assert( szCameraModel );

		m_pCamera = const_cast<IW3DCamera*>( pCamera );
		if( !m_CameraAniModel.Load( "Object", szCameraModel ) )
		{
			return FALSE;
		} //if
		m_nActionNum = m_CameraAniModel.GetActionNum();
		m_wState	 = 0;
		return TRUE;
	} //Create

	BYTE		GetType()	{	return m_btType; }

	void		SetWorld( const Matrix4& matWorld )
	{
		m_matWorld = matWorld;
	} //SetWorld

	virtual void		SetAction( int nAction )
	{
		m_CameraAniModel.SetAction( nAction );
		m_bBegin = TRUE;
	} //SetAction
	
	BYTE				GetAction()
	{
		return m_wState;
	} //GetAction

	void		Begin()		{ m_bBegin = TRUE; } //Begin

	BOOL		IsActionEnd()
	{
		if( m_nActionNum <= m_wState )
		{
			return TRUE;
		} //if

		return FALSE;
	} // IsActionEnd;

	BOOL	LoadFovFile( const char* szFile )
	{
		FILE*	pFile = fopen( szFile, "rt" );
		if( !pFile )
		{
			Assert( !"파일열기 실패" );
			return FALSE;
		} //if

		m_aFov.clear();

		WORD	wFov;
		char	szBuff[8];
		while( fscanf( pFile, "%s", szBuff ) != EOF )
		{
			wFov = atoi( szBuff );
			m_aFov.push_back( wFov );
		} //for
		fclose( pFile );
		
		return TRUE;
	} //LoadFovFile
	virtual void	SkipPrologue(){};

	virtual BOOL	Update( const DWORD dwTick ){ return TRUE; };

	virtual void	Draw(){};
protected:
	IW3DCamera*		m_pCamera;						///< 카메라
	CAniModel		m_CameraAniModel;				///< 카메라 모델

	WORD			m_wState;						///< 상태
	BOOL			m_bBegin;
	int				m_nActionNum;					///< 기본동작 배열.
	vector<int>		m_aFov;							///< 포브
	DWORD			m_dwTick;						///< 틱

	Matrix4			m_matWorld;
	BYTE			m_btType;						/// Produce Type
}; //CProduce

/**
 * 캐릭터 선택하면 연출 클래스.
 */
class CSelectChar_Produce : public CProduce
{
public:
	CSelectChar_Produce()
	{
		m_pBox[0] = NULL;
		m_pBox[1] = NULL;
		m_nTopHeight	= 0;
		m_nBottomHeight = 0;
		m_nWidth		= 0;
		m_nHeight		= 0;
	} //CSelectChar_Produce

	~CSelectChar_Produce()
	{
		SAFE_RELEASE( m_pBox[0] );
		SAFE_RELEASE( m_pBox[1] );
	} //~CSelectChar_Produce

	BOOL	Create( const char* szCameraModel, const IW3DCamera* pCamera )
	{
		m_btType = TYPE_PRODUCE;

		if( !CProduce::Create( szCameraModel, pCamera ) )
		{
			return FALSE;
		} //if

		if( !LoadFovFile( "Object/stage_cameraonly.fov" ) )
		{
			Assert( !"열기실패" );
			return FALSE;
		} //if

		if(	!m_AniModel[0].Load( "Object", "stage_ch_v1" )	||
			!m_AniModel[1].Load( "Object", "stage_ch_v2" )	||
			!m_AniMantle.Load( "Object", "cut_16_clo" )		||
			!m_AniDoor.Load( "Object", "stage_door" ) )
		{
			return FALSE;
		} //if

		m_pBox[0] = MAKE_COMPONENT( Rectangle );
		m_pBox[1] = MAKE_COMPONENT( Rectangle );
		if( !m_pBox[0] || !m_pBox[1] )
		{
			Assert( !"할당실패" );
		} //if

		m_nWidth  = g_nWidth;
		m_nHeight = g_nHeight;
		if( g_nWidth == 800 )
		{
			m_nTopHeight	= 105;
			m_nBottomHeight = 105;
		}
		else
		{
			m_nTopHeight	= 133;
			m_nBottomHeight = 133;
		} //if

		m_pCamera->SetZoom( 0.7F );
		SetAction( m_wState );
		
		return TRUE;
	} //Create

	/**
	 * 동작설정
	 */
	void	SetAction( int nAction )
	{
		CProduce::SetAction( nAction );
		m_AniModel[0].SetAction( nAction );
		m_AniModel[1].SetAction( nAction );
		m_AniMantle.SetAction( nAction );
		m_AniDoor.SetAction( nAction );
	} //SetAction

	/**
	 * 업데이트
	 */
	BOOL	Update( const DWORD dwTick )
	{
		if( !m_bBegin )
		{ //시작이 아니면..
			return TRUE;
		} //if
		
		if( m_wState == 0 )
		{			
			g_pMediaPlayer->StopAll();
			g_pMediaPlayer->Play( BGM_PLAY_EVENT, BGM_EVENT_MAKECHARACTER );
		}

		m_dwTick += dwTick;

		if( m_bBegin )
		{
			if( !Update_Camera( dwTick ) )
			{
				return FALSE;
			} //if
		} //if

		return TRUE;
	} //Update

	BOOL	Update_Camera( const DWORD dwTick )
	{
		Assert( m_pCamera );

		m_CameraAniModel.Update( dwTick );
		m_AniModel[0].Update( dwTick );
		m_AniModel[1].Update( dwTick );
		m_AniMantle.Update( dwTick );
		m_AniDoor.Update( dwTick );

		if( m_CameraAniModel.IsEndAction() )
		{
			if( ++m_wState <= m_nActionNum )
			{
				m_pCamera->SetFov( m_aFov[m_wState-1] );
				SetAction( m_wState );
			}
			else
			{
				m_bBegin = FALSE;
				return FALSE;
			} //if
		} //if

		Matrix4		matCamera = m_CameraAniModel.GetTMMatrix();
		m_pCamera->SetWorld( matCamera * m_matWorld );
		m_pCamera->Execute();

		return TRUE;
	} //Update_Camera

	/**
	 * 그리기.
	 */
	void	Draw()
	{
		m_AniDoor.SetWorld(m_matWorld );
		m_AniDoor.Render();

		m_AniMantle.SetWorld( m_matWorld );
		m_AniMantle.Render();

		m_AniModel[0].SetWorld( m_matWorld );
		m_AniModel[0].Render();

		m_AniModel[1].SetWorld( m_matWorld );
		m_AniModel[1].Render();

		if( m_pBox[0] )	m_pBox[0]->Draw( 
			0, 
			0, 
			m_nWidth, 
			m_nTopHeight, 
			0XFF000000 
			);
		if( m_pBox[1] )	m_pBox[1]->Draw( 
			0, 
			m_nHeight - m_nBottomHeight, 
			m_nWidth, 
			m_nBottomHeight, 
			0XFF000000 
			);
	} //Draw

private:
	CAniModel		m_AniModel[NUM_BACKMODEL];		///< 배경에 쓰이는 모델들
	CAniModel		m_AniMantle;
	CAniModel		m_AniDoor;

	IW3DRectangle*	m_pBox[2];
	
	int				m_nTopHeight;
	int				m_nBottomHeight;
	int				m_nWidth;
	int				m_nHeight;
}; //class CSelectChar_Produce

/**
 * 캐릭터 생성에 대한 연출
 */
class CSelectChar_Create : public CProduce
{
public:
	enum { CAM_DEFAULT = 1, CAM_FACE = 2, CAM_BODY = 3, CAM_MAX = 4 };

	CSelectChar_Create(){}		//CSelectChar_Create
	~CSelectChar_Create(){}		//~CSelectChar_Create

	BOOL	Create( const char* szCameraModel, const IW3DCamera* pCamera )
	{
		m_btType = TYPE_CREATE;

		if( !CProduce::Create( szCameraModel, pCamera ) )
		{
			return FALSE;
		} //if

		if( !LoadFovFile( "Object/stage_make_camera.fov" ) )
		{
			Assert( !"열기실패" );
			return FALSE;
		} //if

		m_pCamera->SetZoom( 0.7F );

		SetCameraMode( CAM_BODY );
		return TRUE;
	} //Create

	void	SetCameraMode( const BYTE btMode )
	{
		Assert( btMode > 0 && btMode < CAM_MAX );

		m_pCamera->SetFov( m_aFov[btMode-1] );
		SetAction( btMode );
	} //SetCameraMode

	BOOL	Update( const DWORD dwTick )
	{
		Assert( m_pCamera );
		
		m_dwTick += dwTick;

		if( m_bBegin )
		{
			Update_Camera( dwTick );
		} //if

		return TRUE;
	} //Update

	BOOL	Update_Camera( const DWORD dwTick )
	{
		m_CameraAniModel.Update( dwTick );
		if( m_CameraAniModel.IsEndAction() )
		{
			m_bBegin = FALSE;
			return FALSE;
		} //if

		Matrix4		matCamera = m_CameraAniModel.GetTMMatrix();
		m_pCamera->SetWorld( matCamera * m_matWorld );
		m_pCamera->Execute();

		return TRUE;
	} //Update_Camera

private:
}; //CSelectChar_Create

/**
 * 캐릭터 선택에 대한 연출
 */
class CSelectChar_Select : public CProduce
{
public:
	CSelectChar_Select()
	{
		m_bSelected		= FALSE;
		m_nSelectNum	= -1;
	} //CSelectChar_Select

	~CSelectChar_Select(){}		//~CSelectChar_Select

	void	CharacterSelect( const int nSelectNum )
	{
		if( nSelectNum >= 0 )
		{
			m_nSelectNum = nSelectNum;
		} //if
		m_bSelected = !m_bSelected;

		CameraMode();
	} //CharacterSelect


private:
	BOOL	Create( const char* szCameraModel, const IW3DCamera* pCamera )
	{
		m_btType = TYPE_SELECT;

		if( !CProduce::Create( szCameraModel, pCamera ) )
		{
			return FALSE;
		} //if

		if( !LoadFovFile( "Object/stage_select_camera.fov" ) )
		{
			Assert( !"열기실패" );
			return FALSE;
		} //if

		m_pCamera->SetZoom( 0.7F );

		CameraMode();

		return TRUE;
	} //Create

	void	CameraMode()
	{
		switch( m_nSelectNum )
		{
		case 0:
			if( m_bSelected )	m_wState = 6;
			else				m_wState = 7;
			break;

		case 1:
			if( m_bSelected )	m_wState = 4;
			else				m_wState = 5;
			break;

		case 2:
			if( m_bSelected )	m_wState = 2;
			else				m_wState = 3;
			break;

		default:
			m_wState = 1;
			break;
		} //switch
		Assert( m_wState > 0 );

		m_pCamera->SetFov( m_aFov[m_wState-1] );
		SetAction( m_wState );
	} //CameraMode

	BOOL	Update( const DWORD dwTick )
	{
		m_dwTick += dwTick;

		if( m_bBegin )
		{
			Update_Camera( dwTick );
		} //if
		return TRUE;
	} //Update

	BOOL	Update_Camera( const DWORD dwTick )
	{
		m_CameraAniModel.Update( dwTick );
		if( m_CameraAniModel.IsEndAction() )
		{
			m_bBegin = FALSE;
			if( !m_bSelected )
			{
				m_nSelectNum = -1;
			} //if
			return FALSE;
		} //if

		Matrix4		matCamera = m_CameraAniModel.GetTMMatrix();
		m_pCamera->SetWorld( matCamera * m_matWorld );
		m_pCamera->Execute();

		return TRUE;
	} //Update_Camera

private:
	BOOL			m_bSelected;
	BYTE			m_nSelectNum;
}; //CSelectChar_Select

#endif //__CSelectCharProduce_Header__