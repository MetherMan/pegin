/**
 * 지형용 그리드 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-10-03 1:15p $
 * @version	$Revision: 1 $
 *
 * <HR>
 * $Log: /Render/Src/TerrainGrid.h $
 * 
 * 1     03-10-03 1:15p Kjmgo
 * 
 * <HR>
 *
 * @file	TerrainGrid.h
 */

#ifndef __TerrainGrid_Header__
#define __TerrainGrid_Header__


#include "Render_Export.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include <string>
#include <vector>

using std::string;
using std::vector;


/**
 *
 */
class CTerrainGrid
{
public:
	MEMORY_UNDER_MGR( CTerrainGrid );

	/**
	 *
	 */
	CTerrainGrid();

	/**
	 *
	 */
	~CTerrainGrid();

	/**
	 *
	 */
	BOOL	Init( int nGrid, int nSize );

	/**
	 *
	 */
	void	SetTexture( CTexture* pTexture );

	/**
	 *
	 */
	CVertexBuffer*	GetVB()
	{
		return m_pVB;
	} //GetVB

	/**
	 *
	 */
	void	Draw();


public:
	struct TVERTEX
	{
		float tu, tv;
	}; //struct TVERTEX

	struct VERTEX
	{
		enum	{ FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
		Vector3	p;
		Vector3	n;
		DWORD	color;
		TVERTEX tv;
	}; //struct VERTEX

private:
	CVertexBuffer*		m_pVB;				///< 버텍스 버퍼.
	CTexture*			m_pTexture;			///< 텍스처

	int					m_nGrid;			///< 갯수( 정사각형이 될것이므로 한 라인의 사각형 갯수 )
	int					m_nSize;			///< 사이즈( M단위 ) 

	int					m_nFaces;			///< 폴리곤 갯수
}; //class CTerrainGrid

#endif //#ifndef __TerrainGrid_Header__