/**
 * 유저 인터페이스용 그림 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-11 12:55p $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIPicture.h $
 * 
 * 6     03-12-11 12:55p Paran
 * 
 * 5     03-12-08 8:50p Paran
 * 
 * 4     03-09-17 5:08p Paran
 * 
 * 3     03-05-24 6:38a Ahastudio
 *
 * 2     03-05-17 4:20a Paran		각 콤포넌트 분리.
 * <HR>
 *
 * @file	UIPicture.h
 */


#ifndef __UIPicture_Header__
#define __UIPicture_Header__


/**
 * 유저 인터페이스 그림 클래스.
 */
class UIPicture : public UICComponent
{
public:
	UNDER_MEMORY_MANGER( UIPicture );

	/**
	 * 생성자.
	 */
	UIPicture()
	:	m_pPicture( NULL ),
		m_bLoad ( FALSE )
	{
	} //UIPicture

	/**
	 * 소멸자.
	 */
	~UIPicture()
	{
		Cleanup();
	} //~UIPicture

	/**
	 * 클린 업
	 */
	void			Cleanup()
	{
		if( m_bLoad )
		{
			m_bLoad = FALSE;
			Assert( m_pPicture );
			SAFE_RELEASE( m_pPicture );
		} //if		
	} //Cleanup

	BOOL			Load( const char* szFileName, int nWidth, int nHeight )
	{
		Cleanup();

		m_pPicture = MAKE_COMPONENT( Sprite );
		if( !m_pPicture )
		{
			Assert( !"할당실패" );
			return FALSE;
		} //if

		if( !m_pPicture->Load( szFileName, nWidth, nHeight ) )
		{
			Assert( !"로드실패" );
			return FALSE;
		} //if

		m_bLoad = TRUE;
		return TRUE;
	} //Load

	void			SetPicture( IW3DSprite* pPicture )	
	{ 
		Cleanup();
		m_pPicture = pPicture; 
	} //SetPicture

	IW3DSprite*		GetPicture()						{ return m_pPicture; }
	

	void			Draw()
	{
		if( !IsShow() )
		{
			return;
		} //if

		if( m_pPicture )
		{
			m_pPicture->Draw( m_rcRegion.left, m_rcRegion.top );
		} //if
	} //Draw

private:
	BOOL			m_bLoad;		///< 해제요함
	IW3DSprite*		m_pPicture;		///< 2D 스프라이트
}; //class UIPicture


#endif //__UIPicture_Header__
