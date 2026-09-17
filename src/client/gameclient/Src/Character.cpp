/**
 * 캐릭터.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 417 $
 *
 * <HR>
 * $Log: /GameClient/Src/Character.cpp $
 * 
 * 417   05-02-18 7:38p Sk8snow2
 * 
 * 416   04-11-16 7:11p Sk8snow2
 * 
 * 415   04-09-24 4:45p Paran
 * 
 * 414   04-09-18 9:01a Sk8snow2
 * 맵로딩시 혹은 게임 종료시 튕기던 버그 수정 ( 정만 )
 * 
 * 413   04-09-17 12:10p Paran
 * 
 * 412   04-09-11 8:44p Sk8snow2
 * 
 * 411   04-09-11 6:20p Sk8snow2
 * 
 * 410   04-09-11 6:17p Sk8snow2
 * 
 * 409   04-09-11 1:47p Sk8snow2
 * 
 * 408   04-09-10 10:28p Sk8snow2
 * 
 * 407   04-09-08 8:37p Paran
 * 
 * 406   04-08-27 8:00p Paran
 * 
 * 377   04-03-15 5:43p Sckyj			운영자 작업 1차완료 - 동작막기 안함
 * 
 * 376   04-03-12 6:42p Sckyj			운영자 작업중
 * 
 * 375   04-03-12 2:12p Paran			Collision & Picking
 * 
 * 372   04-02-27 9:43p Kjmgo			펫추가 ^.^/
 * 
 * 369   04-02-03 6:35p Kjmgo			길드 이름 움하하!
 * 
 * 367   04-02-02 11:43a Kjmgo			바디 메니저 생성
 * 
 * 363   04-01-14 11:47p Kjmgo			공성전시 최소프레임유지에 필요한 코드 추가
 * 
 * 355   04-01-07 2:39p Kjmgo			메이 버그 수정
 * 
 * 353   04-01-06 2:10p Sckyj			BGM 재생 관련 코드수정
 * 
 * 352   04-01-06 12:01a Kjmgo			공속 추가 
 * 
 * 351   04-01-05 6:27p Sk8snow2		UseSkill 수정
 * 
 * 347   04-01-03 11:10a Kjmgo			SetVisible 추가
 * 
 * 346   04-01-03 8:54a Kjmgo			화살이 두번나가는거 수정. ( Flag으로 한번 AddMagic하면 그 다음부터 못하도록 수정 )
 * 
 * 345   04-01-02 7:26p Kjmgo			메인스킬이 마법일경우 특화스킬 사용 불가능 하도록 수정
 * 
 * 344   04-01-02 5:36p Paran			스킬 적용대상 수정
 * 
 * 343   04-01-02 7:23a Kjmgo			현재 캐릭터가 스킬 사용중이라면 방패를 그리지 않습니다. ^^
 * 
 * 342   03-12-31 3:18p Kjmgo			마법쪽 버그 수정 
 * 
 * 341   03-12-31 2:43p Paran			스킬
 * 
 * 340   03-12-31 10:23a Kjmgo			스킬 사용시 Target을 바라보도록 수정
 * 
 * 339   03-12-31 8:35a Kjmgo			토탈레벨이 10이하일때만 메이가 나타나도록 수정( 종전은 가장큰 레벨이 10일떄 )
 * 
 * 337   03-12-30 9:41p Paran			테스트
 * 
 * 336   03-12-29 10:19p Kjmgo			10이하일떄로 수정.ㅠ.ㅜ
 * 
 * 335   03-12-29 5:20p Kjmgo			레벨이 10이하일때만 메이가 나타나도록 수정
 * 
 * 333   03-12-26 2:23p Kjmgo			스킬 발동시엔, 검의 위치를 본을 따르도록 수정
 * 
 * 332   03-12-26 10:33a Kjmgo			선택유지중, 몬스터도 추가
 * 
 * 330   03-12-24 10:30a Kjmgo			스킬적용
 * 
 * 327   03-12-23 4:58p Kjmgo			string으로 인자를 받던것을 수정( LoadModel )
 * 
 * 326   03-12-23 11:13a Kjmgo			메이 뺐음
 * 
 * 325   03-12-22 5:00p Sckyj			2D, 3D sound 분리
 * 
 * 323   03-12-22 6:00a Kjmgo			버그 수정
 * 
 * 320   03-12-21 3:55a Sckyj			사용하지 않는 사운드 재생함수 제거2
 * 
 * 318   03-12-21 1:41a Sckyj			사용하지 않는 사운드 재생코드 삭제
 * 
 * 316   03-12-20 6:18a Kjmgo			메모리 관리
 * 
 * 315   03-12-19 10:47p Kjmgo			플레이어일떄만 May를 보이게 설정
 * 
 * 312   03-12-19 6:23a Kjmgo			석궁일때, 처리
 * 
 * 311   03-12-18 11:22a Kjmgo			임시 코드 주석 처리
 * 
 * 309   03-12-17 10:04p Kjmgo			Alt키를 누를시, 엠블럼 출력
 * 
 * 308   03-12-17 9:15p Kjmgo			성벽위나 다리위를 건널때, 엠블럼및, 채팅 메시지 출력안되던것. 수정
 * 
 * 307   03-12-17 5:10p Sckyj			Attack(), Damage() 함수에 pk 사운드 재생 코드 추가
 * 
 * 306   03-12-17 4:11p Kjmgo			엠블럼 평소에도 보이게 하도록 수정
 * 
 * 305   03-12-16 5:05p Paran			아이템 데이터 수정
 * 
 * 304   03-12-16 2:03a Kjmgo			캐릭터를 해제 할때, 이펙트를 지운다. ( Characterを 解除する時, Effectをしゃくじょ )
 * 
 * 303   03-12-16 1:25a Sckyj			물위에서 물발자국 소리 나도록 추가
 * 
 * 302   03-12-15 10:38a Kjmgo			엠블런추가(  'Emblem' 追加(ついか) )
 * 
 * 301   03-12-14 6:38p Kjmgo			길드 이름 그리기 추가
 * 
 * 300   03-12-14 4:17a Kjmgo			화살 버그 수정
 * 
 * 298   03-12-13 2:43p Kjmgo			이동시 LAYER값 이상있던 버그 수정
 * 
 * 294   03-12-12 6:07p Paran			정만군이 무단 체크인
 * 
 * 293   03-12-11 3:00a Kjmgo			길찾기를 하지 않았을떄, 갈수 없는 곳에 정착하면 정지로 돌림.
 * 
 * 291   03-12-10 10:56p Kjmgo			길찾기, 피킹 1차 완료
 * 
 * 290   03-12-10 8:32p Kjmgo			PickingEffect를 Character로 옮김
 * 
 * 289   03-12-10 2:00p Kjmgo			길찾기및, 피킹 수정
 * 
 * 288   03-12-09 4:01p Paran			아이템 데이터 수정
 * 
 * 287   03-12-06 2:35p Kjmgo			평화모드인지 전투 모드인지 판단하는 함수 추가
 * 
 * 286   03-12-06 12:01a Kjmgo			이동시 길찾기 복구
 * 
 * 284   03-12-04 7:16p Kjmgo			주인공 캐릭터 공속 수정 가능하도록 하는 코드 삽입
 * 
 * 283   03-12-04 10:44a Kjmgo			플레이어의 May의 전역포인터 추가
 * 
 * 282   03-11-18 9:52p Kjmgo			GetHeight 관련 함수 수정및, 추가
 * 
 * 281   03-11-18 8:05p Kjmgo			마법쓸때도, 공격동작 나오던것을 수정
 * 
 * 280   03-11-18 7:27p Kjmgo			Projection Shadow 를 Shadow Dummy를 안쓰고, 실제 Model로 그리도록 수정
 * 
 * 279   03-11-17 6:20p Kjmgo			말탔을때의, 그림자 추가
 * 
 * 276   03-11-16 1:30a Kjmgo			May 관련 코드 추가
 * 
 * 275   03-11-15 9:41p Paran			PK 부분 1차 수정
 * 
 * 273   03-11-12 8:36p Kjmgo			메이 생성 시를 조금 앞당겼음
 * 
 * 272   03-11-12 2:36p Kjmgo			주인공 캐릭터 일때만 프로젝션 새도우 출력하도록 수정( 임시 )
 * 
 * 270   03-10-27 3:25p Kjmgo			이동완료를 그전에, 같았을때로 하던것을...0.1미터의 오차는 무시하도록 수정 And 코드 최적화
 * 
 * 269   03-10-25 1:54p Kjmgo			마법이 끝난후, 현재 위치의 Z 값을 갱신
 * 
 * 268   03-10-23 10:04p Paran			공격효과부분 수정.
 * 
 * 263   03-10-17 1:59p Kjmgo			라이트 이펙트 작업
 * 
 * 262   03-10-16 4:41p Sckyj			sound재생관련 코드 수정
 * 
 * 261   03-10-15 9:25p Paran			타격시 AddMagic으로 교체(일반이펙트시는 마지막 변수 FALSE설정)
 * 
 * 256   03-10-13 2:51p Kjmgo			Character의 Bone Animation 연산을 단 한번만 하도록 수정
 * 
 * 255   03-10-11 12:08a Kjmgo			AddMagic수정 및, CCreature::~CCreature(); 쓰던거 삭제
 * 
 * 254   03-10-10 7:25p Kjmgo			마법 관련해서 수정
 * 
 * 253   03-10-10 11:27a Kjmgo			인자로 넘어오는 포인터 검사 루틴 추가
 * 
 * 252   03-10-09 11:57a Kjmgo			DrawShortWeapon 에서, 여자일경우 위치를 변경시키던것을 안시키는 것으로 수정
 *
 * 245   03-10-07 8:08p Kjmgo			May그리기 순서 수정 + 캐릭터 말 탔을떄의 채팅 그리기 높이 수정
 * 
 * 244   03-10-07 9:28a Kjmgo			Weapon Animation 처리
 * 
 * 243   03-10-06 9:38p Kjmgo			May(땡삐) 1단계 완성
 * 
 * 241   03-10-06 2:54p Kjmgo			활 스킬에서 검을 바꾸면, 맨손 동작 나오던것 수정
 * 
 * 240   03-10-04 4:51p Kjmgo			Update에서 SetAction때리지 않아 억세스 뜨던것 수정
 * 
 * 238   03-10-04 2:02a Kjmgo			동작 버그 잡는중 : 40%
 * 
 * 237   03-10-04 1:18a Kjmgo			캐릭터 공격 부분 수정 및, 근거리 스킬이 아닐경우, 근거리 아이템을
										바꾸면 무기 동작이 안바뀌던것 수정( 원거리가 아닐경우 바꾸기로.)
 * 
 * 236   03-10-03 11:43p Kjmgo			이동중일떄만 스킬이 바뀌던것을 정지상태에서도 바뀌도록 수정
 * 
 * 233   03-10-03 9:56p Kjmgo			양손에 무언가를 들고 있을때, 방패를 해제 했을경우 맨손으로 돌아간경우 수정
 * 
 * 229   03-10-02 11:20p Kjmgo			캐릭터 State 권한 이전 : 완료 
 * 
 * 228   03-10-02 10:33p Kjmgo			캐릭터 State 권한 이전 작업 : 1차 백업
 * 
 * 227   03-10-02 5:50p Kjmgo			EndMagic이 오지 않았을때, 추가적으로 마법이 발동되던것을 막았음
 * 
 * 226   03-10-02 3:54p Kjmgo			Ride관련 수정 및, 캐릭터 삭제시, m_pPlayer 를 NULL하는 작업
 * 
 * 225   03-10-02 3:24p Kjmgo			캐릭터 타격 이펙트 설정
 * 
 * 224   03-10-02 2:41p Kjmgo			몬스터 공격 거리를 몬스터의 Radius만큼의 여유를 줌
 * 
 * 221   03-10-02 4:34a Kjmgo			AABB충돌 추가( 아직 미완성 )
 * 
 * 220   03-10-01 11:08p Kjmgo			크리처들과의 충돌처리 진행
 * 
 * 219   03-10-01 8:40p Kjmgo			말탔을때 말은 보이지 않고 타는 동작만 나오던 버그 수정
 * 
 * 218   03-10-01 6:40p Kjmgo			Skill적용
 * 
 * 217   03-10-01 2:14p Kjmgo			마법이 끝나고 정지하던것을 수정
 * 
 * 216   03-10-01 11:38a Kjmgo			최상위 Creature에 ShowDamageForTarget 함수 추가 및, 하위 클래스 함수추가
 * 
 * 215   03-10-01 2:36a Kjmgo			이름과 채팅 내용을 DrawShadow로 이동
 * 
 * 214   03-09-30 6:07p Kjmgo			이동중 말을 탔을때, 정지 할때까지 말이 보이지 않던 버그 해결
 * 
 * 213   03-09-29 5:05p Kjmgo			Picking용 Interface Pointer를 CGame 내부에서 전역으로 수정
 * 
 * 212   03-09-24 12:44a Kjmgo			아이템 투명화
 * 
 * 211   03-09-23 9:54p Kjmgo			State중, Skill추가
 * 
 * 209   03-09-23 11:20a Kjmgo			Resurrect함수 추가
 * 
 * 205   03-09-22 2:01p Kjmgo			Creature 색상 설정및 알파값 설정 작업(완료)
 * 
 * 204   03-09-18 5:15p Kjmgo			Magic에서 Bone을 사용할수 있게 수정
 * 
 * 203   03-09-18 10:58a Kjmgo			데미지 처리 관련 수정
 * 
 * 202   03-09-17 11:00a Kjmgo			마법 시전 부분 약간 수정( 동작이 중복되서 나가는거 수정 )
 * 
 * 198   03-09-13 1:26p Kjmgo			활을 비롯한 몇가지 옵션 추가
 * 
 * 197   03-09-12 6:42p Kjmgo			Vehicle 관련 수정 및 추가
 * 
 * 190   03-09-08 8:20p Kjmgo			동작~ 작업
 * 
 * 187   03-09-07 6:03p Kjmgo			동작 수정 및, Model Setting할수 있도록 수정
 * 
 * 186   03-09-05 2:14a Kjmgo			방패 착용 
 * 
 * 185   03-08-31 10:19p Kjmgo			Chracter 이동과 공격 수정 
 * 
 * 184   03-08-31 6:18p Kjmgo			Character Action 관련 추가
 * 
 * 183   03-08-30 11:07a Kjmgo			Character Animation Data 읽는 부분 추가
 * 
 * 181   03-08-29 9:19a Kjmgo			Animation 바꿈
 * 
 * 164   03-08-14 10:03a Kjmgo			마법스크립트 두개로 분리
 * 
 * 159   03-08-10 6:59p Admin			말 타고내리기 적용
 * 
 * 128   03-06-27 5:30p Kjmgo			Assert처리
 * 
 * 121   03-06-26 12:41a Paran			사운드 추가.
 * 
 * 119   03-06-23 5:53p Paran			바운드 박스 처리.
 * 
 * 111   03-06-15 5:55p Kjmgo			없는 마법을 Add했을때 억세스 나오는걸 수정
 * 
 * 110   03-06-15 4:44p Paran			STATE_MAGIC 추가.
 * 
 * 109   03-06-14 11:03a Paran			Skill zzang~
 * 
 * 107   03-06-12 4:54p Paran			누드 아이디 얻는 부분 수정
 * 
 * 106   03-06-12 2:05p Paran			매직 관련작업
 * 
 * 105   03-06-08 5:49p Kjmgo			이동시 Trace Effect 효과 끔
 * 
 * 102   03-06-06 9:58a Kjmgo			Shadow Dummy 추가
 * 
 * 101   03-06-05 8:20p Paran			동작분리중...
 * 
 * 100   03-06-05 3:17p Paran			동작 에니 작업
 * 
 * 99    03-06-03 11:38p Paran			조력자 맴버 추가
 * 
 * 97    03-05-28 1:31p Paran			스킬 안맞는 부분, 공격패킷 수정
 * 
 * 95    03-05-27 8:41p Paran			활공격, 폴더 정리
 * 
 * 94    03-05-26 10:52p Paran			여자무기 장착위치 보정.
 *
 * 93    03-05-26 9:06p Paran			사용스킬 별로 모델 사용.
 *
 * 89    03-05-16 11:03a Paran			이름 출력 관련 수정.
 *
 * 88    03-05-15 9:04p Paran			파티 관련 이름 출력.
 *
 * 87    03-05-14 11:56p Kjmgo			Action을 Draw할 때마다 갱신.
 *
 * 85    03-05-14 2:07a Kjmgo			이동할 때 검흔 출력 안 하게 수정.
 *
 * 77    03-05-12 7:05p Kjmgo			마법 싱크 맞춤.
 *
 * 70    03-05-10 7:42p Ahastudio		클래스 내부로 모든 함수를 이동시킴.
 *
 * 69    03-05-10 4:45p Ahastudio		공격 완료 패킷 보내기 추가.
 *
 * 67    03-05-10 4:12p Ahastudio		길찾기 관련 함수의 Interface 수정에 맞춤.
 *
 * 66    03-05-10 12:54p Ahastudio		Engine에서 Creature Implement 관련 클래스를 Game으로 이동함.
 *
 * 65    03-05-10 12:32p Ahastudio		SetLighting() 추가.
 *
 * 63    03-05-08 1:28a Paran			말풍선 작업.
 *
 * 61    03-04-30 7:11p Paran			스킬 선택 수정.
 *
 * 53    03-04-23 3:58p Ahastudio		캐릭터 애니메이션 수정.
 *
 * 49    03-04-23 7:50a Ahastudio		동작들을 모두 virtual로 변경. 새로운 Attack 추가.
 *
 * 48    03-04-19 6:51p Kjmgo			그림자 그릴 때 Model의 World를 다시 세팅하도록 수정.
 *
 * 47    03-04-18 3:09p Ahastudio		SetState를 virtual 함수로 변경.
 *
 * 45    03-04-17 5:27p Ahastudio		길찾기 추가.
 *
 * 41    03-04-13 8:53p Kjmgo			실시간 그림자 추가.
 *
 * 40    03-04-12 3:02p Paran			캐릭터 장착 아이템 변경 기능 추가.
 *
 * 39    03-04-11 5:28p Paran			캐릭터 장착 부분 수정.
 *
 * 34    03-04-08 11:25a Paran			캐릭터 성별 구분 수정.
 *
 * 30    03-03-26 5:38p Ahastudio		캐릭터 모델 이름 리스트 작성 수정.
 *
 * 29    03-03-26 7:11a Kjmgo			움직일 때도 검흔이 남게 했습니다.
 *
 * 24    03-03-18 12:02p Ahastudio		모델 및 특수효과 수정.
 *
 * 11    03-03-13 5:33p Ahastudio		모델 애니메이션 부분 삭제.
 *
 * 8     03-02-24 3:04p Kjmgo			Trace Effect를 사용시 Key를 기존의 문자에서 숫자로 바꾸었음.
 * <HR>
 *
 * @file	Character.cpp
 */


