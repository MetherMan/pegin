/**
 * ·»Áî ÇÃ·¹¾î
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	·Î³Ä
 * @date	$Date: 04-03-25 4:33p $
 * @version	$Revision: 1 $
 *
 * <HR>
 * $Log: /Render/Src/LensFlare.h $
 * 
 * 1     04-03-25 4:33p Kjmgo
 * 
 * <HR>
 *
 * @file	LensFlare.h
 */

#ifndef __LensFlare_Header__
#define __LensFlare_Header__

#define MAX_HALO		12

/**
 *
 */
class CLensFlare
{
public:
	UNDER_MEMORY_MGR( CLensFlare );

	// Constructor And Destructor
		CLensFlare();
		~CLensFlare();
	// ~Constructor And Destructor

	// Init
	void Init( const Vector3& vPos );

	// Set Light Position
	void SetPosition( const Vector3& vPos );

	//	Set Light Texture
	BOOL SetLightTexture( const char* szTexture );

	//	Draw
	void Draw( const DWORD dwTick );

private:
	//---------------------------
	//
	//---------------------------
	struct Vertex
	{
		enum	{ FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
		Vector3 p;
		DWORD	color;
		float	tu, tv;
	}; //struct Vertex

	// Create Light
	BOOL CreateLight();

	// Create Halo
	BOOL CreateHalo();

	// Draw Light
	void DrawLight();

private:
	
	IW3DScene*	m_pScene;
	CTexture*	m_pLightTex;			// Light Texture ( Billboard )
	IW3DSprite*	m_apHalo[MAX_HALO];		// Halo Texture

	Vector3				m_vDir;				// Direction
	Vector3				m_vPos;				// Light Position

	CVertexBuffer*		m_pVB;				// Vertex Buffer

	float				m_fSize;
	int					m_nLightPosX;		// Light Position X ( Screen Coords )
	int					m_nLightPosY;		// Light Position Y ( Screen Coords )

	Vector2				m_vLightPos;
	Vector2				m_vCenter;

	int					m_nCenterX;			// Screen Center X
	int					m_nCenterY;			// Screen Center Y

	DWORD				m_dwColor;		// Light's Color
}; //class CLensFlare


#endif //#ifndef __LensFlare_Header__