/**
 * 아이템 정보 관리자 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 05-02-19 7:11p $
 * @version	$Revision: 54 $
 *
 * <HR>
 * $Log: /Engine/Inc/IW3DItemInfoMgr.h $
 * 
 * 54    05-02-19 7:11p Sk8snow2
 * 
 * 53    04-09-25 2:01p Paran2
 * 
 * 52    04-09-24 4:46p Paran
 * 
 * 51    04-08-27 8:01p Paran
 * 
 * 50    04-08-26 3:31p Paran
 * 
 * 49    04-08-23 8:39p Paran
 * 
 * 46    04-03-28 1:16p Sckyj			낚시대 근거리 아이템 추가
 * 
 * 45    03-12-31 2:44p Paran			스킬
 * 
 * 37    03-12-16 5:04p Paran			아이템 데이터 수정
 * 
 * 36    03-12-12 11:57a Paran			item.dat 로드 수정
 * 
 * 35    03-12-10 1:12a Paran			스킬정보 수정
 * 
 * 34    03-12-09 10:02p Paran			스킬데이터 추가
 * 
 * 31    03-12-09 4:01p Paran			아이템 데이터 로드수정
 * 
 * 9     03-06-21 12:39p Paran			팁 정보 출력 함수 추가.
 * <HR>
 *
 * @file	IW3DItemInfoMgr.h
 */

#ifndef __Wind3D_Engine_IW3DItemInfoMgr_Header__
#define __Wind3D_Engine_IW3DItemInfoMgr_Header__

#define  WIDTH_TIPLINEBOX		250
#define  HEIGHT_TIPLINEBOX		 13
#define  MALE					  0
#define  FEMALE					  1

#define	 dTITLE_LINE_CNT		  8
#define	 dTITLE_NAME_CNT		 18

////////////////////////////////////////////////////////////////
// MAGIC ITEM NUM
////////////////////////////////////////////////////////////////
#define dMAGIC_NUM_CALLLIGHTNING			2512		// 콜라이트닝 
#define dMAGIC_NUM_PRODUCEICE				2513		// 프로듀스 아이스
#define dMAGIC_NUM_SPIKEICE					2514		// 스파이크 아이스 
#define dMAGIC_NUM_LIGHTNINGPIERCE			2515		// 라이트닝 피어스 
#define dMAGIC_NUM_FIREPIERCE				2516		// 화이어 피어스
#define dMAGIC_NUM_CURSECALLLIGHTNING		2517		// 커즈 콜라이트닝
#define dMAGIC_NUM_BLESS					2518		// 블레스 
#define dMAGIC_NUM_REMOVEPIRE				2519		// 리무브피어
#define dMAGIC_NUM_CURELIGHT				2520		// 큐어 라이트
#define dMAGIC_NUM_CUREDIGIZ				2521		// 큐어 디지즈
#define dMAGIC_NUM_BUCKSKIN					2522		// 박스킨
#define dMAGIC_NUM_MAGICBESTMENT			2523		// 매직베스트먼트
#define dMAGIC_NUM_FLAMINGSPHERE			2501		// 플래밍 스피어 
#define dMAGIC_NUM_FLAMEARROW				2502		// 플래임에로우
#define dMAGIC_NUM_LIGHTNINGSTORM			2503		// 라이트닝스톰 
#define dMAGIC_NUM_FIREWALL					2504		// 화이어월 
#define dMAGIC_NUM_CHANELIGHTNING			2505		// 라이트닝애로우
#define dMAGIC_NUM_CLOUDKILL				2506		// 클라우드킬
#define dMAGIC_NUM_MAGICSWORD				2507		// 매직스워드
#define dMAGIC_NUM_GLASSIP					2508		// 글래십 
#define dMAGIC_NUM_ICESWORD					2509		// 아이스스워드 
#define dMAGIC_NUM_SHIELD					2510		// 쉴드 
#define dMAGIC_NUM_FIRESHIELD				2511		// 화이어쉴드