#include "Global.h"
#include "Creature.h"
#include "Pet.h"
#include "May.h"
#include "Game.h"
#include "Emblem.h"
#include "BodyMgr.h"
#include "Pet.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <map>


using std::string;
using std::vector;
using std::list;
using std::map;

#define LENGTH_SHOPTITLE		32
#define TEXT_WIDTHLIMIT			200
#define TICK_SHOWTEXT			3000

#define IS_CHAR_SIEGE( n )		( ( g_nAttackGuild == ( n ) ) && ( ( n ) > 0 ) ) ? TRUE : FALSE 
#define IS_CHAR_DEFENSE( n )	( ( g_nDefenseGuild == ( n ) ) && ( ( n ) > 0 ) ) ? TRUE : FALSE

typedef map<string, IW3DModel*>	ModelMap;

static ModelMap		gs_mapCharacterModel;		///< 캐릭터 모델
static DWORD		g_dwCharacterCount = 0;		///< 현재 클라이언트에 등록된 캐릭터 수
static DWORD		g_dwViewChatacter = 0;		///< 현재 보일 캐릭터 수
static IW3DShadow*	g_pHeroShadow = NULL;
static IW3DShadow*	g_pCharacterShadow = NULL;
CBodyManager		g_BodyMgr;

const int	FOOT_PART	= 0;
const int	HAND_PART	= 1;
const int	LOWER_PART	= 2;
const int	UPPER_PART	= 3;
const int	FACE_PART	= 4;
const int	HAIR_PART	= 5;
const int	MAX_PARTS	= 6;


const int	SHORTWEAPON_ITEMPART = 0;
const int	LONGWEAPON_ITEMPART	 = 1;
const int	HELMET_ITEMPART		 = 2;
const int	SHIELD_ITEMPART		 = 3;
const int	MAX_ITEMPARTS		 = 4;

const int	SWORD_ONEHANDSWORD	= 1;
const int	SWORD_TWOHANDSWORD	= 2;
const int	SWORD_DOUBLESWORD	= 3;
const int	SWORD_TWOHANDSTAFF	= 4;

const int	ACTION_STOP			= 1;
const int	ACTION_MOVE			= 2;
const int	ACTION_ATTACK		= 3;
const int	ACTION_ATTACKMAGIC	= 4;
const int	ACTION_CASTMAGIC	= 5;
const int	ACTION_DEAD			= 6;

BOOL	g_Checkhorse =  FALSE;

/**
 * 캐릭터 클래스.
 * @ingroup		Game
 */
class CW3DCharacter : public CCreature
{
public:
	UNDER_MEMORY_MGR( CW3DCharacter );

	/**
	 * 생성자.
	 */
	CW3DCharacter();

	/**
	 * 소멸자.
	 */
	~CW3DCharacter();

	BOOL		Create(
		IW3DWorld*		pWorld,		///< [IN] 월드 객체
		const DWORD		dwId,		///< [IN] 크리쳐 ID
		const void*		pData		///< [IN] 크리쳐 생성 정보
		);

	/**
	 * 캐릭터 모델을 업데이트 한다.
	 *
	 * @param	wItemType	[IN] 아이템 종류
	 * @param	bOn			[IN] 장착/해제
	 *
	 * @return	업데이트 성공 여부
	 */
	BOOL		UpdateModel( const WORD wItemType, const BOOL bOn );

	/**
	 * 몸 부분별 로드.
	 */
	BOOL		LoadBodyPart( const int nPart, const char* szFileName, BOOL bSkin );

	/**
	 * 하나짜리 캐릭모델 로드. 
	 */
	BOOL		LoadOneBody( SW3DCharacterInfo* pInfoCharacter );

	/**
	 * 장착 장비 부분별 로드.
	 */
	BOOL		LoadEquipPart( const int nPart, const char* szFileName );

	/**
	 * 바운딩 박스 로딩.
	 */
	BOOL		LoadBoundBox( const char* szPath, const char* szBoundBox );

	void		UnloadBoundBox( IW3DModel*	pModel, const char* szBoundBox );

	/**
	 * 벗은 몸의 아이템번호를 얻는다.
	 *
	 * @param		[IN]nPart		몸 부분.
	 * @return		아이템 번호.
	 */
	int			GetNakedBody( const WORD wPart, const UINT nSex );

	/**
	 * 장착부분을 모델부분으로..
	 *
	 * @param		[IN]  wPart			장착위치
	 * @param		[OUT] wModelPart	모델 인덱스
	 *
	 * @return		모델 분류 ( 1 - Body, 2 - Equip )
	 */
	int			GetModelPart( const WORD wPart, WORD& wModelPart );

	/**
	 * 인종 설정.
	 *
	 * @param		[IN] 캐릭터 정보.
	 */
	void		SetRace( SW3DCharacterInfo& infoCharacter );

	/**
	 * 공격모드 설정.
	 *
	 * @param	sAction		[IN] 사용할 동작 이름
	 */
	void		SetAttackMode( const int nSkill	);

	/**
	 * 스킬 적용.
	 */
	void		ApplySkill();

	/**
	 * * 스킬사용 방법이 맞는지 체크하고 사용한다.
	 */
	BOOL		CheckUsageSkill( IW3DCreature* pTarget );

	/**
	 * 자신에게 사용하는 스킬사용.
	 */
	void		SelfSkill( const BYTE btSkillType );

	/**
	 * 상대에게 사용하는 스킬사용.
	 */
	void		TargetSkill( const DWORD dwTargetId, const BYTE btSkillType, BOOL bIsMonster );

	/**
	 * 매직 정보 설정.
	 */
	void		SetMagicInfo( const BYTE btMagicType, const BYTE btMagicPickType );

	/**
	 * 사용중인 스킬적용정보.
	 */
	BOOL		GetApplyInfoSkill( 
		BYTE* btSkillType,			///< [OUT] 0 - 일반공격용, 1 - 마법 기술
		BYTE* btTaget				///< [OUT] 스킬사용가능 상대 ( 0 - 상대, 1 - 자신, 2 - 모두 )
		);

	/**
	 * 사용중인 무기 설정.
	 */
	void		SetWeapon( SItemInfoData* pItemInfo );

	/**
	 * 활살 속성 설정.
	 */
	void		SetArrowInfo( const int nBowIndex );

	/**
	* 데미지 효과.
	*/
	void		UpdateDamageEffect( DWORD dwTick );

	/**
	 * 캐릭터 업데이트.
	 */
	BOOL		Update( const DWORD dwTick );

	/**
	 * 캐릭터를 출력한다.
	 */
	void		Draw();

	/**
	 * 캐릭터의 그림자를 출력한다.
	 */
	void		DrawShadow();

	/**
	 * 이름 찍기.
	 */
	void		DrawBaseInfo();

	/**
	 * 개인상점 타이틀 출력
	 */
	void		DrawShopTitle();

	/**
	 * 쳇 내용 출력.
	 */
	void		DrawChattText();

	/**
	 * 크리쳐 종류를 얻는다.
	 */
	int			GetType();

	/**
	 * 성별을 얻는다.
	 */
	UINT		GetSex();

	/**
	 * 말풍선에 들어갈 문자열을 설정한다.
	 */
	void		SetTalkText( char* szText );

	/**
	 * 길찾기를 통해 목표점까지 이동한다.
	 */
	BOOL		MoveTo( const Vector3& vPos );

	/**
	 * 길찾기
	 */
	void		SetAutoPath();

	/**
	 * 캐릭터의 이동을 멈춘다.
	 */
	void		Stop();

	/**
	 * 캐릭터가 이동을 한다.
	 */
	void		Move();

	/**
	 * 캐릭터가 죽는다.
	 */
	void		Dead();

	/**
	 * 캐릭터가 죽었는지 확인.
	 */
	BOOL		IsDead();

	/**
	 * 다른 크리쳐를 공격한다.
	 */
	void		Attack(
		IW3DCreature*	pTarget		///< [IN] 공격 대상
		);	

	/**
	 * 일반마법 사용한다.
	 */
	void		CastMagic();

	/** 
	 * 마법추가.
	 */
	void		AddMagic( const WORD wMagic, const WORD wID, IW3DCreature* pTarget, BOOL bCompulsion = FALSE );

	/**
	 * 공격을 받는다.
	 */
	void		Damage( BOOL bBegin, IW3DCreature* pAttacker );	

	/*
	 * 공격받은 수치설정.
	 */
	void		Damage( const WORD wDamage, const BYTE btCritical );

	/**
	 *
	 */
	SW3DDamage* GetDamage();

	/**
	 * 데미지 효과.
	 */
	void		DamageEffect( IW3DCreature* pAttacker );
	
	/**
	 * HP 설정.
	 */
	void		SetHp( const int nHp, const int nTotHP );

	/**
	 * 캐릭터 상태를 설정한다.
	 */
	void		SetState( const int nState );
	/**
	 * 캐릭터 몸 모델을 로드한다.
	 */
	BOOL		LoadBody( SW3DCharacterInfo* pInfoCharacter );

	/**
	 * 캐릭터 부속 모델을 로드한다.
	 */
	BOOL		LoadAccessory( SW3DCharacterInfo* pInfoCharacter );

	/**
	 * 캐릭터 모델을 읽는다.
	
	IW3DModel*	LoadModel( 
		const string&	sPath,		///< [IN]
		const string&	sName,		///< [IN]
		const string&	sType,		///< [IN]
		const BOOL		bAni		///< [IN]
		); */

	IW3DModel*	LoadModel( 
		const char*		szPath,		///< [IN]
		const string&	sName,		///< [IN]
		const char*		szType		///< [IN]
		);

	/**
	 * 캐릭터 모델을 제거한다.
	 */
	void		UnloadModel(
		IW3DModel**		ppModel,	///< [IN/OUT]
		const string&	sName		///< [IN]
		);

	/**
	 * 근거리 무기를 출력한다.
	 */
	void		DrawShortWeapon();

	/**
	 * 원거리 무기를 출력한다.
	 */
	void		DrawLongWeapon();

	/**
	 * 방패를 출력한다.
	 */
	void		DrawShield();

	/**
	 * 애니메이션을 설정한다.
	 *
	 * @param	sAction		[IN] 사용할 동작 이름
	 */
	void		SetAction( const WORD wAction );

	/**
	 * 현재 상태의 동작이 마지막인지 확인한다.
	 *
	 * @param	dwTick		[IN] 경과 tick
	 */
	BOOL		IsEndAction( const DWORD dwTick );

	/**
	 * 캐릭터 이동.
	 */
	BOOL		StateMovement( const DWORD dwTick );

	/**
	 * 죽은 상태.
	 */
	BOOL		StateDead( const DWORD dwTick );

	/**
	 *
	 */
	BOOL		StateSkill( const DWORD dwTick );

	/**
	 * 공격.
	 *
	 * @param	dwTick		[IN] 경과 tick
	 * @param	nIndex		[IN] 공격형태
	 */
	BOOL		StateAttack( const DWORD dwTick, const int nIndex );

	/**
	 * 마법사용중...
	 */
	BOOL		StateMagic( const DWORD dwTick );

	/** 
	 * 마법 사용.
	 */
	void		StartMagic( DWORD dwID, DWORD dwMagicId, BOOL bTargetMonster, BOOL bHero );

	/**
	 * 마법이 끝났다.
	 */
	void		EndMagic( DWORD dwID );

	/**
	 * 용병 설정.
	 */
	void		SetHelper( const IW3DCreature* pCreature );

	/**
	 * 용병 제거.
	 */
	void		RemoveHelper();

	/** 
	 *
	 */
	void AddVehicle( const void* pData )
	{
		/*
			TODO : 말 탔다는 상태를 추가 해야 함
		*/
		CCreature::AddVehicle( pData );
		Assert( m_pVehicle );
		
		if( m_pVehicle )
		{
			m_pActionMgr->SetVehicleActionMgr( m_pVehicle->GetActionMgr() );
			//m_pActionMgr->SetWeapon( 
			m_pActionMgr->SetMode( MODE_VEHICLE + m_pVehicle->GetType() );

			m_pActionMgr->Resurrect();
		} //if


		if( m_pHelper )
		{	
			m_pHelper->SetHeight( MAY_HEIGHT + 0.7F );
			m_pHelper->SetSpeed( m_fOffset );
		} //if
		
	} //AddVehicle

	/**
	 *
	 */
	void RemoveVehicle()
	{
		/*
			TODO : 말에서 내렸다는 상태를 추가 해야 함
		*/
		CCreature::RemoveVehicle();

		m_pActionMgr->SetVehicleActionMgr( NULL );
		m_pActionMgr->SetMode( MODE_PEACE );

		if( m_pHelper )
		{	
			m_pHelper->SetHeight( MAY_HEIGHT );
			m_pHelper->SetSpeed( m_fOffset );
		} //if
		g_Checkhorse = FALSE;
	} //RemoveVehicle

	/**
	 *
	 */
	BOOL CastingMagic( DWORD dwID, DWORD dwMagicId );

	/**
	 *
	 */
	void AddAttacker( IW3DCreature* pAttacker );

	/**
	 *
	 */
	void RemoveAttacker( IW3DCreature* pAttacker );

	/**
	 *
	 */
	void AddTarget( IW3DCreature* pTarget );

	/**
	 *
	 */
	void RemoveTarget( IW3DCreature* pTarget );

	/**
	 *
	 */
	void ClearAttackers();

	/**
	 *
	 */
	void ClearTargets();

	/**
	 *
	 */
	IW3DModel* GetModel( int nPart )
	{
		if( m_apModel[nPart] )
		{
			m_apModel[nPart]->SetAction( m_pAction );
			return m_apModel[nPart];
		} //if

		return NULL;
	} //GetModel
	
	/**
	 *
	 */
	void Resurrect()
	{
		m_pActionMgr->Resurrect();
		m_nAdvanceState = MODE_STOP;
		m_bStateLock = FALSE;
		Stop();
		CCreature::SetState( MODE_STOP );
	} //Resurrect

	/**
	 *
	 */
	void ShowDamageForTarget()
	{
		if( m_pActionMgr )
		{
			m_pActionMgr->ShowDamageOfTarget();
			m_pActionMgr->ClearTargets();
		} //if
	} //ShowDamageForTarget

	/**
	 * 
	 */
	void UseSkill( WORD wSkillId );

	/**
	 *
	 */
	void AddSkill( IW3DCreature* pTarget, const WORD wSkillId );

	/**
	 *
	 */
	void SetActionSpeed( DWORD dwTicks )
	{
		if( m_pActionMgr )
		{
			m_pActionMgr->SetAttackSpeed( dwTicks );
		} //if
	} //SetActionSpeed

	/**
	 * 개인상점 정보 설정
	 */
	void SetShopInfo( const char* szTitle )
	{
		if( strlen( szTitle ) > LENGTH_SHOPTITLE - 1 )
		{
			Assert( !"Over Length The ShopTitle" );
		} //if

		strncpy( m_szShopTitle, szTitle, LENGTH_SHOPTITLE - 1 );
	} //SetShopInfo

	/**
	 * 개인상점 정보 얻기
	 */
	char* GetShopInfo()
	{
		if( m_szShopTitle[0] == '\0' )
		{
			return NULL;
		} //if
		
		return m_szShopTitle;
	} //GetShopInfo

	/**
	 * 개인상점 개점 학인
	 */
	BOOL IsOpenShop()
	{
		if( m_szShopTitle[0] == '\0' )
		{
			return FALSE;
		} //if
		return TRUE;
	} //IsOpenShop

	/**
	 * 길드 정보 설정
	 */
	void SetGuildData( const char* szName, int nGuildId, BYTE btGuildLev, int nMark )			///< 길드 등급 0 = 등급 없음 , 1 = 단원 , 2 = 부단장 , 3 = 단장
	{
		m_nGuildId = nGuildId;
		if( btGuildLev == 2 )
		{ //부단장
			sprintf( m_szCorpsName, "%s %s", szName, STR_UIMSG_GUILDBUZZANG );
		}
		else if( btGuildLev == 3 )
		{ //단장
			sprintf( m_szCorpsName, "%s %s", szName, STR_UIMSG_GUILDZZANG );
		}
		else
		{
			if( szName[0] == ' ' && strlen( szName ) < 2 )	m_szCorpsName[0] = '\0';
			else											strcpy( m_szCorpsName, szName );
		} //if..else

		if( m_pEmblem )
		{
//			m_pEmblem->nId = nMark;
//			m_pEmblem->Load();
		}

		ResetEnemy();
	} //SetGuildData

	void ResetEnemy()
	{
		for( int nCnt = 0; nCnt < 6; nCnt++ )
		{
			if( !g_anGuildId[nCnt] )
			{ //no relation
				m_dwGuildTextColor = 0xFF00FFFF;
			}
			else if( g_anGuildId[nCnt] == m_nGuildId )
			{ //Enemy
				m_dwGuildTextColor = 0xFFC00000;
			}
			else
			{ //with
				m_dwGuildTextColor = 0xFF0000C0;
			} //if..else if..else
		} // for
	} //ResetEnemy
	
	// Insert Pet
	void InsertPet( const BYTE btType, void* pData );
	void RemovePet();
	void UpdatePet( const char* szName, void* pData );

