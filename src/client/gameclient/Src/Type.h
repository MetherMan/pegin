/**
 * 게임에서 쓰일 데이터 선언.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-06-08 3:58p $
 * @version	$Revision: 21 $
 *
 * <HR>
 * $Log: /GameClient/Src/Type.h $
 * 
 * 21    04-06-08 3:58p Paran2
 * 
 * 20    04-05-08 2:33a Paran2
 * 
 * 19    04-04-27 9:37p Paran
 * 
 * 18    04-04-08 12:18a Paran2
 * 
 * 15    03-12-31 8:32a Kjmgo			토탈레벨 구하는 함수 추가
 * 
 * 14    03-12-29 5:20p Kjmgo			레벨이 10이하일때만 메이가 나타나도록 수정
 * 
 * 9     03-08-10 6:59p Admin			말 타고내리기 적용
 * 
 * @file	Type.h
 */


#ifndef __Type_Header__
#define __Type_Header__


#include <string>


using std::string;

#define SKILL_SHORT				0
#define SKILL_LONG				1
#define SKILL_WHITE				2
#define SKILL_BLACK				3

/**
 * 플레이어 정보.
 * @ingroup		Game
 */
struct SPlayerInfo
{
	string		sName;				///< 이름
	BYTE		bSex;				///< 성별
	
	string		sGuildName;			///< 길드이름
	WORD		wGuildMark;			///< 길드마크
	int			nGuildId;			///< 길드 아뒤

	WORD		wHair;				///< 머리
	WORD		wFace;				///< 얼굴
	WORD		wUpper;				///< 상의
	WORD		wLower;				///< 하의
	WORD		wHand;				///< 손
	WORD		wFoot;				///< 발

	int			nHp[2];				///< HP
	int			nMp[2];				///< MP

	WORD		wLevel[4];			///< 레벨
	int			nExp[4][2];			///< 경험치

	WORD		wStr;				///< 체력
	WORD		wInt;				///< 지력
	WORD		wDex;				///< 민첩성
	int			nCha;				///< 매력

	int			nGold;				///< 돈
	BYTE		btSpeed;			///< 스피드
	WORD		wRide;				///< 탈것
	BYTE		btClass;			///< 직업
	
	BYTE		btPKMode;			///< PK Mode
	BYTE		btStun;				///< 스턴상태 등

	int			nCash;				///< 캐쉬

	/**
	 *
	 */
	WORD		GetMaxLevel()
	{
		WORD wMax = 0;

		for( int i = 0 ; i < 4 ; i++ )
		{
			if( wLevel[i] > wMax )
			{
				wMax = wLevel[i];
			} //if
		} //for

		return wMax;
	} //GetMaxLevel

	/**
	 *
	 */
	WORD		GetTotalLevel()
	{
		WORD wTotal = 0;

		for( int i = 0 ; i < 4 ; i++ )
		{
			wTotal += wLevel[i];
		} //for

		return wTotal;
	} //GetTotalLevel

}; //struct SPlayerInfo

/**
 * 팻정보
 */
struct SPetInfo
{
	int			nLevel;		// 팻 레벨 
	int			nExp;		// 현재 경험치
	int			nNeedExp;	// 목표 경험치
	WORD		wAngerGage;	// 분노 게이지
}; //struct SPetInfo


#endif // #ifndef __Type_Header__