/////////////////////////////////////////////////////////////////
// I  T  E  M     T  Y  P  E
/////////////////////////////////////////////////////////////////
#define dITEMTYPE_NONE						0		//"0.미설정",
#define dITEMTYPE_ONEHANDSWORD				1		//"1.한손검",
#define dITEMTYPE_TWOHANDSWORD				2		//"2.양손검",
#define dITEMTYPE_DUALSWORD					3		//"3.쌍칼",
#define dITEMTYPE_ONEHANDAXE				4		//"4.한손도끼",
#define dITEMTYPE_TWOHANDAXE				5		//"5.양손도끼",
#define dITEMTYPE_DUALAXE					6		//"6.쌍도끼",
#define dITEMTYPE_ONEHANDBLUNT				7		//"7.한손둔기",
#define dITEMTYPE_TWOHANDBLUNT				8		//"8.양손둔기",
#define dITEMTYPE_DUALBLUNT					9		//"9.쌍둔기",
#define dITEMTYPE_DONPA						10		//"10.돈파",
#define dITEMTYPE_SHORTBOW					11		//"11.단궁",
#define dITEMTYPE_LONGBOW					12		//"12.장궁",
#define dITEMTYPE_DDABALBOW					13		//"13.연노",
#define dITEMTYPE_CROSSBOW					14		//"14.석궁",
#define dITEMTYPE_ARROW						15		//"15.화살",
#define dITEMTYPE_SPHERE					16		//"16.창",
#define dITEMTYPE_BONG						17		//"17.봉",
#define dITEMTYPE_STAFF						18		//"18.지팡이",
#define dITEMTYPE_WORRIOR_UP_ARMOR			19		//"19.전사갑옷(상의)",
#define dITEMTYPE_WORRIOR_LO_ARMOR			20		//"20.전사갑옷(하의)",
#define dITEMTYPE_ARCHER_UP_ARMOR			21		//"21.궁수갑옷(상의)",
#define dITEMTYPE_ARCHER_LO_ARMOR			22		//"22.궁수갑옷(하의)",
#define dITEMTYPE_WIZARD_UP_ARMOR			23		//"23.법사갑옷(상의)",
#define dITEMTYPE_WIZARD_LO_ARMOR			24		//"24.법사갑옷(하의)",
#define dITEMTYPE_WORRIOR_HELMET			25		//"25.전사투구",
#define dITEMTYPE_ARCHER_HELMET				26		//"26.궁수투구",
#define dITEMTYPE_WIZARD_HELMET				27		//"27.법사투구",
#define dITEMTYPE_WARRIOR_BELT				28		//"28.전사벨트",
#define dITEMTYPE_ARCHER_BELT				29		//"29.궁수벨트",
#define dITEMTYPE_WIZZARD_BELT				30		//"30.법사벨트",
#define dITEMTYPE_WARRIOR_GLOVE				31		//"31.전사장갑",
#define dITEMTYPE_ARCHER_GLOVE				32		//"32.궁수장갑",
#define dITEMTYPE_WIZZARD_GLOVE				33		//"33.법사장갑",
#define dITEMTYPE_WARRIOR_SHOES				34		//"34.전사신발",
#define dITEMTYPE_ARCHER_SHOES				35		//"35.궁수신발",
#define dITEMTYPE_WIZZARD_SHOES				36		//"36.법사신발",
#define dITEMTYPE_SHIELD					37		//"37.방패",
#define dITEMTYPE_NECKLACE					38		//"38.목걸이",
#define dITEMTYPE_RING						39		//"39.반지",
#define dITEMTYPE_JEWEL						40		//"40.보석",
#define dITEMTYPE_SCROLL					41		//"41.스크롤",
#define dITEMTYPE_MAGICSCROLL				42		//"42.마법스크롤",
#define dITEMTYPE_CONTAINER					43		//"43.가방(상자)",
#define dITEMTYPE_POTION					44		//"44.포션",
#define dITEMTYPE_OBJECT					45		//"45.오브젝트",
#define dITEMTYPE_FOOD						46		//"46.먹을것",
#define dITEMTYPE_ONETIME					47		//"47.일회용",
#define dITEMTYPE_FISHTACKLE				48		//"48.낚시대"

/////////////////////////////////////////////////////////////////
// V E H I C L E     T Y P E 
/////////////////////////////////////////////////////////////////
#define dITEMTYPE_NONE						0		//"0.미설정",
#define dITEMTYPE_HORSE						1		//"1.말",

/////////////////////////////////////////////////////////////////
// S K I L L     T Y P E 
/////////////////////////////////////////////////////////////////
#define dSKILL_TYPE_SATTACK					0			// 근거리 공격기술
#define dSKILL_TYPE_SASSIST					1			// 근거릴 보조기술
#define dSKILL_TYPE_LATTACK					2			// 원거리 공격기술
#define dSKILL_TYPE_LASSIST					3			// 원거리 보조기술 
#define dSKILL_TYPE_BMAGIC					4			// 공격계 흑마법 
#define dSKILL_TYPE_BASSIST					5			// 보조계 흑마법 
#define dSKILL_TYPE_WMAGIC					6			// 공격계 백마법
#define dSKILL_TYPE_WASSIST					7			// 보조계 백마법
#define dSKILL_TYPE_SPATTACK				8			// 공격계 특화
#define dSKILL_TYPE_SPASSIST				9			// 보조계 특화