	BOOL Pick( const Vector3& vOrig, const Vector3& vDir );

	void Hook();
private:
	BYTE				m_btCountry;					///< 국가( 0 - "모로스", 1 - "네메시스"  3 - "" )
	char				m_cRace;						///< 인종 ( '0' - 백인, '1' - 황인, '2' - 흑인 )
	int					m_nGuildId;						///< 
//	BOOL				m_bSex;							///< 성별 ( 0 - 남, 1 - 여 )
	UINT				m_nSex;							///< 성별 ( 0 - 남, 1 - 여, 2 - 운영자 )
	BOOL				m_bPlayer;						///< 플레이어인가?
	BOOL				m_bDamage;						///< 맞고 있는 중.
	BOOL				m_wDamege;						///< 데미지 수치.
	BYTE				m_btCritical;					///< 크리티컬 데미지.

	SW3DDamage			m_Damage;						///< 데미지 정보
	BOOL				m_bDamageEffect;
	DWORD				m_dwDamageEffectTick;


	BOOL				m_bAttack;						///< 공격중일때, 추가 Flag

	char				m_szShopTitle[LENGTH_SHOPTITLE];	///< 32자 상점이름
	char				m_szCorpsName[32];				///< 16자 길드 이름
	string				m_asModel[MAX_PARTS];			///< 각 부위 이름
	string				m_asItemModel[MAX_ITEMPARTS];	///< 각 아이템 이름
	DWORD				m_dwGuildTextColor;				///< The Color of the Guild Text

	int					m_nChangedSkill;				///< 바뀐스킬

	IW3DModel*			m_pBoundBox;
	IW3DModel*			m_apModel[MAX_PARTS];			///< 각 부위 모델
	IW3DModel*			m_apItemModel[MAX_ITEMPARTS];	///< 각 아이템 모델

	IW3DShadow*			m_pShadow;						///< 그림자

	BOOL				m_bTrace;						///< Trace Effect를 남길지의 여부
	BOOL				m_bIsEndAction;					///< 액션이 끝났는지의 여부

	BYTE				m_btMagicType;					///< 사용할 마법타입 ( 공격형이던지 아니던지 )
	BYTE				m_btMagicPickType;				///< 사용할 마법 적용대상 ( 너, 나, 너랑나랑 )

	SCreatureTarget		m_sTarget;						///< 작업중인 대상

	DWORD				m_dwTalkTime;					///< 대화 문자출력 시간
	string				m_sTalk;						///< 대화 문자열
	int					m_nTalkTxtWidth;				///< 대화 문자열 가로
	int					m_nTalkTxtHeight;				///< 대화 문자열 세로

	DWORD				m_dwMagicTick;					///<> 

	vector<Vector3>		m_aPath;						///< 캐릭터 이동 경로

//	IW3DCreature*		m_pHelper;						///< 조력자.
	IW3DCreature*		m_pAttacker;					///< 공격자.

	BYTE				m_btSwordType;					///< 한손 검, 두손 검, 양손 단검, 지팡이, 맨손
	BYTE				m_btWeaponType;					///< 무기종류
	string				m_sArrow;						///< 화살 (매직)

	STrace*				m_pTrace;						///< 
	SAction*			m_pAction;						///< 액션
	CCharacterAction*   m_pActionMgr;					///< 액션 메니저

	int					m_nShortWeapon;					///< 근거리 무기
	int					m_nLongWeapon;					///< 원거리 무기

	BOOL				m_bStateLock;					///<

	CSkill*				m_pSkill;						///< 스킬 포인터
	IW3DLight*			m_pLightEffect;

	BOOL				m_bAutoPath;					///< 길찾기를 한것인가?

	SEmblem*			m_pEmblem;

	BOOL				m_bMasterPlayer;				///< 운영자 캐릭터인가?
	BOOL				m_bNotUseSkill;

	

	//--=-=-==-=-=-=-=-== Creature List -=-=-=--=-=-==-=-==//
	typedef std::map<DWORD, IW3DCreature*> CreatureMap;
	CreatureMap			m_mapAttackers;
	CreatureMap			m_mapTargets;

	CHelper*			m_pHelper;

	DWORD				m_dwLightTime;


	/**
	 *
	 */
	void SendPacket_BeginMagic();

	/**
	 *
	 */
	BOOL CollisionCreature( Vector3 vPos );

	/**
	 *
	 */
	IW3DCreature*		GetTargetCreature();

	/**
	 *
	 */
	void Update_Light();

	/**
	 *
	 */
	void AddPickedEffect();

	/**
	 *
	 */
	void RemovePickedEffect();

}; //class CW3DCharacter


IMPLEMENT_COMPONENT( CW3DCharacter, Character );


/**
 * 생성자.
 */
CW3DCharacter::CW3DCharacter()
:	m_pBoundBox		( NULL ),
	m_cRace			( '0' ),
	m_pShadow		( NULL ),
	m_pTrace		( NULL ),
	m_pAction		( NULL ),
	m_pAttacker		( NULL ),
	m_pActionMgr	( NULL ),
	m_pSkill		( NULL ),
	m_pLightEffect	( NULL ),
	m_pEmblem		( NULL ),
	m_bNotUseSkill	( FALSE ),
	m_bAttack		( FALSE ),
	m_bPlayer		( FALSE ),
	m_bTrace		( FALSE ),
	m_bIsEndAction	( FALSE ),
	m_bAutoPath		( FALSE ),
	m_dwMagicTick	( 0 ),
	m_dwLightTime	( 0 ),
	m_sTalk			( "" ),
	m_dwTalkTime	( TICK_SHOWTEXT ),
	m_nShortWeapon	( 0 ),
	m_pHelper		( NULL ),
	m_btWeaponType	( WEAPON_NONE ),
	m_nChangedSkill	( -1 ),
	m_bDamage		( FALSE ),
	m_bStateLock	( FALSE ),
	m_wDamege		( 0 ),
	m_bMasterPlayer ( FALSE ),
	m_dwGuildTextColor( 0xFF00FFFF )
{
	m_sTarget.Clean();

	for( int i = 0 ; i < MAX_PARTS ; i++ )
	{
		m_asModel[i] = "";
		m_apModel[i] = NULL;
	} //for

	for( int j = 0 ; j < MAX_ITEMPARTS ; j++ )
	{
		m_asItemModel[j] = "";
		m_apItemModel[j] = NULL;
	} //for

	m_Damage.btCritical	= 0;
	m_Damage.pCreature	= this;
	m_Damage.wDamage	= 0;

	m_MagicData.bCast	= TRUE;

	m_szCorpsName[0]	= '\0';
	m_szShopTitle[0]	= '\0';
	g_dwCharacterCount++;

} //CW3DCharacter::CW3DCharacter

/**
 * 소멸자.
 */
CW3DCharacter::~CW3DCharacter()
{
	Assert( m_pWorld );
	
	g_dwCharacterCount--;

	if( g_pPickCharacter && g_pPickCharacter->GetId() == GetId() )
	{
		g_pPickCharacter = NULL;
	} //if

	if( g_sTarget.IsTarget( CHARACTER, m_dwId ) )
	{ // 공격중인 캐릭터인 경우
		g_sTarget.Clean();
	} //if

	if( m_bPlayer )
	{
		g_pPlayer	= NULL;
		g_pHelper	= NULL;

		if( m_pShadow )
		{
			if( g_pHeroShadow && g_pHeroShadow->GetRefCount() <= 1 )
			{
				SAFE_RELEASE( g_pHeroShadow );
			}
			else
			{
				SAFE_RELEASE( m_pShadow );
			} //if..else..	
		} //if
	} 
	else
	{
		if( m_pShadow )
		{
			if( g_pCharacterShadow && g_pCharacterShadow->GetRefCount() <= 1 )
			{
				SAFE_RELEASE( g_pCharacterShadow );
			} 
			else
			{
				SAFE_RELEASE( m_pShadow );
			} //if		
		} //if
	} //if

	if( g_pSelectMgr && g_pSelectMgr->GetType() == CHARACTER &&
		g_pSelectMgr->GetCreature() &&
		g_pSelectMgr->GetCreature()->GetId() == GetId()
		)
	{
		g_pSelectMgr->Select( NULL );		
	} //if

	ClearTargets();
	ClearAttackers();

	IW3DTraceEffect*	pTraceEffect = m_pWorld->GetTraceEffect();
	Assert( pTraceEffect );

	if( pTraceEffect ) 
	{
		pTraceEffect->DeleteEffect( m_dwId );
	} //if

	//SAFE_RELEASE( m_pShadow );
//	g_pMay = NULL;
	SAFE_DELETE( m_pHelper );
	SAFE_DELETE( m_pActionMgr );
	SAFE_DELETE( m_pEmblem );

	if( m_pSkill )
	{
//		g_mgrSkill.ReleaseSkill( m_pSkill );
	} //if

	for( int i = 0 ; i < MAX_PARTS ; i++ )
	{
		UnloadModel( &m_apModel[i], m_asModel[i].c_str() );
	} //for

	for( int j = 0 ; j < MAX_ITEMPARTS ; j++ )
	{
		UnloadModel( &m_apItemModel[j], m_asItemModel[j].c_str() );
	} //for

	RemovePickedEffect();

	/*
		캐릭터가 모두 삭제되면, 바운드 받스도 모두 삭제 한다.
		World에서 캐릭터를 가장 나중에 소거 한다.
	*/
	if( g_dwCharacterCount <= 0 )
	{
	//	ReleaseCreatureBoundBox();
	} //if


//	CCreature::~CCreature();
} //CW3DCharacter::~CW3DCharacter

/**
 * 캐릭터를 생성한다.
 *
 * @return	성공 여부
 */
BOOL CW3DCharacter::Create(
	IW3DWorld*		pWorld,		///< [IN] 월드 객체
	const DWORD		dwId,		///< [IN] 크리쳐 ID
	const void*		pData		///< [IN] 크리쳐 생성 정보
	)
{
	CCreature::Create( pWorld, dwId, pData );

	Assert( pData );
	Assert( m_pWorld );

	if( !pData || !m_pWorld )
	{
		return FALSE;
	} //if

	SW3DCharacterInfo&	infoCharacter = *(SW3DCharacterInfo*)pData;

	m_bPlayer	= ( dwId == g_dwPlayerId ) ? TRUE : FALSE;
	m_dwId		= dwId;
	m_nGuildId	= infoCharacter.nGuildId;
	m_nSex		= infoCharacter.bSex;
	m_nSex == _MASTER ? m_bMasterPlayer = TRUE : m_bMasterPlayer = FALSE;
	m_sName		= infoCharacter.szName;
	m_vPos.x	= (float)infoCharacter.nX + 0.5F;
	m_vPos.y	= (float)infoCharacter.nY + 0.5F;
	m_nLayer	= infoCharacter.btLayer;
	m_pWorld->SetLayer( m_nLayer );
	m_vPos.z	= GetHeight();
	m_fOffset	= (float)infoCharacter.btOffset / 10.0f;
	m_szCorpsName[0] = '\0';
	

	ResetEnemy();	//
	
	m_vGoalPos	= m_vPos;
	m_qRot		= Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion();

	SetRace( infoCharacter );

	SAFE_RELEASE( m_pSound );
	m_pSound = MAKE_COMPONENT( SoundObject3D );
	m_pSound->SetPosition( m_vPos );


	m_pActionMgr = new CCharacterAction;

	// Load Body
	if( !LoadBody( &infoCharacter ) )
	{
		Assert( !"모델 로드 실패" );
		return FALSE;
	} //if

	m_pActionMgr->Init( &m_apModel[UPPER_PART], m_pWorld, this );
	m_pActionMgr->SetSex( m_nSex );


	// Load Accessory
	if( !LoadAccessory( &infoCharacter ) )
	{
		Assert( !"모델 로드실패" );
		return FALSE;
	} //if
	
	// Load BoundBox
	LoadBoundBox( "Body", "charBound.mod" );

	if( m_bPlayer )
	{
		// Create Shadow
		SAFE_RELEASE( g_pHeroShadow );

		g_pHeroShadow = MAKE_COMPONENT( Shadow );
		CHECK_PTR_RETURN( g_pHeroShadow );

		g_pHeroShadow->SetSize( 3 );
		
		if( !g_pHeroShadow->Init( g_bShadowOn ) )
		{
			Assert( !"그림자 초기화 실패" );
			SAFE_RELEASE( g_pHeroShadow );
		} //if

		m_pShadow = g_pHeroShadow;
	}
	else
	{
		if( g_pCharacterShadow )
		{
			g_pCharacterShadow->AddRef();
		}
		else
		{
			g_pCharacterShadow = MAKE_COMPONENT( Shadow );
			CHECK_PTR_RETURN( g_pCharacterShadow );

			g_pCharacterShadow->SetSize( 3 );

			if( !g_pCharacterShadow->Init( m_bPlayer ) )
			{
				Assert( !"그림자 초기화 실패" );
				SAFE_RELEASE( g_pCharacterShadow );
			} //if
		} //if..else..

		m_pShadow = g_pCharacterShadow;
	}//if

	// Create Trace Effect
	char	szTraceName[256];
	sprintf( szTraceName, "weapon_%d", m_dwId );

	IW3DTraceEffect*	pTraceEffect = m_pWorld->GetTraceEffect();
	Assert( pTraceEffect );

	if( pTraceEffect )
	{
		pTraceEffect->CreateEffect( "검흔", m_dwId );
	} //if

	/////////////////////////
	m_pAction = m_pActionMgr->GetAction();

	SAFE_DELETE( m_pHelper );

	if( m_bPlayer )
	{
		if( !m_bMasterPlayer ) 
		{
			if( g_infoPlayer.GetTotalLevel() <= 9 && g_bShowMay )
			{
				m_pHelper = new CMay;
				m_pHelper->Init( m_pWorld, this );
				m_pHelper->SetInfo( 0, 0, m_bPlayer );
				m_pHelper->SetSpeed( m_fOffset );
				
				g_pHelper = m_pHelper;
				
				g_pGUI->ChattingInput( "", STR_MAY_HELPER, FONTCOLOR_REF );
			} //if
			g_dwPickingEffect = 0;
		}		
	} //if

	/*
		TODO : infoCharacter.wRide 에 따라 말을 탔는지의 여부를 ...
				말을 타지 않은 상태를 추가해야함
	*/
	if( infoCharacter.wRide )
	{
		SW3DVehicle vehicle;
		vehicle.wType = infoCharacter.wRide;
		vehicle.wId = 0;
		vehicle.nSex = infoCharacter.bSex;
		
		AddVehicle( &vehicle );
		Assert( m_pVehicle );

		m_pActionMgr->SetVehicleActionMgr( m_pVehicle->GetActionMgr() );
		m_pActionMgr->SetMode( MODE_VEHICLE + m_pVehicle->GetType() );
	} //if

	if( m_bPlayer )
	{
		m_pLightEffect = m_pWorld->GetLight( 4 );

		if( m_pLightEffect )
		{
			m_pLightEffect->SetPoint();
			m_pLightEffect->SetAmbient( 0xFFFFFFFF );
			m_pLightEffect->SetDiffuse( 0xFFFFFFFF );
			m_pLightEffect->SetSpecular( 0xFFFFFFFF );
			m_pLightEffect->SetRange( 140.0F );
			m_pLightEffect->SetDirection( Vector3( 0.0F, 1.0F, 0.0F ) );
			m_pLightEffect->SetLinearAttenuation( 0.6F );
			m_pLightEffect->SetPosition( m_vPos );
		} //if
	}
	else
	{
		m_pLightEffect = NULL;
	} //if..else..

	SAFE_DELETE( m_pEmblem );

	m_pEmblem = new SEmblem;
	
	/*
		臨時
		sprintf( m_szCorpsName, "황금룡 기사단" );
		m_pEmblem->nId = 1;
		m_pEmblem->Load();
	*/

	/*
		TODO : 펫
	*/

/*	m_pPet = new CPet;
	CHECK_PTR_RETURN( m_pPet );

	m_pPet->Init( this, 1, std::string( "바보의 팻" ) );*/

	return TRUE;
} //CW3DCharacter::Create

/**
 * 캐릭터 모델을 업데이트 한다.
 *
 * @param	wItemType	[IN] 아이템 종류
 * @param	bOn			[IN] 장착/해제
 *
 * @return	업데이트 성공 여부
 */
BOOL CW3DCharacter::UpdateModel( const WORD wItemType, const BOOL bOn )
{
	if( m_bMasterPlayer )	return TRUE;

	IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();
	CHECK_PTR_RETURN( pItemInfoMgr );

	SItemInfoData*		pItemInfo = pItemInfoMgr->GetItemInfo( wItemType );
	if( !pItemInfo )
	{
		Assert( !"없는 아이템" );
		return FALSE;
	} //if

	WORD		wPart = pItemInfo->nItemKind;
	int			nId   = wItemType;
	if( !bOn )
	{	// 장착 해제
		nId = GetNakedBody( wPart, m_nSex );
		if( nId < 0 ) nId   = wItemType;
	} //if

	pItemInfo = pItemInfoMgr->GetItemInfo( nId );
	if( !pItemInfo )
	{
		Assert( !"없는 아이템" );
		return FALSE;
	} //if

	WORD		wUpdatePart;
	char*		pFileName = ITEMFILENAME1( m_nSex, pItemInfo );
	BOOL		bHaveSkin = ITEMHAVESKIN( m_nSex, pItemInfo );
	switch( GetModelPart( wPart, wUpdatePart ) )
	{
	case 1:
		if( !g_btWarMode && 
			!IS_CHAR_SIEGE( m_nGuildId ) &&
			!IS_CHAR_DEFENSE( m_nGuildId ) )
		{
			UnloadModel( &m_apModel[wUpdatePart], m_asModel[wUpdatePart].c_str() );
			LoadBodyPart( wUpdatePart, pFileName, bHaveSkin );
			m_pActionMgr->SetModel( &m_apModel[UPPER_PART] );
		} //if
		break;

	case 2:
		UnloadModel( &m_apItemModel[wUpdatePart], m_asItemModel[wUpdatePart].c_str() );
		if( bOn )
		{
			LoadEquipPart( wUpdatePart, pFileName );
			SetWeapon( pItemInfo );
		} 
		else
		{
			if( m_nSkill == SKILL_LONG )
			{
				if( pItemInfo->btItemType == m_nLongWeapon )
				{
					m_pActionMgr->SetWeapon( 0 );	
				} //if
			}
			else if( m_nSkill == SKILL_SHORT )
			{
				if( pItemInfo->btItemType == m_nShortWeapon && pItemInfo->btItemType != dITEMTYPE_SHIELD )
				{
					/*
						TODO : 	
					*/	
					m_pActionMgr->SetWeapon( 0 );
				} //if
			}//if
		
		} //if
		break;
	}
	return TRUE;
} //CW3DCharacter::UpdateModel

