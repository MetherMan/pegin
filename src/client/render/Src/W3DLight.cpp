/**
 * 광원.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-12-03 8:03p $
 * @version	$Revision: 12 $
 *
 * <HR>
 * $Log: /Render/Src/W3DLight.cpp $
 * 
 * 12    03-12-03 8:03p Kjmgo
 * 버그 수정
 * 
 * 11    03-10-18 12:08a Kjmgo
 * 
 * 10    03-10-18 12:08a Kjmgo
 * 
 * 9     03-10-17 1:59p Kjmgo
 * Update함수 추가
 * 
 * 8     03-10-17 10:50a Kjmgo
 * Fade ( 보간 적용 ) 효과 추가
 * 
 * 7     03-10-17 10:02a Kjmgo
 * Light를 키고 끄는 함수 추가
 * 
 * 6     03-10-15 6:42p Kjmgo
 * 라이트 관리자 추가 ^-^ 
 * 
 * 5     03-05-22 6:11a Ahastudio
 * <HR>
 *
 * @file	W3DLight.cpp
 */


#include "Render_Export.h"

#define		MAX_LIGHT	10

void			Interpolate( const D3DCOLORVALUE& s, const D3DCOLORVALUE& e, D3DCOLORVALUE& v, float f );
void			Interpolate( const D3DLIGHT8& s, const D3DLIGHT8& e, D3DLIGHT8& v, float f );


/**
 * 광원 클래스.
 * @ingroup		Render
 */
class CW3DLight : public IW3DLight
{
public:
	UNDER_MEMORY_MGR( CW3DLight );

	/**
	 * 생성자.
	 */
	CW3DLight()
	{
		_ZeroMemory( &m_Light, sizeof( D3DLIGHT8 ) );
		_ZeroMemory( &m_LightFade, sizeof( D3DLIGHT8 ) );
		_ZeroMemory( &m_LightDummy, sizeof( D3DLIGHT8 ) );

		m_bFade = FALSE;

		m_fFade = 0.0F;
	} //CW3DLight

	/**
	 * 소멸자.
	 */
	~CW3DLight()
	{
	} //~CW3DLight

	/**
	 * 광원을 사용한다.
	 *
	 * @param	nIndex		[IN] 광원 인덱스 (동일한 인덱스가 중복될 경우 나중의 것으로 사용된다)
	 */
	void	SetLight( int nIndex )
	{
		g_pd3dDevice->SetLight( nIndex, &m_Light );
		g_pd3dDevice->LightEnable( nIndex, TRUE );
	} //SetLight

	/**
	 *
	 */
	void	SetPoint()
	{
		if( !m_bFade )
		{
			m_Light.Type = D3DLIGHT_POINT;
		}
		else
		{
			m_LightFade.Type = D3DLIGHT_POINT;
		} //if..else..
	} //SetPoint

	/**
	 *
	 */
	void	SetSpot()
	{
		if( !m_bFade )
		{
			m_Light.Type = D3DLIGHT_SPOT;
		}
		else
		{
			m_LightFade.Type = D3DLIGHT_SPOT;
		} //if..else..
	} //SetSpot

	/**
	 *
	 */
	void	SetDirectional()
	{
		if( !m_bFade )
		{
			m_Light.Type = D3DLIGHT_DIRECTIONAL;
		}
		else
		{
			m_LightFade.Type = D3DLIGHT_DIRECTIONAL;
		} //if..else..
	} //SetDirectional

	/**
	 *
	 */
	void	SetDiffuse( DWORD dwColor )
	{
		if( !m_bFade )
		{
			m_Light.Diffuse.a = ( ( dwColor >> 24 ) & 0xFF ) / 255.0F;
			m_Light.Diffuse.r = ( ( dwColor >> 16 ) & 0xFF ) / 255.0F;
			m_Light.Diffuse.g = ( ( dwColor >> 8  ) & 0xFF ) / 255.0F;
			m_Light.Diffuse.b = (   dwColor         & 0xFF ) / 255.0F;
		}
		else
		{		
			m_LightFade.Diffuse.a = ( ( dwColor >> 24 ) & 0xFF ) / 255.0F;
			m_LightFade.Diffuse.r = ( ( dwColor >> 16 ) & 0xFF ) / 255.0F;
			m_LightFade.Diffuse.g = ( ( dwColor >> 8  ) & 0xFF ) / 255.0F;
			m_LightFade.Diffuse.b = (   dwColor         & 0xFF ) / 255.0F;
		} //if..else..
	} //SetDiffuse