#define IS_ATTACK_MAGIC( skill ) ( skill->shSkillType == dSKILL_TYPE_BMAGIC || skill->shSkillType == dSKILL_TYPE_WMAGIC || skill->shSkillType == dSKILL_TYPE_SPATTACK )
#define IS_SATTACK( skill ) ( skill->shSkillType == dSKILL_TYPE_SATTACK || skill->shSkillType == dSKILL_TYPE_SASSIST )
#define IS_LATTACK( skill ) ( skill->shSkillType == dSKILL_TYPE_LATTACK || skill->shSkillType == dSKILL_TYPE_LASSIST )
#define IS_BMAGIC( skill ) ( skill->shSkillType == dSKILL_TYPE_BMAGIC || skill->shSkillType == dSKILL_TYPE_BASSIST )
#define IS_WMAGIC( skill ) ( skill->shSkillType == dSKILL_TYPE_WMAGIC || skill->shSkillType == dSKILL_TYPE_WASSIST )

#define dSKILL_KIND_MAGIC					0			// 일반 스킬
#define dSKILL_KIND_LIFE					1			// 생활 스킬
#define dSKILL_KIND_GUILD					2			// 단 스킬
#define dSKILL_KIND_COUNTRY					3			// 군단 스킬
#define dSKILL_KIND_SPECIAL					4			// 특화 스킬

#define IS_SKILL_MAGIC( skill )		( skill->btSkiiKind == dSKILL_KIND_MAGIC )
#define IS_SKILL_SPECIAL( skill )	( skill->btSkiiKind == dSKILL_KIND_SPECIAL )
#define IS_SKILL_LIFE( skill )		( skill->btSkiiKind == dSKILL_KIND_LIFE )
#define IS_SKILL_GUILD( skill )		( skill->btSkiiKind == dSKILL_KIND_GUILD )
#define IS_SKILL_COUNTRY( skill )	( skill->btSkiiKind == dSKILL_KIND_COUNTRY )

#define ISSHORTWEAPON( i )							\
	( ( (i) == dITEMTYPE_ONEHANDSWORD	||			\
	  (i) ==dITEMTYPE_TWOHANDSWORD	||				\
	  (i) == dITEMTYPE_DUALSWORD	||				\
	  (i) == dITEMTYPE_ONEHANDAXE	||				\
	  (i) == dITEMTYPE_TWOHANDAXE	||				\
	  (i) == dITEMTYPE_DUALAXE		||				\
	  (i) == dITEMTYPE_ONEHANDBLUNT	||				\
	  (i) == dITEMTYPE_TWOHANDBLUNT	||				\
	  (i) == dITEMTYPE_DUALBLUNT	||				\
	  (i) == dITEMTYPE_DONPA		||				\
	  (i) == dITEMTYPE_SPHERE		||				\
	  (i) == dITEMTYPE_BONG			||				\
	  (i) == dITEMTYPE_STAFF 		||				\
	  (i) == dITEMTYPE_FISHTACKLE )					\
	  ? TRUE : FALSE )

#define ISLONGWEAPON( i )							\
	( ( (i) == dITEMTYPE_SHORTBOW		||			\
	  (i) == dITEMTYPE_LONGBOW		||				\
	  (i) == dITEMTYPE_DDABALBOW	||				\
	  (i) == dITEMTYPE_CROSSBOW		||				\
	  (i) == dITEMTYPE_ARROW )						\
	  ? TRUE : FALSE )

#define ISUPPERARMOR( i )							\
	( ( (i) == dITEMTYPE_WORRIOR_UP_ARMOR ||		\
	  (i) == dITEMTYPE_ARCHER_UP_ARMOR  ||			\
	  (i) == dITEMTYPE_WIZARD_UP_ARMOR	)			\
	  ? TRUE : FALSE )

#define ISLOWERARMOR( i )							\
	( ( (i) == dITEMTYPE_WORRIOR_LO_ARMOR ||		\
	  (i) == dITEMTYPE_ARCHER_LO_ARMOR  ||			\
	  (i) == dITEMTYPE_WIZARD_LO_ARMOR	)			\
	  ? TRUE : FALSE )
		
#define ISHELMET( i )								\
	( ( (i) == dITEMTYPE_WORRIOR_HELMET ||			\
	  (i) == dITEMTYPE_ARCHER_HELMET  ||			\
	  (i) == dITEMTYPE_WIZARD_HELMET )				\
	  ? TRUE : FALSE )