/**
 * 몸 부분별 로드.
 */
BOOL CW3DCharacter::LoadBodyPart( const int nPart, const char* szFileName, BOOL bSkin )
{
	m_asModel[nPart] = szFileName;
	if( bSkin && m_cRace != '0' )
	{
		if( szFileName[1] == 'b' )
		{
			m_asModel[nPart].at( strlen( szFileName ) - 7 ) = m_cRace;
		}
		else
		{
			m_asModel[nPart].at( strlen( szFileName ) - 9 ) = m_cRace;
		} //if..else
	} //if


	/*g_BodyMgr.Load( &m_apModel[nPart],
					"Body",
					m_asModel[nPart],
					"High" 
					);
	*/
	m_apModel[nPart] = LoadModel(
		"Body",
		m_asModel[nPart],
		"High" 
		);
	CHECK_PTR_RETURN( m_apModel[nPart] );

	m_pActionMgr->SetModel( &m_apModel[UPPER_PART] );

	return TRUE;
} //CW3DCharacter::LoadBodyPart

/**
 * Load OneBody Character 
 */
BOOL CW3DCharacter::LoadOneBody( SW3DCharacterInfo* pInfoCharacter )
{
	Assert( pInfoCharacter );

	if( pInfoCharacter->bSex )
	{
		if( IS_CHAR_SIEGE( m_nGuildId ) )			m_asModel[UPPER_PART] = "fe_sg_f001_1.mod";			///< Siege
		else if( IS_CHAR_DEFENSE( m_nGuildId ) )	m_asModel[UPPER_PART] = "fe_sg_f002_1.mod";			///< Defense
		else							return FALSE;
	}
	else
	{
		if( IS_CHAR_SIEGE( m_nGuildId ) )			m_asModel[UPPER_PART] = "ma_sg_f001_1.mod";			///< Siege
		else if( IS_CHAR_DEFENSE( m_nGuildId ) )	m_asModel[UPPER_PART] = "ma_sg_f002_1.mod";			///< Defense
		else							return FALSE;
	} //if..else

	m_apModel[UPPER_PART] = LoadModel(
		"Body",
		m_asModel[UPPER_PART],
		"High" 
		);
	CHECK_PTR_RETURN( m_apModel[UPPER_PART] );

	char*		pFileName;
	BOOL		bHaveSkin;
	char		szExpenName[32];

	SItemInfoData*		pItemInfo[2];
	IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();

	pItemInfo[0] = pItemInfoMgr->GetItemInfo( pInfoCharacter->wHair );
	if( !pItemInfo[0] )
	{
		Assert( !"등록되지 않았다" );
		return FALSE;
	} //if

	bHaveSkin = ITEMHAVESKIN( m_nSex, pItemInfo[0] );
	pFileName = ITEMFILENAME1( m_nSex, pItemInfo[0] );
	strcpy( szExpenName, pFileName );
	strcpy( szExpenName+strlen(szExpenName) - 4, "-s.mod" );
	LoadBodyPart( HAIR_PART, szExpenName, bHaveSkin );

	pItemInfo[1] = pItemInfoMgr->GetItemInfo( pInfoCharacter->wFace );
	if( !pItemInfo[1] )
	{
		Assert( !"등록되지 않았다" );
		return FALSE;
	} //if

	pFileName = ITEMFILENAME1( m_nSex, pItemInfo[1] );
	bHaveSkin = ITEMHAVESKIN( m_nSex, pItemInfo[1] );
	strcpy( szExpenName, pFileName );
	strcpy( szExpenName+strlen(szExpenName) - 4, "-s.mod" );
	LoadBodyPart( FACE_PART, szExpenName, bHaveSkin );

	m_pActionMgr->SetModel( &m_apModel[UPPER_PART] );

	return TRUE;
} //CW3DCharacter::LoadOneBody

/**
 * 장착 장비 부분별 로드.
 */
BOOL CW3DCharacter::LoadEquipPart( const int nPart, const char* szFileName )
{
	m_asItemModel[nPart] = szFileName;

	m_apItemModel[nPart] = LoadModel(
		"Equip",
		m_asItemModel[nPart],
		""
		);

	/*g_BodyMgr.Load( &m_apModel[nPart],
					"Equip",
					m_asItemModel[nPart],
					""
					);*/

	CHECK_PTR_RETURN( m_apItemModel[nPart] );

	if( SHORTWEAPON_ITEMPART == nPart )
	{
		m_pTrace = g_pTrace->LoadTrace( _strlwr( const_cast<char*>( szFileName ) ) );

		if( m_pTrace )
		{	
			IW3DTraceEffect*	pTraceEffect = m_pWorld->GetTraceEffect();
			Assert( pTraceEffect );

			if( pTraceEffect )
			{
				pTraceEffect->ChangeType( m_dwId, m_pTrace->sType.c_str() );
			} //if
			//ChangeType
		} //if
	} //if


	return TRUE;
} //CW3DCharacter::LoadEquipPart

/**
 * 바운딩 박스 로딩.
 */
BOOL CW3DCharacter::LoadBoundBox( const char* szPath, const char* szBoundBox )
{
	Assert( szPath );
	Assert( szBoundBox );

	BoundBoxMap::iterator	itor = gs_mapBoundBoxModel.find( szBoundBox );

	if( itor != gs_mapBoundBoxModel.end() )
	{
		IW3DModel*	pModel = itor->second;
		Assert( pModel );

		m_pBoundBox = pModel;

		return TRUE;
	} //if

	m_pBoundBox = MAKE_COMPONENT( Model );
	CHECK_PTR_RETURN( m_pBoundBox );
	if( !m_pBoundBox->Load( szPath, szBoundBox ) )
	{
		SAFE_RELEASE( m_pBoundBox );
		return FALSE;
	} //if

	gs_mapBoundBoxModel.insert( std::make_pair( szBoundBox, m_pBoundBox ) );

	return TRUE;
} //CW3DCharacter::LoadBoundBox

void CW3DCharacter::UnloadBoundBox( IW3DModel*	pModel, const char* szBoundBox )
{
	if( pModel && pModel->GetRefCount() <= 1 )
	{
		BoundBoxMap::iterator	itor = gs_mapBoundBoxModel.find( szBoundBox );

		if( itor != gs_mapBoundBoxModel.end() )
		{	
			gs_mapBoundBoxModel.erase( itor );
		} //if		

	} //if

	SAFE_RELEASE( pModel );
} //CW3DCharacter::UnloadBoundBox

/**
 * 벗은 몸의 아이템번호를 얻는다.
 *
 * @param		[IN]nPart		몸 부분.
 * @return		아이템 번호.
 */
int CW3DCharacter::GetNakedBody( const WORD wPart, const UINT nSex )
{
	int			nId = -1;

	switch( wPart )
	{
	case EQUIP_SHOES:			nId = 10094;		break;
	case EQUIP_GLOVE:			nId = 10091;		break;
	case EQUIP_UPPERARMOR:		nId = 10092;		break;
	case EQUIP_LOWERARMOR:		nId = 10093;		break;
	}

	return nId;
} //CW3DCharacter::GetNakedBody

/**
 * 장착부분을 모델부분으로..
 *
 * @param		[IN]  wPart			장착위치
 * @param		[OUT] wModelPart	모델 인덱스
 *
 * @return		모델 분류 ( 1 - Body, 2 - Equip )
 */
int	CW3DCharacter::GetModelPart( const WORD wPart, WORD& wModelPart )
{
	int			nModelType = 0;
	switch( wPart )
	{
	case EQUIP_SHOES:		
		nModelType	= 1;
		wModelPart = FOOT_PART;
		break;

	case EQUIP_GLOVE:
		nModelType	= 1;
		wModelPart = HAND_PART;
		break;

	case EQUIP_UPPERARMOR:
		nModelType	= 1;
		wModelPart = UPPER_PART;
		break;

	case EQUIP_LOWERARMOR:
		nModelType	= 1;
		wModelPart = LOWER_PART;
		break;

	case EQUIP_SHORTWEAPON:
		nModelType	= 2;
		wModelPart = SHORTWEAPON_ITEMPART;
		break;

	case EQUIP_LONGWEAPON:
		nModelType	= 2;
		wModelPart = LONGWEAPON_ITEMPART;
		break;

	case EQUIP_HELMET:
		nModelType	= 2;
		wModelPart = HELMET_ITEMPART;
		break;

	case EQUIP_SHIELD:
		nModelType	= 2;
		wModelPart = SHIELD_ITEMPART;
		break;
	} //switch

	return nModelType;
} //CW3DCharacter::GetModelPart

/**
 * 인종설정.
 */
void CW3DCharacter::SetRace( SW3DCharacterInfo& infoCharacter )
{
	IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();
	SItemInfoData*		pItemInfo = pItemInfoMgr->GetItemInfo( infoCharacter.wFace );
	if( !pItemInfo )
	{
		Assert( !"없는 얼굴" );
		return;
	} //if

	char*		pFileName = ITEMFILENAME1( m_nSex, pItemInfo );
	m_cRace = *( pFileName + strlen( pFileName ) - 7);
} //CW3DCharacter::SetRace

/**
 * 공격모드 설정.
 */
void CW3DCharacter::SetAttackMode( const int nSkill	)
{
	m_nChangedSkill = nSkill;
} //CW3DCharacter::SetAttackMode

/**
 * 스길 적용.
 */
void CW3DCharacter::ApplySkill()
{
	if( m_nChangedSkill < 0 || !m_bEndMagic )
	{ //적용스킬 없는지...
		return;
	} //if

	CCreature::SetAttackMode( m_nChangedSkill );

	m_MagicData.bCast = FALSE;

	switch( m_nSkill )
	{
	case SKILL_SHORT:		// 근거리
		if( m_btSwordType == SWORD_ONEHANDSWORD )
		{ // 한 손칼
			if( m_apItemModel[SHIELD_ITEMPART] )			m_btWeaponType = WEAPON_ONEHANDSWORDWITHSHIELD;
			else											m_btWeaponType = WEAPON_ONEHANDSWORD;
		}
		else if( m_btSwordType == SWORD_TWOHANDSWORD )		m_btWeaponType = WEAPON_TWOHANDSWORD;
		break;

	case SKILL_LONG:		// 원거리
		m_btWeaponType = WEAPON_LONGBOW;
		break;

	case SKILL_WHITE:
	case SKILL_BLACK:
		if( m_btSwordType == SWORD_TWOHANDSTAFF )			m_btWeaponType = WEAPON_STAFF;
		else												m_btWeaponType = WEAPON_NONE;
		m_MagicData.bCast = TRUE;
		break;
	}

	switch( m_nChangedSkill )
	{
	case ITEM_SBASICSKILL:
		
		if( m_apItemModel[SHIELD_ITEMPART] ) 
		{
			m_pActionMgr->SetWithShield( TRUE );
		} 
		else
		{
			m_pActionMgr->SetWithShield( FALSE );
		} //if..else..

		m_pActionMgr->SetWeapon( m_nShortWeapon );
		m_MagicData.bCast = FALSE;
		break;
	case ITEM_LBASICSKILL:
		m_pActionMgr->SetWithShield( FALSE );
		m_pActionMgr->SetWeapon( m_nLongWeapon );
		m_MagicData.bCast = FALSE;
		break;
	} //switch

	if( m_nChangedSkill > 2500 && m_nChangedSkill < 2600 )
	{
		m_pActionMgr->SetWeapon( m_nShortWeapon );
		m_MagicData.bCast = TRUE;
	} //if

	m_nChangedSkill = -1;
	g_sTarget.Clean();
} //CW3DCharacter::ApplySkill

/**
 * 매직정보 설정.
 */
void CW3DCharacter::SetMagicInfo( const BYTE btMagicType, const BYTE btMagicPickType )
{
	m_btMagicType	  = btMagicType;
	m_btMagicPickType = btMagicPickType;
} //CW3DCharacter::SetMagicInfo

/**
 * 사용중인 스킬적용정보.
 */
BOOL CW3DCharacter::GetApplyInfoSkill( BYTE* btSkillType, BYTE* btTaget )
{
	switch( m_nSkill )
	{
	case 0:
	case 1:			
		*btSkillType = 0;
		*btTaget = 0;
		return TRUE;

	case 2:
	case 3:
		*btSkillType = m_btMagicType;
		*btTaget = m_btMagicPickType;
		return TRUE;

	default:
		*btSkillType = 0;
		*btTaget = 0;
		return FALSE;
	} //switch
	
	return TRUE;
} //CW3DCharacter::GetApplyInfoSkill

/**
 * 사용무기 설정.
 */
void CW3DCharacter::SetWeapon( SItemInfoData* pItemInfo )
{
	Assert( pItemInfo );

	switch( pItemInfo->nItemKind )
	{
	case EQUIP_SHORTWEAPON:	
		if( m_nSkill != SKILL_LONG )
		{
			m_pActionMgr->SetWeapon( pItemInfo->btItemType );
		}
		else
		{
			m_pActionMgr->SetWeapon( m_nLongWeapon );
		}

		if( ISONEHANDWEAPON( pItemInfo->btItemType ) )
		{
			m_btSwordType = SWORD_ONEHANDSWORD;		
		}
		else
		{
			m_btSwordType = SWORD_TWOHANDSWORD;	
		} //if..else
		
		m_nShortWeapon = pItemInfo->btItemType;	
		break;
	case EQUIP_LONGWEAPON:	
		if( m_nSkill == SKILL_LONG )
		{
			m_pActionMgr->SetWeapon( pItemInfo->btItemType );
		}
		m_nLongWeapon = pItemInfo->btItemType;
		SetArrowInfo( pItemInfo->wItemNum );
		break;
	} //switch
} //CW3DCharacter::SetWeapon

/**
 * 데미지 효과.
 */
void CW3DCharacter::UpdateDamageEffect( DWORD dwTick )
{
	if( !m_bDamageEffect )
	{
		return;
	} //if

	m_dwDamageEffectTick += dwTick;
	if( m_dwDamageEffectTick > DAMAGE_REACTION )
	{
		m_bDamageEffect = FALSE;
		m_dwDamageEffectTick = 0;
		m_vPos = m_vGoalPos;
	} //if
} //CW3DCharacter::UpdateDamageEffect

/**
 * 캐릭터 업데이트.
 */
BOOL CW3DCharacter::Update( const DWORD dwTick )
{
	Assert( m_pWorld );
	Assert( m_pShadow );

	if( !m_pWorld )
	{
		return FALSE;
	} //if

	Frustum& frustum = m_pScene->GetFrustum();
	static Vector3	vPos;
	vPos = m_vPos + Vector3( 0.0F, 0.0F, 1.0F );

	//CalculateDistLevel();
	m_bShow = FALSE;
	
	if( frustum.CheckSphere( vPos, 1.0F ) )
	{
		m_bShow = TRUE;

		m_matWorld.SetWorld( m_vPos, m_qRot );

		if( m_btDistLevel > g_btShowLevel )
		{
			//m_bShow = FALSE;			
		} //if
		
		if( m_pShadow && m_pShadow->IsProjection() )
		{
			m_pShadow->SetLight( m_vPos );

			for( int i = 0 ; i < MAX_PARTS ; i++ )
			{
				if( m_apModel[i] )
				{
					m_apModel[i]->SetAction( m_pAction );
					m_apModel[i]->SetWorld( m_matWorld );
					m_pShadow->InsertBlocker( m_apModel[i] );
				} //if
			} //for
				
			if( m_bUseVehicle && m_pVehicle )
			{
				//m_pVehicle->GetModel
				m_pVehicle->SetPosition( m_vPos );
				m_pVehicle->SetQuaternion( m_qRot );
				m_pShadow->InsertBlocker( m_pVehicle->GetModel() );
			} //if

			m_pShadow->ShadowMatrix( m_vPos );
			m_pShadow->MakeShadowMap( m_dwTick );
		} //if
		
		g_dwViewChatacter++;
	} //if

	BOOL	bLive = TRUE;
	
	if( m_vPos.z < 0 ) 
	{
		m_pActionMgr->SetUnderWater( TRUE );		
	}	
	else
	{
		m_pActionMgr->SetUnderWater( FALSE );
	}	

	m_pActionMgr->Update( dwTick );

	if( m_bUseVehicle )
	{
		m_pVehicle->SetTime( m_pActionMgr->GetTime() );
	} //if


	switch( CCreature::GetState() )
	{
	case STATE_DEAD:
		IsEndAction( m_dwTick + dwTick );
		bLive = StateDead( dwTick );
		break;

	case STATE_ATTACK:
		ApplySkill();
		IsEndAction( m_dwTick + dwTick );
		StateAttack( dwTick, 1 );
		break;

	case STATE_MAGIC:
		ApplySkill();
		IsEndAction( m_dwTick + dwTick );
		StateMagic( dwTick );
		break;

	case STATE_SKILL:
		ApplySkill();
		IsEndAction( m_dwTick + dwTick );
		StateSkill( dwTick );
		break;

	case STATE_MOVE:
		ApplySkill();
		m_bIsEndAction = m_pActionMgr->IsEnd();
		StateMovement( dwTick );
		//ApplySkill();
		break;

	case STATE_STOP:
		ApplySkill();
		if( m_vLastGoal != m_vPos )  //!IsGoal()
		{
			m_pActionMgr->Run();
		} //if
		//ApplySkill();
		break;

	case STATE_FISHING:
		break;

	default:
		ApplySkill();
		m_bIsEndAction = m_pActionMgr->IsEnd();
		StateMovement( dwTick );
		//ApplySkill();
		break;
	} //switch

	UpdateDamageEffect( dwTick );

	if( m_bUseVehicle )
	{
		UpdateVehicle( dwTick );
	} //if

	if( m_pHelper )
	{
		m_pHelper->Update( dwTick );
	} //if

	if( !m_bEndMagic )
	{
		m_dwMagicTick += dwTick;

		if( m_dwMagicTick <= 3000 )
		{
			m_bEndMagic = TRUE;
			m_dwMagicTick = 0;
		} //if
	} //if

	if( m_pLightEffect )
	{
		m_pLightEffect->SetPosition( m_vPos );
		Update_Light();
	} //if
	
	m_dwTick		= m_pActionMgr->GetTime();
	m_dwTalkTime	+= dwTick;
	m_dwLightTime	+= dwTick;

	return bLive;
} //CW3DCharacter::Update

