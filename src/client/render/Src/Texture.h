/**
 * 텍스처 모듈 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	Teammay
 * @date	$Date: 04-01-08 12:01p $
 * @version	$Revision: 20 $
 *
 * <HR>
 * $Log: /Render/Src/Texture.h $
 * 
 * 20    04-01-08 12:01p Kjmgo
 * 코드 정리
 * 
 * <HR>
 *
 * @file	Texture.h
 */


#ifndef __Texture_Header__
#define __Texture_Header__

#include <string>
#include <map>

#include "Render_Export.h"

using std::string;
using std::map;


#define g_pTextureMgr	( CTextureMgr::GetSingletonPtr() )

class	CTexture;
class	CTextureMgr;

/**
 * 텍스처 관리자.
 * @ingroup		Render
 */
class CTextureMgr : public Singleton<CTextureMgr>
{
public:
	enum{ MODEL = 0, SPRITE = 1 };
	enum{ LEV_HIGH = 0, LEV_NORMAL = 1, LEV_LOW = 2 };

	UNDER_MEMORY_MGR( CTextureMgr );

	/**
	 *
	 */
	CTextureMgr( LPDIRECT3DDEVICE8 pd3dDevice );

	/**
	 *
	 */
	~CTextureMgr();

	/**
	 *
	 */
	BOOL		GetFullPathName( string& strDest, const string&	strPath, const string&	strFile );
	
	/**
	 *
	 */
	void		SetLevel( BYTE btLevel );

	/**
	 *
	 */
	CTexture*	LoadTexture( const string& szPath, const string& strName, const int nMode = MODEL );

	/**
	 *
	 */
	void		UnloadTexture( CTexture* pTexture );

private:
	typedef map<std::string, CTexture*>		TextureMap;

	LPDIRECT3DDEVICE8	m_pd3dDevice;		///< Direct3D Device
	TextureMap			m_mapTexture;		///< 텍스처 리스트
	BYTE				m_btLevel;			///< 텍스쳐 레벨
	string				m_strTemp;			///< 텍스쳐 파일이름을 조합하기 위한 변수
}; //class CTextureMgr


/**
 * 텍스처 클래스.
 * @ingroup		Render
 */
class CTexture
{
public:
	UNDER_MEMORY_MGR( CTexture );

	/**
	 * 생성자.
	 */
	CTexture()
	:	m_nRef		( 1 ),
		m_pTexture	( NULL )
	{
		m_sName = "";
	} //CTexture

	/**
	 * Reference Count를 증가시키는 함수.
	 */
	void	AddRef()
	{
		m_nRef++;
	} //AddRef

	/**
	 *
	 */
	void	Release();

	/**
	 *
	 */
	BOOL	Load( 
		LPDIRECT3DDEVICE8	pd3dDevice, 
		const string&		strFileName, 
		const string&		strName,
		int					nMode = CTextureMgr::MODEL 
		);

	/**
	 * 텍스처를 세팅한다. (사용한다)
	 *
	 * @param	nStage			[IN] 텍스처 단계
	 */
	void	SetTexture( int nStage )
	{
		Assert( g_pd3dDevice );
		Assert( m_pTexture );
		Assert( 0 <= nStage && nStage < 4 );

		g_pd3dDevice->SetTexture( nStage, m_pTexture );
	} //SetTexture

	/**
	 * 다이렉트 텍스쳐를 얻는다.
	 */
	const LPDIRECT3DTEXTURE8 GetD3dTexture()
	{
		Assert( m_pTexture );
		return m_pTexture;
	} //GetD3dTexture

	/**
	 * 텍스처의 이름을 얻는다.
	 *
	 * @return	텍스처 이름
	 */
	const string&	GetName()
	{
		return m_sName;
	} //GetName

private:
	/**
	 * 소멸자.
	 */
	~CTexture()
	{
		SAFE_RELEASE( m_pTexture );
	} //~CTexture

	/**
	 *
	 */
	int		ReadFile( const string& strFileName, VOID** ppBuffer );

private:
	int					m_nRef;			///< 레퍼런스 카운트.
	string				m_sName;		///< 이름.
	LPDIRECT3DTEXTURE8	m_pTexture;		///< D3D 텍스처.
}; //class CTexture


/**
 * 텍스처 세팅.
 */
inline void		SET_TEXTURE( int nStage, CTexture* pTexture )
{
	if( pTexture )
	{
		pTexture->SetTexture( nStage );
	}
	else
	{
		g_pd3dDevice->SetTexture( 0, NULL );
	} //if..else..
} //SET_TEXTURE


#endif // #ifndef __Texture_Header__


/* Usage Example:
*/
