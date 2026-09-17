/**
 * 타격치 출력
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-08-22 11:22a $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /Engine/Src/HitIndication.h $
 * 
 * 11    03-08-22 11:22a Kjmgo
 * 
 * 10    03-06-10 2:02a Kjmgo
 * 
 * 9     03-06-09 8:18p Kjmgo
 * 크리티컬 수정
 * 
 * 8     03-06-08 3:22p Kjmgo
 * 
 * 7     03-05-29 8:11p Kjmgo
 * 5     03-05-11 2:53p Kjmgo
 * 타격치 수정
 * 4     03-05-11 10:35a Kjmgo
 * 3     03-05-11 2:49a Kjmgo
 * 타격치 추가
 * 2     03-05-10 6:03p Kjmgo
 * <HR>
 *
 * @file	HitIndication.h
 */


#ifndef __HitIndication_Header__
#define __HitIndication_Header__

#include <list>

using std::list;

#define MAXNUMBER 20

/**
 * 타격치 정보 구조체
 * @ingroup		Engine
 */ 
struct SHitData
{
	UNDER_MEMORY_MGR( SHitData );

	BYTE		aNumber[MAXNUMBER];	///< 각 자리수의 넘버
	DWORD		dwDamage;			///< 데미지
	DWORD		dwFirstColor;		///< 초기 Font 색
	DWORD		dwColor;			///< Font 색
	DWORD		dwTick;				///< 내부 누적 Tick
	float		fSpeed;				///< 속도
	WORD		wTotalWidth;		///< 전체 Font 너비
	int			nCount;				///< 자리수
	int			nColorCount;		///<
	int			nWidth;				///<
	int			nHeight;			///<
	Vector3		vPos;				///< 원래 위치
	Vector3		vScreenPos;			///< 화면상의 위치
	BYTE		btCritical;			///< 크리티컬 여부
	BOOL		bFlag;				///< 크리티컬 중간 여부
}; //struct SHitdata

/**
 * 타격치 구현 클래스
 * @ingroup		Engine
 */
class CHitIndication
{
public:
	UNDER_MEMORY_MGR( CFlyingObject );

	CHitIndication();
	~CHitIndication();

	BOOL Create( const char* szFile, const int nWidth, const int nHeight );
	void Cleanup();

	void AddHit( DWORD dwDamage, const Vector3& vPos, DWORD dwColor, BYTE bCritical = FALSE );
	void AddMiss( const Vector3& vPos, DWORD dwColor );

	void Update( DWORD dwTick );
	void Draw();
	

private:
	IW3DScene* m_pScene;			///< Scene
	IW3DSprite* m_pSprite;			///< Sprite ( Normal )
	IW3DSprite* m_pCritical;		///< Sprite ( Critical )

	typedef list<SHitData*>	HitList;

	HitList	m_listHit;

	// Texture 정보가 아님!
	int	m_nWidth;					///< Font 한개의 너비
	int m_nHeight;					///< Font 한개의 높이 
	int m_nColCount;				///< 한줄에 들어가는 숫자의 갯수

	SHitData*	MakeHit( DWORD dwDamage );
	void		DrawHit( SHitData* pData );
	void		DrawCritical( SHitData* pData );
	void		DrawMiss( SHitData* pData );
	void		DrawNumber( BYTE btNumber, Vector3 vPos, int nWidth, int nHeight );
}; //class CHitIndication



#endif // #ifndef __HitIndication_Header__
