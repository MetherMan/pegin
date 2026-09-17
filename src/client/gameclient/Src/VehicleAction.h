/**
 * Vehicle Action 관리자
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-03-28 7:52p $
 * @version	$Revision: 12 $
 *
 * <HR>
 * $Log: /GameClient/Src/VehicleAction.h $
 * 
 * 12    04-03-28 7:52p Sckyj
 * 
 * 11    04-03-27 10:33a Sckyj
 * 
 * 10    04-03-12 6:41p Sckyj
 * 운영자 작업중
 * 
 * 9     03-12-22 5:00p Sckyj
 * 2D, 3D sound 분리
 * 
 * 8     03-12-16 1:25a Sckyj
 * 물위에서 물발자국 소리 나도록 추가
 * 
 * 7     03-10-17 3:29p Sckyj
 * vehicle sound 재생 관련 코드 추가
 * 
 * 6     03-10-16 4:41p Sckyj
 * sound재생관련 코드 수정
 * 
 * 5     03-10-07 9:27a Kjmgo
 * 활을 들고 말을 탔을때의 오류 수정( AddVehicle시,
 * VehicleAction->SetWeaopn();으로 하던것을 SetWeapon2()로 수정
 * 
 * 4     03-10-02 10:33p Kjmgo
 * 캐릭터 State 권한 이전 작업 : 1차 백업
 * 
 * 3     03-09-14 2:49a Kjmgo
 * 
 * 2     03-09-12 6:42p Kjmgo
 * Vehicle 관련 수정 및 추가
 * 
 * <HR>
 *
 * @file	VehicleAction.h
 */

#ifndef __VehicleAction_Header__
#define __VehicleAction_Header__

#include "Global.h"

////////////////////////////////////////////////////////////////

#define SEX_MALE						0
#define SEX_FEMALE						1

////////////////////////////////////////////////////////////////

/*0 맨손
1 맨손	+ 방패
2 한손검
3 한손검 + 방패
4 양손검
5 쌍칼
6 한손도끼
7 한손도끼 + 방패
8 양손도끼
9 쌍도끼
10 한손둔기
11 한손둔기 + 방패
12 양손둔기
13 쌍둔기
14 돈파
15 단궁 
16 장궁
17 연노
18 석궁
19 창
20 봉
21 지팡이
22 도
23 참마도*/
#define WEAPON_NONE						0		//"0.맨손",
#define WEAPON_NONEWITHSHIELD			1		//"0.맨손",
#define WEAPON_ONEHANDSWORD				2		//"1.한손검",
#define WEAPON_ONEHANDSWORDWITHSHIELD	3		//"1.한손검",
#define WEAPON_TWOHANDSWORD				4		//"2.양손검", WITHSHIELD
#define WEAPON_DUALSWORD				5		//"3.쌍칼",
#define WEAPON_ONEHANDAXE				6		//"4.한손도끼",
#define WEAPON_ONEHANDAXEWITHSHIELD		7		//"4.한손도끼",
#define WEAPON_TWOHANDAXE				8		//"5.양손도끼",
#define WEAPON_DUALAXE					9		//"6.쌍도끼",
#define WEAPON_ONEHANDBLUNT				10		//"7.한손둔기",
#define WEAPON_ONEHANDBLUNTWITHSHIELD	11		//"7.한손둔기",
#define WEAPON_TWOHANDBLUNT				12		//"8.양손둔기",
#define WEAPON_DUALBLUNT				13		//"9.쌍둔기",
#define WEAPON_DONPA					14		//"10.돈파",
#define WEAPON_SHORTBOW					15		//"11.단궁",
#define WEAPON_LONGBOW					16		//"12.장궁",
#define WEAPON_DDABALBOW				17		//"13.연노",
#define WEAPON_CROSSBOW					18		//"14.석궁",
#define WEAPON_SPHERE					19		//"16.창",
#define WEAPON_BONG						20		//"17.봉",
#define WEAPON_STAFF					21		//"18.지팡이",
#define WEAPON_DO						22		// 
#define WEAPON_SLAYER					23		//
// 이벤트용 무기
#define WEAPON_FISHINGTACKLE			24		// 낚시대

////////////////////////////////////////////////////////////////

/*0 정지
1 걷기
2 뛰기
3 뛰기전동작
4 뛰고난후동작
5 사망
6 걷기전쟁모드변경
7 뛰기전쟁모드변경
8 공격1
9 공격2
10 공격3
11 공격4
12 공격5
13 뛰면서공격1
14 뛰면서공격2
15 뛰면서공격3
16 백마법시전
17 흑마법시전
18 이모티콘(피로)
*/