/**
 *
 */
void CW3DCharacter::Update_Light()
{
	if( m_dwLightTime >= 20 )
	{
		m_dwLightTime = 0;
		m_pLightEffect->Update();
	} //if
//	m_pLightEffect->
} //CW3DCharacter::Update_Light

/**
 * 캐릭터를 출력한다.
 */
void CW3DCharacter::Draw()
{
	Assert( m_pWorld );

	if( !m_pWorld || !m_bShow || !m_bVisible )		
	{
		return;
	} //if

#ifdef SHOW_BOUNDBOX
	m_pBoundBox->Render( m_dwTick );
#endif

	static BOOL bFlag = FALSE;
	static BOOL bLighting = FALSE;
	bFlag = FALSE;
	bLighting = FALSE;



	if( m_btGhost || m_bLighting )
		bLighting = TRUE;

	for( int i = 0 ; i < MAX_PARTS ; i++ )
	{
		if( m_apModel[i] )
		{
			m_apModel[i]->SetApplyWorld( FALSE );
			m_apModel[i]->SetAction( m_pAction );
			m_apModel[i]->SetWorld( m_vPos, m_qRot );
						
			/*
				캐릭터의 경우, Bone 애니메이션이 결국 한번만 연산 하면 되므로,ㅡ
				굳이, 모든 Model에 연산할 필요가 없다.
				모델중 단 한번만, Blend Animation을 World Matrix로 넣고,
				나머지는 이걸 그냥 사용만 하면 된다.
			*/
			if( !bFlag )
			{
				m_apModel[i]->SetVertexBlend( m_dwTick );
				bFlag = TRUE;
			} //if

			
			m_apModel[i]->SetColor( m_dwColor );
			m_apModel[i]->Render( m_dwTick, bLighting );
			m_apModel[i]->SetApplyWorld( TRUE );
		} //if
	} //for

	if( m_bUseVehicle )
	{
		DrawVehicle();
	} //if

	switch( m_nSkill )
	{
	case SKILL_SHORT:		// 근거리
		DrawShortWeapon();
		if( SWORD_ONEHANDSWORD == m_btSwordType )	DrawShield();
		break;

	case SKILL_LONG:		// 원거리
		DrawLongWeapon();
		break;

	case SKILL_WHITE:		// 마법 #1
	case SKILL_BLACK:		// 마법 #2
		DrawShortWeapon();
		break;
	} //switch

	g_dwViewChatacter--;
} //CW3DCharacter::Draw

/**
 * 캐릭터의 그림자를 출력한다.
 */
void CW3DCharacter::DrawShadow()
{
	DrawShopTitle();

	if( m_btGhost || !m_bVisible )
	{
		return ;
	} //if
	DrawBaseInfo();

	if( m_nLayer > 0 )
	{
		DrawShopTitle();
		DrawChattText();

		if( m_pHelper )
		{
			m_pHelper->Draw();
		} //if
		return ;
	} //if

	Assert( m_pWorld );
	Assert( m_pShadow );
	if( !m_pScene || !m_pWorld )
	{
		return ;
	} //if

	if( !m_bShow )
	{
		return;
	} //if

	if( m_pShadow )
	{
		if( m_bUseVehicle && !m_bPlayer )
		{ // 때만....
			DrawChattText();

			if( m_pHelper )
			{
				m_pHelper->Draw();
			} //if
			return ;
		} //if

		if( m_pShadow->IsProjection() )
		{
			m_pWorld->GetTerrain()->InsertShadow( m_pShadow, m_vPos );
			m_pWorld->SetLayer( m_nLayer );
			m_pShadow->Render( m_pScene->GetFrustum().m_matView );
		}
		else
		{
			static Matrix4	matWorld;
			matWorld.SetWorld( m_vPos + Vector3( 0.0F, 0.0F, 0.1F ), m_qRot );
			m_pShadow->Render( matWorld );
		} //if..else..
	} //if..else..

	DrawChattText();

	if( m_pHelper )
	{
		m_pHelper->Draw();
		if( !m_bLighting )
		{
			m_pHelper->DrawName( m_pRectangle );
		} //if
	} //if

} //CW3DCharacter::DrawShadow

/**
 * 크리쳐 종류를 얻는다.
 */
int	CW3DCharacter::GetType()
{
	return CHARACTER;
} //CW3DCharacter::GetType

/**
 * 성별을 얻는다.
 */
UINT CW3DCharacter::GetSex()
{
	return m_nSex;
}

/**
 * 말풍선에 들어갈 문자열을 설정한다.
 */
void CW3DCharacter::SetTalkText( char* szText )
{
	Assert( szText );

	char*		pCurr	= szText;
	char*		pEnd	= szText + strlen( szText );// - 1;
	char*		pText	= szText;
	char		pTest[1024];
	char		chBuff;
	int			nLine   = 0;

	pTest [0] = NULL;
	m_sTalk = "";
	while( pCurr < pEnd )
	{
		if( g_pGUI->IsDoubleByteCharset( pCurr ) )
		{
			if( ++pCurr > pEnd )
			{
				break;
			} //if
		} //if
		chBuff = *(pCurr+1);
		*(pCurr+1) = '\0';
		
		m_pScene->GetStringSize( m_nTalkTxtWidth, m_nTalkTxtHeight, pText );
		
		if( m_nTalkTxtWidth > TEXT_WIDTHLIMIT )
		{
			m_sTalk += pText;
			m_sTalk += "\n";
			pText = pCurr + 1;
			nLine++;
		} //if
		*(pCurr+1) = chBuff;
		pCurr++;
	} //while

	int		nStart=0;
	int		nEnd = strlen( szText );
	pCurr	= szText;

	while( nStart < nEnd )
	{
		chBuff = *(pCurr+1);
		*(pCurr+1) = '\0';

		if( strcmp( pCurr, "%" ) ==0 )
		{
			strcat( pTest, "%" );
		}

		strcat( pTest, pCurr );
		
		*(pCurr+1) = chBuff;
		pCurr++;
		nStart++;
	}

	pText = pTest;

	if( !nLine )
	{
		m_sTalk		 = pText;
	//	m_pScene->GetStringSize( m_nTalkTxtWidth, m_nTalkTxtHeight, pText );
	}
	else
	{
		m_sTalk		+= pText;
		m_nTalkTxtWidth  = TEXT_WIDTHLIMIT + 10;
		m_nTalkTxtHeight = 12 * ( nLine + 1 );
	} //if

	m_dwTalkTime = 0;
} //CW3DCharacter::SetTalkText


/**
 * 길찾기를 통해 목표점까지 이동한다.
 */
BOOL CW3DCharacter::MoveTo( const Vector3& vPos )
{
	Assert( m_pWorld );

	m_aPath.clear();

	SetLayer();
	SetGoalPosition( vPos );

	//SetAutoPath();
	m_bAutoPath = TRUE;
	
	if( GetState() == STATE_ATTACK )
	{
		m_bSemiMove = TRUE;
		return TRUE;
	} //if
	
	m_bSemiMove = FALSE;

	//m_pActionMgr->SetS
	m_pActionMgr->Run();
	SetAction( ACTION_MOVE );

	if( !m_sTarget.IsEmpty() )
	{
		IW3DCreature*	pTarget = GetTargetCreature();
		if( pTarget )
		{
			pTarget->SetTarget( TRUE, FALSE );
		} //if
	} //if

	AddPickedEffect();

	return TRUE;
} //CW3DCharacter::MoveTo

/**
 * 자동 길찾기를 실행
 */
void CW3DCharacter::SetAutoPath()
{
/*	if( m_nLayer != 0 )
	{
		return ;
	} //if*/

	m_bAutoPath = FALSE;

	m_aPath.clear();

	int			nPathSize;
	Vector3*	pPath = NULL;

	if( m_bUseVehicle )
	{
		nPathSize = m_pWorld->PathFind( m_vPos,  m_vLastGoal, &pPath, TRUE );
	}
	else
	{
		nPathSize = m_pWorld->PathFind( m_vPos,  m_vLastGoal, &pPath );
	} //if..else..

		
	if( nPathSize <= 0 || !pPath )
	{
		//m_vGoalPos = m_vPos;
		//m_vLastGoal = m_vPos;

		AddPickedEffect();
		//Stop();
		return;
	} //if	

	for( int i = 0 ; i < nPathSize ; i++ )
	{
		m_aPath.push_back( pPath[i] );
	} //for

	m_vGoalPos = m_vPos;
	m_vLastGoal = pPath[nPathSize-1];
	m_vLastGoal.z = m_pWorld->GetHeight( m_vLastGoal ) + 0.2F;
	SetLayer();

	AddPickedEffect();

	m_bAutoPath = TRUE;
	
	SAFE_FREE( pPath );
} //CW3DCharacter::SetAutoPath

/**
 * 캐릭터의 이동을 멈춘다.
 */
void CW3DCharacter::Stop()
{
	m_pActionMgr->Stop();

	RemovePickedEffect();

	SetAction( ACTION_STOP );
} //CW3DCharacter::Stop

/**
 * 캐릭터가 이동을 한다.
 */
void CW3DCharacter::Move()
{
	m_pActionMgr->Run();

	SetAction( ACTION_MOVE );
} //CW3DCharacter::Move

/**
 * 캐릭터가 죽는다.
 */
void CW3DCharacter::Dead()
{
	m_pActionMgr->Dead();

	if( m_bPlayer && m_pHelper )
	{
		m_pHelper->Dead();
	} //if

	SetAction( ACTION_DEAD );
} //CW3DCharacter::Dead

/**
 * 죽었는지 확인.
 */
BOOL CW3DCharacter::IsDead()
{
	if( m_pActionMgr->GetState() == STATE_DEAD )
	{
		return TRUE;
	} //if
	return FALSE;
} //CW3DCharacter::IsDead

/**
 * 다른 크리쳐를 공격한다.
 */
void CW3DCharacter::Attack(
	IW3DCreature*	pTarget		///< [IN] 공격 대상
	)
{
	if( !pTarget )
	{
		Assert( !"대상이 어딨냐구" );
		return ;
	} //if

	if( m_bPlayer )
	{
		if( !CheckUsageSkill( pTarget ) )
		{
			return;
		} //if

		if( m_pHelper && m_pHelper->GetType() == CHelper::MAY_TYPE_MAY )
		{
			m_pHelper->Attack( pTarget );
		} //if
/* 
		if( pTarget->GetType() == CHARACTER && g_bBGM_On )
		{
			g_pMediaPlayer->Play( BGM_TYPE_EVENT, BGM_EVENT4 );	// pk 사운드 재생
		} //if*/
	} //if
	else
	{
		m_pActionMgr->Attack();
	} //if..else.. 		

	IW3DCreature*	pPrevTarget = GetTargetCreature();
	if( pPrevTarget )
	{
		pPrevTarget->SetTarget( FALSE, FALSE );
	} //if
	m_sTarget.SetTarget( pTarget->GetType(), pTarget->GetId() );
	pTarget->SetTarget( m_bPlayer, TRUE );

	m_bSemiMove = FALSE;
	m_dwTick  = 0;
	m_bAttack = FALSE;
	SetAction( ACTION_ATTACK );
} //CW3DCharacter::Attack

/**
 * 낚시 게임에서 낚시대를 드리운다
 */
void CW3DCharacter::Hook()
{
	SetGoalPosition( m_vPos );
	m_pActionMgr->Hook();
} // CW3DCharacter::Hook

/**
 * 일반마법 사용한다.
 */
void CW3DCharacter::CastMagic()
{
} //CW3DCharacter::CastMagic

/** 
 * 마법추가.
 */
void CW3DCharacter::AddMagic( const WORD wMagic, const WORD wID, IW3DCreature* pTarget, BOOL bCompulsion )
{
	Assert( pTarget );

	IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();
	Assert( pItemInfoMgr );
	if( !pItemInfoMgr )
	{
		return ;
	} //if

	SItemInfoData*		pItemInfo = pItemInfoMgr->GetItemInfo( wMagic );
	if( !pItemInfo )
	{
		Assert( !"없는 마법" );
		return;
	} //if

	IW3DMagic* pMagic = NULL;

	char szMagicName[128];

	if( m_MagicData.bCast && !bCompulsion )
	{
		sprintf( szMagicName, "%sA", pItemInfo->szEName );
	}
	else
	{
		sprintf( szMagicName, "%sB", pItemInfo->szEName );
	} //if..else..
	
	pMagic = m_pWorld->AddMagic( szMagicName, this,	pTarget );

	m_bEndMagic = FALSE;

	if( pMagic )
	{
		/*
			캐릭터 마법 동작을 공격에 0번쨰 타입으로 
		*/

		if( m_apItemModel[SHIELD_ITEMPART] ) 
		{
			m_pActionMgr->SetWithShield( TRUE );
		} 
		else
		{
			m_pActionMgr->SetWithShield( FALSE );
		} //if..else..

		m_pActionMgr->SetWeapon( m_nShortWeapon );

		if( m_MagicData.bCast )
		{
			m_pActionMgr->MagicCasting( 0, 0, 1 );
		} //if
	} //if
	
} //CW3DCharacter::AddMagic

/**
 * 공격을 받는다.
 */
void CW3DCharacter::Damage( BOOL bBegin, IW3DCreature* pAttacker )
{
	m_bDamage = bBegin;
	if( bBegin )
	{
		return;
	} //if
	
	if( m_bPlayer )
		DamageEffect( pAttacker );

	/*if( m_bPlayer )
	{
		if( pAttacker->GetType() == CHARACTER && g_bBGM_On )
		{
			g_pMediaPlayer->Play( BGM_TYPE_EVENT, BGM_EVENT4 );	// pk 사운드를 재생한다.
		}
	}*/

	if( m_bPlayer )
	{
		m_pWorld->AddMagic( "자신타격", this, this, FALSE );

		m_pLightEffect->SetAmbient( COLOR_RGBA( 150, 0, 0, 255 ) );
		m_pLightEffect->SetDiffuse( COLOR_RGBA( 150, 0, 0, 255 ) );
		m_pLightEffect->SetRange( 40 );
		m_pLightEffect->SetLinearAttenuation( 0.05F );

		m_pLightEffect->SetFade( TRUE );

		m_pLightEffect->SetAmbient( COLOR_RGBA( 255, 255, 255, 255 ) );
		m_pLightEffect->SetDiffuse( COLOR_RGBA( 255, 255, 255, 255 ) );
		m_pLightEffect->SetRange( 80 );
		m_pLightEffect->SetLinearAttenuation( 0.6F );

//		m_pWorld->GetLight( 
		//m_pWorld->
	} //if

} //CW3DCharacter::Damage

/**
 * 공격받은 수치설정.
 */
void CW3DCharacter::Damage( const WORD wDamage, const BYTE btCritical )
{
	m_wDamege	 = wDamage;
	m_btCritical = btCritical;

	m_Damage.btCritical = btCritical;
	m_Damage.wDamage	= wDamage;
	m_Damage.pCreature	= this;
} //CW3DCharacter::Damage

/**
 * 데미지 효과.
 */
void CW3DCharacter::DamageEffect( IW3DCreature* pAttacker )
{
//	m_pWorld->AddHit( m_wDamege, m_vPos, 0xFFFF0000, m_btCritical );

	if( m_bPlayer )
	{
		m_pWorld->AddHit( m_wDamege, m_vPos, 0xFFFF0000, m_btCritical );
		g_pGUI->SetPosHP( g_infoPlayer.nHp[1], g_infoPlayer.nHp[0] );
		m_pWorld->AddMagic( "자신타격", this, this, FALSE );
	} //if

	m_pWorld->AddHit( m_wDamege, m_vPos, 0xFFFFFFFF, m_btCritical );

/*	m_vGoalPos = m_vPos;

	Vector3		 vDir = pAttacker->GetPosition() - m_vPos;
	vDir.Normalize();
	m_vPos = m_vPos - vDir * DAMAGE_REACTION;

	m_bDamageEffect = TRUE;
	m_dwDamageEffectTick = 0;*/

} //CW3DCharacter::DamageEffect

/**
 *
 */
SW3DDamage* CW3DCharacter::GetDamage()
{
	return &m_Damage;
} //CW3DCharacter::GetDamage

/**
 * HP 설정.
 */
void CW3DCharacter::SetHp( const int nHp, const int nTotHP )
{
	if( m_bPlayer && m_nHp[0] < nHp )
	{
		g_pGUI->SetPosHP( nTotHP, nHp );
	} //if

	m_nHp[0] = nHp;
		
	if( nTotHP > 0 )
	{
		m_nHp[1] = nTotHP;
	} //if
} //CW3DCharacter::SetHp

/**
 * 캐릭터 상태를 설정한다.
 */
void CW3DCharacter::SetState( const int nState )
{
	if( m_nState == STATE_DEAD )		return;
	if( m_bStateLock )					return;
 	
	switch( nState )
	{
	case STATE_STOP:	
//		m_pSound->Stop();
//		m_pSound->Stop3D();
		m_aPath.clear();
		break;

	case STATE_MOVE:
		if( m_nState == STATE_MOVE )	break;
		break;
	case STATE_MAGIC:
		break;
	case STATE_DEAD:
		if( m_nState == STATE_DEAD )	break;
		break;
	} //if

	m_nAdvanceState = nState;

	m_nState = nState;
} //CW3DCharacter::SetState