#define ISBELT( i )									\
	( ( (i) == dITEMTYPE_WARRIOR_BELT	||			\
	  (i) == dITEMTYPE_ARCHER_BELT	||				\
	  (i) == dITEMTYPE_WIZZARD_BELT )				\
	  ? TRUE : FALSE )

#define ISGLOVE( i )								\
	( ( (i) == dITEMTYPE_WARRIOR_GLOVE	||			\
	  (i) == dITEMTYPE_ARCHER_GLOVE		||			\
	  (i) == dITEMTYPE_WIZZARD_GLOVE )				\
	  ? TRUE : FALSE )

#define ISSHOES( i )								\
	( ( (i) == dITEMTYPE_WARRIOR_SHOES	||			\
	  (i) == dITEMTYPE_ARCHER_SHOES		||			\
	  (i) == dITEMTYPE_WIZZARD_SHOES )				\
	  ? TRUE : FALSE )

#define ISONEHANDWEAPON( i )						\
	( ( (i) == dITEMTYPE_ONEHANDSWORD ||			\
		(i) == dITEMTYPE_ONEHANDAXE	  ||			\
		(i) == dITEMTYPE_ONEHANDBLUNT )				\
	  ? TRUE : FALSE )

#define ISTWOHANDWEAPON( i )						\
	( ( (i) == dITEMTYPE_TWOHANDSWORD ||			\
		(i) == dITEMTYPE_TWOHANDAXE	  ||			\
		(i) == dITEMTYPE_TWOHANDBLUNT )				\
	  ? TRUE : FALSE )

#define ISWEAPONITEM( i )							\
	( ( ISSHORTWEAPON( i ) || ISLONGWEAPON( i ) ) ? TRUE : FALSE )

#define ISDEFENSEITEM( i )							\
	( ( (i) >= 19 && (i) <= 37 ) ? TRUE : FALSE )

#define ISMAGICSCROLL( i )							\
	( ( (i) == dITEMTYPE_MAGICSCROLL ) ? TRUE : FALSE )

#define ISQUICKITEM( i )							\
	( ( (i) == dITEMTYPE_MAGICSCROLL || (i) == dITEMTYPE_POTION ) ? TRUE : FALSE )


#define EQUIP_SHORTWEAPON			1
#define EQUIP_LONGWEAPON			2
#define EQUIP_UPPERARMOR			3
#define EQUIP_LOWERARMOR			4
#define EQUIP_HELMET				5
#define EQUIP_BELT					6
#define EQUIP_GLOVE					7
#define EQUIP_SHOES					8
#define EQUIP_SHIELD				9
#define EQUIP_NECKLACE				10
#define EQUIP_RING					11
#define EQUIP_JEWEL					12
#define EQUIP_SCROLL				13
#define EQUIP_MAGICSCROLL			14
#define EQUIP_CONTAINER				15
#define EQUIP_POTION				16
#define EQUIP_OBJECT				17
#define EQUIP_FOOD					18
#define EQUIP_ONETIME				19

#define ITEM_SBASICSKILL			10101
#define ITEM_LBASICSKILL			10102
#define ITEM_BBASICSKILL			10103
#define ITEM_WBASICSKILL			10112
#define ITEM_HP_L					10097		///< HP
#define ITEM_HP_M					10096		///< HP
#define ITEM_HP_S					10095		///< HP
#define ITEM_MP_L					10100		///< MP
#define ITEM_MP_M					10099		///< MP
#define ITEM_MP_S					10098		///< MP
#define ITEM_TELEPORT_VAIL			10175		///< 텔레포트 (vail)
#define ITEM_TELEPORT_LEGYON		10176		///< 텔레포트 (legyon)
#define ITEM_TELEPORT_MOHAS			10177		///< 텔레포트 (mohas)
#define ITEM_TELEPORT_MIRAS			10178		///< 텔레포트 (miras)
#define ITEM_MONEY					10196		///< 돈.
#define ITEM_HORSEBR				10188		///< 말.
#define ITEM_HORSEB					10189		///< 말.
#define ITEM_HORSEW					10190		///< 말.
#define ITEM_ENCHANT_WEAPON			10193		///< Weapon Enchant
#define ITEM_ENCHANT_ARMOR			10194		///< Armor Enchant
#define ITEM_ENCHANT_OPTION			10195		///< Option Enchant