	/**
	 *
	 */
	void	SetAmbient( DWORD dwColor )
	{
		if( !m_bFade )
		{
			m_Light.Ambient.a = ( ( dwColor >> 24 ) & 0xFF ) / 255.0F;
			m_Light.Ambient.r = ( ( dwColor >> 16 ) & 0xFF ) / 255.0F;
			m_Light.Ambient.g = ( ( dwColor >> 8  ) & 0xFF ) / 255.0F;
			m_Light.Ambient.b = (   dwColor         & 0xFF ) / 255.0F;
		}
		else
		{
			m_LightFade.Ambient.a = ( ( dwColor >> 24 ) & 0xFF ) / 255.0F;
			m_LightFade.Ambient.r = ( ( dwColor >> 16 ) & 0xFF ) / 255.0F;
			m_LightFade.Ambient.g = ( ( dwColor >> 8  ) & 0xFF ) / 255.0F;
			m_LightFade.Ambient.b = (   dwColor         & 0xFF ) / 255.0F;
		} //if..else..
	} //SetAmbient

	/**
	 *
	 */
	void	SetSpecular( DWORD dwColor )
	{
		if( !m_bFade )
		{
			m_Light.Specular.a = ( ( dwColor >> 24 ) & 0xFF ) / 255.0F;
			m_Light.Specular.r = ( ( dwColor >> 16 ) & 0xFF ) / 255.0F;
			m_Light.Specular.g = ( ( dwColor >> 8  ) & 0xFF ) / 255.0F;
			m_Light.Specular.b = (   dwColor         & 0xFF ) / 255.0F;
		}
		else
		{
			m_LightFade.Specular.a = ( ( dwColor >> 24 ) & 0xFF ) / 255.0F;
			m_LightFade.Specular.r = ( ( dwColor >> 16 ) & 0xFF ) / 255.0F;
			m_LightFade.Specular.g = ( ( dwColor >> 8  ) & 0xFF ) / 255.0F;
			m_LightFade.Specular.b = (   dwColor         & 0xFF ) / 255.0F;
		} //if..else..
	} //SetSpecular

	/**
	 *
	 */
	void	SetPosition( const Vector3& vPos )
	{
		m_Light.Position = *(D3DVECTOR*)&vPos;
	} //SetPosition

	/**
	 *
	 */
	void	SetDirection( const Vector3& vDir )
	{
		m_Light.Direction = *(D3DVECTOR*)&vDir.Normal();
	} //SetDirection

	/**
	 *
	 */
	void	SetRange( float fRange )
	{
		if( !m_bFade )
		{
			m_Light.Range = fRange;
		}
		else
		{
			m_LightFade.Range = fRange;
		} //if..else..
	} //SetRange

	/**
	 *
	 */
	void	SetConstantAttenuation( float fAttenuation )
	{
		if( !m_bFade )
		{
			m_Light.Attenuation0 = fAttenuation;
		} 
		else
		{
			m_LightFade.Attenuation0 = fAttenuation;
		} //if..else..
	} //SetConstantAttenuation

	/**
	 *
	 */
	void	SetLinearAttenuation( float fAttenuation )
	{
		if( !m_bFade )
		{
			m_Light.Attenuation1 = fAttenuation;
		} 
		else
		{
			m_LightFade.Attenuation1 = fAttenuation;
		} //if..else..
	} //SetLinearAttenuation

	/**
	 *
	 */
	void	SetQuadraticAttenuation( float fAttenuation )
	{
		if( !m_bFade )
		{
			m_Light.Attenuation2 = fAttenuation;
		} 
		else
		{
			m_LightFade.Attenuation2 = fAttenuation;
		} //if..else..
	} //SetQuadraticAttenuation

	/**
	 *
	 */
	void	SetFade( BOOL bOn )
	{
		m_bFade = bOn;
		m_fFade = 0.0F;

		m_LightFade = m_LightDummy = m_Light;
	} //SetFade

	/**
	 *
	 */
	void	Update()
	{
		if( !m_bFade )
		{
			return ;
		} //if
		//m_Light
		m_fFade += 0.05F;

		if( m_fFade >= 1.0F )
		{
			m_bFade = FALSE;
			m_Light = m_LightFade;
			return ;
		} //if

		Interpolate( m_LightDummy, m_LightFade, m_Light, m_fFade );
	} //Update

private:
	D3DLIGHT8		m_Light;		///< D3D 광원 정보
	D3DLIGHT8		m_LightDummy;	///< D3D 광원 정보
	D3DLIGHT8		m_LightFade;	///< D3D 광원 정보
	BOOL			m_bFade;		///< 페이드 사용 여부
	float			m_fFade;		///< 페이드 증가값
}; //class CW3DLight


