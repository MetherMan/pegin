/**
 * 캐릭터 선택용 GUI.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-18 2:58a $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /UInterface/Src/UICharSelect.h $
 * 
 * 9     03-12-18 2:58a Sk8snow2
 * 
 * 8     03-12-12 11:58a Paran
 * 
 * 7     03-12-08 8:50p Paran
 * 
 * 6     03-09-09 4:11p Paran
 * 
 * 5     03-08-15 5:16a Paran
 * 
 * 4     03-08-14 3:59a Paran
 * 
 * 3     03-07-17 9:55p Paran
 * 
 * <HR>
 *
 * @file	UICharSelect.h
 */


#ifndef __UICharSelect_Header__
#define __UICharSelect_Header__


#include <string>
#include <vector>


using std::string;
using std::vector;


#define	MAX_NUMCHARACTER		3		///< 최대 캐릭터 갯수


/**
 * 캐릭터 정보.
 * @ingroup		Engine
 */
struct SCharInfo
{
	BOOL	bEmpty;				///< 정보 입력 완료 여부
	
	int		nClass;				///< 직업
	
	int		nSTR;				///< 힘
	int		nDEX;				///< 민첩
	int		nINT;				///< 지력

	char	szName[64];			///< 캐릭터 이름
	int		nLevel;
	
	int		nLLevel;
	int		nSLevel;
	int		nBLevel;
	int		nWLevel;
	
	BYTE	btSex;				///< 성별
	WORD	wHair;				///< 머리 스타일
	WORD	wFace;				///< 얼굴 스타일
	WORD	wCoat;				///< 윗옷
	WORD	wPants;				///< 바지
	WORD	wHand;				///< 장갑
	WORD	wFoot;				///< 신발
}; //struct SCharInfo


/**
 * 케릭터 선택.
 * @ingroup		Engine
 */
class CCharSelectInfo
{
public:
	UNDER_MEMORY_MGR( CCharSelectInfo );

	CCharSelectInfo();
	~CCharSelectInfo();

	void		Cleanup();

	/**
	 * 모델이름 얻기.
	 */
	void		GetModel1FileName( 
		int					nSex,
		char*				szFileName,					///< [IN] 파일이름을 얻기 위한 포인터.
		SItemInfoData*		pItemInfo,					///< [IN] 해당 아이템 인포.
		char				cRaceNum					///< [IN] 얻고자 하는 인종. ('0'백, '1'황, '2'흑)
		);

	SCharInfo*	GetCharInfo();

	void		SetCharInfo(
		const char*		szName,
		const int		nLevel,
		const int		nLLevel,
		const int		nSLevel,
		const int		nBLevel,
		const int		nWLevel,
		const BYTE		btSex,
		const WORD		wHair,
		const WORD		wFace,
		const WORD		wCoat,
		const WORD		wPants,
		const WORD		wHand,
		const WORD		wFoot,
		const BYTE		btClass
		);

	void		DeleteCharInfo( const int nIndex );

	void		Previous();
	void		Next();

	int			New();
	void		Save();

	BOOL		SetIndex( int nIndex );
	int			GetIndex();

	BOOL		IsFull();

	void		InitPOINT();
	int			GetHP();
	int			GetMP();
	int			GetPOINT();

	int			GetSTR();
	int			GetDEX();
	int			GetINT();
		
	void		Plus_STR();
	void		Minus_STR();
	void		Plus_DEX();
	void		Minus_DEX();
	void		Plus_INT();
	void		Minus_INT();

	void		ClearAllDex();

	void		SetName( const char* szName );

	void		Next_Sex();

	void		Previous_Hair();
	void		Next_Hair();

	void		Previous_Face();
	void		Next_Face();

	void		Previous_Coat();
	void		Next_Coat();

	void		Previous_Pants();
	void		Next_Pants();

private:
	typedef vector<WORD>	VEC_ITEMWORD;

	SCharInfo		m_CharInfo[MAX_NUMCHARACTER];	///< 캐릭터 정보들

	int				m_nIndex;						///< 현재 캐릭터 인덱스.
	int				m_nHairIndex;					///<
	int				m_nFaceIndex;					///<
	int				m_nCoatIndex;					///<
	int				m_nPantsIndex;					///<

	VEC_ITEMWORD	m_vecDefine_Hair;				///<
	VEC_ITEMWORD	m_vecDefine_Face;				///<
	VEC_ITEMWORD	m_vecDefine_Coat;				///<
	VEC_ITEMWORD	m_vecDefine_Pants;				///<
	VEC_ITEMWORD	m_vecDefine_Hands;				///<
	VEC_ITEMWORD	m_vecDefine_Foots;				///<
}; //class CharSelectInfo


#endif //__UICharSelect_Header__