#define ITEM_WOARAIM13_10H			11907		///< 웜쓰오브 아라임
#define ITEM_WOARAIM13_20H			11908		///< 웜쓰오브 아라임
#define ITEM_WOARAIM13_30H			11909		///< 웜쓰오브 아라임
#define ITEM_WOARAIM13_1M			11991		///< 웜쓰오브 아라임
#define ITEM_WOARAIM15_10H			11910		///< 웜쓰오브 아라임
#define ITEM_WOARAIM15_20H			11911		///< 웜쓰오브 아라임
#define ITEM_WOARAIM15_30H			11912		///< 웜쓰오브 아라임
#define ITEM_WOARAIM15_1M			11992		///< 웜쓰오브 아라임

#define ITEM_AOBRILIANCE_5			11916		///< 오오라 오브 브릴리언스
#define ITEM_AOBRILIANCE_10			11917		///< 오오라 오브 브릴리언스
#define ITEM_AOBRILIANCE_15			11918		///< 오오라 오브 브릴리언스

#define ITEM_PHANTOMGET_5			11919		///< 팬텀 게더러
#define ITEM_PHANTOMGET_10			11920		///< 팬텀 게더러
#define ITEM_PHANTOMGET_15			11921		///< 팬텀 게더러

#define ITEM_SHOUTPILL_10			11922		///< 외침용 알약				
#define ITEM_SHOUTPILL_20			11923		///< 외침용 알약
#define ITEM_SHOUTPILL_30			11924		///< 외침용 알약

#define ITEM_IOARAIM13_10H			11925		///< 아이템 오브 아라임
#define ITEM_IOARAIM13_20H			11926		///< 아이템 오브 아라임
#define ITEM_IOARAIM13_30H			11927		///< 아이템 오브 아라임
#define ITEM_IOARAIM13_1M			11993		///< 아이템 오브 아라임
#define ITEM_IOARAIM15_10H			11928		///< 아이템 오브 아라임
#define ITEM_IOARAIM15_20H			11929		///< 아이템 오브 아라임
#define ITEM_IOARAIM15_30H			11930		///< 아이템 오브 아라임
#define ITEM_IOARAIM15_1M			11994		///< 아이템 오브 아라임

#define ITEM_REVIVEROLLPAPER_5		11931		///< 부활의 두루마리
#define ITEM_REVIVEROLLPAPER_10		11932		///< 부활의 두루마리
#define ITEM_REVIVEROLLPAPER_15		11933		///< 부활의 두루마리

#define ITEM_GUILDNOTICEPILL10		11934		///< 길드공지의 알약
#define ITEM_GUILDNOTICEPILL20		11935		///< 길드공지의 알약
#define ITEM_GUILDNOTICEPILL30		11936		///< 길드공지의 알약

#define ITEM_PREMIUMZONE10H			11937		///< 프리미엄존 서비스
#define ITEM_PREMIUMZONE20H			11938		///< 프리미엄존 서비스
#define ITEM_PREMIUMZONE30H			11939		///< 프리미엄존 서비스
#define ITEM_PREMIUMZONE1M			11995		///< 프리미엄존 서비스

#define ITEM_PREMIUMACCOUNT10H		11940		///< 프리미엄계정 서비스
#define ITEM_PREMIUMACCOUNT20H		11941		///< 프리미엄계정 서비스
#define ITEM_PREMIUMACCOUNT30H		11942		///< 프리미엄계정 서비스
#define ITEM_PREMIUMACCOUNT1M		11996		///< 프리미엄계정 서비스

#define ITEM_BLESSOFGOD10H			11946		///< 신의 축복
#define ITEM_BLESSOFGOD20H			11947		///< 신의 축복
#define ITEM_BLESSOFGOD30H			11948		///< 신의 축복
#define ITEM_BLESSOFGOD1M			11997		///< 신의 축복

#define ITEM_ENCHANTCHANGE5			11949		///< 인첸트 체인지
#define ITEM_ENCHANTCHANGE10		11950		///< 인첸트 체인지
#define ITEM_ENCHANTCHANGE15		11951		///< 인첸트 체인지

#define ITEM_ENCHANTCOLLECTION5		11952		///< 인첸트 콜렉션
#define ITEM_ENCHANTCOLLECTION10	11953		///< 인첸트 콜렉션
#define ITEM_ENCHANTCOLLECTION15	11954		///< 인첸트 콜렉션

#define ITEM_ROOK10					11955		///< 아이템 룩
#define ITEM_ROOK20					11956		///< 아이템 룩
#define ITEM_ROOK30					11957		///< 아이템 룩

#define ITEM_PROTECTION10			11958		///< 아이템 프로텍션
#define ITEM_PROTECTION20			11959		///< 아이템 프로텍션
#define ITEM_PROTECTION30			11960		///< 아이템 프로텍션