/**
 * 캐릭터 몸 모델을 로드한다.
 */
BOOL CW3DCharacter::LoadBody( SW3DCharacterInfo* pInfoCharacter )
{
	if( !pInfoCharacter )
	{
		Assert( !"NULL" );
		return FALSE;
	} //if

	if( g_btWarMode )
	{ // 공/수성
		if( LoadOneBody( pInfoCharacter ) )
		{ // 통짜 
			return TRUE;
		} //if
	} //if

	IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();
	CHECK_PTR_RETURN( pItemInfoMgr );

	if( !pInfoCharacter->wUpper )	
		pInfoCharacter->wUpper	= GetNakedBody( EQUIP_UPPERARMOR, m_nSex );
	if( !pInfoCharacter->wLower )	
		pInfoCharacter->wLower	= GetNakedBody( EQUIP_LOWERARMOR, m_nSex );
	if( !pInfoCharacter->wHand )	
		pInfoCharacter->wHand	= GetNakedBody( EQUIP_GLOVE, m_nSex );
	if( !pInfoCharacter->wFoot )	
		pInfoCharacter->wFoot	= GetNakedBody( EQUIP_SHOES, m_nSex );

	SItemInfoData*		pItemInfo[MAX_PARTS];

	pItemInfo[HAIR_PART] = pItemInfoMgr->GetItemInfo( pInfoCharacter->wHair );
	if( !pItemInfo[HAIR_PART] )
	{
		Assert( !"등록되지 않았다" );
		return FALSE;
	} //if

	pItemInfo[FACE_PART] = pItemInfoMgr->GetItemInfo( pInfoCharacter->wFace );
	if( !pItemInfo[FACE_PART] )
	{
		Assert( !"등록되지 않았다" );
		return FALSE;
	} //if

	pItemInfo[UPPER_PART] = pItemInfoMgr->GetItemInfo( pInfoCharacter->wUpper );
	if( !pItemInfo[UPPER_PART] )
	{
		Assert( !"등록되지 않았다" );
		return FALSE;
	} //if

	pItemInfo[LOWER_PART] = pItemInfoMgr->GetItemInfo( pInfoCharacter->wLower );
	if( !pItemInfo[LOWER_PART] )
	{
		Assert( !"등록되지 않았다" );
		return FALSE;
	} //if

	pItemInfo[HAND_PART] = pItemInfoMgr->GetItemInfo( pInfoCharacter->wHand );
	if( !pItemInfo[HAND_PART] )
	{
		Assert( !"등록되지 않았다" );
		return FALSE;
	} //if

	pItemInfo[FOOT_PART] = pItemInfoMgr->GetItemInfo( pInfoCharacter->wFoot );
	if( !pItemInfo[FOOT_PART] )
	{
		Assert( !"등록되지 않았다" );
		return FALSE;
	} //if

	// 운영자 캐릭터는 성별이 2로 되어 있기에 
	// ITEMFILENAME1, ITEMHAVESKIN 매크로에서 모두 false가 된다
	// 그러므로 item.dat파일의 false부분에 해당하는 mod 파일을 넣어주거나
	// true, false( 남자, 여자 ) 두 부분에 모두 넣어가 해야한다.
	
	for( int i = 0 ; i < MAX_PARTS ; i++ )
	{
		char*		pFileName = ITEMFILENAME1( m_nSex, pItemInfo[i] );
		BOOL		bHaveSkin = ITEMHAVESKIN( m_nSex, pItemInfo[i] );
		LoadBodyPart( i, pFileName, bHaveSkin );
	} //for

	return TRUE;
} //CW3DCharacter::LoadBody

/**
 * 캐릭터 부속 모델을 로드한다.
 */
BOOL CW3DCharacter::LoadAccessory( SW3DCharacterInfo* pInfoCharacter )
{
	Assert( pInfoCharacter );

	if( !pInfoCharacter )
	{
		return FALSE;
	} //if

	IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();
	CHECK_PTR_RETURN( pItemInfoMgr );

	SItemInfoData*	pItemInfo = NULL;
	// 근거리 무기
	if( pInfoCharacter->wShortWeapon )
	{
		pItemInfo = pItemInfoMgr->GetItemInfo( pInfoCharacter->wShortWeapon );
		if( !pItemInfo )
		{
			Assert( !"없는 근거리 무기" );
			return FALSE;
		} //if
		char*		pFileName = ITEMFILENAME1( m_nSex, pItemInfo );
		if( !LoadEquipPart( SHORTWEAPON_ITEMPART, pFileName ) )	return FALSE;
		SetWeapon( pItemInfo );
	} //if

	// 원거리 무기
	if( pInfoCharacter->wLongWeapon )
	{
		pItemInfo = pItemInfoMgr->GetItemInfo( pInfoCharacter->wLongWeapon );
		if( !pItemInfo )
		{
			Assert( !"없는 원거리 무기" );
			return FALSE;
		} //if
		char*		pFileName = ITEMFILENAME1( m_nSex, pItemInfo );
		if( !LoadEquipPart( LONGWEAPON_ITEMPART, pFileName ) )	return FALSE;
		SetWeapon( pItemInfo );
	} //if

	// 방패
	if( pInfoCharacter->wShield )
	{
		pItemInfo = pItemInfoMgr->GetItemInfo( pInfoCharacter->wShield );
		if( !pItemInfo )
		{
			Assert( !"없는 방패" );
			return FALSE;
		} //if
		char*		pFileName = ITEMFILENAME1( m_nSex, pItemInfo );
		if( !LoadEquipPart( SHIELD_ITEMPART, pFileName ) )	return FALSE;
	} //if

	// 헬멧
	if( pInfoCharacter->wHelmet )
	{
		pItemInfo = pItemInfoMgr->GetItemInfo( pInfoCharacter->wHelmet );
		if( !pItemInfo )
		{
			Assert( !"없는 헬멧" );
			return FALSE;
		} //if
		char*		pFileName = ITEMFILENAME1( m_nSex, pItemInfo );
		if( !LoadEquipPart( HELMET_ITEMPART, pFileName ) )	return FALSE;
	} //if

	return TRUE;
} //CW3DCharacter::LoadAccessory

/**
 * 캐릭터 모델을 읽는다.
 
IW3DModel* CW3DCharacter::LoadModel( 
	const string&	sPath,		///< [IN]
	const string&	sName,		///< [IN]
	const string&	sType,		///< [IN]
	const BOOL		bAni		///< [IN]
	)
{
	ModelMap::iterator	itor = gs_mapCharacterModel.find( sName );
	if( itor != gs_mapCharacterModel.end() )
	{	// 기존에 읽었던 모델
		IW3DModel*	pModel = itor->second;
		Assert( pModel );

		if( !pModel )
		{
			return NULL;
		} //if

		pModel->AddRef();
		return pModel;
	} //if

	IW3DModel*	pModel = MAKE_COMPONENT( Model );
	CHECK_PTR_RETURN( pModel );

	if( !pModel->Load( sPath.c_str(), ( sType + "/" + sName ).c_str() ) )
	{
		char	szFile[128];
		sprintf( szFile, "%s - 캐릭터 모델 로딩 실패", sName.c_str() );
		Assert( !szFile );
		SAFE_RELEASE( pModel );
		return NULL;
	} //if

	gs_mapCharacterModel.insert( std::make_pair( sName, pModel ) );

	DEBUG_OUT( "Load CHARACTER Model - " + sName.c_str() );

	return pModel;
} //CW3DCharacter::LoadModel
*/

IW3DModel*	CW3DCharacter::LoadModel( 
	const char*		szPath,		///< [IN]
	const string&	sName,		///< [IN]
	const char*		szType		///< [IN]
	)
{
	ModelMap::iterator	itor = gs_mapCharacterModel.find( sName );
	if( itor != gs_mapCharacterModel.end() )
	{	// 기존에 읽었던 모델
		IW3DModel*	pModel = itor->second;

		if( !pModel )
		{
			return NULL;
		} //if

		pModel->AddRef();
		return pModel;
	} //if

	IW3DModel*	pModel = MAKE_COMPONENT( Model );
	CHECK_PTR_RETURN( pModel );

	char szBuffer[128];
	sprintf( szBuffer, "%s/%s", szType, sName.c_str() );
	//sName = szType + sName;

	if( !pModel->Load( szPath, szBuffer ) )
	{
		char	szFile[128];
		sprintf( szFile, "%s - 캐릭터 모델 로딩 실패", sName.c_str() );
		Assert( !szFile );
		SAFE_RELEASE( pModel );
		return NULL;
	} //if

	gs_mapCharacterModel.insert( std::make_pair( sName, pModel ) );

	DEBUG_OUT( "Load CHARACTER Model - " + sName.c_str() );

	return pModel;
} //CW3DCharacter::LoadModel

/**
 * 캐릭터 모델을 제거한다. 
 */
void CW3DCharacter::UnloadModel(
	IW3DModel**		ppModel,	///< [IN/OUT]
	const string&	sName		///< [IN]
	)
{
	if( *ppModel )
	{
		IW3DModel*	pModel = *ppModel;

		Assert( pModel->GetRefCount() >= 1 );

		if( pModel->GetRefCount() <= 1 )
		{
			ModelMap::iterator	itor = gs_mapCharacterModel.find( sName );
			if( itor != gs_mapCharacterModel.end() )
			{
				DEBUG_OUT( "Remove Character Model - " + sName.c_str() );
				gs_mapCharacterModel.erase( itor );
			}
			else
			{
				Assert( !"모델 리스트에 없을 수가 없다. 내부 로직에 문제 있을 가능성이 큼." );
			} //if..else..
		} //if
	} //if

	SAFE_RELEASE( *ppModel );

//	g_BodyMgr.Unload( "High", sName, ppModel );
} //CW3DCharacter::UnloadModel

/**
 * 근거리 무기를 출력한다.
 */
void CW3DCharacter::DrawShortWeapon()
{
	if( !m_apItemModel[SHORTWEAPON_ITEMPART] )		return;
	if( !m_apModel[UPPER_PART] )					return;

	Assert( m_pWorld );

	Matrix4		matWorld;
	matWorld.SetWorld( m_vPos, m_qRot );
	if( m_pActionMgr->IsWeaponAction() && m_nState != STATE_SKILL )
	{
		m_apItemModel[SHORTWEAPON_ITEMPART]->SetAction( m_pActionMgr->GetWeaponAction() );
	}
	else
	{		
		Matrix4		matBone	= m_apModel[UPPER_PART]->GetBoneMatrix( "Bip01 R Finger0", m_dwTick );
		matWorld = matBone * matWorld;
		
		m_apItemModel[SHORTWEAPON_ITEMPART]->SetAction( NULL );
	} //if..else..

	m_apItemModel[SHORTWEAPON_ITEMPART]->SetWorld( matWorld );
	m_apItemModel[SHORTWEAPON_ITEMPART]->SetColor( m_dwColor );
	m_apItemModel[SHORTWEAPON_ITEMPART]->Render( m_dwTick );
	
	Vector3		vPoint[2];

	vPoint[0] = Vector3( 0.9F,  -0.2F, 0.0F );
	vPoint[1] = Vector3( 0.3F, -0.1F, 0.0F );

	if( m_bTrace )
	{
		IW3DTraceEffect*	pTraceEffect = m_pWorld->GetTraceEffect();
		Assert( pTraceEffect );
		
		if( pTraceEffect )
		{
			if( m_pTrace )
			{
				pTraceEffect->InsertPoint( m_dwId, m_pTrace->vP[0] * matWorld, m_pTrace->vP[1] * matWorld );
			}
			else
			{
				pTraceEffect->InsertPoint( m_dwId, vPoint[0] * matWorld, vPoint[1] * matWorld );
			} //if..else..

			if( m_pTrace->pBillboard )
			{
			//	m_pTrace->pBillboard->SetLocal( matWorld );
			//	m_pTrace->pBillboard->Draw( Vector3( 0.0F, 0.0F, 0.0F ) );
			} //if
		} //if
	} //if
} //CW3DCharacter::DrawShortWeapon

/**
 * 원거리 무기를 출력한다.
 */
void CW3DCharacter::DrawLongWeapon()
{
	if( !m_apItemModel[LONGWEAPON_ITEMPART] )		return;
	if( !m_apModel[UPPER_PART] )					return;


	Matrix4		matWorld;
	Matrix4		matBone	= m_apModel[UPPER_PART]->GetBoneMatrix( "Bip01 L Finger0", m_dwTick );

	matWorld.SetWorld( m_vPos, m_qRot );
	matWorld = matBone * matWorld;

	m_apItemModel[LONGWEAPON_ITEMPART]->SetAction( NULL );

	m_apItemModel[LONGWEAPON_ITEMPART]->SetWorld( matWorld );
	m_apItemModel[LONGWEAPON_ITEMPART]->SetColor( m_dwColor );
	m_apItemModel[LONGWEAPON_ITEMPART]->Render( m_dwTick );
} //CW3DCharacter::DrawLongWeapon

/**
 * 방패를 출력한다.
 */
void CW3DCharacter::DrawShield()
{
	if( !m_apItemModel[SHIELD_ITEMPART] )			return;
	if( !m_apModel[UPPER_PART] )					return;
	if( m_nState == STATE_SKILL )					return;

	Matrix4		matBone;
	Matrix4		matWorld;

	if( m_pActionMgr->IsPeaceMode() )
	{
		matBone	= m_apModel[UPPER_PART]->GetBoneMatrix( "Bip01 Spine1", m_dwTick );
	}
	else
	{
		matBone = m_apModel[UPPER_PART]->GetBoneMatrix( "Bip01 L Finger0", m_dwTick );
	} //if..else..

	matWorld.SetWorld( m_vPos, m_qRot );
	matWorld = matBone * matWorld;

	m_apItemModel[SHIELD_ITEMPART]->SetWorld( matWorld );
	m_apItemModel[SHIELD_ITEMPART]->SetColor( m_dwColor );
	m_apItemModel[SHIELD_ITEMPART]->Render( m_dwTick );
} //CW3DCharacter::DrawShield

/**
 * 애니메이션을 설정한다.
 *
 * @param	sAction		[IN] 사용할 동작 이름
 */
void CW3DCharacter::SetAction( const WORD wAction )
{
	IW3DTraceEffect*	pTraceEffect;

	switch( wAction )
	{
	case ACTION_STOP:
		m_bTrace = FALSE;

		pTraceEffect = m_pWorld->GetTraceEffect();
		Assert( pTraceEffect );
		if( pTraceEffect )
			pTraceEffect->PauseEffect( m_dwId );
		break;

	case ACTION_ATTACK:
		m_bTrace = TRUE;
		break;
	case ACTION_MOVE:
		m_bTrace = FALSE;
		break;

	case ACTION_DEAD:
		m_bTrace = FALSE;
		break;
	} //switch
} //CW3DCharacter::SetAction

/**
 * 현재 상태의 동작이 마지막인지 확인한다.
 *
 * @param	dwTick		[IN] 경과 tick
 */
BOOL CW3DCharacter::IsEndAction( const DWORD dwTick )
{
	m_bIsEndAction = m_pActionMgr->IsEnd();

/*	if( m_bIsEndAction )
	{
		Stop();
	} //if*/

	return m_bIsEndAction;
} //CW3DCharacter::IsEndAction

/**
 * 캐릭터 이동.
 */
BOOL CW3DCharacter::StateMovement( const DWORD dwTick )
{
	Assert( m_pWorld );

	if( IsDead() )	return FALSE;

	SetLayer();

	Vector3 vPos;
	Vector3 vDir;
	vDir = m_vGoalPos - m_vPos;

	if( vDir.Length() <= 0.1F )
	//if( IsGoal() )
	{	// 목적지까지 도달했을 때
		m_vPos = m_vGoalPos;
		if( m_dwFollowTarget > 0 )
		{
			IW3DCreature	*pFollow = m_pWorld->GetCharacter( m_dwFollowTarget );
			if( pFollow )
			{
				if( ( m_vPos - pFollow->GetPosition() ).Length() > 5.0F )
				{
					MoveTo( pFollow->GetPosition() );
				} //if
			} //if
		} //if

		if( m_aPath.empty() )
		{	// 최종 목적지에 도착했을 때
			
		/*	if( m_vLastGoal != m_vPos )
			{
				SetAutoPath();
				return TRUE;
			} //if*/
			
			m_vPos.z = GetHeight();
			
			if( STATE_STOP != m_nState )
			{
				Stop();
			} //if

			m_vLastGoal = m_vPos;
			return TRUE;
		}
		else
		{	// 다음 방향을 결정한다.
			vPos = m_aPath[0];
			vPos.z = m_pWorld->GetHeight( vPos );
			
			SetGoalPosition( vPos );

			m_aPath.erase( m_aPath.begin() );
		} //if..else..
	} //if

	//if( m_nLayer != s_nLayer )
	{
	//	SetLayer( m_pWorld->GetTerrain()->GetLevel() );
	//	SetLayer();

	//	SetAutoPath();
	} //if

	vDir = m_vGoalPos - m_vPos;
	
	float		fOffset = m_fOffset * dwTick / 1000;
	float		fLength = vDir.Length();
	vDir.Normalize();

	if( fOffset > fLength )
	{
		fOffset = fLength;
	} //if

	vPos = m_vPos + vDir * fOffset;
	vPos.z = m_pWorld->GetHeight( vPos );
	
	static int s_nLayer = 0;
	s_nLayer = m_pWorld->GetTerrain()->GetLevel();
	
	if( m_dwId == g_dwPlayerId )
	{		
		SetLayer( s_nLayer );
		m_pWorld->SetLayer( s_nLayer );
		if( !m_pWorld->IsMovable( vPos.x, vPos.y )  )
		{ // 갈 수 없는  길이거늘...			
			if( m_bAutoPath )
			{
				SetAutoPath();
			} 
			else
			{
				m_vLastGoal = m_vGoalPos = m_vPos;
				Stop();
			} //if..else..

			return TRUE;
		}
		else if( g_btWarMode )
		{
			if( m_pWorld->CollisionWithCreature( this, vPos, m_vPos ) ||
				m_pWorld->CollisionWithEntity( vPos, m_vPos) )
			{
				m_vLastGoal = m_vGoalPos = m_vPos;
				return TRUE;
			} //if
		} //if..else
	}
	else
	{
		SetLayer( s_nLayer );
		SetLayer();
	} //if	
	
	float		fZ = vPos.z - m_vPos.z;

	Vector3		vDistance = vPos - m_vPos;
	vDistance.z = 0.0F;

	float		fDistance = vDistance.Length();

	if( fDistance > MATH_EPSILON || fDistance > 3.0F )
	{
		m_vPos = vPos;
		m_qRot = vDir.GetQuaternion();
	}
	else
	{
		m_vPos = vPos;
		//m_qRot = vDir.GetQuaternion();
	} //if

	if( m_vPos.z < m_pWorld->GetWaterHeight() )
	{
		for( int i = 0 ; i < random( 10 ) - 2 ; i++ )
		{
			vPos = Vector3( 0.0F, 0.0F, 0.0F );
			vPos.x = random( 100 ) / 200.0F;
			vPos.RotateY( ANGLE( random( 360 ) ) );

			vPos  += m_vPos;
			vPos.z = m_pWorld->GetWaterHeight();

			m_pWorld->AddEffect( "pamoon.wed", vPos, m_qRot );
		} //for
	} //if

	m_pSound->SetPosition( m_vPos );

	m_pActionMgr->Run();
	SetAction( ACTION_MOVE );

	m_pActionMgr->Run();
	
	return TRUE;
} //CW3DCharacter::StateMovement