#define STATE_STOP						0
#define STATE_WALK						1
#define STATE_RUN						2
#define STATE_DIE						3
#define STATE_WALK_MODECHANGE			4
#define STATE_RUN_MODECHANGE			5
#define STATE_ATTACK1					6
#define STATE_ATTACK2					7
#define STATE_ATTACK3					8
#define STATE_ATTACK4					9
#define STATE_ATTACK5					10
#define STATE_RUN_ATTACK1				11
#define STATE_RUN_ATTACK2				12
#define STATE_RUN_ATTACK3				13
#define STATE_FISHING					14

// 공격형 마법
#define STATE_MAGIC_ATTACK_UPANDDOWN	14
#define STATE_MAGIC_ATTACK_DOWNANDUP	15
#define STATE_MAGIC_ATTACK_GO			16
#define STATE_MAGIC_ATTACK_AROUND		17

// 축복
#define STATE_MAGIC_BLESS_UPANDDOWN		18
#define STATE_MAGIC_BLESS_DOWNANDUP		19
#define STATE_MAGIC_BLESS_GO			20
#define STATE_MAGIC_BLESS_AROUND		21

// 소환
#define STATE_MAGIC_RECALL_UPANDDOWN	22
#define STATE_MAGIC_RECALL_DOWNANDUP	23
#define STATE_MAGIC_RECALL_GO			24
#define STATE_MAGIC_RECALL_AROUND		25

// 저
#define STATE_MAGIC_CURSE_UPANDDOWN		26
#define STATE_MAGIC_CURSE_DOWNANDUP		27
#define STATE_MAGIC_CURSE_GO			28
#define STATE_MAGIC_CURSE_AROUND		29

//////////////////////////////////////////////////////

#define MODE_STOP			0
#define MODE_WALK			1
#define MODE_RUN			2
#define MODE_WALK_ATTACK	3
#define MODE_RUN_ATTACK		4
#define MODE_ATTACK			5
#define MODE_DEAD			6
#define MODE_ATTACK_RUN		7
#define MODE_MAGIC_CASTING	8

//////////////////////////////////////////////////////

#define MAX_WEAPON		25
#define MAX_STATE		12
#define MAX_VEHICLE		10

//////////////////////////////////////////////////////

#define MODE_PEACE			0
#define MODE_WAR			1
#define MODE_VEHICLE_HORSE	2
#define MODE_VEHICLE		2

//////////////////////////////////////////////////////


namespace Vehicle
{
	/**
	 *
	 */
	struct  SMotion
	{
		BOOL	bUse;				///< 애니메이션이 있는지의 여부
		DWORD	dwBegin;			///< 시작 프레임
		DWORD	dwEnd;				///< 끝 프레임

		IW3DAnimation* pAnimation;	///< 애니메이션
		IW3DAnimation* pWeapon;		///< 무기용 애니메이션

		WORD	wSoundFile;
		
		void	Clear()
		{
			dwBegin = 1;
			dwEnd = 0;
			pAnimation	= NULL;
			pWeapon		= NULL;
			wSoundFile	= 0;
			bUse		= FALSE;
		} //Clear

	}; //struct SMotion

	/**
	 *
	 */
	struct SAct
	{
		int			nCount;
		int			nSelect;
		SMotion		aMotion[3];

		void		Clear()
		{
			nCount = 0;
			nSelect = 0;

			for( int i = 0; i < 3 ; i++ )
			{
				aMotion[i].Clear();
			} //for
		} //if
	}; // struct SAction

	/**
	 *
	 */
	struct SMode
	{
		int			nCount;
		SAct		aAct[50];

		void		Clear()
		{
			nCount = MAX_STATE;
			for( int i = 0 ; i < 50 ; i++ )
			{
				aAct[i].Clear();
			} //for
		} //if
	}; //struct SMode

	/**
	 *
	 */
	struct SWeapon
	{
		BOOL		bUse;
		int			nCount;
		SMode		aMode[MAX_VEHICLE];

		void		Clear()
		{
			nCount = 0;
		
			for( int i = 0 ; i < MAX_VEHICLE ; i++ )
			{
				aMode[i].Clear();
			} //for
		} //if
	}; //struct SWeapon
} //namespace 

/**
 *
 */
class CVehicleAction
{
public:
	UNDER_MEMORY_MGR( CVehicleAction );

	/**
	 *
	 */
	CVehicleAction();

