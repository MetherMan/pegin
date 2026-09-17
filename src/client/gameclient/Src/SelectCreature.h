/**
 * 선택 유지
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐 ( かね せいばん )
 * @date	$Date: 04-09-24 4:45p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /GameClient/Src/SelectCreature.h $
 * 
 * 3     04-09-24 4:45p Paran
 * 
 * 2     03-12-26 10:33a Kjmgo
 * 선택유지중, 몬스터도 추가
 * 
 * 1     03-12-17 6:51a Kjmgo
 * 
 * <HR>
 *
 * @file	SelectCreature.h
 */
#ifndef __SelectCreature_Header__
#define __SelectCreature_Header__

/*
	CHARACTER = 0,
	NPC,
	MONSTER
*/

/**
 *
 */
class CSelectCreature
{
public:
	MEMORY_UNDER_MGR( CSelectCreature );

	/**
	 *
	 */
	CSelectCreature();

	/**
	 *
	 */
	~CSelectCreature();

	/**
	 *
	 */
	BOOL	Init( IW3DWorld* pWorld );

	/**
	 *
	 */
	void	Select( IW3DCreature* pCreature, BYTE btType = CHARACTER );

	/**
	 *
	 */
	BYTE	GetType()
	{
		return m_btType;
	} //GetType

	/**
	 *
	 */
	void	SetWorld( Vector3& vPos, Quaternion& qRot );

	/**
	 *
	 */
	void	SetWorld( Matrix4& matWorld );

	/**
	 *
	 */
	void	Draw( const DWORD dwTick );

	/**
	 *
	 */
	IW3DCreature* GetCreature()
	{
		return m_pCreature;
	} //GetCreature
	
	/**
	 *  
	 */
	DWORD	GetId()
	{
		if( m_pCreature )	return m_pCreature->GetId();
		
		return 0;
	} //GetId

private:
	
	IW3DCreature*		m_pCreature;
	IW3DWorld*			m_pWorld;
	//---------------------------------
	// 선택 이펙트
	// せんたく イペッツ
	//---------------------------------
	Matrix4				m_matWorld;			///< World Matrix
	IW3DSpecialEffect*  m_pSelectEffect;	///< 선택 이펙트
	BOOL				m_bSelect;			///< 선택 여부
	DWORD				m_dwTicks;			///< 누적 시간
	BYTE				m_btType;			///< 크리처의 상태
	
}; //struct SSelectEffect

#endif //#ifndef __SelectCreature_Header__