#define ITEM_LVROOK10				11961		///< 레벨 룩
#define ITEM_LVROOK20				11962		///< 레벨 룩
#define ITEM_LVROOK30				11963		///< 레벨 룩

#define ITEM_LVPROTECTION10			11964		///< 레벨 프로텍션
#define ITEM_LVPROTECTION20			11965		///< 레벨 프로텍션
#define ITEM_LVPROTECTION30			11966		///< 레벨 프로텍션

#define ITEM_MOBILELIQUOR20H		11967		///< 기동성 물약
#define ITEM_MOBILELIQUOR40H		11968		///< 기동성 물약
#define ITEM_MOBILELIQUOR1M			11969		///< 기동성 물약

#define ITEM_WEIGHTDRINK5			11970		///< 웨이트 드링크
#define ITEM_WEIGHTDRINK10			11971		///< 웨이트 드링크
#define ITEM_WEIGHTDRINK15			11972		///< 웨이트 드링크

#define ITEM_HPDRINK5				11973		///< HP 드링크
#define ITEM_HPDRINK10				11974		///< HP 드링크
#define ITEM_HPDRINK15				11975		///< HP 드링크

#define ITEM_MPDRINK5				11976		///< MP 드링크
#define ITEM_MPDRINK10				11977		///< MP 드링크
#define ITEM_MPDRINK15				11978		///< MP 드링크

#define ITEM_GWOARAIM1				11913		///< 그룹 웜스 오브 아라임
#define ITEM_GWOARAIM5				11914		///< 그룹 웜스 오브 아라임
#define ITEM_GWOARAIM10				11915		///< 그룹 웜스 오브 아라임

#define ITEM_GROUPHPDRINK1			11979		///< 그룹 HP 드링크
#define ITEM_GROUPHPDRINK5			11980		///< 그룹 HP 드링크
#define ITEM_GROUPHPDRINK10			11981		///< 그룹 HP 드링크

#define ITEM_GROUPMPDRINK1			11982		///< 그룹 MP 드링크
#define ITEM_GROUPMPDRINK5			11983		///< 그룹 MP 드링크
#define ITEM_GROUPMPDRINK10			11984		///< 그룹 MP 드링크

#define ITEM_GROUPHPARMYDRINK1		11985		///< 군단원 HP 드링크
#define ITEM_GROUPHPARMYDRINK5		11986		///< 군단원 HP 드링크
#define ITEM_GROUPHPARMYDRINK10		11987		///< 군단원 HP 드링크

#define ITEM_GROUPMPARMYDRINK1		11988		///< 군단원 MP 드링크
#define ITEM_GROUPMPARMYDRINK5		11989		///< 군단원 MP 드링크
#define ITEM_GROUPMPARMYDRINK10		11990		///< 군단원 MP 드링크

#define ITEM_SUMMON_GUILD_MEMBER	11998	

#define ITEM_MINUS					12016

#define ITEM_WARCRYOFCURSE			12010
#define ITEM_WARCRYOFPROVOCATION	12013
#define ITEM_WARCRYOFVICTORY		12001
#define ITEM_WARCRYOFBLESSING		12004
#define ITEM_WARCRYOFPRAYER			12007

#define ITEM_HEARTEMOTICON			12019
#define ITEM_HAPPYEMOTICON			12022
#define ITEM_SORROWEMOTICON			12025
#define ITEM_WRATHEMOTICON			12028



#define ITEM_LINESKIP				10197		///< 라인.
#define ITEM_PAGESKIP				10198		///< 페이지.

#define AM_ENCHANT_CARD				12098		///아머인첸트카드
#define WP_ENCHANT_CARD				12097		///웨폰인첸트카드

#define ISSKIPITEM( i )												\
	( ( ITEM_LINESKIP == i || ITEM_PAGESKIP == i ) ? TRUE : FALSE )

#define ISENCHANT( i )												\
	( ( ITEM_ENCHANT_WEAPON == i ||									\
		ITEM_ENCHANT_ARMOR  == i ||									\
		ITEM_ENCHANT_OPTION == i ) ? TRUE : FALSE )

#define SMITHENCHANT( i )												\
	( ( WP_ENCHANT_CARD == i ||									\
		AM_ENCHANT_CARD  == i ) ? TRUE : FALSE )



#define ISENCHANTCHANGE( i )										\
	( ( ITEM_ENCHANTCHANGE5  == i ||								\
		ITEM_ENCHANTCHANGE10 == i ||								\
		ITEM_ENCHANTCHANGE15 == i ) ? TRUE : FALSE )

