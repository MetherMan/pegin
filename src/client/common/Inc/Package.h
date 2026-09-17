/**
 * 팩키지 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-05 4:55a $
 * @version	$Revision: 1 $
 *
 * <HR>
 * $Log: /Common/Inc/Package.h $
 * 
 * 1     03-12-05 4:55a Kjmgo
 * 
 * <HR>
 *
 * @file	Package.h
 */

#ifndef __Package_Header__
#define __Package_Header__

#include <vector>

/**
 *
 */
class CPackage
{
public:
	MEMORY_UNER_MGR( CPackage );

	/**
	 *
	 */
	CPackage();

	/**
	 *
	 */
	~CPackage();

	/**
	 *
	 */
	BOOL		Load( const char* szFile );

	/**
	 *
	 */
	void		Add( void*	p, int nSize );

	/**
	 *
	 */
	void		Remove( WORD wId );

	/**
	 *
	 */
	void*		Get( WORD wId );

	/**
	 *
	 */
	WORD		GetBegin();

	/**
	 *
	 */
	WORD		GetEnd();

	/**
	 *
	 */
	void		Compress( const char* szFile );

private:
	typedef std::vector<void*>	DataVector;

	DataVector		m_aData;		///< 데이타 리스트
	int				m_nSize;		///< 파일의 총 크기
	int				m_nCount;		///< 데이타들의 갯수 

}; //class CPackage


#endif //#ifndef __Package_Header__
