/**
 * Block ( Terrain )
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-13 9:43p $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /Render/Src/Block.h $
 * 
 * 11    04-01-13 9:43p Kjmgo
 * 퀄리티 설정 적용.
 * 
 * 10    04-01-08 8:50a Kjmgo
 * 코드 정리
 * 
 * <HR>
 *
 * @file	Block.h
 */

#ifndef __Block_Header__
#define __Block_Header__

#include <list>
#include "VertexBuffer.h"
#include "Texture.h"

using std::list;

#define MAXLAYER 2
#define MAXPOLYGON 100

/**
 *
 */
class CBlockList
{
public:
	UNDER_MEMORY_MGR( CBlockList );

	CBlockList();
	~CBlockList();

	BOOL Init( const int nSize, const int nBlockSize, IW3DTerrain* pTerrain );
	BOOL Add( IW3DBlock* pBlock );

	void SetQuality( int nQuality )
	{
		m_nQuality = nQuality;
	} //SetQuality

	void Draw( int nX, int nY );

	// 최대 제한을 건다.
	void SetMaxCount( int nMax )
	{
		m_wMax = nMax;
	} //SetMaxCount

private:
	typedef std::list<IW3DBlock*> BlockList;

	/**
	 *
	 */
	struct BLOCK
	{
		MEMORY_UNDER_MGR( BLOCK );

		BlockList m_listBlock;		///< Block List
		int nCount;					///< Block 갯수
		Vector3	vPos;				///< 가운데 중심점

		/**
		 *
		 */
		BLOCK()
			:	nCount( 0 ),
				vPos( 0.0F, 0.0F, 0.0F )
		{
		} //BLOCK

		/**
		 *
		 */
		void Draw( int& nCount )
		{
			BlockList::iterator itor = m_listBlock.begin();

			while( itor != m_listBlock.end() )
			{
				IW3DBlock* pBlock = *itor;

				Assert( pBlock );
				if( pBlock )
				{
					pBlock->Draw();					
					nCount++;
				} //if

				itor++;
			} //while
		} //Draw

	}; //strucy BLOCK

	int		m_nSize;				///< 총 크기
	int		m_nBlockSize;			///< 한개의 블럭 크기
	int		m_nQuality;				///< 퀄리티
	WORD	m_wMax;					///< 최대 보여줄 갯수를 정한다.
	BLOCK*	m_apBlock;				///< 블럭 배열
	IW3DTerrain* m_pTerrain;
	
	/**
	 *
	 */
	void	SetRenderState();

}; //class CBlockList

/**
 *
 */
class CBlock : public IW3DBlock
{
public:
	UNDER_MEMORY_MGR( CBlock );

	CBlock();
	~CBlock();

	/**
	 *
	 */
	BOOL Init( const SMultiPass* pPass, IW3DTerrain* pTerrain );

	/**
	 *
	 */
	BOOL IsInRect(	
		const float fStartX,	///< 시작 위치
		const float fStartY,	///< 시작 위치
		const float fSize		///< 크기
		);

	/**
	 *
	 */
	void RenderState();

	/**
	 *
	 */
	void Draw();

	/**
	 *
	 */
	Vector3 GetPosition()
	{
		return m_vPos;
	} //GetPosition
	
	/**
	 *
	 */	
	BOOL	Pick( const int nX, const int nY );

	/**
	 *
	 */
	void	Move( const int nX, const int nY );

	/**
	 *
	 */
	SMultiPass GetPass()
	{
		return m_MultiPass;
	} //GetPass

	/**
	 *
	 */
	void SetAlpha( BYTE btAlpha )
	{
		//-- NULL --//	 
	} //SetAlpha

private:
	struct TVERTEX
	{
		float tu, tv;
	}; //struct TVERTEX

	struct VERTEX
	{
		enum	{ FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
		Vector3 p;
		float u,v;
	}; //struct VERTEX

	Matrix4				m_matTexture;			///< Texture Matrix
	
	IW3DTerrain*		m_pTerrain;				///< Terrain Pointer

	SMultiPass			m_MultiPass;			///< 멀티 패스 정보
	Vector3				m_vPos;					///< 중심점 ( 위치 )
	int					m_nVertex;				///< 버텍스 갯수
	int					m_nFaces;				///< 폴리곤 갯수
	int					m_nRot;					///< 방향( 회전 값 )
	
	VERTEX				m_vTerrain[MAXPOLYGON * 3];		///<
	CTexture*			m_pTexture[MAXLAYER];			///< 텍스처.

private:
	
	void Make();
	void InsertVertex( VERTEX& vP );
}; //class CBlock


#endif //__Block_Header__