IMPLEMENT_COMPONENT( CW3DLight, Light );

/**
 * 광원 클래스.
 * @ingroup		Render
 */
class CW3DLightMgr : public IW3DLightMgr
{
public:
	MEMORY_UNDER_MGR( CW3DLightMgr );

	/**
	 *
	 */
	CW3DLightMgr();

	/**
	 *
	 */
	~CW3DLightMgr();

	/**
	 *
	 */
	void Init( int nCount );

	/**
	 *
	 */
	void Apply();

	/**
	 *
	 */
	IW3DLight* GetLight( int nIndex );

	/**
	 *
	 */
	int	GetLightCount()
	{
		return m_nCount;
	} //GetLightCount

	/**
	 *
	 */
	void Turn( int nIndex, BOOL bOn )
	{
		m_pUse[nIndex] = bOn;
	} //Turn 

private:

	int			m_nCount;				///< Light 갯수
	IW3DLight*	m_apLight[MAX_LIGHT];	///< 동적인 배열

	int			m_nUseCount;			///< 사용 하고 있는 갯수

	BOOL*		m_pUse;
	
}; //class CW3DLightMgr

IMPLEMENT_COMPONENT( CW3DLightMgr, LightMgr );

/**
 *
 */
CW3DLightMgr::CW3DLightMgr()
:		m_nCount			( 0 ),
		m_nUseCount			( 0 )
{
	for( int i = 0 ; i < MAX_LIGHT ; i++ )
	{
		m_apLight[i] = NULL;
	}
} //CW3DLightMgr::CW3DLightMgr	

/**
 *
 */
CW3DLightMgr::~CW3DLightMgr()
{
	IW3DLight* pLight = NULL;
	for( int i = 0 ; i < MAX_LIGHT ; i++ )
	{
		pLight = m_apLight[i];
		SAFE_RELEASE( pLight );
	} //for

	SAFE_DELETE_ARRAY( m_pUse );
} //CW3DLightMgr::~CW3DLightMgr

/**
 *
 */
void CW3DLightMgr::Init( int nCount )
{
	Assert( nCount >= 0 );
	m_nCount = nCount;

	if( m_nCount < 8 )
	{
		m_nCount = 8;
	} //if

	m_pUse	= new BOOL[m_nCount];

	for( int i = 0 ; i < m_nCount ; i++ )
	{
		m_apLight[i] = MAKE_COMPONENT( Light );
		m_pUse[i] = FALSE;
	} //for
} //CW3DLightMgr::Init

/**
 *
 */
void CW3DLightMgr::Apply()
{
	for( int i = 0 ; i < m_nCount ; i++ )
	{
		if( m_apLight[i] && m_pUse[i] )
		{
			m_apLight[i]->SetLight( i );
		} //if
	} //for
} //CW3DLightMgr::Apply

/**
 *
 */
IW3DLight* CW3DLightMgr::GetLight( int nIndex )
{
	if( nIndex < 0 || m_nCount <= nIndex )
	{
		return NULL; 
	} //if

	m_pUse[nIndex] = TRUE;
	
	return m_apLight[nIndex];
} //CW3DLightMgr::LoadLight

/**
 *
 */
void	Interpolate( const D3DCOLORVALUE& s, const D3DCOLORVALUE& e, D3DCOLORVALUE& v, float f )
{
	v.a = Interpolate( s.a, e.a, f );
	v.r = Interpolate( s.r, e.r, f );
	v.g = Interpolate( s.g, e.g, f );
	v.b = Interpolate( s.b, e.b, f );
} //Interpolate

/**
 *
 */
void	Interpolate( const D3DLIGHT8& s, const D3DLIGHT8& e, D3DLIGHT8& v, float f )
{
	Interpolate( s.Diffuse, e.Diffuse, v.Diffuse, f );
	Interpolate( s.Ambient, e.Ambient, v.Ambient, f );
	Interpolate( s.Specular, e.Specular, v.Specular, f );

	v.Range			= Interpolate( s.Range, e.Range, f );
	
	v.Attenuation0	= Interpolate( s.Attenuation0, e.Attenuation0, f );
	v.Attenuation1	= Interpolate( s.Attenuation1, e.Attenuation1, f );
	v.Attenuation2	= Interpolate( s.Attenuation2, e.Attenuation2, f );
} //Interpolate