#define ISENCHANTCOLLECTION( i )									\
	( ( ITEM_ENCHANTCOLLECTION5  == i ||							\
		ITEM_ENCHANTCOLLECTION10 == i ||							\
		ITEM_ENCHANTCOLLECTION15 == i ) ? TRUE : FALSE )

#define ISITEMROOK( i )								\
	( ( ITEM_ROOK10 == i ||							\
		ITEM_ROOK20 == i ||							\
		ITEM_ROOK30 == i ) ? TRUE : FALSE )

#define ISLEVELROOK( i )							\
	( ( ITEM_LVROOK10 == i ||						\
		ITEM_LVROOK20 == i ||						\
		ITEM_LVROOK30 == i ) ? TRUE : FALSE )

#define ISPOTION( i )												\
	( ( ITEM_HP_L == i || ITEM_HP_M == i || ITEM_HP_S == i ||		\
	  ITEM_MP_L == i || ITEM_MP_M == i || ITEM_MP_S == i ) ? TRUE : FALSE )

#define IS_CASH_CNT_ITEM( i )										\
	( (  i <= 12030 && i >= ITEM_WOARAIM13_10H ) ? TRUE : FALSE )

#define ISHORSE( i )													\
	( ( ITEM_HORSEBR == i || ITEM_HORSEB == i || ITEM_HORSEW == i ) ? TRUE : FALSE )

#define ITEMFILENAME1( sex, info )	\
	( (sex) == MALE ) ? (info)->szMimageFile1 : (info)->szFimageFile1

#define ITEMFILENAME2( sex, info )	\
	( (sex) == MALE ) ? (info)->szMimageFile2 : (info)->szFimageFile2

#define ITEMFILENAME3( sex, info )	\
	( (sex) == MALE ) ? (info)->szMimageFile3 : (info)->szFimageFile3

#define ITEMFILENAME4( sex, info )	\
	( (sex) == MALE ) ? (info)->szMimageFile4 : (info)->szFimageFile4

#define ITEMFILENAME5( sex, info )	\
	( (sex) == MALE ) ? (info)->szMimageFile5 : (info)->szFimageFile5

#define ITEMHAVESKIN( sex, info )	\
	( (sex) == MALE ) ? (info)->btMHaveSkin : (info)->btFHaveSkin


/**
 * 스킬 데이터 구조
 * @ingroup		Engine
 */
struct SSkillInfoData
{
	int			nItemNum;			// 아이템 번호
	char		szHName[50];		// 한글이름 
	WORD		wItemNum;			// 아이템 번호

	BYTE		btSkiiKind;			// 기술종류
	short		shSkillType;		// 기술형태
	short		shMobTimer;			// 몹타이머
	BYTE		btCheckReuse;		// 중복검색
	short		shSkillAttr;		// 기술속성
	BYTE		btNeedStr;			// 요구힘
	BYTE		btNeedDex;			// 요구민첩
	BYTE		btNeedInt;			// 요구지혜
	BYTE		btNeedCha;			// 요구매력
	BYTE		btNeedClass;		// 습득가능직업

	WORD		wMinDamage;			// 최소데미지
	WORD		wMaxDamage;			// 최대데미지

	BYTE		btEquipType;		// 장착종류
	short		shAttr;				// 성향
	WORD		wNeedItem;			// 시전필요재료 

	WORD		wNeedHP;			// 시전소모 HP
	WORD		wNeedMP;			// 시전소모 MP
	
	BYTE		btTargetType;		// 시전대상
	BYTE		btRange;			// 시전거리
	BYTE		btTimer;			// 지속시간 

	BYTE		btSuccessDice;		// 최소시전확률
	BYTE		btSuccessDexDice;	// 추가시전확률 민첩
	BYTE		btSuccessIntDice;	// 추가시전확률 지혜
	BYTE		btExtraCode;		// 추가기능코드
	BYTE		btCodeVariable;		// 코드변수

	char		szSkillInfo1[64];	//내용
	char		szSkillInfo2[64];	//내용
	char		szLevel[64];		//정보
	char		szRange[64];		
}; //struct SSkillInfoData

/**
 * 아이템 데이터 구조
 * @ingroup		Engine
 */
struct SItemInfoData
{
	WORD		wItemNum;			// 아이템번호 
	BYTE		bItemIndex;			// 유니크 

	char		szHName[64];		// 한글이름 
	char		szEName[64];		// 영문이름

	BYTE		btItemType;			// 아이템 종류 
	WORD		wItemLev;			/// 아이템 레벨
	
	WORD		wMinDamage;			// 최소 공격력
	WORD		wMaxDamage;			// 최대 공격력 
	