/*
*		switch( CCreature::GetState() )
{
case STATE_DEAD:
IsEndAction( m_dwTick + dwTick );
bLive = StateDead( dwTick );
break;

  case STATE_ATTACK:
		ApplySkill();
		IsEndAction( m_dwTick + dwTick );
		StateAttack( dwTick, 1 );
		break;
		
		  case STATE_MAGIC:
		  ApplySkill();
		  IsEndAction( m_dwTick + dwTick );
		  StateMagic( dwTick );
		  break;
		  
			case STATE_SKILL:
			ApplySkill();
			IsEndAction( m_dwTick + dwTick );
			StateSkill( dwTick );
			break;
			
			  case STATE_MOVE:
			  ApplySkill();
			  m_bIsEndAction = m_pActionMgr->IsEnd();
			  StateMovement( dwTick );
			  //ApplySkill();
			  break;
			  
				case STATE_STOP:
				ApplySkill();
				if( m_vLastGoal != m_vPos )  //!IsGoal()
				{
				m_pActionMgr->Run();
				} //if
				//ApplySkill();
				break;
				
				  case STATE_FISHING:
				  break;
				  
					default:
					ApplySkill();
					m_bIsEndAction = m_pActionMgr->IsEnd();
					StateMovement( dwTick );
					//ApplySkill();
					break;
					} //switch
 */

/**
 * 죽은 상태.
 */
BOOL CW3DCharacter::StateDead( const DWORD dwTick )
{
	Assert( m_pWorld );
 	Assert( m_apModel[UPPER_PART] );

	//if( IsEndAction( m_dwTick + dwTick ) )
	if( m_bIsEndAction )
	{
		m_pWorld->AddEffect( "die.wed", m_vPos, m_qRot );
		return FALSE;
	} //if

	return TRUE;
} //CW3DCharacter::StateDead

/**
 * 공격.
 *
 * @param	dwTick		[IN] 경과 tick
 * @param	nIndex		[IN] 공격형태
 */
BOOL CW3DCharacter::StateAttack( const DWORD dwTick, const int nIndex )
{
	Assert( m_pWorld );

	if( STATE_DEAD == m_nState )	return FALSE;
	if( m_bStateLock )				return TRUE;

	IW3DCreature*	pTarget = NULL;

	DWORD	dwCurTick = m_dwTick + dwTick;
	DWORD	dwAttackTick;

	switch( m_nSkill )
	{	
	case SKILL_SHORT:// 근거리 공격
		dwAttackTick = ( 22 * 1000 / 30 );

		if( ( m_dwTick  <= dwAttackTick ) &&
			( dwCurTick >  dwAttackTick ) &&
			!m_bAttack )
		{
			pTarget = GetTargetCreature();
			if( pTarget )
			{
				pTarget->SetTarget( m_bPlayer, TRUE );
				//pMonster->Damage( TRUE );
			} //if
			m_bAttack = TRUE;
		} //if
		break;
		
	case SKILL_LONG:	// 원거리 공격
		dwAttackTick = 1000;

		if( m_dwTick <= dwAttackTick && 
			dwCurTick > dwAttackTick &&
			!m_bAttack )
		{
			pTarget = GetTargetCreature();
			if( pTarget )
			{
			//	m_pSound->SetPosition( m_vPos );
			//	m_pSound->Play3D( "bow100047.wav" );
				m_pWorld->AddMagic( m_sArrow.c_str(), this, pTarget );
			} //if

			m_bAttack = TRUE;
		} //if
		break;

	default:
		break;
	} //if..else if..else..

	if( m_bIsEndAction )
	{
		if( m_bPlayer )
		{	// 사용자의 캐릭터인 경우, 공격 완료 패킷을 서버로 보낸다.
			if( !m_bSemiMove )
			{
				if( ( m_nSkill == SKILL_SHORT ) && m_nChangedSkill < 0 )
				{ // 근거리에서만 보낸다.
				  // 마법의(홀쏘기) 경우 Creature의 EndMagic에서 보낸다.

#ifdef dCLIENT_AUTO_ATTACK
					SendPacket_Attack();
#else
					CPacket		sendPacket;
					sendPacket.Tag( dPACKET_END_PLAYER_ATTACK );
					g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
#endif
				} //if
				if( ( m_nSkill == SKILL_LONG ) && m_nChangedSkill < 0 )
				{ // 근거리에서만 보낸다.
					// 마법의(홀쏘기) 경우 Creature의 EndMagic에서 보낸다.
					
#ifdef dCLIENT_AUTO_ATTACK
					SendPacket_Attack();
#else
					CPacket		sendPacket;
					sendPacket.Tag( dPACKET_END_PLAYER_ATTACK );
					g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
#endif
				} //if

			} //if
		} //if

		m_vPos.z = m_pWorld->GetHeight( m_vPos );

	//	SetGoalPosition( m_vPos );
	//	Stop();
		m_pActionMgr->CompulsionStop();

		m_bAttack = FALSE;
	} //if

	return TRUE;
} //CW3DCharacter::StateAttack

/**
 *
 */
BOOL CW3DCharacter::StateSkill( const DWORD dwTick )
{
	if( STATE_DEAD == m_nState )	return FALSE;

	if( m_pActionMgr->GetState() != STATE_MAGIC )
	{
		return FALSE;
	} //if

	if( m_bIsEndAction )
	{
		m_pActionMgr->Stop();
		Stop();
	} //if..else..

	return TRUE;
} //CW3DCharacter::StateSkill

/**
 * 마법 상태
 */
BOOL CW3DCharacter::StateMagic( const DWORD dwTick )
{
	if( STATE_DEAD == m_nState )	return FALSE;

	if( m_pActionMgr->GetState() != STATE_MAGIC )
	{
		return FALSE;
	} //if

	m_dwMagicTick += dwTick;
	
	if( m_dwMagicTick >= 1000 )
	{
		if( m_MagicData.bCast )
		{
			DEBUG_OUT( "*시전이 완료" );
			if( m_bPlayer )
			{ // 플레이어라면, 서버에 패킷을 보낸다.
				SendPacket_BeginMagic();
			} //if
					
			m_MagicData.bCast	= FALSE;

			if( STATE_MAGIC == m_nState )
			{
				m_pActionMgr->Stop(); 
				Stop();
				m_pActionMgr->Compulsion();
			}
		 	
			if( g_pGUI->GetCheck() == TRUE )
			{
				g_pGUI->UseSkill2( (BYTE)g_pGUI->GetSaveQuickSlot() );
				g_pGUI->SetAttackMonster( TRUE );
			//	Sleep( 1500 );
				g_pGUI->SetCheck( FALSE );
			}
			else
			{
				g_pGUI->SetAttackMonster( FALSE );
			}
			//
		} //if
	}

	if( m_bIsEndAction )//|| m_dwMagicTick >= 1500 )
	{
/*		if( m_MagicData.bCast )
		{
			DEBUG_OUT( "*시전이 완료" );
			if( m_bPlayer )
			{ // 플레이어라면, 서버에 패킷을 보낸다.
				SendPacket_BeginMagic();
			} //if
					
			m_MagicData.bCast	= FALSE;
		} //if*/
		
		if( m_bIsEndAction )
		{
			m_pActionMgr->Stop();
			Stop();
			m_pActionMgr->Compulsion();
		} //if..else..
		m_dwMagicTick = 0;
	} //if

	return TRUE;
} //CW3DCharacter::StateMagic

/**
 * 마법이 끝났다.
 */
void CW3DCharacter::EndMagic( DWORD dwID )
{
	//m_pMagic = NULL;
//	CCreature::EndMagic( dwID );
	
	if( m_bPlayer && !m_MagicData.bCast )
	{
	//	if( m_pActionMgr->IsLongAttack() )
		{
			m_pActionMgr->ShowDamageOfTarget();
			m_pActionMgr->ClearTargets();
		} //if

		if( !m_bSemiMove )
		{
			if( m_pActionMgr->IsLongAttack() && m_nChangedSkill < 0 )
			{
#ifdef dCLIENT_AUTO_ATTACK
//				SendPacket_Attack();
#else
				CPacket		sendPacket;
				sendPacket.Tag( dPACKET_END_PLAYER_ATTACK );
				g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
#endif
			}
			else
			{
				MagicMap::iterator itor = m_mapOwnerMagic.find( dwID );

				if( itor != m_mapOwnerMagic.end() )
				{
					IW3DMagic* pMagic = itor->second;

/*					if( pMagic )
					{
						if( pMagic->IsSendEndAttack() )
						{
							g_sTarget.Clean();
						}
					}
					else
					{
						g_sTarget.Clean();
					}//if
*/				} //if

				
			} //if..else..
			m_vPos.z = m_pWorld->GetHeight( m_vPos );
		//	SetGoalPosition( m_vPos );
		}
		else
		{	
			Move();
		}//if
	} //if

	CCreature::EndMagic( dwID );
} //CW3DCharacter::EndMagic

/**
 * 용병 설정.
 */
void CW3DCharacter::SetHelper( const IW3DCreature* pCreature )
{
	Assert( pCreature );

	if( !pCreature )
	{
		return ;
	} //if

//	SAFE_RELEASE( m_pHelper );
//	m_pHelper = const_cast<IW3DCreature*>( pCreature );
} //CW3DCharacter::SetHelper

/**
 * 용병 제거.
 */
void CW3DCharacter::RemoveHelper()
{
//	SAFE_RELEASE( m_pHelper );
} //CW3DCharacter::RemoveHelper

/**
 *
 */
void CW3DCharacter::AddAttacker( IW3DCreature* pAttacker )
{
	Assert( pAttacker );

	if( pAttacker )
	{
	//	m_mapAttackers.insert( std::make_pair( pAttacker->GetId(), pAttacker ) );
	
		CreatureMap::iterator itor = m_mapAttackers.find( pAttacker->GetId() );

		if( itor != m_mapAttackers.end() )
		{
			m_mapAttackers.erase( itor );
		} //if

		m_mapAttackers.insert( std::make_pair( pAttacker->GetId(), pAttacker ) );
	} //if

} //CW3DCharacter::AddAttacker

/**
 *
 */
void CW3DCharacter::RemoveAttacker( IW3DCreature* pAttacker )
{
	if( !pAttacker )
	{
		return ;
	} //if

	CreatureMap::iterator itor = m_mapAttackers.find( pAttacker->GetId() );

	if( itor != m_mapAttackers.end() )
	{
		// 있다.!!!
		m_mapAttackers.erase( itor );
	} 
	else
	{
		Assert( !"Remove Attackers : None " );
	} //if
} //CW3DCharacter::RemoveAttacker

/**
 *
 */
void CW3DCharacter::AddTarget( IW3DCreature* pTarget )
{
	Assert( pTarget );

	if( pTarget )
	{
		pTarget->Damage( TRUE );

		CreatureMap::iterator itor = m_mapTargets.find( pTarget->GetId() );

		if( itor != m_mapTargets.end() )
		{
			m_mapTargets.erase( itor );
		} //if

		m_mapTargets.insert( std::make_pair( pTarget->GetId(), pTarget ) );
		m_pActionMgr->AddTarget( pTarget );

#ifdef _DEBUG
		int nSize = m_mapTargets.size();
#endif //#ifdef _DEBUG
	} //if
} //CW3DCharacter::AddTarget

/**
 *
 */
void CW3DCharacter::RemoveTarget( IW3DCreature* pTarget )
{
	if( !pTarget )
	{
		return ;
	} //if

	CreatureMap::iterator itor = m_mapTargets.find( pTarget->GetId() );

	if( itor != m_mapTargets.end() )
	{
		// 있다.!!!
		m_mapTargets.erase( itor );
	} 
	else
	{
		Assert( !"RemoveTarget : Target Creature None" );
	} //if

	m_pActionMgr->RemoveTarget( pTarget );

} //CW3DCharacter::RemoveTarget

/**
 *
 */
void CW3DCharacter::ClearAttackers()
{
	if( m_mapAttackers.empty() )
	{
		return ;
	} //if

	CreatureMap::iterator itor = m_mapAttackers.begin();
	CreatureMap::iterator itorNext;

	while( itor != m_mapAttackers.end() )
	{
		itorNext = itor;
		itorNext++;

		IW3DCreature* pCreature = itor->second;

		if( pCreature )
		{
			pCreature->RemoveTarget( this );
		} //if

		m_mapAttackers.erase( itor );
		itor = itorNext;
	} //while
} //CW3DCharacter::ClearAttackers

/**
 *
 */
void CW3DCharacter::ClearTargets()
{
	if( m_mapTargets.empty() )
	{
		return ;
	} //if

	CreatureMap::iterator itor = m_mapTargets.begin();
	CreatureMap::iterator itorNext;

	while( itor != m_mapTargets.end() )
	{
		itorNext = itor;
		itorNext++;

		IW3DCreature* pCreature = itor->second;

		if( pCreature )
		{
			pCreature->RemoveAttacker( this );
		} //if

		m_mapTargets.erase( itor );
		itor = itorNext;
	} //while
} //CW3DCharacter::ClearTargets

/**
 * 마법시작.
 */
void CW3DCharacter::StartMagic( DWORD dwID, DWORD dwMagicId, BOOL bTargetMonster, BOOL bHero )
{
	if( !m_bEndMagic )
	{
		return ;
	} //if

	m_bEndMagic		= FALSE;
	m_dwMagicTick	= 0;

	if( dwMagicId > 0 && !m_bUseVehicle )
	{
		m_MagicData.dwId				= dwMagicId;
		m_MagicData.dwTargetId			= dwID;
		m_MagicData.bCast				= TRUE;
		m_MagicData.bIsTargetMonster	= bTargetMonster;

		if( bHero )
		{
			CPacket		sendPacket;
			sendPacket.Tag( dPAKCET_BEGIN_CASTMAGIC )
					<< m_MagicData.dwTargetId
					<< (BYTE)m_MagicData.bIsTargetMonster;
			g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
		} //if
	} //if
} //CW3DCharacter::StartMagic

/**
 * 스킬사용 방법이 맞는지 체크하고 사용한다.
 */
BOOL CW3DCharacter::CheckUsageSkill( IW3DCreature* pTarget )
{
	BYTE		btSkillType;
	BYTE		btTargetType;

	if( !CanAttack() )
	{ //우선은 타고 있다면 근거리만 가능하게... (담에수정..)
		return FALSE;
	} //if

	if( GetApplyInfoSkill( &btSkillType, &btTargetType ) )
	{ // 사용중인 스킬이 있을때...
		BOOL	bIsMonster;
		if( pTarget->GetType() == MONSTER )		bIsMonster = TRUE;
		else									bIsMonster = FALSE;

		if( btTargetType == 0 )
		{ //상대에게 적용하는 스킬.
			if( pTarget->GetType() != CHARACTER ||
				pTarget->GetId() != m_dwId )
			{ //몬스터나 다른 케릭터에게 적용.
				TargetSkill( pTarget->GetId(), btSkillType, bIsMonster );
				return TRUE;
			} //if
		}
		else if( btTargetType == 1 )
		{ //자신에게 적용하는 스킬.
			if( pTarget->GetType() == CHARACTER && 
				pTarget->GetId() == m_dwId )
			{ // 스킬 자신에게 적용.
				SelfSkill( btSkillType );
				return TRUE;
			} //if
		}
		else if( btTargetType == 2 )
		{ // 모두에게 적용하는 스킬.
			if( pTarget->GetType() == CHARACTER && 
				pTarget->GetId() == m_dwId )
			{ // 스킬 자신에게 적용.
				SelfSkill( btSkillType );
			}
			else
			{
				TargetSkill( pTarget->GetId(), btSkillType, bIsMonster );
			} //if..else
			return TRUE;
		} //if..else..
	} //if

	return FALSE;
} //CW3DCharacter::CheckUsageSkill

/**
 * 자신에게 사용하는 스킬사용.
 */
void CW3DCharacter::SelfSkill( const BYTE btSkillType )
{
	StartMagic( m_dwId, 1, FALSE, TRUE );
} //CW3DCharacter::SelfSkill

/**
 * 상대에게 사용하는 스킬사용.
 */