	/**
	 *
	 */
	~CVehicleAction();

	/**
	 *	초기화 ( 인자는 애니메이션의 종료 여부를 확인하기 위한 더미 )
	 */
	void			Init( IW3DModel* pModel );

	/**
	 *
	 */
	void			SetModel( IW3DModel* pModel );

	/**
	 *	성별 설정
	 */
	void			SetSex( UINT nSex );

	/**
	 *	Mode 설정
	 */
	void			SetMode( WORD wMode );

	/**
	 *	무기를 설정
	 */
	void			SetWeapon( WORD wWeapon );

	/**
	 *	무기를 설정( 아이템 값이 아닌, 무기 종류를 인자로 )
	 */
	void			SetWeapon2( WORD wWeapon );
	
	/**
	 * 사운드를 설정
	 */
	void			SetSound( IW3DSoundObject3D* pSound )
	{
		Assert( pSound );
		m_pSound = pSound;
	}

	/**
	 * 사운드 파일 번호를 반환
	 */
	DWORD			GetSoundFile()
	{
		return m_pMotion->wSoundFile;
	}

	/**
	 *	Action을 얻는다.
	 */
	SAction*		GetAction();

	/**
	 * Mode에 맞게, Action 에 적용
	 */
	void			ApplyAction();

	/**
	 *	설정된 Option을 Action에 적용
	 */
	void			ChangeAction();

	/**
	 *	갱신
	 */
	void			Update( DWORD dwTick );

	/**
	 *	동작이 끝났는지의 여부
	 */
	BOOL			IsEnd()
	{
		 return m_bEndAction;
	} //IsEnd

	/**
	 *
	 */
	void			SetState( WORD wState );

	/**
	 *
	 */
	void			SetLevel( BYTE btLevel );

	/**
	 *
	 */
	BOOL			IsEndAction();

	void			SetTime( DWORD dwTime )
	{
		m_dwTime = dwTime;
	} //SetTime
	
	/**
	 *
	 */
	DWORD			GetTime()
	{
		return m_dwTime;
	} //GetTime

	/**
	 *
	 */
	WORD			GetType()
	{
		return m_wMode;
	} //GetType
	
	void			SetUnderWater( BOOL bUnder )
	{
		m_bUnderWater = bUnder;
	}

private:
	Vehicle::SWeapon*		m_pWeapon;		///< 무기
	Vehicle::SMode*			m_pMode;		///< 모드
	Vehicle::SAct*			m_pAct;			///< 
	Vehicle::SMotion*		m_pMotion;		///< 모션 
	SAction*				m_pAction;		///< 액션 ( 모션 데이타 )

	DWORD					m_dwTick;		///< 경과 틱
	DWORD					m_dwTime;		///< 누적 틱
	DWORD					m_dwDeadLine;	///< 한 동작 당 이 시간을 넘길수 없다.

	IW3DModel*				m_pModel;		///< 애니메이션 확인용 Model

//	BOOL					m_bSex;			///< 성별( 0 = 남자, 1 = 여자 )
	UINT					m_nSex;			///< 0 : 남자, 1 : 여자, 2 : 운영자
	WORD					m_wWeapon;		///< 무기 
	WORD					m_wMode;		///< 모드 ( 평화, 전쟁, 말 ... )
	WORD					m_wState;		///< 상태
	WORD					m_wLevel;		///< 레벨 ( 상, 중, 하 )

	WORD					m_wAction;		///< 캐릭터 행동
	WORD					m_wAdvanced;	///< 예비 행동
	WORD					m_wVehicleType;	///< 탈것의 타입
	BOOL					m_bEndAction;	///< 모든 행동이 끝났는가?
	BOOL					m_bSameChange;  ///< 같은 행동을 지시 받았을때, 새로 갱신할지의 여부
	BOOL					m_bWithShield;	///< 방패 착용 여부

	BYTE					m_btMagicKind;	///< 마법 종류
	BYTE					m_btMagicKind2; ///< 마법 소분

	int						m_nMagicRepeat; ///< 마법 중간 반복

	IW3DSoundObject3D*		m_pSound;		///< 사운드 파일
	DWORD					m_dwSoundTick;	///< 사운드 틱
	BOOL					m_bUnderWater;	///< UnderWater?

private:

	/**
	 *
	 */
	WORD			GetWeaponId( WORD wWeapon );

	/**
	 *
	 */
	WORD			GetVehicleType( WORD wId );


}; //class CVehicleAction

#endif //#ifndef __VehicleAction_Header__