	WORD		wMinMana;			// 최소 마법력
	WORD		wMaxMana;			// 최대 마법력

	BYTE		btSlotNum;			// 슬롯 개수
	BYTE		btSlotProbab;		// 슬롯 확률

	int			nEquipClass;		// 착용 직업

	BYTE		btAttackRange;		// 공격 범위 
	WORD		wDurability;		// 내구력 
	BYTE		btAttackSpeed;		// 공격 속도 
	
	int			nItemAttr;			// 아이템 속성
	int			nItemDispos;		// 아이템 성향 
	WORD		wItemWeight;		// 아이템 무게
	
	WORD		wNeedStr;			// 필요힘 
	WORD		wNeedInt;			// 필요지혜
	WORD		wNeedDex;			// 필요민첩
	WORD		wNeedCha;			// 필요카리스마
	
	int			nPixPrice;			// 수리 가격 
	int			nSellPrice;			// 판매 가격 
	int			nPrice;				// 구입 가격 
	
	WORD		wPluseStr;			// 힘+
	WORD		wPluseInt;			// 지혜+
	WORD		wPluseDex;			// 민첩+
	WORD		wPluseCha;			// 카리스마+
	WORD		wPluseHP;			// 체력+
	WORD		wPluseMP;			// 마나+
	WORD		wPlusePhyDamage;	// 물리데미지+
	WORD		wPluseMagicDamage;	// 마법데미지+
	WORD		wPluseDefense;		// 방어력+
	WORD		wPluseMagicDefense;	// 마법방어력+
	WORD		wPluseSAttack;		// 근거리+
	WORD		wPluseLAttack;		// 원거리+
	WORD		wPluseBMagic;		// 흑마법+
	WORD		wPluseWMagic;		// 백마법 +
	
	WORD		wMinusStr;			// 힘-
	WORD		wMinusInt;			// 지혜-
	WORD		wMinusDex;			// 민첩-
	WORD		wMinusCha;			// 카리스마-
	WORD		wMinusHP;			// 체력-
	WORD		wMinusMP;			// 마나-
	WORD		wMinusPhyDamage;	// 물리데미지-
	WORD		wMinusMagicDamage;	// 마법데미지-
	WORD		wMinusDefense;		// 방어력-
	WORD		wMinusMagicDefense;	// 마법방어력-
	WORD		wMinusSAttack;		// 근거리-
	WORD		wMinusLAttack;		// 원거리-
	WORD		wMinusBMagic;		// 흑마법-
	WORD		wMinusWMagic;		// 백마법-
	
	char		szMimageFile1[64];	// 남자 이미지 파일1
	char		szMimageFile2[64];	// 남자 이미지 파일2
	char		szMimageFile3[64];	// 남자 이미지 파일3
	char		szMimageFile4[64];	// 남자 이미지 파일4
	char		szMimageFile5[64];	// 남자 이미지 파일5
	BYTE		btMHaveSkin;		// 남자스킨 여부
	
	char		szFimageFile1[64];	// 남자 이미지 파일1
	char		szFimageFile2[64];	// 남자 이미지 파일2
	char		szFimageFile3[64];	// 남자 이미지 파일3
	char		szFimageFile4[64];	// 남자 이미지 파일4
	char		szFimageFile5[64];	// 남자 이미지 파일5
	BYTE		btFHaveSkin;		// 남자스킨 여부

	int			nItemKind;			// 아이템 종류(파일에서 얻지 않는다)

	int			nMuriPlus;
	int			nManaPlus;

	SSkillInfoData*		pSkillData;
}; //struct SItemInfoData

/**
 * 아이템 정보 관리자.
 * @ingroup		Engine
 */
class IW3DItemInfoMgr : public IComponent
{
public:
	virtual BOOL			Init() = 0;

	virtual SItemInfoData*	GetItemInfo( int nIndex ) = 0;

	virtual void			SetSex( int nSex ) = 0;
	virtual int				GetSex() = 0;
	
	virtual SSkillInfoData* GetSkillInfo( int nIndex ) = 0;

	/**
	 * 아이템 팀 정보를 보여준다.
	 */
	virtual void			DrawTipInfo(
		int	anSkillLevel[],
		SItemInfoData* pItemInfoDat,			///< [IN] 출력 아이템 데이터.
		const int Height,
		const int nX,							
		const int nY, 
		IW3DFont* pFont,
		BOOL	  bShowInfo = TRUE,
		int		  nMode = 0,
		int		  nSellPrice = -1
	) = 0;
}; //class IW3DItemInfoMgr

#endif // __Wind3D_Engine_IW3DItemInfoMgr_Header__