void CW3DCharacter::TargetSkill( const DWORD dwTargetId, const BYTE btSkillType, BOOL bIsMonster )
{
	if( m_nSkill == SKILL_WHITE || m_nSkill == SKILL_BLACK )
	{ //마법 스킬.
		if( btSkillType == 0 )
		{ //일반매직.
			StartMagic( dwTargetId, 1, bIsMonster, TRUE );
		}
		else if( btSkillType == 1 )
		{ //캐스팅 매직.
			StartMagic( dwTargetId, 1, bIsMonster, TRUE );
		} //if..else
	}
	else
	{ //근,원거리

		if( m_nSkill == SKILL_LONG )
		{
			m_pActionMgr->SetLongAttack( TRUE );
		}
		else
		{
			m_pActionMgr->SetLongAttack( FALSE );
		} //if..else..

		m_pActionMgr->Attack();
	} //if..else

} //CW3DCharacter::TargetSkill

/**
 * 시전 다음 바로.
 */
void CW3DCharacter::SendPacket_BeginMagic()
{
	CPacket		sendPacket;
	
	sendPacket.Tag( dPACKET_MAGIC_CAST )
				<< m_MagicData.dwTargetId
				<< m_MagicData.bIsTargetMonster;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CW3DCharacter::SendPacket_BeginMagic

/**
 *
 */
BOOL CW3DCharacter::CastingMagic( DWORD dwID, DWORD dwMagicId )
{
//	m_pState->BeginMagic();
//	SetState( STATE_MAGIC );
	BYTE		btSkillType;
	BYTE		btTargetType;

	if( !GetApplyInfoSkill( &btSkillType, &btTargetType ) )
	{ // 사용중인 스킬이 있을때...

	} //if

	IW3DCreature*	pTarget = GetTargetCreature();
	if( pTarget )
	{
		pTarget->Damage( TRUE );		///< 한대 때리기 시작.
	} //if

	m_MagicData.bCast				= TRUE;
	m_MagicData.dwId				= dwMagicId;
	m_MagicData.dwTargetId			= dwID;
	m_bEndMagic						= FALSE;

	return TRUE;
} //CW3DCharacter::CastingMagic

/**
 * 활살 정보 설정.
 */
void CW3DCharacter::SetArrowInfo( const int nBowIndex )
{
	m_sArrow = "단궁기본";
/*	switch( nBowIndex )
	{
	case 901:	case 902:	case 903:	default:
 		m_sArrow = "기본화살";
		break;

	case 904:	case 905:	case 906:	case 907:	case 908:	case 909:
		m_sArrow = "기본화살2연타";
		break;

	case 910:	case 911:	case 912:	case 913:	case 914:	case 915:	case 916:	case 917:
		m_sArrow = "푸른화살2연타";
		break;

	case 923:	case 924:	case 925:
		m_sArrow = "기본화살3연타";
		break;
	} //switch
*/
} //CW3DCharacter::SetArrowInfo

/**
 * 스킬을 사용한다
 */
void CW3DCharacter::UseSkill( WORD wSkillId )
{
	if( m_nState == STATE_SKILL )
	{
		g_pGUI->ChattingInput( "", STR_SKILL_ALREADY, FONTCOLOR_REF );
		return;
	} //if


	if( m_bNotUseSkill )
	{
		g_pGUI->ChattingInput( "", STR_SKILL_ERROR, FONTCOLOR_REF );
		return ;
	} //if

//	if( m_nSkill >= 2 )
//	{
//		g_pGUI->ChattingInput( "", STR_SKILL_CHANGESHORTORLONG, FONTCOLOR_REF );
//		return;
//	} //if

	IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();
	if( !pItemInfoMgr )
	{
		Assert( pItemInfoMgr );
		return;
	} //if

	SItemInfoData*		pItemInfoDat = pItemInfoMgr->GetItemInfo( wSkillId );
	if( !pItemInfoDat )
	{
		Assert( !"없는 아이템" );
		m_bNotUseSkill = TRUE;
		return;
	} //if
	
	SSkillInfoData*		pSkillDat = pItemInfoDat->pSkillData;
	if( !pSkillDat )
	{
		Assert( !"없는 스킬 데이터" );
		m_bNotUseSkill = TRUE;
		return;
	} //if
	
	IW3DCreature*	pTarget = g_pSelectMgr->GetCreature();
	if( !pTarget || pTarget->GetType() == NPC )
	{
		if( pSkillDat->btTargetType == 0 || pSkillDat->btTargetType == 1 )
			pTarget = g_pPlayer;
		else
		{
			g_pGUI->ChattingInput( "", STR_SKILL_NOTFOUNDTARGET, FONTCOLOR_REF );
			return;
		}
	} //if

	if( pSkillDat->btTargetType == 0 )
	{ // 상대에게 적용하는 스킬
		if( pTarget->GetType() == CHARACTER &&
			pTarget->GetId() == m_dwId )
		{
			g_pGUI->ChattingInput( "", STR_SKILL_NOTUSINGTARGET, FONTCOLOR_REF );			
			return;
		} //if
	}
	else if( pSkillDat->btTargetType == 1 )
	{ // 자신에게 적용하는 스킬.
		if( pTarget->GetType() != CHARACTER || 
			pTarget->GetId() != m_dwId )
		{
			g_pGUI->ChattingInput( "", STR_SKILL_NOTUSINGTARGET, FONTCOLOR_REF );			
			return;
		} //if
	} //if..else

	char*		szSkillName = pItemInfoDat->szEName;

	int nWeapon = 0;
	switch( m_nSkill )
	{
	case SKILL_SHORT:
		nWeapon = m_pActionMgr->GetWeaponId( m_nShortWeapon );
		break;
	case SKILL_LONG:
		nWeapon = m_pActionMgr->GetWeaponId( m_nLongWeapon );
		break;
	default:
		nWeapon = m_pActionMgr->GetWeaponId( m_nShortWeapon );
		break;
	}; //switch
	
//	if( !g_mgrSkill.IsUseSkill( szSkillName, nWeapon ) )
//	{
//		g_pGUI->ChattingInput( "", STR_SKILL_NOTUSINGWEAPON, FONTCOLOR_REF );
//		return ;
//	} //if

	DWORD		dwTargetId	 = pTarget->GetId();
	BYTE		btTargetType = ( pTarget->GetType() == MONSTER ) ? 1 : 0;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_USE_SKILL )
		<< wSkillId
		<< dwTargetId
		<< btTargetType;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CW3DCharacter::UseSkill

/**
 *
 */
void CW3DCharacter::AddSkill( IW3DCreature* pTarget, const WORD wSkillId )
{
	Assert( pTarget );

	IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();
	if( !pItemInfoMgr )
	{
		Assert( pItemInfoMgr );
		return;
	} //if

	SItemInfoData*		pItemInfoDat = pItemInfoMgr->GetItemInfo( wSkillId );
	if( !pItemInfoDat )
	{
		Assert( !"없는 아이템(스킬)" );
		m_bNotUseSkill = TRUE;
		return;
	} //if

	char*		szSkillName = pItemInfoDat->szEName;
	SAction* pAction = g_mgrSkill.Add( this, pTarget, szSkillName, m_nSex );

	if( pAction )
	{
		m_pActionMgr->Skill( pAction );
	} 
	else
	{
		m_bNotUseSkill = TRUE;
	} //if

	Vector3		vDir = pTarget->GetPosition() - m_vPos;

	if( m_vPos == pTarget->GetPosition() )
	{
		vDir = Vector3( 0.0F, 1.0F, 0.0F );
	} //if

	SetDirection( vDir );
} //CW3DCharacter::AddSkill

/**
 * 이름 찍기.
 */
void CW3DCharacter::DrawBaseInfo()
{
	Vector3			vDir = g_pPlayer->GetPosition() - m_vPos;
	if( vDir.Length() > 20.0f )
	{
		return;
	} //if

	Vector3	vSize = m_pBoundBox->GetSize();
	float	fHeight = vSize.y + 0.2F;
	if( m_bUseVehicle )
	{
		fHeight += 1.0f;
	} //if
	m_vTextPos = m_pScene->GetScreenPos( m_vPos + Vector3( 0.0F, 0.0F, fHeight ) );

	int		nTxtWidth1, nTxtHeight1;
	int		nTxtWidth2, nTxtHeight2;
	int		nX, nY;

	m_pScene->GetStringSize( nTxtWidth1, nTxtHeight1, m_sName.c_str() );
	nX = m_vTextPos.x - ( nTxtWidth1 / 2 );//( nTxtWidth1 >> 1 );
	nY = m_vTextPos.y + 1;
	m_pRectangle->Draw( nX-2, nY-2, nTxtWidth1+10, nTxtHeight1+5, COLOR_ARGB( 150, 0,0,0 ) );
	m_pScene->DrawText( nX, nY, COLOR_ARGB(255,173,250,173), m_sName.c_str() );

	int nLenCorpsName = strlen( m_szCorpsName ) ;
	if( nLenCorpsName > 0 )
	{ //Draw the guild info
		m_pScene->GetStringSize( nTxtWidth2, nTxtHeight2, m_szCorpsName );
		nX = m_vTextPos.x - ( nTxtWidth2 >> 1 );
		nY = m_vTextPos.y - nTxtHeight1;
		
		m_pRectangle->Draw( nX-2, nY-6, nTxtWidth2+4, nTxtHeight2+4, COLOR_ARGB( 150, 0,0,0 ) );
		m_pScene->DrawText( nX, nY-4, m_dwGuildTextColor, m_szCorpsName );

		if( m_pEmblem && m_pEmblem->pSprite )
		{
			if( nTxtWidth1 > nTxtWidth2 )	nX = nX - ( nTxtWidth1 >> 1 );
			else							nX = nX - ( nTxtWidth2 >> 1 );
			
			m_pEmblem->pSprite->Draw( nX - 21, nY - 1 );
		} //if
	} //if
} //CW3DCharacter::DrawBaseInfo

/**
 * Draw The Shop Title
 */
void CW3DCharacter::DrawShopTitle()
{
	if( m_szShopTitle[0] == '\0' )
	{
		return;
	} //if

	Vector3	vSize = m_pBoundBox->GetSize();
	float	fHeight = vSize.y + 0.2F;
	if( m_bUseVehicle )
	{
		fHeight += 1.0f;
	} //if
	m_vTextPos = m_pScene->GetScreenPos( m_vPos + Vector3( 0.0F, 0.0F, fHeight ) );

	int		nX = m_vTextPos.x;
	int		nY = m_vTextPos.y;

	nY -= 12;
	if( m_szCorpsName[0] != '\0' )		nY -= 15;

	int		nTxtWidth, nTxtHeight;
	m_pScene->GetStringSize( nTxtWidth, nTxtHeight, m_szShopTitle );
	nX = nX - ( nTxtWidth >> 1 );

	m_pRectangle->Draw( nX-3, nY-10, nTxtWidth+6, nTxtHeight+6, COLOR_ARGB( 255, 150,150,150 ) );
	m_pRectangle->Draw( nX-2, nY-9, nTxtWidth+4, nTxtHeight+4, 0xFF000000 );
	m_pScene->DrawText( nX, nY-7, nTxtWidth, nTxtHeight, 0xFF00FFFF, m_szShopTitle );
} //CW3DCharacter::DrawShopTitle

/**
 * 쳇 내용 출력.
 */
void CW3DCharacter::DrawChattText()
{
	if( m_dwTalkTime < TICK_SHOWTEXT )
	{
		Vector3	vSize = m_pBoundBox->GetSize();
		float	fHeight = vSize.y + 0.2F;

		if( m_bUseVehicle )
		{
			fHeight += 1.0F;
		} //if
		m_vTextPos = m_pScene->GetScreenPos( m_vPos + Vector3( 0.0F, 0.0F, fHeight ) );

		int		nX = m_vTextPos.x;
		int		nY = m_vTextPos.y;

		if( m_szCorpsName[0] != '\0' )		nY -= 12;
		if( m_szShopTitle[0] != '\0' )		nY -= 18;

		nX -= ( m_nTalkTxtWidth >> 1 );
		nY -= m_nTalkTxtHeight + 3;

		m_pRectangle->Draw( nX-2, nY-8, m_nTalkTxtWidth+4, m_nTalkTxtHeight+4, COLOR_ARGB( 200, 241, 241, 241 ) );
		m_pScene->DrawText( nX, nY-6, m_nTalkTxtWidth, m_nTalkTxtHeight, COLOR_ARGB( 255, 3, 3, 3 ), m_sTalk.c_str() );
	} //if
} //CW3DCharacter::DrawChattText

/**
 *
 */
BOOL CW3DCharacter::CollisionCreature( Vector3 vPos )
{
/*	IW3DCreature* pCreature = NULL;
	LIST_CREATURE::iterator itor = g_listCollisionCreature.begin();

	float fRadius = 0.5F;
	float fDist = 0.0F;

	Vector3 vCreaturePos;
	

	while( itor != g_listCollisionCreature.end() )
	{
		pCreature = *itor;

		if( !pCreature )
		{
			itor = g_listCollisionCreature.erase( itor );
			continue;
		} //if

		if( pCreature->CheckPoint( vPos ) )
		{	// 1차적 거리 점검에서 충돌.
			// 방향으로 크리처에 접근하는 놈인지 알아 낸다.

			vCreaturePos = pCreature->GetPosition();
			Vector3	vDir = ( m_vGoalPos - m_vPos ).Normal();
			Vector3 vDirForCreature = ( vCreaturePos - m_vPos ).Normal();

			float fLength = ( vDir - vDirForCreature ).Length();
			if( fLength <= 0.5F ) 
			{
				DEBUG_OUT( "충돌!!!!!!" );
				return TRUE;
			} //if

		} //if

		itor = g_listCollisionCreature.erase( itor );
	} //while

	g_listCollisionCreature.clear();

  */

	return FALSE;
} //CW3DCharacter::CollisionCreature

/**
 * 현재 작업중인 크리쳐를 얻는다.
 */
IW3DCreature* CW3DCharacter::GetTargetCreature()
{
	IW3DCreature*		pTarget = NULL;
	if( m_sTarget.btType == MONSTER )
	{
		pTarget = m_pWorld->GetMonster( m_sTarget.dwId );
	}
	else if( m_sTarget.btType == CHARACTER )
	{
		pTarget = m_pWorld->GetCharacter( m_sTarget.dwId );
	} //if..else

	return pTarget;
} //CW3DCharacter::GetTargetCreature

/**
 *
 */
void CW3DCharacter::AddPickedEffect()
{
	if( !m_bPlayer )
	{
		return ;
	} //if

	if( g_dwPickingEffect )
	{
		RemovePickedEffect();
	} //if

	Quaternion	qRot = Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion();

	g_dwPickingEffect = m_pWorld->AddEffect(
				"pointer.wed",
				m_vLastGoal + Vector3( 0.0F, 0.0F, 0.3F ),
				qRot,
				TRUE );
} //CW3DCharacter::AddPickedEffect

/**
 *
 */
void CW3DCharacter::RemovePickedEffect()
{
	if( !m_bPlayer && !g_dwPickingEffect )
	{
		return ;
	} //if

	m_pWorld->RemoveEffect( g_dwPickingEffect );
	g_dwPickingEffect = 0;
} //CW3DCharacter::RemovePickedEffect

/**
 *
 */
void CW3DCharacter::InsertPet( const BYTE btType, void* pData )
{
	SAFE_DELETE( m_pHelper );
	
	// 타입이 100을 넘어가면, 오히려 삭제로 받아 들인다.
	if( btType >= 100 )
	{
		return ;
	} //if

	CPet*	pPet = new CPet;
	if( !pPet )
	{
		Assert( !"Failed Allocate Memory" );
		return ;
	} //if
	m_pHelper = dynamic_cast<CHelper*>( pPet );
	if( m_bPlayer )
	{
		g_pHelper = m_pHelper;
	} //if
	Assert( m_pHelper );

	pPet->Init( m_pWorld, this );
	pPet->SetSpeed( m_fOffset );
	pPet->SetInfo( btType );
	if( pData )
	{
		pPet->SetData( pData );
	} //if
} //CW3DCharacter::InsertPet

/**
 * Remove Pet
 */
void CW3DCharacter::RemovePet()
{
	SAFE_DELETE( m_pHelper );
	g_pHelper = NULL;
} //CW3DCharacter::RemovePet

/**
 * Update Pet Data
 */
void CW3DCharacter::UpdatePet( const char* szName, void* pData )
{
	if( !m_pHelper )
	{
		Assert( !"Have No the Pet" );
		return;
	} //if
	CPet*	pPet = reinterpret_cast<CPet*>( m_pHelper );
	Assert( pPet );

	if( szName )
	{
		pPet->SetName( szName );
	} //if

	if( pData )
	{
		pPet->SetData( pData );
	} //if
} //CW3DCharacter::UpdatePet

/**
 * 피킹.
 *
 * @return	vOrig		[IN] Picking Ray Origin
 * @return	vDir		[IN] Picking Ray Direction
 *
 * @return	Picking
 */
BOOL CW3DCharacter::Pick( const Vector3& vOrig, const Vector3& vDir )
{
	Assert( m_pBoundBox );

	if( !m_bShow || !m_pBoundBox || ( m_btGhost && !m_bVisible ) )
	{
		return FALSE;
	} //if

	m_pBoundBox->SetWorld( m_matWorld );
	if( m_pBoundBox->Pick( vOrig, vDir ) )
	{
		return TRUE;
	} //if
	return FALSE;
} //CW3DCharacter::Pick

/**
 *
 */
void	ReleaseCreatureBoundBox()
{
	if( gs_mapCharacterModel.empty() )
	{
		BoundBoxMap::iterator itor		= gs_mapBoundBoxModel.begin();
		BoundBoxMap::iterator itorNext;

		while( itor != gs_mapBoundBoxModel.end() )
		{
			itorNext = itor;
			itorNext++;

			IW3DModel* pModel = itor->second;

			if( pModel )
			{
				int nCount = pModel->GetRefCount();
				for( int i = 0 ; i < nCount ; i++ )
				{
					SAFE_RELEASE( pModel );
				} //for
			} //if

			gs_mapBoundBoxModel.erase( itor );
			itor = itorNext;
		} //while
		
		Assert( gs_mapBoundBoxModel.empty() );
	} //if
	
//	Assert( gs_mapCharacterModel.empty() );
} //ReleaseCreatureBoundBox
